  

#include "Plateau.h"

char value_RX;

void PLA_Init(void)
{
MyGPIO_Struct_TypeDef gpioStr={0};
	
F103_USART_Init(RX_USART,9600,WORD_8,STOP_BITS_1,NO_PARITY);
F103_USART_RX_DMA_Init(RX_USART);
F103_GTIM_Base_Init (PLA_PWM_TIM, 720-1, 1-1); //100kHz : PSC 1, ARR 720
F103_GTIM_PWM(PLA_PWM_TIM,3,0,MODE_1); // Duty cycle = 0% 
F103_GTIM_PWM_PinConf(PLA_PWM_TIM,PLA_PWM_CH,NO_REMAP);
gpioStr.GPIO = PLA_DIR_PORT;
gpioStr.GPIO_Conf = Out_Ppull;
gpioStr.GPIO_Pin=PLA_DIR_PIN;
F103_GPIO_Init(&gpioStr);

}



void PLA_Start(void)
{

F103_USART_RX_DMA_Start(RX_USART,&value_RX,1);	// 1 char to RX-> &consigne
F103_GTIM_Base_Start(PLA_PWM_TIM);
F103_GPIO_Reset(PLA_DIR_PORT,PLA_DIR_PIN);

}

void PLA_Setpoint(void)
{
int Kduty; signed char curr_value = (signed char) value_RX; // record value_RX as it may change anytime
	
  if (curr_value<-PLA_DEAD_ZONE) F103_GPIO_Set(PLA_DIR_PORT,PLA_DIR_PIN);
	if (curr_value>PLA_DEAD_ZONE) F103_GPIO_Reset(PLA_DIR_PORT,PLA_DIR_PIN);
	
	if (abs(curr_value)<=PLA_DEAD_ZONE) Kduty=0;
	if (abs(curr_value)>PLA_DEAD_ZONE) {
		curr_value=abs(curr_value);
		Kduty=1000* (curr_value-PLA_DEAD_ZONE)/(100-PLA_DEAD_ZONE);
	
	}
	
	
	F103_GTIM_PWM_Update(PLA_PWM_TIM,PLA_PWM_CH,Kduty ); 



}


