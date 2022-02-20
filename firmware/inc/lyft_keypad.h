/**
 * @file lyft_keypad.h
 * @author Matthias Koenig (mgkoenig) 
 * @date 13 Nov 2021 
 * @brief LYFT Keypad Driver
 *
 * @details LYFT uses an <a href="https://ams.com/en/as1115 ">AS1115 display driver</a> 
 * which also provides a key readback function. This keyscan feature is used to 
 * detect any key presses. 
 *
 * This file contains all functions and defines related to the keyscan mode 
 * of the <a href="https://ams.com/en/as1115 ">AS1115 driver</a>. 
 */

#ifndef LYFT_KEYPAD_H_
#define LYFT_KEYPAD_H_


#include <asf.h>
#include "as1115.h"


#define KEYPAD_SCAN_BTN_1				AS1115_SEG_A		/**< Keypad button 1 is connected to segment A of the AS1115 driver */
#define KEYPAD_SCAN_BTN_2				AS1115_SEG_F		/**< Keypad button 2 is connected to segment F of the AS1115 driver */
#define KEYPAD_SCAN_BTN_3				AS1115_SEG_C		/**< Keypad button 3 is connected to segment C of the AS1115 driver */
#define KEYPAD_SCAN_BTN_4				AS1115_SEG_E		/**< Keypad button 4 is connected to segment E of the AS1115 driver */
#define KEYPAD_SCAN_BTN_UP				AS1115_SEG_DP		/**< Keypad button UP is connected to segment DP of the AS1115 driver */
#define KEYPAD_SCAN_BTN_DOWN			AS1115_SEG_D		/**< Keypad button DOWN is connected to segment D of the AS1115 driver */

#define KEYPAD_POLL_INTERVAL			25					/**< Keypad timer period. Polling the keypad every 25ms. */

#define KEYPAD_POLL_DELAY_NONE			0					/**< No delay of the keypad polling */
#define KEYPAD_POLL_DELAY_SINGLE		1					/**< Delay keypad polling for one cycle (25ms) */
#define KEYPAD_POLL_DELAY_SHORT			5					/**< Delay keypad polling for five cycles (125ms) */
#define KEYPAD_POLL_DELAY_DEFAULT		10					/**< Delay keypad polling for ten cycles (250ms) */
#define KEYPAD_POLL_DELAY_LONG			20					/**< Delay keypad polling for 20 cycles (500ms) */


/** 
 * @enum keypad_layout 
 * The keypad on the HMI board consists of six buttons: 
 * Memory button (1, 2, 3, 4), button (up) and button (down).
 */
enum keypad_layout {
	BUTTON_1,				/**< Memory Button 1 */
	BUTTON_2,				/**< Memory Button 2 */
	BUTTON_3,				/**< Memory Button 3 */
	BUTTON_4,				/**< Memory Button 4 */
	BUTTON_UP,				/**< Button UP */
	BUTTON_DOWN,			/**< Button DOWN */
	BUTTON_MAX
};

/** 
 * @enum keypad_button_state 
 * Depending on its history, a button can have different states. 
 */
enum keypad_button_state {
	BUTTON_STATE_PRESSED,			/**< A button press was triggered. */
	BUTTON_STATE_RELEASED,			/**< A button was pressed and is released now. */
	BUTTON_STATE_CONT_PRESS,		/**< A button press was detected previously and it's still pressed. */
	//BUTTON_STATE_LONG_PRESS,
	//BUTTON_STATE_DOUBLE_PRESS,
	BUTTON_STATE_UNKNOWN			/**< Initial state. We do not know whether a button is pressed or not. */
};

/** 
 * @struct keypad_button
 * A single button is described with different characteristics.  
 */
struct keypad_button {
	enum keypad_button_state		state;					/**< The currebt state of the button, eg. #BUTTON_STATE_PRESSED. */
	bool							enable;					/**< Unused feature to (temporarily) disable a single button. */
	uint16_t						memory_position;		/**< A desk height which was saved to a certain button. Only used for memory buttons (1 to 4). */
	uint16_t						trigger_time;			/**< Timestamp when a button was triggered. This information is important to determine if a button is pressed for certain amount of time, eg. to enter the settings menu. */
	uint16_t						release_time;			/**< Timestamp when a button was released. */
};


/** 
 * @brief Keypad initialization 
 *
 * Initializes each button of the keypad with default values.
 *
 * @param[in] keypad A button array which contains all buttons of the keypad
 */
void keypad_init(struct keypad_button *keypad);

/** 
 * @brief Read the current button states
 *
 * Reads the state of the keypad, which buttons are pressed or released. The 
 * current states will be set within the keypad array for each button separately. 
 * The timestamp is just relative time information related to the polling intervals. 
 *
 * @param[in] keypad The button array which contains all buttons of the keypad
 * @param[in] timestamp The current time (which polling interval) of the system 
 */
uint8_t keypad_read(struct keypad_button *keypad, uint16_t timestamp);

/** 
 * @brief Continuously holding time of a button
 *
 * This function computes the holding time of a button, how long a button is continuously pressed. 
 *
 * @param[in] ts_now The current timestamp (polling interval) of the system
 * @param[in] ts_trigger The timestamp when the button was triggered 
 *
 * @return Returns the time how long a button is continuously pressed in milliseconds
 */
uint16_t keypad_holdTime(uint16_t ts_now, uint16_t ts_trigger);


#endif /* LYFT_KEYPAD_H_ */