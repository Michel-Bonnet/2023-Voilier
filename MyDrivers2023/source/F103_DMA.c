#include "F103_DMA.h"
/*
*
*  DMA Driver for F103. DMA CLK must be ENABLED with DMA_CK_ENABLE MACRO
*  DMA Channel steps
*  1 - Set peripheral register in DMA_CPAR
*  2 - Set memory address in DMA_CMAR
*  3 - Set # of data to tranfer in DMA_CNDTR
*  4 - Set Channel Priority in DMA_CCR
*  5 - Set data direction, mode, inc mode, data size and IT settings in DMA_CCR
*  6 - Activate channel in DMA_CCR
*/

/*
* F103_DMA_Channel_Conf : See RM for CCR bits. Does NOT enable DMA Channel
*/
void F103_DMA_Channel_Conf(DMA_Channel_TypeDef * DMAChannel, uint32_t cmar, uint32_t cpar,uint32_t ccr,uint32_t cndtr )
{
uint32_t mask=0;

	
mask=0x00007FFF; DMAChannel->CCR &= ~mask;// Clear CCR B0-B14
mask=0x0000FFFF; DMAChannel->CNDTR &= ~mask; // Clear CNDTR B0-B15
	
 DMAChannel->CPAR = cpar;
 DMAChannel->CMAR = cmar;
 DMAChannel->CCR |= ccr;
 DMAChannel->CNDTR |= cndtr;


}
