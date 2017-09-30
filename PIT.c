/*
 * PIT.c
 *
 *  Created on: Sep 17, 2017
 *      Author: Andres Hernandez
 */

#include "MK64F12.h"
#include "PIT.h"

/**Flag of the Interrupt Status **/
uint8 intrFlag = 0;

uint8 PIT_interruptFlagStatus(void);

uint8 PIT_getIntrStutus(void){
	/**This function returns the value of the flag calculated in previus functions**/
	return intrFlag;
}

void PIT0_IRQHandler(void){
	/**Dummy variable to read the control register**/
	uint32 dummy;

	/**Clean the flag **/
	PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;
	/**Read the timer **/
	dummy = PIT->CHANNEL[0].TCTRL;
	PIT->CHANNEL[0].TCTRL;
	/**Enables Pit Timer Interrupt **/
	PIT->CHANNEL[0].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
	/**Enable timer 0 **/
	PIT->CHANNEL[0].TCTRL &= ~(PIT_TCTRL_TIE_MASK);
	/**Changes flag to continue with the process**/
	intrFlag = TRUE;

}

void PIT1_IRQHandler(void){
	/**Dummy variable to read the control register**/
	uint32 dummy;

	/**Clean the flag **/
	PIT->CHANNEL[1].TFLG |= PIT_TFLG_TIF_MASK;
	/**Read the timer **/
	dummy = PIT->CHANNEL[1].TCTRL;
	PIT->CHANNEL[1].TCTRL;
	/**Enables Pit Timer Interrupt **/
	PIT->CHANNEL[1].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
	/**Enable timer 0 **/
	PIT->CHANNEL[1].TCTRL &= ~(PIT_TCTRL_TIE_MASK);
	/**Changes flag to continue with the process**/
	intrFlag = TRUE;

}

void PIT2_IRQHandler(void){
	/**Dummy variable to read the control register**/
	uint32 dummy;

	/**Clean the flag **/
	PIT->CHANNEL[2].TFLG |= PIT_TFLG_TIF_MASK;
	/**Read the timer **/
	dummy = PIT->CHANNEL[2].TCTRL;
	PIT->CHANNEL[2].TCTRL;
	/**Enables Pit Timer Interrupt **/
	PIT->CHANNEL[2].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
	/**Enable timer 0 **/
	PIT->CHANNEL[2].TCTRL &= ~(PIT_TCTRL_TIE_MASK);
	/**Changes flag to continue with the process**/
	intrFlag = TRUE;

}

void PIT3_IRQHandler(void){
	/**Dummy variable to read the control register**/
	uint32 dummy;

	/**Clean the flag **/
	PIT->CHANNEL[3].TFLG |= PIT_TFLG_TIF_MASK;
	/**Read the timer **/
	dummy = PIT->CHANNEL[3].TCTRL;
	PIT->CHANNEL[3].TCTRL;
	/**Enables Pit Timer Interrupt **/
	PIT->CHANNEL[3].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
	/**Enable timer 0 **/
	PIT->CHANNEL[3].TCTRL &= ~(PIT_TCTRL_TIE_MASK);
	/**Changes flag to continue with the process**/
	intrFlag = TRUE;

}

void PIT_delay(PIT_Timer_t pitTimer,float systemClock ,float perior){
	/**Float variables to calculate the cycles **/
	float clockPeriod;
	float cycles;
	float cyclesLDVAL;

	/**Value of period of systemClock **/
	clockPeriod = (1/(systemClock/2));
	/**Value of cycles necessary to assign to the PIT **/
	cycles = perior/clockPeriod;
	/**Rests 1 because starts at 0**/
	cyclesLDVAL = cycles - 1;
	/**Enable the timers**/
	PIT->MCR = 0x0000;
	/**Setup the timer for value of cycleS **/
	PIT->CHANNEL[pitTimer].LDVAL = (uint32)cyclesLDVAL;
	/**Start the timer**/
	PIT->CHANNEL[pitTimer].TCTRL |= PIT_TCTRL_TIE_MASK;
	PIT->CHANNEL[pitTimer].TCTRL |= PIT_TCTRL_TEN_MASK;

	/**Changes the flag to continue with loop **/
	intrFlag = FALSE;

}

void PIT_clockGating(void){
	/**Value the System Clock Gating to activate the PIT**/
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
}

void PIT_clear(void){
	/**Changes the flag to continue with the loop in main**/
	intrFlag = TRUE;
}



