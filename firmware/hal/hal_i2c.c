/*
 * hal_i2c.c
 *
 * Created: 19/11/2021 13:29:52
 *  Author: Sire
 */ 

#include "hal_i2c.h"

enum i2c_status i2c_state = I2C_STATE_IDLE;
struct i2c_master_packet wr_packet;
struct i2c_master_packet rd_packet;
struct i2c_master_module i2c_master_instance;

static enum i2c_error hal_i2c_wait(uint16_t timeout);
static void i2c_cb_tx_ready (struct i2c_master_module *const module);
static void i2c_cb_rx_ready (struct i2c_master_module *const module);


enum i2c_error hal_i2c_init(void)
{
	enum i2c_error i2c_err = I2C_ERR_OK;
	struct i2c_master_config config_i2c_master;
	
	if (i2c_state == I2C_STATE_IDLE)
	{
		i2c_master_get_config_defaults(&config_i2c_master);
		config_i2c_master.generator_source = GCLK_GENERATOR_0;
		config_i2c_master.buffer_timeout = 65535;
		config_i2c_master.baud_rate = I2C_MASTER_BAUD_RATE_400KHZ;
		config_i2c_master.pinmux_pad0 = I2C_PINMUX_PAD0;
		config_i2c_master.pinmux_pad1 = I2C_PINMUX_PAD1;
		
		while(i2c_master_init(&i2c_master_instance, I2C_MODULE, &config_i2c_master) != STATUS_OK);
		i2c_master_enable(&i2c_master_instance);
		
		i2c_master_register_callback(&i2c_master_instance, i2c_cb_tx_ready, I2C_MASTER_CALLBACK_WRITE_COMPLETE);
		i2c_master_register_callback(&i2c_master_instance, i2c_cb_rx_ready, I2C_MASTER_CALLBACK_READ_COMPLETE);
		i2c_master_enable_callback(&i2c_master_instance, I2C_MASTER_CALLBACK_WRITE_COMPLETE);
		i2c_master_enable_callback(&i2c_master_instance, I2C_MASTER_CALLBACK_READ_COMPLETE);
		
		i2c_state = I2C_STATE_READY;
	}
	else {
		i2c_err = I2C_ERR_INVALID;
	}
	
	return i2c_err;
}

enum i2c_error hal_i2c_write(uint8_t address, uint8_t *data, uint8_t len, bool stop)
{
	uint16_t		timeout;
	enum i2c_error	i2c_err = I2C_ERR_OK;
	
	if (i2c_state == I2C_STATE_READY)
	{
		if ((data != NULL) && (len > 0))
		{
			wr_packet.address			= address;
			wr_packet.data				= data;
			wr_packet.data_length		= len;
			wr_packet.high_speed		= false;
			wr_packet.ten_bit_address	= false;

			timeout = ((len + I2C_HEADER_LENGTH) * I2C_TIMEOUT_BYTE);
			i2c_state = I2C_STATE_BUSY;
			
			if (stop == true) {
				i2c_master_write_packet_job(&i2c_master_instance, &wr_packet);
			} else {
				i2c_master_write_packet_job_no_stop(&i2c_master_instance, &wr_packet);
			}
			
			i2c_err = hal_i2c_wait(timeout);
		}
		else {
			i2c_err = I2C_ERR_INVALID;
		}
	}
	else {
		i2c_err = I2C_ERR_BUSY;
	}
	
	return i2c_err;
}

enum i2c_error hal_i2c_read(uint8_t address, uint8_t *data, uint8_t len, bool stop)
{
	uint16_t		timeout;
	enum i2c_error	i2c_err = I2C_ERR_OK;
	
	if (i2c_state == I2C_STATE_READY)
	{
		if ((data != NULL) && (len > 0))
		{
			rd_packet.address			= address;
			rd_packet.data				= data;
			rd_packet.data_length		= len;
			rd_packet.high_speed		= false;
			rd_packet.ten_bit_address	= false;
			
			timeout = ((len + I2C_HEADER_LENGTH) * I2C_TIMEOUT_BYTE);
			i2c_state = I2C_STATE_BUSY;
			
			if (stop == true) {
				i2c_master_read_packet_job(&i2c_master_instance, &rd_packet);
			} else {
				i2c_master_read_packet_job_no_stop(&i2c_master_instance, &rd_packet);
			}
			
			i2c_err = hal_i2c_wait(timeout);
		}
		else {
			i2c_err = I2C_ERR_INVALID;
		}
	}
	else {
		i2c_err = I2C_ERR_BUSY;
	}
	
	return i2c_err;
}

static enum i2c_error hal_i2c_wait(uint16_t timeout)
{
	enum i2c_error err = I2C_ERR_OK;
	uint16_t count = 0;
	
	while(i2c_state != I2C_STATE_READY) {
		count++;
		if (count > timeout) {
			err = I2C_ERR_TIMEOUT;
			break;
		}
	}
	
	return err;
}

static void i2c_cb_tx_ready (struct i2c_master_module *const module)
{
	i2c_state = I2C_STATE_READY;
}

static void i2c_cb_rx_ready (struct i2c_master_module *const module)
{
	i2c_state = I2C_STATE_READY;
}