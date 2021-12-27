/*
 * lyft_display.h
 *
 * Created: 13/11/2021 12:39:45
 *  Author: Matthias Koenig
 */ 


#ifndef LYFT_DISPLAY_H_
#define LYFT_DISPLAY_H_

#include "as1115.h"
#include "hal_i2c.h"


#define DISPLAY_DELAY_INTERVAL		1000		// 1000ms for the display timer period
#define DISPLAY_OFFSET_RANGE		9			// offset range to fine adjust the desk height (as shown in display) by plus/minus 9cm


enum char_set {
	CHAR_DIGIT_0					= 0,
	CHAR_DIGIT_1					= 1,
	CHAR_DIGIT_2					= 2,
	CHAR_DIGIT_3					= 3,
	CHAR_DIGIT_4					= 4,
	CHAR_DIGIT_5					= 5,
	CHAR_DIGIT_6					= 6,
	CHAR_DIGIT_7					= 7,
	CHAR_DIGIT_8					= 8,
	CHAR_DIGIT_9					= 9,
	CHAR_UPPER_A					= 10,
	CHAR_UPPER_C					= 11,
	CHAR_UPPER_E					= 12,
	CHAR_UPPER_F					= 13,
	CHAR_UPPER_H					= 14,
	CHAR_UPPER_L					= 15,
	CHAR_UPPER_O					= 16,
	CHAR_UPPER_P					= 17,
	CHAR_UPPER_U					= 18,
	CHAR_LOWER_C					= 19,
	CHAR_LOWER_D					= 20,
	CHAR_LOWER_I					= 21,
	CHAR_LOWER_N					= 22,
	CHAR_LOWER_O					= 23,
	CHAR_LOWER_R					= 24,
	CHAR_DP							= 25,
	CHAR_BLANK						= 26,
	CHAR_COLON						= 27,
	CHAR_MINUS						= 28,
	CHAR_UNDERLINE					= 29,
	CHAR_ALPHABET					= 30
};

enum symbol_set {
	SYMBOL_BAR_LOW,
	SYMBOL_BAR_MID,
	SYMBOL_BAR_HIGH,
	SYMBOL_MAX	
};

enum display_state {
	DISPLAY_OFF						= 0x00,
	DISPLAY_IDLE					= 0x01,
	DISPLAY_ERROR					= 0x02,
	DISPLAY_DESKPOSITION			= 0x03,
	DISPLAY_LIMIT					= 0x04,
	DISPLAY_NO_POSITION				= 0x05,
	DISPLAY_CALIBRATION				= 0x06,
	DISPLAY_LOCKED					= 0x07,
	DISPLAY_CRITICAL				= 0x08,

	DISPLAY_MENU					= 0x10,
	DISPLAY_MENU_POSITION			= 0x11,
	DISPLAY_MENU_BRIGHTNESS			= 0x12,
	DISPLAY_MENU_VOLUME				= 0x13,
	DISPLAY_MENU_SCREENTIME			= 0x14,
	DISPLAY_MENU_UNIT				= 0x15,
	DISPLAY_MENU_DRIFT				= 0x16,
	DISPLAY_MENU_OFFSET				= 0x17
};

enum display_brightness {
	DISPLAY_BRIGHTNESS_25,
	DISPLAY_BRIGHTNESS_50,
	DISPLAY_BRIGHTNESS_75,
	DISPLAY_BRIGHTNESS_100,
	DISPLAY_BRIGHTNESS_MAX
};

enum display_unit {
	DISPLAY_UNIT_CENTIMETER,
	DISPLAY_UNIT_INCH,
	DISPLAY_UNIT_MAX
};


void display_init(void);
void display_clear(void);
void display_setBrightness(enum display_brightness bri);

void display_show_deskHeight(uint16_t height, bool dp);
void display_show_menuPosition(uint8_t pos);
void display_show_menuBrightness(uint8_t bri);
void display_show_menuVolume(uint8_t volume);
void display_show_menuUnit(uint8_t unit);
void display_show_menuDrift(uint8_t drift);
void display_show_menuOffset(uint8_t offset);
void display_show_menuScreentime(uint8_t screentime);
void display_show_error(void);
void display_show_noPosition(void);
void display_show_end(void);
void display_show_locked(void);
void display_show_calibration(void);
void display_show_critical(void);


#endif /* LYFT_DISPLAY_H_ */