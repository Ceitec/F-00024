/*
 * board.c
 *
 * Created: 10.2.2017 15:24:44
 *  Author: atom2
 */ 

#include <avr/io.h>

#include "def_init.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <string.h>
#include "common_defs.h"
#include "trinamic.h"


void Init_Board( void )
{
	// Nastavení RS485 Enable
	sbi(DDRD, DDD4);
	// UART0 RX
	cbi(DDRD, DDD0);
	// UART1 RX
	cbi(DDRD, DDD2);
	// UART0 TX
	sbi(DDRD, DDD1);
	// UART1 TX
	sbi(DDRD, DDD3);
	// LED1
	sbi(DDRD, DDD6);
	// LED2
	sbi(DDRD, DDD7);
	
	cbi(PORTD, PORTD7);
	cbi(PORTD, PORTD6);
	
	// Testovací výstupy
	sbi(DDRA, DDA3);
	sbi(DDRA, DDA4);
	sbi(DDRA, DDA5);
	sbi(DDRA, DDA6);

	
	// Povolení pøijmu dat
 	//RS485_EN_INT_receive;
	RS485_EN_EXT_receive;
}