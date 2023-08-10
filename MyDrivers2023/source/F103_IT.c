/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/


#include "F103_IT.h"

/*
 *
 * User Globals -------------------------------------------------------------
 *
*/
static volatile unsigned int scount=0; //systick overflow count
/*
 * USER CALLBACKS / FUNCTION POINTERS
 */

void (*IT_SysTick) (void)= 0; 		// CallBack for SysTick
void (*IT_TIM [8])(void) ={0};		//Callback array of function pointers for TIM
																	// 0 : TIM1  ... 3:TIM4
void (*IT_EXTI[16]) (void)={0};   	//Callback array of function pointers for EXTI
void (*IT_DMA_TC[7])(void) = {0}; // Callback for DMA1 handlers Transfer Complete
void (*IT_DMA_HT[7])(void) = {0}; // Callback for DMA1 handlers Half Transfer
void (*IT_USART[3])(void)={0}; 		//Callback for USART handlers

/*******************************************************************************
*  
*      EXTI initialisation
*
*  IRQn : EXTI 0 -> EXTI 4 : EXTI# +6 (i.e. : EXTI0, IRQn = 6)
*  EXTI 5-9 : IRQn=23
*******************************************************************************/

void F103_IT_EXTI_Init(MyGPIO_Struct_TypeDef *myGpio, uint8_t prio, void (*IT_Func)(void))
{
	uint32_t irqNum = myGpio->GPIO_Pin+6;
	if (myGpio->GPIO_Pin>4) irqNum=23;
  F103_GPIO_Init(myGpio);
	EXTI->IMR |= (0x01<<myGpio->GPIO_Pin);	// set IMR mask to 1 : unmask IT
	EXTI->RTSR |= (0x01<<myGpio->GPIO_Pin); // Enable IT on rising edge
  if ((myGpio->GPIO==GPIOA)&& (myGpio->GPIO_Pin<4))  AFIO->EXTICR[0] |= (0x0<<((myGpio->GPIO_Pin)*4)); // Select Port for EXTI line <Pin>
  if ((myGpio->GPIO==GPIOA)&& (myGpio->GPIO_Pin>4))  AFIO->EXTICR[1] |= (0x0<<(((myGpio->GPIO_Pin)%8)*4));
	if ((myGpio->GPIO==GPIOB)&& (myGpio->GPIO_Pin<4))  AFIO->EXTICR[0] |= (0x1<<((myGpio->GPIO_Pin)*4));
  if ((myGpio->GPIO==GPIOB)&& (myGpio->GPIO_Pin>4))  AFIO->EXTICR[1] |= (0x1<<(((myGpio->GPIO_Pin)%8)*4));
	if ((myGpio->GPIO==GPIOC)&& (myGpio->GPIO_Pin<4))  AFIO->EXTICR[0] |= (0x2<<((myGpio->GPIO_Pin)*4));
  if ((myGpio->GPIO==GPIOC)&& (myGpio->GPIO_Pin>4))  AFIO->EXTICR[1] |= (0x2<<(((myGpio->GPIO_Pin)%8)*4));
	
	Register_EXTI_Callback(myGpio->GPIO_Pin,IT_Func);
	NVIC_SetPriority((IRQn_Type) irqNum,prio);
	NVIC_EnableIRQ((IRQn_Type)irqNum);
}



/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{while (1);}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{while (1);}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{while (1);}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{while (1);}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{while (1);}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{while (1);}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{while (1);}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{while (1);}

/**
  * @brief This function handles System tick timer.
  */

void SysTick_Handler(void) //NVIC automatically resets Pending flag on active ISR
{
	
	if(IT_SysTick!=0) (*IT_SysTick) ();
	   scount++;
	
}
/* ---------------------------------------------------------------------
 * USER CODE Getters/Setters
 *
 */
// Systick IT (RCC Module)

uint32_t GetSysTickCounter(void) //Gets SysCount in IT Handler

{
return(scount);
}


/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/* ----------------------------------------------------------------------------
 * USER INTERRUPT HANDLERS
 * *** CLEAR IT FLAG in IT_ROUTINE
 *
 ----------------------------------------------------------------------------*/
/*
 *  TIM Handlers
 */
	void TIM1_UP_IRQHandler(void)
	{if(TIM1->SR & TIM_SR_UIF) TIM1->SR &= ~TIM_SR_UIF;	if(IT_TIM[0]!=0)(*IT_TIM[0])();}
	void TIM2_IRQHandler(void)
	{if(TIM2->SR & TIM_SR_UIF) TIM2->SR &= ~TIM_SR_UIF;	if(IT_TIM[1]!=0)(*IT_TIM[1])();}
	void TIM3_IRQHandler(void)
	{if(TIM3->SR & TIM_SR_UIF)TIM3->SR &= ~TIM_SR_UIF;	if(IT_TIM[2]!=0)(*IT_TIM[2])();}
	void TIM4_IRQHandler(void)
	{if(TIM4->SR & TIM_SR_UIF)TIM4->SR &= ~TIM_SR_UIF;	if(IT_TIM[3]!=0)(*IT_TIM[3])();}
	void TIM5_IRQHandler(void)
	{if(TIM5->SR & TIM_SR_UIF)TIM5->SR &= ~TIM_SR_UIF;	if(IT_TIM[4]!=0)(*IT_TIM[4])();}
	// TIM1 : additional check of UIF interrupt flag, line is shared with TIM1
	
	
	// TIM1 BRK COM IT NOT IMPLEMENTED
	// BRK -> IT_TIM[5]=IT_Function;
	// COM -> IT_TIM[6]=IT_Function;
	void TIM1_BRK_IRQHandler(void)
	{if(TIM1->SR & TIM_SR_BIF) TIM1->SR &= ~TIM_SR_BIF;	if(IT_TIM[5]!=0)(*IT_TIM[5])();}
	void TIM1_TRG_COM_IRQHandler(void)
	{if(TIM1->SR & TIM_SR_COMIF) TIM1->SR &= ~TIM_SR_COMIF;	if(IT_TIM[6]!=0)(*IT_TIM[6])();}


/*
 *  EXTI Handlers
 */
	void EXTI0_IRQHandler(void)
	{ EXTI->PR |=EXTI_PR_PR0;if(IT_EXTI[0]!=0)(*IT_EXTI[0])();}
	void EXTI1_IRQHandler(void)
	{ EXTI->PR |=EXTI_PR_PR1;if(IT_EXTI[1]!=0)(*IT_EXTI[1])();}
	void EXTI2_IRQHandler(void)
	{ EXTI->PR |=EXTI_PR_PR2;if(IT_EXTI[2]!=0)(*IT_EXTI[2])();}
	void EXTI3_IRQHandler(void)
	{ EXTI->PR |=EXTI_PR_PR3;if(IT_EXTI[3]!=0)(*IT_EXTI[3])();}
	void EXTI4_IRQHandler(void)
	{ EXTI->PR |=EXTI_PR_PR4;if(IT_EXTI[4]!=0)(*IT_EXTI[4])();}
	
void EXTI9_5_IRQHandler(void)
	{
		uint8_t line;
		for(line=5;line<10;line++)
		if (EXTI->PR & ((0x01)<<line)){
			EXTI->PR |= (0x01<<line); //Clear IT Flag
			if(IT_EXTI[line]!=0)(*IT_EXTI[line])();
		}
	}
	void EXTI15_10_IRQHandler(void)
	{
		uint8_t line;
		for(line=10;line<=15;line++)
		if (EXTI->PR & ((0x01)<<line)){
			EXTI->PR |= (0x01<<line); //Clear IT Flag
			if(IT_EXTI[line]!=0)(*IT_EXTI[line])();
		}
	}
	
/*
 *
 * DMA1 handlers
 *
 */
void DMA1_Channel1_IRQHandler(void)
	{if((DMA1->ISR & DMA_ISR_TCIF1)!=0) {DMA1->IFCR |=DMA_IFCR_CTCIF1; if((IT_DMA_TC[0])!=0) (*IT_DMA_TC[0])(); }
	if((DMA1->ISR & DMA_ISR_HTIF1)!=0) {DMA1->IFCR |=DMA_IFCR_CHTIF1; if((IT_DMA_HT[0])!=0) (*IT_DMA_HT[0])(); }	}
void DMA1_Channel2_IRQHandler(void)
	{if((DMA1->ISR & DMA_ISR_TCIF2)!=0) {DMA1->IFCR |=DMA_IFCR_CTCIF2; if((IT_DMA_TC[1])!=0) (*IT_DMA_TC[1])(); }
	if((DMA1->ISR & DMA_ISR_HTIF2)!=0) {DMA1->IFCR |=DMA_IFCR_CHTIF2; if((IT_DMA_HT[1])!=0) (*IT_DMA_HT[1])(); }	}	
void DMA1_Channel3_IRQHandler(void)
	{if((DMA1->ISR & DMA_ISR_TCIF3)!=0) {DMA1->IFCR |=DMA_IFCR_CTCIF3; if((IT_DMA_TC[2])!=0) (*IT_DMA_TC[2])(); }
	if((DMA1->ISR & DMA_ISR_HTIF3)!=0) {DMA1->IFCR |=DMA_IFCR_CHTIF3; if((IT_DMA_HT[2])!=0) (*IT_DMA_HT[2])(); }	}		
void DMA1_Channel4_IRQHandler(void)
	{if((DMA1->ISR & DMA_ISR_TCIF4)!=0) {DMA1->IFCR |=DMA_IFCR_CTCIF4; if((IT_DMA_TC[3])!=0) (*IT_DMA_TC[3])(); }
	if((DMA1->ISR & DMA_ISR_HTIF4)!=0) {DMA1->IFCR |=DMA_IFCR_CHTIF4; if((IT_DMA_HT[3])!=0) (*IT_DMA_HT[3])(); }	}	
void DMA1_Channel5_IRQHandler(void)
	{if((DMA1->ISR & DMA_ISR_TCIF5)!=0) {DMA1->IFCR |=DMA_IFCR_CTCIF5; if((IT_DMA_TC[4])!=0) (*IT_DMA_TC[4])(); }
	if((DMA1->ISR & DMA_ISR_HTIF5)!=0) {DMA1->IFCR |=DMA_IFCR_CHTIF5; if((IT_DMA_HT[4])!=0) (*IT_DMA_HT[4])(); }	}	
void DMA1_Channel6_IRQHandler(void)
	{if((DMA1->ISR & DMA_ISR_TCIF6)!=0) {DMA1->IFCR |=DMA_IFCR_CTCIF6; if((IT_DMA_TC[5])!=0) (*IT_DMA_TC[5])(); }
	if((DMA1->ISR & DMA_ISR_HTIF6)!=0) {DMA1->IFCR |=DMA_IFCR_CHTIF6; if((IT_DMA_HT[5])!=0) (*IT_DMA_HT[5])(); }	}	
void DMA1_Channel7_IRQHandler(void)
	{if((DMA1->ISR & DMA_ISR_TCIF7)!=0) {DMA1->IFCR |=DMA_IFCR_CTCIF7; if((IT_DMA_TC[6])!=0) (*IT_DMA_TC[6])(); }
	if((DMA1->ISR & DMA_ISR_HTIF7)!=0) {DMA1->IFCR |=DMA_IFCR_CHTIF7; if((IT_DMA_HT[6])!=0) (*IT_DMA_HT[6])(); }	}	

	
	/*
	*    USART Handler
	*    Interrpt flag must be cleared
	*/
	void USART1_IRQHandler (void)
	{ if (IT_USART[0] !=0) (*IT_USART[0])();}
	void USART2_IRQHandler (void)
	{ if (IT_USART[1] !=0) (*IT_USART[1])();}
	void USART3_IRQHandler (void)
	{ if (IT_USART[2] !=0) (*IT_USART[2])();}
	
/*
 * USER CALLBACK REGISTRATION
 *
 */
void Register_SysTick_Callback (void (*IT_Function)(void))
{
if(IT_Function!=0)IT_SysTick=IT_Function;
}

void Register_GTIM_Callback (TIM_TypeDef *TIM,void (*IT_Function)(void) ){
	if(TIM==TIM2)IT_TIM[1]=IT_Function;
	if(TIM==TIM3)IT_TIM[2]=IT_Function;
	if(TIM==TIM4)IT_TIM[3]=IT_Function;
	if(TIM==TIM5)IT_TIM[4]=IT_Function;
	if(TIM==TIM1)IT_TIM[0]=IT_Function;
	// TIM1 BRK COM IT NOT IMPLEMENTED
	// BRK -> IT_TIM[5]=IT_Function;
	// COM -> IT_TIM[6]=IT_Function;

}

void Register_EXTI_Callback (uint8_t ExtiLine, void(*IT_Function)(void))
{
	IT_EXTI[ExtiLine]=IT_Function; }

void Register_DMA_HT_Callback (uint8_t Channel, void(*IT_Function)(void))
{
	IT_DMA_HT[Channel-1]=IT_Function	;}

void Register_DMA_TC_Callback (uint8_t Channel, void(*IT_Function)(void))
{
	IT_DMA_TC[Channel-1]=IT_Function	;}


void Register_USART_Callback(USART_TypeDef *usart,void(*IT_Function) (void))
	{
if(usart==USART1) IT_USART[0]=IT_Function;
if(usart==USART2) IT_USART[1]=IT_Function;
if(usart==USART3) IT_USART[2]=IT_Function;
}
