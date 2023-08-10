//-------------------------------------------------------------
//
// USART driver
// TX : DMA
// RX : IT. if 1 endcar : stops on EITHER endcar1 or Endcar2
//
//-------------------------------------------------------------


#ifndef F103_USART_H
#define F103_USART_H

#include "stm32f10x.h"
#include "F103_GPIO.h"
#include "F103_RCC.h"
#include "F103_IT.h"
#include "F103_DMA.h"
#include <string.h>

#define STOP_BITS_1 0x0
#define STOP_BITS_0_5 0x1
#define STOP_BITS_2 0x2
#define STOP_BITS_1_5 0x3
#define WORD_9 1  //Wordlength
#define WORD_8 0  //Wordlength
#define PARITY 1  //Parity Bits
#define NO_PARITY 0

#define MAX_LEN 50 // Max size of RX string
#define NUM_ENDCAR 1 // 1 or 2 End car 

#define ENDCAR1  0x0D
#define ENDCAR2  0x0A


// Exported Macros



//-------------------------------------------------------------------------
//
// Configure USART
// @ * usart :  USART address
// @ baudrate : baud in symbols/s
// @ wordlength :either 8 or 9 : WORD_8 or WORD_9
// @ stopBits : 0.5, 1, 1.5 or 2 (STOP_BITS_0_5 etc...)
// @ parity : either PARITY or NO_PARITY
// Does NOT enable Tx or RX (TE or RE flag): IT must be configured before
// Exemple My_USART_Init(USART2,9600,WORD_8,STOP_BITS_1,NO_PARITY);
//
//-------------------------------------------------------------------------

void F103_USART_Init(USART_TypeDef *usart,uint32_t baudRate, uint8_t wordLength,uint32_t stopBits, uint8_t parityBits  );
static float baudRate_Usartdiv (USART_TypeDef *usart,uint32_t baudRate); //Computes the USARTDIV needed in USART_BRR register



//************************************************************************
//  DMA  TX with USART
//  DMA1 clock must be enabled : DMA_CK_ENABLE
//  Usage
// 1 - once : Init USART
// 2 - once : USART CR1 : Enable Transmitter
// 3 - once : write DMA TC IT callback : Stop DMA, Clear IT flag, wait for USART TC.
// 4 - once : Initialize DMA : TX_DMA_Init
//   USART DMAT
//   Registers DMA TC callback
//   CPAR4 : 0x40013804 for USART1->DR 
//   Dir : M to P  
//   Msize 8bit Psize 8bit TCIE 
//   CCR 0x0092
//   Enable NVIC for DMAch4 IRQ
// 		CCR bit fields : b15-b31 : reserved 
// 		MEM2MEM / PL(1:0) // MSIZE (1:0) / PSIZE (1:0) // MINC / PINC / CIRC / DIR // 
//    TEIE / HTIE / TCIE / EN
//  5 : each string : TX_DMA_Send.
//   Sets CMAR (buffer address 0x200xxxxx)
//   Sets CNDTR
//   Clears TC flag in USART SR and enables DMA
// 
//************************************************************************
void DMA1_CH4_IT_Func (void); // DMA IT for USART1
void DMA1_CH7_IT_Func (void); // DMA IT for USART2
void DMA1_CH2_IT_Func (void); // DMA IT for USART3

void F103_USART_TX_DMA_Init(USART_TypeDef*usart);
void F103_USART_TX_DMA_Send(USART_TypeDef*usart, char * TxStr);

// RX with DMA for set number of char in Circ mode
// USART 1 : CH5  USART 2 : CH6  USART3 : CH3
void DMA1_CH5_IT_Func (void); // DMA IT for USART1
void DMA1_CH6_IT_Func (void); // DMA IT for USART2
void DMA1_CH3_IT_Func (void); // DMA IT for USART3



void F103_USART_RX_DMA_Init(USART_TypeDef *usart);
void F103_USART_RX_DMA_Start(USART_TypeDef*usart, char * RxStr, uint8_t charNum);


//************************************************************************
// 2023 IT RX receive string functions. String = array of char that ends
// with ENDCAR1 or ENDCAR2
// Rx_flag : +1 if 0x0D/0x0A is found in IT routine. 
// Limit to MAX_LEN
// final char is \0
// Usage : Active IT (set IT function is written in  USART.c)
// Check Rx flag (completed string) with Is_RX_Str_Rdy
// Get received string buffer address with Get_RX_Str. Clears the RX_flag.
// User MUST re-enable RE flag in CR1.
//************************************************************************

void F103_USART_ActiveIT(USART_TypeDef *usart, char prio);


// GETTERS / SETTERS
char * Get_RX_Str(void); // Returns pointer to received char
uint8_t Is_Tx_Pending (void); // 1 if still busy (USART TC =0) , 0 is free
uint8_t Is_RX_Str_Rdy (void); //  1 if ready (EOC1 EOC2 detected)


#endif

