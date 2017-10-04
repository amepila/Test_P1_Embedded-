/*
 * Keyboard.h
 *
 *  Created on: Sep 20, 2017
 *      Author: Andres Hernandez
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "DataTypeDefinitions.h"
#include "MK64F12.h"


/****************VALUE OF COLUMNS************/
#define COLUMN_1	1<<9
#define COLUMN_2 	1<<0
#define COLUMN_3 	1<<7
#define COLUMN_4	1<<5

/***************VALUE OF ROWS****************/
#define ROW_1		1<<18
#define	ROW_2		1<<19
#define ROW_3		1<<1
#define ROW_4		1<<8

/**************VALUE OF KEY*******************/
#define KEY_1		1<<0
#define KEY_2		1<<1
#define KEY_3		1<<2
#define KEY_A		1<<3
#define KEY_4		1<<4
#define KEY_5		1<<5
#define KEY_6		1<<6
#define KEY_B		1<<7
#define KEY_7		1<<8
#define KEY_8		1<<9
#define KEY_9		1<<10
#define KEY_C		1<<11
#define KEY_AST		1<<12
#define KEY_0		1<<13
#define KEY_SHARP	1<<14
#define KEY_D		1<<15


/*****Data Type of pointer to function to save the functions of ROW*****/
typedef void(*const ptrToRows)(void);
/*****Data Type of pointer to function to save the functions of KEY*****/
typedef uint32(*fptrKey)(void);


/**Tags of Main Process**/
typedef enum{
	MASTER,
	PROCESS,
	MOTOR,
	SIGNAL
}StateGeneral;

/**Start the clock of the system**/
void initSystemClockKeyBoard();
/**Initialize the configurations of KeyPad**/
void initKeyBoard();
/**Turn off the energy in all Rows**/
void clearRows();
/**Turn on the energy in all Rows **/
void setRows();
/**Turn on the energy in the Row 1**/
void setRow1();
/**Turn on the energy in the Row 2**/
void setRow2();
/**Turn on the energy in the Row 3**/
void setRow3();
/**Turn on the energy in the Row 4**/
void setRow4();
/**Scanner to find the correct pressed Key**/
void scanKeyPad(uint32 intrKey);
/**Enable the energy in the row, rotates the energy by column**/
void energyDetect();

/**Function of each Key**/
/**Return the Key **/
uint32 valueKeyA();
uint32 valueKeyB();
uint32 valueKeyC();
uint32 valueKeyD();
uint32 valueKey1();
uint32 valueKey2();
uint32 valueKey3();
uint32 valueKey4();
uint32 valueKey5();
uint32 valueKey6();
uint32 valueKey7();
uint32 valueKey8();
uint32 valueKey9();
uint32 valuesKey0();
uint32 valuesKeyAst();
uint32 valuesKeySharp();

#endif /* KEYBOARD_H_ */
