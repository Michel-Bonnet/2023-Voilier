#include "ADXL345.h"
#include <string.h>
#include <math.h>
void ADXL345_SPI_Init_Start(void)
{
 uint16_t data;
	F103_SPI_Init(ADXL345_SPI,NO_REMAP,ADXL345_SPI_BRC,1,1);
  
  data=0x08;	
  ADXL345_NWRite(POWER_CTL,&data,1); // Set measurement enablebit
	data=0x08;	
  ADXL345_NWRite(DATA_FORMAT,&data,1);
	
}

void ADXL345_NRead( uint8_t reg, uint16_t *data,  uint8_t nBytes)
{
  uint16_t *Tx_Array,*Rx_Array;
	Tx_Array = calloc (nBytes+1,sizeof(uint16_t));
	Rx_Array = calloc (nBytes+1,sizeof(uint16_t));
	if (nBytes==1) Tx_Array[0]=(reg | 0x80);  // read bit =1
	else Tx_Array[0]=(reg | 0xC0);            //read and multiple bit =1
  F103_SPI_RW_Array(ADXL345_SPI,  Tx_Array, Rx_Array,nBytes+1, SSMOD_HLLH);
	memcpy(data, Rx_Array+1,2*nBytes); //memcpy last argument is Bytes = 0.5 uint16
  // for (i=0;i<nBytes;i++) *(data+i)=* (Rx_Array+i+1);
  free(Tx_Array); free(Rx_Array);

}

void ADXL345_NWRite(uint8_t reg,uint16_t *data,  uint8_t nBytes)
{
  uint16_t *Tx_Array,*Rx_Array;
	Tx_Array = calloc (nBytes+1,sizeof(uint16_t));
	Rx_Array = calloc (nBytes+1,sizeof(uint16_t));
  if (nBytes==1) Tx_Array[0]=reg ;  // read bit =0, reg not modified
	else Tx_Array[0]=(reg | 0x40);            //read and multiple bit =1
  //for (i=0;i<nBytes;i++) Tx_Array[i+1]=*(data+i);
	memcpy(Tx_Array+1,data,2*nBytes);
	F103_SPI_RW_Array(ADXL345_SPI,  Tx_Array, Rx_Array,nBytes+1, SSMOD_HLLH);
	free(Tx_Array); free(Rx_Array); 
}

void ADXL345_Get_Axl(int16_t *ax, int16_t *ay , int16_t *az)
{
uint16_t data[8]={0};
ADXL345_NRead(DATAX0,data,6);
		
//F103_SPI_RW_Array(SPI2,SentStr,ReadStr,7,SSMOD_HLLH);
	*ax = data[0]+ (data[1]<<8) ; 
	*ay = data[2]+ (data[3]<<8) ;	
	*az = data[4]+ (data[5]<<8) ;

}


int ADXL345_Get_RollAngle(void)
{
	uint8_t i=0;
	uint16_t xData [6]={0},xc; int32_t gData [3]={0};double rollAngle=0;
	ADXL345_NRead(DATAX0,xData,6);
	
	for(i=0;i<3;i++) {
	 xc = xData[2*i]+ (xData[2*i+1]<<8);
	if ((xc & (1<<15))) // xc is negative
		{
		xc--;xc=~xc;
			gData[i]= - (int32_t)xc;
	   }
		else gData[i]=(int32_t) xc;
	
	}
	rollAngle = atan( (double)gData[0]/ gData[2])*180.0 / M_PI;
	 return ((int32_t) rollAngle);
}


