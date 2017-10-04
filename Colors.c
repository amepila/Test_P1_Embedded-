/*
 * Colors.c
 *
 *  Created on: Sep 23, 2017
 *      Author: Andres Hernandez
 */


#include "DataTypeDefinitions.h"
#include "MK64F12.h"
#include "GPIO.h"
#include "PIT.h"

#define SYSTEM_CLOCK 21000000
#define DELAY_LONG 0.05F
#define DELAY_SHORT 0.02F

#define L 1000000
#define S 65000

void setAllRGB(){
	//GPIO_clockGating(GPIO_B);
	//GPIO_clockGating(GPIO_E);

	GPIO_pinControlRegisterType pinControlRegisterPortB = GPIO_MUX1;
	GPIO_pinControlRegisterType pinControlRegisterPortE = GPIO_MUX1;

	GPIO_pinControlRegister(GPIO_B,BIT21,&pinControlRegisterPortB);
	GPIO_pinControlRegister(GPIO_B,BIT22,&pinControlRegisterPortB);
	GPIO_pinControlRegister(GPIO_E,BIT26,&pinControlRegisterPortE);

	/**Led Blue**/
	GPIO_writePORT(GPIO_B,BIT21);
	/**Led Red**/
	GPIO_writePORT(GPIO_B,BIT22);
	/**Led Green**/
	GPIO_writePORT(GPIO_E,BIT26);

	GPIO_dataDirectionPIN(GPIO_B,GPIO_OUTPUT,BIT21);
	GPIO_dataDirectionPIN(GPIO_B,GPIO_OUTPUT,BIT22);
	GPIO_dataDirectionPIN(GPIO_E,GPIO_OUTPUT,BIT26);

}

void clearAllRGB(){
	GPIO_setPIN(GPIO_B, BIT21);
	GPIO_setPIN(GPIO_B, BIT22);
	GPIO_setPIN(GPIO_E, BIT26);
}



void Green(void){
	/**Leds Off**/
	GPIO_setPIN(GPIO_B, BIT21);
	GPIO_setPIN(GPIO_B, BIT22);
	GPIO_setPIN(GPIO_E, BIT26);
	//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY_SHORT);

	/**Led On **/
	GPIO_clearPIN(GPIO_E, BIT26);
	//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY_LONG);

}
void Blue(void){
	/**Leds Off**/
	GPIO_setPIN(GPIO_B, BIT21);
	GPIO_setPIN(GPIO_B, BIT22);
	GPIO_setPIN(GPIO_E, BIT26);
	/**Led On **/
	GPIO_clearPIN(GPIO_B, BIT21);


}
void Red(void){
	/**Leds Off**/
	GPIO_setPIN(GPIO_B, BIT21);
	GPIO_setPIN(GPIO_B, BIT22);
	GPIO_setPIN(GPIO_E, BIT26);
	//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY_SHORT);

	/**Led On **/
	GPIO_clearPIN(GPIO_B, BIT22);
	//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY_LONG);
}
void Purple(void){
	/**Leds Off**/
	GPIO_setPIN(GPIO_B, BIT21);
	GPIO_setPIN(GPIO_B, BIT22);
	GPIO_setPIN(GPIO_E, BIT26);

	/**Led On **/
	GPIO_clearPIN(GPIO_B, BIT21);
	GPIO_clearPIN(GPIO_B, BIT22);
	//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY_LONG);
}
void White(void){
	/**Leds Off**/
	GPIO_setPIN(GPIO_B, BIT21);
	GPIO_setPIN(GPIO_B, BIT22);
	GPIO_setPIN(GPIO_E, BIT26);
	//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY_SHORT);

	/**Led On **/
	GPIO_clearPIN(GPIO_B, BIT21);
	GPIO_clearPIN(GPIO_B, BIT22);
	GPIO_clearPIN(GPIO_E, BIT26);
	//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY_LONG);

}
void Yellow(void){
	/**Leds Off**/
	GPIO_setPIN(GPIO_B, BIT21);
	GPIO_setPIN(GPIO_B, BIT22);
	GPIO_setPIN(GPIO_E, BIT26);
	//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY_SHORT);

	/**Led On **/
	GPIO_clearPIN(GPIO_B, BIT22);
	GPIO_clearPIN(GPIO_E, BIT26);
	//PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY_LONG);
}
