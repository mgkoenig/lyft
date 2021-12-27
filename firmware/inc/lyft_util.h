/*
 * lyft_util.h
 *
 * Created: 13/11/2021 12:35:59
 *  Author: Matthias Koenig
 */ 


#ifndef LYFT_UTIL_H_
#define LYFT_UTIL_H_

#include <asf.h>


#define BIT(byte,nbit)						((byte>>nbit) & 1)

#define BIT_SET(byte,nbit)					((byte) |=  (1<<(nbit)))
#define BIT_CLEAR(byte,nbit)				((byte) &= ~(1<<(nbit)))
#define BIT_FLIP(byte,nbit)					((byte) ^=  (1<<(nbit)))
#define BIT_CHECK(byte,nbit)				((byte) &   (1<<(nbit)))

#define BITMASK_SET(byte,mask)				((byte) |= (mask))
#define BITMASK_CLEAR(byte,mask)			((byte) &= (~(mask)))
#define BITMASK_FLIP(byte,mask)				((byte) ^= (mask))
#define BITMASK_CHECK_ALL(byte, mask)		(!(~(byte) & (mask)))
#define BITMASK_CHECK_ANY(byte,mask)		((byte) & (mask))

#define MIN(a,b)							(((a) < (b)) ? (a) : (b))
#define MAX(a,b)							(((a) > (b)) ? (a) : (b))


#endif /* LYFT_UTIL_H_ */