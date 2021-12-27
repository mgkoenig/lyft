/*
 * hal_dac.c
 *
 * Created: 10/12/2021 11:49:17
 *  Author: Sire
 */ 

#include "hal_dac.h"


struct dac_module		dac_instance;


void hal_dac_init(void)
{
	struct dac_config		config_dac;
	struct dac_chan_config	config_dac_chan;
	
	dac_get_config_defaults(&config_dac);
	dac_init(&dac_instance, DAC, &config_dac);
	dac_enable(&dac_instance);
	
	dac_chan_get_config_defaults(&config_dac_chan);
	dac_chan_set_config(&dac_instance, DAC_CHANNEL_0, &config_dac_chan);
	dac_chan_enable(&dac_instance, DAC_CHANNEL_0);	
	
	dac_chan_write(&dac_instance, DAC_CHANNEL_0, 0);
}

void hal_dac_setVoltage(uint16_t volt)
{
	uint16_t	dac_value;
	
	if (volt <= DAC_VOLTAGE_MAX)
	{
		dac_value = ((volt * DAC_RESOLUTION) / DAC_VOLTAGE_MAX);
		dac_chan_write(&dac_instance, DAC_CHANNEL_0, dac_value);
	}
}
