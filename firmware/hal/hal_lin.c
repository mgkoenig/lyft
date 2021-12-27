/*
 * hal_lin.c
 *
 * Created: 19/11/2021 14:03:29
 *  Author: Sire
 */ 

#include "hal_lin.h"


uint8_t					rx_byte;
struct usart_module		usart_instance;


static uint8_t	lin_parity(uint8_t node_id);
static uint8_t	lin_checksum_classic(const uint8_t *data, uint8_t len);
static uint8_t	lin_checksum_enhanced(uint8_t pid, const uint8_t *data, uint8_t len);


void hal_lin_init()
{
	struct usart_config config_usart;

	usart_get_config_defaults(&config_usart);
	
	config_usart.baudrate			= 19200;
	config_usart.mux_setting		= LIN_MUXSET;
	config_usart.pinmux_pad0		= LIN_PINMUX_PAD0;
	config_usart.pinmux_pad1		= LIN_PINMUX_PAD1;
	config_usart.pinmux_pad2		= PINMUX_UNUSED;
	config_usart.pinmux_pad3		= PINMUX_UNUSED;
	config_usart.data_order			= USART_DATAORDER_LSB;
	config_usart.generator_source	= GCLK_GENERATOR_3;
	
	while (usart_init(&usart_instance, LIN_MODULE, &config_usart) != STATUS_OK);
	usart_enable(&usart_instance);	
}

void hal_lin_write(uint8_t node_id, const uint8_t *data, uint8_t len)
{
	uint8_t i, pid;
	uint8_t checksum;
	uint8_t packet_len = (len + LIN_HEADER_LENGTH);
	uint8_t lin_packet[packet_len];
	
	if ((data != NULL) && (len > 0))
	{
		pid = (lin_parity(node_id) | node_id);

		if (node_id == LIN_NODE_DIAG_TX) {
			checksum = lin_checksum_classic(data, len);
		} else {
			checksum = lin_checksum_enhanced(pid, data, len);
		}

		lin_packet[0] = LIN_SYNC_FIELD;
		lin_packet[1] = pid;

		for (i=0; i<len; i++)
		{
			lin_packet[i+2] = data[i];
		}

		lin_packet[len+2] = checksum;
		
		usart_disable(&usart_instance);
		port_pin_set_output_level(LIN_PIN_TX, false);
		hal_delay_usec(LIN_BREAK_LENGTH);
		usart_enable(&usart_instance);
		hal_delay_usec(LIN_BREAK_DELIMITER);
		
		while (usart_write_buffer_wait(&usart_instance, lin_packet, packet_len) != STATUS_OK);
	}
}

uint8_t hal_lin_read(uint8_t node_id, uint8_t *data, uint8_t len)
{
	uint8_t				i, pid;
	uint8_t				byte_cnt = 0;
	uint8_t				read_announcement[2];
	uint16_t			timeout;	
	enum status_code	errno;
	
	pid = (lin_parity(node_id) | node_id);
	read_announcement[0] = LIN_SYNC_FIELD;
	read_announcement[1] = pid;
	
	usart_disable(&usart_instance);
	port_pin_set_output_level(LIN_PIN_TX, false);
	hal_delay_usec(LIN_BREAK_LENGTH);
	usart_enable(&usart_instance);
	hal_delay_usec(LIN_BREAK_DELIMITER);
	
	while (usart_write_buffer_wait(&usart_instance, read_announcement, 2) != STATUS_OK);
	
	if ((data != NULL) && (len > 0))
	{	
		for (i=0; i<len; i++)
		{
			timeout = 0;
			do {
				errno = usart_read_wait(&usart_instance, &rx_byte);
				timeout++;
			} while ((errno != STATUS_OK) && (timeout < LIN_TIMEOUT_BYTE));
			
			if (errno == STATUS_OK)
			{
				data[i] = rx_byte;
				byte_cnt++;
			}
		}
	}
	
	return byte_cnt;
}


static uint8_t lin_parity(uint8_t node_id)
{
	uint8_t par, p0, p1;
	
	p0 = BIT(node_id,0) ^ BIT(node_id,1) ^ BIT(node_id,2) ^ BIT(node_id,4);
	p1 = ~(BIT(node_id,1) ^ BIT(node_id,3) ^ BIT(node_id,4) ^ BIT(node_id,5));
	par = ((p0 | (p1<<1)) << 6);

	return par;
}

static uint8_t lin_checksum_classic(const uint8_t *data, uint8_t len)
{
	uint8_t     i;
	uint16_t    cc = 0;

	for (i=0; i<len; i++) {
		cc += data[i];

		if (cc > 255) {
			cc -= 255;
		}
	}

	cc = ((~cc) & 0xFF);

	return (uint8_t) cc;
}

static uint8_t lin_checksum_enhanced(uint8_t pid, const uint8_t *data, uint8_t len)
{
	uint8_t     i;
	uint16_t    cc = pid;

	for (i=0; i<len; i++) {
		cc += data[i];

		if (cc > 255) {
			cc -= 255;
		}
	}

	cc = ((~cc) & 0xFF);

	return (uint8_t) cc;
}

