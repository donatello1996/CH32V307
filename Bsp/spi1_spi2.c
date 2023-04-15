#include "spi1_spi2.h"

void SPI1_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    SPI_InitTypeDef  SPI_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA , &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);

}

uint8_t SPI1_ReadWriteByte(uint8_t data)
{		
	uint8_t retry = 0;				 	
	while ((SPI1->STATR & SPI_I2S_FLAG_TXE) == RESET) 
	{
		retry ++;
		if(retry > 200)return 0;
	}			  
	SPI1->DATAR = data;
	retry=0;

	while ((SPI1->STATR & SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry ++;
		if(retry > 200)return 0;
    }
	return SPI1->DATAR;
}

uint8_t W5500_SendSpiData(uint8_t byte)
{
	uint8_t retry = 0;				 	
	while ((SPI1->STATR & SPI_I2S_FLAG_TXE) == RESET) 
	{
		retry ++;
		if(retry > 200)return 0;
	}			  
	SPI1->DATAR = byte;
	retry=0;

	while ((SPI1->STATR & SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry ++;
		if(retry > 200)return 0;
    }
	return SPI1->DATAR;
}

void SPI2_Init()
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 , ENABLE );

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE );
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE );

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB , &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB , &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB , &GPIO_InitStructure);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2 , &SPI_InitStructure );
	SPI_Cmd(SPI2 , ENABLE);
}

uint8_t SPI2_ReadWriteByte(uint8_t data)
{		
	uint8_t retry = 0;				 	
	while ((SPI2->STATR & SPI_I2S_FLAG_TXE) == RESET) 
	{
		retry ++;
		if(retry > 200)return 0;
	}			  
	SPI2->DATAR = data;
	retry=0;

	while ((SPI2->STATR & SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry ++;
		if(retry > 200)return 0;
    }
	return SPI2->DATAR;
}