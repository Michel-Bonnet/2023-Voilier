/*
RCC Init and functions
06-2023
M.Bonnet
XTAL : 8MHz
HSE prediv 1   PLLMult : x9
AHB Presc /1	APB1Presc /2
APB2Presc /1	ADC Pres /6 (-> 12MHz, Max Adc 14MHz)
HCLK : 72MHz
PCLK1 : 36MHz
PCLK2 : 72MHz
ADCClk : 12 MHz

*/
#ifndef F103_RCC_H
#define F103_RCC_H

#include "stm32f10x.h"
//----------------------------------------------------
//
// Configures clock registers 
//
//----------------------------------------------------

// Clock Defines
// Used in USART to calculate BRR etc

#define HSI_RC 	08000000 // External XTal
#define SYSCLK 	72000000
#define PCLK1 	36000000
#define PCLK2 	72000000
#define TCLK1 	72000000
#define TCLK2 	72000000


void RCC_Init (void); // Initialize Clock System
#endif
