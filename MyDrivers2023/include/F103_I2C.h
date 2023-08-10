
#ifndef MY_I2C_H
#define MY_I2C_H


#include "stm32f10x.h"
#include "F103_GPIO.h"
#include "F103_DMA.h"
#include "F103_IT.h"

//-------------------------------------------------------------
// Defines
// Remap / No Remap : I2C port remap
// STD_SPEED 100kHz FATS_SPEED 400kHz
// STOP/NO_STOP : select NO_STOP to initiate a
// restart condition after MWrite
//-------------------------------------------------------------
#define NO_REMAP 0
#define REMAP 1
#define STD_SPEED 0
#define FAST_SPEED 1
#define NO_STOP 0
#define STOP 1

//-------------------------------------------------------------
// My_I2C_Init
// Initialise I2C Port
// Busy flag lock hardware glitch fix 
// Pin I2C1 PB6 SCL PB7 SDA  All pins AltOpen Drain
// I2C1 Remap PB8 SCL PB9 SDA 
// APB1 36 MHz
// Verify that Pullup resistors are connected to SDA/SCL
//-------------------------------------------------------------

void F103_I2C_Init(I2C_TypeDef *i2c,uint8_t remap, uint8_t SpeedMode);

//-------------------------------------------------------------
// My_I2C_MRead : I2C master read
// 
// @ slaveADD : right-aligned (7 bits, MSB always clear)
// @ readArray : pointer to uint16 data array
// nBytes : number of bytes to be read
//
//-------------------------------------------------------------


void F103_I2C_MRead(I2C_TypeDef *i2c,uint8_t slaveAdd, uint16_t *readArray, uint16_t nBytes);

//-------------------------------------------------------------
// My_I2C_MWrite : I2C master write
// 
// @ slaveADD : right-aligned (7 bits, MSB always clear)
// @ writeArray : pointer to uint16 data array to be written
//-------------------------------------------------------------

void F103_I2C_MWrite(I2C_TypeDef *i2c,uint8_t slaveAdd, uint16_t *writeArray, uint16_t nBytes, uint8_t Stop);
void F103_I2C_MWrite8b(I2C_TypeDef *i2c,uint8_t slaveAdd, uint8_t *writeArray, uint16_t nBytes, uint8_t Stop);

//-------------------------------------------------------------
// I2C with DMA
// I2C1 TX : CH6  RX : CH7
// I2C2 TX : CH4  RX : CH5
//-------------------------------------------------------------

void F103_I2C_TX_DMA_Init (I2C_TypeDef *i2c);
void F103_I2C_MWrite8b_DMA (I2C_TypeDef *i2c,uint8_t slaveAdd, uint8_t *writeArray, uint16_t nBytes);
void DMA1_I2C_CH4_IT(void);
void DMA1_I2C_CH6_IT(void);
//-------------------------------------------------------------
// My_I2C_IsBusy
// Checks Busy flag
// blocking if flag never cleared
// Waits until I2C Bus is available
//-------------------------------------------------------------
void F103_I2C_IsBusy(I2C_TypeDef *i2c);






#endif
