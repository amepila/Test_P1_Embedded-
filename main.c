
#include <stdio.h>
#include "MK64F12.h"
#include "NVIC.h"
#include "PIT.h"
#include "GPIO.h"
#include "DataTypeDefinitions.h"
#include "Keyboard.h"
#include "Colors.h"

#define SYSTEM_CLOCK 21000000
#define DELAY 0.1F

int main(void) {
	initSystemClockKeyBoard();
	initKeyBoard();
	setAllRGB();
	PIT_clockGating();
	NVIC_setBASEPRI_threshold(PRIORITY_10);
	NVIC_enableInterruptAndPriotity(PORTB_IRQ, PRIORITY_4);
	NVIC_enableInterruptAndPriotity(PORTC_IRQ, PRIORITY_5);
	EnableInterrupts;
	clearRows();
	while(1){
		if(TRUE == GPIO_getIRQStatus(GPIO_C)){
			/**Run the Interruptor Vector **/
		}
		clearAllRGB();
	}
    return 0 ;
}
