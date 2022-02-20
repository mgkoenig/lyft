/**
 * @file lyft_motor.h
 * @author Matthias Koenig (mgkoenig) 
 * @date 19 Nov 2021 
 * @brief LYFT Motor Driver
 *
 * @details The motor driver contains all node IDs which are used 
 * on the LIN bus as well as all commands and states of the motor 
 * control. 
 *
 * Some naming convention:
 * - Step value: the motors specify their current position with 
 * numerical values between 162 and 6537. These values do not represent
 * a certain height in centimetrs rather than a stepper position. 
 * - Position: the desk position equals the numerical value of the 
 * the stepper motors. It's the current height of the desk represented 
 * as 'step value'. 
 * - Height: the desk height is the current height of the desk represented 
 * as inch or centimeter value. This value gives the user a concrete 
 * idea of the desk's position. The desk height is directly connected to 
 * the desk position. 
 */

#ifndef LYFT_MOTOR_H_
#define LYFT_MOTOR_H_


#include <asf.h>
#include "hal_lin.h"
#include "hal_delay.h"
#include "lyft_util.h"


#define NODE_ONE							1			/**< LIN node ID 1 */
#define NODE_MOTOR_A						8			/**< LIN node ID 8 (Motor A) */
#define NODE_MOTOR_B						9			/**< LIN node ID 9 (Motor B) */
#define NODE_SIXTEEN						16			/**< LIN node ID 16 */
#define NODE_SEVENTEEN						17			/**< LIN node ID 17 */
#define NODE_MASTER							18			/**< LIN node ID 18 (Master) */

#define STARTUP_PACKET_LENGTH				8			/**< Each data packet of the startup sequence has a length of 8 bytes */
#define STARTUP_SEQUENCE_LENGTH				21			/**< The startup sequence consists of 21 data packets */

#define STARTUP_SEQUENCE_LOOP_IDX_A			4			/**< The startup packet #4 will be re-transmitted */
#define STARTUP_SEQUENCE_LOOP_IDX_B			11			/**< The startup packet #11 will be re-transmitted */
#define STARTUP_SEQUENCE_LOOP_IDX_C			18			/**< The startup packet #18 will be re-transmitted */
#define STARTUP_SEQUENCE_ITERATION			8			/**< The looped packets will be re-transmitted 8 times */

#define MOTOR_DELAY_TX						12			/**< When a packet was sent wait for 12ms before writing new data */
#define MOTOR_DELAY_RX						5			/**< When a packet was received wait for 5ms before receiving new data */

#define MOTOR_PACKET_LEN_RX					6			/**< A data packet received from a motor is always 6 bytes in length */
#define MOTOR_PACKET_LEN_CMD				3			/**< A motor command packet contains always 3 bytes of data (2 bytes position, 1 byte command) */
#define ZERO_PACKET_LEN						3			/**< A zero packet consists of three zero bytes (0x00, 0x00, 0x00)  */
#define FINISH_PACKET_LEN					3			/**< The data packet to finalize the startup sequence consists of three bytes */

#define MOTOR_CMD_IDLE						0xFC		/**< Motor command idle */
#define MOTOR_CMD_MOVE_START				0xC4		/**< Motor command to announce a moving command */
#define MOTOR_CMD_MOVE_UP					0x86		/**< Motor command move up */
#define MOTOR_CMD_MOVE_DOWN					0x85		/**< Motor command move down */
#define MOTOR_CMD_MOVE_SLOW					0x87		/**< Motor command move slow */
#define MOTOR_CMD_MOVE_STOP					0x84		/**< Motor command stop */
#define MOTOR_CMD_LIMIT_START				0x80		/**< Motor command when trying to go above/beneath the limit */
#define MOTOR_CMD_LIMIT_HIGH				0x82		/**< Motor command when upper limit is reached */
#define MOTOR_CMD_LIMIT_LOW					0x81		/**< Motor command when lower limit is reached */

#define MOTOR_CMD_CALIBRATION_START			0xC4		/**< Motor command to announce a calibration */
#define MOTOR_CMD_CALIBRATION				0xBD		/**< Motor command calibrate */
#define MOTOR_CMD_CALIBRATION_STOP			0xBC		/**< Motor command to finish calibration */

#define MOTOR_STATE_IDLE					0x00		/**< Motor state idle */
#define MOTOR_STATE_CALIBRATED				0x01		/**< Motor state calibrated (finished) */
#define MOTOR_STATE_MOVING					0x02		/**< Motor state moving */
#define MOTOR_STATE_MOVING_SLOW				0x03		/**< Motor state moving slow */
#define MOTOR_STATE_CALIBRATING				0x04		/**< Motor state calibrating (ongoing) */
#define MOTOR_STATE_LIMIT_A					0xA5		/**< Motor state unknown (measured on LIN bus) */
#define MOTOR_STATE_LIMIT_B					0x25		/**< Motor state unknown (measured on LIN bus) */
#define MOTOR_STATE_BLOCKAGE				0x62		/**< Motor state blocked (error state) */
#define MOTOR_STATE_ERROR					0x65		/**< Motor state general error condition */

#define MOTOR_SAFETY_PROTOCOL				23			/**< The safety protocol consists of 23 steps */
#define MOTOR_SAFETY_MARGIN					137			/**< No motor movement if position difference is less than 137 'step values' */
#define MOTOR_POSITION_MIN					(162 + MOTOR_SAFETY_MARGIN)		/**< Lowest desk position (measured from original controller) */
#define MOTOR_POSITION_MAX					(6537 - MOTOR_SAFETY_MARGIN)	/**< Highest desk position (measured from original controller) */
//#define MOTOR_POSITION_MAX				(6777 - MOTOR_SAFETY_MARGIN)	

#define MOTOR_SERVICE_INTERVAL				50			/**< Run motor burst every 50ms */
#define MOTOR_BRAKING_DISTANCE				5			/**< Before stopping the motors move slow for 5 iterations (measured from original controller) */

#define MOTOR_STEP_SIZE						100			/**< 100 motor steps per centimeter */
#define MOTOR_BASE							60			/**< Lowest desk position in centimeter */

#define MOTOR_OFFSET_RANGE					9			/**< Fine adjust of desk height by plus/minus 9cm for the displayed value */


/** 
 * @enum motor_mode 
 * The motors might be busy in different operation modes. 
 */
enum motor_mode {
	MOTOR_MODE_NORMAL,			/**< Motors are ready and operating normal */
	MOTOR_MODE_CALIBRATION,		/**< Motors are currently calibrating */
	MOTOR_MODE_CRITICAL			/**< Motors got stuck and are therefore in a critical condition */
};

/** 
 * @enum motor_event 
 * The motors might be triggered for some action with different events. 
 */
enum motor_event {
	//MOTOR_EVENT_IDLE,
	MOTOR_EVENT_STOP,			/**< Stop moving motors */
	MOTOR_EVENT_MOVE_UP,		/**< Move motors up */
	MOTOR_EVENT_MOVE_DOWN,		/**< Move motors down */
	MOTOR_EVENT_CALIBRATE		/**< Calibrate both motors (to reduce drift) */
};


/** 
 * @brief Motor initialization 
 *
 * Initializes the LIN module and run the startup procedure
 * to get both motors ready. 
 */
void motor_init(void);

/** 
 * @brief Periodic motor service routine
 *
 * This routine is called periodically to retrieve motor states, positions and 
 * to trigger motor actions like moving or stopping. 
 *
 * @param[in] mot_ev An event to be triggered like #MOTOR_EVENT_MOVE_UP
 *
 * @return Returns the current motor mode like #MOTOR_MODE_NORMAL, #MOTOR_MODE_CALIBRATION or #MOTOR_MODE_CRITICAL
 */
enum motor_mode motor_run(enum motor_event mot_ev);

/** 
 * @brief Get the motor drift 
 *
 * This functions returns the absolute drift of both motor positions to 
 * each other. The smaller the value the better. 
 *
 * @return The absolute drift value as 'step value'
 */
uint8_t motor_getDrift(void);

/** 
 * @brief Get the current motor position
 *
 * This routine returns the current motor position as 'step value'. 
 * For example, the current position is 2575. This value is not
 * related to any desk height in centimeters - it's just the 
 * current step position of the motor. The lowest position is 
 * #MOTOR_POSITION_MIN and the highest position is #MOTOR_POSITION_MAX. 
 *
 * @return The current motor position as 'step value'
 */
uint16_t motor_getPosition(void);

/** 
 * @brief Get the desk height in inch
 *
 * This function computes the current height of the desk in deca-inches. 
 * The formula to compute the desk height follows a classical linear equation 
 * related to the current desk position and a base height. To simplify the 
 * computation the height is first calculated in centimeters and converted to 
 * inch afterwards. 
 * 
 * @f$ y_{[cm]} = mx + y_0 @f$, where  
 * - @f$y@f$ equals the desk height in centimeters
 * - @f$m@f$ equals the step size of the motors per centimeter (100 steps per centimeter)
 * - @f$x@f$ equals the current motor position as 'step value'
 * - @f$y_0@f$ equals the base height of the desk in centimeters (60cm)
 *
 * This leads to the final equation: 
 *
 * @f$ y_{[cm]} = \frac{1}{100} * x + 60 + offset @f$ 
 *
 * and finally 
 *
 * @f$ y_{[in]} = \frac{y_{[cm]}}{2,54} @f$ 
 *
 * For example, if the current motor position equals 2575 and the user 
 * has set an offset of +2cm we get: 
 *
 * @f$ y_{[cm]} = \frac{1}{100} * 2575 + 60 + 2 = 87,75cm @f$ 
 *
 * @f$ y_{[in]} = \frac{y_{[cm]}}{2,54} = \frac{87,75}{2,54} = 34,5 inch @f$ 
 *
 * Since the final result is provided as deca-inch, the return value from the 
 * result above would be 34,5 inch = 345 deca-inch. This value will be displayed 
 * with a dot point before the last digit. 
 * 
 * @param[in] offset An offset value (between plus/minus 9 centimeters) which is set by the user
 *
 * @return The current desk height as deca-inch value
 */
uint16_t motor_getHeightInch(uint8_t offset);

/** 
 * @brief Get the desk height in centimeters
 *
 * This function computes the current height of the desk in centimeters. 
 * The formula to compute the desk height follows a classical linear equation 
 * related to the current desk position and a base height. 
 * 
 * @f$ y_{[cm]} = mx + y_0 @f$, where  
 * - @f$y@f$ equals the desk height in centimeters
 * - @f$m@f$ equals the step size of the motors per centimeter (100 steps per cm)
 * - @f$x@f$ equals the current motor position as 'step value'
 * - @f$y_0@f$ equals the base height of the desk in centimeters (60cm)
 *
 * This leads to the final equation: 
 *
 * @f$ y_{[cm]} = \frac{1}{100} * x + 60 + offset @f$ 
 *
 * For example, if the current motor position equals 2575 and the user 
 * has set an offset of +2cm we get: 
 *
 * @f$ y_{[cm]} = \frac{1}{100} * 2575 + 60 + 2 = 87,75cm @f$ 
 *
 * Since no position after the decimal point is shown on the display, the value 
 * to be displayed would be 87cm. 
 * 
 * @param[in] offset An offset value (between plus/minus 9 centimeters) which is set by the user
 *
 * @return The current desk height as centimeter value
 */
uint16_t motor_getHeightCentimeter(uint8_t offset);


#endif /* LYFT_MOTOR_H_ */