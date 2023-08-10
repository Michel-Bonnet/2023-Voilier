
#ifndef BORDAGE_H
#define BORDAGE_H


#include "F103_GTIM.h"
#include "E6B2.h"

#define BOR_PWM_TIM (TIM1)  //PWM TIMER 
#define BOR_PWM_CH  4		    //Channel : TIM1 CH4 = A11
#define SERVO_LOW_DUR 500   //SERVO Low DutyCycle duration µs
#define SERVO_HIGH_DUR 2500 //SERVO High DutyCycle Duration


/*
*  Initialize GTIM1 CH4  Pin A11 :  PWM servo bordage
*  Check defines for PWM conf
*  Initialize Encoder : check E6B2 defines
*
*  Usage : Init();
*          Start();
*          Wait_Zero();
*  
*/
void BOR_Init(void);
void BOR_Start(void);
void BOR_Wait_Zero(void);

// Changes PWM duration (1000us-2000us)
// 5% DCy -> 1ms = 1000 us
// 10% DCy -> 2ms = 2000 us
// DCy = (us)/20000
void BOR_PWM_US(int usec);

// BOR Setpoint
// Reads angle phi
// Sends PWM (check SERVO defines)
// Law : phi <45° pwm 1ms
//       phi> 45° linear 1ms->2ms
//     45° 1ms    180° : 2ms
void BOR_Setpoint(void);
int BOR_Angle(void);












#endif
