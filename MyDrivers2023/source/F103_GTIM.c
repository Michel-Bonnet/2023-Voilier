

#include "F103_GTIM.h"

void F103_GTIM_Base_Init (TIM_TypeDef * Timer, uint16_t arr, uint16_t psc)
{

	if (Timer == TIM1)  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN ; 
	if (Timer == TIM2)  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 
	if (Timer == TIM3)  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; 
	if (Timer == TIM4)  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; 
	

Timer->CR1 |= 0 ; //Reset CR1
Timer->ARR = arr;
Timer->PSC = psc;
}

void F103_GTIM_ActiveIT(TIM_TypeDef * Timer, char prio, void (*IT_Func_Ptr) (void) )
{
	Register_GTIM_Callback(Timer,IT_Func_Ptr);
// Enable TIM IRQ (DIER / UIE ) and define IT function pointer

	if (Timer == TIM1)  {TIM1->DIER|= TIM_DIER_UIE; NVIC_SetPriority(TIM1_UP_IRQn,(uint32_t)prio); 
											NVIC_EnableIRQ(TIM1_UP_IRQn); } 
	if (Timer == TIM2)  {TIM2->DIER|= TIM_DIER_UIE; NVIC_SetPriority(TIM2_IRQn,(uint32_t)prio);
											NVIC_EnableIRQ(TIM2_IRQn); } 
	if (Timer == TIM3)  {TIM3->DIER|= TIM_DIER_UIE;TIM3->CR1|=TIM_CR1_URS; NVIC_SetPriority(TIM3_IRQn,(uint32_t)prio);
											NVIC_EnableIRQ(TIM3_IRQn); }
	if (Timer == TIM4)  {TIM4->DIER|= TIM_DIER_UIE;TIM4->CR1|=TIM_CR1_URS; NVIC_SetPriority(TIM4_IRQn,(uint32_t)prio);
											NVIC_EnableIRQ(TIM4_IRQn); }
	
}


// PWM Functions Mode

void F103_GTIM_PWM(TIM_TypeDef *Timer, char Channel,int KdutyCycle, int mode)
{
int ccr;
	
	
	KdutyCycle=KdutyCycle%1001;   // Eliminate irrelevant values
	ccr = (KdutyCycle*(Timer->ARR+1)) / 1000;
	
	Timer->CR1 &= ~TIM_CR1_CEN ; //Disable Counter
	Timer->EGR |= TIM_EGR_UG; //SET Update generation as per RM 15.3.9
	Timer->CR1 |= TIM_CR1_ARPE; //Enable buffered ARR register
	Timer->CR1 &= ~TIM_CR1_CMS; // Clear CMS bits = Edge aligned
	
	switch(Channel%5) 
{
	case (1) :  
			Timer->CCMR1&= ~TIM_CCMR1_CC1S; // Clear 2 last bits = Channel is output
			Timer->CCER |= TIM_CCER_CC1E; //Enable output
			if(mode==1) Timer->CCMR1 |= TIM_CCMR1_OC1M; //Mode 1
			if(mode!=1) Timer->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1); //Mode 2
			Timer->CCR1 = ccr;  //SET CCR Value
			break;
	case (2) :  
			Timer->CCMR1 &= ~TIM_CCMR1_CC2S; // Clear 2 last bits = Channel is output
			Timer->CCER |= TIM_CCER_CC2E;
			if(mode==1) Timer->CCMR1 |= TIM_CCMR1_OC2M; //Mode 1
			if(mode!=1) Timer->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1); //Mode 2
			Timer->CCR2 = ccr;  //SET CCR Value
			break;
	case (3) :   
			Timer->CCMR2 &= ~TIM_CCMR2_CC3S; // Clear 2 last bits = Channel is output
			Timer->CCER |= TIM_CCER_CC3E;
			if(mode==1) Timer->CCMR2 |= TIM_CCMR2_OC3M; //Mode 1
			if(mode!=1) Timer->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1); //Mode 2
			Timer->CCR3 = ccr;  //SET CCR Value
			break;
	case (4) :
			Timer->CCMR2 &= ~TIM_CCMR2_CC4S; // Clear 2 last bits = Channel is output
	    Timer->CCER |= TIM_CCER_CC4E;
			if(mode==1) Timer->CCMR2 |= TIM_CCMR2_OC4M; //Mode 1
			if(mode!=1) Timer->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1); //Mode 2
			Timer->CCR4 = ccr;  //SET CCR Value
			break;
	
		}	
	}

	void F103_GTIM_PWM_Update(TIM_TypeDef *Timer,char Channel,int KdutyCycle)
	{
	int ccr; 
	KdutyCycle=KdutyCycle%1001;
	ccr = (KdutyCycle*(Timer->ARR+1)) / 1000; //In Edge Aligned mode PWM=1 if ccr>arr
	
			if(Channel==1) Timer->CCR1 = ccr;  //SET CCR Value	
			if(Channel==2) Timer->CCR2 = ccr;  //SET CCR Value
			if(Channel==3) Timer->CCR3 = ccr;  //SET CCR Value
			if(Channel==4) Timer->CCR4 = ccr;  //SET CCR Value
	
		}	

// MyTimer_PWM_Conf
// Configures pins for CH1 to CH4 only
// TIM1 : CH1 PA8 CH2 PA9 CH3 PA10 CH4 PA11 ETR PA12 
// TIM1 : CH1N PB13 CH2N PB14  BKIN PB12
// TIM2 : CH1 PA0 CH2 PA1 CH3 PA2 CH4 PA3
// TIM3 : CH1 PA6 CH2 PA7 CH3 PB0 CH4 PB1
// TIM4 : CH1 PB6 CH2 PB7 CH3 PB8 CH4 PB9
	
void F103_GTIM_PWM_PinConf (TIM_TypeDef *Timer, char Channel,uint8_t remap)
{
		MyGPIO_Struct_TypeDef MyGpio;
		// Array pin[Timer][Channel-Pin#] Const int and const Port pointers
		const int Pins[2][4][4]={{{8,9,10,11},{0,1,2,3},{6,7,0,1},{6,7,8,9}},{{9,11,13,14},{15,3,10,11},{6,7,8,9},{12,13,14,15}}};
		GPIO_TypeDef * const Ports[2][4][4] = {{{GPIOA,GPIOA,GPIOA,GPIOA},{GPIOA,GPIOA,GPIOA,GPIOA},
			{GPIOA,GPIOA,GPIOB,GPIOB},{GPIOB,GPIOB,GPIOB,GPIOB}},
			{{GPIOE,GPIOE,GPIOE,GPIOE},{GPIOA,GPIOB,GPIOB,GPIOB},
			{GPIOC,GPIOC,GPIOC,GPIOC},{GPIOD,GPIOD,GPIOD,GPIOD}}   };
	
		MyGpio.GPIO_Conf=AltOut_Ppull;
	
	// if TIM1 set MOE bit (Main Output Enable)

	if (Timer == TIM1)  {MyGpio.GPIO=Ports[remap][0][Channel]; MyGpio.GPIO_Pin=Pins[remap][0][Channel-1]; if(remap) {AFIO->MAPR |= AFIO_MAPR_TIM1_REMAP_FULLREMAP;}
											TIM1->BDTR |= TIM_BDTR_MOE;}
	if (Timer == TIM2)  {MyGpio.GPIO=Ports[remap][1][Channel]; MyGpio.GPIO_Pin=Pins[remap][1][Channel-1]; if(remap) {AFIO->MAPR |= AFIO_MAPR_TIM2_REMAP_FULLREMAP;}}
	if (Timer == TIM3)  {MyGpio.GPIO=Ports[remap][2][Channel]; MyGpio.GPIO_Pin=Pins[remap][2][Channel-1]; if(remap) {AFIO->MAPR |= AFIO_MAPR_TIM3_REMAP_FULLREMAP;}}
	if (Timer == TIM4)  {MyGpio.GPIO=Ports[remap][3][Channel]; MyGpio.GPIO_Pin=Pins[remap][3][Channel-1]; if(remap) {AFIO->MAPR |= AFIO_MAPR_TIM4_REMAP;}} 
	

	F103_GPIO_Init(&MyGpio);

}



void F103_GTIM_EIM_PinConf(TIM_TypeDef *Timer,uint8_t remap)
{
uint16_t mask=0; // Mask for clearing bits
MyGPIO_Struct_TypeDef MyGpio;
		// Array pin[remap}[Timer#][Channel#] 
		const int Pins[2][4][2]={{{8,9},{0,1},{6,7},{6,7}},{{9,11},{15,3},{6,7},{12,13}}};
		GPIO_TypeDef * const Ports[2][4][2] = {{{GPIOA,GPIOA},{GPIOA,GPIOA},
			{GPIOA,GPIOA},{GPIOB,GPIOB}},
			{{GPIOE,GPIOE},{GPIOA,GPIOB},
			{GPIOC,GPIOC},{GPIOD,GPIOD}}   };
		
			MyGpio.GPIO_Conf=In_Floating; // EIM mode : CH1 and CH2 = floating inputs
	
	// if TIM1 set MOE bit (Main Output Enable)
	
	// Set PORT and Pins for  CH1 + CH2  in floating input mode. Full remap support
	if (Timer == TIM1) {MyGpio.GPIO=Ports[remap][0][0]; MyGpio.GPIO_Pin=Pins[remap][0][0];F103_GPIO_Init(&MyGpio); 
											MyGpio.GPIO_Pin=Pins[remap][0][1];F103_GPIO_Init(&MyGpio);if(remap) {AFIO->MAPR |= AFIO_MAPR_TIM1_REMAP_FULLREMAP;};
											TIM1->BDTR |= TIM_BDTR_MOE; }
	if (Timer == TIM2) {MyGpio.GPIO=Ports[remap][1][0]; MyGpio.GPIO_Pin=Pins[remap][1][0];F103_GPIO_Init(&MyGpio); 
											MyGpio.GPIO_Pin=Pins[remap][1][1];F103_GPIO_Init(&MyGpio); if(remap) {AFIO->MAPR |= AFIO_MAPR_TIM2_REMAP_FULLREMAP;};
	if (Timer == TIM3) {MyGpio.GPIO=Ports[remap][2][0]; MyGpio.GPIO_Pin=Pins[remap][2][0];F103_GPIO_Init(&MyGpio); }
											MyGpio.GPIO_Pin=Pins[remap][2][1];F103_GPIO_Init(&MyGpio); if(remap) {AFIO->MAPR |= AFIO_MAPR_TIM3_REMAP_FULLREMAP;}; 
	if (Timer == TIM4) {MyGpio.GPIO=Ports[remap][3][0]; MyGpio.GPIO_Pin=Pins[remap][3][0];F103_GPIO_Init(&MyGpio); }
											MyGpio.GPIO_Pin=Pins[remap][3][1];F103_GPIO_Init(&MyGpio); if(remap) {AFIO->MAPR |= AFIO_MAPR_TIM4_REMAP;}; }
  
	Timer->CCMR1 =0; // CLears CCMR1									
	Timer->CCMR1 |= TIM_CCMR1_CC1S_0; // CC1 Channel mapped on TI1
	Timer->CCMR1 |= TIM_CCMR1_CC2S_0; // CC1 Channel mapped on TI2
	Timer->CCMR1 |= 0x0090; // 0x0090 :Filter @ fdts/8, N=8 samples 	 0x00F0 : 	fdts/32, N=8							
											
	
	Timer->CCER &= ~TIM_CCER_CC1P; // Non inverted capture
	Timer->CCER |= TIM_CCER_CC1E; // Capture enable
	Timer->CCER &= ~TIM_CCER_CC2P; // Non inverted capture
	Timer->CCER |= TIM_CCER_CC2E; // Capture enable
	
	mask=0xFFF7; Timer->SMCR &= ~mask; // Clear SMCR	
	Timer->SMCR |= 0x0003; // 01 : Encoder mode 1  02 : mode 2  03 : mode 3
		
											
	Timer->CR1 |= TIM_CR1_UDIS; //Disable update or counting stops at first event (TBI)


}



