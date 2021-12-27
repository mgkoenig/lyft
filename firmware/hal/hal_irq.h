/*
 * hal_irq.h
 *
 * Created: 19/11/2021 12:49:12
 *  Author: Sire
 */ 


#ifndef HAL_IRQ_H_
#define HAL_IRQ_H_

#include <asf.h>
#include "conf_board.h"


enum irq_source {
	IRQ_SOURCE_KEYPAD,
	IRQ_MAX	
};


void hal_irq_init(enum irq_source source, void (*cb_fct));
void hal_irq_enable(enum irq_source source);
void hal_irq_disable(enum irq_source source);


#endif /* HAL_IRQ_H_ */