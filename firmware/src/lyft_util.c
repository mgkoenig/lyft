/*
 * lyft_util.c
 *
 * Created: 21/02/2022 11:42:02
 *  Author: Matthias Koenig (mgkoenig)
 */ 

#include "lyft_util.h"


uint16_t abs_difference(uint16_t num1, uint16_t num2)
{
	uint16_t result;
	
	if (num1 > num2) {
		result = (num1 - num2);
	} else if (num1 < num2) {
		result = (num2 - num1);
	} else {
		result = 0;
	}
	
	return result;
}