/*

Program: Bronkhorst
Knihovna: RS232.c
Vlastník: Lukáš Novák

*/
//#define F_CPU 14745600UL
#include <util/delay.h>
#include <avr/io.h>
#include "RS232.h"


//////////////////////////////////////////////////////////////////////
/*		Inicializaèní procedura obsahující nastavení odesílání,		*/
/*		pøíjem dat a povolení pøerušení pøíjmu po sériové lince.	*/
/*			Dále nastavení R/T pomocí 8 bitù a 1 stop bit.			*/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/*						Vytvoøenopro uC Atmega328P						*/
/*																		*/
//////////////////////////////////////////////////////////////////////////

//TXD0
//RXD0

void RS232_Init( unsigned int baud )
{
	
	UBRR0H = (unsigned char)(baud>>8);							//Registr pro nastavení vrchních bytù rychlosti sériové linky.
	UBRR0L = (unsigned char)baud;								//Registr pro nastavení spodních bytù rychlosti sériové linky.
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);		//Povolení v registru odesílání, pøíjmu a povolení pøerušení pøíjmu
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);						//Nastavení R/T 8 bitù data, 1 stop bitu, asynchronní pøenos a bez parity.
}

//////////////////////////////////////////////////////////////////////////
/*				Procedura pro pøíjem jednoho znaku a ukládání			*/
/*					z registru UDR do vrácené promìnné.					*/
//////////////////////////////////////////////////////////////////////////
uint8_t	RS232_Receive_Status(void)
{
	if(UCSR0A & (1<<RXC0))
	{
		return UDR0;
	}
	else
	{
		return 0;
	}
}

unsigned char RS232_Receive_Char( void )
{
  unsigned char data;
  while ( !(UCSR0A & (1<<RXC0)) );	//Èekání dokud není dokonèen pøenos dat
  data=UDR0;							//Ètení dat z registru UDR a ukládání do promìnné
  return data;						//Vrácení pøijmutého znaku
}

//////////////////////////////////////////////////////////////////////////
/*  		Procedura pro prijem vice znaku po sériové lince.			*/
//////////////////////////////////////////////////////////////////////////

void RS232_Receive_String( void )
{
	int RxBPtr=0;
	RxBPtr = 256;
	char RxB[RxBPtr];
	while (RS232_Receive_Char() != 0x0A)
	{
		RxB[RxBPtr] = RS232_Receive_Char();
		RS232_Transmit_Char(RxB[RxBPtr]);
		RxBPtr++;
		
	}
}

//////////////////////////////////////////////////////////////////////////
/*		Procedura pro odesílání jednoho znaku po sériové lince.			*/
//////////////////////////////////////////////////////////////////////////

void RS232_Transmit_Char( unsigned char data )
{
  while ( !( UCSR0A & (1<<UDRE0)) );	//Èekání dokud není prázdný buffer
  UDR0 = data;						//Nastavení znaku do registru
}

// UINT8_T Data
void RS232_Transmit_uint8( uint8_t data )
{
	while ( !( UCSR0A & (1<<UDRE0)) );	//Èekání dokud není prázdný buffer
	UDR0 = data;						//Nastavení znaku do registru
}

// UINT16_T Data
void RS232_Transmit_uint16( uint16_t data )
{
	RS232_Transmit_uint8( data >> 8 );
	RS232_Transmit_uint8( data );
}


// Odešle se pouze CR
void RS232_Transmit_Char_CR(void)
{
	while ( !( UCSR0A & (1<<UDRE0)) );	//Èekání dokud není prázdný buffer
	UDR0 = 0x0D;						//Nastavení znaku do registru
}

//////////////////////////////////////////////////////////////////////////
/*			Rozšíøení procedury pro odesílání jednoho znaku,			*/
/*						 na odesílání pole znakù.						*/
//////////////////////////////////////////////////////////////////////////

void RS232_Transmit_String(char* StringPtr){
	
	while(*StringPtr != 0x00)			//Opakování cyklu dokud se nevyprázdní buffer
	{
		RS232_Transmit_Char(*StringPtr);		//Využití procedury pro odeslání jednoho znaku
		StringPtr++;					//Posunutí o další znak
	}
	
}

//////////////////////////////////////////////////////////////////////////
/*				 Upravená procedura odesílání pole znakù				*/
/*			s pøidaným ukonèovacím znakem <CR> pro AT pøíkazy.			*/
//////////////////////////////////////////////////////////////////////////

void RS232_Transmit_String_CR(char* StringPtr){
	
	while(*StringPtr != 0x00)			//Opakování cyklu dokud se nevyprázdní buffer
	{
		RS232_Transmit_Char(*StringPtr);		//Využití procedury pro odesílání jednoho znaku
		StringPtr++;					//Posunutí o další znak
	}
	RS232_Transmit_Char(0x0D);				//Na konci odeslán znak <CR>
}

void RS232_Transmit_String_CRLF(char* StringPtr){
	
	while(*StringPtr != 0x00)			//Opakování cyklu dokud se nevyprázdní buffer
	{
		RS232_Transmit_Char(*StringPtr);		//Využití procedury pro odesílání jednoho znaku
		StringPtr++;					//Posunutí o další znak
	}
	RS232_Transmit_Char(0x0D);				//Na konci odeslán znak <CR>
	RS232_Transmit_Char(0x0A);				//Na konci odeslán znak <LF>
}


void RS232_Transmit_String_Buffer(char* StringPtr, int TXStringPtr)
{
	
//	while(*StringPtr != 0x0A)			//Opakování cyklu dokud se nevyprázdní buffer
	while(TXStringPtr != 0)
	{
		RS232_Transmit_Char(*StringPtr);		//Využití procedury pro odesílání jednoho znaku
		StringPtr++;					//Posunutí o další znak
		TXStringPtr--;
	}
}

