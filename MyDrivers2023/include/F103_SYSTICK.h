#ifndef MY_SYSTICK_H
#define MY_SYSTICK_H

#include "stm32f10x.h"
#include "F103_IT.h"

//----------------------------------------------------
//
// Configures systick 
// software delay (in us)
//
//----------------------------------------------------


// Configures 24bit systick downcounter and enables IT on overflow of SYSCLK (72MHz)
// Counts from Ticks to 0 (Ticks+1 CLK cycles)
// Ticks 0 to 0x0FFFFFF
// Ticks=71: 1us  Ticks=71999: 1ms   Ticks=0xFFFFFF: 0.233 s
void F103_SysTick_Init(uint32_t ticks);

// Delay from 1us (delay =1) to 4295s (delay = 0xFFFFFFFF)
// IF ticks = 72
//void SysTick_Delay(uint32_t delay);


#endif
