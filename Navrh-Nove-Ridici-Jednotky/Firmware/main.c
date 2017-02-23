/*
 * Firmware.c
 *
 * Created: 22.2.2017 18:34:50
 * Author : atom2
 */ 

/*
Nastavení fuse bits
	High		0x91
	Low			0xD7
	Extended	0xFC
	Bez Bootloader režimu
*/



#include <avr/io.h>

#include "inc/def_init.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <string.h>
#include "inc/common_defs.h"
#include "inc/def_init.h"
#include "inc/uart0.h"
#include "inc/uart1.h"
#include "inc/trinamic.h"
#include "inc/timer.h"
#include "inc/board.h"


uint8_t uart0_error=0,uart1_error=0;

// Deklarování promìnných z uart0.c
uint8_t uart0_rx_ptr;
uint8_t uart0_buf_rx[BUFFER_CHAR_PACKET];
uint8_t uart1_buf_tx[BUFFER_CHAR_PACKET];

uint8_t uart1_rx_ptr;
uint8_t uart1_buf_rx[BUFFER_CHAR_PACKET];
uint8_t uart1_buf_tx[BUFFER_CHAR_PACKET];

uint8_t uart1_tx_flag;
uint8_t uart1_tx_iptr;

uint8_t uart1_rx_flag;
uint8_t uart1_rx_iptr;

uint8_t uart0_tx_flag;
uint8_t uart0_tx_ptr;
uint8_t uart0_tx_iptr;

uint8_t uart0_rx_flag;
uint8_t uart0_rx_ptr;
uint8_t uart0_rx_iptr;


Trinamicpac TB_Buf_In;
Trinamicpac TB_Buf_Out;


uint32_t UB = 0;

uint16_t citacka0, citacka1;

#define ADDRESS_EXT	1


void try_receive_data_uart0( void )
{
	uint8_t j=0;
	j = check_uart0();
	if (j==1)
	{
		// Naplní Buffer trinamic
		TB_buf_fill_In();
		LED1_ON;
		// Kontrola adresy
		if (TB_Buf_In.n.addr == TB_gbparam0.address)
		{
			// Rozebrání pøíkazu
			switch (TB_Process0())
			{
				// Zde se vypisují pøíkazy uživatelské, které chceme implementovat
				case TB_CMD_SIO:
				switch(TB_Buf_In.b[TB_BUF_TYPE])
				{
					case 0:
					switch(TB_Buf_In.b[TB_BUF_MOTOR])
					{
						case 0:
						TB_SendAck(TB_ERR_OK, 0);
						break;
						default:
						TB_SendAck(TB_ERR_MOTOR, TB_Buf_In.b[TB_BUF_MOTOR]);
						break;
					}
					break;
					default:
					TB_SendAck(TB_ERR_TYPE, TB_Buf_In.b[TB_BUF_TYPE]);
					break;
				}
				case TB_CMD_GIO:
				switch(TB_Buf_In.b[TB_BUF_TYPE])
				{
					case 0:
					switch(TB_Buf_In.b[TB_BUF_MOTOR])
					{
						case 0:
						TB_SendAck(TB_ERR_OK, 0);
						break;
						default:
						TB_SendAck(TB_ERR_MOTOR, TB_Buf_In.b[TB_BUF_MOTOR]);
						break;
					}
					break;
					default:
					TB_SendAck(TB_ERR_TYPE, TB_Buf_In.b[TB_BUF_TYPE]);
					break;
				}
			}
		}
		else if (TB_Buf_In.n.addr >= ADDRESS_EXT)
		{
			// Povolení odesílání dat na externí linku
			uart1_tx_flag=TRUE;
			RS485_EN_EXT_transmite;
			UART1_UDR = TB_Buf_In.b[uart1_tx_iptr++];
		}
	}
	else if(j==2)
	{
		TB_SendAck(TB_ERR_SUM, 0);
	}
}

void try_receive_data_uart1( void )
{
	uint8_t j=0;
	j = check_uart1();
	if (j==1)
	{	
		// Naplní Buffer trinamic
		TB_buf_fill_Out();
		// Kontrola adresy
		// Povolení odesílání dat na externí linku (Nastartování pøerušení)
		uart0_tx_flag=TRUE;
		//RS485_EN_INT_transmite;
		// Odeslání prvního Bytu
		UART0_UDR = TB_Buf_Out.b[uart0_tx_iptr++];
	}
}

int main(void)
{
	
	UB = UART0_DEFAULT_BAUD;
	uart0_error = uart0_init(UB);
	UB = UART1_DEFAULT_BAUD;
	uart1_error = uart1_init(UB);
	uart0_interrupt_rx(TRUE);
	uart0_interrupt_tx(TRUE);
	uart1_interrupt_rx(TRUE);
	uart1_interrupt_tx(TRUE);
	
	TB_Init0();
	TB_Init1();
	
	Init_Board();
	
	// Povolení globálního pøerušení
	sei();

	
	// Hlavní smyèka programu
	while (1) 
    {
		
		// Odeslání uart0 na uart1, nebo-li data z USB na RS485.
 		try_receive_data_uart0();

		try_receive_data_uart1();
		

		simple_flag_null0();
		
		simple_flag_null1();

    }
}



