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

typedef struct{
	uint32 value1;
	uint32 value2;
	uint32 value3;
	uint32 value4;
}valuesKey;

void initSystemClockKeyBoard();
void initKeyBoard();
void setInputKeyboard();
valuesKey setColumns();
valuesKey clearColumns();
valuesKey setRows();
valuesKey clearRows();
valuesKey clearRows1();
valuesKey clearRows2();
valuesKey clearRows3();
valuesKey clearRows4();


/**Function of each Key**/
/**Return the Key **/
valuesKey valueKeyA();
valuesKey valueKeyB();
valuesKey valueKeyC();
valuesKey valueKeyD();
valuesKey valueKey1();
valuesKey valueKey2();
valuesKey valueKey3();
valuesKey valueKey4();
valuesKey valueKey5();
valuesKey valueKey6();
valuesKey valueKey7();
valuesKey valueKey8();
valuesKey valueKey9();
valuesKey valuesKey0();
valuesKey valuesKeyAst();
valuesKey valuesKeySharp();

#endif /* KEYBOARD_H_ */
