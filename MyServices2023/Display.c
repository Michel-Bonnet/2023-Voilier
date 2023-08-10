
#include "Display.h"

char buffer[40]={0};

void DIS_Init_Display(void){

SSD1306_Init();
	
}

void DIS_Static_Display(void)
{

SSD1306_Fill_Vram(0);
SSD1306_Write_Vram_String(ASC6X8,0,0,"Voilier 2.0 (mb)");
if (ADXL345_SPI == SPI1) sprintf(buffer,"ADx:SPI1 "); else sprintf(buffer,"ADx:SPI2 ");
if (DS1307_I2C == I2C1) sprintf(buffer+9,"DS:I2C1 "); else sprintf(buffer+9,"DS:I2C2 ");

SSD1306_Write_Vram_String(ASC6X8,1,0,buffer);
SSD1306_Vram_To_Display();	

}


void DIS_Current_Info(void){
DateStamp_Typedef ds;
DS1307_Read_Date(&ds);
sprintf(buffer,"%02uH %02um %02us",ds.hour,ds.min,ds.sec); 
SSD1306_Write_Vram_String(ASC6X8,2,0,buffer);
sprintf(buffer, "Roll %03u Wnd %03u",abs(ADXL345_Get_RollAngle()),abs(E6B2_Get_Angle()));
SSD1306_Write_Vram_String(ASC6X8,3,0,buffer);	
SSD1306_Vram_To_Display_Partial(2,3);		
}

