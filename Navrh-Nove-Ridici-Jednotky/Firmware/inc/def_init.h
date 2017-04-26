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
#define MODULE_ADDRESS MCE


#define	MCE	0	// Module Control electronic
#define MOT	1	// Module 6 Step Motor
#define MPI	3	// Module primary input (Osvìtlovaè)
#define MPR	5	// Module Pressure regulation
#define MIL	8	// Module Interlock system
#define MGP	9	// Module gas purge (Ofuk)
#define MSH	10	// Module Sample holder
#define MMB	11	// Module MotherBoard
#define DDG1	12	// Module DelayGenerator
#define MGE	13	// Module gas purge (Ofuk)
#define DDG2	14	// Module DelayGenerator
#define DBB	15	// Module DelayGenerator
#define MGO	16	// Module General purpose Output
#define MRSCON	17	// Module RS485 Converter
#define MET	100	// Module External test
#define MBS	101	// Module Basic Slave


// Deska Slave - Základní firmware pro Slave zaøízení
//#define MODULE_TYPE	200	
// Deska Pøevodník RS485 - Pøevodník z interní na externí RS485
//#define MODULE_TYPE	201	
// Deska øídící jednotky - USB na RS485
#define MODULE_TYPE	202
// Deska MET
//#define MODULE_TYPE	203
// Deska DBB
//#define MODULE_TYPE	204
// Deska DDG
//#define MODULE_TYPE	205
// Deska MGE
//#define MODULE_TYPE	206
// Deska MMB
//#define MODULE_TYPE	207
// Deska MSH
//#define MODULE_TYPE	208
// Deska MGP
//#define MODULE_TYPE	209
// Deska MIL
//#define MODULE_TYPE	210
// Deska MPR
//#define MODULE_TYPE	211
// Deska MGO
//#define MODULE_TYPE	212
// Deska MPI
//#define MODULE_TYPE	213
// Deska MOT
//#define MODULE_TYPE	214
// Deska MCE
//#define MODULE_TYPE	215
// Deska MBS
//#define MODULE_TYPE	216

#define LED2_ON		sbi(PORTD, PORTD6);
#define LED2_OFF	cbi(PORTD, PORTD6);
#define LED1_ON		sbi(PORTD, PORTD7);
#define LED1_OFF	cbi(PORTD, PORTD7);



#endif /* DEF_INIT_H_ */