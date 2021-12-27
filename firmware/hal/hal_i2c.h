/*
 * hal_i2c.h
 *
 * Created: 19/11/2021 13:30:00
 *  Author: Sire
 */ 


#ifndef HAL_I2C_H_
#define HAL_I2C_H_

#include <asf.h>
#include "conf_board.h"

#define I2C_TIMEOUT_BYTE		(160)		// t_bit = 2.5us; t_byte = (2.5 * 8) / 125ns = 160
#define I2C_HEADER_LENGTH		(2)			// address byte plus some margin

enum i2c_status {
	I2C_STATE_IDLE,
	I2C_STATE_BUSY,
	I2C_STATE_READY
};

enum i2c_error {
	I2C_ERR_OK,
	I2C_ERR_BUSY,
	I2C_ERR_INVALID,
	I2C_ERR_TIMEOUT
};

enum i2c_error hal_i2c_init(void);
enum i2c_error hal_i2c_write(uint8_t address, uint8_t *data, uint8_t len, bool stop);
enum i2c_error hal_i2c_read(uint8_t address, uint8_t *data, uint8_t len, bool stop);


#endif /* HAL_I2C_H_ */