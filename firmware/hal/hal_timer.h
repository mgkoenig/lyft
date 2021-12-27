/*
 * hal_timer.h
 *
 * Created: 16/11/2021 21:02:15
 *  Author: Sire
 */ 


#ifndef HAL_TIMER_H_
#define HAL_TIMER_H_

#include <asf.h>


enum timer_instance {
	TIMER_DISPLAY,
	TIMER_KEYPAD,
	TIMER_MOTOR,
	TIMER_AUDIO_DURATION,
	TIMER_AUDIO_FREQUENCY
};

/*************************************************************************
 * \brief timer init function
 *
 * Initializes a timer to  its assigned HW timer module. 
 *
 * \param timer which of the used timers to initialize
 * \param period the period in milliseconds when an overflow interrupt shall happen
 * \param cb_func the callback function for the interrupt as function pointer
 ************************************************************************/
void hal_timer_init(enum timer_instance timer, uint16_t period, void (*cb_fct));
void hal_timer_start(enum timer_instance timer);
void hal_timer_stop(enum timer_instance timer);
void hal_timer_disable(enum timer_instance timer);
void hal_timer_setCountValue(enum timer_instance timer, uint16_t value);
void hal_timer_setPeriod(enum timer_instance timer, uint16_t period);


#endif /* HAL_TIMER_H_ */