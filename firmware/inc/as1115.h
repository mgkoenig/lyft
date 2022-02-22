/**
 * @file as1115.h
 * @author Matthias Koenig (mgkoenig) 
 * @date 13 Sept 2021 
 * @brief AS1115 Display Driver 
 *
 * @details The HMI board uses a 7-segment display and six 
 * buttons for user interaction. In order to reduce hard
 * wired connections between the controller board and the 
 * HMI board the AS1115 driver is used. 
 *
 * This display driver allows both to drive display segments
 * as well as to read button inputs. All data is send/received
 * using the well known I2C interafce. 
 */

#ifndef AS1115_H_
#define AS1115_H_


#include <asf.h>
#include "hal_i2c.h"


#define AS1115_I2C_ADDRESS					0x00					/**< Default I2C address of the driver. */


#define AS1115_SEG_DP						((uint8_t) (1 << 7))	/**< Corresponding bit within the readback register for segment dot point */
#define AS1115_SEG_A						((uint8_t) (1 << 6))	/**< Corresponding bit within the readback register for segment A */
#define AS1115_SEG_B						((uint8_t) (1 << 5))	/**< Corresponding bit within the readback register for segment B */
#define AS1115_SEG_C						((uint8_t) (1 << 4))	/**< Corresponding bit within the readback register for segment C */
#define AS1115_SEG_D						((uint8_t) (1 << 3))	/**< Corresponding bit within the readback register for segment D */
#define AS1115_SEG_E						((uint8_t) (1 << 2))	/**< Corresponding bit within the readback register for segment E */
#define AS1115_SEG_F						((uint8_t) (1 << 1))	/**< Corresponding bit within the readback register for segment F */
#define AS1115_SEG_G						((uint8_t) (1 << 0))	/**< Corresponding bit within the readback register for segment G */


/** 
 * @enum as1115_operation_mode 
 * The display driver supports four different shutdown/enable modes.
 * Depending on the mode the Feature Register will maintain 
 * its settings or reset to its default values. 
 *
 * @note The AS1115 should not be shut down at any time (like a 
 * standby mode if the display is currently not used) because 
 * otherwise no keypad inputs would be detected! 
 */
enum as1115_operation_mode {
	AS1115_OPMODE_SHUTDOWN_DEFAULT			= 0x00,			/**< Shutdown Mode, Reset Feature Register to Default Settings */
	AS1115_OPMODE_SHUTDOWN_FEATURE			= 0x80,			/**< Shutdown Mode, Feature Register Unchanged */
	AS1115_OPMODE_NORMAL_DEFAULT			= 0x01,			/**< Normal Operation, Reset Feature Register to Default Settings */
	AS1115_OPMODE_NORMAL_FEATURE			= 0x81			/**< Normal Operation, Feature Register Unchanged */
};

/** 
 * @enum as1115_decode_digit 
 * The value of the Decode Enable Register defines which digits 
 * of the display are going to be decoded. Whether as HEX or 
 * BCD value (see #as1115_decode_mode). 
 */
enum as1115_decode_digit {
	AS1115_DECODE_DIGIT_NONE				= 0x00,			/**< No decode for digits 7:0
	AS1115_DECODE_DIGIT_0					= 0x01,			/**< BCD/HEX decoding for digit 0. No decode for digits 7:1 */
	AS1115_DECODE_DIGIT_0TO2				= 0x07,			/**< BCD/HEX decoding for digits 0:2. No decode for digits 7:3 */
	AS1115_DECODE_DIGIT_0TO5				= 0x3F,			/**< BCD/HEX decoding for digits 0:5. No decode for digits 7:6 */
	AS1115_DECODE_DIGIT_0_2_5				= 0x25			/**< BCD/HEX decoding for digits 0, 2 and 5. No decode for digits 1, 3, 4, 6 and 7 */
};

/** 
 * @enum as1115_decode_mode 
 * Data which should be displayed might be interpreted as 
 * BCD or HEX coded. For example, a HEX coded byte with the
 * value 0x0F would be displayed as 'F'. 
 */
enum as1115_decode_mode {
	AS1115_DECODE_MODE_BCD,					/**< Received date will be interpreted as BCD code */
	AS1115_DECODE_MODE_HEX					/**< Received date will be interpreted as HEX code */
};

/** 
 * @enum as1115_keyscan 
 * The display driver is equipped with two keyscan ports. 
 * Each port/register is capable of reading 8 buttons. 
 */
enum as1115_keyscan {
	AS1115_KEYSCAN_A,						/**< Keyscan register A */
	AS1115_KEYSCAN_B						/**< Keyscan register B */
};

/** 
 * @enum as1115_blink_freq 
 * The display driver also supports the feature to let 
 * some segments blink with two different frequencies. This 
 * feature is commonly used for clock applications (blinking
 * colon). Unfortunately, this feature should not be used if 
 * keyscans are used.
 */
enum as1115_blink_freq {
	AS1115_BLINKFREQ_1SEC,					/**< Blinking frequency of 1 sec. */
	AS1115_BLINKFREQ_2SEC					/**< Blinking frequency of 2 sec. */
};

/** 
 * @enum as1115_scanlimit 
 * The Scan-Limit Register controls which of the digits 
 * are to be displayed. If the number of digits display
 * is reduced, the update frequency is increased. 
 */
enum as1115_scanlimit {
	AS1115_SCANLIMIT_DIGIT_0				= 0x00,		/**< Display digit 0 only */
	AS1115_SCANLIMIT_DIGIT_0TO1				= 0x01,		/**< Display digits 0:1 */
	AS1115_SCANLIMIT_DIGIT_0TO2				= 0x02,		/**< Display digits 0:2 */
	AS1115_SCANLIMIT_DIGIT_0TO3				= 0x03,		/**< Display digits 0:3 */
	AS1115_SCANLIMIT_DIGIT_0TO4				= 0x04,		/**< Display digits 0:4 */
	AS1115_SCANLIMIT_DIGIT_0TO5				= 0x05,		/**< Display digits 0:5 */
	AS1115_SCANLIMIT_DIGIT_0TO6				= 0x06,		/**< Display digits 0:6 */
	AS1115_SCANLIMIT_DIGIT_0TO7				= 0x07,		/**< Display digits 0:7 */
};

/** 
 * @enum as1115_intensity_dc 
 * The brightness of the display can be controlled by 
 * digital means using the Intensity Control Registers. 
 * The intensity can be controlled globally for all digits, 
 * or for each digit individually. Display brightness is 
 * controlled by an integrated pulse-width modulator.
 */
enum as1115_intensity_dc {
	AS1115_INTENSITY_DC_1					= 0x00,		/**< Intensity level 1/16 (min) */
	AS1115_INTENSITY_DC_2					= 0x01,		/**< Intensity level 2/16 */
	AS1115_INTENSITY_DC_3					= 0x02,		/**< Intensity level 3/16 */
	AS1115_INTENSITY_DC_4					= 0x03,		/**< Intensity level 4/16 */
	AS1115_INTENSITY_DC_5					= 0x04,		/**< Intensity level 5/16 */
	AS1115_INTENSITY_DC_6					= 0x05,		/**< Intensity level 6/16 */
	AS1115_INTENSITY_DC_7					= 0x06,		/**< Intensity level 7/16 */
	AS1115_INTENSITY_DC_8					= 0x07,		/**< Intensity level 8/16 */
	AS1115_INTENSITY_DC_9					= 0x08,		/**< Intensity level 9/16 */
	AS1115_INTENSITY_DC_10					= 0x09,		/**< Intensity level 10/16 */
	AS1115_INTENSITY_DC_11					= 0x0A,		/**< Intensity level 11/16 */
	AS1115_INTENSITY_DC_12					= 0x0B,		/**< Intensity level 12/16 */
	AS1115_INTENSITY_DC_13					= 0x0C,		/**< Intensity level 13/16 */
	AS1115_INTENSITY_DC_14					= 0x0D,		/**< Intensity level 14/16 */
	AS1115_INTENSITY_DC_15					= 0x0E,		/**< Intensity level 15/16 */
	AS1115_INTENSITY_DC_16					= 0x0F		/**< Intensity level 16/16 (max) */
};

/** 
 * @enum as1115_intensity_digit 
 * The intensity can be controlled globally for all digits, 
 * or for each digit individually. The global intensity command 
 * will write intensity data to all four individual brightness 
 * registers, while the individual intensity command will only 
 * write to the associated individual intensity register.
 *
 * So far, only a global setting is supported by the 
 * driver software. 
 */
enum as1115_intensity_digit {
	AS1115_INTENSITY_DIGIT_0,							/**< Set intensity of digit 0 */
	AS1115_INTENSITY_DIGIT_1,							/**< Set intensity of digit 1 */
	AS1115_INTENSITY_DIGIT_2,							/**< Set intensity of digit 2 */
	AS1115_INTENSITY_DIGIT_3,							/**< Set intensity of digit 3 */
	AS1115_INTENSITY_DIGIT_4,							/**< Set intensity of digit 4 */
	AS1115_INTENSITY_DIGIT_5,							/**< Set intensity of digit 5 */
	AS1115_INTENSITY_DIGIT_6,							/**< Set intensity of digit 6 */
	AS1115_INTENSITY_DIGIT_7,							/**< Set intensity of digit 7 */
	AS1115_INTENSITY_GLOBAL								/**< Set intensity of all digits */
};

/** 
 * @enum as1115_register 
 * Overview of all digit and control registers. 
 */
enum as1115_register {
	AS1115_REG_DIGIT_0						= 0x01,		/**< Digit register contains the data to be displayed on digit 0 */
	AS1115_REG_DIGIT_1						= 0x02,		/**< Digit register contains the data to be displayed on digit 1 */
	AS1115_REG_DIGIT_2						= 0x03,		/**< Digit register contains the data to be displayed on digit 2 */
	AS1115_REG_DIGIT_3						= 0x04,		/**< Digit register contains the data to be displayed on digit 3 */
	AS1115_REG_DIGIT_4						= 0x05,		/**< Digit register contains the data to be displayed on digit 4 */
	AS1115_REG_DIGIT_5						= 0x06,		/**< Digit register contains the data to be displayed on digit 5 */
	AS1115_REG_DIGIT_6						= 0x07,		/**< Digit register contains the data to be displayed on digit 6 */
	AS1115_REG_DIGIT_7						= 0x08,		/**< Digit register contains the data to be displayed on digit 7 */
	AS1115_REG_DECODE_ENABLE				= 0x09,		/**< Control register to enable BCD/HEX decoding */
	AS1115_REG_INTENSITY_GLOBAL				= 0x0A,		/**< Control register to set a global intensity level */
	AS1115_REG_SCANLIMIT					= 0x0B,		/**< Control register to set the scan limit (number of used digits) */
	AS1115_REG_SHUTDOWN						= 0x0C,		/**< Control register to enable/disable the driver */
	AS1115_REG_SELF_ADDRESSING				= 0x0D,		/**< Control register for self addressing (to set a different I2C address) */
	AS1115_REG_FEATURE						= 0x0E,		/**< Control register for certain special features like blinking or clock settings */
	AS1115_REG_DISPLAYTEST					= 0x0F,		/**< Control register to run display tests */
	AS1115_REG_INTENSITY_DIG_0TO1			= 0x10,		/**< Control register to set the intensity of digits 0:1 */
	AS1115_REG_INTENSITY_DIG_2TO3			= 0x11,		/**< Control register to set the intensity of digits 2:3 */
	AS1115_REG_INTENSITY_DIG_4TO5			= 0x12,		/**< Control register to set the intensity of digits 4:5 */
	AS1115_REG_INTENSITY_DIG_6TO7			= 0x13,		/**< Control register to set the intensity of digits 6:7 */
	AS1115_REG_KEYSCAN_A					= 0x1C,		/**< Control register to read button states which are connected on port A */
	AS1115_REG_KEYSCAN_B					= 0x1D		/**< Control register to read button states which are connected on port B */
};

/** 
 * @enum as1115_error 
 * General error codes (unused). 
 */
enum as1115_error {
	AS1115_ERR_OK,
	AS1115_ERR_BUSY,
	AS1115_ERR_INVALID,
};


/** 
 * @brief En/disable the driver
 * 
 * By setting the operation mode the driver will be en- or
 * disabled. Depending on the mode the feature register 
 * will maintain its settings or reset to default values. 
 * 
 * @param[in] op_mode which mode should run, eg. #AS1115_OPMODE_NORMAL_DEFAULT
 */
void as1115_setOperationMode(enum as1115_operation_mode op_mode);

/** 
 * @brief Set scan limit
 * 
 * The scan limit defines the number of used digits of
 * the display. If the number of displayed digits is
 * reduced, the update frequency is increased. 
 * 
 * @param[in] limit the number of used digits, eg. #AS1115_SCANLIMIT_DIGIT_0TO2
 */
void as1115_setScanlimit(enum as1115_scanlimit limit);

/** 
 * @brief Set display brightness
 * 
 * This function sets the brightness level of the display. 
 *
 * @note So far only the global setting is supported!
 * 
 * @param[in] digit which digit should be configured. Right now only #AS1115_INTENSITY_GLOBAL is supported. 
 * @param[in] intensity which brightness level should be applied, eg. #AS1115_INTENSITY_DC_16 for full brightness.
 */
void as1115_setIntensity(enum as1115_intensity_digit digit, enum as1115_intensity_dc intensity);

/** 
 * @brief En/disable decoding
 * 
 * This function en- or disables the decoding feature of the driver. 
 * Display data will be interpreted either as BCD or HEX values
 * and displayed accordingly. The decoding mode (if BCD or HEX)
 * is set with function as1115_setDecodingMode(). 
 *
 * @param[in] dec_digits the digits for which the decoding feature 
 * should apply, eg. #AS1115_DECODE_DIGIT_0TO2 to decode only the 
 * first three digits of the display. 
 */
void as1115_enableDecoding(enum as1115_decode_digit dec_digits);

/** 
 * @brief Set decoding mode
 * 
 * If decoding is used, data might be interpreted as BCD or HEX 
 * values. This function sets the decoding mode which one to use. 
 *
 * @param[in] dec_mode whether #AS1115_DECODE_MODE_BCD or #AS1115_DECODE_MODE_HEX
 */
void as1115_setDecodingMode(enum as1115_decode_mode dec_mode);

/** 
 * @brief Write data to a digit
 * 
 * Send a byte pattern which represents a character or symbol 
 * according to the segments of the digit. Standard marking 
 * for 7 segment displays applies: 
 *
 * Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0
 * ------|-------|-------|-------|-------|-------|-------|-------
 *   DP  |   A   |   B   |   C   |   D   |   E   |   F   |   G   
 *
 * @param[in] digit which digit of the display (between 0 and 3)
 * @param[in] value the character/symbol to be displayed (see table)
 */
void as1115_setDigit(uint8_t digit, uint8_t value);

/** 
 * @brief En/disable blinking mode
 * 
 * The display will blink according to the pre-defined blink 
 * frequency or glow steady. 
 *
 * @param[in] blink 'True' to enable or 'False' to disable
 */
void as1115_enableBlinking(bool blink);

/** 
 * @brief Set blink frequency 
 * 
 * The driver supports two different blink frequencies: 
 * A period of 1 sec (0.5 sec on, 0.5 sec off) or two 
 * seconds (1 sec on, 1 sec off). 
 *
 * @param[in] freq the desired frequency, eg. #AS1115_BLINKFREQ_1SEC
 */
void as1115_setBlinkFreq(enum as1115_blink_freq freq);

/** 
 * @brief Read button states 
 * 
 * The key readback of the AS1115 can be used for push buttons
 * as well as switches. Two channles (A and B) can detect up to
 * 16 button states. In case a button was pressed an interrupt 
 * (falling edge) will be triggered to notify the controller. 
 * Afterwards the controller can read the keyscan status register
 * to get the current button states.  
 *
 * @param[in] channel either #AS1115_KEYSCAN_A or #AS1115_KEYSCAN_B
 */
uint8_t as1115_getKeyscan(enum as1115_keyscan channel);


#endif /* AS1115_H_ */
