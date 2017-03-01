/*
 * Define_Setup.h
 *
 * Created: 12.4.2016 10:11:35
 *  Author: atom2
 */ 


#ifndef DEFINE_SETUP_H_
#define DEFINE_SETUP_H_



#define SIGNAT_BYTE_0	0x00
#define SIGNAT_BYTE_1	0x02
#define	SIGNAT_BYTE_2	0x04
// Verze 00 je k ZM
#define	BOOTLOADER_VERSION	"00"
//Verze 01 je k RS485
//#define	BOOTLOADER_VERSION	01
//#define SOFTWARE_IDENTIFIER 32000
//#define	BOOT_SIZE	512
//#define	START_ADDRESS	0x7E00
//#define	PAGES	4
//#define	BOOT_SIZE	1024
//#define	START_ADDRESS	0x7C00
//#define	PAGES	8
//#define	BOOT_SIZE	2048
//#define	START_ADDRESS	0x7800
//#define	PAGES	16


// Velikost BOOTLOADER sekce p�i ur�it�m nastaven� (Datasheet str.290)
/*Bootloader pro Atmega644p*/	
#define	BOOTLOADER	4096
/*Bootloader pro Atmega32*///		#define	BOOTLOADER	2048
// Velikost cel� FLASH pam�ti 64kBytes
/*Bootloader pro Atmega644p*/
#define FLASH		65536
/*Bootloader pro Atmega32*///	#define FLASH		32768

// Jedno slovo je velik� 16 bit� (2 BYTES)
#define ONE_WORDS	2
// Po�et slov pro velikost BOOTLOADER sekce 4096 (Datasheet str.290)
#define	BOOT_WORDS	(SPM_PAGESIZE / 2)		// 128
// Po�et slov pro velikost BOOTLOADER sekce 4096 (Datasheet str.290)
#define	BOOT_BYTES	SPM_PAGESIZE			// 256
// Po�et str�nek (Datasheet str.290) 256 PAGES
#define PAGE_SIZE		(FLASH / SPM_PAGESIZE)					// 65536 / 256 = 256 --- Kolik str�nek m� �ip
// Velikost BOOTLOADER ��sti o velikosti PAGES (Str�nek) 32 PAGES
#define	BOOT_SIZE		(BOOTLOADER / BOOT_WORDS)				// 32 Pages in BootLoader Section
// Velikost APPLICATION PAGES (Applika�n�ch str�nek) 224 PAGES
#define APP_SIZE		(PAGE_SIZE - BOOT_SIZE)					// 224 Pages in Application Section
// Address Flash in Words
#define START_APP_ADDRESS	0x0000								// Start Address 0x0000
#define	END_APP_ADDRESS		((APP_SIZE * BOOT_WORDS) - 1)		// Last address in Application in Words 0x6FFF
#define	START_BOOT_ADDRESS	(APP_SIZE * BOOT_WORDS)				// First address BootLoader in Words - 0x7000
#define END_ADDRESS			((PAGE_SIZE * BOOT_WORDS) - 1)		// Last address Flash in Words - 0x7FFF
#define FLASH_SIZE			(PAGE_SIZE * BOOT_WORDS)			// Last address Flash in Words - 0x8000
// Address Flash in Bytes
#define START_APP_ADDRESS_BYTES		0x0000								// Start Address 0x0000
#define	END_APP_ADDRESS_BYTES		((APP_SIZE * BOOT_BYTES) - 1)		// Last address in Application in Words 0xDFFF
#define	START_BOOT_ADDRESS_BYTES	(APP_SIZE * BOOT_BYTES)				// First address BootLoader in Words - 0xE000
#define END_ADDRESS_BYTES			((PAGE_SIZE * BOOT_BYTES) - 1)		// Last address Flash in Words - 0xFFFF
#define FLASH_SIZE_BYTES			(PAGE_SIZE * BOOT_BYTES)			// Last address Flash in Words - 0x10000

// Address EEPROM in Bytes
#define START_EEPROM_ADDRESS 0x0000
#define	END_EEPROM_ADDRESS	(E2END + 1)
//#define NUM_O_PAGES_EEPROM		256
#define PAGE_SIZE_EEPROM		SPM_PAGESIZE


#define ACK	0x06
#define NAK	0x15


#endif /* DEFINE_SETUP_H_ */