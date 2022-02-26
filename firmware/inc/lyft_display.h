/**
 * @file lyft_display.h
 * @author Matthias Koenig (mgkoenig) 
 * @date 13 Dec 2021 
 * @brief LYFT Display Driver
 *
 * @details LYFT uses a 7-segment display to show user information 
 * like desk height or menu entries. The display is driven by a 
 * <a href="https://ams.com/en/as1115 ">AS1115 display driver</a> which 
 * is addressed using a I2C bus. 
 *
 * This file contains all functions to display the desired information 
 * (whether desk height or menu entries). No configuration will be done
 * here (except display brightness). All other settings are done within 
 * their corresponding modules. For example, the display module only 
 * shows the current audio volume. But the volume itself will be set 
 * within the audio module. 
 */

#ifndef LYFT_DISPLAY_H_
#define LYFT_DISPLAY_H_


#include "as1115.h"
#include "hal_i2c.h"


#define DISPLAY_DELAY_INTERVAL		1000		/**< Display timer period of 1 sec related to screentime (display-on time) */
#define DISPLAY_OFFSET_RANGE		9			/**< Offset range to fine adjust the desk height (as shown on display) by plus/minus 9cm */


/** 
 * @enum char_set 
 * Set of characters and digits (including spaces, underlines, ...) 
 * which can be shown on the 7-segment display.
 */
enum char_set {
	CHAR_DIGIT_0,					/**< Digit 0 */
	CHAR_DIGIT_1,					/**< Digit 1 */
	CHAR_DIGIT_2,					/**< Digit 2 */
	CHAR_DIGIT_3,					/**< Digit 3 */
	CHAR_DIGIT_4,					/**< Digit 4 */
	CHAR_DIGIT_5,					/**< Digit 5 */
	CHAR_DIGIT_6,					/**< Digit 6 */
	CHAR_DIGIT_7,					/**< Digit 7 */
	CHAR_DIGIT_8,					/**< Digit 8 */
	CHAR_DIGIT_9,					/**< Digit 9 */
	CHAR_UPPER_A,					/**< Char 'A' */
	CHAR_UPPER_C,					/**< Char 'C' */
	CHAR_UPPER_E,					/**< Char 'E' */
	CHAR_UPPER_F,					/**< Char 'F' */
	CHAR_UPPER_G,					/**< Char 'G' */
	CHAR_UPPER_H,					/**< Char 'H' */
	CHAR_UPPER_L,					/**< Char 'L' */
	CHAR_UPPER_O,					/**< Char 'O' */
	CHAR_UPPER_P,					/**< Char 'P' */
	CHAR_UPPER_U,					/**< Char 'U' */
	CHAR_LOWER_C,					/**< Char 'c' */
	CHAR_LOWER_D,					/**< Char 'd' */
	CHAR_LOWER_I,					/**< Char 'i' */
	CHAR_LOWER_N,					/**< Char 'n' */
	CHAR_LOWER_O,					/**< Char 'o' */
	CHAR_LOWER_R,					/**< Char 'r' */
	CHAR_DP,						/**< Dot point */
	CHAR_BLANK,						/**< Space */
	CHAR_COLON,						/**< Colon sign (available as own segment on the display) */
	CHAR_MINUS,						/**< Minus sign */
	CHAR_UNDERLINE,					/**< Underline */
	CHAR_ALPHABET					
};

/** 
 * @enum symbol_set 
 * Set of symbols (or special characters) which might be shown on the display.
 */
enum symbol_set {
	SYMBOL_BAR_LOW,			/**< One bar for volume control */
	SYMBOL_BAR_MID,			/**< Two bars for volume control */
	SYMBOL_BAR_HIGH,		/**< Three bars for volume control */
	SYMBOL_MAX	
};

/** 
 * @enum display_state 
 * Different display states. The current display state defines 
 * what is seen on the screen. 
 */
enum display_state {
	DISPLAY_OFF						= 0x00,			/**< Display is turned off */
	DISPLAY_IDLE					= 0x01,			/**< Display is idle (obsolete, unused state) */
	DISPLAY_ERROR					= 0x02,			/**< Display shows an error */
	DISPLAY_DESKPOSITION			= 0x03,			/**< Display shows current desk height (default screen) */
	DISPLAY_LIMIT					= 0x04,			/**< Display shows upper/lower limit is reached */
	DISPLAY_NO_POSITION				= 0x05,			/**< Display shows 'noP' (no saved memory position is available) */
	DISPLAY_CALIBRATION				= 0x06,			/**< Display shows calibration mode */
	DISPLAY_LOCKED					= 0x07,			/**< Display shows lock screen */
	DISPLAY_CRITICAL				= 0x08,			/**< Display shows an alarm state (when desk is blocked) */

	DISPLAY_MENU					= 0x10,			/**< Display shows a menu entry (general menu state) */
	DISPLAY_MENU_POSITION			= 0x11,			/**< Display shows the 'save position' menu entry */
	DISPLAY_MENU_BRIGHTNESS			= 0x12,			/**< Display shows the brightness menu entry */
	DISPLAY_MENU_VOLUME				= 0x13,			/**< Display shows the volume menu entry */
	DISPLAY_MENU_SCREENTIME			= 0x14,			/**< Display shows the screentime menu entry */
	DISPLAY_MENU_UNIT				= 0x15,			/**< Display shows the displayed unit menu entry */
	DISPLAY_MENU_DRIFT				= 0x16,			/**< Display shows the drift of both motors */
	DISPLAY_MENU_OFFSET				= 0x17,			/**< Display shows the desk height offset (plus/minus 9cm) */
	DISPLAY_MENU_DRIVEMODE			= 0x18			/**< Display shows the driving mode (desk moves automatic or manual to its position) */
};

/** 
 * @enum display_brightness 
 * Four different display brightness levels to configure. 
 */
enum display_brightness {
	DISPLAY_BRIGHTNESS_25,			/**< Display brightness 25% */
	DISPLAY_BRIGHTNESS_50,			/**< Display brightness 50% */
	DISPLAY_BRIGHTNESS_75,			/**< Display brightness 75% */
	DISPLAY_BRIGHTNESS_100,			/**< Display brightness 100% */
	DISPLAY_BRIGHTNESS_MAX
};

/** 
 * @enum display_unit
 * The user can choose if the desk height should be shown as centimeters or inches. 
 */
enum display_unit {
	DISPLAY_UNIT_CENTIMETER,		/**< Desk height is shown as centimeters */
	DISPLAY_UNIT_INCH,				/**< Desk height is shown as inches */
	DISPLAY_UNIT_MAX
};


/** 
 * @brief Display initialization
 *
 * Initializes the I2C module on the controller and configures the AS1115 
 * display driver with basic settings (number of used segments, 
 * decoding mode, ...). Finally the display is cleared (turned off). 
 */
void display_init(void);

/** 
 * @brief Clear the display
 *
 * Clears all segments in order to turn the display off. The AS1115 display driver
 * must not turned off entirely because this would disable the keyscan as well. 
 */
void display_clear(void);

/** 
 * @brief Set display brightness
 *
 * Configure the AS1115 display driver to set a pre-defined brightness level.
 *
 * @param[in] bri the level of the display brightness, one of #DISPLAY_BRIGHTNESS_25, 
 * #DISPLAY_BRIGHTNESS_50, #DISPLAY_BRIGHTNESS_75 or #DISPLAY_BRIGHTNESS_100.
 */
void display_setBrightness(enum display_brightness bri);

/** 
 * @brief Display shows desk height
 *
 * This is the default display screen. The display shows the current desk height. 
 * Depending on the configuration either as centimeters or inches.  
 *
 * @param[in] height the current height of the desk as numerical value (either centimeter or inch)
 * @param[in] dp if the unit is set to inch a comma must be displayed as well 
 */
void display_show_deskHeight(uint16_t height, bool dp);

/** 
 * @brief Menu page 'Position'
 *
 * The display shows the first menu entry which allows the user to save the current 
 * desk height to one of the four memory buttons. 
 *
 * @param[in] pos the memory button (1-4) which is used to store the current desk height
 */
void display_show_menuPosition(uint8_t pos);

/** 
 * @brief Menu page 'Brightness'
 *
 * This menu page shows the current brightness level. 
 * - Level (1) equals 25%
 * - Level (2) equals 50%
 * - Level (3) equals 75%
 * - Level (4) equals 100%
 *
 * @param[in] bri the brightness level according to the memory buttons (1 to 4).  
 */
void display_show_menuBrightness(uint8_t bri);

/** 
 * @brief Menu page 'Volume' or 'Audio'
 *
 * This menu page shows the current volume of the buzzer. 
 * - Volume level (o): sound turned off
 * - Volume level (one bar): low volume
 * - Volume level (two bars): mid volume
 * - Volume level (three bars): high volume
 *
 * @param[in] volume the buzzer volume according to the memory buttons (1 to 4).
 */
void display_show_menuVolume(uint8_t volume);

/** 
 * @brief Menu page 'Unit'
 *
 * This menu page shows the unit of the displayed desk height.  
 * - Memory button (1): centimeters
 * - Memory button (2): inches
 * - Memory button (3): not available
 * - Memory button (4): not available
 *
 * @param[in] unit the chosen unit according to the memory buttons (1 to 2).
 */
void display_show_menuUnit(uint8_t unit);

/** 
 * @brief Menu page 'Drift'
 *
 * This menu entry is just a note for the user. It informs the user about 
 * the drift between both motors. 
 *
 * @param[in] drift the current drift value between both motors.
 */
void display_show_menuDrift(uint8_t drift);

/** 
 * @brief Menu page 'Offset'
 *
 * This menu page shows the currently configured offset of the desk height. 
 * In case the displayed desk height does not correlate with the actual height 
 * the user has the opportunity to adjust the value. The range to adjust the 
 * value is between plus/minus 9cm (no matter which unit is set). 
 * - Memory button (1): decrease the offset value
 * - Memory button (2): increase the offset value
 * - Memory button (3): not available
 * - Memory button (4): not available
 *
 * @param[in] offset the current offset of the desk height
 */
void display_show_menuOffset(uint8_t offset);

/** 
 * @brief Menu page 'Drive Mode'
 *
 * This page shows the current drive mode. It might be
 * - (A) for automatic, or
 * - (H) for manual (hand driven)
 *
 * @param[in] drive_mode the mode the desk is moved, either manually or automatically
 */
void display_show_menuDrivemode(uint8_t drive_mode);

/** 
 * @brief Menu page 'Screentime'
 *
 * This menu page shows the currently configured display-on time (screentime).  
 * - Memory button (1): display-on time is set to 5 sec
 * - Memory button (2): display-on time is set to 10 sec
 * - Memory button (3): display-on time is set to 15 sec
 * - Memory button (4): display-on time is set to 20 sec
 *
 * @param[in] screentime the current display-on time of the display (either 5, 10, 15 or 20 seconds)
 */
void display_show_menuScreentime(uint8_t screentime);

/** 
 * @brief Display shows an error state
 *
 * In case of an unexpected error the display shows the letters 'Err'.
 */
void display_show_error(void);

/** 
 * @brief Display shows 'no Position'
 *
 * In case no desk height was saved to a memory button so far, the display shows 
 * the letters 'noP' to inform the user that no position is available. 
 */
void display_show_noPosition(void);

/** 
 * @brief Display shows 'End'
 *
 * If the desk is moved to its upper/lower limit the display shows
 * the letters 'End' to inform the user that the limit is reached. 
 */
void display_show_end(void);

/** 
 * @brief Display shows 'Locked'
 *
 * In case the user tries to move the desk without a safety pin connected the 
 * display shows the letters 'LOC'. This means that all features are unavailable
 * as long as no safety pin is plugged in. 
 */
void display_show_locked(void);

/** 
 * @brief Display shows 'Calibration'
 *
 * The display shows the letters 'CAL' while the motors are calibrating themselves.
 */
void display_show_calibration(void);

/** 
 * @brief Display shows 'Critical'
 *
 * In case the desk gets stuck while moving the motors will stop immediately and 
 * an alarm screen will be shown on the display. 
 */
void display_show_critical(void);


#endif /* LYFT_DISPLAY_H_ */