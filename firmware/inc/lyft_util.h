/**
 * @file lyft_util.h
 * @author Matthias Koenig (mgkoenig) 
 * @date 13 Nov 2021 
 * @brief LYFT Utilities
 *
 * @details This file contains helper macros for bitwise operations. 
 * Also basic mathematical operations are provided in order to avoid 
 * including extensive math libs. 
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
#define BITMASK_CHECK_ALL(byte,mask)		(!(~(byte) & (mask)))
#define BITMASK_CHECK_ANY(byte,mask)		((byte) & (mask))

#define MIN(a,b)							(((a) < (b)) ? (a) : (b))
#define MAX(a,b)							(((a) > (b)) ? (a) : (b))


/** 
 * @brief Difference of two numbers as absolute value
 *
 * Computes the difference of two (unsigned) integer values and returns 
 * the result as absolute value. 
 *
 * @param[in] num1 value one
 * @param[in] num2 value two
 */
uint16_t abs_difference(uint16_t num1, uint16_t num2);


#endif /* LYFT_UTIL_H_ */