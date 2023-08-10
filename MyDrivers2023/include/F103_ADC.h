 #ifndef F103_ADC_H
 #define F103_ADC_H

 
 
 
#include "stm32f10x.h"
#include "F103_GPIO.h"
#include "F103_DMA.h"

// Sample Time Codes defines Add 12.5 Cy for total conversion time
#define ADC_TS_2 		0 // 1.5 Cycles   Total 12.5+1.5 = 14 Cy @12MHz 1.2 탎
#define ADC_TS_8 		1 // 7.5 Cycles   Tc  1.7 탎 
#define ADC_TS_14 	2 // 13.5 Cycles  Tc  2.2 탎
#define ADC_TS_29 	3 // 28.5 Cycles  Tc  3.4 탎
#define ADC_TS_42 	4 // 41.5 Cycles  Tc  4.5 탎
#define ADC_TS_56 	5 // 55.5 Cycles  Tc  5.7 탎
#define ADC_TS_72 	6 // 71.5 Cycles  Tc  7.0 탎
#define ADC_TS_239 	7 // 239 Cycles   Tc 20.9 탎 (17.5탎 @ 14MHz)

#define ADC1_START (ADC1->CR2 |= ADC_CR2_ADON)
/*
*
* ADC Pinout
* CH0 A0  CH1 A1 CH2 A2 CH3 A3  CH4 A4  CH5 A5 CH6  A6  CH7 A7  
* CH8 B0  CH9 B1 CH10 C0 CH11 C1 CH12 C2 CH13 C3 CH14 C4  CH15 C5
*
*/

void F103_ADC_Single_Init(ADC_TypeDef *Adc, uint8_t Channel, uint8_t SampleTime);
int F103_ADC_Single_Conv(ADC_TypeDef *Adc);

void F103_ADC_DMA_SingleCont_Init( uint8_t adcChannel, uint8_t SampleTime);

// Multi Channel with Scan chSequence is a 16 element array of bytes
void F103_ADC1_DMA_SingleTRGO_RegScan_Init(uint8_t chNumber,uint8_t *chSequence,uint8_t * SampleSeq); // regular scan sequence TIM3 TRGO
 #endif

