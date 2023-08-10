#ifndef PLATEAU_H
#define PLATEAU_H


#include "stm32f10x.h"
#include "F103_USART.h"
#include "F103_DMA.h"
#include "F103_GPIO.h"
#include "F103_GTIM.h"


#define PLA_PWM_TIM (TIM3)  //PWM TIMER 
#define PLA_PWM_CH  3		    //Channel : TIM3 CH3 = B0
#define PLA_DIR_PIN 2       // DIR PIN and Port
#define PLA_DIR_PORT (GPIOB) 
#define PLA_DEAD_ZONE 10    //Ignores values inf to DEAD_ZONE
#define RX_USART (USART1)


void PLA_Init(void);   //Only init USART RX
void PLA_Start(void);
void PLA_Setpoint(void); // Sends PWM to rotating board







#endif

