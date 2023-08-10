#include "E6B2.h"

static volatile int zero_cnt=0;

void E6B2_IT (void)
{

E6B2_TIM->CNT = 1*720;	
zero_cnt++;
	
}

void E6B2_Init(void) {
// Peripheral : Timer config
	
	
	F103_GTIM_Base_Init(E6B2_TIM,2*720-1, 1-1);
	E6B2_TIM->CNT =1*720;
	F103_GTIM_EIM_PinConf( E6B2_TIM,NO_REMAP);

}

void E6B2_Start(void) {
MyGPIO_Struct_TypeDef myGpio;
	myGpio.GPIO=E6B2_IT_PORT;
	myGpio.GPIO_Pin=E6B2_IT_PIN;
	myGpio.GPIO_Conf=In_Floating;
F103_IT_EXTI_Init(&myGpio,E6B2_IT_PRIO,E6B2_IT);
F103_GTIM_Base_Start( E6B2_TIM);
}

int E6B2_Get_Angle(void)
{
	int angle = ((E6B2_TIM->CNT)-720)/4 ;
return (angle);
}
int E6B2_Get_Zcount(void)
{
return(zero_cnt);

}


