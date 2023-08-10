
#include "F103_USART.h"

static char rbuffer[MAX_LEN]={0};
static uint16_t rindex=0;
static volatile uint8_t rflag=0, tflag=0;


//***************************************************************
// Init function DOES NOT enable TX or RX (set Te Re flag in CR1)
//*************************************************************************

void F103_USART_Init(USART_TypeDef *usart,uint32_t baudRate, uint8_t wordLength,uint32_t STOPBITS, uint8_t parityBits  )
{

 MyGPIO_Struct_TypeDef gpioStr={0};
 
 float bdrateDiv=468.75; //Float for BRR register baud rate
 uint16_t bdrateMan=468,bdrateFrac=12;
 
	
	// Enable USART Clock and configure Pins
	
	if (usart==USART1) {
		RCC->APB2ENR |=RCC_APB2ENR_USART1EN ; // Enable USART1 CLK
		gpioStr.GPIO =GPIOA; gpioStr.GPIO_Conf=AltOut_Ppull;gpioStr.GPIO_Pin=9;
		F103_GPIO_Init(&gpioStr); // PA9 : TX = Alt PushPull
		gpioStr.GPIO =GPIOA; gpioStr.GPIO_Conf=In_PullUp;gpioStr.GPIO_Pin=10;
		F103_GPIO_Init(&gpioStr); 			// PA10 RX In Pullup
		
	}
	
	if (usart==USART2) {
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // Enable USART2 CLK
		gpioStr.GPIO =GPIOA; gpioStr.GPIO_Conf=AltOut_Ppull;gpioStr.GPIO_Pin=2;
		F103_GPIO_Init(&gpioStr); // PA2 : TX = Alt PushPull
		gpioStr.GPIO =GPIOA; gpioStr.GPIO_Conf=In_PullUp;gpioStr.GPIO_Pin=3;
		F103_GPIO_Init(&gpioStr); 			// PA3 RX In Pullup
		
	}
	
	if (usart==USART3) {
		RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // Enable USART2 CLK
		gpioStr.GPIO =GPIOB; gpioStr.GPIO_Conf=AltOut_Ppull;gpioStr.GPIO_Pin=10;
		F103_GPIO_Init(&gpioStr); // PB10 : TX = Alt PushPull
		gpioStr.GPIO =GPIOA; gpioStr.GPIO_Conf=In_PullUp;gpioStr.GPIO_Pin=11;
		F103_GPIO_Init(&gpioStr); 			// PB11 RX In Pullup
	}
	
	// Enable USART Peripheral
	usart->CR1=0 ; // clear CR1 
	usart->CR1 |= USART_CR1_UE ; 
	
	
	//Write Mantissa and fractional part to BRR
	bdrateDiv = baudRate_Usartdiv(usart,baudRate); //Baud rate divider depends on the Peripheral clock PCLK1 or PCLK2
	bdrateMan = (uint16_t) bdrateDiv;
	bdrateFrac = (uint16_t) ((bdrateDiv - bdrateMan)*16.0);
	usart->BRR = (bdrateMan<<4)+bdrateFrac;
	
	//Word length, parity and stop bits
		
		//Set
		if (wordLength==WORD_9) usart->CR1|=USART_CR1_M;   //9 bits words
		if (parityBits==PARITY) usart->CR1 |= USART_CR1_PCE;  //Parity Control
		usart->CR2 |= (STOPBITS<<12); //Stop Bits
	
		// Software must Enable transmitter receiver
	
}


//*************************************************************************
// Get baud rate divider in float
static float baudRate_Usartdiv (USART_TypeDef *usart,uint32_t baudRate)
{
float bdratediv=0.0;
if (usart==USART1) bdratediv= (float) PCLK2 / (16*baudRate);
	else bdratediv= (float) PCLK1 / (16*baudRate);

return(bdratediv);

}


//************************************************************************
//  DMA  TX with USART
//  DMA1 clock must be enabled : DMA_CK_ENABLE
//  Usage
// 1 - once : Init USART
// 2 - once : USART CR1 : Enable Transmitter
// 3 - once : write DMA TC IT callback : Stop DMA, Clear IT flag, wait for USART TC.
// 4 - once : Initialize DMA : TX_DMA_Init
//   USART DMAT
//   USART TX enable
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
void DMA1_CH4_IT_Func (void)
{
DMA1_CH4_STOP; //Disable DMA
while ((USART1->SR & USART_SR_TC)==0) {}; // Wait TC flag
	tflag=0; // clear tflag : TX done
}

void DMA1_CH7_IT_Func (void){}
void DMA1_CH2_IT_Func (void){}

void F103_USART_TX_DMA_Init(USART_TypeDef *usart)
{
	// USART1 : TX DMA ch 4  USART 2 : ch7  USART3 Ch 2
  
	DMA_CK_ENABLE;
	usart->CR3 |= USART_CR3_DMAT; //Enable USART TX with DMA
  usart->CR1 |= USART_CR1_TE; // TX enable
	if (usart == USART1) {
		Register_DMA_TC_Callback(4,DMA1_CH4_IT_Func); //Set DMA TC IT function (post DMA)
		DMA1_CH4_STOP;
		DMA1_Channel4->CPAR = (uint32_t) (usart) + 0x04;
		DMA1_Channel4->CCR=0x00000092;// 8b->8b M+1->P TCIE EN
		NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	  }
	
	if (usart == USART2) { DMA1_CH7_STOP;
		Register_DMA_TC_Callback(7,DMA1_CH7_IT_Func); //Set DMA TC IT function (post DMA)
		DMA1_CH7_STOP;
		DMA1_Channel7->CPAR = (uint32_t) (usart) + 0x04;
		DMA1_Channel7->CCR=0x00000092;// 8b->8b M+1->P TCIE EN
		NVIC_EnableIRQ(DMA1_Channel7_IRQn);
	  }
	
	if (usart == USART3) { DMA1_CH2_STOP;
		Register_DMA_TC_Callback(2,DMA1_CH2_IT_Func); //Set DMA TC IT function (post DMA)
		DMA1_CH2_STOP;
		DMA1_Channel2->CPAR = (uint32_t) (usart) + 0x04;
		DMA1_Channel2->CCR=0x00000092;// 8b->8b M+1->P TCIE EN
		NVIC_EnableIRQ(DMA1_Channel2_IRQn);
	  }	
}

void F103_USART_TX_DMA_Send(USART_TypeDef*usart, char * TxStr)
{
	if (usart == USART1) 
		{ DMA1_CH4_STOP; // DMA can't be updated if active
		DMA1_Channel4->CNDTR=strlen(TxStr);
		DMA1_Channel4->CMAR = (uint32_t) TxStr ;
		}
	if (usart == USART2) { DMA1_CH7_STOP;
		DMA1_Channel7->CNDTR=strlen(TxStr);
		DMA1_Channel7->CMAR = (uint32_t) TxStr ;
		}
	if (usart == USART3) { DMA1_CH2_STOP;
		DMA1_Channel2->CNDTR=strlen(TxStr);
		DMA1_Channel2->CMAR = (uint32_t) TxStr ;
		}
  
	usart->SR &= ~USART_SR_TC; // As per RM
		tflag=1; // Set flag : TX pending
	if (usart == USART1) DMA1_CH4_START; 
	if (usart == USART2) DMA1_CH7_START;
	if (usart == USART3) DMA1_CH2_START;
}

//
// RX using DMA for a set number of char in circ mode
// USART 1 : CH5  USART 2 : CH6  USART3 : CH3
// 
//
void DMA1_CH5_IT_Func (void){rflag=1;}// DMA IT for USART1
void DMA1_CH6_IT_Func (void){} 				// DMA IT for USART2
void DMA1_CH3_IT_Func (void){} 				// DMA IT for USART3

void F103_USART_RX_DMA_Init(USART_TypeDef *usart){
	
	DMA_CK_ENABLE;
	usart->CR3 |= USART_CR3_DMAR; //Enable USART RX with DMA
	
	if (usart == USART1) {
		Register_DMA_TC_Callback(5,DMA1_CH5_IT_Func); //Set DMA TC IT function (post DMA)
		DMA1_CH5_STOP;
		DMA1_Channel5->CPAR = (uint32_t ) &usart->DR;
		DMA1_Channel5->CCR=0x000000A2;// 8b->8b P->M+1 CIRC TCIE EN
		NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	  }
	
	if (usart == USART2) { DMA1_CH6_STOP;
		Register_DMA_TC_Callback(6,DMA1_CH6_IT_Func); //Set DMA TC IT function (post DMA)
		DMA1_CH7_STOP;
		DMA1_Channel6->CPAR = (uint32_t ) &usart->DR;
		DMA1_Channel6->CCR=0x000000A2;// 8b->8b P->M+1 CIRC TCIE EN
		NVIC_EnableIRQ(DMA1_Channel6_IRQn);
	  }
	
	if (usart == USART3) { DMA1_CH3_STOP;
		Register_DMA_TC_Callback(3,DMA1_CH3_IT_Func); //Set DMA TC IT function (post DMA)
		DMA1_CH2_STOP;
		DMA1_Channel3->CPAR = (uint32_t ) &usart->DR;
		DMA1_Channel3->CCR=0x000000A2;// 8b->8b P->M+1 CIRC TCIE EN
		NVIC_EnableIRQ(DMA1_Channel3_IRQn);
	  }	
}
void F103_USART_RX_DMA_Start(USART_TypeDef*usart, char * RxStr, uint8_t charNum)
{
	rflag=0; // Clears flag in DMA mode
	if (charNum>MAX_LEN)charNum=MAX_LEN; //Parameter check
if (usart == USART1) 
		{ DMA1_CH5_STOP; // DMA can't be updated if active
		DMA1_Channel5->CNDTR=charNum;
		DMA1_Channel5->CMAR = (uint32_t) RxStr ;
		}
	if (usart == USART2) { DMA1_CH6_STOP;
		DMA1_Channel6->CNDTR=charNum;
		DMA1_Channel6->CMAR = (uint32_t) RxStr;
		}
	if (usart == USART3) { DMA1_CH3_STOP;
		DMA1_Channel3->CNDTR=charNum;
		DMA1_Channel3->CMAR = (uint32_t) RxStr ;
		}
  USART1->CR1 |= USART_CR1_RE; // RX enable
	if (usart == USART1) DMA1_CH5_START; 
	if (usart == USART2) DMA1_CH6_START;
	if (usart == USART3) DMA1_CH3_START;
}


// RX using INTERRUPT FUNCTIONS----------------------------------------------------------
// Non blocking 
// Stores received char in rbuffer and stops reception when EOC1 or EOC2
// Or MAX_LEN is reached. Sets flag and stops reception.
// Same IT_Function for RX and TX (chack IT Flag status)

void USART1_IT_Func(void)
{
//Check SR for RXNE flag (avoiding TXE interrupt)
if ((USART1->SR |= USART_SR_RXNE) !=0) {
	rbuffer[rindex]=USART1->DR; // RXNE IT bit is cleared after DR read
	rindex+=1;
	//Endcar case or MAX_LEN reached : adds NULL to str and sets flag
	if ( (USART1->DR == ENDCAR1)||(USART1->DR==ENDCAR2) ) {rbuffer[rindex]='\0';
		rflag+=1; rindex=0;  }	
		if ( rindex== MAX_LEN-1 ) {rbuffer[rindex]='\0';
		rflag+=1; rindex=0; }
 }
}


	
void F103_USART_ActiveIT(USART_TypeDef *usart, char prio)
{
  usart->CR1 |= USART_CR1_RXNEIE; // Enable IT on RX
	
	
	if (usart== USART1) { NVIC_SetPriority(USART1_IRQn,(uint32_t)prio);
												NVIC_EnableIRQ(USART1_IRQn);Register_USART_Callback(usart,USART1_IT_Func); }
	if (usart== USART2) { NVIC_SetPriority(USART2_IRQn,(uint32_t)prio);
												NVIC_EnableIRQ(USART2_IRQn); }
  if (usart== USART3) { NVIC_SetPriority(USART3_IRQn,(uint32_t)prio);
												NVIC_EnableIRQ(USART3_IRQn); }
	
}

uint8_t Is_RX_Str_Rdy (void)
{
return(rflag);
}

char * Get_RX_Str(void)
{
	rflag=0;
	return rbuffer;

}
	
uint8_t Is_Tx_Pending(void)
{
	return (tflag);
}
