#ifndef MY_SPI_H
#define MY_SPI_H
#endif

#include "stm32f10x.h"
#include "F103_GPIO.h"
#include "F103_IT.h"
#include "F103_DMA.h"

#define SSMOD_HLLH 0  // SS  set to H -> Low and L to H  this mode is by default
#define SSMOD_HLLL 1  // SS set to H -> L and not deasserted. Must be folloed by SSMOD_XXXH
#define SSMOD_XXXH 2  // SS previous state and set to H. Use with SSMOD_HLLL
#define NO_REMAP 0
#define REMAP 1

// Exported MAcros


// My_Spi_Init()
// GPIO INIT (SS disabled) remap<>0 for SPI1  baudRateDiv = BR register value 
// SS is GP ppull Output
// Master mode , Full duplex
// LSBFirst 8 bits
// Master mode 4 Wires implemented 
// NSS pin disabled : software control SSM=1 SSI=1
//
// SPI1 SS: A4 Out_PPul  SCK : A5 AltOutPPul  MISO : A6 In_float 
//      MOSI : A7 AltOutPPull
// SPI2 SS: B12 Out_PPul  SCK : B13 AltOutPPul  MISO : B14 In_float 
//      MOSI : B15 AltOutPPull
//
// *** Remap SPI1_REMAP=1
// SPI1 SS: A15 Out_PPul  SCK : B3 AltOutPPul  MISO : B4 In_float 
//      MOSI : B5 AltOutPPull
// PCLK1=36MHz SPI2  PCLK2=72MHz = SPI1
// bRateDiv : 00=2, 01=4, 8,16,32,64,128,256.  
// SPI1 bRateDiv=5 is 72MHz/64 = 1.12 MHz
void F103_SPI_Init(SPI_TypeDef *Spi, uint8_t remap, int bRateDiv,uint8_t cpol,uint8_t calpha); // Remap <> 0 : SPI1 Remap



// My_Spi_RW_Array
// Sends Tx_Array and reads Rx_Array
// Length = # of data to send
// SSMode defines above (
// SS Mode controls the assert/deassert of SS pin


void F103_SPI_RW_Array(SPI_TypeDef *Spi, uint16_t  *Tx_Array, uint16_t *Rx_Array,uint8_t length, uint8_t SSMode);

