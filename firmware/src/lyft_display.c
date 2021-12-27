/*
 * lyft_display.c
 *
 * Created: 13/11/2021 12:39:32
 *  Author: Matthias Koenig
 */ 

#include "lyft_display.h"


const uint8_t ascii[CHAR_ALPHABET] = {
	0x7E,			// 0
	0x30,			// 1
	0x6D,			// 2
	0x79,			// 3
	0x33,			// 4
	0x5B,			// 5
	0x5F,			// 6
	0x70,			// 7
	0x7F,			// 8
	0x7B,			// 9
	0x77,			// A
	0x4E,			// C
	0x4F,			// E
	0x47,			// F
	0x37,			// H
	0x0E,			// L
	0x7E,			// O
	0x67,			// P
	0x3E,			// U
	0x0D,			// c
	0x3D,			// d
	0x10,			// i
	0x15,			// n
	0x1D,			// o
	0x05,			// r
	0x80,			// DP
	0x00,			// BLANK
	0x60,			// COLON
	0x01,			// MINUS
	0x08			// UNDERLINE
};

const uint8_t symbol[SYMBOL_MAX] = {
	0x08,			// one bar
	0x09,			// two bars
	0x49			// three bars
};


void display_init(void)
{
	hal_i2c_init();
	
	as1115_setScanlimit(AS1115_SCANLIMIT_DIGIT_0TO3);
	as1115_enableDecoding(AS1115_DECODE_DIGIT_NONE);
	as1115_setOperationMode(AS1115_OPMODE_NORMAL_DEFAULT);
	as1115_getKeyscan(AS1115_KEYSCAN_A);
	
	display_clear();
}

void display_clear(void)
{
	as1115_setOperationMode(AS1115_OPMODE_SHUTDOWN_FEATURE);
	as1115_setDigit(0, 0x00);
	as1115_setDigit(1, 0x00);
	as1115_setDigit(2, 0x00);
	as1115_setDigit(3, 0x00);
	as1115_setOperationMode(AS1115_OPMODE_NORMAL_DEFAULT);
}

void display_setBrightness(enum display_brightness bri)
{
	switch (bri)
	{
		case DISPLAY_BRIGHTNESS_25: as1115_setIntensity(AS1115_INTENSITY_GLOBAL, AS1115_INTENSITY_DC_1); break;		
		case DISPLAY_BRIGHTNESS_50: as1115_setIntensity(AS1115_INTENSITY_GLOBAL, AS1115_INTENSITY_DC_4); break;		
		case DISPLAY_BRIGHTNESS_75: as1115_setIntensity(AS1115_INTENSITY_GLOBAL, AS1115_INTENSITY_DC_8); break;		
		case DISPLAY_BRIGHTNESS_100: as1115_setIntensity(AS1115_INTENSITY_GLOBAL, AS1115_INTENSITY_DC_16); break;		
		default: as1115_setIntensity(AS1115_INTENSITY_GLOBAL, AS1115_INTENSITY_DC_4); break;
	}
}

void display_show_deskHeight(uint16_t height, bool dp)
{
	uint8_t		ones, tens, hundreds;
	
	hundreds = ((height / 100) % 10);
	tens	 = ((height / 10)  % 10);
	ones	 = ((height / 1)   % 10);
	
	if (hundreds == 0) {
		as1115_setDigit(0, ascii[CHAR_BLANK]);
	} else {
		as1115_setDigit(0, ascii[hundreds]);
	}
	
	if (dp == true) {
		as1115_setDigit(1, (ascii[tens] | ascii[CHAR_DP]));
	} else {
		as1115_setDigit(1, ascii[tens]);
	}
	
	as1115_setDigit(2, ascii[ones]);
	as1115_setDigit(3, ascii[CHAR_BLANK]);
}

void display_show_menuPosition(uint8_t pos)
{
	as1115_setDigit(0, ascii[CHAR_UPPER_P]);
	as1115_setDigit(1, ascii[CHAR_BLANK]);	
	
	switch (pos)
	{
		case 0: as1115_setDigit(2, ascii[CHAR_DIGIT_1]); break;
		case 1: as1115_setDigit(2, ascii[CHAR_DIGIT_2]); break;
		case 2: as1115_setDigit(2, ascii[CHAR_DIGIT_3]); break;
		case 3: as1115_setDigit(2, ascii[CHAR_DIGIT_4]); break;
		default: as1115_setDigit(2, ascii[CHAR_UNDERLINE]); break;
	}	
	
	as1115_setDigit(3, ascii[CHAR_COLON]);
}


void display_show_menuBrightness(uint8_t bri)
{
	as1115_setDigit(0, ascii[CHAR_UPPER_L]);
	as1115_setDigit(1, ascii[CHAR_BLANK]);
	as1115_setDigit(2, ascii[++bri]);
	as1115_setDigit(3, ascii[CHAR_COLON]);
}

void display_show_menuVolume(uint8_t volume)
{
	as1115_setDigit(0, ascii[CHAR_UPPER_A]);
	as1115_setDigit(1, ascii[CHAR_BLANK]);
	
	switch (volume)
	{
		case 0: as1115_setDigit(2, ascii[CHAR_LOWER_O]); break; 
		case 1: as1115_setDigit(2, symbol[SYMBOL_BAR_LOW]); break;	
		case 2: as1115_setDigit(2, symbol[SYMBOL_BAR_MID]); break;	
		case 3: as1115_setDigit(2, symbol[SYMBOL_BAR_HIGH]); break;	
	}
	
	as1115_setDigit(3, ascii[CHAR_COLON]);
}

void display_show_menuUnit(uint8_t unit)
{
	as1115_setDigit(0, ascii[CHAR_UPPER_U]);
	
	switch (unit)
	{
		case 0:
			as1115_setDigit(1, ascii[CHAR_BLANK]);
			as1115_setDigit(2, ascii[CHAR_LOWER_C]);
			break;
		
		case 1:
			as1115_setDigit(1, ascii[CHAR_LOWER_I]);
			as1115_setDigit(2, ascii[CHAR_LOWER_N]);
			break;
	}
	
	as1115_setDigit(3, ascii[CHAR_COLON]);
}

void display_show_menuDrift(uint8_t drift)
{
	uint8_t		ones, tens;
	
	tens	 = ((drift / 10)  % 10);
	ones	 = ((drift / 1)   % 10);
	
	as1115_setDigit(0, ascii[CHAR_LOWER_D]);
	
	if (tens > 0) {
		as1115_setDigit(1, ascii[tens]);
	} else {
		as1115_setDigit(1, ascii[CHAR_BLANK]);
	}
		
	as1115_setDigit(2, ascii[ones]);		
	as1115_setDigit(3, ascii[CHAR_COLON]);
}

void display_show_menuScreentime(uint8_t screentime)
{
	as1115_setDigit(0, ascii[CHAR_UPPER_F]);
	
	switch (screentime)
	{
		case 5:
			as1115_setDigit(1, ascii[CHAR_BLANK]);
			as1115_setDigit(2, ascii[CHAR_DIGIT_5]);
			break;
		
		case 10:
			as1115_setDigit(1, ascii[CHAR_DIGIT_1]);
			as1115_setDigit(2, ascii[CHAR_DIGIT_0]);
			break;
		
		case 15:
			as1115_setDigit(1, ascii[CHAR_DIGIT_1]);
			as1115_setDigit(2, ascii[CHAR_DIGIT_5]);
			break;
		
		case 20:
			as1115_setDigit(1, ascii[CHAR_DIGIT_2]);
			as1115_setDigit(2, ascii[CHAR_DIGIT_0]);
			break;
	}
	
	as1115_setDigit(3, ascii[CHAR_COLON]);
}

void display_show_menuOffset(uint8_t offset)
{
	uint8_t number;
	
	as1115_setDigit(0, ascii[CHAR_UPPER_C]);
	
	if (offset < DISPLAY_OFFSET_RANGE)
	{
		number = DISPLAY_OFFSET_RANGE - offset;
		as1115_setDigit(1, ascii[CHAR_MINUS]);
		as1115_setDigit(2, ascii[number]);
	}
	else if (offset == DISPLAY_OFFSET_RANGE)
	{
		as1115_setDigit(1, ascii[CHAR_BLANK]);
		as1115_setDigit(2, ascii[CHAR_DIGIT_0]);
	}
	else 
	{
		number = offset - DISPLAY_OFFSET_RANGE;
		as1115_setDigit(1, ascii[CHAR_BLANK]);
		as1115_setDigit(2, ascii[number]);
	}

	as1115_setDigit(3, ascii[CHAR_COLON]);
}

void display_show_error(void)
{
	as1115_setDigit(0, ascii[CHAR_UPPER_E]);
	as1115_setDigit(1, ascii[CHAR_LOWER_R]);
	as1115_setDigit(2, (ascii[CHAR_LOWER_R] | ascii[CHAR_DP]));
	as1115_setDigit(3, ascii[CHAR_BLANK]);
}

void display_show_noPosition(void)
{
	as1115_setDigit(0, ascii[CHAR_LOWER_N]);
	as1115_setDigit(1, ascii[CHAR_LOWER_O]);
	as1115_setDigit(2, ascii[CHAR_UPPER_P]);
	as1115_setDigit(3, ascii[CHAR_BLANK]);
}

void display_show_end(void)
{
	as1115_setDigit(0, ascii[CHAR_UPPER_E]);
	as1115_setDigit(1, ascii[CHAR_LOWER_N]);
	as1115_setDigit(2, ascii[CHAR_LOWER_D]);
	as1115_setDigit(3, ascii[CHAR_BLANK]);
}

void display_show_locked(void)
{
	as1115_setDigit(0, ascii[CHAR_UPPER_L]);
	as1115_setDigit(1, ascii[CHAR_UPPER_O]);
	as1115_setDigit(2, ascii[CHAR_UPPER_C]);
	as1115_setDigit(3, ascii[CHAR_BLANK]);
}

void display_show_calibration(void)
{
	as1115_setDigit(0, ascii[CHAR_UPPER_C]);
	as1115_setDigit(1, ascii[CHAR_UPPER_A]);
	as1115_setDigit(2, (ascii[CHAR_UPPER_L] | ascii[CHAR_DP]));
	as1115_setDigit(3, ascii[CHAR_BLANK]);
}

void display_show_critical(void)
{
	as1115_setDigit(0, symbol[SYMBOL_BAR_HIGH]);
	as1115_setDigit(1, symbol[SYMBOL_BAR_HIGH]);
	as1115_setDigit(2, symbol[SYMBOL_BAR_HIGH]);
	as1115_setDigit(3, ascii[CHAR_BLANK]);
}