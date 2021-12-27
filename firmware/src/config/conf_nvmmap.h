/*
 * conf_nvmmap.h
 *
 * Created: 09/12/2021 11:54:02
 *  Author: Matthias Koenig
 */ 


#ifndef CONF_NVMMAP_H_
#define CONF_NVMMAP_H_


#define NVM_CONFIG_LEN					13		// 13 bytes of NVM memory for whole configuration 


#define NVMIDX_POSITION_1				NVM_INDEX_0			// Desk memory position 1
#define NVMLEN_POSITION_1				2

#define NVMIDX_POSITION_2				NVM_INDEX_2			// Desk memory position 2
#define NVMLEN_POSITION_2				2

#define NVMIDX_POSITION_3				NVM_INDEX_4			// Desk memory position 3
#define NVMLEN_POSITION_3				2

#define NVMIDX_POSITION_4				NVM_INDEX_6			// Desk memory position 4
#define NVMLEN_POSITION_4				2

#define NVMIDX_BRIGHTNESS				NVM_INDEX_8			// Display Brightness
#define NVMLEN_BRIGHTNESS				1

#define NVMIDX_VOLUME					NVM_INDEX_9			// Beeper Loudness
#define NVMLEN_VOLUME					1

#define NVMIDX_SCREENTIME				NVM_INDEX_10		// Display-On time in seconds
#define NVMLEN_SCREENTIME				1

#define NVMIDX_UNIT						NVM_INDEX_11		// Shown unit on display (cm or inch)
#define NVMLEN_UNIT						1

#define NVMIDX_OFFSET					NVM_INDEX_12		// desk height offset (+/- 9cm)
#define NVMLEN_OFFSET					1



#endif /* CONF_NVMMAP_H_ */
