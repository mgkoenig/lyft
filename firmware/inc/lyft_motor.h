/*
 * lyft_motor.h
 *
 * Created: 19/11/2021 14:01:47
 *  Author: Matthias Koenig
 */ 


#ifndef LYFT_MOTOR_H_
#define LYFT_MOTOR_H_

#include <asf.h>
#include "hal_lin.h"
#include "hal_delay.h"
#include "lyft_util.h"


#define NODE_ONE							1
#define NODE_MOTOR_A						8
#define NODE_MOTOR_B						9
#define NODE_SIXTEEN						16
#define NODE_SEVENTEEN						17
#define NODE_MASTER							18

#define STARTUP_PACKET_LENGTH				8
#define STARTUP_SEQUENCE_LENGTH				21

#define STARTUP_SEQUENCE_LOOP_IDX_A			4
#define STARTUP_SEQUENCE_LOOP_IDX_B			11
#define STARTUP_SEQUENCE_LOOP_IDX_C			18
#define STARTUP_SEQUENCE_ITERATION			8

#define MOTOR_DELAY_TX						12		// 12ms delay before writing new data
#define MOTOR_DELAY_RX						5		// 5ms delay before receiving data

#define MOTOR_PACKET_LEN_RX					6
#define MOTOR_PACKET_LEN_CMD				3
#define ZERO_PACKET_LEN						3
#define FINISH_PACKET_LEN					3

#define MOTOR_CMD_IDLE						0xFC		// 252
#define MOTOR_CMD_MOVE_START				0xC4		// 196 - premove
#define MOTOR_CMD_MOVE_UP					0x86		// 134 - raise
#define MOTOR_CMD_MOVE_DOWN					0x85		// 133 - lower
#define MOTOR_CMD_MOVE_SLOW					0x87		// 135 - fine
#define MOTOR_CMD_MOVE_STOP					0x84		// 132 - finish
#define MOTOR_CMD_LIMIT_START				0x80
#define MOTOR_CMD_LIMIT_HIGH				0x82
#define MOTOR_CMD_LIMIT_LOW					0x81

#define MOTOR_CMD_CALIBRATION_START			0xC4		// 196
#define MOTOR_CMD_CALIBRATION				0xBD		// 189
#define MOTOR_CMD_CALIBRATION_STOP			0xBC		// 188

#define MOTOR_STATE_IDLE					0x00		// 0
#define MOTOR_STATE_CALIBRATED				0x01		// 1
#define MOTOR_STATE_MOVING					0x02		// 2
#define MOTOR_STATE_MOVING_SLOW				0x03		// 3
#define MOTOR_STATE_CALIBRATING				0x04
#define MOTOR_STATE_LIMIT_A					0xA5
#define MOTOR_STATE_LIMIT_B					0x25
#define MOTOR_STATE_BLOCKAGE				0x62
#define MOTOR_STATE_ERROR					0x65

#define MOTOR_SAFETY_PROTOCOL				23			// 23 steps for the safety protocol to follow
#define MOTOR_SAFETY_MARGIN					137			// do not move if height difference is less than 137
#define MOTOR_POSITION_MIN					(162 + MOTOR_SAFETY_MARGIN)
#define MOTOR_POSITION_MAX					(6537 - MOTOR_SAFETY_MARGIN)
//#define MOTOR_POSITION_MAX				(6777 - MOTOR_SAFETY_MARGIN)

#define MOTOR_SERVICE_INTERVAL				50		// run motor service every 50ms
#define MOTOR_BRAKING_DISTANCE				5		// move slow for 5 iterations until full stop

#define MOTOR_STEP_SIZE						100		// 100 motor steps per centimeter 
#define MOTOR_BASE							60		// lowest desk position in centimeter

#define MOTOR_OFFSET_RANGE					9		// fine adjust of desk height by plus/minus 9cm


enum motor_mode {
	MOTOR_MODE_NORMAL,
	MOTOR_MODE_CALIBRATION,
	MOTOR_MODE_CRITICAL	
};

enum motor_event {
	//MOTOR_EVENT_IDLE,
	MOTOR_EVENT_STOP,
	MOTOR_EVENT_MOVE_UP,
	MOTOR_EVENT_MOVE_DOWN,
	MOTOR_EVENT_CALIBRATE	
};


void				motor_init(void);
enum motor_mode		motor_run(enum motor_event mot_ev);
uint8_t				motor_getDrift(void);
uint16_t			motor_getPosition(void);
uint16_t			motor_getHeightInch(uint8_t offset);
uint16_t			motor_getHeightCentimeter(uint8_t offset);


#endif /* LYFT_MOTOR_H_ */