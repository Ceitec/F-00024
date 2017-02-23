/*
 * uart1.h
 *
 * Created: 30.1.2017 13:57:03
 *  Author: atom2
 */ 


#ifndef UART1_H_
#define UART1_H_


#define UART1_BUFFER	
#define UART1_DEFAULT_BAUD		115200
#define DEFAULT_TIMEOUT		5

		
#define UART1_UDR		UDR1
#define UART1_UCSRC		UCSR1C
#define UART1_UCSRB		UCSR1B
#define UART1_UCSRA		UCSR1A
#define UART1_UBRRL		UBRR1L				
#define UART1_UBRRH		UBRR1H
#define UART1_RXIE		RXCIE1
#define UART1_TXIE		TXCIE1
#define UART1_RX_vect	USART1_RX_vect
#define UART1_TX_vect	USART1_TX_vect
#define UART1_RXEN		RXEN1
#define UART1_TXEN		TXEN1
#define UART1_UDRE		UDRE1


#define UART1_UCSZ10	UCSZ10
#define	UART1_UCSZ11	UCSZ11
#define UART1_UCSZ12	UCSZ12



/* Deklarování funkcí */

uint8_t uart1_init(uint32_t MYUBRR0);
uint8_t uart1_interrupt_rx(uint8_t enable);
uint8_t uart1_interrupt_tx(uint8_t enable);
uint8_t uart1_ptr_ask();
void uart1_receive_char(uint8_t data);
uint8_t check_uart1( void );
void uart1_transmit_char(uint8_t data);
void uart1_set_baud(uint8_t baud);




#endif /* UART1_H_ */