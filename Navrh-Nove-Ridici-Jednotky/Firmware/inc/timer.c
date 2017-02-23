/*
 * timer.c
 *
 * Created: 1.2.2017 11:01:37
 *  Author: atom2
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <string.h>


volatile uint16_t citacka0=0;
volatile uint16_t citacka1=0;

uint8_t uart0_rx_ptr;
uint8_t uart0_rx_iptr;

uint8_t uart1_rx_ptr;
uint8_t uart1_rx_iptr;

void simple_flag_null0( void )
{
	if (citacka0 > 10000)
	{
		// Vymaže každách 50ms flag interní èítaè
		uart0_rx_iptr=0;
		uart0_rx_ptr=0;
		citacka0=0;
	}
	citacka0++;
}

void simple_flag_null1( void )
{
	if (citacka1 > 10000)
	{
		// Vymaže každách 50ms flag interní èítaè
		uart1_rx_iptr=0;
		uart1_rx_ptr=0;
		citacka1=0;
	}
	citacka1++;
}