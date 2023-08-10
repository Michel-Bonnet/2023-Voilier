#ifndef ADXL345_H
#define ADXL345_H



#include "stm32f10x.h"
#include "F103_SPI.h"

//Hardware configuration
#define ADXL345_SPI SPI2
#define ADXL345_SPI_BRC 5 // Baud Rate Control, 5 -> 1.12 MHz

#define M_PI 3.1415926535 
//************** ADXL345  Registers defines

#define DEVID		 	0x00
#define OFSX		 	0x1E
#define OFSY 			0x1F
#define OFSZ 			0x20
#define BW_RATE 	0x2C
#define POWER_CTL 0x2D
#define DATAX0		0x32
#define DATAX1 		0x33
#define DATAY0 		0x34
#define DATAY1 		0x35
#define DATAZ0 		0x36
#define DATAZ1 		0x37
#define DATA_FORMAT 0x31

// PCLK1=36MHz SPI2  PCLK2=72MHz = SPI1
// bRateDiv : 00=2, 01=4, 8,16,32,64,128,256.  
// SPI1 bRateDiv=5 is 72MHz/64 = 1.12 MHz
// CPOL =1  CALPHA=1

// Application level functions

int ADXL345_Get_RollAngle(void);
void ADXL345_Get_Axl(int16_t *ax, int16_t *ay , int16_t *az);

// Driver oriented function
void ADXL345_SPI_Init_Start(void);

void ADXL345_NRead(uint8_t reg, uint16_t *data,  uint8_t nBytes);

void ADXL345_NWRite(uint8_t reg, uint16_t *data,  uint8_t nBytes);

#endif
