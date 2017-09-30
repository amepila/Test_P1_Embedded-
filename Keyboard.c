/*
 * Keyboard.c
 *
 *  Created on: Sep 21, 2017
 *      Author: Andres Hernandez
 */

#include "MK64F12.h"
#include "Keyboard.h"
#include "GPIO.h"

void initSystemClockKeyBoard(){
	/**Set the clocks of each port to use in the KEYPAD**/
	GPIO_clockGating(GPIO_B);
	GPIO_clockGating(GPIO_C);
}

void initKeyBoard(){
	/**Set the pin such as GPIO, with PullUp enable and interruption in falling edge **/
	/***Two versions of pin control register to INPUTS and OUTPUTS ***/

	/**Pin Control Register with interruptions to INPUTS**/
	/***Set like GPIO, PULL UP Enabled, Interruption Rising Edge, Interrupt is DETECTED***/
	GPIO_pinControlRegisterType pinControlRegisterPortB_Interrupt = GPIO_MUX1|GPIO_PE|GPIO_PS|INTR_RISING_EDGE;//INT_RISING_EDGE|0x01000000;
	GPIO_pinControlRegisterType pinControlRegisterPortC_Interrupt = GPIO_MUX1|GPIO_PE|GPIO_PS|INTR_RISING_EDGE;//INT_RISING_EDGE|0x01000000;

	/**Pin Control Register without interruptions to OUTPUTS**/
	/***Only set like GPIO***/
	GPIO_pinControlRegisterType pinControlRegisterPortC = GPIO_MUX1;

	/**The custom Pin Control Register for each PIN of Column and Row**/
	/***Rows 1-4(OUTPUTS) ***/
	GPIO_pinControlRegister(GPIO_C,BIT5,&pinControlRegisterPortC);
	GPIO_pinControlRegister(GPIO_C,BIT7,&pinControlRegisterPortC);
	GPIO_pinControlRegister(GPIO_C,BIT0,&pinControlRegisterPortC);
	GPIO_pinControlRegister(GPIO_C,BIT9,&pinControlRegisterPortC);
	/***Columns 1-4(INPUTS) ***/
	GPIO_pinControlRegister(GPIO_C,BIT8,&pinControlRegisterPortC_Interrupt);
	GPIO_pinControlRegister(GPIO_C,BIT1,&pinControlRegisterPortC_Interrupt);
	GPIO_pinControlRegister(GPIO_B,BIT19,&pinControlRegisterPortB_Interrupt);
	GPIO_pinControlRegister(GPIO_B,BIT18,&pinControlRegisterPortB_Interrupt);



	/**Set like GPIO **/
	/***Rows ***/

	GPIO_writePORT(GPIO_C,BIT5);
	GPIO_writePORT(GPIO_C,BIT7);
	GPIO_writePORT(GPIO_C,BIT0);
	GPIO_writePORT(GPIO_C,BIT9);

	/**Set the pin like OUTPUT for keyboard**/
	/***Rows ***/
	GPIO_dataDirectionPIN(GPIO_C,GPIO_OUTPUT,BIT5);
	GPIO_dataDirectionPIN(GPIO_C,GPIO_OUTPUT,BIT7);
	GPIO_dataDirectionPIN(GPIO_C,GPIO_OUTPUT,BIT0);
	GPIO_dataDirectionPIN(GPIO_C,GPIO_OUTPUT,BIT9);
	/**Set the pin like INPUT for keyboard**/
	/***Columns ***/
	GPIO_dataDirectionPIN(GPIO_C,GPIO_INPUT,BIT8);
	GPIO_dataDirectionPIN(GPIO_C,GPIO_INPUT,BIT1);
	GPIO_dataDirectionPIN(GPIO_B,GPIO_INPUT,BIT19);
	GPIO_dataDirectionPIN(GPIO_B,GPIO_INPUT,BIT18);
}

valuesKey setRows(){

	valuesKey valKey = {0};

	valKey.value1 = GPIO_setPIN(GPIO_C,BIT5);
	valKey.value2 = GPIO_setPIN(GPIO_C,BIT7);
	valKey.value3 = GPIO_setPIN(GPIO_C,BIT0);
	valKey.value4 = GPIO_setPIN(GPIO_C,BIT9);

	return valKey;
}

valuesKey clearRows(){

	GPIO_clearPIN(GPIO_C,BIT5);
	GPIO_clearPIN(GPIO_C,BIT7);
	GPIO_clearPIN(GPIO_C,BIT0);
	GPIO_clearPIN(GPIO_C,BIT9);
}

valuesKey clearRows1(){
	uint32 flag1, flag2, flag3, flag4;
	valuesKey valKey;

	GPIO_clearPIN(GPIO_C,BIT5);
	GPIO_setPIN(GPIO_C,BIT7);
	GPIO_setPIN(GPIO_C,BIT0);
	GPIO_setPIN(GPIO_C,BIT9);

	flag1 = FALSE;
	flag2 = TRUE;
	flag3 = TRUE;
	flag4 = TRUE;

	valKey.value1 = flag1;
	valKey.value2 = flag2;
	valKey.value3 = flag3;
	valKey.value4 = flag4;

	return valKey;

}
valuesKey clearRows2(){
	uint32 flag1, flag2, flag3, flag4;
	valuesKey valKey;

	GPIO_setPIN(GPIO_C,BIT5);
	GPIO_clearPIN(GPIO_C,BIT7);
	GPIO_setPIN(GPIO_C,BIT0);
	GPIO_setPIN(GPIO_C,BIT9);

	flag1 = TRUE;
	flag2 = FALSE;
	flag3 = TRUE;
	flag4 = TRUE;

	valKey.value1 = flag1;
	valKey.value2 = flag2;
	valKey.value3 = flag3;
	valKey.value4 = flag4;


	return valKey;

}
valuesKey clearRows3(){
	uint32 flag1, flag2, flag3, flag4;
	valuesKey valKey;

	GPIO_setPIN(GPIO_C,BIT5);
	GPIO_setPIN(GPIO_C,BIT7);
	GPIO_clearPIN(GPIO_C,BIT0);
	GPIO_setPIN(GPIO_C,BIT9);

	flag1 = TRUE;
	flag2 = TRUE;
	flag3 = FALSE;
	flag4 = TRUE;

	valKey.value1 = flag1;
	valKey.value2 = flag2;
	valKey.value3 = flag3;
	valKey.value4 = flag4;

	return valKey;

}
valuesKey clearRows4(){
	uint32 flag1, flag2, flag3, flag4;
	valuesKey valKey;

	GPIO_setPIN(GPIO_C,BIT5);
	GPIO_setPIN(GPIO_C,BIT7);
	GPIO_setPIN(GPIO_C,BIT0);
	GPIO_clearPIN(GPIO_C,BIT9);

	flag1 = TRUE;
	flag2 = TRUE;
	flag3 = TRUE;
	flag4 = FALSE;

	valKey.value1 = flag1;
	valKey.value2 = flag2;
	valKey.value3 = flag3;
	valKey.value4 = flag4;

	return valKey;

}

valuesKey setColumns(){

	valuesKey valKey = {0};

	valKey.value1 = GPIO_readPIN(GPIO_C,BIT8);
	valKey.value2 = GPIO_readPIN(GPIO_C,BIT1);
	valKey.value3 = GPIO_readPIN(GPIO_B,BIT19);
	valKey.value4 = GPIO_readPIN(GPIO_B,BIT18);

	return (valKey);
}

valuesKey clearColumns(){

	valuesKey valKey = {0};

	valKey.value1 = GPIO_clearPIN(GPIO_C,BIT8);
	valKey.value2 = GPIO_clearPIN(GPIO_C,BIT1);
	valKey.value3 = GPIO_clearPIN(GPIO_B,BIT19);
	valKey.value4 = GPIO_clearPIN(GPIO_B,BIT18);

	return (valKey);
}

valuesKey valueKeyA(){

}

valuesKey valueKeyB(){

}

valuesKey valueKeyC(){

}

valuesKey valueKeyD(){

}

valuesKey valueKey1(){

}

valuesKey valueKey2(){

}

valuesKey valueKey3(){

}

valuesKey valueKey4(){

}

valuesKey valueKey5(){

}

valuesKey valueKey6(){

}

valuesKey valueKey7(){

}

valuesKey valueKey8(){

}

valuesKey valueKey9(){

}

valuesKey valuesKey0(){

}

valuesKey valuesKeyAst(){

}

valuesKey valuesKeySharp(){

}


