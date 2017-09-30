/*
 * Colors.c
 *
 *  Created on: Sep 23, 2017
 *      Author: Andres Hernandez
 */


#include "DataTypeDefinitions.h"
#include "MK64F12.h"
#include "GPIO.h"

#define K 1000000 //Long delay
#define W 65000		//Short delay


void setAllRGB(){
	SIM->SCGC5 = 0x2E00;

	//LEDS
	PORTB->PCR[21]= 0x00000100;
	PORTB->PCR[22]= 0x00000100;
	PORTE->PCR[26]= 0x00000100;

	//Assign the values to the GPIO

	GPIOB->PDOR = 0x00200000;
 	GPIOB->PDOR |= 0x00400000;
	GPIOE->PDOR = 0x04000000;


	//Configures the GPIO as output
	GPIOB->PDDR = 0x00200000;
	GPIOB->PDDR |= 0x00400000;
	GPIOE->PDDR = 0x04000000;

}

void clearAllRGB(){
	GPIO_setPIN(GPIO_B, BIT21);
	GPIO_setPIN(GPIO_B, BIT22);
	GPIO_setPIN(GPIO_C, BIT26);
}

void delay(uint32 delay){
	volatile uint32 counter;

	for(counter=delay;counter>0;counter--){
	}
}

void Green(void){
		//LEDS OFF
		GPIOB->PDOR |= 0x00600000;
		GPIOE->PDOR |= 0x04000000;
		delay(W);
		//LED OFF
		GPIOE->PDOR |= 0x04000000;
		delay(W);
		//LED ON
		GPIOE->PDOR &= ~(0x04000000);
		delay(K);

}
void Blue(void){
		//LEDS OFF
		GPIOB->PDOR |= 0x00600000;
		GPIOE->PDOR |= 0x04000000;
		delay(W);
		//LED OFF
		GPIOB->PDOR |= 0x00200000;
		delay(W);
		//LED ON
		GPIOB->PDOR &= ~(0x00200000);
		delay(K);


}
void Red(void){
		//LEDS OFF
		GPIOB->PDOR |= 0x00600000;
		GPIOE->PDOR |= 0x04000000;
		delay(W);
		//LED OFF
		GPIOB->PDOR |= 0x00400000;
		delay(W);
		//LED ON
		GPIOB->PDOR &= ~(0x00400000);
		delay(K);

}
void Purple(void){
		//LEDS OFF
		GPIOB->PDOR |= 0x00600000;
		GPIOE->PDOR |= 0x04000000;
		delay(W);
		//LED OFF
		GPIOB->PDOR |= 0x00400000;
		GPIOB->PDOR |= 0x00200000;
		delay(W);
		//LED ON
		GPIOB->PDOR &= ~(0x00400000);
		GPIOB->PDOR &= ~(0x00200000);
		delay(K);

}
void White(void){
		//LEDS OFF
		GPIOB->PDOR |= 0x00600000;
		GPIOE->PDOR |= 0x04000000;
		delay(W);
		//LED OFF
		GPIOB->PDOR |= 0x00400000;
		GPIOB->PDOR |= 0x00200000;
		GPIOE->PDOR |= 0x04000000;

		delay(W);
		//LED ON
		GPIOB->PDOR &= ~(0x00400000);
		GPIOB->PDOR &= ~(0x00200000);
		GPIOE->PDOR &= ~(0x04000000);
		delay(K);
}
void Yellow(void){
		//LEDS OFF
		GPIOB->PDOR |= 0x00600000;
		GPIOE->PDOR |= 0x04000000;
		delay(W);
		//LED OFF
		GPIOB->PDOR |= 0x00400000;
		GPIOE->PDOR |= 0x04000000;

		delay(W);
		//LED ON
		GPIOB->PDOR &= ~(0x00400000);
		GPIOE->PDOR &= ~(0x04000000);
		delay(K);
}
