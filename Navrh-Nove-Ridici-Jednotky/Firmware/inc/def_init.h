/*
 * def_init.h
 *
 * Created: 16.1.2017 17:57:17
 *  Author: atom2
 */ 


#ifndef DEF_INIT_H_
#define DEF_INIT_H_

//Velikost pou�it�ho krystalu
#define F_CPU 16000000UL

//Velikost bufferu pro p��jem dat
#define	BUFFER_CHAR_PACKET	18 // Mus� b�t n�sobky 9!!!

// Adresa modulu
#define MODULE_ADDRESS 0

// Deska Slave - Z�kladn� firmware pro Slave za��zen�
//#define MODULE_TYPE	200
// Deska P�evodn�k RS485 - P�evodn�k z intern� na extern� RS485
//#define MODULE_TYPE	201
// Deska ��d�c� jednotky - USB na RS485
#define MODULE_TYPE	202

#define LED2_ON		sbi(PORTD, PORTD6);
#define LED2_OFF	cbi(PORTD, PORTD6);
#define LED1_ON		sbi(PORTD, PORTD7);
#define LED1_OFF	cbi(PORTD, PORTD7);



#endif /* DEF_INIT_H_ */