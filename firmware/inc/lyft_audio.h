/*
 * lyft_audio.h
 *
 * Created: 10/12/2021 11:48:58
 *  Author: Matthias Koenig
 */ 


#ifndef LYFT_AUDIO_H_
#define LYFT_AUDIO_H_

#include <asf.h>
#include "hal_dac.h"
#include "hal_timer.h"


#define DAC_VOLUME_OFF			0		// DAC output voltage in mV 
#define DAC_VOLUME_LOW			115		// DAC output voltage in mV 
#define DAC_VOLUME_MID			380		// DAC output voltage in mV 
#define DAC_VOLUME_HIGH			950		// DAC output voltage in mV 


enum audio_scale {
	PAUSE,
	C4,
	D4,
	E4,
	F4,
	G4,
	A4,
	B4_MINOR,
	B4,
	C5,
	D5,
	E5,
	F5,
	G5,
	A5,
	B5,
	C6,
	D6,
	E6,
	F6,
	G6,
	A6,
	B6,
	C7,
	D7,
	SCALE_MAX
};

enum audio_repertoire {
	TITLE_NONE,
	TITLE_ANTHEM,
	TITLE_LIMIT,
	TITLE_DOUBLE_CLICK,
	TITLE_DEMO,
	TITLE_STARTUP,
	TITLE_MENU_LEAVE,
	TITLE_ALARM,
	REPERTOIRE_MAX	
};

enum audio_volume {
	AUDIO_VOLUME_OFF,
	AUDIO_VOLUME_LOW,
	AUDIO_VOLUME_MID,
	AUDIO_VOLUME_HIGH,
	AUDIO_VOLUME_MAX
};

struct melody {
	uint8_t				play_time;		// number of notes
	enum audio_scale	*sequence;		// note sequence
	uint16_t			*duration;		// duration of each note
};


void audio_init(void);
void audio_setVolume(enum audio_volume volume);
void audio_play(enum audio_repertoire title);


#endif /* LYFT_AUDIO_H_ */