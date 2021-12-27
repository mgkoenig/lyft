/*
 * as1115.c
 *
 * Created: 13/09/2021 16:45:17
 *  Author: Matthias Koenig
 */ 

#include "as1115.h"


static uint8_t	as1115_getRegister(enum as1115_register reg);
static void		as1115_setRegister(enum as1115_register reg, uint8_t value);


void as1115_setOperationMode(enum as1115_operation_mode op_mode)
{
	as1115_setRegister(AS1115_REG_SHUTDOWN, op_mode);
}

void as1115_setScanlimit(enum as1115_scanlimit limit)
{
	as1115_setRegister(AS1115_REG_SCANLIMIT, limit);
}

void as1115_setIntensity(enum as1115_intensity_digit digit, enum as1115_intensity_dc intensity)
{
	if (digit == AS1115_INTENSITY_GLOBAL)
	{
		as1115_setRegister(AS1115_REG_INTENSITY_GLOBAL, intensity);
	}
}

void as1115_enableDecoding(enum as1115_decode_digit dec_digits)
{
	as1115_setRegister(AS1115_REG_DECODE_ENABLE, dec_digits);
}

void as1115_setDecodingMode(enum as1115_decode_mode dec_mode)
{
	uint8_t feature_reg;
	
	feature_reg = as1115_getRegister(AS1115_REG_FEATURE);
	
	switch (dec_mode)
	{
		case AS1115_DECODE_MODE_BCD: (feature_reg &= 0xFB); break;
		case AS1115_DECODE_MODE_HEX: (feature_reg |= 0x04); break;
		default: break;
	}
	
	as1115_setRegister(AS1115_REG_FEATURE, feature_reg);
}

void as1115_setDigit(uint8_t digit, uint8_t value)
{
	switch (digit)
	{
		case 0: as1115_setRegister(AS1115_REG_DIGIT_0, value); break;
		case 1: as1115_setRegister(AS1115_REG_DIGIT_1, value); break;
		case 2: as1115_setRegister(AS1115_REG_DIGIT_2, value); break;
		case 3: as1115_setRegister(AS1115_REG_DIGIT_3, value); break;
		case 4: as1115_setRegister(AS1115_REG_DIGIT_4, value); break;
		case 5: as1115_setRegister(AS1115_REG_DIGIT_5, value); break;
		case 6: as1115_setRegister(AS1115_REG_DIGIT_6, value); break;
		case 7: as1115_setRegister(AS1115_REG_DIGIT_7, value); break;
		default: break;
	}
}

void as1115_enableBlinking(bool blink)
{
	uint8_t feature_reg;
	
	feature_reg = as1115_getRegister(AS1115_REG_FEATURE);
	
	if (blink)
	{
		feature_reg |= 0x10;
	} else {
		feature_reg &= 0xEF;
	}
	
	as1115_setRegister(AS1115_REG_FEATURE, feature_reg);
}

void as1115_setBlinkFreq(enum as1115_blink_freq freq)
{
	uint8_t feature_reg;
	
	feature_reg = as1115_getRegister(AS1115_REG_FEATURE);
	
	switch (freq)
	{
		case AS1115_BLINKFREQ_1SEC: (feature_reg |= 0x20); break;
		case AS1115_BLINKFREQ_2SEC: (feature_reg &= 0xDF); break;
		default: break;
	}
	
	as1115_setRegister(AS1115_REG_FEATURE, feature_reg);
}

uint8_t as1115_getKeyscan(enum as1115_keyscan channel)
{
	uint8_t keys;
	
	switch (channel)
	{
		case AS1115_KEYSCAN_A: keys = as1115_getRegister(AS1115_REG_KEYSCAN_A); break;
		case AS1115_KEYSCAN_B: keys = as1115_getRegister(AS1115_REG_KEYSCAN_B); break;
		default: keys = 0xFF;
	}
	
	return keys;
}

static uint8_t as1115_getRegister(enum as1115_register reg)
{
	uint8_t reg_val;
	
	hal_i2c_write(AS1115_I2C_ADDRESS, &reg, 1, true);
	//hal_i2c_wait();
	
	hal_i2c_read(AS1115_I2C_ADDRESS, &reg_val, 1, true);
	//hal_i2c_wait();
	
	return reg_val;
}

static void as1115_setRegister(enum as1115_register reg, uint8_t value)
{
	uint8_t data[2] = {reg, value};

	hal_i2c_write(AS1115_I2C_ADDRESS, data, 2, true);
	//hal_i2c_wait();
}
