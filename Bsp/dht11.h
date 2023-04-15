#ifndef _DHT11_H_
#define _DHT11_H_
#include "ch32v30x.h"

#define DHT11_GPIO 			GPIOD
#define DHT11_PIN 			GPIO_Pin_10

#define DHT11_IO_IN()  	    {DHT11_GPIO->CFGHR &= 0XFFFFF0FF ; DHT11_GPIO->CFGHR |= 0X00000800;}
#define DHT11_IO_OUT() 	    {DHT11_GPIO->CFGHR &= 0XFFFFF0FF ; DHT11_GPIO->CFGHR |= 0X00000300;}
 									   
#define	DHT11_DQ_OUT(n)     (n ? (DHT11_GPIO->BSHR |= DHT11_PIN) : (DHT11_GPIO->BCR |= DHT11_PIN))
#define	DHT11_DQ_IN         GPIO_ReadInputDataBit(DHT11_GPIO , DHT11_PIN)

unsigned char BSP_DHT11_Init(void);
unsigned char DHT11_Read_Data(unsigned char *temp , unsigned char *humi);
unsigned char DHT11_Read_Byte(void);
unsigned char DHT11_Read_Bit(void);
unsigned char DHT11_Check(void);
void DHT11_Rst(void);

#endif
