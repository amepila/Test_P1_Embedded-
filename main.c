
#include <stdio.h>
#include "MK64F12.h"
#include "NVIC.h"
#include "PIT.h"
#include "GPIO.h"
#include "DataTypeDefinitions.h"
#include "Keyboard.h"
#include "DAC.h"
#include "Colors.h"
#include "MCG.h"
#define SYSTEM_CLOCK 60000000
//Elementos para aumentar el clock
#define CLK_FREQ_HZ 50000000  /* CLKIN0 frequency */
#define SLOW_IRC_FREQ 32768	/*This is the approximate value for the slow irc*/
#define FAST_IRC_FREQ 4000000 /*This is the approximate value for the fast irc*/
#define EXTERNAL_CLOCK 0 /*It defines an external clock*/
#define PLL_ENABLE 1 /**PLL is enabled*/
#define PLL_DISABLE 0 /**PLL is disabled*/
#define CRYSTAL_OSC 1  /*It defines an crystal oscillator*/
#define LOW_POWER 0     /* Set the oscillator for low power mode */
#define SLOW_IRC 0 		/* Set the slow IRC */
#define CLK0_TYPE 0     /* Crystal or canned oscillator clock input */
#define PLL0_PRDIV 25    /* PLL predivider value */
#define PLL0_VDIV 30    /* PLL multiplier value*/
//Estados para el funcionamiento del generador de ondas
#define NO_WAVE 0
#define SQUARE 1
#define SINE 2
#define TRIANGLE 3
//Look up tables del generador de Ondas.
#define SINEH {0x7,0xa,0xb,0xd,0xe,0xf,0xf,0xf,0xe,0xc,0xb,0x8,0x6,0x4,0x2,0x1,0x0,0x0,0x0,0x0,0x2,0x3,0x5} //Parte alta de la lookup table de seno Bit[11:8]
#define SINEL {0xe2,0x02,0xfa,0xa4,0xe1,0x99,0xbe,0x4f,0x52,0xdb,0x05,0xf4,0xcf,0xbe,0xe8,0x71,0x74,0x5,0x2a,0xe2,0x1f,0xc9,0xc1}//Parte baja de la lookup table de seno Bit[7:0]
#define TRIANGLEH {0x1,0x2,0x4,0x5,0x6,0x8,0x9,0xa,0xc,0xd,0xf,0xf,0xf,0xd,0xc,0xa,0x9,0x8,0x6,0x5,0x4,0x2,0x1} //Parte alta de la lookup table de una onda triangular Bit[11:8]
#define TRIANGLEL {0x5f,0xbe,0x1d,0x7b,0xda,0x39,0x98,0xf7,0x56,0xb5,0x14,0xc3,0x14,0xb5,0x56,0xf7,0x98,0x39,0xda,0x7b,0x1d,0xbe,0x5f}//Parte baja de la lookup table de una onda triangular Bit[7:0]
#define SQUAREH {0x0F,0x00,0x0F,0x00,0x0F,0x00} //Parte alta de una onda cuadrada Bit[11:8]
#define SQUAREL {0xFF,0x00,0xFF,0x00,0xFF,0x00} //Parte baja de una onda cuadrada Bit[7:0]
#define emptyT {0} //Array de cero para cuando no se requiera una onda.
//Estados del funcionamiento del motor.
#define OFF 0
#define SECUENCIAUNOSTART 1
#define SECUENCIAUNODOS 2
#define SECUENCIAUNOTRES 3
#define SECUENCIAUNOCUATRO 4
#define SECUENCIADOSSTART 5
#define SECUENCIADOSDOS 6
void globalSetup();
void globalClockGating();
typedef struct{
	float delay;
	uint8 tlength;
	void(*wave)(uint8,uint8);
	uint8 tableH[23];
	uint8 tableL[23];
	uint8 next[3];
	uint8 ledWaveBIT1;
	uint8 ledWaveBIT2;
	uint8 ledWaveBITOff;
	void (*RGB)();
	void (*ledON)(uint8,uint8);
	void (*ledOFF)(uint8,uint8);
} Generator;
typedef struct{
	float time;
	void (*output)(GPIO_portNameType,uint8);
	uint8 mNext[3];
	uint8 ledMotorBIT1;
	uint8 ledMotorBIT2;
	uint8 ledMotorBITOff;
	void (*ledON)(uint8,uint8);
	void (*ledOFF)(uint8,uint8);
}Motor;


const Generator waveStateMachine[4]={
		{0.000000,0,DACLoad,{},emptyT,{SQUARE,NO_WAVE,NO_WAVE},0,0,0,clearAllRGB,GPIO_clearPIN,GPIO_clearPIN},
		{0.1,4,DACLoad,SQUAREH,SQUAREL,{SINE,SQUARE,NO_WAVE},BIT10,BIT11,BIT11,Blue,GPIO_setPIN,GPIO_clearPIN},
		{0.00869565,23,DACLoad,SINEH,SINEL,{TRIANGLE,SINE,NO_WAVE},BIT10,BIT11,BIT10,Red,GPIO_setPIN,GPIO_clearPIN},
		{0.00869565,23,DACLoad,TRIANGLEH,TRIANGLEL,{SQUARE,TRIANGLE,NO_WAVE},BIT10,BIT11,BIT11,Green,GPIO_setPIN,GPIO_setPIN}
};

const Motor motorStateMachine[7]={
		{0,GPIO_clearPIN,{OFF,OFF,OFF},BIT2,BIT3,BIT3,GPIO_clearPIN,GPIO_clearPIN},
		{1,GPIO_setPIN,{SECUENCIAUNOSTART,SECUENCIAUNODOS,OFF},BIT2,BIT3,BIT2,GPIO_setPIN,GPIO_clearPIN},
		{1,GPIO_clearPIN,{SECUENCIAUNOSTART,SECUENCIAUNOTRES,OFF},BIT2,BIT3,BIT2,GPIO_setPIN,GPIO_clearPIN},
		{3,GPIO_setPIN,{SECUENCIAUNOSTART,SECUENCIAUNOCUATRO,OFF},BIT2,BIT3,BIT2,GPIO_setPIN,GPIO_clearPIN},
		{1,GPIO_clearPIN,{SECUENCIAUNOSTART,SECUENCIAUNOSTART,OFF},BIT2,BIT3,BIT2,GPIO_setPIN,GPIO_clearPIN},
		{4,GPIO_setPIN,{SECUENCIADOSSTART,SECUENCIADOSDOS,OFF},BIT3,BIT2,BIT3,GPIO_setPIN,GPIO_clearPIN},
		{4,GPIO_clearPIN,{SECUENCIADOSSTART,SECUENCIADOSSTART,OFF},BIT3,BIT2,BIT3,GPIO_setPIN,GPIO_clearPIN,
		}
};
/**Number of row in detection of Keypad interrupter**/
uint32 numberRow;
/**Value of pressed key**/
extern uint32 valueKeyPressed;
/**Counter of pressed Keys**/
extern uint8 counter_PassW;

/**Start with the MASTER Mode**/
uint8 flag_Process = MASTER;

int main(void) {
	/**Start the clock that uses the Keypad**/
	initSystemClockKeyBoard();
	/**Initialize the parameters and functions of the Keypad**/

	initKeyBoard();
	/**Enable the clock of PIT**/
	PIT_clockGating();
	//Enables all required components clock gating
	globalClockGating();
	/**Enable the configuration of RGB**/
	setAllRGB();
	/**Set the reference priority **/
	NVIC_setBASEPRI_threshold(PRIORITY_10);
	/**Set the priority of PORT C **/
	NVIC_enableInterruptAndPriotity(PORTC_IRQ, PRIORITY_4);
	/**Enable all the interrupts **/
	EnableInterrupts;
	/**Passwords pattern**/
	/***Master Password***/
	const uint32 password_Master[4] = {KEY_1,KEY_2,KEY_3,KEY_4};
	/***Motor Password***/
	const uint32 password_Motor[4] = {KEY_4,KEY_5,KEY_6,KEY_7};
	/***Signal Password***/
	const uint32 password_Signal[4] = {KEY_7,KEY_8,KEY_9,KEY_0};

	/**Array that contents the pressed password**/
	uint32 input_Password[4];
	/**Counter to comparison each element between passwords**/
	uint8 counter_Comp;
	/**This flag indicates the correct keys**/
	uint8 flag_Success = 0;


	/**The counter of password is cleaned to start at 0**/
	counter_PassW = 0;
	//Variables para la deteccion de switches
	uint8 Switch_3=0;
	uint8 Switch_2=0;
	//Se inicializan los estados de las maquinas en 0;
	uint8 waveCurrentState=NO_WAVE;


	uint8 motorCurrentState=OFF;
	//Set the multiple configurations for the GPIO ports
	globalSetup();
	//Enable interrupt and set priority for the PIT channels
	NVIC_enableInterruptAndPriotity(PIT_CH0_IRQ, PRIORITY_9);
	NVIC_enableInterruptAndPriotity(PIT_CH1_IRQ, PRIORITY_9);
	NVIC_enableInterruptAndPriotity(PIT_CH2_IRQ, PRIORITY_9);
	//Initialize the control variables for the DAC and Motor control
	int waveInit=0;
	int motorInit=0;
	int tablePosition=0;
	int waveControl=0;
	int motorControl=0;
	PIT_clearSelect(PIT_0);
	while(1){
		/*Launch the energy in the rows to detect a key of Keypad*/
		energyDetect();

		/**Assign the value in an array**/
		input_Password[counter_PassW-1] = valueKeyPressed;
		/**Reboot the counter of digits of password**/
		if(counter_PassW == 4){counter_PassW = 0;}

		/******************Comparison the keypad input with MASTER PASSWORD*****************************************************************/
		/***********************************************************************************************************************************/
		/**This condition starts when a password is pressed**/
		/**The tag Master is to recognize the state**/
		if((counter_PassW == 0)&&(valueKeyPressed != 0)&&(flag_Process == MASTER)){
			/**The condition initiates the comparison between the pressed password and the correct password**/
			for(counter_Comp = 0; counter_Comp < 4; counter_Comp++){
				/**Comparison between the elements of passwords**/
				if(input_Password[counter_Comp] == password_Master[counter_Comp]){
					/**This flag indicates the correct elements**/
					flag_Success++;
					/**If all the elements are correct then continue with the tasks**/
					/**Clean the values of pressed key and flag success**/
					if(flag_Success == 4){flag_Process = PROCESS; valueKeyPressed = 0; flag_Success = 0;
					//////PARPADEA LED INICIO DOS VECES CON PERIODO DE 1 S

					}
				/**If an element is wrong then exit of the comparison**/
				/**Clean the values of pressed key and flag success**/
				}else{valueKeyPressed = 0; flag_Success = 0; break;
				/////////////PARPADEA LED ERROR DOS VECES CON PERIODO DE 1 S
				}
			}
		}
		/***********************************************************************************************************************************/

		/****************Comparison the keypad input with MOTOR PASSWORD********************************************************************/
		/***********************************************************************************************************************************/
		/**This condition starts when a password is pressed**/
		if((counter_PassW == 0)&&(valueKeyPressed != 0)&&(flag_Process == PROCESS)){
			/**The condition initiates the comparison between the pressed password and the correct password**/
			for(counter_Comp = 0; counter_Comp < 4; counter_Comp++){
				/**Comparison between the elements of passwords**/
				if(input_Password[counter_Comp] == password_Motor[counter_Comp]){
					/**This flag indicates the correct elements**/
					flag_Success++;
					/**If all the elements are correct then continue with the tasks**/
					/**Clean the values of pressed key and flag success**/
					if(flag_Success == 4){flag_Process = MOTOR; valueKeyPressed = 0; flag_Success = 0;
					//////////PARPADEA LED INICIO DOS VECES CON PERIODO DE 1 S
					}
				/**If an element is wrong then exit of the comparison**/
				/**Clean the values of pressed key and flag success**/
				}else{flag_Success = 0; break;
				////////PARPADEA LED ERROR DOS VECES CON PERIODO DE 1 S
				}
			}
		}

		/****************Comparison the keypad input with SIGNAL PASSWORD******************************************************************/
		/**********************************************************************************************************************************/
		/**This condition starts when a password is pressed**/
		if((counter_PassW == 0)&&(valueKeyPressed != 0)&&(flag_Process == PROCESS)){
			/**The condition initiates the comparison between the pressed password and the correct password**/
			for(counter_Comp = 0; counter_Comp < 4; counter_Comp++){
				/**Comparison between the elements of passwords**/
				if(input_Password[counter_Comp] == password_Signal[counter_Comp]){
					/**This flag indicates the correct elements**/
					flag_Success++;
					/**If all the elements are correct then continue with the tasks**/
					/**Clean the values of pressed key and flag success**/
					if(flag_Success == 4){flag_Process = SIGNAL; valueKeyPressed = 0; flag_Success = 0;
					/**************************************************/
					/*PARPADEA LED INICIO DOS VECES CON PERIODO DE 1 S*/
					/**************************************************/
					}
				/**If an element is wrong then exit of the comparison**/
				/**Clean the values of pressed key and flag success**/
				}else{flag_Success = 0; break;
				/**************************************************/
				/*PARPADEA LED ERROR DOS VECES CON PERIODO DE 1 S**/
				/**************************************************/
				}
			}
		}
		/************************************************PROCESS OF MOTOR********************************************************************/

		/**While the tag be MOTOR then execute the process**/
		if((flag_Process == MOTOR)){
			/**If key A or key B are pressed then clean the flags and return a Process Mode**/
			if((valueKeyPressed == KEY_A)||(valueKeyPressed == KEY_B)){
				/**The counter of key pressed is cleaned**/
				counter_PassW = 0;
				flag_Success = 0;
				flag_Process = PROCESS;
				motorCurrentState=0;
				valueKeyPressed = 0;
				//clearRows();
			}
			/********************************/
			/**Execute the Process of MOTOR**/
			/********************************/
			if(motorInit==0){
			   	PIT_clearSelect(PIT_2);
			    motorInit=2;
		   		motorStateMachine[motorCurrentState].ledON(GPIO_B,motorStateMachine[motorCurrentState].ledMotorBIT1);
		   		motorStateMachine[motorCurrentState].ledON(GPIO_B,motorStateMachine[motorCurrentState].ledMotorBIT2);
		  		motorStateMachine[motorCurrentState].ledOFF(GPIO_B,motorStateMachine[motorCurrentState].ledMotorBITOff);
			    	}
			    	//Se lanza la señal de activacion al puerto del motor y se ejecuta en base al tiempo de los estados
			    	if((TRUE==PIT_getIntrStutus2())&(motorControl==0)){
			    		motorStateMachine[motorCurrentState].output(GPIO_B,BIT20);
			    		PIT_delay(PIT_2,SYSTEM_CLOCK,motorStateMachine[motorCurrentState].time);
			    		motorControl=1;
			    	}
			    	//Cuando el PIT termina reinicia el control del motor y avanza de estado.
			    	if(TRUE==PIT_getIntrStutus2()){
			    		motorCurrentState=motorStateMachine[motorCurrentState].mNext[1];
			    		motorControl=0;
			    	}
			        Switch_2 = GPIOC->PDIR;
			        Switch_2 &=(0x40);
			        //Si se detecta el switch avanza al estado donde inicia la siguiente secuencia y se prepara para entrar en las inicializaciones.
			        if(Switch_2==FALSE){
			            motorInit=0;
			        	if(motorCurrentState==0){
			        		motorCurrentState=1;
			        	}
			        	else if(motorCurrentState>0&&motorCurrentState<5){
			            	motorCurrentState=5;
			            }
			            else if(motorCurrentState>0&&motorCurrentState>4){
			            	motorCurrentState=1;
			            }
			        }


		}
		/************************************************PROCESS OF SIGNAL*******************************************************************/

		/**While the tag be SIGNAL then execute the process**/
		if((flag_Process == SIGNAL)){
			/**If a key A or key B are pressed then clean the flags and return a Process Mode**/
			if((valueKeyPressed == KEY_A)||(valueKeyPressed == KEY_B)){
				counter_PassW = 0;
				flag_Success = 0;
				flag_Process = PROCESS;
				valueKeyPressed = 0;
				waveCurrentState=0;
				//clearRows();
			}
			/*******************************************/
			/**Execute the Process of SIGNAL GENERATOR**/
			/*******************************************/
			 //Inicializacion de cada estado, Carga el primer delay e inicia los LEDs que se requieren
			    	if(waveInit==0){
			    		PIT_delay(PIT_1,SYSTEM_CLOCK,waveStateMachine[waveCurrentState].delay);
			    		waveInit=2;
			    		waveStateMachine[waveCurrentState].RGB();
			    		waveStateMachine[waveCurrentState].ledON(GPIO_B,waveStateMachine[waveCurrentState].ledWaveBIT1);
			    		waveStateMachine[waveCurrentState].ledON(GPIO_B,waveStateMachine[waveCurrentState].ledWaveBIT2);
			    		waveStateMachine[waveCurrentState].ledOFF(GPIO_B,waveStateMachine[waveCurrentState].ledWaveBITOff);

			    	}
			    	//Tras la inicializacion el se inicia el recorrido de la tabla de referencia, solo se accesara al delay si el control lo permite.
			    	if((TRUE==PIT_getIntrStutus1())&&(waveControl==0)){
			    		PIT_delay(PIT_1,SYSTEM_CLOCK,waveStateMachine[waveCurrentState].delay);
			    		waveControl=1;
			    	}
			    	//Tras la interrupcion del pit se envia un dato al DAC y se recorre la tabla un punto.
			    	if(TRUE==PIT_getIntrStutus1()){
			        	waveStateMachine[waveCurrentState].wave(waveStateMachine[waveCurrentState].tableH[tablePosition],waveStateMachine[waveCurrentState].tableL[tablePosition]);
			    		tablePosition++;
			        	waveControl=0;

			    	}
			    	//Para limitar la table se tiene esta condicion que depende la longitud de la tabla, donde al llegar a su punto maximo se reiniciara
			    	if(tablePosition>waveStateMachine[waveCurrentState].tlength-1){
			    		tablePosition=0;
			    	}

			    	Switch_3 = GPIOA->PDIR;
			    	Switch_3 &=(0x10);
			    	//Se detecta el cambio del switch y se ejecutan los reinicios necesarios para avanzar el estado y que ejecute las inicializaciones de manera correcta
			    	if(Switch_3==FALSE){
			    		waveCurrentState=waveStateMachine[waveCurrentState].next[0];
			    		tablePosition=0;
			    		PIT_clearSelect(PIT_1);
			    		waveInit=0;


			    		//Si no se detecta el switch se continua en el estado actual
			    	}else{
			    		waveCurrentState=waveStateMachine[waveCurrentState].next[1];
			    	}
		}

	}
    return 0;
}
void globalClockGating(){
	//Se activan todos los clock gating necesarios.
	GPIO_clockGating(GPIO_D);
	GPIO_clockGating(GPIO_A);
	GPIO_clockGating(GPIO_B);
	GPIO_clockGating(GPIO_C);
	GPIO_clockGating(GPIO_E);

	DAC_ClockGating();
	PIT_clockGating();
}
void globalSetup(){
	DACSetup();
	setAllRGB();
	//Se decide como actuaran los puertos y que pines se usaran
	GPIO_pinControlRegisterType pinControlRegisterPORTD = GPIO_MUX1;
	GPIO_pinControlRegisterType pinControlRegisterPORTA = GPIO_MUX1|GPIO_PE|GPIO_PS;
	GPIO_pinControlRegisterType pinControlRegisterPORTB = GPIO_MUX1|GPIO_PE|GPIO_PS;
	GPIO_pinControlRegisterType pinControlRegisterPORTC = GPIO_MUX1|GPIO_PE|GPIO_PS;
	GPIO_pinControlRegister(GPIO_C,BIT6,&pinControlRegisterPORTC);
	GPIO_pinControlRegister(GPIO_B,BIT2,&pinControlRegisterPORTB);
	GPIO_pinControlRegister(GPIO_B,BIT3,&pinControlRegisterPORTB);
	GPIO_pinControlRegister(GPIO_B,BIT10,&pinControlRegisterPORTB);
	GPIO_pinControlRegister(GPIO_B,BIT11,&pinControlRegisterPORTB);
	GPIO_pinControlRegister(GPIO_B,BIT20,&pinControlRegisterPORTB);
	GPIO_pinControlRegister(GPIO_A,BIT4,&pinControlRegisterPORTA);
	GPIO_pinControlRegister(GPIO_D,BIT0,&pinControlRegisterPORTD);
	//Se define si los pines van a actuar como salida o como entrada
	GPIO_dataDirectionPIN(GPIO_C,GPIO_INPUT,BIT6);
	GPIO_dataDirectionPIN(GPIO_D,GPIO_OUTPUT,BIT0);
	GPIO_dataDirectionPIN(GPIO_A,GPIO_INPUT,BIT4);
	GPIO_dataDirectionPIN(GPIO_B,GPIO_OUTPUT,BIT2);
	GPIO_dataDirectionPIN(GPIO_B,GPIO_OUTPUT,BIT3);
	GPIO_dataDirectionPIN(GPIO_B,GPIO_OUTPUT,BIT10);
	GPIO_dataDirectionPIN(GPIO_B,GPIO_OUTPUT,BIT11);
	GPIO_dataDirectionPIN(GPIO_B,GPIO_OUTPUT,BIT20);
	//Se ponen los pines en una posicion default;
	GPIO_clearPIN(GPIO_D,BIT0);
	GPIO_clearPIN(GPIO_A,BIT4);
	GPIO_clearPIN(GPIO_B,BIT2);
	GPIO_clearPIN(GPIO_B,BIT3);
	GPIO_clearPIN(GPIO_B,BIT10);
	GPIO_clearPIN(GPIO_B,BIT11);
	GPIO_clearPIN(GPIO_B,BIT20);
	GPIO_clearPIN(GPIO_C,BIT6);

}
