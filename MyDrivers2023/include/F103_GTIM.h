#ifndef F103_GTIM_H
#define F103_GTIM_H

// Default Hardware Pins
// F103RB
//
// TIM1 : CH1 PA8 CH2 PA9 CH3 PA10 CH4 PA11 ETR PA12 
// TIM1 : CH1N PB13 CH2N PB14  BKIN PB12
// TIM2 : CH1 PA0 CH2 PA1 CH3 PA2 CH4 PA3
// TIM3 : CH1 PA6 CH2 PA7 CH3 PB0 CH4 PB1
// TIM4 : CH1 PB6 CH2 PB7 CH3 PB8 CH4 PB9
#define REMAP 1
#define NO_REMAP 0
#define MODE_1 0
#define MODE_2 1


#include "stm32f10x.h"
#include "F103_RCC.h" // need Clock defines PCLK1 = TIM2 TIM3... and PCLK2 = TIM1
#include "F103_GPIO.h"
#include "F103_IT.h"

typedef struct{

TIM_TypeDef * Timer;
	uint16_t ARR;
	uint16_t PSC;

} MyTimer_Struct_TypeDef;

// ***************************************************************************************
//
// MyTimer_Base Init : TIM register configuration
//
// Counter is NOT enabled, use My_Timer_Base_Start Macro
//****************************************************************************************
void F103_GTIM_Base_Init (TIM_TypeDef * Timer, uint16_t arr, uint16_t psc);

// ***************************************************************************************
//
// PWM functions
// Use F103_GTIM_Base_Init to configure Timer clock ARR and PSC
// Use F103_GTIM_PWM to configure dutycycle and mode (de-enables COUNTER)
// Use F103_GTIM_PWM_Pin_Conf to route OC output to GPIO pins and remap option
// Use My_Timer_Base_Start to start PWM generation
//
// @ channel = 1 to 4
// @ KdutyCycle : 0 to 1000, steps of 1/1000 for accuracy
//****************************************************************************************


// F103_GTIM_PWM : Sets CCR  Dutycycle from 0 to 1000, mode 1 (Normal) or 2 (Inverse), 
// Disables Counter,  Channel is 1 to 4
void F103_GTIM_PWM(TIM_TypeDef *Timer, char Channel, int KdutyCycle, int mode);

// F103_PWM_Pin_Conf : GPIO conf for PWM output channel. Only FULL remap supported (REMAP)
void F103_GTIM_PWM_PinConf(TIM_TypeDef *Timer, char Channel, uint8_t remap);
void F103_GTIM_PWM_Update(TIM_TypeDef *Timer, char Channel,int KdutyCycle);

/****************************************************************************************/
/*																																											*/
/*												Encoder Interface Mode Function																*/
/*  Does NOT Start counter																															*/
/*  TIM1 : CH1 = A8  CH2 = A9  5V Tolerant																							*/
/*	Counts +/-2 (Mode 1/2) +/-4 (Mode 3)																								*/
/*	Check Filter settings (recommended to avoid glitches/noise													*/
/*																																											*/
/****************************************************************************************/
// Encoder Interface Mode EIM Pin configure , T1+T2 on CH1 and CH2. Full remap supported
// Filter Ftds/8 8 samples
void F103_GTIM_EIM_PinConf(TIM_TypeDef *Timer,uint8_t remap);



// ***************************************************************************************
//
// IT functions
//
// 
//****************************************************************************************

void F103_GTIM_ActiveIT(TIM_TypeDef * Timer, char prio,void (*IT_Func_Ptr) (void));


//Macros

#define F103_GTIM_Base_Start(Timer) Timer->CR1 |= TIM_CR1_CEN 
#define F103_GTIM_Base_Stop(Timer) Timer->CR1 &= ~(TIM_CR1_CEN) 

#endif

