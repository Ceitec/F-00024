/*
 * def_init.h
 *
 * Created: 16.1.2017 17:57:17
 *  Author: atom2
 */ 


#ifndef DEF_INIT_H_
#define DEF_INIT_H_

//Velikost použitého krystalu
#define F_CPU 16000000UL

//Velikost bufferu pro pøíjem dat
#define	BUFFER_CHAR_PACKET	18 // Musí být násobky 9!!!

// Adresa modulu
#define MODULE_ADDRESS 0

// Deska Slave - Základní firmware pro Slave zaøízení
//#define MODULE_TYPE	200
// Deska Pøevodník RS485 - Pøevodník z interní na externí RS485
//#define MODULE_TYPE	201
// Deska øídící jednotky - USB na RS485
#define MODULE_TYPE	202

#define LED2_ON		sbi(PORTD, PORTD6);
#define LED2_OFF	cbi(PORTD, PORTD6);
#define LED1_ON		sbi(PORTD, PORTD7);
#define LED1_OFF	cbi(PORTD, PORTD7);



#endif /* DEF_INIT_H_ */