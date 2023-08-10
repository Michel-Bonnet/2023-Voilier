#ifndef E6B2_H
#define E6B2_H

#include "stm32f10x.h"
#include "F103_GPIO.h"
#include "F103_GTIM.h"
#include "F103_IT.h"



#define E6B2_TIM (TIM2)
#define E6B2_IT_PORT GPIOA
#define E6B2_IT_PIN 04
#define E6B2_IT_PRIO 05
/*
* Usage
* E6B2_IT : write IT for action when Zero is detected
* E6B2_Init : set encoder mode 3 on E6B2_TIM CH1 and CH2 with some filtering
* E6B2_Start : configures EXTI on IT_PIN . Register and Enables IT. Starts TIM
* E6B2_Get_Angle : direct signed reading of angle in degrees.
* E6B2_Get_Zcount : number of times Zero pulse has been detected.
* 
* Recommendation : use  while( E6B2_Get_Zcount()==0) to initialize encoder.
*
*/
void E6B2_IT (void);
void E6B2_Init(void);
void E6B2_Start(void);
int E6B2_Get_Angle(void);
int E6B2_Get_Zcount(void);





#endif
