#include "tos_lcd.h"

#if BOARD_MODEL == BOARD_CH32_TOS_LORA

extern unsigned char ASCII_1632[];

void TOS_LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOE , GPIO_Pin_0 | GPIO_Pin_1 , 1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD , &GPIO_InitStructure);
    GPIO_WriteBit(GPIOD , GPIO_Pin_0 , 0);
    GPIO_WriteBit(GPIOD , GPIO_Pin_1 | GPIO_Pin_3 , 1);
}

void TOS_LCD_Writ_Bus(uint8_t dat) 
{	
	uint8_t i;
	for(i = 0 ; i < 8 ; i ++)
	{			  
		TOS_LCD_SCLK_Clr();
		if(dat & 0x80)
		{
		   TOS_LCD_MOSI_Set();
		}
		else
		{
		   TOS_LCD_MOSI_Clr();
		}
		TOS_LCD_SCLK_Set();
		dat <<= 1;
	}	
}

void TOS_LCD_WR_DATA8(uint8_t dat)
{
	TOS_LCD_Writ_Bus(dat);
}

void TOS_LCD_WR_DATA16(uint16_t dat)
{
	TOS_LCD_Writ_Bus(dat >> 8);
	TOS_LCD_Writ_Bus(dat);
}

void TOS_LCD_WR_REG(uint8_t dat)
{
	TOS_LCD_DC_Clr();
	TOS_LCD_Writ_Bus(dat);
	TOS_LCD_DC_Set();
}

void TOS_LCD_Address_Set(uint16_t x1 , uint16_t y1 , uint16_t x2 , uint16_t y2)
{
	if(TOS_LCD_USE_HORIZONTAL == 0)
	{
		TOS_LCD_WR_REG(0x2a);//列地址设置
		TOS_LCD_WR_DATA16(x1);
		TOS_LCD_WR_DATA16(x2);
		TOS_LCD_WR_REG(0x2b);//行地址设置
		TOS_LCD_WR_DATA16(y1);
		TOS_LCD_WR_DATA16(y2);
		TOS_LCD_WR_REG(0x2c);//储存器写
	}
	else if(TOS_LCD_USE_HORIZONTAL == 1)
	{
		TOS_LCD_WR_REG(0x2a);//列地址设置
		TOS_LCD_WR_DATA16(x1);
		TOS_LCD_WR_DATA16(x2);
		TOS_LCD_WR_REG(0x2b);//行地址设置
		TOS_LCD_WR_DATA16(y1+80);
		TOS_LCD_WR_DATA16(y2+80);
		TOS_LCD_WR_REG(0x2c);//储存器写
	}
	else if(TOS_LCD_USE_HORIZONTAL == 2)
	{
		TOS_LCD_WR_REG(0x2a);//列地址设置
		TOS_LCD_WR_DATA16(x1);
		TOS_LCD_WR_DATA16(x2);
		TOS_LCD_WR_REG(0x2b);//行地址设置
		TOS_LCD_WR_DATA16(y1);
		TOS_LCD_WR_DATA16(y2);
		TOS_LCD_WR_REG(0x2c);//储存器写
	}
	else
	{
		TOS_LCD_WR_REG(0x2a);//列地址设置
		TOS_LCD_WR_DATA16(x1+80);
		TOS_LCD_WR_DATA16(x2+80);
		TOS_LCD_WR_REG(0x2b);//行地址设置
		TOS_LCD_WR_DATA16(y1);
		TOS_LCD_WR_DATA16(y2);
		TOS_LCD_WR_REG(0x2c);//储存器写
	}
}

void TOS_LCD_Init(void)
{
	TOS_LCD_GPIO_Init();//初始化GPIO
	
	TOS_LCD_RES_Clr();//复位
	Delay_ms(100);
	TOS_LCD_RES_Set();
	Delay_ms(100);
	
	TOS_LCD_BLK_Set();//打开背光
    Delay_ms(100);
	
	//************* Start Initial Sequence **********//
	TOS_LCD_WR_REG(0x11);       //Sleep out 
	Delay_ms(120);              //Delay 120ms
	//************* Start Initial Sequence **********// 
	TOS_LCD_WR_REG(0x36);
	if(TOS_LCD_USE_HORIZONTAL == 0)
        TOS_LCD_WR_DATA8(0x00);
	else if(TOS_LCD_USE_HORIZONTAL == 1)
        TOS_LCD_WR_DATA8(0xC0);
	else if(TOS_LCD_USE_HORIZONTAL == 2)
        TOS_LCD_WR_DATA8(0x70);
	else TOS_LCD_WR_DATA8(0xA0);

	TOS_LCD_WR_REG(0x3A);			
	TOS_LCD_WR_DATA8(0x05);

	TOS_LCD_WR_REG(0xB2);			
	TOS_LCD_WR_DATA8(0x0C);
	TOS_LCD_WR_DATA8(0x0C); 
	TOS_LCD_WR_DATA8(0x00); 
	TOS_LCD_WR_DATA8(0x33); 
	TOS_LCD_WR_DATA8(0x33); 			

	TOS_LCD_WR_REG(0xB7);			
	TOS_LCD_WR_DATA8(0x35);

	TOS_LCD_WR_REG(0xBB);			
	TOS_LCD_WR_DATA8(0x32);
					
	TOS_LCD_WR_REG(0xC2);
	TOS_LCD_WR_DATA8(0x01);

	TOS_LCD_WR_REG(0xC3);			
	TOS_LCD_WR_DATA8(0x15); //GVDD=4.8V  颜色深度
				
	TOS_LCD_WR_REG(0xC4);			
	TOS_LCD_WR_DATA8(0x20); //VDV, 0x20:0v

	TOS_LCD_WR_REG(0xC6);			
	TOS_LCD_WR_DATA8(0x0F); //0x0F:60Hz        	

	TOS_LCD_WR_REG(0xD0);			
	TOS_LCD_WR_DATA8(0xA4);
	TOS_LCD_WR_DATA8(0xA1); 

	TOS_LCD_WR_REG(0xE0);
	TOS_LCD_WR_DATA8(0xD0);   
	TOS_LCD_WR_DATA8(0x08);   
	TOS_LCD_WR_DATA8(0x0E);   
	TOS_LCD_WR_DATA8(0x09);   
	TOS_LCD_WR_DATA8(0x09);   
	TOS_LCD_WR_DATA8(0x05);   
	TOS_LCD_WR_DATA8(0x31);   
	TOS_LCD_WR_DATA8(0x33);   
	TOS_LCD_WR_DATA8(0x48);   
	TOS_LCD_WR_DATA8(0x17);   
	TOS_LCD_WR_DATA8(0x14);   
	TOS_LCD_WR_DATA8(0x15);   
	TOS_LCD_WR_DATA8(0x31);   
	TOS_LCD_WR_DATA8(0x34);   

	TOS_LCD_WR_REG(0xE1);     
	TOS_LCD_WR_DATA8(0xD0);   
	TOS_LCD_WR_DATA8(0x08);   
	TOS_LCD_WR_DATA8(0x0E);   
	TOS_LCD_WR_DATA8(0x09);   
	TOS_LCD_WR_DATA8(0x09);   
	TOS_LCD_WR_DATA8(0x15);   
	TOS_LCD_WR_DATA8(0x31);   
	TOS_LCD_WR_DATA8(0x33);   
	TOS_LCD_WR_DATA8(0x48);   
	TOS_LCD_WR_DATA8(0x17);   
	TOS_LCD_WR_DATA8(0x14);   
	TOS_LCD_WR_DATA8(0x15);   
	TOS_LCD_WR_DATA8(0x31);   
	TOS_LCD_WR_DATA8(0x34);
	TOS_LCD_WR_REG(0x21); 

	TOS_LCD_WR_REG(0x29);
} 

void TOS_LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
	uint16_t i,j; 
	TOS_LCD_Address_Set(xsta,ysta,xend-1,yend-1);
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			TOS_LCD_WR_DATA16(color);
		}
	} 					  	    
}

void TOS_LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	TOS_LCD_Address_Set(x,y,x,y);
	TOS_LCD_WR_DATA16(color);
} 

void TOS_LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		TOS_LCD_DrawPoint(uRow,uCol,color);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

void TOS_LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	TOS_LCD_DrawLine(x1,y1,x2,y1,color);
	TOS_LCD_DrawLine(x1,y1,x1,y2,color);
	TOS_LCD_DrawLine(x1,y2,x2,y2,color);
	TOS_LCD_DrawLine(x2,y1,x2,y2,color);
}

void TOS_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		TOS_LCD_DrawPoint(x0-b,y0-a,color);             //3           
		TOS_LCD_DrawPoint(x0+b,y0-a,color);             //0           
		TOS_LCD_DrawPoint(x0-a,y0+b,color);             //1                
		TOS_LCD_DrawPoint(x0-a,y0-b,color);             //2             
		TOS_LCD_DrawPoint(x0+b,y0+a,color);             //4               
		TOS_LCD_DrawPoint(x0+a,y0-b,color);             //5
		TOS_LCD_DrawPoint(x0+a,y0+b,color);             //6 
		TOS_LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//判断要画的点是否过远
		{
			b--;
		}
	}
}

void TOS_LCD_Show_Char_1632(int x , int y , unsigned char ch , uint16_t fc , uint16_t bc)
{
	int i,j;
	int n = ch - ' ';
	unsigned char pix;
	for(i=0;i<32;i++)
	{
		pix=ASCII_1632[i*2+n*64];
		for(j=0;j<8;j++)
		{
			if(pix&0x80)
				TOS_LCD_DrawPoint(x+j,y+i,fc);
			else TOS_LCD_DrawPoint(x+j,y+i,bc);
			pix<<=1;
		}
		pix=ASCII_1632[i*2+n*64+1];
		for(j=8;j<16;j++)
		{
			if(pix&0x80)
				TOS_LCD_DrawPoint(x+j,y+i,fc);
			else TOS_LCD_DrawPoint(x+j,y+i,bc);
			pix<<=1;
		}
	}
}

void TOS_LCD_ShowString_1632(uint16_t x , uint16_t y , const uint8_t *p , uint16_t fc , uint16_t bc)
{         
	while(*p != '\0')
	{       
		TOS_LCD_Show_Char_1632(x , y , *p , fc , bc);
		x += 16;
		p++;
	}  
}



#endif
