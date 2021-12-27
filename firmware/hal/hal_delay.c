/*
 * hal_delay.c
 *
 * Created: 16/11/2021 21:19:39
 *  Author: Sire
 */ 

#include "hal_delay.h"


#define F_CPU						48000000UL

#define cpu_ms_2_cy(ms, f_cpu)		(((uint64_t)(ms) * (f_cpu) + (uint64_t)(7e3-1ul)) / (U64)7e3)
#define cpu_us_2_cy(us, f_cpu)		(((uint64_t)(us) * (f_cpu) + (uint64_t)(7e6-1ul)) / (U64)7e6)
#define delay_cycles				portable_delay_cycles
#define cpu_delay_sec(delay)		delay_cycles(cpu_ms_2_cy(1000 * delay, F_CPU))
#define cpu_delay_msec(delay)		delay_cycles(cpu_ms_2_cy(delay, F_CPU))
#define cpu_delay_usec(delay)		delay_cycles(cpu_us_2_cy(delay, F_CPU))
#define _delay_us					cpu_delay_usec
#define _delay_ms					cpu_delay_msec
#define _delay_s					cpu_delay_sec


void portable_delay_cycles(uint32_t n);


__attribute__ ((section(".ramfunc"))) void portable_delay_cycles(uint32_t n)
{
	__asm (
	"loop: DMB	\n"
	#ifdef __ICCARM__
	"SUBS r0, r0, #1 \n"
	#else
	"SUB r0, r0, #1 \n"
	#endif
	"CMP r0, #0  \n"
	"BNE loop         "
	);
}

void hal_delay_init(void)
{
	// nothing to be done here. 
}

void hal_delay_sec(uint32_t delay)
{
	if (delay > 0) {
		_delay_s(delay);
	}	
}

void hal_delay_msec(uint32_t delay)
{
	if (delay > 0) {
		_delay_ms(delay);
	}
}

void hal_delay_usec(uint32_t delay)
{
	if (delay > 0) {
		_delay_us(delay);
	}
}