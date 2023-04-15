#include "dht11.h"

void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT();
  	DHT11_DQ_OUT(0);
  	Delay_ms(20);
  	DHT11_DQ_OUT(1);
	Delay_us(30);
}

unsigned char DHT11_Check() 	   
{   
	unsigned char retry=0;
	DHT11_IO_IN();
  	while (DHT11_DQ_IN&&retry<100)
	{
		retry++;
		Delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)
	{
		retry++;
		Delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

unsigned char DHT11_Read_Bit(void) 			 
{
 	unsigned char retry=0;
	while(DHT11_DQ_IN && retry<100)
	{
		retry++;
		Delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN && retry<100)
	{
		retry++;
		Delay_us(1);
	}
	Delay_us(40);
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

unsigned char DHT11_Read_Byte(void)    
{        
    unsigned char i , dat;
    dat = 0;
	for (i = 0 ; i < 8 ; i++) 
	{
   		dat <<= 1; 
	    dat |= DHT11_Read_Bit();
    }						    
    return dat;
}

unsigned char DHT11_Read_Data(unsigned char * temp , unsigned char * humi)    
{        
 	unsigned char buf[5];
	unsigned char i;
	DHT11_Rst();
	if(DHT11_Check() == 0)
	{
		for(i = 0 ; i < 5 ; i++)
		{
			buf[i] = DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3]) == buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}
	else return 1;
	return 0;	    
}
   	 
unsigned char BSP_DHT11_Init()
{
  	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);
	
  	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_GPIO , &GPIO_InitStructure);

  	DHT11_Rst();
	return DHT11_Check();
}
