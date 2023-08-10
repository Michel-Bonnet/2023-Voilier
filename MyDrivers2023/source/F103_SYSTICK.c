
#include "F103_SYSTICK.h"

static volatile int timeDelay;



void F103_SysTick_Init(uint32_t ticks)
{
	
	SysTick->CTRL = 0; // Clear CTRL
	SysTick->LOAD = ticks;
	SysTick->VAL = 0;
	NVIC_SetPriority(SysTick_IRQn,15);

	SysTick->CTRL|= SysTick_CTRL_CLKSOURCE;
	SysTick->CTRL|= SysTick_CTRL_TICKINT;
	SysTick->CTRL|= SysTick_CTRL_ENABLE;

}


