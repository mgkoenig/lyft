/**
 * @file lyft.h
 * @author Matthias Koenig (mgkoenig) 
 * @date 13 Nov 2021 
 * @brief LYFT Routines 
 *
 * @details  
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


#define LYFT_MENU_ENTER_DELAY				2000    /**< duration of pressing the buttons to enter the menu in ms */
#define LYFT_MENU_LEAVE_DELAY				6		/**< duration to stay within the menu in s */
#define LYFT_CALIBRATION_POSITION			1000	/**< desk position must be lower than 1000 (about 70cm) before starting a calibration */
#define LYFT_CALIBRATION_START_DELAY		2000	/**< duration until calibration will start in ms */
#define LYFT_AUTOMATIC_DRIVE_DELAY			1500	/**< 1500ms delay before starting to move the desk automatically */
#define LYFT_OFFSET_RANGE					9		/**< adjustable desk height (on display) by plus/minus 9cm */
	

/** 
 * @enum lyft_screentime 
 * Four values for the display-on time configuration.
 */
enum lyft_screentime {
	LYFT_SCREENTIME_5SEC					= 5,	/**< Display-on time equals 5 sec. */
	LYFT_SCREENTIME_10SEC					= 10,	/**< Display-on time equals 10 sec. */
	LYFT_SCREENTIME_15SEC					= 15,	/**< Display-on time equals 15 sec. */
	LYFT_SCREENTIME_20SEC					= 20	/**< Display-on time equals 20 sec. */
};

/** 
 * @enum lyft_offset 
 * A correction value of the shown desk height. The 
 * level offset correction allows the user to adjust 
 * the shown desk height by plus/minus 9 cm. 
 *
 * In order to avoid signed integer values this feature
 * uses enum values which represents the proper 
 * correction value.  
 */
enum lyft_offset
{
	LYFT_OFFSET_MINUS_9						= 0,	/**< Level offset correction of minus 9 centimeter */
	LYFT_OFFSET_MINUS_8						= 1,	/**< Level offset correction of minus 8 centimeter */
	LYFT_OFFSET_MINUS_7						= 2,	/**< Level offset correction of minus 7 centimeter */
	LYFT_OFFSET_MINUS_6						= 3,	/**< Level offset correction of minus 6 centimeter */
	LYFT_OFFSET_MINUS_5						= 4,	/**< Level offset correction of minus 5 centimeter */
	LYFT_OFFSET_MINUS_4						= 5,	/**< Level offset correction of minus 4 centimeter */
	LYFT_OFFSET_MINUS_3						= 6,	/**< Level offset correction of minus 3 centimeter */
	LYFT_OFFSET_MINUS_2						= 7,	/**< Level offset correction of minus 2 centimeter */
	LYFT_OFFSET_MINUS_1						= 8,	/**< Level offset correction of minus 1 centimeter */
	LYFT_OFFSET_ZERO						= 9,	/**< Level offset correction of zero centimeter */
	LYFT_OFFSET_PLUS_1						= 10,	/**< Level offset correction of plus 1 centimeter */
	LYFT_OFFSET_PLUS_2						= 11,	/**< Level offset correction of plus 2 centimeter */
	LYFT_OFFSET_PLUS_3						= 12,	/**< Level offset correction of plus 3 centimeter */
	LYFT_OFFSET_PLUS_4						= 13,	/**< Level offset correction of plus 4 centimeter */
	LYFT_OFFSET_PLUS_5						= 14,	/**< Level offset correction of plus 5 centimeter */
	LYFT_OFFSET_PLUS_6						= 15,	/**< Level offset correction of plus 6 centimeter */
	LYFT_OFFSET_PLUS_7						= 16,	/**< Level offset correction of plus 7 centimeter */
	LYFT_OFFSET_PLUS_8						= 17,	/**< Level offset correction of plus 8 centimeter */
	LYFT_OFFSET_PLUS_9						= 18,	/**< Level offset correction of plus 9 centimeter */
	LYFT_OFFSET_MAX							= 19
};

/** 
 * @enum lyft_drive_mode
 * The moving mode of the desk. 
 *
 * In manual mode the memory button has to be hold until the 
 * final desk height is reached while in automatic mode the 
 * button has to be pressed for a short term and then the desk
 * moves autonomously to the memorized position. Obviously, 
 * manual mode is the safer option while automatic mode is 
 * more comfortable. 
 */
enum lyft_drive_mode {
	LYFT_DRIVE_MODE_MANUAL,
	LYFT_DRIVE_MODE_AUTOMATIC,
	LYFT_DRIVE_MODE_MAX
};

/** 
 * @struct lyft_config 
 * A configuration file which holds the current configuration. 
 * On the very first start-up the config filed is filled with 
 * default values for all settings. The user has the chance 
 * to change each setting individually using the config menu. 
 * All settings will be saved to the NVM when leaving the menu. 
 * They are restored at the next start-up of the system.  
 */
struct lyft_config {
	enum display_unit			unit;				/**< The shown unit of the desk height (either cm or inch) */
	enum audio_volume			volume;				/**< The volume of the audio buzzer (off, low, mid, high) */
	enum lyft_offset			offset;				/**< The offset of the desk height (height adjustment) between plus/minus 9cm */
	enum display_brightness		brightness;			/**< The brightness of the display (25%, 50%, 75% or 100%) */
	enum lyft_screentime		screentime;			/**< The display-on time after the last user interaction or change of display information (5s, 10s, 15s or 20s) */
	enum lyft_drive_mode		drive_mode;			/**< The drive mode of the desk, if the user has to hold a button all the time while moving or if the desk moves automatically */
};	


/** 
 * @brief LYFT Init
 *
 * Initializes LYFT with all its hardware modules and 
 * default values of runtime variables. 
 */
void lyft_init(void);

/** 
 * @brief LYFT Runtime 
 *
 * The runtime function handles all routines to ensure a 
 * smooth LYFT experience. Taking care of the motor control, 
 * driving the display, evaluating user inputs (buttons) 
 * and everything else. 
 *
 * This function has to be called periodically 
 * by the main function.  
 */
void lyft_run(void);


#endif /* LYFT_H_ */
