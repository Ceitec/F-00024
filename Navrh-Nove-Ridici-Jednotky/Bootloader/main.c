/*
 * Bootloader.c
 *
 * Created: 24.2.2017 10:12:19
 * Author : atom2
 */ 


#include <avr/io.h>

#include "inc/Define_Setup.h"
#define	F_CPU	16000000UL
#include "inc/AllInit.h"
#include "inc/common_defs.h"
#include "inc/RS232.h"
#include <avr/boot.h>
//#include <avr/fuse.h>

#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "inc/CRC_interface.h"
#include <avr/eeprom.h>



/************************************************************************/
/* Programovací pojistky se musí nastavit
BOOTRSZ = 0
BOOTSZ = "00"
LOW FUSE BITS = 0xD7
HIGH FUSE BITS = 0x90 / Stará verze s D0 bez Jtag
EXTENDED FUSE BITS = 0xFC
																		*/
/************************************************************************/



/************************************************************************/
/* Definování Bufferu, do kterého se vloží pøijatá data                 */
/************************************************************************/

static uint8_t BufferFlash[SPM_PAGESIZE], BufferEeprom[PAGE_SIZE_EEPROM] ={0x01, 0x02, 0x03, 0x04};
static uint16_t	cnt=0;
static uint16_t	i=0;
static uint8_t	BootStatus=0;
uint8_t SOFTWARE_IDENTIFIER[9]="VA000001";

// Skoèení na adresu 0x0000
void	jumpaddress(void)
{
	asm("jmp 0x0000");
}

// První funkce, která zajistí odeslání informací
void FirstFunction(void)
{
	for (cnt = 0; cnt < 250; cnt++)
	//#warning "Pouze Simulator"
	//for (cnt = 0; cnt < 2; cnt++)
	{
		if (RS232_Receive_Status() == 'P')
		{
			RS232_Transmit_String_CR("AtomTraceBootLoader");
			BootStatus = 1;
			break;
		}
		_delay_ms(1);
	}
	// Pouze pro pozici Testování simulátoru
	//#warning	"Pouze Simulator";
	//BootStatus = 1;
	
	if(!BootStatus)
	{
		jumpaddress();
	}
}

// Pøeètení lock bitù
void ReadLockBits(void)
{
	
}

// Vymazání celé aplikaèní pamìti
void ChipErase(void)
{
	uint16_t	address = 0x0000, konec = 0x0000;
	konec = END_APP_ADDRESS_BYTES;
	// Maže od adresy 0x0000 po koneènou adresu Bootloaderu
	while (address < konec)
	{
		// Vymaže pøíslušnou adresu
		
		boot_page_erase(address);
		boot_spm_busy_wait();
		address += SPM_PAGESIZE;
	}
}

void EepromErase(void)
{
	uint16_t	address = 0x0000, konec = 0x0000;
	konec = END_EEPROM_ADDRESS;
	// Maže od adresy 0x0000 po koneènou adresu Bootloaderu
	while (address < konec)
	{
		// Vymaže pøíslušnou adresu
		if(eeprom_read_byte(address) != 0xFF)
		{
			eeprom_update_byte(address, 0xFF);
		}
		address++;
	}
}


// Zapsání Lock bitù
void WriteLockBits(void)
{
	// V této verzi není zápis Lock bitù povolen, kvùli zamknutí MCU...
	//boot_lock_bits_set(RS232_Receive_Char());
	//boot_spm_busy_wait();
}

// Verify Flash pamìti


// Verify EEPROM pamìti

// Zápis po pages do pamìti Flash
void WriteFlashPages(uint16_t address, uint8_t	*Buffer)
{
	uint16_t i;
	uint16_t Data=0;
	for (i = 0; i < SPM_PAGESIZE; i += 2)
	{
		Data = *Buffer++;
		Data |= *Buffer++ << 8;
		// Plní Page buffer (256 velikost u 644p)
		boot_page_fill (address + i, Data);
	}

	boot_page_write (address);     //Naplní buffer
	boot_spm_busy_wait();       // Èeká dokud se neuvolní
}

// Zápis po pages do pamìti EEPROM
void WriteEepromPages(uint16_t address, uint8_t *Buffer)
{
	uint16_t cnt=0;
	for (cnt = 0; cnt < PAGE_SIZE_EEPROM; cnt++)
	{
// 		Data = *Buffer++;
// 		Data |= *Buffer++ << 8;
		eeprom_update_byte(address++, *Buffer++);
	}
}

// Ètení pamìti Flash
void ReadFlashPages(uint8_t end, uint16_t address)
{
	uint16_t Last=0x0000;
	
	// Konec 0 - 0x0000 až konec aplikaèní èásti
	// Konec 1 - Zaèátek Bootloader èásti až konec
	// Konec 2 - Celá pamì
	if (end == 0)
	{
		address = START_APP_ADDRESS_BYTES;
	}
	
	Last = (START_BOOT_ADDRESS_BYTES - PAGE_SIZE);
	while (address <= Last)
	{
		
		RS232_Transmit_uint16(address);
		for ( i = 0; i < SPM_PAGESIZE; i++)
		{
			RS232_Transmit_Char(pgm_read_byte(address + i));
		}
		RS232_Transmit_Char_CR();
		if (end == 1)
		{
			break;
		}
		address += SPM_PAGESIZE;
	}
}

void ReadEepromPages(uint8_t end, uint16_t address)
{
	uint16_t Last=0x0000;
	
	if (end == 0)
	{
		address = START_EEPROM_ADDRESS;
	}
	Last = (END_EEPROM_ADDRESS - PAGE_SIZE_EEPROM);
	
	while (address <= Last)
	{
		RS232_Transmit_uint16(address);
		for ( i = 0; i < PAGE_SIZE_EEPROM; i++)
		{
			RS232_Transmit_Char(eeprom_read_byte(address + i));
		}
		RS232_Transmit_Char_CR();
		if (end == 1)
		{
			break;
		}
		address += E2PAGESIZE;
	}
}


int main(void)
{
	
    /* Replace with your application code */

	uint8_t ID_Data=0;
	uint16_t	address=0x0000;
	uint8_t	all=0x00;
	
	cli();
	RS232_Init(RS232_115200);
	
	// Spuštìní režimu BOOTloaderu
	
	FirstFunction();
	
    while (1) 
    {
		
		ID_Data = RS232_Receive_Char();
		//#warning "Pouze Simulator"
		//ID_Data = 'g';
		
		switch (ID_Data)
		{
			// Read Lock Bits
			case 'r':
				RS232_Transmit_Char(boot_lock_fuse_bits_get(GET_LOCK_BITS));
				RS232_Transmit_Char(ACK);
				RS232_Transmit_Char_CR();
				break;
			// Chip Erase
			case 'e':
				// Pøijme 1 Byte, který rozhodne jestli EEPROM, Flash, nebo oboje
				ID_Data = RS232_Receive_Char();
				
				//#warning "Pouze Simulator"
				//ID_Data = 'F';
				// Chceme Flash nebo EEProm Vymazat
				switch(ID_Data)
				{
					case 'F':
						ChipErase();
						RS232_Transmit_Char(ACK);
						RS232_Transmit_Char_CR();
						break;
					case 'E':
						EepromErase();
						RS232_Transmit_Char(ACK);
						RS232_Transmit_Char_CR();
						break; 
					case 'A':
						ChipErase();
						EepromErase();
						RS232_Transmit_Char(ACK);
						RS232_Transmit_Char_CR();
						break;
					default:
						RS232_Transmit_Char(NAK);
						RS232_Transmit_Char_CR();
					break;
				}
				break;
			// Write Lock Bits
			case 'l':
				WriteLockBits();
				RS232_Transmit_Char(NAK);
				RS232_Transmit_Char_CR();
				break;
			//Read LOW FUSE
			case 'F':
				RS232_Transmit_Char(boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS));
				RS232_Transmit_Char(ACK);
				RS232_Transmit_Char_CR();
				break;
			//Read High FUSE
			case 'N':
				RS232_Transmit_Char(boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS));
				RS232_Transmit_Char(ACK);
				RS232_Transmit_Char_CR();
				break;
			//Read Extended FUSE
			case 'Q':
				RS232_Transmit_Char(boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS));
				RS232_Transmit_Char(ACK);
				RS232_Transmit_Char_CR();
				break;	
			//Read Signature
			case 's':
				RS232_Transmit_Char(boot_signature_byte_get(SIGNAT_BYTE_0));
				RS232_Transmit_Char(boot_signature_byte_get(SIGNAT_BYTE_1));				
				RS232_Transmit_Char(boot_signature_byte_get(SIGNAT_BYTE_2));				
				RS232_Transmit_Char(ACK);
				RS232_Transmit_Char_CR();
				break;
			// Return Software Identifier
			case 'S':
				RS232_Transmit_String(SOFTWARE_IDENTIFIER);
				RS232_Transmit_Char(ACK);
				RS232_Transmit_Char_CR();
				break;
			// Return Software Version
			case 'V':
				RS232_Transmit_String(BOOTLOADER_VERSION);
				RS232_Transmit_Char(ACK);
				RS232_Transmit_Char_CR();
				break;
			// Exit from Bootloader
			case 'E':
				RS232_Transmit_String("Exit from BOOTLOADER. BYE");
				RS232_Transmit_Char_CR();
				jumpaddress();
				break;
			// Check Block Support (Vytvoøení Verify pamìti)
			case 'b':
				VerifyFlash();
				break;
			// Write Data into Flash memory with specific Address
			case 'B':
				// Pøijme 1 Byte, který rozhodne jestli 
				ID_Data = RS232_Receive_Char();
				address = RS232_Receive_Char() << 8;
				address |= RS232_Receive_Char();
				// Chceme Flash nebo EEProm naplnit daty
				switch(ID_Data)
				{
					// Èást pro naprogramování FLASH
					case 'F':
						// Zaplní buffer o velikosti 1 stránky
						for (cnt = 0; cnt < SPM_PAGESIZE; cnt++)
						{
							BufferFlash[cnt] = RS232_Receive_Char();
						}
						// Pokud je zvolená adresa vìtší jak 0xDF00 což pøi zápisu 1 stránky by jsme se dostali do Bootlaoder èásti, tak buffer zahodí a odešla NAK + CR
						if (address <= (START_BOOT_ADDRESS_BYTES - PAGE_SIZE))
						{
							WriteFlashPages(address, BufferFlash);
							RS232_Transmit_Char(ACK);
							RS232_Transmit_Char_CR();
						} 
						else
						{
							RS232_Transmit_Char(NAK);
							RS232_Transmit_Char_CR();
						}
						break;
					// Èást pro naprogramování EEPROM
					case 'E':
						for (cnt = 0; cnt < PAGE_SIZE_EEPROM; cnt++)
						{
							//BufferEeprom[cnt] = cnt;
							BufferEeprom[cnt] = RS232_Receive_Char();
						}
						if (address <= (END_EEPROM_ADDRESS - PAGE_SIZE_EEPROM))
						{
							WriteEepromPages(address, BufferEeprom);
							RS232_Transmit_Char(ACK);
							RS232_Transmit_Char_CR();
						}
						else
						{
							RS232_Transmit_Char(NAK);
							RS232_Transmit_Char_CR();
						}
						break;
				}
				break;
			case 'g':
				// Pøijme 1 Byte, který rozhodne jestli 
				ID_Data = RS232_Receive_Char();
				address = RS232_Receive_Char() << 8;
				address |= RS232_Receive_Char();
				switch(ID_Data)
				{
					case 'F':
						// Pøeètení celé pamìti dle stavového automatu
						all = RS232_Receive_Char();
						if (address <= (START_BOOT_ADDRESS_BYTES - PAGE_SIZE))
						{
							ReadFlashPages(all, address);
							RS232_Transmit_Char(ACK);
							RS232_Transmit_Char_CR();
						}
						else
						{
							RS232_Transmit_Char(NAK);
							RS232_Transmit_Char_CR();	
						}
						break;
					case 'E':
						all = RS232_Receive_Char();
						if (address <= (START_EEPROM_ADDRESS - PAGE_SIZE_EEPROM))
						{
							ReadEepromPages(all, address);
							RS232_Transmit_Char(ACK);
							RS232_Transmit_Char_CR();
						}
						else
						{
							RS232_Transmit_Char(NAK);
							RS232_Transmit_Char_CR();
						}
						break;
				}
				break;
		}
    }
	
}

