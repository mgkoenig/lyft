/*
 * lyft_keypad.c
 *
 * Created: 13/11/2021 13:42:12
 *  Author: Matthias Koenig
 */ 

#include "lyft_keypad.h"


void keypad_init(struct keypad_button *keypad)
{
	uint8_t button;

	for (button=0; button<BUTTON_MAX; button++)
	{
		keypad[button].state = BUTTON_STATE_UNKNOWN;
		keypad[button].enable = true;
		//keypad[button].memory_position = 0;		// already loaded from config file
		keypad[button].trigger_time = 0;
		keypad[button].release_time = 0;
	}
}

uint8_t keypad_read(struct keypad_button *keypad, uint16_t timestamp)
{
	uint8_t						i, num;
	uint8_t						keyscan;
	uint8_t						key_index;
	enum keypad_layout			button;
	enum keypad_button_state	prev_state;
	
	num = 0;
	keyscan = as1115_getKeyscan(AS1115_KEYSCAN_A);
	
	for (i=0; i<8; i++)
	{
		key_index = (1 << i);
		
		switch (key_index)
		{
			case KEYPAD_SCAN_BTN_1: button = BUTTON_1; break;
			case KEYPAD_SCAN_BTN_2: button = BUTTON_2; break;
			case KEYPAD_SCAN_BTN_3: button = BUTTON_3; break;
			case KEYPAD_SCAN_BTN_4: button = BUTTON_4; break;
			case KEYPAD_SCAN_BTN_UP: button = BUTTON_UP; break;
			case KEYPAD_SCAN_BTN_DOWN: button = BUTTON_DOWN; break;
			default: button = BUTTON_MAX;
		}
		
		if (button < BUTTON_MAX)
		{
			prev_state = keypad[button].state;
			
			if (keyscan & key_index) 		// button NOT pressed
			{
				switch (prev_state)
				{
					case BUTTON_STATE_UNKNOWN:
					case BUTTON_STATE_PRESSED:
					case BUTTON_STATE_CONT_PRESS:
						keypad[button].state = BUTTON_STATE_RELEASED;
						keypad[button].release_time = timestamp;
						break;
					
					case BUTTON_STATE_RELEASED:
						break;

					default: keypad[button].state = BUTTON_STATE_UNKNOWN; break;
				}
			}
			else 							// button pressed
			{
				switch (prev_state)
				{
					case BUTTON_STATE_UNKNOWN:
					case BUTTON_STATE_RELEASED:
						keypad[button].state = BUTTON_STATE_PRESSED;
						keypad[button].trigger_time = timestamp;
						break;

					case BUTTON_STATE_PRESSED:
						keypad[button].state = BUTTON_STATE_CONT_PRESS;
						break;
					
					case BUTTON_STATE_CONT_PRESS:
						break;

					default: keypad[button].state = BUTTON_STATE_UNKNOWN; break;
				}

				num++;
			}
		}
	}
	
	return num;
}

uint16_t keypad_holdTime(uint16_t ts_now, uint16_t ts_trigger)
{
	uint16_t elap = 0;
	
	if (ts_trigger > 0) {
		elap = ((ts_now - ts_trigger) * KEYPAD_POLL_INTERVAL);
	}
	
	return elap;
}