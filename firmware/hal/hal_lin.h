/*
 * hal_lin.h
 *
 * Created: 19/11/2021 14:03:20
 *  Author: Sire
 */ 


#ifndef HAL_LIN_H_
#define HAL_LIN_H_

#include <asf.h>
#include "conf_board.h"
#include "hal_delay.h"
#include "lyft_util.h"


/************************************************************************
 * Special LIN nodes
 ************************************************************************/
#define LIN_NODE_DIAG_TX			60
#define LIN_NODE_DIAG_RX			61

/************************************************************************
 * LIN sync field
 ************************************************************************/
#define LIN_SYNC_FIELD              0x55

/************************************************************************
 * LIN break field length
 * 
 * LIN baudrate = 19200bd -> t_bit = 52us 
 * LIN break field > 13 bits in length
 * 
 * t_break = (13 * 52us) = 676us
 ************************************************************************/
#define LIN_BREAK_LENGTH            676

/************************************************************************
 * LIN break delimiter length
 * 
 * The typical length of the break delimiter is 
 * between one and four bit lengths.  
 * 
 * t_delimiter = (2 * 52us) = 104us
 ************************************************************************/
#define LIN_BREAK_DELIMITER         104

/************************************************************************
 * LIN header and trailer length
 * 
 * Three additional bytes (sync field, PID, checksum) are used 
 * to form a complete transmission packet (header + data). 
 ************************************************************************/
#define LIN_HEADER_LENGTH           3       
#define LIN_HEADER_BYTES            3

/************************************************************************
 * LIN timeout length for a byte:
 * 
 * LIN baudrate  = 19200bd -> t_bit = 52us 
 * CPU frequency = 8MHz    -> t_per = 125ns    
 * CPU frequency = 48MHz   -> t_per = 21ns
 * 
 * t_byte = ((52us * 8 bit) / 125ns) = 3328
 * t_byte = ((52us * 8 bit) / 21ns) = 19810
 * 
 * Note: this is only a rough estimation of the timings 
 *       in order to get a countable timeout value 
 *       for a transmitted byte with some margin. 
 *       Start/Stop-bits are included. 
 ************************************************************************/
#define LIN_TIMEOUT_BYTE            19810


void	hal_lin_init(void);
void	hal_lin_write(uint8_t node_id, const uint8_t *data, uint8_t len);
uint8_t hal_lin_read(uint8_t node_id, uint8_t *data, uint8_t len);


#endif /* HAL_LIN_H_ */