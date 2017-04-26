/*
 * trinamic.c
 *
 * Created: 17.1.2017 13:27:56
 *  Author: atom2
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "common_defs.h"
#include "def_init.h"
#include <avr/interrupt.h>
#include "trinamic.h"
#include "uart0.h"
#include "uart1.h"
#include <avr/eeprom.h>


volatile Trinamicpac TB_Buf_In;
uint8_t uart0_rx_ptr;
uint8_t uart0_buf_rx[BUFFER_CHAR_PACKET];

volatile Trinamicpac TB_Buf_Out;
uint8_t uart1_rx_ptr;
uint8_t uart1_buf_rx[BUFFER_CHAR_PACKET];


volatile uint8_t TB_addr_Reply=0;
volatile uint8_t TB_addr_Module=0;

uint8_t uart0_tx_flag;
uint8_t uart0_tx_iptr;

uint32_t TB_Value=0;


uint8_t TB_Process0( void )
{
	uint8_t b=0;
	TB_Value =	(((int32_t) TB_Buf_In.b[4]) << 24) |
	(((int32_t) TB_Buf_In.b[5]) << 16) |
	(((int32_t) TB_Buf_In.b[6]) <<  8) |
	(((int32_t) TB_Buf_In.b[7])      ) ;
	switch(TB_Buf_In.b[TB_BUF_COMMAND])
	{
		case TB_CMD_SIO:
			return TB_CMD_SIO;
			break;
		case TB_CMD_GIO:
			return TB_CMD_GIO;
			break;
		case TB_CMD_SGP:
			// Nastavování uart0 - Komunikaèní interní RS485
			if (TB_Buf_In.b[TB_BUF_MOTOR] == 0)
			{
				switch (TB_Buf_In.b[TB_BUF_TYPE])
				{
					case TB_GBPARAM_EEMAGIC:
						if (TB_Value != TB_gbparam0.eemagic)
						{
							TB_gbparam0.eemagic = TB_Value;
							b = (void *) &(TB_gbparam0.eemagic) - (void *) &(TB_gbparam0);
							eeprom_update_byte( (uint8_t * ) (b+EEPROM_START_ADDRESS_0), TB_gbparam0.eemagic);
						}
						TB_SendAck(TB_ERR_EEPROM_OK, TB_Value);
						break;
					case TB_GBPARAM_BAUD:
						if (TB_Value != TB_gbparam0.baud)
						{
							TB_gbparam0.baud = TB_Value;
							b = (void *) &(TB_gbparam0.baud) - (void *) &(TB_gbparam0);
							eeprom_update_byte( (uint8_t * ) (b+EEPROM_START_ADDRESS_0), TB_gbparam0.baud);
						}
						TB_SendAck(TB_ERR_EEPROM_OK, TB_Value);
						break;
					case TB_GBPARAM_ADDRESS:
						if (TB_Value != TB_gbparam0.address)
						{
							TB_gbparam0.address = TB_Value;
							b = (void *) &(TB_gbparam0.address) - (void *) &(TB_gbparam0);
							eeprom_update_byte( (uint8_t * ) (b+EEPROM_START_ADDRESS_0), TB_gbparam0.address);
						}
						TB_SendAck(TB_ERR_EEPROM_OK, TB_Value);
						break;
					case TB_GBPARAM_HOST_ADDR:
						if (TB_Value != TB_gbparam0.host_address)
						{
							TB_gbparam0.host_address = TB_Value;
							b = (void *) &(TB_gbparam0.host_address) - (void *) &(TB_gbparam0);
							eeprom_update_byte( (uint8_t * ) (b+EEPROM_START_ADDRESS_0), TB_gbparam0.host_address);
						}
						TB_SendAck(TB_ERR_EEPROM_OK, TB_Value);
						break;
					default:
						TB_SendAck(TB_ERR_TYPE, TB_Value); // invalid value
						break;
				}
			}
			// Nastavování uart1 - Komunikaèní externí RS485
			else if (TB_Buf_In.b[TB_BUF_MOTOR] == 1)
			{
				switch (TB_Buf_In.b[TB_BUF_TYPE])
				{
					case TB_GBPARAM_EEMAGIC:
						if (TB_Value != TB_gbparam1.eemagic)
						{
							TB_gbparam1.eemagic = TB_Value;
							b = (void *) &(TB_gbparam1.eemagic) - (void *) &(TB_gbparam1);
							eeprom_update_byte( (uint8_t * ) (b+EEPROM_START_ADDRESS_1), TB_gbparam1.eemagic);
						}
						TB_SendAck(TB_ERR_EEPROM_OK, TB_Value);
						break;
					case TB_GBPARAM_BAUD:
						if (TB_Value != TB_gbparam1.baud)
						{
							TB_gbparam1.baud = TB_Value;
							b = (void *) &(TB_gbparam1.baud) - (void *) &(TB_gbparam1);
							eeprom_update_byte( (uint8_t * ) (b+EEPROM_START_ADDRESS_1), TB_gbparam1.baud);
						}
						TB_SendAck(TB_ERR_EEPROM_OK, TB_Value);
						break;
					case TB_GBPARAM_ADDRESS:
						if (TB_Value != TB_gbparam1.address)
						{
							TB_gbparam1.address = TB_Value;
							b = (void *) &(TB_gbparam1.address) - (void *) &(TB_gbparam1);
							eeprom_update_byte( (uint8_t * ) (b+EEPROM_START_ADDRESS_1), TB_gbparam1.address);
						}
						TB_SendAck(TB_ERR_EEPROM_OK, TB_Value);
						break;
					case TB_GBPARAM_HOST_ADDR:
						if (TB_Value != TB_gbparam1.host_address)
						{
							TB_gbparam1.host_address = TB_Value;
							b = (void *) &(TB_gbparam1.host_address) - (void *) &(TB_gbparam1);
							eeprom_update_byte( (uint8_t * ) (b+EEPROM_START_ADDRESS_1), TB_gbparam1.host_address);
						}
						TB_SendAck(TB_ERR_EEPROM_OK, TB_Value);
						break;
					default:
						TB_SendAck(TB_ERR_TYPE, TB_Value); // invalid value
						break;
				}
			}
			else
			{
				TB_SendAck(TB_ERR_MOTOR, TB_Buf_In.b[TB_BUF_MOTOR]); // invalid value
			}
			break;
		case TB_CMD_GGP:
			if (TB_Buf_In.b[TB_BUF_MOTOR] == 0)
			{
				switch (TB_Buf_In.b[TB_BUF_TYPE])
				{
					case TB_GBPARAM_BAUD:
						TB_SendAck(TB_ERR_EEPROM_OK, TB_gbparam0.baud);
						break;
					case TB_GBPARAM_ADDRESS:
						TB_SendAck(TB_ERR_EEPROM_OK, TB_gbparam0.address);
						break;
					case TB_GBPARAM_HOST_ADDR:
						TB_SendAck(TB_ERR_EEPROM_OK, TB_gbparam0.host_address);
						break;
					case TB_GBPARAM_EEMAGIC:
						TB_SendAck(TB_ERR_EEPROM_OK, TB_gbparam0.eemagic);
						break;
					default:
						TB_SendAck(TB_ERR_TYPE, TB_Buf_In.b[TB_BUF_TYPE]); // invalid value
						break;
				}
			}
			else if (TB_Buf_In.b[TB_BUF_MOTOR] == 1)
			{
				switch (TB_Buf_In.b[TB_BUF_TYPE])
				{
					case TB_GBPARAM_BAUD:
						TB_SendAck(TB_ERR_EEPROM_OK, TB_gbparam1.baud);
						break;
					case TB_GBPARAM_ADDRESS:
						TB_SendAck(TB_ERR_EEPROM_OK, TB_gbparam1.address);
						break;
					case TB_GBPARAM_HOST_ADDR:
						TB_SendAck(TB_ERR_EEPROM_OK, TB_gbparam1.host_address);
						break;
					case TB_GBPARAM_EEMAGIC:
						TB_SendAck(TB_ERR_EEPROM_OK, TB_gbparam1.eemagic);
						break;
					default:
						TB_SendAck(TB_ERR_TYPE, TB_Buf_In.b[TB_BUF_TYPE]); // invalid value
						break;
				}
			}
			else
			{
				TB_SendAck(TB_ERR_MOTOR, TB_Buf_In.b[TB_BUF_MOTOR]); // invalid value
			}
			break;
		case 136: // get module version
			//verze Firmware
			if (TB_Buf_In.b[TB_BUF_TYPE] == 0)
			{
				// text mode
				TB_Buf_Out.b[0] = TB_addr_Reply;					// Standartnì adresa 2
				TB_Buf_Out.b[1] = MODULE_ADDRESS;					// Jaká adresa modulu
				TB_Buf_Out.b[2] = MODULE_TYPE;								// Jaký modul odpovídá
				TB_Buf_Out.b[3] = TB_Buf_In.b[TB_BUF_COMMAND];		//
				TB_Buf_Out.b[4] = 'V';								// Verze softwaru.
				TB_Buf_Out.b[5] = '0';
				TB_Buf_Out.b[6] = '.';
				TB_Buf_Out.b[7] = '1';
				TB_calcSum();
				TB_Send_Com();
			}
			else if (TB_Buf_In.b[TB_BUF_TYPE] == 0)
			{
				// binary mode
				TB_SendAck(TB_ERR_OK, (0x10203040));
			}
			else
			{
				TB_SendAck(TB_ERR_TYPE, TB_Buf_In.b[TB_BUF_TYPE]);
			}
			break;
		default:
			TB_SendAck(TB_ERR_COMMAND, TB_Buf_In.b[TB_BUF_COMMAND]); // invalid command
			return 0;
	}
	
	return 0;
}


uint8_t TB_buf_fill_In()
{
	for (uint8_t i=9;i>0;i--)
	{
		TB_Buf_In.b[9-i] = uart0_buf_rx[uart0_rx_ptr-i];	 
	}
	if (uart0_rx_ptr > 17)
	{
		uart0_rx_ptr = 0;
	}
	return 1;
}

uint8_t TB_buf_fill_Out()
{
	for (uint8_t i=9;i>0;i--)
	{
		TB_Buf_Out.b[9-i] = uart1_buf_rx[uart1_rx_ptr-i];	 
	}
	if (uart1_rx_ptr > 17)
	{
		uart1_rx_ptr = 0;
	}
	return 1;
}

void TB_Send_Com(void)
{
	uart0_tx_flag=TRUE;
	//RS485_EN_INT_transmite;
	// Odeslání prvního Bytu
	UART0_UDR = TB_Buf_Out.b[uart0_tx_iptr++];
}


void TB_calcSum(void)
{
	uint8_t i=0, sum=0;
	
	for(i=0; i<8; i++)
	{
		sum += TB_Buf_Out.b[i];
	}
	TB_Buf_Out.b[TB_BUF_SUM] = sum;
}

void TB_Send(uint8_t AddrReply, uint8_t AddrModule, uint8_t Status, uint8_t Command, uint32_t value)
{
	TB_Buf_Out.b[0] = AddrReply;
	TB_Buf_Out.b[1] = AddrModule;
	TB_Buf_Out.b[2] = Status;
	TB_Buf_Out.b[3] = Command;
	TB_Buf_Out.b[4] = (uint8_t) (value >> 24);
	TB_Buf_Out.b[5] = (uint8_t) (value >> 16);
	TB_Buf_Out.b[6] = (uint8_t) (value >> 8);
	TB_Buf_Out.b[7] = (uint8_t) (value >> 0);
	TB_calcSum();
	TB_Send_Com();
}

void TB_SendAck(uint8_t status, uint32_t value)
{
	TB_Buf_Out.b[0] = TB_addr_Reply;
	TB_Buf_Out.b[1] = TB_addr_Module;
	TB_Buf_Out.b[2] = status;
	TB_Buf_Out.b[3] = TB_Buf_In.b[TB_BUF_COMMAND];
	TB_Buf_Out.b[4] = (value >> 24);
	TB_Buf_Out.b[5] = (value >> 16);
	TB_Buf_Out.b[6] = (value >> 8);
	TB_Buf_Out.b[7] = (value >> 0);
	TB_calcSum();
	TB_Send_Com();
}


// Nastavení RS485 - Interní
void TB_Init0( void )
{
	//  DST,   SRC, size
	eeprom_read_block((void *) &TB_gbparam0, (const void *) EEPROM_START_ADDRESS_0, sizeof(struct TB_GBPARAM));
	if (TB_gbparam0.eemagic != 66)
	{
		// not valid data in eeprom
		TB_gbparam0.eemagic = 66;
		TB_gbparam0.baud = 10;
		TB_gbparam0.address = MODULE_ADDRESS;
		TB_gbparam0.telegram_pause_time = 0;
		TB_gbparam0.host_address = 2;
		// save default setting to eeprom
		eeprom_write_block((void *) &TB_gbparam0, (void *) EEPROM_START_ADDRESS_0, sizeof(struct TB_GBPARAM));
	}
	else
	{
		// zvolíme správnou komunikaèní rychlost:
		uart0_set_baud(TB_gbparam0.baud);
	}
	// poznaèíme si adresy
	TB_addr_Reply = TB_gbparam0.host_address;
	TB_addr_Module = TB_gbparam0.address;
}


/************************************************************************/
/*  Èást zamìøená na uart1                                              */
/************************************************************************/

// Nastavení USB
void TB_Init1( void )
{
	//  DST,   SRC, size
	eeprom_read_block((void *) &TB_gbparam1, (const void *) EEPROM_START_ADDRESS_1, sizeof(struct TB_GBPARAM));
	if (TB_gbparam1.eemagic != 66)
	{
		// not valid data in eeprom
		TB_gbparam1.eemagic = 66;
		TB_gbparam1.baud = 7;
		TB_gbparam1.address = MODULE_ADDRESS;
		TB_gbparam1.telegram_pause_time = 0;
		TB_gbparam1.host_address = 2;
		// save default setting to eeprom
		eeprom_write_block((void *) &TB_gbparam1, (void *) EEPROM_START_ADDRESS_1, sizeof(struct TB_GBPARAM));
	}
	else
	{
		// zvolíme správnou komunikaèní rychlost:
		uart1_set_baud(TB_gbparam1.baud);
	}
	// poznaèíme si adresy
	TB_addr_Reply = TB_gbparam1.host_address;
	TB_addr_Module = TB_gbparam1.address;
}