/*
 * hal_timer.c
 *
 * Created: 16/11/2021 21:02:06
 *  Author: Sire
 */ 
#include "hal_timer.h"

struct tc_module				timer_inst_display;
struct tc_module				timer_inst_motor;
struct tc_module				timer_inst_audio_duration;
struct tcc_module				timer_inst_keypad;
struct tcc_module				timer_inst_audio_frequency;

void hal_timer_init(enum timer_instance timer, uint16_t period, void (*cb_fct))
{
	uint16_t cc_reg;
	struct tc_config	tc_timer_conf;	
	struct tcc_config	tcc_timer_conf;
	
	if (timer == TIMER_DISPLAY)
	{
		tc_get_config_defaults(&tc_timer_conf);
		
		cc_reg = ((1000 * period) / 32);						// register value according to timer config
		tc_timer_conf.counter_size								= TC_COUNTER_SIZE_16BIT;
		tc_timer_conf.clock_source								= GCLK_GENERATOR_3;
		tc_timer_conf.clock_prescaler							= TC_CLOCK_PRESCALER_DIV256;
		tc_timer_conf.counter_16_bit.compare_capture_channel[0]	= cc_reg;		// granularity of 1 sec
		tc_init(&timer_inst_display, TC3, &tc_timer_conf);
		tc_enable(&timer_inst_display);
		tc_stop_counter(&timer_inst_display);
		
		tc_register_callback(&timer_inst_display, cb_fct, TC_CALLBACK_CC_CHANNEL0);
		tc_enable_callback(&timer_inst_display, TC_CALLBACK_CC_CHANNEL0);
	}
		
	if (timer == TIMER_KEYPAD)
	{
		tcc_get_config_defaults(&tcc_timer_conf, TCC1);
				
		cc_reg = (period * 125);
		tcc_timer_conf.counter.period = cc_reg;
		tcc_timer_conf.counter.clock_source = GCLK_GENERATOR_3;
		tcc_timer_conf.counter.clock_prescaler = TCC_CLOCK_PRESCALER_DIV64;			// granularity of 1ms
		tcc_timer_conf.compare.wave_generation = TCC_WAVE_GENERATION_NORMAL_FREQ;
		
		tcc_init(&timer_inst_keypad, TCC1, &tcc_timer_conf);
		tcc_enable(&timer_inst_keypad);
		tcc_stop_counter(&timer_inst_keypad);
		
		tcc_register_callback(&timer_inst_keypad,  cb_fct, (enum tcc_callback)(TCC_CALLBACK_CHANNEL_0));
		tcc_enable_callback(&timer_inst_keypad, (enum tcc_callback)(TCC_CALLBACK_CHANNEL_0));		
	}
			
	if (timer == TIMER_MOTOR)
	{
		tc_get_config_defaults(&tc_timer_conf);
		
		cc_reg = (period * 125);								// register value according to timer config
		tc_timer_conf.counter_size								= TC_COUNTER_SIZE_16BIT;
		tc_timer_conf.clock_source								= GCLK_GENERATOR_3;
		tc_timer_conf.clock_prescaler							= TC_CLOCK_PRESCALER_DIV64;
		tc_timer_conf.counter_16_bit.compare_capture_channel[0]	= cc_reg;		// granularity of 1 ms
		tc_init(&timer_inst_motor, TC5, &tc_timer_conf);
		tc_enable(&timer_inst_motor);
		tc_stop_counter(&timer_inst_motor);
		
		tc_register_callback(&timer_inst_motor, cb_fct, TC_CALLBACK_CC_CHANNEL0);
		tc_enable_callback(&timer_inst_motor, TC_CALLBACK_CC_CHANNEL0);
	}
	
	if (timer == TIMER_AUDIO_DURATION)
	{
		tc_get_config_defaults(&tc_timer_conf);
		
		cc_reg = (period * 125);								// register value according to timer config
		tc_timer_conf.counter_size								= TC_COUNTER_SIZE_16BIT;
		tc_timer_conf.clock_source								= GCLK_GENERATOR_3;
		tc_timer_conf.clock_prescaler							= TC_CLOCK_PRESCALER_DIV64;
		tc_timer_conf.counter_16_bit.compare_capture_channel[0]	= cc_reg;		// granularity of 1 ms
		tc_init(&timer_inst_audio_duration, TC4, &tc_timer_conf);
		tc_enable(&timer_inst_audio_duration);
		
		tc_register_callback(&timer_inst_audio_duration, cb_fct, TC_CALLBACK_CC_CHANNEL0);
		tc_enable_callback(&timer_inst_audio_duration, TC_CALLBACK_CC_CHANNEL0);
	}
	
	if (timer == TIMER_AUDIO_FREQUENCY)
	{
		tcc_get_config_defaults(&tcc_timer_conf, TCC0);
		
		cc_reg = (period / 2);
		tcc_timer_conf.counter.period = cc_reg;
		tcc_timer_conf.counter.clock_source = GCLK_GENERATOR_3;
		tcc_timer_conf.counter.clock_prescaler = TCC_CLOCK_PRESCALER_DIV8;			// granularity of 1us
		tcc_timer_conf.compare.wave_generation = TCC_WAVE_GENERATION_NORMAL_FREQ;
		tcc_timer_conf.compare.match[0] = (cc_reg/2);		// always 50% duty cycle

		tcc_timer_conf.pins.enable_wave_out_pin[0] = true;
		tcc_timer_conf.pins.wave_out_pin[0]        = AUDIO_WAVE_GENERATOR_PIN;
		tcc_timer_conf.pins.wave_out_pin_mux[0]    = AUDIO_WAVE_GENERATOR_PINMUX;

		tcc_init(&timer_inst_audio_frequency, TCC0, &tcc_timer_conf);
		tcc_enable(&timer_inst_audio_frequency);
	}
}

void hal_timer_start(enum timer_instance timer)
{
	switch (timer)
	{
		case TIMER_DISPLAY: tc_start_counter(&timer_inst_display); break;
		case TIMER_KEYPAD: tcc_restart_counter(&timer_inst_keypad); break;
		case TIMER_MOTOR: tc_start_counter(&timer_inst_motor); break;
		case TIMER_AUDIO_DURATION: tc_start_counter(&timer_inst_audio_duration); break;
		case TIMER_AUDIO_FREQUENCY: tcc_restart_counter(&timer_inst_audio_frequency); break;
		default: break;
	}
}

void hal_timer_stop(enum timer_instance timer)
{
	switch (timer)
	{
		case TIMER_DISPLAY: tc_stop_counter(&timer_inst_display); break;
		case TIMER_KEYPAD: tcc_stop_counter(&timer_inst_keypad); break;
		case TIMER_MOTOR: tc_stop_counter(&timer_inst_motor); break;
		case TIMER_AUDIO_DURATION: tc_stop_counter(&timer_inst_audio_duration); break;
		case TIMER_AUDIO_FREQUENCY: tcc_stop_counter(&timer_inst_audio_frequency); break;
		default: break;
	}
}

void hal_timer_disable(enum timer_instance timer)
{
	switch (timer)
	{
		case TIMER_DISPLAY: tc_disable(&timer_inst_display); break;
		case TIMER_KEYPAD: tcc_disable(&timer_inst_keypad); break;
		case TIMER_MOTOR: tc_disable(&timer_inst_motor); break;
		case TIMER_AUDIO_DURATION: tc_disable(&timer_inst_audio_duration); break;
		case TIMER_AUDIO_FREQUENCY: tcc_disable(&timer_inst_audio_frequency); break;
		default: break;
	}
}

void hal_timer_setCountValue(enum timer_instance timer, uint16_t value)
{
	switch (timer)
	{
		case TIMER_DISPLAY: tc_set_count_value(&timer_inst_display, value); break;
		case TIMER_KEYPAD: tcc_set_count_value(&timer_inst_keypad, value); break;
		case TIMER_MOTOR: tc_set_count_value(&timer_inst_motor, value); break;
		case TIMER_AUDIO_DURATION: tc_set_count_value(&timer_inst_audio_duration, value); break;
		case TIMER_AUDIO_FREQUENCY: tcc_set_count_value(&timer_inst_audio_frequency, value); break; 
		default: break;
	}
}

void hal_timer_setPeriod(enum timer_instance timer, uint16_t period)
{
	switch (timer)
	{
		case TIMER_DISPLAY: tc_set_top_value(&timer_inst_display, (1000 * period) / 32); break;
		case TIMER_KEYPAD: tcc_set_top_value(&timer_inst_keypad, (period * 125)); break;
		case TIMER_MOTOR: tc_set_top_value(&timer_inst_motor, (period * 125)); break; 
		case TIMER_AUDIO_DURATION: tc_set_top_value(&timer_inst_audio_duration, (period * 125)); break; 
		case TIMER_AUDIO_FREQUENCY: 
			tcc_set_top_value(&timer_inst_audio_frequency, (period / 2));
			tcc_set_compare_value(&timer_inst_audio_frequency, 0, (period / 4));
			break;
		default: break;
	}
}
