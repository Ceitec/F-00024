/*
 * uart1.c
 *
 * Created: 30.1.2017 13:56:41
 *  Author: atom2
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "common_defs.h"
#include "def_init.h"
#include "uart1.h"
#include "def_init.h"
#include <avr/interrupt.h>
#include "trinamic.h"
#include "timer.h"


#define MACRO_BAUDRATE(BAUDRATE) (UART1_UBRRL = (((F_CPU) / (BAUDRATE * 16UL)) - 1 + (F_CPU % (BAUDRATE * 16UL) > (BAUDRATE * 8UL) ? 1 : 0)))


struct tmp
{
	uint8_t	tmpData;
	uint8_t tmpStatus;
	uint8_t	tmpTimer;
};

struct tmp tmpUart1;


volatile uint8_t uart1_buf_rx[BUFFER_CHAR_PACKET];
volatile uint8_t uart1_buf_tx[BUFFER_CHAR_PACKET];

volatile uint8_t uart1_sum=0;
volatile uint8_t uart1_i=0;
volatile uint8_t uart1_ret=0, uart1_check_sum=0;

volatile uint8_t uart1_tx_flag=FALSE;
volatile uint8_t uart1_tx_iptr=0;
volatile uint8_t uart1_tx_ptr=0;

volatile uint8_t uart1_rx_flag=FALSE;
volatile uint8_t uart1_rx_iptr=0;
volatile uint8_t uart1_rx_ptr=0;


Trinamicpac TB_Buf_In;

uint16_t citacka0, citacka1;


uint8_t uart1_init(uint32_t MYUBRR1)
{
	uint16_t UBRR1_COUNT = 0;
	// Výpoèet rychlosti
	UBRR1_COUNT = (((F_CPU) / (MYUBRR1 * 16UL)) - 1 + (F_CPU % (MYUBRR1 * 16UL) > (MYUBRR1 * 8UL) ? 1 : 0));
	//UBRR1_COUNT = MYUBRR
	// Nastavení UBRR0 pro rychlost
	UART1_UBRRL = (unsigned char) (UBRR1_COUNT);
	UART1_UBRRH = (unsigned char) (UBRR1_COUNT >> 8);
	// Povolení RX a TX pinù
	UART1_UCSRB |= BV(UART1_TXEN) | BV(UART1_RXEN);
	// 8bit, 1stop, no parity
	UART1_UCSRC |= BV(UART1_UCSZ10) | BV(UART1_UCSZ11);
	return 0;
}

uint8_t uart1_interrupt_rx(uint8_t enable)
{
	if (enable)
		UART1_UCSRB |= BV(UART1_RXIE);
	else
		UART1_UCSRB &= BV(UART1_RXIE);
	
	return 0;
}

uint8_t uart1_interrupt_tx(uint8_t enable)
{
	if (enable)
		UART1_UCSRB |= BV(UART1_TXIE);
	else
		UART1_UCSRB &= BV(UART1_TXIE);
	
	return 0;
}

uint8_t uart1_ptr_ask()
{
	return uart1_rx_ptr;
}


void uart1_receive_char(uint8_t data)
{
	uart1_buf_rx[uart1_rx_ptr] = data;
	uart1_rx_ptr++;
	uart1_rx_iptr++;
}


ISR(UART1_RX_vect)
{
	citacka1=0;
	LED1_OFF;
 	tmpUart1.tmpData = UART1_UDR;
 	tmpUart1.tmpStatus = UART1_UCSRA;
	tmpUart1.tmpTimer =	DEFAULT_TIMEOUT;
 	uart1_receive_char(tmpUart1.tmpData);
}

ISR(UART1_TX_vect)
{
	if (uart1_tx_flag)
	{
		//Odeslání 9 bytù dat
		if (uart1_tx_iptr > 8)
		{
			// Vypnutí odesílání a povolení pøíjmu
			uart1_tx_flag = FALSE;
			RS485_EN_EXT_receive;
			uart1_tx_iptr=0;
		}
		else
		{
			UART1_UDR = TB_Buf_In.b[uart1_tx_iptr];
			uart1_tx_iptr++;
		}
		uart1_rx_iptr=0;
		uart1_rx_ptr=0;
	}
}



uint8_t check_uart1( void )
{
	// Vnitøní èítaè 9 pøíchozích Bytù
	if (uart1_rx_iptr > 8)
	{
		uart1_rx_flag = TRUE;
	}
	// Vypnutí pøerušení pøed kontrolou dat
	cli();
	if (uart1_rx_flag)
	{
		uart1_sum=0;
		uart1_i=0;
		for (uart1_i=9; uart1_i>1; uart1_i--)
		{
			uart1_sum += uart1_buf_rx[uart1_rx_ptr-uart1_i];
		}
		uart1_check_sum = uart1_buf_rx[uart1_rx_ptr-1];
		if (uart1_sum == uart1_check_sum)
		{
			uart1_ret = 1;
			uart1_rx_iptr=0;
		}
		else
		{
			uart1_rx_iptr=0;
			uart1_rx_ptr=0;
			uart1_ret=2;
		}
		// Vynulování crc
		uart1_check_sum=0;
	}
	else
	{
		uart1_ret = 0;
	}
	sei();
	uart1_rx_flag=FALSE;
	return uart1_ret;
	
}


void uart1_transmit_char(uint8_t data)
{
	while ( !( UART1_UCSRA & (1 << UART1_UDRE)) );	UART1_UDR = data;}void uart1_set_baud(uint8_t baud)
{
	// UART port
	switch (baud) {
		case 0:
			MACRO_BAUDRATE(9600);
			break;
		case 1:
			MACRO_BAUDRATE(14400);
			break;
		case 2:
			MACRO_BAUDRATE(19200);
			break;
		case 3:
			MACRO_BAUDRATE(28800);
			break;
		case 4:
			MACRO_BAUDRATE(38400);
			break;
		case 5:
			MACRO_BAUDRATE(57600);
			break;
		case 6:
			MACRO_BAUDRATE(76800);
			break;
		case 7:
			MACRO_BAUDRATE(115200);
			break;
		case 8:
			MACRO_BAUDRATE(230400);
			break;
		case 9:
			MACRO_BAUDRATE(250000);
			break;
		case 10:
			MACRO_BAUDRATE(500000);
			break;
		case 11:
			MACRO_BAUDRATE(1000000);
			break;
		default:
			MACRO_BAUDRATE(115200);
			break;
	}
	return;
}