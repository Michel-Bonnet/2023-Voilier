#include "Bordage.h"


/*
*  Initialize GTIM1 CH4  Pin A11 :  PWM servo bordage
*  Check defines for PWM conf
*  Initialize Encoder : check E6B2 defines
*
*  Usage : Init_BOR();
*  
*/
void BOR_Init(void)
{

F103_GTIM_Base_Init (BOR_PWM_TIM, 32000, 45-1); //50Hz : PSC 45, ARR 32000
F103_GTIM_PWM(BOR_PWM_TIM,4,50,MODE_1); // Duty cycle = 5% (1ms/20ms)
F103_GTIM_PWM_PinConf(BOR_PWM_TIM,BOR_PWM_CH,NO_REMAP);
E6B2_Init();

}

void BOR_Start(void)
{
E6B2_Start();
F103_GTIM_Base_Start(BOR_PWM_TIM);
	
}

void BOR_Wait_Zero(void)
{
while(E6B2_Get_Zcount()==0);
}

// Changes PWM duration (1000us-2000us)
// 5% DCy -> 1ms = 1000 us
// 10% DCy -> 2ms = 2000 us
// DCy = (us)/20000
void BOR_PWM_US(int usec)
{

F103_GTIM_PWM_Update(BOR_PWM_TIM,BOR_PWM_CH, usec/20); 

}

// BOR Setpoint
// Reads angle phi
// Sends PWM
// Law : phi <45° pwm 1ms
//       phi> 45° linear 1ms->2ms
//     45° 1ms    180° : 2ms
void BOR_Setpoint(void){

int pwmDur,phi=abs(E6B2_Get_Angle());

if (phi<=45) pwmDur=SERVO_LOW_DUR;
if (phi>45) pwmDur=SERVO_LOW_DUR + ((phi-45)*(SERVO_HIGH_DUR-SERVO_LOW_DUR))/(180-45);	

BOR_PWM_US(pwmDur);


}

int BOR_Angle(void)
{
return(E6B2_Get_Angle());
}




