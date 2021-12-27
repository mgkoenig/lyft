/*
 * hal_delay.h
 *
 * Created: 16/11/2021 21:19:49
 *  Author: Sire
 */ 


#ifndef HAL_DELAY_H_
#define HAL_DELAY_H_

#include <asf.h>


void hal_delay_init(void);
void hal_delay_sec(uint32_t delay);
void hal_delay_msec(uint32_t delay);
void hal_delay_usec(uint32_t delay);


#endif /* HAL_DELAY_H_ */