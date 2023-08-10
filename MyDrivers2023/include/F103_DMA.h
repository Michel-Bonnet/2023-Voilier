#ifndef F103_DMA_H
#define F103_DMA_H
#include "stm32f10x.h"
/*
*
*  DMA Driver for F103
*  DMA Channel steps
*  1 - Set peripheral register in DMA_CPAR
*  2 - Set memory address in DMA_CMAR
*  3 - Set # of data to tranfer in DMA_CNDTR
*  4 - Set Channel Priority in DMA_CCR
*  5 - Set data direction, mode, inc mode, data size and IT settings in DMA_CCR
*  6 - Activate channel in DMA_CCR
*
*  CCR bit fields : b15-b31 : reserved 
*  MEM2MEM / PL(1:0) // MSIZE (1:0) / PSIZE (1:0) // MINC / PINC / CIRC / DIR // TEIE / HTIE / TCIE / EN
*/
// Exported Macros

#define DMA_CK_ENABLE (RCC->AHBENR |= RCC_AHBENR_DMA1EN)
#define DMA1_CH1_START (DMA1_Channel1->CCR|=DMA_CCR1_EN)
#define DMA1_CH2_START (DMA1_Channel2->CCR|=DMA_CCR1_EN)
#define DMA1_CH3_START (DMA1_Channel3->CCR|=DMA_CCR1_EN)
#define DMA1_CH4_START (DMA1_Channel4->CCR|=DMA_CCR1_EN)
#define DMA1_CH5_START (DMA1_Channel5->CCR|=DMA_CCR1_EN)
#define DMA1_CH6_START (DMA1_Channel6->CCR|=DMA_CCR1_EN)
#define DMA1_CH7_START (DMA1_Channel7->CCR|=DMA_CCR1_EN)


#define DMA1_CH1_STOP (DMA1_Channel1->CCR&= ~DMA_CCR1_EN)
#define DMA1_CH2_STOP (DMA1_Channel2->CCR&= ~DMA_CCR1_EN)
#define DMA1_CH3_STOP (DMA1_Channel3->CCR&= ~DMA_CCR1_EN)
#define DMA1_CH4_STOP (DMA1_Channel4->CCR&= ~DMA_CCR1_EN)
#define DMA1_CH5_STOP (DMA1_Channel5->CCR&= ~DMA_CCR1_EN)
#define DMA1_CH6_STOP (DMA1_Channel6->CCR&= ~DMA_CCR1_EN)
#define DMA1_CH7_STOP (DMA1_Channel7->CCR&= ~DMA_CCR1_EN)

void F103_DMA_Channel_Conf(DMA_Channel_TypeDef * DMAChannel, uint32_t cmar, uint32_t cpar,uint32_t ccr,uint32_t cndtr );
	





















#endif
