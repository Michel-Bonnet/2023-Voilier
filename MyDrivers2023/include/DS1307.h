
#ifndef DS1307_H
#define DS1307_H

#include "stm32f10x.h"
#include "F103_GPIO.h"
#include "F103_I2C.h"

#define DS1307_I2C I2C2
#define DS1307_ADD 0x68

typedef struct  {
uint8_t sec;
uint8_t min;
uint8_t hour;
uint8_t weekday;
uint8_t day;
uint8_t month;
uint8_t year2d;	
}DateStamp_Typedef;

/*
*  Usage
*  1- choose I2C and address in defines
*  2 - I2C Init. This configures pins and speed (Std)
*  3 - use Set_Date_Time_Start. This enables the clock.
*  4 - retrieve timestamp data with Read_Date. Duration : 1ms
* code exemple
*   DateStamp_Typedef dateStamp={0};
*   dateStamp.day=2; dateStamp.month=7;dateStamp.year2d=23;dateStamp.weekday=7;
*   dateStamp.hour=15;dateStamp.min=01; dateStamp.sec=45;
*   DS1307_I2C_Init();
*   DS1307_Set_Date_Time_Start(&dateStamp);
*   DS1307_Read_Date(&dateStamp);
*/
void DS1307_I2C_Init(void);
void DS1307_Set_Date_Time_Start(DateStamp_Typedef * dateStamp);
void DS1307_Set_Control_Reg(uint8_t command);
void DS1307_Write_Ram(uint8_t startRamAddress,uint8_t dataNum, uint16_t * dataStr);
void DS1307_Read_Ram(uint8_t startRamAddress,uint8_t dataNum, uint16_t * dataStr);
void DS1307_Read_Date(DateStamp_Typedef * dateStamp);



#endif
