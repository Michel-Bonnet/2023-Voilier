#include "F103_RCC.h"
#include "F103_SYSTICK.h"

#include "Bordage.h"
#include "Comm.h"
#include "Plateau.h"
#include "Surveil.h"
#include <string.h>  

/*
*   VOILIER 01
* TX, RX : DMA, ADC : DMA
* BOR : Bordage
* COM : Communications
* PLA : Plateau
* SUR : Surveillance
*/

int main(void)
{

//Local	variables
	
uint32_t currentTick;  									// time elapsed in ms
uint16_t Roll_Status=0, Batt_Status=0;  // Alarm status flags
	
// RCC Init
RCC_Init();

// Systick start, overflow 1ms
F103_SysTick_Init(72000-1);


// Periph init & config : .h for details
BOR_Init();
COM_Init();
PLA_Init();
SUR_Init();


//peripherals start
BOR_Start();
COM_Start();
PLA_Start();
SUR_Start();

// Wait encoder zero 
BOR_Wait_Zero();

// main loop
  while(1){
		
		currentTick=GetSysTickCounter();
	  if(currentTick%500 ==0)
		{
		 SUR_Status_Update( &Batt_Status,&Roll_Status); // Sets flags if alarm conditions are met
		 COM_Send_Bor_Message();  	 										//Sends ship status message
		 COM_Send_Alarm(Batt_Status,Roll_Status);				//Sends alarm status message
		}
		
		if(currentTick%20 ==0){
				PLA_Setpoint();		// Reads TX signed char and sends PWM to rotating board
			}
			
		if(currentTick%10 ==0){
				if (Roll_Status==0)  BOR_Setpoint(); else BOR_PWM_US(SERVO_LOW_DUR); // Sends PWM to servo
			}
	
	while(currentTick== GetSysTickCounter()); // Waits for next systick overflow
	}

}
