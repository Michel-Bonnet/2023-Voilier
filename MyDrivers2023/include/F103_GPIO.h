#ifndef F103_GPIO_H
#define F103_GPIO_H
#include "stm32f10x.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct
{ GPIO_TypeDef *GPIO;
	char GPIO_Pin;
	char GPIO_Conf;
}MyGPIO_Struct_TypeDef;


#define In_Floating 			0x1
#define In_PullDown 			0x2 		
#define In_PullUp   			0x3		
#define In_Analog   			0x4
#define Out_Ppull 				0x5  //ODR initial state to be set/reset
#define Out_OpenDrain 		0x6  //ODR initial state to be set/reset
#define AltOut_Ppull 			0x7
#define AltOut_OpenDrain 	0x8


void F103_GPIO_Init(MyGPIO_Struct_TypeDef *GPIOStructPtr);
int F103_GPIO_Read(GPIO_TypeDef * GPIO, char GPIO_Pin);
void F103_GPIO_Set(GPIO_TypeDef * GPIO, char GPIO_Pin);
void F103_GPIO_Reset(GPIO_TypeDef * GPIO, char GPIO_Pin);
void F103_GPIO_Toggle(GPIO_TypeDef * GPIO, char GPIO_Pin);
void F103_GPIO_Pin0_IT_Conf(GPIO_TypeDef * GPIO,uint8_t priority, void (* IT_Function) (void));
uint8_t  F103_GPIO_PortNum(GPIO_TypeDef * GPIO);   // Returns port number : A=0 B=1 ... E=4


#endif

