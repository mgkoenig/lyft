/*
 * lyft_keypad.h
 *
 * Created: 13/11/2021 13:42:23
 *  Author: Matthias Koenig
 */ 


#ifndef LYFT_KEYPAD_H_
#define LYFT_KEYPAD_H_

#include <asf.h>
#include "as1115.h"


#define KEYPAD_SCAN_BTN_1				AS1115_SEG_A	//AS1115_SEG_F
#define KEYPAD_SCAN_BTN_2				AS1115_SEG_F	//AS1115_SEG_A
#define KEYPAD_SCAN_BTN_3				AS1115_SEG_C
#define KEYPAD_SCAN_BTN_4				AS1115_SEG_E
#define KEYPAD_SCAN_BTN_UP				AS1115_SEG_DP
#define KEYPAD_SCAN_BTN_DOWN			AS1115_SEG_D

#define KEYPAD_POLL_INTERVAL			25		// polling keypad every 25ms

#define KEYPAD_POLL_DELAY_NONE			0
#define KEYPAD_POLL_DELAY_SINGLE		1
#define KEYPAD_POLL_DELAY_SHORT			5
#define KEYPAD_POLL_DELAY_DEFAULT		10
#define KEYPAD_POLL_DELAY_LONG			20


enum keypad_layout {
	BUTTON_1,
	BUTTON_2,
	BUTTON_3,
	BUTTON_4,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_MAX
};

enum keypad_button_state {
	BUTTON_STATE_PRESSED,
	BUTTON_STATE_RELEASED,
	BUTTON_STATE_CONT_PRESS,
	//BUTTON_STATE_LONG_PRESS,
	//BUTTON_STATE_DOUBLE_PRESS,
	BUTTON_STATE_UNKNOWN
};

struct keypad_button {
	enum keypad_button_state		state;
	bool							enable;
	uint16_t						memory_position;
	uint16_t						trigger_time;	
	uint16_t						release_time;
};


void		keypad_init(struct keypad_button *keypad);
uint8_t		keypad_read(struct keypad_button *keypad, uint16_t timestamp);
uint16_t	keypad_holdTime(uint16_t ts_now, uint16_t ts_trigger);


#endif /* LYFT_KEYPAD_H_ */