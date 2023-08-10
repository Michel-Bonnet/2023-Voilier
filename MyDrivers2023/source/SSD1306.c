#include "SSD1306.h"
#include <string.h>
#include <math.h>

static uint8_t vram[VRAM]={0x40,[1 ... (VRAM-1)]=0x18}; // Vram Buffer, 1st byte is alwys 0x40 (GRAM write)
#if MODEL==1
static uint8_t bf[BUFFER]={0,SSD1306_DISPLAY_OFF,SSD1306_SET_MUX_RATIO,MAX_Y,SSD1306_DISPLAY_OFFSET,0,SSD1306_SET_START_LINE|0x00,
	SSD1306_SEG_REMAP_OP,SSD1306_COM_SCAN_DIR_OP,SSD1306_COM_PIN_CONF,0x12,SSD1306_SET_CONTRAST,0x7F,SSD1306_DIS_ENT_DISP_ON,SSD1306_DIS_NORMAL,
	SSD1306_SET_OSC_FREQ,0x80,SSD1306_SET_CHAR_REG,0x14,SSD1306_DISPLAY_ON }; //Command Buffer, init with initial start sequence
#endif
#if MODEL==2
static uint8_t bf[BUFFER]={0,SSD1306_DISPLAY_OFF,SSD1306_SET_MUX_RATIO,MAX_Y,SSD1306_DISPLAY_OFFSET,0,SSD1306_SET_START_LINE|0x00,
	SSD1306_SEG_REMAP_OP,SSD1306_COM_SCAN_DIR_OP,SSD1306_COM_PIN_CONF,0x02,SSD1306_SET_CONTRAST,0x7F,SSD1306_DIS_ENT_DISP_ON,SSD1306_DIS_NORMAL,
	SSD1306_SET_OSC_FREQ,0x80,SSD1306_SET_CHAR_REG,0x14,SSD1306_DISPLAY_ON }; //Command Buffer, init with initial start sequence
#endif


void SSD1306_Init(void)
{

F103_I2C_Init(SSD1306_I2C,NO_REMAP,FAST_SPEED);
F103_I2C_MWrite8b(SSD1306_I2C,SSD1306_ADD,bf,20,STOP);
bf[1]=SSD1306_MEMORY_ADDR_MODE;
bf[2]=0x00; // Horizontal	
bf[3]=SSD1306_SET_COLUMN_ADDR;
bf[4]=START_COLUMN_ADDR;
bf[5]=END_COLUMN_ADDR;
bf[6]=SSD1306_SET_PAGE_ADDR;
bf[7]=START_PAGE_ADDR;
bf[8]=END_PAGE_ADDR;
F103_I2C_MWrite8b(SSD1306_I2C,SSD1306_ADD,bf,9,STOP);
SSD1306_FONT_Register_Fonts();
F103_I2C_TX_DMA_Init(SSD1306_I2C);
	
}

void SSD1306_Fill_Vram(uint16_t value)
{
uint16_t i;
for(i=0;i<VRAM-1;i++)vram[i+1]=value;
vram[0]=0x40;
}
void SSD1306_Vram_To_Display(void)
{
bf[0]=0;
bf[1]=SSD1306_SET_COLUMN_ADDR;
bf[2]=START_COLUMN_ADDR;
bf[3]=END_COLUMN_ADDR;
bf[4]=SSD1306_SET_PAGE_ADDR;
bf[5]=START_PAGE_ADDR;
bf[6]=END_PAGE_ADDR;
F103_I2C_IsBusy(SSD1306_I2C);
F103_I2C_MWrite8b(SSD1306_I2C,SSD1306_ADD,bf,7,STOP);
F103_I2C_IsBusy(SSD1306_I2C);
F103_I2C_MWrite8b_DMA(SSD1306_I2C,SSD1306_ADD,vram,VRAM);	
}

void SSD1306_Vram_To_Display_Partial(uint8_t startPage, uint8_t endPage)
{
uint8_t savedByte = vram[startPage*128];

bf[0]=0;
bf[1]=SSD1306_SET_COLUMN_ADDR;
bf[2]=START_COLUMN_ADDR;
bf[3]=END_COLUMN_ADDR;
bf[4]=SSD1306_SET_PAGE_ADDR;
bf[5]=startPage;
bf[6]=endPage;
	
F103_I2C_IsBusy(SSD1306_I2C);
F103_I2C_MWrite8b(SSD1306_I2C,SSD1306_ADD,bf,7,STOP);
F103_I2C_IsBusy(SSD1306_I2C);
vram[startPage*128]=0x40;	
F103_I2C_MWrite8b_DMA(SSD1306_I2C,SSD1306_ADD,vram+startPage*128,(endPage-startPage+1)*128+1);	

vram[startPage*128]=savedByte;

}


void SSD1306_Write_Vram_String(enum font_name ftname,uint8_t pageAdd, uint8_t colAdd, char * string)
{
uint8_t hbytes, vbytes; // Hor and vert Bytes of FONT
uint8_t len=strlen(string), index,ih,iv;
const unsigned char * char_ptr;

if (ftname==ASC6X8){hbytes=6; vbytes=1;}
if (ftname==ASC6X12){hbytes=6; vbytes=2;}
if (ftname==ASC8X16){hbytes=8; vbytes=2;}
if (ftname==ASC12X24){hbytes=12; vbytes=3;}
char_ptr = SSD1306_FONT_GetFontAdd(ftname);

for (index=0;index<len;index++)
    for(iv=0;iv<vbytes;iv++){
			for(ih=0;ih<hbytes;ih++){
          vram[(pageAdd*128)+iv*128+colAdd+index*hbytes+ih+1]=(unsigned char) char_ptr[(string[index]-0x20)*(hbytes*vbytes)+ih+iv*hbytes];
			}
		}
}


static void SSD1306_Vram_Pixel(uint8_t x,uint8_t y)
{
	uint8_t page, offset;
// Param check
	x=x%(MAX_X +1); y = y%(MAX_Y+1);
  page = y/8;
	offset = y- 8*page;
	vram[128*page+1+x] |= (1<<offset);

}


void SSD1306_DrawLine(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2)
{
	int xcur=0,x1=0,x2=0,i=0,ycur=0,y1=0,y2=0;

	            x2=X2; y2=Y2;x1=X1;y1=Y1;

	            if (abs(X1-X2)>=abs(Y1-Y2)) // boucle y=f(x)
	            {
	                if (X1>X2) {x1=X2;x2=X1;y1=Y2;y2=Y1;}
	                for (i=0;i<(x2-x1+1);i++)
	                {
	                    ycur=  y1 + i*(y2-y1)/(x2-x1);
	                    SSD1306_Vram_Pixel((x1+i), ycur);
	                }
	            }
	            else
	            {
	                  // boucle x=f(y)
	                if (Y1>Y2) {x1=X2;x2=X1;y1=Y2;y2=Y1;}

	                for (i=0;i<((y2-y1+1));i++)
	                    {
	                    xcur= x1 + i*(x2-x1)/(y2-y1);
	                    SSD1306_Vram_Pixel(xcur, (y1+i));
	                    }}
}

void SSD1306_DrawRect(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2)
{

	uint8_t x2=0,x1=0,y2=0,y1=0;

	            x2=X2; y2=Y2; x1=X1; y1=Y1;

	            if (X1>X2) {x1=X2;x2=X1;}
	            if (Y1>Y2) {y1=Y2;y2=Y1;}

	            SSD1306_DrawLine(x1, y1, x2, y1);
	            SSD1306_DrawLine(x1, y1, x1, y2);
	            SSD1306_DrawLine(x2, y1, x2, y2);
	            SSD1306_DrawLine(x1, y2, x2, y2);
}

void SSD1306_DrawCircle(uint8_t XC, uint8_t YC, uint8_t R)
{

	uint8_t xcur, ycur;

	            for (xcur=0;xcur<(R/sqrt(2))+1; xcur++)
	            {

	            ycur = (uint8_t) sqrtf(R*R - xcur*xcur);
	            SSD1306_Vram_Pixel(XC+xcur, YC+ycur);
	            SSD1306_Vram_Pixel(XC-xcur, YC+ycur);
	            SSD1306_Vram_Pixel(XC+xcur, YC-ycur);
	            SSD1306_Vram_Pixel(XC-xcur, YC-ycur);

	            SSD1306_Vram_Pixel(XC+ycur, YC+xcur);
	            SSD1306_Vram_Pixel(XC-ycur, YC+xcur);
	            SSD1306_Vram_Pixel(XC+ycur, YC-xcur);
	            SSD1306_Vram_Pixel(XC-ycur, YC-xcur);
	            }

}


void SSD1306_DrawFCircle(uint8_t XC, uint8_t YC, uint8_t R)
	{

	uint8_t xcur, ycur;

		for (xcur=0;xcur<(R/sqrt(2))+1; xcur++){
	         ycur = sqrtf(R*R - xcur*xcur);
	         SSD1306_DrawLine(XC+xcur,YC+ycur,XC-xcur,YC+ycur);
	         SSD1306_DrawLine(XC+xcur,YC-ycur,XC-xcur,YC-ycur);
	         SSD1306_DrawLine(XC+ycur,YC+xcur,XC-ycur,YC+xcur);
	         SSD1306_DrawLine(XC+ycur,YC-xcur,XC-ycur,YC-xcur);
	           }
	}


