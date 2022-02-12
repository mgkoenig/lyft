/*
 * lyft.c
 *
 * Created: 13/11/2021 12:33:28
 *  Author: Matthias Koenig
 */ 

#include "lyft.h"


static void lyft_menu_enter(void);
static void lyft_menu_leave(void);
static void lyft_menu_nextPage(void);
static void lyft_menu_prevPage(void);
static void lyft_menu_action(enum keypad_layout button);

static void lyft_config_save(void);
static void lyft_config_load(void);
static void lyft_config_setPosition(enum keypad_layout button);
static void lyft_config_setUnit(enum display_unit unit);
static void lyft_config_setVolume(enum audio_volume volume);
static void lyft_config_setBrightness(enum display_brightness bri);
static void lyft_config_setScreentime(enum lyft_screentime screentime);
static void lyft_config_incOffset(void);
static void lyft_config_decOffset(void);

static void lyft_cb_display(void);
static void lyft_cb_keypad(void);
static void lyft_cb_motor(void);
static void lyft_irq_keypad(void);


bool						irq_keypad_suppress;
uint16_t					time_index;
struct lyft_config			config;

struct keypad_button		keypad[BUTTON_MAX];
bool						keypad_poll;
uint8_t						keypad_poll_delay;
uint16_t					temp_memory_position;
enum keypad_layout			temp_memory_button;
enum keypad_layout			prev_button;

enum display_state			display_screen;
enum display_state			previous_screen;
bool						display_refresh;
uint8_t						display_runtime;

uint16_t					desk_position;
uint16_t					desk_height;
bool						motor_service;
enum motor_event			motor_action;
enum motor_mode				prev_motor_state;


void lyft_init(void)
{
	hal_nvm_init();
	hal_delay_init();	
	
	lyft_config_load();
	
	display_init();
	display_setBrightness(config.brightness);
	previous_screen = DISPLAY_OFF;
	display_screen = DISPLAY_OFF;
	display_refresh = false;
	display_runtime = 0;
	
	keypad_init(keypad);
	keypad_poll = false;
	keypad_poll_delay = KEYPAD_POLL_DELAY_NONE;
	prev_button = BUTTON_MAX;
	
	audio_init();
	audio_setVolume(config.volume);
	
	hal_timer_init(TIMER_DISPLAY, DISPLAY_DELAY_INTERVAL, lyft_cb_display);
	hal_timer_init(TIMER_KEYPAD, KEYPAD_POLL_INTERVAL, lyft_cb_keypad);
	hal_timer_init(TIMER_MOTOR, MOTOR_SERVICE_INTERVAL, lyft_cb_motor);
	time_index = 0;
	
	irq_keypad_suppress = false;
	hal_irq_init(IRQ_SOURCE_KEYPAD, lyft_irq_keypad);

	hal_gpio_init(PIN_SAFETY);
	hal_gpio_init(PIN_LED);
	hal_gpio_setPinState(PIN_LED, false);
	
	motor_init();
	motor_service = false;
	motor_action = MOTOR_EVENT_STOP;
	prev_motor_state = MOTOR_MODE_NORMAL;
	
	audio_play(TITLE_STARTUP);
	hal_delay_msec(400);		// let's finish the song
	
	hal_timer_start(TIMER_MOTOR);
}

void lyft_run(void)
{
	bool				dot_point; 
	bool				safety_pin;
	uint8_t				num_pressed_buttons;
	uint16_t			prev_height;
	static uint8_t		motor_drift;
	enum motor_mode		motor_state;

	if (keypad_poll == true)
	{
		num_pressed_buttons = keypad_read(keypad, time_index);
		safety_pin = !(hal_gpio_getPinState(PIN_SAFETY));		// safety pin is low active
		
		if (num_pressed_buttons > 0)
		{
			display_runtime = 0;
			hal_timer_start(TIMER_DISPLAY); 	
			
			if (safety_pin == false)
			{	
				if (display_screen == DISPLAY_OFF) {
					display_screen = DISPLAY_DESKPOSITION;
				}
				else if (display_screen != DISPLAY_LOCKED) {
					display_screen = DISPLAY_LOCKED;
				}
				
				motor_action = MOTOR_EVENT_STOP;
				keypad_poll_delay = KEYPAD_POLL_DELAY_LONG;					
			}		
			else 
			{
				if (num_pressed_buttons == 1)
				{
					if (keypad[BUTTON_1].state == BUTTON_STATE_PRESSED)
					{
						if ((display_screen & DISPLAY_MENU) == DISPLAY_MENU) {
							lyft_menu_action(BUTTON_1);
						} else if (display_screen == DISPLAY_OFF) {
							display_screen = DISPLAY_DESKPOSITION;
						}
					
						keypad_poll_delay = KEYPAD_POLL_DELAY_DEFAULT;
					}
				
					if (keypad[BUTTON_1].state == BUTTON_STATE_CONT_PRESS)
					{											
						if (display_screen == DISPLAY_DESKPOSITION)
						{						
							if ((MOTOR_POSITION_MIN < keypad[BUTTON_1].memory_position) && (keypad[BUTTON_1].memory_position < MOTOR_POSITION_MAX))
							{
								if (desk_position < (keypad[BUTTON_1].memory_position - MOTOR_SAFETY_MARGIN)) {
									motor_action = MOTOR_EVENT_MOVE_UP;
								} else if (desk_position > (keypad[BUTTON_1].memory_position + MOTOR_SAFETY_MARGIN)) {
									motor_action = MOTOR_EVENT_MOVE_DOWN;
								} else {
									motor_action = MOTOR_EVENT_STOP;
									if (prev_button != BUTTON_1) {
										audio_play(TITLE_DOUBLE_CLICK);
										prev_button = BUTTON_1;
									}								
								}
							} else {
								prev_button = BUTTON_1;
								display_screen = DISPLAY_NO_POSITION;
							}
						}				
					}
				
					if (keypad[BUTTON_2].state == BUTTON_STATE_PRESSED)
					{
						if ((display_screen & DISPLAY_MENU) == DISPLAY_MENU) {
							lyft_menu_action(BUTTON_2);
						} else if (display_screen == DISPLAY_OFF) {
							display_screen = DISPLAY_DESKPOSITION;
						}
					
						keypad_poll_delay = KEYPAD_POLL_DELAY_DEFAULT;
					}
				
					if (keypad[BUTTON_2].state == BUTTON_STATE_CONT_PRESS)
					{
						if (display_screen == DISPLAY_DESKPOSITION)
						{
							if ((MOTOR_POSITION_MIN < keypad[BUTTON_2].memory_position) && (keypad[BUTTON_2].memory_position < MOTOR_POSITION_MAX))
							{
								if (desk_position < (keypad[BUTTON_2].memory_position - MOTOR_SAFETY_MARGIN)) {
									motor_action = MOTOR_EVENT_MOVE_UP;
								} else if (desk_position > (keypad[BUTTON_2].memory_position + MOTOR_SAFETY_MARGIN)) {
									motor_action = MOTOR_EVENT_MOVE_DOWN;
								} else {
									motor_action = MOTOR_EVENT_STOP;
									if (prev_button != BUTTON_2) {
										audio_play(TITLE_DOUBLE_CLICK);
										prev_button = BUTTON_2;
									}
								}
							} else {
								prev_button = BUTTON_2;
								display_screen = DISPLAY_NO_POSITION;
							}					
						}
					}
				
					if (keypad[BUTTON_3].state == BUTTON_STATE_PRESSED)
					{
						if ((display_screen & DISPLAY_MENU) == DISPLAY_MENU) {
							lyft_menu_action(BUTTON_3);
						} else if (display_screen == DISPLAY_OFF) {
							display_screen = DISPLAY_DESKPOSITION;
						}
					
						keypad_poll_delay = KEYPAD_POLL_DELAY_DEFAULT;
					}
				
					if (keypad[BUTTON_3].state == BUTTON_STATE_CONT_PRESS)
					{
						if (display_screen == DISPLAY_DESKPOSITION)
						{
							if ((MOTOR_POSITION_MIN < keypad[BUTTON_3].memory_position) && (keypad[BUTTON_3].memory_position < MOTOR_POSITION_MAX))
							{
								if (desk_position < (keypad[BUTTON_3].memory_position - MOTOR_SAFETY_MARGIN)) {
									motor_action = MOTOR_EVENT_MOVE_UP;
								} else if (desk_position > (keypad[BUTTON_3].memory_position + MOTOR_SAFETY_MARGIN)) {
									motor_action = MOTOR_EVENT_MOVE_DOWN;
								} else {
									motor_action = MOTOR_EVENT_STOP;
									if (prev_button != BUTTON_3) {
										audio_play(TITLE_DOUBLE_CLICK);
										prev_button = BUTTON_3;
									}								
								}
							} else {
								prev_button = BUTTON_3;
								display_screen = DISPLAY_NO_POSITION;
							}
						}					
					}
				
					if (keypad[BUTTON_4].state == BUTTON_STATE_PRESSED)
					{
						if ((display_screen & DISPLAY_MENU) == DISPLAY_MENU) {
							lyft_menu_action(BUTTON_4);
						} else if (display_screen == DISPLAY_OFF) {
							display_screen = DISPLAY_DESKPOSITION;
						}
					
						keypad_poll_delay = KEYPAD_POLL_DELAY_DEFAULT;
					}
				
					if (keypad[BUTTON_4].state == BUTTON_STATE_CONT_PRESS)
					{
						if (display_screen == DISPLAY_DESKPOSITION)
						{
							if ((MOTOR_POSITION_MIN < keypad[BUTTON_4].memory_position) && (keypad[BUTTON_4].memory_position < MOTOR_POSITION_MAX))
							{
								if (desk_position < (keypad[BUTTON_4].memory_position - MOTOR_SAFETY_MARGIN)) {
									motor_action = MOTOR_EVENT_MOVE_UP;
								} else if (desk_position > (keypad[BUTTON_4].memory_position + MOTOR_SAFETY_MARGIN)) {
									motor_action = MOTOR_EVENT_MOVE_DOWN;
								} else {
									motor_action = MOTOR_EVENT_STOP;
									if (prev_button != BUTTON_4) {
										audio_play(TITLE_DOUBLE_CLICK);
										prev_button = BUTTON_4;
									}								
								}
							} else {
								prev_button = BUTTON_4;
								display_screen = DISPLAY_NO_POSITION;
							}
						}
					}
				
					if (keypad[BUTTON_UP].state == BUTTON_STATE_PRESSED)
					{
						if ((display_screen & DISPLAY_MENU) == DISPLAY_MENU) {
							lyft_menu_nextPage();
						} else if (display_screen == DISPLAY_OFF) {
							display_screen = DISPLAY_DESKPOSITION;
						}
					
						keypad_poll_delay = KEYPAD_POLL_DELAY_DEFAULT;
					}
				
					if (keypad[BUTTON_UP].state == BUTTON_STATE_CONT_PRESS)
					{
						if (display_screen == DISPLAY_DESKPOSITION) 
						{
							if (desk_position < (MOTOR_POSITION_MAX - MOTOR_SAFETY_MARGIN)) {
								motor_action = MOTOR_EVENT_MOVE_UP;
							} else {
								audio_play(TITLE_LIMIT);
								motor_action = MOTOR_EVENT_STOP;
								prev_button = BUTTON_UP;
								display_screen = DISPLAY_LIMIT;
							}	
						}
								
						keypad_poll_delay = KEYPAD_POLL_DELAY_NONE;
					}
						
					if (keypad[BUTTON_DOWN].state == BUTTON_STATE_PRESSED)
					{
						if ((display_screen & DISPLAY_MENU) == DISPLAY_MENU) {
							lyft_menu_prevPage();
						} else if (display_screen == DISPLAY_OFF) {
							display_screen = DISPLAY_DESKPOSITION;
						}

						keypad_poll_delay = KEYPAD_POLL_DELAY_DEFAULT;
					}
				
					if (keypad[BUTTON_DOWN].state == BUTTON_STATE_CONT_PRESS)
					{
						if (display_screen == DISPLAY_DESKPOSITION) 					
						{
							if (desk_position > (MOTOR_POSITION_MIN + MOTOR_SAFETY_MARGIN)) {
								motor_action = MOTOR_EVENT_MOVE_DOWN;
							} else {
								audio_play(TITLE_LIMIT);
								motor_action = MOTOR_EVENT_STOP;
								prev_button = BUTTON_DOWN;
								display_screen = DISPLAY_LIMIT;
							}
						}
					
						keypad_poll_delay = KEYPAD_POLL_DELAY_NONE;
					}			
				}
			
				else if (num_pressed_buttons == 2)
				{
					motor_action = MOTOR_EVENT_STOP;				
				
					if ((keypad[BUTTON_UP].state == BUTTON_STATE_CONT_PRESS) && (keypad[BUTTON_DOWN].state == BUTTON_STATE_CONT_PRESS))
					{
						uint16_t btn_hold_up = keypad_holdTime(time_index, keypad[BUTTON_UP].trigger_time);
						uint16_t btn_hold_down = keypad_holdTime(time_index, keypad[BUTTON_DOWN].trigger_time);

						if ((btn_hold_up > LYFT_MENU_ENTER_DELAY) && (btn_hold_down > LYFT_MENU_ENTER_DELAY))
						{
							if (display_screen == DISPLAY_DESKPOSITION)
							{
								lyft_menu_enter();
							}
						}
					}
				
					if ((keypad[BUTTON_1].state == BUTTON_STATE_CONT_PRESS) && (keypad[BUTTON_2].state == BUTTON_STATE_CONT_PRESS))
					{
						uint16_t btn_hold_1 = keypad_holdTime(time_index, keypad[BUTTON_1].trigger_time);
						uint16_t btn_hold_2 = keypad_holdTime(time_index, keypad[BUTTON_2].trigger_time);

						if ((btn_hold_1 > LYFT_CALIBRATION_START_DELAY) && (btn_hold_2 > LYFT_CALIBRATION_START_DELAY))
						{
							if (display_screen == DISPLAY_DESKPOSITION)
							{
								motor_action = MOTOR_EVENT_CALIBRATE;
							}
							
							keypad_poll_delay = KEYPAD_POLL_DELAY_DEFAULT;
						}
					}
				}
			} // end-if safety_pin
		}
		else // (num_pressed_buttons > 0)
		{
			motor_action = MOTOR_EVENT_STOP;
			
			if (safety_pin == false)
			{
				if ((display_screen & DISPLAY_MENU) == DISPLAY_MENU) {
					lyft_menu_leave();
				}
				
				display_screen = DISPLAY_DESKPOSITION;
				keypad_poll_delay = KEYPAD_POLL_DELAY_LONG;
			} 		
			
			if (prev_button < BUTTON_MAX)
			{
				if (keypad[prev_button].state == BUTTON_STATE_RELEASED)
				{
					prev_button = BUTTON_MAX;
					display_screen = DISPLAY_DESKPOSITION;
				}				
			}
		}		
		
		keypad_poll = false;
	}
	
	if (motor_service)
	{			
		motor_state = motor_run(motor_action);

		prev_height = desk_height;
		desk_position = motor_getPosition();
		motor_drift = motor_getDrift();
			
		if (config.unit == DISPLAY_UNIT_CENTIMETER) {
			desk_height = motor_getHeightCentimeter(config.offset);
		} else {
			desk_height = motor_getHeightInch(config.offset);
		}
			
		if (prev_height != desk_height) {
			display_refresh = true;
		}
		
		if ((prev_motor_state == MOTOR_MODE_NORMAL) && (motor_state == MOTOR_MODE_CRITICAL)) {
			audio_play(TITLE_ALARM);
			display_screen = DISPLAY_CRITICAL;
			display_runtime = 0;			
		}
		else if (motor_state == MOTOR_MODE_CRITICAL) {
			display_screen = DISPLAY_CRITICAL;
			display_runtime = 0;
		}
		else if (motor_state == MOTOR_MODE_CALIBRATION) {
			display_screen = DISPLAY_CALIBRATION;
			display_runtime = 0;
		}
		else if ((prev_motor_state == MOTOR_MODE_CRITICAL) && (motor_state == MOTOR_MODE_NORMAL)) {
			display_screen = DISPLAY_DESKPOSITION;
			display_runtime = 0;
		}
		else if ((prev_motor_state == MOTOR_MODE_CALIBRATION) && (motor_state == MOTOR_MODE_NORMAL)) {
			display_screen = DISPLAY_DESKPOSITION;
			display_runtime = 0;
		}
		
		prev_motor_state = motor_state;
		motor_service = false;	
		hal_timer_start(TIMER_MOTOR);	
	}
	
	if ((previous_screen != display_screen) || (display_refresh == true))
	{			
		if (config.unit == DISPLAY_UNIT_CENTIMETER) {
			dot_point = false;
		} else {
			dot_point = true;
		}
		
		switch (display_screen)
		{
			case DISPLAY_OFF: display_clear(); break;			
			case DISPLAY_LIMIT: display_show_end(); break;
			case DISPLAY_LOCKED: display_show_locked(); break;
			case DISPLAY_NO_POSITION: display_show_noPosition(); break; 
			case DISPLAY_DESKPOSITION: display_show_deskHeight(desk_height, dot_point); break;
			case DISPLAY_CALIBRATION: display_show_calibration(); break;
			case DISPLAY_CRITICAL: display_show_critical(); break; 
			case DISPLAY_MENU_UNIT: display_show_menuUnit(config.unit); break;
			case DISPLAY_MENU_DRIFT: display_show_menuDrift(motor_drift); break;
			case DISPLAY_MENU_OFFSET: display_show_menuOffset(config.offset); break;
			case DISPLAY_MENU_POSITION: display_show_menuPosition(temp_memory_button); break;
			case DISPLAY_MENU_VOLUME: display_show_menuVolume(config.volume); break;
			case DISPLAY_MENU_BRIGHTNESS: display_show_menuBrightness(config.brightness); break;
			case DISPLAY_MENU_SCREENTIME: display_show_menuScreentime((uint8_t) config.screentime); break;
			default: display_show_error(); break;
		}
		
		previous_screen = display_screen;
		display_refresh = false;
	}	
}

/************************************************************************/
/* MENU CONFIGURATION                                                   */
/************************************************************************/
static void lyft_menu_enter(void)
{
	temp_memory_button = BUTTON_MAX;
	temp_memory_position = 0;
	display_screen = DISPLAY_MENU_POSITION;
}

static void lyft_menu_leave(void)
{
	display_screen = DISPLAY_DESKPOSITION; 
	display_runtime = 0;
}

static void lyft_menu_nextPage(void)
{
	switch (display_screen)
	{
		case DISPLAY_MENU_POSITION: display_screen = DISPLAY_MENU_BRIGHTNESS; break;
		case DISPLAY_MENU_BRIGHTNESS: display_screen = DISPLAY_MENU_VOLUME; break;
		case DISPLAY_MENU_VOLUME: display_screen = DISPLAY_MENU_SCREENTIME; break;
		case DISPLAY_MENU_SCREENTIME: display_screen = DISPLAY_MENU_UNIT; break;
		case DISPLAY_MENU_UNIT: display_screen = DISPLAY_MENU_OFFSET; break;
		case DISPLAY_MENU_OFFSET: display_screen = DISPLAY_MENU_DRIFT; break; 
		case DISPLAY_MENU_DRIFT: display_screen = DISPLAY_MENU_POSITION; break;
		default: display_screen = DISPLAY_ERROR; break;
	}
}

static void lyft_menu_prevPage(void)
{
	switch (display_screen)
	{
		case DISPLAY_MENU_POSITION: display_screen = DISPLAY_MENU_DRIFT; break;
		case DISPLAY_MENU_BRIGHTNESS: display_screen = DISPLAY_MENU_POSITION; break;
		case DISPLAY_MENU_VOLUME: display_screen = DISPLAY_MENU_BRIGHTNESS; break;
		case DISPLAY_MENU_SCREENTIME: display_screen = DISPLAY_MENU_VOLUME; break;
		case DISPLAY_MENU_UNIT: display_screen = DISPLAY_MENU_SCREENTIME; break;
		case DISPLAY_MENU_OFFSET: display_screen = DISPLAY_MENU_UNIT; break;
		case DISPLAY_MENU_DRIFT: display_screen = DISPLAY_MENU_OFFSET; break;
		default: display_screen = DISPLAY_ERROR; break;
	}
}

static void lyft_menu_action(enum keypad_layout button)
{	
	if (button == BUTTON_1)
	{
		switch (display_screen)
		{
			case DISPLAY_MENU_POSITION: lyft_config_setPosition(BUTTON_1); break;
			case DISPLAY_MENU_UNIT: lyft_config_setUnit(DISPLAY_UNIT_CENTIMETER); break;
			case DISPLAY_MENU_DRIFT: break;
			case DISPLAY_MENU_OFFSET: lyft_config_decOffset(); break;
			case DISPLAY_MENU_VOLUME: lyft_config_setVolume(AUDIO_VOLUME_OFF); break;
			case DISPLAY_MENU_BRIGHTNESS: lyft_config_setBrightness(DISPLAY_BRIGHTNESS_25); break;
			case DISPLAY_MENU_SCREENTIME: lyft_config_setScreentime(LYFT_SCREENTIME_5SEC); break;
			default: display_show_error(); break;
		}
	}
	else if (button == BUTTON_2)
	{
		switch (display_screen)
		{
			case DISPLAY_MENU_POSITION: lyft_config_setPosition(BUTTON_2); break;
			case DISPLAY_MENU_UNIT: lyft_config_setUnit(DISPLAY_UNIT_INCH); break;
			case DISPLAY_MENU_DRIFT: break;
			case DISPLAY_MENU_OFFSET: lyft_config_incOffset(); break;
			case DISPLAY_MENU_VOLUME: lyft_config_setVolume(AUDIO_VOLUME_LOW); break;
			case DISPLAY_MENU_BRIGHTNESS: lyft_config_setBrightness(DISPLAY_BRIGHTNESS_50); break;
			case DISPLAY_MENU_SCREENTIME: lyft_config_setScreentime(LYFT_SCREENTIME_10SEC); break;
			default: display_show_error(); break;
		}
	}
	else if (button == BUTTON_3)
	{
		switch (display_screen)
		{
			case DISPLAY_MENU_POSITION: lyft_config_setPosition(BUTTON_3); break;
			case DISPLAY_MENU_UNIT: break;
			case DISPLAY_MENU_DRIFT: break;
			case DISPLAY_MENU_OFFSET: break;
			case DISPLAY_MENU_VOLUME: lyft_config_setVolume(AUDIO_VOLUME_MID); break;
			case DISPLAY_MENU_BRIGHTNESS: lyft_config_setBrightness(DISPLAY_BRIGHTNESS_75); break;
			case DISPLAY_MENU_SCREENTIME: lyft_config_setScreentime(LYFT_SCREENTIME_15SEC); break;
			default: display_show_error(); break;
		}
	}
	else if (button == BUTTON_4)
	{
		switch (display_screen)
		{
			case DISPLAY_MENU_POSITION: lyft_config_setPosition(BUTTON_4); break;
			case DISPLAY_MENU_UNIT: break;
			case DISPLAY_MENU_DRIFT: break;
			case DISPLAY_MENU_OFFSET: break;
			case DISPLAY_MENU_VOLUME: lyft_config_setVolume(AUDIO_VOLUME_HIGH); break;
			case DISPLAY_MENU_BRIGHTNESS: lyft_config_setBrightness(DISPLAY_BRIGHTNESS_100); break;
			case DISPLAY_MENU_SCREENTIME: lyft_config_setScreentime(LYFT_SCREENTIME_20SEC); break;
			default: display_show_error(); break;
		}
	}
}


/************************************************************************/
/* CONFIGURATION SETTINGS                                               */
/************************************************************************/
static void lyft_config_save(void)
{
	uint8_t		conf_file[NVM_CONFIG_LEN];

	if (temp_memory_button < BUTTON_MAX) {
		keypad[temp_memory_button].memory_position = temp_memory_position;
	}
	
	conf_file[NVMIDX_POSITION_1]	= ((keypad[BUTTON_1].memory_position & 0xFF00) >> 8);
	conf_file[NVMIDX_POSITION_1 + 1] = (keypad[BUTTON_1].memory_position & 0xFF);
	
	conf_file[NVMIDX_POSITION_2]	= ((keypad[BUTTON_2].memory_position & 0xFF00) >> 8);
	conf_file[NVMIDX_POSITION_2 + 1] = (keypad[BUTTON_2].memory_position & 0xFF);
	
	conf_file[NVMIDX_POSITION_3]	= ((keypad[BUTTON_3].memory_position & 0xFF00) >> 8);
	conf_file[NVMIDX_POSITION_3 + 1] = (keypad[BUTTON_3].memory_position & 0xFF);
	
	conf_file[NVMIDX_POSITION_4]	= ((keypad[BUTTON_4].memory_position & 0xFF00) >> 8);
	conf_file[NVMIDX_POSITION_4 + 1] = (keypad[BUTTON_4].memory_position & 0xFF);
		
	conf_file[NVMIDX_VOLUME] = config.volume;
	conf_file[NVMIDX_BRIGHTNESS] = config.brightness;
	conf_file[NVMIDX_SCREENTIME] = config.screentime;
	conf_file[NVMIDX_UNIT] = config.unit;
	conf_file[NVMIDX_OFFSET] = config.offset;
	
	hal_nvm_write(0, conf_file, NVM_CONFIG_LEN);
}

static void lyft_config_load(void)
{
	uint8_t		conf_file[NVM_CONFIG_LEN];
	uint16_t	mem_pos;
	
	hal_nvm_read(0, conf_file, NVM_CONFIG_LEN);
	
	mem_pos = ((conf_file[NVMIDX_POSITION_1] << 8) | conf_file[NVMIDX_POSITION_1 + 1]);
	keypad[BUTTON_1].memory_position = mem_pos;
	
	mem_pos = ((conf_file[NVMIDX_POSITION_2] << 8) | conf_file[NVMIDX_POSITION_2 + 1]);
	keypad[BUTTON_2].memory_position = mem_pos;
	
	mem_pos = ((conf_file[NVMIDX_POSITION_3] << 8) | conf_file[NVMIDX_POSITION_3 + 1]);
	keypad[BUTTON_3].memory_position = mem_pos;
	
	mem_pos = ((conf_file[NVMIDX_POSITION_4] << 8) | conf_file[NVMIDX_POSITION_4 + 1]);
	keypad[BUTTON_4].memory_position = mem_pos;
	
	if (conf_file[NVMIDX_BRIGHTNESS] < DISPLAY_BRIGHTNESS_MAX) {
		config.brightness = conf_file[NVMIDX_BRIGHTNESS];
	} else {	// default value
		config.brightness = DISPLAY_BRIGHTNESS_25;
	}
		
	if (conf_file[NVMIDX_UNIT] < DISPLAY_UNIT_MAX) {
		config.unit = conf_file[NVMIDX_UNIT];
	} else {	// default value
		config.unit = DISPLAY_UNIT_CENTIMETER;
	}
	
	if (conf_file[NVMIDX_SCREENTIME] <= LYFT_SCREENTIME_20SEC) {
		config.screentime = conf_file[NVMIDX_SCREENTIME];
	} else {	// default value
		config.screentime = LYFT_SCREENTIME_10SEC;
	}
	
	if (conf_file[NVMIDX_OFFSET] < LYFT_OFFSET_MAX) {
		config.offset = conf_file[NVMIDX_OFFSET];
	} else {	// default value
		config.offset = LYFT_OFFSET_ZERO;
	}
	
	if (conf_file[NVMIDX_VOLUME] < AUDIO_VOLUME_MAX) {
		config.volume = conf_file[NVMIDX_VOLUME];
	} else {	// default value
		config.volume = AUDIO_VOLUME_MID;
	}
}

static void lyft_config_setPosition(enum keypad_layout button)
{
	if (temp_memory_button == button) {
		temp_memory_button = BUTTON_MAX;
		temp_memory_position = 0;
	} else {
		temp_memory_button = button;
		temp_memory_position = desk_position;
	}
	
	display_refresh = true;
}

static void lyft_config_setUnit(enum display_unit unit)
{
	if (unit != config.unit)
	{
		config.unit = unit;
		display_refresh = true;
	}	
}

static void lyft_config_setVolume(enum audio_volume volume)
{
	if (volume != config.volume)
	{
		audio_setVolume(volume);		
		config.volume = volume;
		display_refresh = true;
	}	
	
	audio_play(TITLE_DEMO);
}

static void lyft_config_setBrightness(enum display_brightness bri)
{
	if (bri != config.brightness)
	{
		display_setBrightness(bri);
		config.brightness = bri;
		display_refresh = true;
	}	
}

static void lyft_config_setScreentime(enum lyft_screentime screentime)
{
	if (screentime != config.screentime)
	{
		config.screentime = screentime;
		display_refresh = true;
	}
}

static void lyft_config_incOffset(void)
{
	if (config.offset < LYFT_OFFSET_PLUS_9) {
		config.offset++;
		display_refresh = true;
	}
}

static void lyft_config_decOffset(void)
{
	if (config.offset > LYFT_OFFSET_MINUS_9) {
		config.offset--;
		display_refresh = true;
	}
}

/************************************************************************/
/* INTERRUPT AND CALLBACK FUNCTIONS                                     */
/************************************************************************/
static void lyft_cb_display(void)
{	
	hal_timer_setCountValue(TIMER_DISPLAY, 0);
	display_runtime++;
	
	if ((display_screen & DISPLAY_MENU) == DISPLAY_MENU)
	{
		if (display_runtime == LYFT_MENU_LEAVE_DELAY)
		{
			lyft_config_save();
			lyft_menu_leave();
			audio_play(TITLE_MENU_LEAVE);
		}
	}
	else
	{
		if (display_runtime == config.screentime)
		{
			hal_timer_stop(TIMER_DISPLAY);
			hal_timer_stop(TIMER_KEYPAD);
			time_index = 0;
			
			display_screen = DISPLAY_OFF;
			
			irq_keypad_suppress = true;
			hal_irq_enable(IRQ_SOURCE_KEYPAD);
		}
	}
}

static void lyft_cb_keypad(void)
{
	hal_timer_setCountValue(TIMER_KEYPAD, 0);
	time_index++;
	
	if ((keypad_poll_delay == 0) && (keypad_poll == false)) {
		keypad_poll = true;
	} else if (keypad_poll_delay > 0) {
		keypad_poll_delay--;
	}
}

static void lyft_cb_motor(void)
{
	hal_timer_stop(TIMER_MOTOR);
	motor_service = true;
}

static void lyft_irq_keypad(void)
{
	if (irq_keypad_suppress == true) {
		irq_keypad_suppress = false;
	} else {
		hal_irq_disable(IRQ_SOURCE_KEYPAD);
		
		keypad_poll_delay = 1;
		keypad_poll = true;
		hal_timer_start(TIMER_KEYPAD);
	}
}
