


#include "F103_RCC.h"




void RCC_Init (void)
{
	// HSE and Clock security
	RCC->CR |= (RCC_CR_HSEBYP | RCC_CR_HSEON); // Bypass HSE and enable Ext source at 0SCIN
	while((RCC->CR & RCC_CR_HSERDY)==0); 					//Wait until RCC_CR_HSERDY bit is 1 = stable HSE
	RCC->CR |= RCC_CR_CSSON;										// Enable Clock security
	
	// BDCR write : enable writing first see RTC in RM008
	RCC->APB1ENR |= RCC_APB1ENR_PWREN; //Enable power Interface
	RCC->APB1ENR |= RCC_APB1ENR_BKPEN; //Enable backup Interface
	PWR->CR |= PWR_CR_DBP;							//Enable backup domain access
	//BDCR Writes
	RCC->BDCR = 0x00 ; 									//RESET value BDCR 
	RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;		//RTC is LSE	//External 32k
	RCC->BDCR |= RCC_BDCR_LSEON;			//LSE is RTC Clock
	while((RCC->BDCR & RCC_BDCR_LSERDY)==0); 		//LSE ready Wait
	
	
	// PLL Setting
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSE;  //Source pour SYSCLK = HSE
	RCC->CFGR |= RCC_CFGR_PLLMULL9;
	RCC->CR |= RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY)==0);
	
	// Prescaler settings
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6 ;
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1 ;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2 ;
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	
	

}

