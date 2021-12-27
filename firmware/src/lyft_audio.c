/*
 * lyft_audio.c
 *
 * Created: 10/12/2021 11:48:47
 *  Author: Matthias Koenig
 */ 

#include "lyft_audio.h"


static void audio_cb_playback (void);


bool					audio_enable;
enum audio_repertoire	current_title;
uint8_t					beat;


const uint16_t tone_pitch[SCALE_MAX] = {	// all tone periods in us
	16,			// PAUSE,
	3831,		// C4,
	3413,		// D4,
	3039,		// E4,
	2863,		// F4,
	2551,		// G4,
	2272,		// A4 = 440Hz,
	2146,		// B4_MINOR
	2024,		// B4,	
	1912,		// C5,
	1703,		// D5,
	1517,		// E5,
	1432,		// F5,
	1275,		// G5,
	1136,		// A5,
	1013,		// B5,
	956,		// C6,
	852,		// D6,
	758,		// E6,
	716,		// F6,
	637,		// G6,
	568,		// A6,
	506,		// B6,
	477,		// C7,
	426			// D7
};

/************************************************************************
 * ATTENTION! 
 * Tone durations must not be longer than 520ms.
 * Otherwise the 16bit counter would overflow! 
 * Generally: the shorter the cue the better. 
 *
 ***********************************************************************/
const struct melody repertoire[REPERTOIRE_MAX] = {
	{	// title 'none'
		.play_time = 0,
		.sequence = (enum audio_scale[]) {PAUSE},
		.duration = (uint16_t[]) {10}
	},
	{	// title 'anthem'
		.play_time = 8,
		.sequence = (enum audio_scale[]) {D5, PAUSE, D5, B4_MINOR, PAUSE, B4_MINOR, PAUSE, B4_MINOR},
		.duration = (uint16_t[]) {260, 20, 520, 260, 20, 260, 20, 520}
	},
	{	// title 'limit'
		.play_time = 1,
		.sequence = (enum audio_scale[]) {A6},
		.duration = (uint16_t[]) {120}
	},
	{	// title 'double_click'
		.play_time = 3,
		.sequence = (enum audio_scale[]) {A6, PAUSE, A6},
		.duration = (uint16_t[]) {60, 20, 60}
	},
	{	// title 'demo'
		.play_time = 1,
		.sequence = (enum audio_scale[]) {A6},
		.duration = (uint16_t[]) {300}
	},
	{	// title 'startup'
		.play_time = 3,
		.sequence = (enum audio_scale[]) {A5, C6, E6},
		.duration = (uint16_t[]) {200, 100, 75}
	},
	{	// title 'menu_leave'
		.play_time = 1,
		.sequence = (enum audio_scale[]) {D7},
		.duration = (uint16_t[]) {25}
	},
	{	// title 'alarm'
		.play_time = 5,
		.sequence = (enum audio_scale[]) {A5, PAUSE, A5, PAUSE, A5},
		.duration = (uint16_t[]) {350, 100, 350, 100, 350}
	}
};


void audio_init(void)
{
	hal_dac_init();
	current_title = TITLE_NONE;	
}

void audio_setVolume(enum audio_volume volume)
{
	switch (volume)
	{
		case AUDIO_VOLUME_OFF: hal_dac_setVoltage(DAC_VOLUME_OFF); audio_enable = false; break;
		case AUDIO_VOLUME_LOW: hal_dac_setVoltage(DAC_VOLUME_LOW); audio_enable = true; break;
		case AUDIO_VOLUME_MID: hal_dac_setVoltage(DAC_VOLUME_MID); audio_enable = true; break;
		case AUDIO_VOLUME_HIGH: hal_dac_setVoltage(DAC_VOLUME_HIGH); audio_enable = true; break;
		default: break;
	}	
}

void audio_play(enum audio_repertoire title)
{
	uint8_t				playtime;
	uint16_t			duration;
	uint16_t			pitch;
	enum audio_scale	note;
	
	if (audio_enable == true)
	{
		if ((current_title == TITLE_NONE) && (title < REPERTOIRE_MAX))
		{
			beat = 0;
			
			current_title = title;
			playtime = repertoire[current_title].play_time;
			
			if (playtime > 0)
			{
				duration = repertoire[current_title].duration[0];
				note = repertoire[current_title].sequence[0];
				pitch = tone_pitch[note];
				
				hal_timer_init(TIMER_AUDIO_DURATION, duration, audio_cb_playback);
				hal_timer_init(TIMER_AUDIO_FREQUENCY, pitch, NULL);
			}			
		}
	}
}

static void audio_cb_playback (void)
{	
	uint8_t				playtime;
	uint16_t			duration;
	uint16_t			pitch;
	enum audio_scale	prev_note, note;
		
	beat++;
	playtime = repertoire[current_title].play_time;
			
	if (beat < playtime)
	{
		duration = repertoire[current_title].duration[beat];
		note = repertoire[current_title].sequence[beat];
		prev_note = repertoire[current_title].sequence[beat - 1];
		pitch = tone_pitch[note];
				
		hal_timer_setPeriod(TIMER_AUDIO_DURATION, duration);
		hal_timer_setCountValue(TIMER_AUDIO_DURATION, 0);
		
		if (note == PAUSE)
		{
			hal_timer_stop(TIMER_AUDIO_FREQUENCY);
		}
		else
		{
			if (prev_note == PAUSE) {
				hal_timer_start(TIMER_AUDIO_FREQUENCY);
			}
			hal_timer_setPeriod(TIMER_AUDIO_FREQUENCY, pitch);
		}
	}
	else
	{
		hal_timer_disable(TIMER_AUDIO_DURATION);
		hal_timer_disable(TIMER_AUDIO_FREQUENCY);
		current_title = TITLE_NONE;
	}
}
