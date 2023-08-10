
#include "F103_ADC.h"

struct ADC_CH_MAP{ GPIO_TypeDef *Gpio; uint8_t Pin;};
MyGPIO_Struct_TypeDef MyGPIO;
const struct ADC_CH_MAP adc_map[16] = {{GPIOA,0},{GPIOA,1},{GPIOA,2},{GPIOA,3},{GPIOA,4},{GPIOA,5},{GPIOA,6},
																				{GPIOA,7},{GPIOB,0},{GPIOB,1}, {GPIOC,0},{GPIOC,1},{GPIOC,2},{GPIOC,3},{GPIOC,4},{GPIOC,5}};

																				
static void F103_ADC_GPIO(	ADC_TypeDef *Adc, uint8_t Channel)
{

// RCC setup

  if (Adc == ADC1)   RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	if (Adc == ADC2)   RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;  // Adc CLK = 72Mhz/6 = 12MHz (must be < 14Mhz)	
	if (Channel<16){
	if (adc_map[Channel].Gpio == GPIOA) RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	if (adc_map[Channel].Gpio == GPIOB) RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	if (adc_map[Channel].Gpio == GPIOC) RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;}
	
//GPIO Init
	if (Channel<16)
	{
		MyGPIO.GPIO = adc_map[Channel].Gpio;
		MyGPIO.GPIO_Pin= adc_map[Channel].Pin;
		MyGPIO.GPIO_Conf=In_Analog;
		F103_GPIO_Init(&MyGPIO);
	}
}	
																				
void F103_ADC_Single_Init(ADC_TypeDef *Adc, uint8_t Channel, uint8_t sampleTimeCode)
{
 F103_ADC_GPIO(Adc,Channel);

 //ADC Init
	//Set Sample RAte
	if (Channel < 10) {Adc->SMPR2 &= ~(ADC_SMPR2_SMP0<< (3*Channel)) ;   Adc->SMPR2 |= (sampleTimeCode<< (3*Channel)) ;   } // Clear and set Sample Rate 
	if (Channel >=10) {Adc->SMPR1 &= ~(ADC_SMPR1_SMP10		<< (3*(Channel-10))); Adc->SMPR1 |= (sampleTimeCode<< (3*(Channel-10))) ;} // Clear and set Sample Rate 
	//SetChannel in first conv position
	Adc->SQR3 |= Channel;
	Adc->CR2 |= ADC_CR2_ADON;  //Activates ADC, Next ADON starts conversion
		
}

void F103_ADC_DMA_SingleCont_Init(uint8_t Channel, uint8_t SampleTime)
{
// Only ADC1 has DMA capabilities
F103_ADC_GPIO(ADC1,Channel);

//ADC Init
	//Set Sample RAte
	if (Channel < 10) {ADC1->SMPR2 &= ~(ADC_SMPR2_SMP0<< (3*Channel)) ;   ADC1->SMPR2 |= (SampleTime<< (3*Channel)) ;   } // Clear and set Sample Rate 
	if (Channel >=10) {ADC1->SMPR1 &= ~(ADC_SMPR1_SMP10		<< (3*(Channel-10))); ADC1->SMPR1 |= (SampleTime<< (3*(Channel-10))) ;} // Clear and set Sample Rate 
	//SetChannel in first conv position
	
	ADC1->SQR3 |= Channel;
	ADC1->CR2 |= (ADC_CR2_CONT|ADC_CR2_DMA|ADC_CR2_ADON);  //Activates ADC + DMA, Continuous, Next ADON starts conversion
}

void F103_ADC1_DMA_SingleTRGO_RegScan_Init(uint8_t chNumber,uint8_t *chSequence,uint8_t * sampleSeq) // ADC1 regular scan sequence TIM3 TRGO
{
uint8_t i;
	for (i=0;i<chNumber;i++) {
	F103_ADC_GPIO( ADC1,chSequence[i]);}

ADC1->SQR1 |= ((chNumber-1) <<20); // Set number of regular channels
ADC1->SQR1 |= ((chSequence[15]<<15 | chSequence[14]<<10 | chSequence[13]<<5 | chSequence[12]<<0 ));
ADC1->SQR2 |= ((chSequence[11]<<25 | chSequence[10]<<20 | chSequence[9]<<15 | chSequence[8]<<10 | chSequence[7]<<5 | chSequence[6]<<0));
ADC1->SQR3 |= ((chSequence[5]<<25 | chSequence[4]<<20 | chSequence[3]<<15 | chSequence[2]<<10 | chSequence[1]<<5 | chSequence[0]<<0));	
ADC1->SMPR1|= ((sampleSeq[17]<<21 | sampleSeq[16]<<18 | sampleSeq[15]<<15 | sampleSeq[14]<<12 | sampleSeq[13]<<9 | sampleSeq[12]<<6 | sampleSeq[11]<<3 | sampleSeq[10]<<0));	
ADC1->SMPR2|= ((sampleSeq[9]<<27 | sampleSeq[8]<<24 | sampleSeq[7]<<21 | sampleSeq[6]<<18 | sampleSeq[5]<<15 | sampleSeq[4]<<12 | sampleSeq[3]<<9 | sampleSeq[2]<<6 | sampleSeq[1]<<3 | sampleSeq[0]<<0));		
ADC1->CR1 |= ADC_CR1_SCAN;
ADC1->CR2 |= (ADC_CR2_TSVREFE | ADC_CR2_DMA | ADC_CR2_EXTTRIG);
ADC1->CR2 |= 0x4<<17; // TIM3 TRGO Event

}

int F103_ADC_Single_Conv(ADC_TypeDef *Adc)
{

	Adc->CR2 &= ~ADC_CR2_CONT; // Clear CONT bit
	Adc->CR2 |= ADC_CR2_ADON;  //Start Conversion
	while(!(Adc->SR & ADC_SR_EOC )) {}; //Wait until EOF Flag is set
	return (Adc->DR);

}


