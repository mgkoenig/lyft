/*
 * lyft.h
 *
 * Created: 13/11/2021 12:33:39
 *  Author: Matthias Koenig
 */ 


#ifndef LYFT_H_
#define LYFT_H_

#include <asf.h>
#include "hal_nvm.h"
#include "hal_irq.h"
#include "hal_gpio.h"
#include "hal_timer.h"
#include "hal_delay.h"

#include "lyft_util.h"
#include "lyft_audio.h"
#include "lyft_keypad.h"
#include "lyft_display.h"
#include "lyft_motor.h"


#define VERSION_MAJOR						1
#define VERSION_MINOR						0
#define VERSION_PATCH						0

#define LYFT_MENU_ENTER_DELAY				2000    // duration of pressing the buttons to enter the menu in ms
#define LYFT_MENU_LEAVE_DELAY				6		// duration to stay within the menu in s
#define LYFT_CALIBRATION_START_DELAY		2000	// duration until calibration will start in ms
#define LYFT_OFFSET_RANGE					9		// adjustable desk height (on display) by plus/minus 9cm
	

enum lyft_screentime {
	LYFT_SCREENTIME_5SEC					= 5,
	LYFT_SCREENTIME_10SEC					= 10,
	LYFT_SCREENTIME_15SEC					= 15,
	LYFT_SCREENTIME_20SEC					= 20
};

enum lyft_offset
{
	LYFT_OFFSET_MINUS_9						= 0,
	LYFT_OFFSET_MINUS_8						= 1,
	LYFT_OFFSET_MINUS_7						= 2,
	LYFT_OFFSET_MINUS_6						= 3,
	LYFT_OFFSET_MINUS_5						= 4,
	LYFT_OFFSET_MINUS_4						= 5,
	LYFT_OFFSET_MINUS_3						= 6,
	LYFT_OFFSET_MINUS_2						= 7,
	LYFT_OFFSET_MINUS_1						= 8,
	LYFT_OFFSET_ZERO						= 9,
	LYFT_OFFSET_PLUS_1						= 10,
	LYFT_OFFSET_PLUS_2						= 11,
	LYFT_OFFSET_PLUS_3						= 12,
	LYFT_OFFSET_PLUS_4						= 13,
	LYFT_OFFSET_PLUS_5						= 14,
	LYFT_OFFSET_PLUS_6						= 15,
	LYFT_OFFSET_PLUS_7						= 16,
	LYFT_OFFSET_PLUS_8						= 17,
	LYFT_OFFSET_PLUS_9						= 18,
	LYFT_OFFSET_MAX							= 19
};


struct lyft_config {
	enum display_unit			unit;
	enum audio_volume			volume;
	enum lyft_offset			offset;
	enum display_brightness		brightness;
	enum lyft_screentime		screentime;		
};	


void lyft_init(void);
void lyft_run(void);


#endif /* LYFT_H_ */