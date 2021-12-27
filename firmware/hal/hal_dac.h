/*
 * hal_dac.h
 *
 * Created: 10/12/2021 11:49:29
 *  Author: Sire
 */ 


#ifndef HAL_DAC_H_
#define HAL_DAC_H_

#include <asf.h>


#define DAC_VOLTAGE_MAX			1000		// 1000mV max output voltage
#define DAC_RESOLUTION			0x3FF		// 10 bit DAC


void hal_dac_init(void);
void hal_dac_setVoltage(uint16_t volt);



#endif /* HAL_DAC_H_ */