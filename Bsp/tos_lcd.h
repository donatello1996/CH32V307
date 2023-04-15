#ifndef _TOS_LCD_H_
#define _TOS_LCD_H_

#include "ch32v30x.h"
#include "global_variables.h"

#if BOARD_MODEL == BOARD_CH32_TOS_LORA

#define TOS_LCD_USE_HORIZONTAL 0

#define WHITE         	            0xFFFF
#define BLACK         	            0x0000	  
#define BLUE           	            0x001F  
#define BRED                        0XF81F
#define GRED 			            0XFFE0
#define GBLUE			            0X07FF
#define RED           	            0xF800
#define MAGENTA       	            0xF81F
#define GREEN         	            0x07E0
#define CYAN          	            0x7FFF
#define YELLOW        	            0xFFE0
#define BROWN 			            0XBC40
#define BRRED 			            0XFC07
#define GRAY  			            0X8430
#define DARKBLUE      	            0X01CF
#define LIGHTBLUE      	            0X7D7C
#define GRAYBLUE       	            0X5458
#define LIGHTGREEN     	            0X841F
#define LGRAY 			            0XC618
#define LGRAYBLUE                   0XA651
#define LBBLUE                      0X2B12

#define TOS_LCD_W                   240
#define TOS_LCD_H                   240

#define TOS_LCD_SCLK_Clr() GPIO_WriteBit(GPIOE,GPIO_Pin_1,0)
#define TOS_LCD_SCLK_Set() GPIO_WriteBit(GPIOE,GPIO_Pin_1,1)

#define TOS_LCD_MOSI_Clr() GPIO_WriteBit(GPIOD,GPIO_Pin_1,0)
#define TOS_LCD_MOSI_Set() GPIO_WriteBit(GPIOD,GPIO_Pin_1,1)

#define TOS_LCD_RES_Clr()  GPIO_WriteBit(GPIOD,GPIO_Pin_3,0)
#define TOS_LCD_RES_Set()  GPIO_WriteBit(GPIOD,GPIO_Pin_3,1)

#define TOS_LCD_DC_Clr()   GPIO_WriteBit(GPIOE,GPIO_Pin_0,0)
#define TOS_LCD_DC_Set()   GPIO_WriteBit(GPIOE,GPIO_Pin_0,1)

#define TOS_LCD_BLK_Clr()  GPIO_WriteBit(GPIOD,GPIO_Pin_0,1)
#define TOS_LCD_BLK_Set()  GPIO_WriteBit(GPIOD,GPIO_Pin_0,0)

void TOS_LCD_GPIO_Init(void);

void TOS_LCD_Writ_Bus(uint8_t dat);
void TOS_LCD_WR_DATA8(uint8_t dat);
void TOS_LCD_WR_DATA16(uint16_t dat);
void TOS_LCD_WR_REG(uint8_t dat);
void TOS_LCD_Address_Set(uint16_t x1 , uint16_t y1 , uint16_t x2 , uint16_t y2);
void TOS_LCD_Init(void);

void TOS_LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);

void TOS_LCD_Show_Char_1632(int x,int y,unsigned char ch , uint16_t fc , uint16_t bc);

void TOS_LCD_ShowString_1632(uint16_t x , uint16_t y , const uint8_t *p , uint16_t fc , uint16_t bc);

#endif

#endif