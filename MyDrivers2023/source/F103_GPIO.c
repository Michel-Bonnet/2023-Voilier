

#include "F103_GPIO.h"
#include "F103_IT.h"

// IT function pointers declaration
//void (* EXTI0_IT_Ptr) (void) = 0; 

//---------------MyGPIO_Init----------------
void F103_GPIO_Init(MyGPIO_Struct_TypeDef *GPIOStructPtr)
{
uint8_t numport=0;
uint32_t mask= 0xF, conf=0; 																	//Effacer 2 bits conf + 2 bits Mode

uint32_t rank=((uint16_t) GPIOStructPtr->GPIO_Pin )%8; // numero de pin mudulo 8
		
	
	//**** CLOCKING  *****
	//Clock enable Ports A..E et Alternate si besoin
	numport = F103_GPIO_PortNum(GPIOStructPtr->GPIO);
	switch(numport)
	{
		case 0 :
			RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;break;
		case 1 :
			RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;break;
		case 2 :
			RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;break;
		case 3 :
			RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;break;
		case 4 :
			RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;break;
		default : ;
		
	}
		// Enable AFIO Clock if Alternate function
		if((GPIOStructPtr->GPIO_Conf == AltOut_Ppull)||(GPIOStructPtr->GPIO_Conf == AltOut_OpenDrain))RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	//***** PIN CONFIG  ******
	
	// Output :Mod=10 (10MHz) Mode 11 (50MHz)
	
	switch(GPIOStructPtr->GPIO_Conf)
	{
		case In_Floating : conf = 0x4; 			break; 	// CNF 01 MOD 00		
		case In_PullDown : conf = 0x8;		 	break; 	// CNF 10 MOD 00 (ODR=0)		
		case In_PullUp : conf = 0x8; 				break;	// CNF 10 MOD 00  (ODR=1)		
		case In_Analog : conf = 0x0;				break; 	// CNF 00 MOD 00		
		case Out_Ppull : conf = 0x3;				break; 	// CNF 00 MOD 11		
		case Out_OpenDrain : conf = 0x7;		break;  // CNF 01 MOD 11		
		case AltOut_Ppull : conf = 0xB;			break; 	// CNF 10 MOD 11		
		case AltOut_OpenDrain : conf = 0xF;	break; 	//CNF 11  MOD 11
		default : ;
		
	}
	
	
	//CRL Register if pin<8
	if(GPIOStructPtr->GPIO_Pin<=7){																			// Si pin de 0 à 7
	GPIOStructPtr->GPIO->CRL &= (~(mask<<(rank*4)));    // Efface 4 bits emplacement CNF/MODE 
	GPIOStructPtr->GPIO->CRL |= (conf<<(rank*4))  ;  // Recopie 4 bits de GPIO_Conf
	}
	//CRH Register if pin>7
	if(GPIOStructPtr->GPIO_Pin>=8){
	GPIOStructPtr->GPIO->CRH &= ~(mask<<(rank*4));
	GPIOStructPtr->GPIO->CRH |= (conf)<<(rank*4)  ;
	}
	
	// Registre ODR traitement In_PullUp ou In_Pulldown
		
			if (GPIOStructPtr->GPIO_Conf == In_PullDown) GPIOStructPtr->GPIO->ODR &= ~(1<<((uint8_t) GPIOStructPtr->GPIO_Pin)); //Mettre à 0 ODR pin
			if (GPIOStructPtr->GPIO_Conf == In_PullUp)GPIOStructPtr->GPIO->ODR |= (1<<(GPIOStructPtr->GPIO_Pin)); //Mettre à 1 ODR PIN
}


uint8_t  F103_GPIO_PortNum(GPIO_TypeDef * GPIO)
	{
		uint8_t portNum=0;
		if (GPIO==GPIOA) portNum=0;
		if (GPIO==GPIOB) portNum=1;
		if (GPIO==GPIOC) portNum=2;
		if (GPIO==GPIOD) portNum=3;
		if (GPIO==GPIOE) portNum=4;
		
		return(portNum);
	}

	//------------------------------ GPIOPin0 IT
	
	void F103_GPIO_Pin0_IT_Conf(GPIO_TypeDef * Gpio,uint8_t priority,void (* IT_Function) (void))

{
	//Step 1 : initialise GPIO pin s input.
	MyGPIO_Struct_TypeDef MyGPIO;
	MyGPIO.GPIO=Gpio;
	MyGPIO.GPIO_Pin=0;
	MyGPIO.GPIO_Conf=In_PullDown;
	F103_GPIO_Init(&MyGPIO);
	//Enable AFIO clock
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	//Step 2 : Configure mask bits
	EXTI->IMR |= EXTI_IMR_MR0 ; // Unmask EXTI line 0
	EXTI->RTSR |= EXTI_RTSR_TR0 ; // Enable rising trigger interrupt
	//Step 3 : assign source for EXTI0
	AFIO->EXTICR[0] |= F103_GPIO_PortNum(Gpio);
	//Step 3bis : assign pointer to IT function (IT function is defined in main or application layer)
	Register_EXTI_Callback (0, IT_Function);
	//Step 4 : NVIC : priority and enable
	NVIC_SetPriority(EXTI0_IRQn,(uint32_t)priority);
	NVIC_EnableIRQ(EXTI0_IRQn);
	
	
	

}
	
 //--------------------- A TESTER GPIORead
	
int F103_GPIO_Read(GPIO_TypeDef * GPIO, char GPIO_Pin)
{
	int value=0;
	value = (GPIO->IDR >> GPIO_Pin) & 0x01; // Returns 0 or 1. 
	return (value);
}

void F103_GPIO_Set(GPIO_TypeDef * GPIO, char GPIO_Pin)
{
GPIO->BSRR |= ((uint32_t) 1<<GPIO_Pin);
}

void F103_GPIO_Reset(GPIO_TypeDef * GPIO, char GPIO_Pin)
{
GPIO->BSRR |= ((uint32_t) 1<<(GPIO_Pin+16));
}

void F103_GPIO_Toggle(GPIO_TypeDef * GPIO, char GPIO_Pin)
{
GPIO->ODR ^= ((uint32_t) 1<<GPIO_Pin);
}


