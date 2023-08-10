/*
  
  ******************************************************************************
  *
  * IT Routines and Handlers
	* F103 RB
	* 2023 M.Bonnet
  *
 ******************************************************************************
  */

#include "stm32f10x.h"
#include "F103_GPIO.h"
#ifndef __F103_IT_H
#define __F103_IT_H

/* Exported functions prototypes ---------------------------------------------*/

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void TIM1_UP_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void TIM1_BRK_IRQHandler(void); //Not implemented
void TIM1_TRG_COM_IRQHandler(void); //Not Implemented

void USART1_IRQHandler (void);
void USART2_IRQHandler (void);
void USART3_IRQHandler (void);

void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);


void DMA1_Channel1_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);

/*******************************************************************************
*  
*      EXTI initialisation
*      On rising edge RSR
*      pins 0->7 (8+ : not implemented)
*  IRQn : EXTI 0 -> EXTI 4 : EXTI# +6 (i.e. : EXTI0, IRQn = 6)
*  EXTI 5-9 : IRQn=23
*******************************************************************************/
void F103_IT_EXTI_Init(MyGPIO_Struct_TypeDef *myGpio, uint8_t prio, void (*IT_Func)(void));


/******************************************************************
 * Getters/Setters
 *
 ******************************************************************
 */
// Systick IT (RCC Module)

uint32_t GetSysTickCounter(void); //Gets SysCount in IT Handler
//void ClearSysCount(void);   //Clears SysCount in IT Handler
//uint64_t GetTIM2Count(void); //Gets TIM2 Count in IT Handler
//void ClearTIM2Count(void);   //Clears TIM2 Count in IT Handler

/* -------------------------------------------------------------------
 *
 * USER CALLBACKS - Registration
 *
 * -------------------------------------------------------------------
*/
// Registered Function Pointers
/*
void (*IT_SysTick) (void); // CallBack for SysTick
void (*IT_TIM [8])(void);	//Callback array of function pointers for TIM
void (*IT_EXTI[5]) (void);   //Callback array of function pointers for EXTI
void (*IT_DMA_TC[7])(void); // Callback for DMA1 handlers Transfer Complete
void (*IT_DMA_HT[7])(void); // Callback for DMA1 handlers Half Transfer
*/

// Callback Register Functions

	// Systick IT (RCC)
	void Register_SysTick_Callback (void (*IT_Function)(void)); 

	// TIM IT (TIM)
	void Register_GTIM_Callback (TIM_TypeDef *TIM,void (*IT_Function)(void) );

	// EXTI IT
	void Register_EXTI_Callback (uint8_t ExtiLine, void(*IT_Function)(void));

	// DMA
	void Register_DMA_HT_Callback (uint8_t Channel, void(*IT_Function)(void));
	void Register_DMA_TC_Callback (uint8_t Channel, void(*IT_Function)(void));
	// USART
	void Register_USART_Callback(USART_TypeDef * usart,void(*IT_Function) (void));
#endif /* __STM32F103_IT_H */

