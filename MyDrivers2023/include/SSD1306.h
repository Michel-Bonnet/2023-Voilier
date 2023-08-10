#ifndef SSD1306_H
#define SSD1306_H

#include "stm32f10x.h"
#include "F103_GPIO.h"
#include "F103_I2C.h"
#include "F103_DMA.h"
#include "SSD1306_font.h"

#define SSD1306_I2C (I2C1)
#define SSD1306_ADD 0x3C  //7b address
#define MODEL 1  // 1 : 128X64  2 :  128X32



// Command definition
  // ------------------------------------------------------------------------------------
  #define SSD1306_COMMAND           0x80  // Continuation bit=1, D/C=0; 1000 0000
  #define SSD1306_COMMAND_STREAM    0x00  // Continuation bit=0, D/C=0; 0000 0000
  #define SSD1306_DATA              0xC0  // Continuation bit=1, D/C=1; 1100 0000
  #define SSD1306_DATA_STREAM       0x40  // Continuation bit=0, D/C=1; 0100 0000
                 
  #define SSD1306_SET_MUX_RATIO     0xA8
  #define SSD1306_DISPLAY_OFFSET    0xD3
  #define SSD1306_DISPLAY_ON        0xAF
  #define SSD1306_DISPLAY_OFF       0xAE
  #define SSD1306_DIS_ENT_DISP_ON   0xA4
  #define SSD1306_DIS_IGNORE_RAM    0xA5
  #define SSD1306_DIS_NORMAL        0xA6
  #define SSD1306_DIS_INVERSE       0xA7
  #define SSD1306_DEACT_SCROLL      0x2E
  #define SSD1306_ACTIVE_SCROLL     0x2F
  #define SSD1306_SET_START_LINE    0x40
  #define SSD1306_MEMORY_ADDR_MODE  0x20
  #define SSD1306_SET_COLUMN_ADDR   0x21
  #define SSD1306_SET_PAGE_ADDR     0x22
  #define SSD1306_SEG_REMAP         0xA0
  #define SSD1306_SEG_REMAP_OP      0xA1
  #define SSD1306_COM_SCAN_DIR      0xC0
  #define SSD1306_COM_SCAN_DIR_OP   0xC8
  #define SSD1306_COM_PIN_CONF      0xDA
  #define SSD1306_SET_CONTRAST      0x81
  #define SSD1306_SET_OSC_FREQ      0xD5
  #define SSD1306_SET_CHAR_REG      0x8D
  #define SSD1306_SET_PRECHARGE     0xD9
  #define SSD1306_VCOM_DESELECT     0xDB

 // AREA definition
  // ------------------------------------------------------------------------------------
  #define START_PAGE_ADDR           0
	#define START_COLUMN_ADDR         0
  #define END_COLUMN_ADDR           127
	
	#if (MODEL==1)
	#define END_PAGE_ADDR             7     // 7 for 128x64, 3 for 128x32 version  
  #endif
	
	#if (MODEL==2)
	#define END_PAGE_ADDR             3     // 7 for 128x64, 3 for 128x32 version
  #endif
	
  #define VRAM           (1 + END_PAGE_ADDR) * (1 + END_COLUMN_ADDR) + 1
	#define BUFFER					40

  #define MAX_X                     END_COLUMN_ADDR   // 0...127
  #define MAX_Y                     (END_PAGE_ADDR + 1) * 8 -1  // 0...63 or 0 ... 31
	
	
	
	
void SSD1306_Init(void);
void SSD1306_Fill_Vram(uint16_t value);
void SSD1306_Vram_To_Display(void);
void SSD1306_Vram_To_Display_Partial(uint8_t startPage, uint8_t endPage);
void SSD1306_Write_Vram_String(enum font_name ftname,uint8_t pageAdd, uint8_t colAdd, char * string);
// Graphic functions
void SSD1306_DrawLine(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);
void SSD1306_DrawRect(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);
void SSD1306_DrawCircle(uint8_t XC, uint8_t YC, uint8_t R);
void SSD1306_DrawFCircle(uint8_t XC, uint8_t YC, uint8_t R);

#endif
