/*
 * trinamic.h
 *
 * Created: 17.1.2017 13:27:46
 *  Author: atom2
 */ 


#ifndef TRINAMIC_H_
#define TRINAMIC_H_

//#define RS485_EN_INT_receive		cbi(PORTD, PORTD4);
//#define RS485_EN_INT_transmite		sbi(PORTD, PORTD4);

#define RS485_EN_EXT_receive		cbi(PORTD, PORTD4);
#define RS485_EN_EXT_transmite		sbi(PORTD, PORTD4);

#define EEPROM_START_ADDRESS_0 16
#define EEPROM_START_ADDRESS_1 32


// data format
#define TB_BUF_ADDRESS  0
#define TB_BUF_COMMAND  1
#define TB_BUF_TYPE     2
#define TB_BUF_MOTOR    3
#define TB_BUF_SUM      8


// instructions
#define TB_CMD_DEBUG    254
#define TB_CMD_ROR      1
#define TB_CMD_ROL      2
#define TB_CMD_MST      3
#define TB_CMD_MVP      4
#define TB_CMD_SAP      5
#define TB_CMD_GAP      6
#define TB_CMD_STAP     7
#define TB_CMD_RSAP     8
#define TB_CMD_SGP      9
#define TB_CMD_GGP     10
#define TB_CMD_STGP    11
#define TB_CMD_RSGP    12
#define TB_CMD_RFS     13
#define TB_CMD_SIO     14
#define TB_CMD_GIO	   15
/* Vlastnì definované */
#define TB_CMD_VENTIL	16
#define TB_CMD_VZOREK	17
#define TB_CMD_BLACKBOX 18

#define TB_CMD_CALC		19	//Calc
#define TB_CMD_COMP		20	//COMP
#define TB_CMD_JC		21	//JC
#define TB_CMD_JA		22	//JA
#define TB_CMD_CSUB		23	//CSUB
#define TB_CMD_RSUB		24	//RSUB
#define TB_CMD_EI		25
#define TB_CMD_DI		26
#define TB_CMD_WAIT		27
#define TB_CMD_INTERLOCK	28
#define TB_CMD_OFUK		29
//Volné 28 29
#define TB_CMD_SCO     30
#define TB_CMD_GCO     31
#define TB_CMD_CCO     32
#define TB_CMD_CALCX	33
#define TB_CMD_AAP		34
#define TB_CMD_AGP		35
#define TB_CMD_CLE		36
#define TB_CMD_VECT		37
#define TB_CMD_RETI		38
#define TB_CMD_ACO		39
// Pøíkazy pro jednotlivé desky (Upravovatelne)
#define TB_CMD_SETUP	40



/* Lze si je upravit tyto */
#define TB_CMD_USER0	64
#define TB_CMD_USER1	65
#define TB_CMD_USER2	66
#define TB_CMD_USER3	67
#define TB_CMD_USER4	68
#define TB_CMD_USER5	69
#define TB_CMD_USER6	70
#define TB_CMD_USER7	71

#define TB_CMD_CONTROLFUNCTIONS0		136
#define TB_CMD_CONTROLFUNCTIONS1		137
#define TB_CMD_REQUEST_TARGET_POSITION	138




//Prikazy pouze pro BlackBox
#define BB_INFO			0
#define BB_ACTIVE		1
#define BB_SETOUTPUT	2
#define BB_SETTIME		3
#define BB_SETMASK		4

//Prikazy pouze pro interlock
#define IL_INFO			0
#define IL_SHIELD		1
#define IL_SERVIS		2
#define IL_CHAMBER		3
#define IL_LASER		4

//Prikazy pouze pro interlock
#define OF_INFO			0
#define OF_SETOUTPUT	1
#define OF_SETTINGS		2



// error codes
#define TB_ERR_OK           100
#define TB_ERR_EEPROM_OK    101
#define TB_ERR_SUM            1
#define TB_ERR_COMMAND        2
#define TB_ERR_TYPE           3
#define TB_ERR_VALUE          4
#define TB_ERR_EEPROM_LOCK    5
#define TB_ERR_COMMAND_NA     6
#define TB_ERR_MOTOR		  7

// axis parameters
#define TB_PARAM_TARGET_POSITION 0
#define TB_PARAM_ACTUAL_POSITION 1
#define TB_PARAM_SPEED           4
#define TB_PARAM_ACCELERATION    5
#define TB_PARAM_CURRENT_RUN     6
#define TB_PARAM_CURRENT_HOLD    7
#define TB_PARAM_RESOLUTION    140
#define TB_PARAM_RFS_DISTANCE  196


#define TB_GBPARAM_EEMAGIC      64
#define TB_GBPARAM_BAUD         65
#define TB_GBPARAM_ADDRESS      66
#define TB_GBPARAM_TEL_PAU_TIME 75
#define TB_GBPARAM_HOST_ADDR    76

struct TB_GBPARAM {
	uint8_t eemagic;
	uint8_t baud;
	uint8_t address;
	uint8_t telegram_pause_time;
	uint8_t host_address;

};

struct TB_GBPARAM TB_gbparam0;
struct TB_GBPARAM TB_gbparam1;



typedef union {
	struct {
		uint8_t addr;
		uint8_t cmd;
		uint8_t type;
		uint8_t motor;
		union {
			uint32_t dw;
			struct {
				uint8_t b0;
				uint8_t b1;
				uint8_t b2;
				uint8_t b3;
			} b;
		} val;
		uint8_t uart0_sum;
	} n;
	uint8_t b[9];
} Trinamicpac;

/* deklarování funkcí */
uint8_t TB_Process0( void );
uint8_t TB_buf_fill_In( void );
uint8_t TB_buf_fill_Out( void );
void TB_Send_Com(void);
void TB_calcSum(void);
void TB_Send(uint8_t AddrReply, uint8_t AddrModule, uint8_t Status, uint8_t Command, uint32_t value);
void TB_SendAck(uint8_t status, uint32_t value);
void TB_Init0( void );
void TB_Init1( void );


#endif /* TRINAMIC_H_ */