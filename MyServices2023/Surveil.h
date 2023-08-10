#ifndef SURVEIL_H
#define SURVEIL_H

#define SUR_FREEZE 5  // ticks to deactivate roll alarm
#define SUR_BATT_TRESH  0x0200 // Battery treshhold in ADC counts

#include "stm32f10x.h"
#include "ADXL345.h"
#include "F103_DMA.h"
#include "F103_ADC.h"
#include "F103_SPI.h"
#include "F103_IT.h"

void SUR_Init(void); // Init SPI, ADXL and ADC
void SUR_Start(void);
void SUR_Status_Update(uint16_t *Bat_st, uint16_t * Roll_st);
int SUR_Get_Adcval (void);
#endif


