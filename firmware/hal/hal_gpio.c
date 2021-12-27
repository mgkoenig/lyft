/*
 * hal_gpio.c
 *
 * Created: 25/12/2021 14:39:17
 *  Author: Sire
 */ 

#include "hal_gpio.h"

void hal_gpio_init(enum gpio_pin pin)
{
	struct port_config config_port_pin;	
	port_get_config_defaults(&config_port_pin);
	
	if (pin == PIN_LED)
	{
		config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(GPIO_PIN_LED, &config_port_pin);
	}
	
	if (pin == PIN_SAFETY)
	{
		config_port_pin.direction = PORT_PIN_DIR_INPUT;
		config_port_pin.input_pull = PORT_PIN_PULL_UP;		
		port_pin_set_config(GPIO_PIN_SAFETY, &config_port_pin);
	}	
}

void hal_gpio_setPinState(enum gpio_pin pin, bool state)
{	
	switch (pin)
	{
		case PIN_LED: port_pin_set_output_level(GPIO_PIN_LED, state); break;
		default: break;
	}
}

bool hal_gpio_getPinState(enum gpio_pin pin)
{
	bool state;
	
	switch (pin)
	{
		case PIN_LED: state = port_pin_get_input_level(GPIO_PIN_LED); break;
		case PIN_SAFETY: state = port_pin_get_input_level(GPIO_PIN_SAFETY); break;
		default: state = false;
	}
	
	return state;
}