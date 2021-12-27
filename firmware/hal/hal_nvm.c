/*
 * hal_nvm.c
 *
 * Created: 09/12/2021 11:48:17
 *  Author: Sire
 */ 
#include "hal_nvm.h"


void hal_nvm_init(void)
{
	struct nvm_config config_nvm;

	nvm_get_config_defaults(&config_nvm);
	config_nvm.manual_page_write = true;
	nvm_set_config(&config_nvm);
}

void hal_nvm_read(uint8_t nvm_idx, uint8_t *data, uint16_t len)
{
	enum status_code	error_code;
	uint8_t				page_data[NVMCTRL_PAGE_SIZE];
	uint8_t				row_data[NVMCTRL_ROW_SIZE];
	uint8_t				page_in_row;
	uint8_t				page_idx;
	uint16_t			data_idx;
	uint16_t			row_idx;
	uint32_t			page_address;

	if ((data != NULL) && len > 0)
	{
		if ((nvm_idx < NVM_INDEX_MAX) && (len <= (NVM_INDEX_MAX - nvm_idx)))
		{
			page_in_row = (nvm_idx / NVMCTRL_PAGE_SIZE);
			
			// Only one page to read
			if ((nvm_idx + len) <= ((page_in_row + 1) * NVMCTRL_PAGE_SIZE))
			{
				page_address = NVM_BASE_ADDRESS + (page_in_row * NVMCTRL_PAGE_SIZE);
				
				do {
					error_code = nvm_read_buffer(page_address, page_data, NVMCTRL_PAGE_SIZE);
				} while (error_code == STATUS_BUSY);
				
				for (data_idx=0; data_idx<len; data_idx++)
				{
					page_idx = (nvm_idx - (page_in_row * NVMCTRL_PAGE_SIZE) + data_idx);
					data[data_idx] = page_data[page_idx];
				}
			}
			else		// read the whole row
			{
				for (page_in_row=0; page_in_row<NVMCTRL_ROW_PAGES; page_in_row++)
				{
					page_address = NVM_BASE_ADDRESS + (page_in_row * NVMCTRL_PAGE_SIZE);
					
					do {
						error_code = nvm_read_buffer(page_address, page_data, NVMCTRL_PAGE_SIZE);
					} while (error_code == STATUS_BUSY);
					
					for (page_idx = 0; page_idx<NVMCTRL_PAGE_SIZE; page_idx++)
					{
						row_idx = (page_in_row * NVMCTRL_PAGE_SIZE) + page_idx;
						row_data[row_idx] = page_data[page_idx];
					}
				}
				
				for (data_idx=0; data_idx<len; data_idx++) {
					data[data_idx] = row_data[nvm_idx + data_idx];
				}
			}
		}
	}
}

void hal_nvm_write(uint8_t nvm_idx, uint8_t *data, uint16_t len)
{
	enum status_code	error_code;
	uint8_t				page_data[NVMCTRL_PAGE_SIZE];
	uint8_t				row_data[NVMCTRL_ROW_SIZE] = {0};
	uint8_t				page_in_row;
	uint8_t				page_idx;
	uint16_t			row_idx;
	uint32_t			page_address;
	uint16_t			i;
	
	if ((data != NULL) && len > 0)
	{
		if ((nvm_idx < NVM_INDEX_MAX) && (len <= (NVM_INDEX_MAX - nvm_idx)))
		{
			hal_nvm_read(0, row_data, NVMCTRL_ROW_SIZE);
			
			for (i=0; i<len; i++) {
				row_data[nvm_idx + i] = data[i];
			}
			
			do {
				error_code = nvm_erase_row(NVM_BASE_ADDRESS);
			} while (error_code == STATUS_BUSY);

			for (page_in_row=0; page_in_row<NVMCTRL_ROW_PAGES; page_in_row++)
			{
				page_address = NVM_BASE_ADDRESS + (page_in_row * NVMCTRL_PAGE_SIZE);
				
				for (page_idx=0; page_idx<NVMCTRL_PAGE_SIZE; page_idx++)
				{
					row_idx = (page_in_row * NVMCTRL_PAGE_SIZE) + page_idx;
					page_data[page_idx] = row_data[row_idx];
				}
				
				do {
					error_code = nvm_write_buffer(page_address, page_data, NVMCTRL_PAGE_SIZE);
				} while (error_code == STATUS_BUSY);
				
				error_code = nvm_execute_command(NVM_COMMAND_WRITE_PAGE, page_address, 0);
			}
		}
	}
}
