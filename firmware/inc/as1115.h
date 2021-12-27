/*
 * as1115.h
 *
 * Created: 13/09/2021 16:45:27
 *  Author: Matthias Koenig
 */ 


#ifndef AS1115_H_
#define AS1115_H_

#include <asf.h>
#include "hal_i2c.h"


#define AS1115_I2C_ADDRESS							0x00


#define AS1115_SEG_DP								((uint8_t) (1 << 7))
#define AS1115_SEG_A								((uint8_t) (1 << 6))
#define AS1115_SEG_B								((uint8_t) (1 << 5))
#define AS1115_SEG_C								((uint8_t) (1 << 4))
#define AS1115_SEG_D								((uint8_t) (1 << 3))
#define AS1115_SEG_E								((uint8_t) (1 << 2))
#define AS1115_SEG_F								((uint8_t) (1 << 1))
#define AS1115_SEG_G								((uint8_t) (1 << 0))


enum as1115_operation_mode {
	AS1115_OPMODE_SHUTDOWN_DEFAULT					= 0x00,
	AS1115_OPMODE_SHUTDOWN_FEATURE					= 0x80,
	AS1115_OPMODE_NORMAL_DEFAULT					= 0x01,
	AS1115_OPMODE_NORMAL_FEATURE					= 0x81
};

enum as1115_decode_digit {
	AS1115_DECODE_DIGIT_NONE						= 0x00,
	AS1115_DECODE_DIGIT_0							= 0x01,
	AS1115_DECODE_DIGIT_0TO2						= 0x07,
	AS1115_DECODE_DIGIT_0TO5						= 0x3F,
	AS1115_DECODE_DIGIT_0_2_5						= 0x25
};

enum as1115_decode_mode {
	AS1115_DECODE_MODE_BCD,
	AS1115_DECODE_MODE_HEX
};

enum as1115_keyscan {
	AS1115_KEYSCAN_A,
	AS1115_KEYSCAN_B
};

enum as1115_blink_freq {
	AS1115_BLINKFREQ_1SEC,
	AS1115_BLINKFREQ_2SEC
};

enum as1115_scanlimit {
	AS1115_SCANLIMIT_DIGIT_0						= 0x00,
	AS1115_SCANLIMIT_DIGIT_0TO1						= 0x01,
	AS1115_SCANLIMIT_DIGIT_0TO2						= 0x02,
	AS1115_SCANLIMIT_DIGIT_0TO3						= 0x03,
	AS1115_SCANLIMIT_DIGIT_0TO4						= 0x04,
	AS1115_SCANLIMIT_DIGIT_0TO5						= 0x05,
	AS1115_SCANLIMIT_DIGIT_0TO6						= 0x06,
	AS1115_SCANLIMIT_DIGIT_0TO7						= 0x07,
};

enum as1115_intensity_dc {
	AS1115_INTENSITY_DC_1							= 0x00,
	AS1115_INTENSITY_DC_2							= 0x01,
	AS1115_INTENSITY_DC_3							= 0x02,
	AS1115_INTENSITY_DC_4							= 0x03,
	AS1115_INTENSITY_DC_5							= 0x04,
	AS1115_INTENSITY_DC_6							= 0x05,
	AS1115_INTENSITY_DC_7							= 0x06,
	AS1115_INTENSITY_DC_8							= 0x07,
	AS1115_INTENSITY_DC_9							= 0x08,
	AS1115_INTENSITY_DC_10							= 0x09,
	AS1115_INTENSITY_DC_11							= 0x0A,
	AS1115_INTENSITY_DC_12							= 0x0B,
	AS1115_INTENSITY_DC_13							= 0x0C,
	AS1115_INTENSITY_DC_14							= 0x0D,
	AS1115_INTENSITY_DC_15							= 0x0E,
	AS1115_INTENSITY_DC_16							= 0x0F
};

enum as1115_intensity_digit {
	AS1115_INTENSITY_DIGIT_0,
	AS1115_INTENSITY_DIGIT_1,
	AS1115_INTENSITY_DIGIT_2,
	AS1115_INTENSITY_DIGIT_3,
	AS1115_INTENSITY_DIGIT_4,
	AS1115_INTENSITY_DIGIT_5,
	AS1115_INTENSITY_DIGIT_6,
	AS1115_INTENSITY_DIGIT_7,
	AS1115_INTENSITY_GLOBAL
};

enum as1115_register {
	AS1115_REG_DIGIT_0								= 0x01,
	AS1115_REG_DIGIT_1								= 0x02,
	AS1115_REG_DIGIT_2								= 0x03,
	AS1115_REG_DIGIT_3								= 0x04,
	AS1115_REG_DIGIT_4								= 0x05,
	AS1115_REG_DIGIT_5								= 0x06,
	AS1115_REG_DIGIT_6								= 0x07,
	AS1115_REG_DIGIT_7								= 0x08,
	AS1115_REG_DECODE_ENABLE						= 0x09,
	AS1115_REG_INTENSITY_GLOBAL						= 0x0A,
	AS1115_REG_SCANLIMIT							= 0x0B,
	AS1115_REG_SHUTDOWN								= 0x0C,
	AS1115_REG_SELF_ADDRESSING						= 0x0D,
	AS1115_REG_FEATURE								= 0x0E,
	AS1115_REG_DISPLAYTEST							= 0x0F,
	AS1115_REG_INTENSITY_DIG_0TO1					= 0x10,
	AS1115_REG_INTENSITY_DIG_2TO3					= 0x11,
	AS1115_REG_INTENSITY_DIG_4TO5					= 0x12,
	AS1115_REG_INTENSITY_DIG_6TO7					= 0x13,
	AS1115_REG_KEYSCAN_A							= 0x1C,
	AS1115_REG_KEYSCAN_B							= 0x1D
};

enum as1115_error {
	AS1115_ERR_OK,
	AS1115_ERR_BUSY,
	AS1115_ERR_INVALID,
};


void as1115_setOperationMode(enum as1115_operation_mode op_mode);
void as1115_setScanlimit(enum as1115_scanlimit limit);
void as1115_setIntensity(enum as1115_intensity_digit digit, enum as1115_intensity_dc intensity);
void as1115_enableDecoding(enum as1115_decode_digit dec_digits);
void as1115_setDecodingMode(enum as1115_decode_mode dec_mode);
void as1115_setDigit(uint8_t digit, uint8_t value);
void as1115_enableBlinking(bool blink);
void as1115_setBlinkFreq(enum as1115_blink_freq freq);
uint8_t as1115_getKeyscan(enum as1115_keyscan channel);


#endif /* AS1115_H_ */
