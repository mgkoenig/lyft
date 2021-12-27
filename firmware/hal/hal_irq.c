/*
 * hal_irq.c
 *
 * Created: 19/11/2021 12:49:04
 *  Author: Sire
 */ 

#include "hal_irq.h"

void hal_irq_init(enum irq_source source, void (*cb_fct))
{
	struct extint_chan_conf	pin_cfg_extint;		
	extint_chan_get_config_defaults(&pin_cfg_extint);
	
	if (source == IRQ_SOURCE_KEYPAD)
	{		
		pin_cfg_extint.gpio_pin           = IRQ_PIN_KEYPAD;
		pin_cfg_extint.gpio_pin_mux       = IRQ_MUX_KEYPAD;
		pin_cfg_extint.gpio_pin_pull      = EXTINT_PULL_UP;
		pin_cfg_extint.detection_criteria = EXTINT_DETECT_FALLING;
		
		extint_chan_set_config(IRQ_EIC_KEYPAD, &pin_cfg_extint);
		extint_register_callback(cb_fct, IRQ_EIC_KEYPAD, EXTINT_CALLBACK_TYPE_DETECT);
		extint_chan_enable_callback(IRQ_EIC_KEYPAD, EXTINT_CALLBACK_TYPE_DETECT);
	}	
}

void hal_irq_enable(enum irq_source source)
{
	switch (source)
	{
		case IRQ_SOURCE_KEYPAD: extint_chan_enable_callback(IRQ_EIC_KEYPAD, EXTINT_CALLBACK_TYPE_DETECT); break;
		default: break;
	}
	
}

void hal_irq_disable(enum irq_source source)
{
	switch (source)
	{
		case IRQ_SOURCE_KEYPAD: extint_chan_disable_callback(IRQ_EIC_KEYPAD, EXTINT_CALLBACK_TYPE_DETECT); break;
		default: break;
	}	
}
