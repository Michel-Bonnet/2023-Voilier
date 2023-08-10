#ifndef COMM_H
#define COMM_H


#include "stm32f10x.h"
#include "F103_USART.h"
#include "DS1307.h"
#include "F103_DMA.h"
#include "E6B2.h"
#include <string.h>

#define TX_USART (USART1)

#define D_DAY (19)
#define D_MONTH (7)
#define D_YEAR (23)
#define D_HOUR (20)
#define D_MIN (50)
#define D_SEC (0)
#define D_WEEKDAY (6)
#define COM_MAX_LEN 60

void COM_Init(void);
void COM_Start(void);
void COM_Send_Bor_Message(void);

/*
*
*  Batt status : 0 OK >0 : low battery
*  roll_status : 0 OK >0 : overflow, counts remaining
*  issues alarm messages 
*/
void COM_Send_Alarm(uint16_t Batt_status, uint16_t roll_status);








#endif
