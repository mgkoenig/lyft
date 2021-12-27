/*
 * hal_gpio.h
 *
 * Created: 25/12/2021 14:39:29
 *  Author: Sire
 */ 


#ifndef HAL_GPIO_H_
#define HAL_GPIO_H_

#include <asf.h>
#include "conf_board.h"


enum gpio_pin {
	PIN_LED,
	PIN_SAFETY,	
	PIN_MAX
};


void hal_gpio_init(enum gpio_pin pin);
void hal_gpio_setPinState(enum gpio_pin pin, bool state);
bool hal_gpio_getPinState(enum gpio_pin pin);


#endif /* HAL_GPIO_H_ */