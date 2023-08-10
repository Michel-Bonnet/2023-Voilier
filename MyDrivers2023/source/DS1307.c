#include "DS1307.h"
#include <string.h>

void DS1307_I2C_Init(void)
{
F103_I2C_Init(DS1307_I2C,NO_REMAP,STD_SPEED); //Only 100kHz
}

void DS1307_Set_Date_Time_Start(DateStamp_Typedef * dateStamp)
{
uint16_t reg[8];
	reg[0]=0; // Start reg address to write
	reg[1]=  (dateStamp->sec)%10 + ((dateStamp->sec/10)<<4);
	reg[2]=  (dateStamp->min)%10 + ((dateStamp->min/10)<<4) ;
	reg[3]=  (dateStamp->hour)%10 + ((dateStamp->hour/10)<<4) ;
	reg[4]=  dateStamp->weekday;
	reg[5]=  (dateStamp->day)%10 + ((dateStamp->day/10)<<4) ;
	reg[6]=  (dateStamp->month)%10 + ((dateStamp->month/10)<<4) ;
  reg[7]=  (dateStamp->year2d)%10 + ((dateStamp->year2d/10)<<4) ;

 F103_I2C_MWrite(DS1307_I2C,DS1307_ADD,reg,8,STOP);
}

void DS1307_Read_Date(DateStamp_Typedef * dateStamp)
{
	uint16_t readData[8]={0}, cmd[2]={0};
F103_I2C_MWrite(DS1307_I2C,DS1307_ADD,cmd,1,NO_STOP); // Sets register pointer to 0
F103_I2C_MRead(DS1307_I2C,DS1307_ADD,readData,7);
dateStamp->sec = (readData[0]&0x000F)+10*(readData[0]>>4);
dateStamp->min = (readData[1]&0x000F)+10*(readData[1]>>4);
dateStamp->hour = (readData[2]&0x000F)+10*(readData[2]>>4);
dateStamp->weekday=readData[3];
dateStamp->day = (readData[4]&0x000F)+10*(readData[4]>>4);
dateStamp->month = (readData[5]&0x000F)+10*(readData[5]>>4);
dateStamp->year2d = (readData[6]&0x000F)+10*(readData[6]>>4);
}


void DS1307_Set_Control_Reg(uint8_t command)
{
uint16_t reg[2];
	command &= 0x93; //parameter check : only b7 b4 b1 and b0 may be logic 1
	reg[0]=0x07; // Command register index
	reg[1]= command;
	F103_I2C_MWrite(DS1307_I2C,DS1307_ADD,reg,2,STOP);
}
	
void DS1307_Write_Ram(uint8_t startRamAddress,uint8_t dataNum, uint16_t * dataStr)
{
uint16_t *wrData;
	wrData = (uint16_t *) malloc((dataNum+1)*sizeof(uint16_t));
	memcpy(wrData+1,dataStr,2*dataNum); // last memcpy param is number of *Bytes*
	*wrData = startRamAddress;	// First element of array is register address
	F103_I2C_MWrite(DS1307_I2C,DS1307_ADD,wrData,dataNum+1,STOP);
free(wrData);
}

void DS1307_Read_Ram(uint8_t startRamAddress,uint8_t dataNum, uint16_t * dataStr)
{
uint16_t reg = startRamAddress;

F103_I2C_MWrite(DS1307_I2C,DS1307_ADD,&reg,1,NO_STOP); // Sets register pointer to start Add
F103_I2C_MRead(DS1307_I2C,DS1307_ADD,dataStr,dataNum);
}

