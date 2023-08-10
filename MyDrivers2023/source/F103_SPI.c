
#include "F103_SPI.h"

//----------------------------------------------------------
//
// My Spi Init : configure SPI : GPIO, Clock,
// Master mode 4 Wires implemented LSBFirst 8 bit
// NSS pin disabled : software control SSOE=0
// SPI1 SS: A4 Out_PPul  SCK : A5 AltOutPPul  MISO : A6 In_float 
//      MOSI : A7 AltOutPPul
// SPI2 SS: B12 Out_PPul  SCK : B13 AltOutPPul  MISO : B14 In_float 
//      MOSI : B15 AltOutPPul
// *** Remap SPI1_REMAP=1
// SPI1 SS: A15 Out_PPul  SCK : B3 AltOutPPul  MISO : B4 In_float 
//      MOSI : B5 AltOutPPul
// bRateDiv : PCLK=
//----------------------------------------------------------
void F103_SPI_Init(SPI_TypeDef *Spi,uint8_t remap, int bRateDiv, uint8_t cpol, uint8_t calpha)
{
	MyGPIO_Struct_TypeDef  MyGpio;
	bRateDiv = bRateDiv %8; // coerce to 0-7 range
	

			if(Spi== SPI1) { // RCC, port enable
													RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; 
													RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; if (remap!=0) RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
													// Pin init SS- SCLK - MISO - MOSI
													if (remap==0) {
													AFIO->MAPR &= ~AFIO_MAPR_SPI1_REMAP;
													MyGpio.GPIO=GPIOA;
													MyGpio.GPIO_Pin=4;MyGpio.GPIO_Conf=Out_Ppull ;F103_GPIO_Init(&MyGpio); //SS Hardware management
													MyGpio.GPIO->ODR |= (1<<(MyGpio.GPIO_Pin)); // SS HIGH at start
													MyGpio.GPIO_Pin=5;MyGpio.GPIO_Conf=AltOut_Ppull;F103_GPIO_Init(&MyGpio);	
													MyGpio.GPIO_Pin=6;MyGpio.GPIO_Conf=In_PullUp;F103_GPIO_Init(&MyGpio); 
													MyGpio.GPIO_Pin=7;MyGpio.GPIO_Conf=AltOut_Ppull;F103_GPIO_Init(&MyGpio);}
			               }
													
				if((Spi== SPI1)&& (remap!=0))			{		
													AFIO->MAPR |= AFIO_MAPR_SPI1_REMAP;
													MyGpio.GPIO=GPIOA;
													MyGpio.GPIO_Pin=15;MyGpio.GPIO_Conf=Out_Ppull ;F103_GPIO_Init(&MyGpio);
													MyGpio.GPIO->ODR |= (1<<(MyGpio.GPIO_Pin)); // SS HIGH at start	
													MyGpio.GPIO=GPIOB;
													MyGpio.GPIO_Pin=3;MyGpio.GPIO_Conf=AltOut_Ppull;F103_GPIO_Init(&MyGpio);	
													MyGpio.GPIO_Pin=4;MyGpio.GPIO_Conf=In_PullUp;F103_GPIO_Init(&MyGpio); 
													MyGpio.GPIO_Pin=5;MyGpio.GPIO_Conf=AltOut_Ppull;F103_GPIO_Init(&MyGpio);
											}	
												
			if(Spi== SPI2) {// RCC, port enable
													RCC->APB1ENR |= RCC_APB1ENR_SPI2EN ; 
													RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
													// Pin init
													MyGpio.GPIO=GPIOB; //modif B origin
													MyGpio.GPIO_Pin=12;MyGpio.GPIO_Conf=Out_Ppull ;F103_GPIO_Init(&MyGpio);
													MyGpio.GPIO=GPIOB;
													MyGpio.GPIO->ODR |= (1<<(MyGpio.GPIO_Pin)); // SS HIGH at start
													MyGpio.GPIO_Pin=13;MyGpio.GPIO_Conf=AltOut_Ppull;F103_GPIO_Init(&MyGpio);	
													MyGpio.GPIO_Pin=14;MyGpio.GPIO_Conf=In_PullUp;F103_GPIO_Init(&MyGpio);
													MyGpio.GPIO_Pin=15;MyGpio.GPIO_Conf=AltOut_Ppull; F103_GPIO_Init(&MyGpio); 
										 }

		// SPI Registers init
		
		Spi->CR1 =0; //Clear all CR1
		Spi->CR1 |= SPI_CR1_MSTR;
		Spi->CR1 |= bRateDiv<<3;
		Spi->CR1 |= (SPI_CR1_SSM | SPI_CR1_SSI); // SSM 1 SSI 1 : Soft Management
		if(cpol) Spi->CR1 |= SPI_CR1_CPOL;
		if(calpha) Spi->CR1 |= SPI_CR1_CPHA;
		
		Spi->CR1 |= SPI_CR1_SPE;
		if (remap) AFIO->MAPR |= AFIO_MAPR_SPI1_REMAP; // Make sure SPI REMAP flag is set, after SPE set
}

// My_Spi_RW array
// Sends Tx_Array and reads Rx_String
// Length = lengthj of Tx String
// Rx String must be larger than TxString
// SS Mode controls the assert/deassert of SS pin


void F103_SPI_RW_Array(SPI_TypeDef *Spi, uint16_t  *Tx_Array, uint16_t *Rx_Array,uint8_t length, uint8_t SSMode)
{
	int i; //strlen does not count null char
	// SS GPIO
	MyGPIO_Struct_TypeDef MyGPIO;
	if (Spi==SPI1) {MyGPIO.GPIO=GPIOA;MyGPIO.GPIO_Pin=4;}
	if (Spi==SPI2) {MyGPIO.GPIO=GPIOB;MyGPIO.GPIO_Pin=12;} //Modif B origin
	if ((Spi==SPI1)&&(AFIO->MAPR & AFIO_MAPR_SPI1_REMAP  )){MyGPIO.GPIO=GPIOA;MyGPIO.GPIO_Pin=15;}

	
	//Ready to send ?
	while ((Spi->SR & SPI_SR_BSY)!=0){}; // Check SPI not BSY
	

	//SS start toggle
		if ((SSMode==SSMOD_HLLH)|| (SSMode==SSMOD_HLLL))
		{
		MyGPIO.GPIO->BSRR |= 0x01<<MyGPIO.GPIO_Pin; // SS to H
		}
	// Delay ?
		//for (j=0;j<5;j++);
    
		MyGPIO.GPIO->BSRR |= 0x01<< ((MyGPIO.GPIO_Pin)+16); //SS to L
	
		//for (j=0;j<5;j++);
	
	// SPI Transaction
		
		// 1 byte transmit
		if (length==1) {
		Spi->DR = (uint8_t) *Tx_Array;
		while ((Spi->SR & SPI_SR_RXNE)==0){};
				*Rx_Array = Spi->DR;
		while ((Spi->SR & SPI_SR_BSY)!=0){}; // Check SPI not BSY
		}
		
		  if(length>1) {
			//Send 1st Byte
			Spi->DR =  *Tx_Array;
				
				for (i=0;i<length-1;i++){
				while ((Spi->SR & SPI_SR_TXE)==0){}; //Wait TXE set
					Tx_Array++;
					Spi->DR = *Tx_Array;
					while ((Spi->SR & SPI_SR_RXNE)==0){};
					*Rx_Array = Spi->DR;
					Rx_Array++;
					
				}
			
				while ((Spi->SR & SPI_SR_RXNE)==0){};
				*Rx_Array = Spi->DR;
		
				while ((Spi->SR & SPI_SR_BSY)!=0){}; // Check SPI not BSY
				}
		
	// SS end	toggle
		if ((SSMode==SSMOD_HLLH)|| (SSMode==SSMOD_XXXH))
		{
		MyGPIO.GPIO->BSRR |= 0x01<<MyGPIO.GPIO_Pin; // SS to H
		}
	
}	





