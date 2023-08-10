
#include "Comm.h"

DateStamp_Typedef dateStamp; 

const char bor_status[8][50]={"Vent Debout\n","au Pres\n","Bon Plein\n","au Petit Largue\n","au Travers\n","au Largue\n","au Grand Largue\n","Vent Arriere\n"};
const char alrm_status[2][20]={"Batterie Faible\n","Roulis trop fort\n"};

/*
*
*  Init_Com
*  Configures all comm related functions
*  Sets I2C and DS1307, writes current timestamp
*  Enables DMA CLK, init USART
*
*/

void COM_Init(void)
{

dateStamp.day=D_DAY; dateStamp.month=D_MONTH;dateStamp.year2d=D_YEAR;dateStamp.weekday=D_WEEKDAY;
dateStamp.hour=D_HOUR;dateStamp.min=D_MIN; dateStamp.sec=D_SEC;

	
DMA_CK_ENABLE; // Enable DMA clock
DS1307_I2C_Init(); // Start I2C
F103_USART_Init(TX_USART,9600,WORD_8,STOP_BITS_1,NO_PARITY);


DS1307_Set_Date_Time_Start(&dateStamp);
}


void COM_Start(void){

F103_USART_TX_DMA_Init(TX_USART);


}


void COM_Send_Bor_Message(void){
uint8_t status;
char Bor_Message[COM_MAX_LEN]={'\0'};
char Time_Message[COM_MAX_LEN]={0};
int angle= abs(E6B2_Get_Angle());

DS1307_Read_Date(&dateStamp);
sprintf(Time_Message,"A %02uH:%02u:%02u le voilier navigue ",dateStamp.hour,dateStamp.min,dateStamp.sec);

if (angle<30) status = 0;
if ((angle>=30)&&(angle<45)) status = 1;
if ((angle>=45)&&(angle<60)) status = 2;
if ((angle>=60)&&(angle<80)) status = 3;
if ((angle>=80)&&(angle<100)) status = 4;
if ((angle>=100)&&(angle<120)) status = 5;
if ((angle>=120)&&(angle<150)) status = 6;
if (angle>=150) status = 7;	
strcpy(Bor_Message,bor_status[status]);
strcat(Time_Message,Bor_Message);

while(Is_Tx_Pending());
F103_USART_TX_DMA_Send(TX_USART,Time_Message);

}
	
void COM_Send_Alarm(uint16_t Batt_status, uint16_t Roll_status)
{

	if (Batt_status!=0){
	while(Is_Tx_Pending());
	F103_USART_TX_DMA_Send(TX_USART,(char*) alrm_status[0]);	
	}
	
	if (Roll_status!=0){
	while(Is_Tx_Pending());
	F103_USART_TX_DMA_Send(TX_USART,(char*) alrm_status[1]);	
	}


}






