/*
 * uart0.h
 *
 * Created: 16.1.2017 17:39:54
 *  Author: atom2
 */ 


#ifndef UART0_H_
#define UART0_H_


#define UART0_BUFFER	
#define UART0_DEFAULT_BAUD		500000
#define DEFAULT_TIMEOUT		5

		
#define UART0_UDR		UDR0
#define UART0_UCSRC		UCSR0C
#define UART0_UCSRB		UCSR0B
#define UART0_UCSRA		UCSR0A
#define UART0_UBRRL		UBRR0L				
#define UART0_UBRRH		UBRR0H
#define UART0_RXIE		RXCIE0
#define UART0_TXIE		TXCIE0
#define UART0_RX_vect	USART0_RX_vect
#define UART0_TX_vect	USART0_TX_vect
#define UART0_RXEN		RXEN0
#define UART0_TXEN		TXEN0
#define UART0_UDRE		UDRE0


#define UART0_UCSZ00	UCSZ00
#define	UART0_UCSZ01	UCSZ01
#define UART0_UCSZ02	UCSZ02


/* Deklarování funkcí */

uint8_t uart0_init(uint32_t MYUBRR0);
uint8_t uart0_interrupt_rx(uint8_t enable);
uint8_t uart0_interrupt_tx(uint8_t enable);
uint8_t uart0_ptr_ask();
void uart0_receive_char(uint8_t data);
uint8_t check_uart0( void );
void uart0_transmit_char(uint8_t data);
void uart0_set_baud(uint8_t baud);


#endif /* UART0_H_ */