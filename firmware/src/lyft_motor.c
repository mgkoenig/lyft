/*
 * lyft_motor.c
 *
 * Created: 19/11/2021 14:01:37
 *  Author: Matthias Koenig
 */ 

#include "lyft_motor.h"


uint16_t			mot_pos_a;
uint16_t			mot_pos_b;
uint16_t			mot_pos_hi;
uint16_t			mot_pos_lo;
uint8_t				mot_state_a;
uint8_t				mot_state_b;
uint8_t				prev_cmd;
uint8_t				braking_distance;
uint8_t				safety_procedure;
uint8_t				safety_command;
enum motor_mode		op_mode;


static void			motor_burst(void);
static void			motor_startup(void);
static void			motor_sendStartupPacket(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3);
static uint8_t		motor_readStartupPacket(void);
static void			motor_sendCommand(uint16_t pos, uint8_t cmd);

static void			motor_move_up(void);
static void			motor_move_down(void);
static void			motor_move_stop(void);
static void			motor_calibrate(void);
static void			motor_safety_protocol(void);


const uint8_t	zero_packet[3] = {0, 0, 0};	
const uint8_t   sfin_packet[3] = {246, 255, 191};		// startup finishing packet
const uint8_t	startup_sequence[STARTUP_SEQUENCE_LENGTH][(STARTUP_PACKET_LENGTH/2)] = {
	{255,   7, 255, 255},       // seq_idx 0 (no resp)
	{255,   7, 255, 255},       // seq_idx 1 (no resp)
	{255,   1,   7, 255},       // seq_idx 2 (no resp)
	{208,   2,   7, 255},       // seq_idx 3
	{0,     2,   7, 255},       // seq_idx 4 LOOP_A until resp > 0
	{0,     6,   9,   0},       // seq_idx 5
	{0,     6,  12,   0},       // seq_idx 6
	{0,     6,  13,   0},       // seq_idx 7
	{0,     6,  10,   0},       // seq_idx 8
	{0,     6,  11,   0},       // seq_idx 9
	{0,     4,   0,   0},       // seq_idx 10
	{0,     2,   0,   0},       // seq_idx 11 LOOP_B until resp > 0
	{0,     6,   9,   0},       // seq_idx 12
	{0,     6,  12,   0},       // seq_idx 13
	{0,     6,  13,   0},       // seq_idx 14
	{0,     6,  10,   0},       // seq_idx 15
	{0,     6,  11,   0},       // seq_idx 16
	{0,     4,   1,   0},       // seq_idx 17
	{0,     2,   1,   0},       // seq_idx 18 LOOP_C while seq < 8 (no resp)
	{208,   1,   7,   0},       // seq_idx 19 (no resp)
	{208,   2,   7,   0}        // seq_idx 20 (no resp)
};


void motor_init(void)
{
	hal_lin_init();
	hal_delay_msec(250);
	motor_startup();
	
	safety_command = MOTOR_CMD_IDLE;
	safety_procedure = MOTOR_SAFETY_PROTOCOL;
	//op_mode = MOTOR_MODE_NORMAL;
	op_mode = MOTOR_MODE_READY;
}

enum motor_mode motor_run(enum motor_event mot_ev)
{	
	motor_burst();
	
	if ((mot_state_a == MOTOR_STATE_BLOCKAGE) || (mot_state_b == MOTOR_STATE_BLOCKAGE))
	{
		if (safety_procedure == MOTOR_SAFETY_PROTOCOL)
		{
			op_mode = MOTOR_MODE_CRITICAL;
			safety_procedure = 0;
			
			if (prev_cmd == MOTOR_CMD_MOVE_UP) {
				safety_command = MOTOR_CMD_MOVE_DOWN;
			} else if (prev_cmd == MOTOR_CMD_MOVE_DOWN) {
				safety_command = MOTOR_CMD_MOVE_UP;
			} else {
				safety_command = MOTOR_CMD_MOVE_STOP;
			}
		}	
	}
	
	if (op_mode == MOTOR_MODE_CRITICAL)
	{
		motor_safety_protocol();
	}
	else if (op_mode == MOTOR_MODE_CALIBRATION) {
		motor_calibrate();
	}
	else 
	{
		switch (mot_ev)
		{
			case MOTOR_EVENT_MOVE_UP: motor_move_up(); op_mode = MOTOR_MODE_MOVE; break;
			case MOTOR_EVENT_MOVE_DOWN: motor_move_down(); op_mode = MOTOR_MODE_MOVE; break;
			case MOTOR_EVENT_STOP: motor_move_stop(); op_mode = MOTOR_MODE_READY; break;
			case MOTOR_EVENT_CALIBRATE: op_mode = MOTOR_MODE_CALIBRATION; break;
			default: motor_move_stop; op_mode = MOTOR_MODE_READY; break;
		}		
	}	
	
	return op_mode;
}

uint8_t motor_getDrift(void)
{
	return (uint8_t) (mot_pos_hi - mot_pos_lo);
}

uint16_t motor_getPosition(void)
{
	return ((mot_pos_a + mot_pos_b) / 2);
}

uint16_t motor_getHeightInch(uint8_t offset)
{
	uint16_t pos, height;
	
	pos = ((mot_pos_a + mot_pos_b) / 2);
	height = ((pos / MOTOR_STEP_SIZE) + MOTOR_BASE);
	height = (height + offset - MOTOR_OFFSET_RANGE);
	height = ((height / 2.54) * 10);
	
	return height;
}

uint16_t motor_getHeightCentimeter(uint8_t offset)
{
	uint16_t pos, height;
	
	pos = ((mot_pos_a + mot_pos_b) / 2);
	height = ((pos / MOTOR_STEP_SIZE) + MOTOR_BASE);
	height = (height + offset - MOTOR_OFFSET_RANGE);
	
	return height;
}


static void motor_burst(void)
{
	uint8_t			i;
	static uint8_t	rx_data[MOTOR_PACKET_LEN_RX];
	
	hal_lin_write(NODE_SEVENTEEN, zero_packet, ZERO_PACKET_LEN);
	hal_delay_msec(MOTOR_DELAY_RX);
		
	hal_lin_read(NODE_MOTOR_A, rx_data, MOTOR_PACKET_LEN_RX);
	mot_pos_a = ((rx_data[3] << 8) | ( rx_data[2]));
	mot_state_a = rx_data[4];
	hal_delay_msec(MOTOR_DELAY_RX);
		
	hal_lin_read(NODE_MOTOR_B, rx_data, MOTOR_PACKET_LEN_RX);
	mot_pos_b = ((rx_data[3] << 8) | ( rx_data[2]));
	mot_state_b = rx_data[4];
	hal_delay_msec(MOTOR_DELAY_RX);
		
	for (i=0; i<6; i++)
	{
		hal_lin_read(NODE_SIXTEEN, NULL, 0);
		hal_delay_msec(MOTOR_DELAY_RX);
	}
		
	hal_lin_read(NODE_ONE, NULL, 0);
	hal_delay_msec(MOTOR_DELAY_RX);
	
	if (mot_pos_a != mot_pos_b) {
		mot_pos_lo = MIN(mot_pos_a, mot_pos_b);
		mot_pos_hi = MAX(mot_pos_a, mot_pos_b);
	} else {
		mot_pos_hi = mot_pos_a;
		mot_pos_lo = mot_pos_a;
	}
}

static void motor_move_up(void)
{
	uint8_t			cmd = MOTOR_CMD_MOVE_STOP;
	uint16_t		pos = mot_pos_a;
	
	if (mot_pos_hi < MOTOR_POSITION_MAX)
	{		
		if ((mot_state_a == MOTOR_STATE_IDLE) && (mot_state_b == MOTOR_STATE_IDLE))
		{
			if (prev_cmd == MOTOR_CMD_IDLE) 
			{
				pos = mot_pos_a;
				cmd = MOTOR_CMD_MOVE_START;
			}
			else if (prev_cmd == MOTOR_CMD_MOVE_START)
			{
				pos = mot_pos_lo;
				cmd = MOTOR_CMD_MOVE_UP;
			}
		}
		else if ((mot_state_a == MOTOR_STATE_MOVING) && (mot_state_b == MOTOR_STATE_MOVING))
		{
			if (prev_cmd == MOTOR_CMD_MOVE_UP)
			{
				pos = mot_pos_lo;
				cmd = MOTOR_CMD_MOVE_UP;
			}
		}
		
		braking_distance = 0;
		motor_sendCommand(pos, cmd);
	}
	else 
	{
		motor_move_stop();
	}
}

static void motor_move_down(void)
{
	uint8_t			cmd = MOTOR_CMD_MOVE_STOP;
	uint16_t		pos = mot_pos_a;
	
	if (mot_pos_lo > MOTOR_POSITION_MIN)
	{
		if ((mot_state_a == MOTOR_STATE_IDLE) && (mot_state_b == MOTOR_STATE_IDLE))
		{
			if (prev_cmd == MOTOR_CMD_IDLE)
			{
				pos = mot_pos_a;
				cmd = MOTOR_CMD_MOVE_START;
			}
			else if (prev_cmd == MOTOR_CMD_MOVE_START)
			{
				pos = mot_pos_hi;
				cmd = MOTOR_CMD_MOVE_DOWN;
			}
		}
		else if ((mot_state_a == MOTOR_STATE_MOVING) && (mot_state_b == MOTOR_STATE_MOVING))
		{
			if (prev_cmd == MOTOR_CMD_MOVE_DOWN)
			{
				pos = mot_pos_hi;
				cmd = MOTOR_CMD_MOVE_DOWN;
			}
		}
		
		braking_distance = 0;
		motor_sendCommand(pos, cmd);
	}
	else
	{
		motor_move_stop();
	}
}

static void motor_move_stop(void)
{
	uint8_t			cmd = MOTOR_CMD_MOVE_STOP;
	uint16_t		pos = mot_pos_a;
	
	if ((mot_state_a == MOTOR_STATE_MOVING) && (mot_state_b == MOTOR_STATE_MOVING))
	{
		pos = mot_pos_a;
		cmd = MOTOR_CMD_MOVE_SLOW;
	}
	else if ((mot_state_a == MOTOR_STATE_MOVING_SLOW) && (mot_state_b == MOTOR_STATE_MOVING_SLOW))
	{
		braking_distance++;	
		pos = mot_pos_a;
		
		if (braking_distance < MOTOR_BRAKING_DISTANCE) {
			cmd = MOTOR_CMD_MOVE_SLOW;
		} else {
			cmd = MOTOR_CMD_MOVE_STOP;
		}
	}
	else if ((mot_state_a == MOTOR_STATE_LIMIT_A) && (mot_state_b == MOTOR_STATE_LIMIT_A))
	{
		pos = 0xFFF6;
		cmd = 0xBF;
	}
	else if ((mot_state_a == MOTOR_STATE_LIMIT_B) && (mot_state_b == MOTOR_STATE_LIMIT_B))
	{
		pos = 0xFFF6;
		cmd = 0xFF;
	}	
	else	// ((mot_state_a == MOTOR_STATE_IDLE) && (mot_state_b == MOTOR_STATE_IDLE))
	{
		pos = mot_pos_a;
		cmd = MOTOR_CMD_IDLE;
	}
	
	motor_sendCommand(pos, cmd);
}

static void motor_calibrate(void)
{
	uint8_t			cmd = MOTOR_CMD_IDLE;
	uint16_t		pos = mot_pos_a;
	
	if ((mot_state_a == MOTOR_STATE_IDLE) && (mot_state_b == MOTOR_STATE_IDLE))
	{
		if (prev_cmd == MOTOR_CMD_IDLE)
		{
			pos = mot_pos_a;
			cmd = MOTOR_CMD_CALIBRATION_START;
		}
		else if (prev_cmd == MOTOR_CMD_CALIBRATION_START)
		{
			pos = 0;
			cmd = MOTOR_CMD_CALIBRATION;
		}
	}
	
	else if ((mot_state_a == MOTOR_STATE_CALIBRATING) && (mot_state_b == MOTOR_STATE_CALIBRATING))
	{
		pos = 0;
		cmd = MOTOR_CMD_CALIBRATION;
	}
	
	else if ((mot_state_a == MOTOR_STATE_CALIBRATED) && (mot_state_b == MOTOR_STATE_CALIBRATED))
	{
		pos = mot_pos_hi;
		cmd = MOTOR_CMD_CALIBRATION_STOP;
		op_mode = MOTOR_MODE_READY;
	}
	
	motor_sendCommand(pos, cmd);	
}

static void	motor_safety_protocol(void)
{
	uint8_t			cmd = MOTOR_CMD_IDLE;
	uint16_t		pos = mot_pos_a;
	
	safety_procedure++;
	
	if (safety_procedure < 4)
	{
		pos = mot_pos_a;
		cmd = MOTOR_CMD_MOVE_STOP;
	}
	
	else if ((4 <= safety_procedure) && (safety_procedure < 12))
	{
		switch (safety_command)
		{
			case MOTOR_CMD_MOVE_UP: pos = mot_pos_lo; break;
			case MOTOR_CMD_MOVE_DOWN: pos = mot_pos_hi; break;
			default: pos = mot_pos_a;
		}
		
		cmd = safety_command;
	}
	
	else if ((12 <= safety_procedure) && (safety_procedure < 16))
	{
		pos = mot_pos_a;
		cmd = MOTOR_CMD_MOVE_SLOW;
	}
	
	else if ((16 <= safety_procedure) && (safety_procedure < 19))
	{
		pos = mot_pos_a;
		cmd = MOTOR_CMD_MOVE_STOP;
	}
	
	else if (safety_procedure == 19)
	{
		pos = mot_pos_a;
		cmd = MOTOR_CMD_MOVE_START;
	}
	
	else if (safety_procedure > 19)
	{
		pos = mot_pos_a;
		cmd = MOTOR_CMD_IDLE;
	}
	
	if (safety_procedure == MOTOR_SAFETY_PROTOCOL)
	{
		op_mode = MOTOR_MODE_READY;
	}	
	
	motor_sendCommand(pos, cmd);
}

static void motor_startup(void)
{
	uint8_t			i;
	uint8_t         seq_idx;
	uint8_t         lap_cnt;
	uint8_t			rx_count;
	static uint8_t	rx_data[MOTOR_PACKET_LEN_RX];
	
	lap_cnt = 0;

	for (seq_idx=0; seq_idx<STARTUP_SEQUENCE_LENGTH; seq_idx++)
	{
		if ((seq_idx == STARTUP_SEQUENCE_LOOP_IDX_A) || (seq_idx == STARTUP_SEQUENCE_LOOP_IDX_B))
		{
			while (lap_cnt < STARTUP_SEQUENCE_ITERATION)
			{
				hal_delay_msec(MOTOR_DELAY_TX);
				motor_sendStartupPacket(lap_cnt, startup_sequence[seq_idx][1], startup_sequence[seq_idx][2], startup_sequence[seq_idx][3]);
				hal_delay_msec(MOTOR_DELAY_RX);
				rx_count = motor_readStartupPacket();

				if (rx_count > 0) {
					break;
				} else {
					lap_cnt++;
				}
			}
		}
		else if (seq_idx == STARTUP_SEQUENCE_LOOP_IDX_C)
		{
			while (lap_cnt < STARTUP_SEQUENCE_ITERATION)
			{
				hal_delay_msec(MOTOR_DELAY_TX);
				motor_sendStartupPacket(lap_cnt, startup_sequence[seq_idx][1], startup_sequence[seq_idx][2], startup_sequence[seq_idx][3]);
				hal_delay_msec(MOTOR_DELAY_RX);
				motor_readStartupPacket();
				lap_cnt++;
			}
		}
		else
		{
			hal_delay_msec(MOTOR_DELAY_TX);
			if (startup_sequence[seq_idx][0]) {
				motor_sendStartupPacket(startup_sequence[seq_idx][0], startup_sequence[seq_idx][1], startup_sequence[seq_idx][2], startup_sequence[seq_idx][3]);
			} else {
				motor_sendStartupPacket(lap_cnt, startup_sequence[seq_idx][1], startup_sequence[seq_idx][2], startup_sequence[seq_idx][3]);
			}
			
			hal_delay_msec(MOTOR_DELAY_RX);
			motor_readStartupPacket();
		}
	}
	
	hal_delay_msec(15);
	hal_lin_write(NODE_SEVENTEEN, zero_packet, ZERO_PACKET_LEN);
	hal_delay_msec(MOTOR_DELAY_RX);
	
	hal_lin_read(NODE_MOTOR_A, rx_data, MOTOR_PACKET_LEN_RX);
	mot_pos_a = ((rx_data[2] << 8) | ( rx_data[3]));
	hal_delay_msec(MOTOR_DELAY_RX);
	
	hal_lin_read(NODE_MOTOR_B, rx_data, MOTOR_PACKET_LEN_RX);
	mot_pos_b = ((rx_data[2] << 8) | ( rx_data[3]));
	hal_delay_msec(MOTOR_DELAY_RX);
	
	for (i=0; i<6; i++)
	{
		hal_lin_read(NODE_SIXTEEN, NULL, 0);
		hal_delay_msec(MOTOR_DELAY_RX);
	}
	
	hal_lin_read(NODE_ONE, NULL, 0);
	hal_delay_msec(15);
	
	if (mot_pos_a != mot_pos_b) {
		mot_pos_lo = MIN(mot_pos_a, mot_pos_b);
		mot_pos_hi = MAX(mot_pos_a, mot_pos_b);
	} else {
		mot_pos_hi = mot_pos_a;
		mot_pos_lo = mot_pos_a;
	}
	
	hal_lin_write(NODE_MASTER, sfin_packet, FINISH_PACKET_LEN);
	hal_delay_msec(MOTOR_DELAY_RX);
}

static void motor_sendStartupPacket(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3)
{
	uint8_t startup_packet[STARTUP_PACKET_LENGTH] = {0, 0, 0, 0, 255, 255, 255, 255};

	startup_packet[0] = byte0;
	startup_packet[1] = byte1;
	startup_packet[2] = byte2;
	startup_packet[3] = byte3;

	hal_lin_write(LIN_NODE_DIAG_TX, startup_packet, STARTUP_PACKET_LENGTH); 
}

static uint8_t motor_readStartupPacket(void)
{
	uint8_t num;
	uint8_t	rx_buf[STARTUP_PACKET_LENGTH];
	
	num = hal_lin_read(LIN_NODE_DIAG_RX, rx_buf, STARTUP_PACKET_LENGTH);
	
	if (num >= 2) {			// ignore echo bytes (sync and PID)
		num -= 2;
	} else {
		num = 0;
	}

	return num;
}

static void motor_sendCommand(uint16_t pos, uint8_t cmd)
{
	uint8_t		tx_data[MOTOR_PACKET_LEN_CMD];
	
	prev_cmd = cmd;
	tx_data[0] = (pos & 0x00FF);
	tx_data[1] = ((pos & 0xFF00) >> 8);
	tx_data[2] = cmd;
	
	hal_lin_write(NODE_MASTER, tx_data, MOTOR_PACKET_LEN_CMD);
}