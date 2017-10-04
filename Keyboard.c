/*
 * Keyboard.c
 *
 *  Created on: Sep 21, 2017
 *      Author: Andres Hernandez
 */

#include "MK64F12.h"
#include "Keyboard.h"
#include "GPIO.h"
#include "PIT.h"

#define SYSTEM_CLOCK 21000000
#define DELAY 0.1F

/**Value that saves the pressed Key **/
uint32 valueKeyPressed;
/**Value that saves the number of interrupter row **/
extern uint32 numberRow;

/************************************************************************/
/*********Micro functions that return the value of pressed Key***********/
/************************************************************************/

uint32 valueKeyA(){
	uint32 value = KEY_A;
	return value;
}

uint32 valueKeyB(){
	uint32 value = KEY_B;
	return value;
}

uint32 valueKeyC(){
	uint32 value = KEY_C;
	return value;
}

uint32 valueKeyD(){
	uint32 value = KEY_D;
	return value;
}

uint32 valueKey1(){
	uint32 value = KEY_1;
	return value;
}

uint32 valueKey2(){
	uint32 value = KEY_2;
	return value;
}

uint32 valueKey3(){
	uint32 value = KEY_3;
	return value;
}

uint32 valueKey4(){
	uint32 value = KEY_4;
	return value;
}

uint32 valueKey5(){
	uint32 value = KEY_5;
	return value;
}

uint32 valueKey6(){
	uint32 value = KEY_6;
	return value;
}

uint32 valueKey7(){
	uint32 value = KEY_7;
	return value;

}

uint32 valueKey8(){
	uint32 value = KEY_8;
	return value;
}

uint32 valueKey9(){
	uint32 value = KEY_9;
	return value;
}

uint32 valueKey0(){
	uint32 value = KEY_0;
	return value;
}

uint32 valueKeyAst(){
	uint32 value = KEY_AST;
	return value;
}

uint32 valueKeySharp(){
	uint8 value = 15;
	return value;
}

/**Array of functions of Rows**/

ptrToRows initRow[4]= {
		setRow1,
		setRow2,
		setRow3,
		setRow4
};
/**Array of value of columns**/
uint32 valueColumns[4] = {
		COLUMN_1,
		COLUMN_2,
		COLUMN_3,
		COLUMN_4
};
/**Matrix of the map of the Keypad **/
fptrKey matrixKeypad[4][4] = {
	{valueKey1,		valueKey2,		valueKey3,		valueKeyA},
	{valueKey4,		valueKey5,		valueKey6,		valueKeyB},
	{valueKey7,		valueKey8,		valueKey9,		valueKeyC},
	{valueKeyAst,	valueKey0,		valueKeySharp,	valueKeyD}
};

/**Scanner of the keypad that localize the pressed value**/
void scanKeyPad(uint32 intrKey){
/*******************************************************************/
	/**Counter of columns**/
	uint8 counter;
	/**This variable saves the correct Key pressed**/
	uint32 keyPressed;

	/**Loop that localizes the correct value**/
	for(counter = 0; counter < 4; counter++){
		/**Conditional that compare the pressed with the columns**/
		if((intrKey == valueColumns[counter])){
			/**If the value is found then its saves in the KeyPressed**/
			keyPressed = matrixKeypad[numberRow-1][counter]();
			/**The value is saved in the global variable**/
			valueKeyPressed = keyPressed;
			/**End the scanner**/
			break;
		}
	}

/********************************************************************/
/********************************************************************
	if(numberRow == 1){
		if(intrKey == COLUMN_1){valueKeyPressed = KEY_1;}
		if(intrKey == COLUMN_2){valueKeyPressed = KEY_2;}
		if(intrKey == COLUMN_3){valueKeyPressed = KEY_3;}
		if(intrKey == COLUMN_4){valueKeyPressed = KEY_A;}
		//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY);
	}


	if(numberRow == 2){
		if(intrKey == COLUMN_1){valueKeyPressed = KEY_4;}
		if(intrKey == COLUMN_2){valueKeyPressed = KEY_5;}
		if(intrKey == COLUMN_3){valueKeyPressed = KEY_6;}
		if(intrKey == COLUMN_4){valueKeyPressed = KEY_B;}
		//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY);
	}

	if(numberRow == 3){
		if(intrKey == COLUMN_1){valueKeyPressed = KEY_7;}
		if(intrKey == COLUMN_2){valueKeyPressed = KEY_8;}
		if(intrKey == COLUMN_3){valueKeyPressed = KEY_9;}
		if(intrKey == COLUMN_4){valueKeyPressed = KEY_C;}
		//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY);
	}

	if(numberRow == 4){
		if(intrKey == COLUMN_1){valueKeyPressed = KEY_AST;}
		if(intrKey == COLUMN_2){valueKeyPressed = KEY_0;}
		if(intrKey == COLUMN_3){valueKeyPressed = KEY_SHARP;}
		if(intrKey == COLUMN_4){valueKeyPressed = KEY_D;}
		//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY);
	}
*******************************************************************/
}
/**This function launches energy in the rows of keypad**/
void energyDetect(){

	/**Set the row 1 and saves the value in numerRow**/
	setRow1();
	numberRow = 1;
	//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY);

	/**Set the row 2 and saves the value in numerRow**/
	setRow2();
	numberRow = 2;
	//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY);

	/**Set the row 3 and saves the value in numerRow**/

	setRow3();
	numberRow = 3;
	//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY);

	/**Set the row 4 and saves the value in numerRow**/
	setRow4();
	numberRow = 4;
	//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY);
}

void initSystemClockKeyBoard(){
	/**Set the clocks of each port to use in the KEYPAD**/
	GPIO_clockGating(GPIO_C);
}

void initKeyBoard(){
	/**Set the pin such as GPIO, with PullUp enable and interruption in RISING edge **/
	/***Two versions of pin control register to INPUTS and OUTPUTS ***/

	/**Pin Control Register with interruptions to INPUTS**/
	/***Set like GPIO, PULL UP Enabled, Interruption Rising Edge***/
	GPIO_pinControlRegisterType pinControlRegisterPortC_Interrupt = GPIO_MUX1|GPIO_PE|GPIO_PS|INTR_RISING_EDGE;

	/**Pin Control Register without interruptions to OUTPUTS**/
	/***Only set like GPIO***/
	GPIO_pinControlRegisterType pinControlRegisterPortC = GPIO_MUX1;

	/**The custom Pin Control Register for each PIN of Column and Row**/
	/***Rows 1-4(OUTPUTS) ***/
	GPIO_pinControlRegister(GPIO_C,BIT16,&pinControlRegisterPortC);
	GPIO_pinControlRegister(GPIO_C,BIT17,&pinControlRegisterPortC);
	GPIO_pinControlRegister(GPIO_C,BIT1,&pinControlRegisterPortC);
	GPIO_pinControlRegister(GPIO_C,BIT8,&pinControlRegisterPortC);

	/***Columns 1-4(INPUTS) ***/
	GPIO_pinControlRegister(GPIO_C,BIT9,&pinControlRegisterPortC_Interrupt);
	GPIO_pinControlRegister(GPIO_C,BIT0,&pinControlRegisterPortC_Interrupt);
	GPIO_pinControlRegister(GPIO_C,BIT7,&pinControlRegisterPortC_Interrupt);
	GPIO_pinControlRegister(GPIO_C,BIT5,&pinControlRegisterPortC_Interrupt);

	/**Set like GPIO **/
	/***Rows ***/
	GPIO_writePORT(GPIO_C,BIT16);
	GPIO_writePORT(GPIO_C,BIT17);
	GPIO_writePORT(GPIO_C,BIT1);
	GPIO_writePORT(GPIO_C,BIT8);

	/**Set the pin like OUTPUT for keyboard**/
	/***Rows ***/
	GPIO_dataDirectionPIN(GPIO_C,GPIO_OUTPUT,BIT16);
	GPIO_dataDirectionPIN(GPIO_C,GPIO_OUTPUT,BIT17);
	GPIO_dataDirectionPIN(GPIO_C,GPIO_OUTPUT,BIT1);
	GPIO_dataDirectionPIN(GPIO_C,GPIO_OUTPUT,BIT8);

	/**Set the pin like INPUT for keyboard**/
	/***Columns ***/
	GPIO_dataDirectionPIN(GPIO_C,GPIO_INPUT,BIT9);
	GPIO_dataDirectionPIN(GPIO_C,GPIO_INPUT,BIT0);
	GPIO_dataDirectionPIN(GPIO_C,GPIO_INPUT,BIT7);
	GPIO_dataDirectionPIN(GPIO_C,GPIO_INPUT,BIT5);
}

void clearRows(){
	/**Disable the energy to Rows**/
	/***The logic is active in low***/
	GPIO_setPIN(GPIO_C,BIT16);
	GPIO_setPIN(GPIO_C,BIT17);
	GPIO_setPIN(GPIO_C,BIT1);
	GPIO_setPIN(GPIO_C,BIT8);
}

void setRows(){
	/**Enable all the rows **/
	/***The logic is active in low***/
	GPIO_clearPIN(GPIO_C,BIT16);
	GPIO_clearPIN(GPIO_C,BIT17);
	GPIO_clearPIN(GPIO_C,BIT1);
	GPIO_clearPIN(GPIO_C,BIT8);
}

void setRow1(){
	/**Activate only the ROW 1**/

	GPIO_clearPIN(GPIO_C,BIT16);
	GPIO_setPIN(GPIO_C,BIT17);
	GPIO_setPIN(GPIO_C,BIT1);
	GPIO_setPIN(GPIO_C,BIT8);
}
void setRow2(){
	/**Activate only the ROW 2**/

	GPIO_setPIN(GPIO_C,BIT16);
	GPIO_clearPIN(GPIO_C,BIT17);
	GPIO_setPIN(GPIO_C,BIT1);
	GPIO_setPIN(GPIO_C,BIT8);
}
void setRow3(){
	/**Activate only the ROW 3**/

	GPIO_setPIN(GPIO_C,BIT16);
	GPIO_setPIN(GPIO_C,BIT17);
	GPIO_clearPIN(GPIO_C,BIT1);
	GPIO_setPIN(GPIO_C,BIT8);
}
void setRow4(){
	/**Activate only the ROW 4**/

	GPIO_setPIN(GPIO_C,BIT16);
	GPIO_setPIN(GPIO_C,BIT17);
	GPIO_setPIN(GPIO_C,BIT1);
	GPIO_clearPIN(GPIO_C,BIT8);
}

