/**
 * @file lyft_audio.h
 * @author Matthias Koenig (mgkoenig) 
 * @date 10 Dec 2021 
 * @brief LYFT Audio Features
 *
 * @details LYFT uses an audio buzzer to create acoustic feedback for 
 * the user. This file contains all routines to drive the buzzer. 
 * 
 * In order to set the volume a DAC drives an op-amp which delivers 
 * a certain voltage level for the buzzer. Since the buzzer does not 
 * need much power, the op-amp is sufficient to drive the piezo 
 * without further amplification. 
 * 
 * To play a melody, a sequence has to be defined using the available
 * scale consisting of three ocaves and a pause element. Only one 
 * melody at a time can be played. There is no playing queue available. 
 */

#ifndef LYFT_AUDIO_H_
#define LYFT_AUDIO_H_


#include <asf.h>
#include "hal_dac.h"
#include "hal_timer.h"


#define DAC_VOLUME_OFF			0		/**< DAC output voltage in mV for no sound */
#define DAC_VOLUME_LOW			115		/**< DAC output voltage in mV for low volume */
#define DAC_VOLUME_MID			380		/**< DAC output voltage in mV for mid volume */
#define DAC_VOLUME_HIGH			950		/**< DAC output voltage in mV for high volume */


/** 
 * @enum audio_scale 
 * Scale of three octaves to create simple melodies 
 */
enum audio_scale {
	PAUSE,                  /**< No tune, pause */
	C4,                     /**< Note C4 = 262Hz */
	D4,                     /**< Note D4 = 294Hz */
	E4,                     /**< Note E4 = 330Hz */
	F4,                     /**< Note F4 = 349Hz */ 
	G4,                     /**< Note G4 = 392Hz */
	A4,                     /**< Note A4 = 440Hz */
	B4_MINOR,               /**< Note B4_MINOR = 466Hz */ 
	B4,                     /**< Note B4 = 494Hz */
	C5,                     /**< Note C5 = 523Hz */
	D5,                     /**< Note D5 = 587Hz */
	E5,                     /**< Note E5 = 659Hz */
	F5,                     /**< Note F5 = 698Hz */
	G5,                     /**< Note G5 = 784Hz */
	A5,                     /**< Note A5 = 880Hz */
	B5,                     /**< Note B5 = 988Hz */
	C6,                     /**< Note C6 = 1047Hz */
	D6,                     /**< Note D6 = 1175Hz */
	E6,                     /**< Note E6 = 1319Hz */
	F6,                     /**< Note F6 = 1397Hz */
	G6,                     /**< Note G6 = 1568Hz */
	A6,                     /**< Note A6 = 1760Hz */
	B6,                     /**< Note B6 = 1976Hz */
	C7,                     /**< Note C7 = 2093Hz */
	D7,                     /**< Note D7 = 2349Hz */
	SCALE_MAX
};

/** 
 * @enum audio_repertoire 
 * A repertoire of different melodies for an appropriate audio feedback 
 */
enum audio_repertoire {
	TITLE_NONE,             /**< No title playing, audio is paused */
	TITLE_ANTHEM,           /**< First two bars of the swedish anthem */
	TITLE_LIMIT,            /**< Sound when lowest/highest desk position is reached */
	TITLE_DOUBLE_CLICK,     /**< Double click sound */
	TITLE_DEMO,             /**< Demo sound to check volume */
	TITLE_STARTUP,          /**< Simple startup sound when powering up */
	TITLE_MENU_LEAVE,       /**< Short sound when menu is left and config is saved */
	TITLE_ALARM,            /**< If the desk is in a criticial condition */
	REPERTOIRE_MAX	
};

/** 
 * @enum audio_volume
 * Different audio volume levels 
 */
enum audio_volume {
	AUDIO_VOLUME_OFF,       /**< audio completely turned off */
	AUDIO_VOLUME_LOW,       /**< audio volume low */
	AUDIO_VOLUME_MID,       /**< audio volume medium */
	AUDIO_VOLUME_HIGH,      /**< audio volume high */
	AUDIO_VOLUME_MAX
};

/**
 * @struct melody
 * This struct describes a simple melody consisting of the overall 
 * sequence length (play_time), all notes in the played order 
 * (sequence) and the duration of each note. 
 */
struct melody {
	uint8_t				play_time;		/**< Total number of notes */ 
	enum audio_scale	*sequence;		/**< Tone sequence */ 
	uint16_t			*duration;		/**< Duration of each note */ 
};


/** 
 * @brief Audio Module Init
 *
 * Initializes the DAC module for volume control. Audio timers 
 * (for tone control like pitch and duration) will be initialized 
 * and enabled for each title separately. 
 */
void audio_init(void);

/** 
 * @brief Set the audio volume
 *
 * Set the volume of the audio buzzer. 
 * 
 * @param[in] volume the volume level, one of #AUDIO_VOLUME_OFF, 
 * #AUDIO_VOLUME_LOW, #AUDIO_VOLUME_MID, #AUDIO_VOLUME_HIGH
 */
void audio_setVolume(enum audio_volume volume);

/** 
 * @brief Play a melody
 * 
 * Play a certain title of the repertoire. This function also 
 * initializes and starts the audio timers (frequency and duration) 
 * accoring to the sequence tunes. 
 * 
 * @param[in] title the title to be played, eg. #TITLE_STARTUP
 */
void audio_play(enum audio_repertoire title);


#endif /* LYFT_AUDIO_H_ */