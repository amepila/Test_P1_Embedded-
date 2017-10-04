/*
 * DAC.c
 *
 *  Created on: 26/09/2017
 *      Author: eric_
 */
#include "MK64F12.h"
#include "DAC.h"
#define DAC0_CLOCK_GATING 0x1000;



void DACSetup(){
	DAC0->C0=0xC0;

}
void DACLoad(uint8 tableH,uint8 tableL){
	DAC0->DAT[0].DATL=tableL;
	DAC0->DAT[0].DATH=tableH;

}
void DAC_ClockGating(){
	SIM->SCGC2|=DAC0_CLOCK_GATING;
}
/*void DACTrigger(){
	DAC0->C0|=~(0x10);
	//DAC0->C0&=~(0x10);

}*/

/*void VREFSetup(){
	VREF->SC=0xE0;
	VREF->TRM=0xF0;
	SIM->SCGC4=0x100000;
}*/



