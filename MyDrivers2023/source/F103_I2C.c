#include "F103_I2c.h"


// Pin I2C1 PB6 SCL PB7 SDA  All pins AltOpen Drain
// I2C1 Remap PB8 SCL PB9 SDA 
// I2C2 B10 SCL B11 SDA
// APB1 36 MHz

void F103_I2C_Init(I2C_TypeDef *i2c,uint8_t remap, uint8_t SpeedMode)
{
	
	// GPIO Config 
	
	MyGPIO_Struct_TypeDef MyGPIO;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;  //Enable Port B
	if(i2c==I2C1) RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;  //Enable I2C1 CK
	if(i2c==I2C2) RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;  //Enable I2C2 CK
	MyGPIO.GPIO=GPIOB;
	if ((i2c==I2C1)&&remap) {
		
			// Busy Flag glitch fix from STMF100x ErrataSheet p22 sec 2.11.17
		  MyGPIO.GPIO_Pin=8; MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
		  MyGPIO.GPIO->ODR |= (1<<(MyGPIO.GPIO_Pin)); // Set SCL ODR=1
		  while(! (MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); // check bit set
			MyGPIO.GPIO_Pin=9; MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
			MyGPIO.GPIO->ODR |= (1<<(MyGPIO.GPIO_Pin)); // Set SDA ODR=1
		  while(! (MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); //Check bit set
		  MyGPIO.GPIO_Pin=8;MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
		  MyGPIO.GPIO->ODR &= ~(1<<(MyGPIO.GPIO_Pin)); // Set SCL ODR=0
		  while((MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); //Check bit clear
		  MyGPIO.GPIO_Pin=9;MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
		  MyGPIO.GPIO->ODR &= ~(1<<(MyGPIO.GPIO_Pin)); // Set SDA ODR=0
		  while((MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); //Check bit clear
		  MyGPIO.GPIO_Pin=8;MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
		  MyGPIO.GPIO->ODR |= (1<<(MyGPIO.GPIO_Pin)); // Set SCL ODR=1
		  while(!(MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); //Check bit set
		  MyGPIO.GPIO_Pin=9;MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
		  MyGPIO.GPIO->ODR |= (1<<(MyGPIO.GPIO_Pin)); // Set SDA ODR=1
		  while(!(MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); //Check bit set
			MyGPIO.GPIO_Pin=8; MyGPIO.GPIO_Conf=AltOut_OpenDrain; F103_GPIO_Init(&MyGPIO);
			MyGPIO.GPIO_Pin=9; MyGPIO.GPIO_Conf=AltOut_OpenDrain; F103_GPIO_Init(&MyGPIO);
			}	
	if (i2c==I2C1) {
		  // Busy Flag glitch fix from STMF100x ErrataSheet p22 sec 2.11.17
			MyGPIO.GPIO_Pin=6; MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
		  MyGPIO.GPIO->ODR |= (1<<(MyGPIO.GPIO_Pin)); // Set SCL ODR=1
		  while(! (MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); // check bit set
			MyGPIO.GPIO_Pin=7; MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
			MyGPIO.GPIO->ODR |= (1<<(MyGPIO.GPIO_Pin)); // Set SDA ODR=1
		  while(! (MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); //Check bit set
		  MyGPIO.GPIO_Pin=6;MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
		  MyGPIO.GPIO->ODR &= ~(1<<(MyGPIO.GPIO_Pin)); // Set SCL ODR=0
		  while((MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); //Check bit clear
		  MyGPIO.GPIO_Pin=7;MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
		  MyGPIO.GPIO->ODR &= ~(1<<(MyGPIO.GPIO_Pin)); // Set SDA ODR=0
		  while((MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); //Check bit clear
		  MyGPIO.GPIO_Pin=6;MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
		  MyGPIO.GPIO->ODR |= (1<<(MyGPIO.GPIO_Pin)); // Set SCL ODR=1
		  while(!(MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); //Check bit set
		  MyGPIO.GPIO_Pin=7;MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
		  MyGPIO.GPIO->ODR |= (1<<(MyGPIO.GPIO_Pin)); // Set SDA ODR=1
		  while(!(MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); //Check bit set
			MyGPIO.GPIO_Pin=6;MyGPIO.GPIO_Conf=AltOut_OpenDrain; F103_GPIO_Init(&MyGPIO);
			MyGPIO.GPIO_Pin=7; MyGPIO.GPIO_Conf=AltOut_OpenDrain; F103_GPIO_Init(&MyGPIO);	
		}
	
		if (i2c==I2C2) {
		  // Busy Flag glitch fix from STMF100x ErrataSheet p22 sec 2.11.17
			MyGPIO.GPIO_Pin=10; MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
		  MyGPIO.GPIO->ODR |= (1<<(MyGPIO.GPIO_Pin)); // Set SCL ODR=1
		  while(! (MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); // check bit set
			MyGPIO.GPIO_Pin=11; MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
			MyGPIO.GPIO->ODR |= (1<<(MyGPIO.GPIO_Pin)); // Set SDA ODR=1
		  while(! (MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); //Check bit set
		  MyGPIO.GPIO_Pin=10;MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
		  MyGPIO.GPIO->ODR &= ~(1<<(MyGPIO.GPIO_Pin)); // Set SCL ODR=0
		  while((MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); //Check bit clear
		  MyGPIO.GPIO_Pin=11;MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
		  MyGPIO.GPIO->ODR &= ~(1<<(MyGPIO.GPIO_Pin)); // Set SDA ODR=0
		  while((MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); //Check bit clear
		  MyGPIO.GPIO_Pin=10;MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
		  MyGPIO.GPIO->ODR |= (1<<(MyGPIO.GPIO_Pin)); // Set SCL ODR=1
		  while(!(MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); //Check bit set
		  MyGPIO.GPIO_Pin=11;MyGPIO.GPIO_Conf=Out_OpenDrain; F103_GPIO_Init(&MyGPIO);
		  MyGPIO.GPIO->ODR |= (1<<(MyGPIO.GPIO_Pin)); // Set SDA ODR=1
		  while(!(MyGPIO.GPIO->IDR & (1<<(MyGPIO.GPIO_Pin)))); //Check bit set
			MyGPIO.GPIO_Pin=10;MyGPIO.GPIO_Conf=AltOut_OpenDrain; F103_GPIO_Init(&MyGPIO);
			MyGPIO.GPIO_Pin=11; MyGPIO.GPIO_Conf=AltOut_OpenDrain; F103_GPIO_Init(&MyGPIO);	
		}
		
		
		i2c->CR1 |= I2C_CR1_SWRST; //RESET I2C set
		i2c->CR1 &= ~I2C_CR1_SWRST; //RESET I2C clear ***END of fix***
		
	 // Registers config PE must be disabled to configure CCR
		
		i2c->CR2 |= 36; // APB1 clock 36MHz
		i2c->TRISE &= ~(I2C_TRISE_TRISE); //Reset  TRISE value
		if (SpeedMode==STD_SPEED) {i2c->CCR = 180; i2c->TRISE |= 36;} // = 1000ns / TClock I2C (36MHz) Datsheet F103 p 71
		if (SpeedMode==FAST_SPEED) {i2c->CCR =45;  i2c->TRISE |= 11;}  // = 300nS / TClock I2C
	 
		i2c->CR1 |= I2C_CR1_PE; // Enable I2C
		if (remap) AFIO->MAPR |= AFIO_MAPR_I2C1_REMAP; //Set MAPR if remap
}



void F103_I2C_MRead(I2C_TypeDef *i2c,uint8_t slaveAdd, uint16_t *readArray, uint16_t nBytes)
{
volatile	uint16_t buffer, count=0;
	
	// Initiate Read Sequence
	
	i2c->CR1 |= I2C_CR1_ACK ; // Set ACK NOT MENTIONED IN REFERENCE MANUAL
	i2c->CR1 |= I2C_CR1_START; //Start condition (see RM008 p 759)
	
	while(!(i2c->SR1 & I2C_SR1_SB )){};	//Wait for SB flag	
	buffer = i2c->SR1; // Dummy read 
  
		
// 1 byte Read 
		
	if (nBytes==1)
			{
			i2c->DR = (slaveAdd<<1) +1;  //Clear SB : Write Slave Address shifted, LSB 1 to read (and 0 to write)
			while (!(i2c->SR1 & I2C_SR1_ADDR)) {};//Wait for ADDR
			i2c->CR1 &= ~I2C_CR1_ACK ; // NACK : Acknowledge disable
			buffer = i2c->SR1; // Dummy read to clear ADDR
			buffer = i2c->SR2; // Dummy read to clear ADDR
			i2c->CR1 |= I2C_CR1_STOP ; // STOP condition
			while (!(i2c->SR1 & I2C_SR1_RXNE)) {}; //Wait RXNE
			readArray[0]=i2c->DR; //Read data
			}

// 2 Bytes Read			
	else if (nBytes==2) 
			{
			i2c->CR1 |= I2C_CR1_ACK ; // Set ACK
			i2c->CR1 |= I2C_CR1_POS ; // Set POS	
			i2c->DR = (slaveAdd<<1) +1;  //Clear SB : Write Slave Address shifted, LSB 1 to read (and 0 to write)
			while (!(i2c->SR1 & I2C_SR1_ADDR)) {};//Wait for ADDR
			buffer = i2c->SR1; // Dummy read to clear ADDR
			buffer = i2c->SR2; // Dummy read to clear ADDR
			i2c->CR1 &= ~I2C_CR1_ACK ; // NACK : Acknowledge disable
			while (!(i2c->SR1 & I2C_SR1_BTF)) {}; //Wait BTF
			i2c->CR1 |= I2C_CR1_STOP ; // STOP condition
			readArray[0]=i2c->DR;
			readArray[1]=i2c->DR;
			}
			
// N>2 Bytes Read
		
	else if (nBytes>2) {	
			
			i2c->DR = (slaveAdd<<1) +1;  //Clear SB : Write Slave Address shifted, LSB 1 to read (and 0 to write)
			while (!(i2c->SR1 & I2C_SR1_ADDR)) {};//Wait for ADDR
		
					buffer = i2c->SR1;  // Dummy read to clear ADDR
					buffer = i2c->SR2;  // Dummy read to clear ADDR
				
					// 3 or more bytes to read remaining
					for (count =0; count <nBytes-3; count++){
					while (!(i2c->SR1 & I2C_SR1_RXNE)) {}; //Wait for RXNE
					readArray[count]=i2c->DR;							//read DR, clear RXNE
					i2c->CR1 |= I2C_CR1_ACK ;	//ACK
					}
					
					// 3 last Bytes cf RM008p726
					while (!(i2c->SR1 & I2C_SR1_RXNE)) {}; //Wait for RXNE NO DR Read
					i2c->CR1 |= I2C_CR1_ACK ;	//ACK
						
					while (!(i2c->SR1 & I2C_SR1_BTF)) {}; //Wait for BTF, don't read DR (N-2 is in DR and N-1 in Shift Register)
					i2c->CR1 &= ~I2C_CR1_ACK ; // clear ACK bit (NACK generation)
					readArray[count]=i2c->DR; // Read N-2 Byte, data N being received, N-1 goes to DR
					i2c->CR1 |= I2C_CR1_STOP ; // STOP condition
					readArray[count+1]=i2c->DR; //Read N-1
					while (!(i2c->SR1 & I2C_SR1_RXNE)) {}; //Wait for RXNE
					readArray[count+2]=i2c->DR; // Read last byte
						
			}

}

void F103_I2C_MWrite(I2C_TypeDef *i2c,uint8_t slaveAdd, uint16_t *writeArray, uint16_t nBytes, uint8_t stop)
{


volatile	uint16_t buffer, count=0;
	
	
	// Initiate Write Sequence
	
	i2c->CR1 |= I2C_CR1_ACK ; // Set ACK NOT MENTIONED IN REFERENCE MANUAL
	i2c->CR1 |= I2C_CR1_START; //Start condition (see RM008 p 759)

	while(!(i2c->SR1 & I2C_SR1_SB )){};	//Wait for SB flag	
	buffer = i2c->SR1; // Dummy read 
	i2c->CR1 &= ~I2C_CR1_ACK ; // NACK : Acknowledge disable
	i2c->DR = (slaveAdd<<1) +0;  //Clear SB : Write Slave Address shifted, LSB 0 = write
			while (!(i2c->SR1 & I2C_SR1_ADDR)) {};//Wait for ADDR
		
			buffer = i2c->SR1;  // Dummy read to clear ADDR
			buffer = i2c->SR2;  // Dummy read to clear ADDR
				
			//Send N-1 bytes
			for (count =0; count <nBytes-1; count++){
			i2c->DR = writeArray[count];	
			while (!(i2c->SR1 & I2C_SR1_TXE)) {}; //Wait for TX empty TXE=1				
			}
			
			// Last Byte
      		
			i2c->DR = writeArray[count];
			while (!(i2c->SR1 & I2C_SR1_TXE)) {};
			if(stop) i2c->CR1 |= I2C_CR1_STOP ; // STOP condition if STOP is <> 0
 

}

void F103_I2C_MWrite8b(I2C_TypeDef *i2c,uint8_t slaveAdd, uint8_t *writeArray, uint16_t nBytes, uint8_t stop)
{

volatile	uint16_t buffer, count=0;
	
	
	// Initiate Write Sequence
	
	i2c->CR1 |= I2C_CR1_ACK ; // Set ACK NOT MENTIONED IN REFERENCE MANUAL
	i2c->CR1 |= I2C_CR1_START; //Start condition (see RM008 p 759)

	while(!(i2c->SR1 & I2C_SR1_SB )){};	//Wait for SB flag	
	buffer = i2c->SR1; // Dummy read 
	i2c->CR1 &= ~I2C_CR1_ACK ; // NACK : Acknowledge disable
	i2c->DR = (slaveAdd<<1) +0;  //Clear SB : Write Slave Address shifted, LSB 0 = write
			while (!(i2c->SR1 & I2C_SR1_ADDR)) {};//Wait for ADDR
		
			buffer = i2c->SR1;  // Dummy read to clear ADDR
			buffer = i2c->SR2;  // Dummy read to clear ADDR
				
			//Send N-1 bytes
			for (count =0; count <nBytes-1; count++){
			i2c->DR =(uint16_t)  writeArray[count];	
			while (!(i2c->SR1 & I2C_SR1_TXE)) {}; //Wait for TX empty TXE=1				
			}
			
			// Last Byte
      		
			i2c->DR = (uint16_t) writeArray[count];
			while (!(i2c->SR1 & I2C_SR1_TXE)) {};
			if(stop) i2c->CR1 |= I2C_CR1_STOP ; // STOP condition if STOP is <> 0

}
//-------------------------------------------------------------
// I2C with DMA
// I2C1 TX : CH6  RX : CH7
// I2C2 TX : CH4  RX : CH5
//-------------------------------------------------------------
void F103_I2C_TX_DMA_Init (I2C_TypeDef *i2c){
	DMA_Channel_TypeDef * dmaChannel;
	DMA_CK_ENABLE;

	i2c->CR2|= I2C_CR2_DMAEN;
	if (i2c==I2C1){dmaChannel=DMA1_Channel6;Register_DMA_TC_Callback(6,DMA1_I2C_CH6_IT); DMA1_CH6_STOP;NVIC_EnableIRQ(DMA1_Channel6_IRQn);}
	if (i2c==I2C2) {dmaChannel=DMA1_Channel4;Register_DMA_TC_Callback(4,DMA1_I2C_CH4_IT);DMA1_CH4_STOP;NVIC_EnableIRQ(DMA1_Channel4_IRQn);}
	
	dmaChannel->CCR |= 0x00001092; //Pri M, 8b->8b, M+1->P, TCIE
	dmaChannel->CPAR = (uint32_t) &i2c->DR;
	
	
}
void F103_I2C_MWrite8b_DMA (I2C_TypeDef *i2c,uint8_t slaveAdd, uint8_t *writeArray, uint16_t nBytes)
{
	volatile	uint16_t buffer, count=0;
	DMA_Channel_TypeDef * dmaChannel;
	if (i2c==I2C1){dmaChannel=DMA1_Channel6; DMA1_CH6_STOP;}
	if (i2c==I2C2){dmaChannel=DMA1_Channel4;DMA1_CH4_STOP;}
	i2c->CR2|= I2C_CR2_DMAEN;
		dmaChannel->CMAR = (uint32_t) writeArray;
		dmaChannel->CNDTR= nBytes;

			
	
	// Initiate Write Sequence
	
	i2c->CR1 |= I2C_CR1_ACK ; // Set ACK NOT MENTIONED IN REFERENCE MANUAL
	i2c->CR1 |= I2C_CR1_START; //Start condition (see RM008 p 759)

	while(!(i2c->SR1 & I2C_SR1_SB )){};	//Wait for SB flag	
	buffer = i2c->SR1; // Dummy read 
	i2c->CR1 &= ~I2C_CR1_ACK ; // NACK : Acknowledge disable
	i2c->DR = (slaveAdd<<1) +0;  //Clear SB : Write Slave Address shifted, LSB 0 = write

		  if (i2c==I2C1) DMA1_CH6_START;
		  if (i2c==I2C2) DMA1_CH4_START;
			while (!(i2c->SR1 & I2C_SR1_ADDR)) {};//Wait for ADDR
		
			buffer = i2c->SR1;  // Dummy read to clear ADDR
			buffer = i2c->SR2;  // Dummy read to clear ADDR
		
	
	
}


void DMA1_I2C_CH4_IT(void) // Disable DMA, wait for BTF and generate STOP
{

DMA1_CH4_STOP;	
I2C2->CR2&= ~I2C_CR2_DMAEN;

while((I2C2->SR1 & I2C_SR1_BTF)==0); // End BTF event
I2C2->CR1 |= I2C_CR1_STOP ;

}
	
void DMA1_I2C_CH6_IT(void)
{
DMA1_CH6_STOP;
I2C1->CR2&= ~I2C_CR2_DMAEN;
while((I2C1->SR1 & I2C_SR1_BTF)==0); // End BTF event
I2C1->CR1 |= I2C_CR1_STOP ;
}

void F103_I2C_IsBusy(I2C_TypeDef *i2c)
{
 while((i2c->SR2 & I2C_SR2_BUSY));
}


