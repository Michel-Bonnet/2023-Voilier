#ifndef SSD1306_FONT_H
#define SSD1306_FONT_H

#include "stm32f10x.h"
#define NUM_FONT 4

enum font_name {ASC6X8 =0 , ASC6X12 , ASC8X16 , ASC12X24 };

void SSD1306_FONT_Register_Fonts(void);

const unsigned char * SSD1306_FONT_GetFontAdd(enum font_name ftname);

#endif
