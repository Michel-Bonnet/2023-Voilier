
#include "Surveil.h"

volatile int adc_value;

void SUR_Init(void)
{

	DMA1_CH1_STOP;
	F103_ADC_DMA_SingleCont_Init(10,ADC_TS_239);
	F103_DMA_Channel_Conf(DMA1_Channel1,(uint32_t ) &adc_value,(uint32_t) &ADC1->DR,0x00005A0,1); // CCR : 16b->16b, P->M+1, CIRC
  ADXL345_SPI_Init_Start();

}

void SUR_Start(void)
{
DMA1_CH1_START;
ADC1_START;

}


int SUR_Get_Adcval (void)
{
return (adc_value);
}

void SUR_Status_Update(uint16_t * Bat_st, uint16_t * Roll_st)
{
int16_t ax,ay,az;
uint16_t curr_adcval=adc_value;
		
ADXL345_Get_Axl(&ax,&ay,&az);

if (abs(ay)>abs(az)) {(*Roll_st) = SUR_FREEZE; }else {if((*Roll_st)>0) (*Roll_st)-=1;}	
if (curr_adcval<SUR_BATT_TRESH) {(*Bat_st) =1;} else {(*Bat_st)=0;}


}
