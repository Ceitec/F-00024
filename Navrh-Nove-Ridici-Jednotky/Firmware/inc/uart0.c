/*
 * uart0.c
 *
 * Created: 16.1.2017 17:39:01
 *  Author: atom2
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "common_defs.h"
#include "def_init.h"
#include "uart0.h"
#include "def_init.h"
#include <avr/interrupt.h>
#include "trinamic.h"
#include "timer.h"


#define MACRO_BAUDRATE(BAUDRATE) (UART0_UBRRL = (((F_CPU) / (BAUDRATE * 16UL)) - 1 + (F_CPU % (BAUDRATE * 16UL) > (BAUDRATE * 8UL) ? 1 : 0)))


struct tmp
{
	uint8_t	tmpData;
	uint8_t tmpStatus;
	uint8_t	tmpTimer;
};

struct tmp tmpUart0;



volatile uint8_t uart0_buf_rx[BUFFER_CHAR_PACKET];
volatile uint8_t uart0_buf_tx[BUFFER_CHAR_PACKET];

volatile uint8_t uart0_sum=0;
volatile uint8_t uart0_i=0;
volatile uint8_t uart0_ret=0, uart0_check_sum=0;

volatile uint8_t uart0_tx_flag=FALSE;
volatile uint8_t uart0_tx_ptr=0;
volatile uint8_t uart0_tx_iptr=0;

volatile uint8_t uart0_rx_flag=FALSE;
volatile uint8_t uart0_rx_ptr=0;
volatile uint8_t uart0_rx_iptr=0;

Trinamicpac TB_Buf_Out;
uint8_t uart1_buf_rx[BUFFER_CHAR_PACKET];
uint8_t uart1_rx_iptr;

uint16_t citacka0, citacka1;


uint8_t uart0_init(uint32_t MYUBRR0)
{
	uint16_t UBRR0_COUNT = 0;
	// Výpoèet rychlosti
	UBRR0_COUNT = (((F_CPU) / (MYUBRR0 * 16UL)) - 1 + (F_CPU % (MYUBRR0 * 16UL) > (MYUBRR0 * 8UL) ? 1 : 0));
	// Nastavení UBRR0 pro rychlost
	UART0_UBRRL = (unsigned char) (UBRR0_COUNT);
	UART0_UBRRH = (unsigned char) (UBRR0_COUNT >> 8);
	// Povolení RX a TX pinù
	UART0_UCSRB |= BV(UART0_TXEN) | BV(UART0_RXEN);
	// 8bit, 1stop, no parity
	UART0_UCSRC |= BV(UART0_UCSZ00) | BV(UART0_UCSZ01);
	return 0;
}

uint8_t uart0_interrupt_rx(uint8_t enable)
{
	if (enable)
		UART0_UCSRB |= BV(UART0_RXIE);
	else
		UART0_UCSRB &= BV(UART0_RXIE);
	
	return 0;
}

uint8_t uart0_interrupt_tx(uint8_t enable)
{
	if (enable)
		UART0_UCSRB |= BV(UART0_TXIE);
	else
		UART0_UCSRB &= BV(UART0_TXIE);
	
	return 0;
}

uint8_t uart0_ptr_ask()
{
	return uart0_rx_ptr;
}

void uart0_receive_char(uint8_t data)
{
	
	uart0_buf_rx[uart0_rx_ptr] = data;
	uart0_rx_ptr++;
	uart0_rx_iptr++;
}


ISR(UART0_RX_vect)
{
	citacka0=0;
	LED2_OFF;
	nbi(PORTA, DDA5);
	tmpUart0.tmpData = UART0_UDR;
 	tmpUart0.tmpStatus = UART0_UCSRA;
	tmpUart0.tmpTimer =	DEFAULT_TIMEOUT;
 	uart0_receive_char(tmpUart0.tmpData);
}

ISR(UART0_TX_vect)
{
	nbi(PORTA, DDA6);
	if (uart0_tx_flag)
	{
		//Odeslání 9 bytù dat
		if (uart0_tx_iptr > 8)
		{
			// Vypnutí odesílání a povolení pøíjmu
			uart0_tx_flag = FALSE;
			//RS485_EN_INT_receive;
			uart0_tx_iptr=0;
		}
		else
		{
			UART0_UDR = TB_Buf_Out.b[uart0_tx_iptr];
			uart0_tx_iptr++;
		}
		uart0_rx_iptr=0;
		uart0_rx_ptr=0;
	}
}




uint8_t check_uart0( void )
{
	// Vnitøní èítaè 9 pøíchozích Bytù
	if (uart0_rx_iptr > 8)
	{
		uart0_rx_flag = TRUE;
	}
	// Vypnutí pøerušení pøed kontrolou dat
	
	if (uart0_rx_flag)
	{
		cli();
		uart0_sum=0;
		uart0_i=0;
		for (uart0_i=9; uart0_i>1; uart0_i--)
		{
			uart0_sum += uart0_buf_rx[uart0_rx_ptr-uart0_i];
		}
		uart0_check_sum = uart0_buf_rx[uart0_rx_ptr-1];
		if (uart0_sum == uart0_check_sum)
		{
			uart0_ret = 1;
			uart0_rx_iptr=0;
		}
		else
		{
			uart0_rx_iptr=0;
			uart0_rx_ptr=0;
			uart0_ret=2;
		}
		// Vynulování crc
		uart0_check_sum=0;
		sei();
	}
	else
	{
		uart0_ret = 0;
	}
	
	uart0_rx_flag=FALSE;
	return uart0_ret;
	
}


void uart0_transmit_char(uint8_t data)
{
	while ( !( UART0_UCSRA & (1 << UART0_UDRE)) );	UART0_UDR = data;
}

void uart0_set_baud(uint8_t baud)
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