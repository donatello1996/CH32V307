#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "ch32v30x.h"
#include "debug.h"
#include "uart.h"
#include "adc_pa0.h"

#include "spi1_spi2.h"
#include "lora.h"

#include "i2c.h"
#include "bmp280.h"
#include "paj7620.h"
#include "mpu6050.h"
#include "bh1750.h"

#if BOARD_MODEL == BOARD_CH32_TOS_LORA

#include "tos_lcd.h"

#endif

uint8_t lora_tx_buffer[50] = {1 , 3};

extern uint8_t spo2 , heart;

#define READ	        1   
#define WRITE	        0   
                            
#define ADRW 	        0x20
#define ADRR 	        0x21

uint16_t gChipID = 0;
uint8_t  RDA5807P_REGW[10];

uint8_t RDA5807P_initialization_reg[]={
#if defined(_SHARE_CRYSTAL_24MHz_)
    0xC0, 0x51, //02H:
#elif defined(_SHARE_CRYSTAL_12MHz_)
    0xC0, 0x11, //02H:
#elif defined(_SHARE_CRYSTAL_32KHz_)
    0xC0, 0x01, //02H:
#else
    0xC0, 0x01,
#endif

#if defined(_FM_STEP_50K_)
    0x00, 0x12,        
#else
    0x00, 0x10,
#endif
    0x04, 0x00,        
    0x88, 0xAF, //05H: 
    0x00, 0x00,        
    0x5E, 0xC6,        
    0x50, 0x96,        
    0x00, 0x00,        
    0x40, 0x00, //0AH: 
    0x00, 0x8F,        
    0x00, 0x00,        
    0x00, 0x00,        
    0x00, 0x00,        
    0x00, 0x00,        
    0xF0, 0x05, //10H: 
    0x90, 0x00,        
    0xF4, 0x84,        
    0x70, 0x01,        
    0x40, 0xF0,        
    0x21, 0x80, //15H: 
    0x7A, 0xD0,        
    0x3E, 0x40,        
    0x55, 0xA9,        
    0xE8, 0x48,        
    0x50, 0x80, //1AH: 
    0x00, 0x00,        
    0x37, 0xB6,        
    0x40, 0x0C,        
    0x07, 0x9B,        
    0x4C, 0x1D,        
    0x81, 0x11, //20H: 
    0x45, 0xC0,        
};


uint8_t RDA5807PE_initialization_reg[]={
#if defined(_SHARE_CRYSTAL_24MHz_)
    0xc4, 0x51, //02H:
#elif defined(_SHARE_CRYSTAL_12MHz_)
    0xc4, 0x11, //02H:
#elif defined(_SHARE_CRYSTAL_32KHz_)
    0xc4, 0x01, //02H:
#else
    0xC0, 0x01,
#endif

#if defined(_FM_STEP_50K_)
    0x1b, 0x92,
    0x0C, 0x00,
#else       //Step 100K
    0x00, 0x10,
    0x04, 0x00,
#endif

    0x86, 0xad, //05H:
    0x80, 0x00,
    0x5F, 0x1A, //07H
    0x5e, 0xc6,
    0x00, 0x00,
    0x40, 0x6e, //0AH: 
    0x2d, 0x80,
    0x58, 0x03,
    0x58, 0x04,
    0x58, 0x04,
    0x58, 0x04,
    0x00, 0x47, //10H: 
    0x90, 0x00,
    0xF5, 0x87,
    0x7F, 0x0B, //13H:
	0x04, 0xF1,
	0x5E, 0xc0, //15H: 0x42, 0xc0
    0x41, 0xe0,
    0x50, 0x6f,
    0x55, 0x92,
    0x00, 0x7d,
    0x10, 0xC0,//1AH
    0x07, 0x80,
    0x41, 0x1d,//1CH,
    0x40, 0x06,
    0x1f, 0x9B,
    0x4c, 0x2b,//1FH. 
    0x81, 0x10, //20H: 
    0x45, 0xa0,// 21H

#if defined(_FM_STEP_50K_)
    0x55, 0x3F, //22H
#else
    0x19, 0x3F, //22H
#endif

    0xaf, 0x40, 
	0x06, 0x81,
    0x1b, 0x2a, //25H
    0x0D, 0x04,
	0x80, 0x9F, //0x80, 0x2F, 
    0x17, 0x8A,
    0xD3, 0x49,
    0x11, 0x42,
    0xA0, 0xC4, //2BH
	0x3E, 0xBB,
    0x00, 0x00,
    0x58, 0x04,
    0x58, 0x04, //2FH
    0x58, 0x04,
    0x00, 0x74,
    0x3D, 0x00,
    0x03, 0x0C,
    0x2F, 0x68,
    0x38, 0x77, //35H
};

uint8_t RDA5807PH_initialization_reg[]={
#if defined(_SHARE_CRYSTAL_24MHz_)
    0xC4, 0x51, //02H:
#elif defined(_SHARE_CRYSTAL_12MHz_)
    0xC4, 0x11, //02H:
#elif defined(_SHARE_CRYSTAL_32KHz_)
    0xC4, 0x01, //02H:
#else
    0xC0, 0x01,
#endif
#if defined(_FM_STEP_50K_)
    0x00,0x12,
#else
    0x00,0x10,
#endif
    0x04,0x00,
    0x86,0xBF,  //05H   
    0x40,0x00,
    0x56,0xC6,
    0x00,0x00,
    0x00,0x00,
    0x00,0x00,  //0AH
    0x00,0x00,
    0x00,0x00,
    0x00,0x00,
    0x00,0x00,
    0x00,0x00,
    0x00,0x06,  //10H
    0x00,0x19, //
    0x2A,0x11,
    0x00,0x2E,
    0x2A,0x30,
    0xB8,0x3C,  //15H
    0x90,0x00,
    0x2A,0x91,
    0x84,0x12,
    0x00,0xA8,
    0xC4,0x00,  //1AH
    0xE0,0x00,
    0x30,0x1D,//0x24,0x9D,1cH
    0x81,0x6A,
    0x46,0x08,
    0x00,0x86,  //1FH
    0x06,0x61,  //20H
    0x00,0x00,
    0x10,0x9E,
    0x24,0xC9,//   0x24,0x47,//0830//23H
    0x04,0x08,//0830
    0x06,0x08,  //0x06,0x08,//0830  //0X25H
    0xE1,0x05,
    0x3B,0x6C,
    0x2B,0xEC,
    0x09,0x0F,
    0x34,0x14,  //2AH
    0x14,0x50,
    0x09,0x6D,
    0x2D,0x96,
    0x01,0xDA,
    0x2A,0x7B,
    0x08,0x21,   //30H
    0x13,0xD5,
    0x48,0x91,
    0x00,0xbc,
    0x08,0x96,   //34H
    0x15,0x3C,  //35H
    0x0B,0x80,   //36H
    0x25,0xC7,   //37H
    0x00,0x00,   //38H
	0x3C,0x58,
	0x2A,0x50,
	0x26,0x2C,
	0x06,0x08,
	0x02,0x00,
	0x00,0x00,
};

uint8_t RDA5807N_initialization_reg[]={
#if defined(_SHARE_CRYSTAL_24MHz_)
    0xC4, 0x51, //02H:
#elif defined(_SHARE_CRYSTAL_12MHz_)
    0xC4, 0x11, //02H:
#elif defined(_SHARE_CRYSTAL_32KHz_)
    0xC4, 0x01, //02H:
#else
    0xC0, 0x01,
#endif
    0x00, 0x00,
    0x04, 0x00,
    0xC6, 0xad, //05h
    0x60, 0x00,
    0x42, 0xC6,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,  //0x0ah
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,  //0x10h
    0x00, 0x19,
    0x2a, 0x11,
    0xB0, 0x42,  
    0x2A, 0x11,  
    0xb8, 0x31,  //0x15h 
    0xc0, 0x00,
    0x2a, 0x91,
    0x94, 0x00,
    0x00, 0xa8,
    0xc4, 0x00,  //0x1ah
    0xF7, 0xcF,   
    0x2A, 0xDC,  //0x1ch
    0x80, 0x6F, 
    0x46, 0x08,
    0x00, 0x86,
    0x06, 0x61, //0x20H
    0x00, 0x00,
    0x10, 0x9E,
    0x23, 0xC8,
    0x04, 0x06,
    0x0E, 0x1C, //0x25H
};

uint8_t OperationRDAFM_2w(uint8_t operation , uint8_t *datas , uint8_t numBytes)
{
	uint8_t j , acknowledge = 0;
	
	I2C_Start_PD8_PD9();
	if(operation == READ)
	{
		acknowledge = I2C_Send_Byte_PD8_PD9_Ack(ADRR);
	}
	else
	{
		acknowledge = I2C_Send_Byte_PD8_PD9_Ack(ADRW);
	}

	for(j = 0; j < numBytes; j++, datas++)
	{
		if(operation == READ)
		{
			if(j == (numBytes - 1))
			{
				*datas = I2C_Read_Byte_PD8_PD9_Ack(1);
			} 
			else
			{
				*datas = I2C_Read_Byte_PD8_PD9_Ack(0);
			} 
		}
		else
		{
			acknowledge = I2C_Send_Byte_PD8_PD9_Ack(*datas); 
		}
	}
	I2C_Stop_PD8_PD9();
	return acknowledge;
}

uint16_t RDA5807P_FreqToChan(uint16_t frequency) 
{
	uint8_t channelSpacing;
	uint16_t bottomOfBand;
	uint16_t channel;

	if ((RDA5807P_REGW[3] & 0x0c) == 0x00) 
		bottomOfBand = 8700;
	else if ((RDA5807P_REGW[3] & 0x0c) == 0x04)	
		bottomOfBand = 7600;
	else if ((RDA5807P_REGW[3] & 0x0c) == 0x08)	
		bottomOfBand = 7600;	
	if ((RDA5807P_REGW[3] & 0x03) == 0x00) 
		channelSpacing = 10;
	else if ((RDA5807P_REGW[3] & 0x03) == 0x01) 
		channelSpacing = 10;
	else if ((RDA5807P_REGW[3] & 0x03) == 0x02) 
		channelSpacing = 5;
	channel = (frequency - bottomOfBand) / channelSpacing;
	return (channel);
}

void RDA5807P_SetFreq(short curFreq)
{   
	uint16_t curChan;
	
	curChan = RDA5807P_FreqToChan(curFreq);
	
	//SetNoMute
	RDA5807P_REGW[0] |=	1<<7;
	RDA5807P_REGW[2]=curChan>>2;
	RDA5807P_REGW[3]=(((curChan&0x0003)<<6)|0x10) | (RDA5807P_REGW[3]&0x0f);	//set tune bit
	
	OperationRDAFM_2w(WRITE, &(RDA5807P_REGW[0]), 4);
	Delay_ms(50);     //Delay five ms
}

bool RDA5807P_Intialization(void)
{
	uint8_t error_ind = 0;
	uint8_t RDA5807P_REGR[10]={0x0};
    uint8_t i = 0;

    RDA5807P_REGW[0] = 0x00;
    RDA5807P_REGW[1] = 0x02;

	error_ind = OperationRDAFM_2w(WRITE, (uint8_t *)&RDA5807P_REGW[0], 2);
	Delay_ms(50);

	error_ind = OperationRDAFM_2w(READ, (uint8_t *)&RDA5807P_REGR[0], 10);
	Delay_ms(50);

    gChipID = RDA5807P_REGR[8];
    gChipID = ((gChipID<<8) | RDA5807P_REGR[9]);

	printf("gChipID = %d\n" , gChipID);

    if (gChipID == 0x5808)  //RDA5807N
    {
        for (i=0;i<8;i++)
            RDA5807P_REGW[i] = RDA5807N_initialization_reg[i];
     
        error_ind = OperationRDAFM_2w(WRITE, (uint8_t *)&RDA5807N_initialization_reg[0], 2);
        Delay_ms(600); 
	    error_ind = OperationRDAFM_2w(WRITE, (uint8_t *)&RDA5807N_initialization_reg[0], sizeof(RDA5807N_initialization_reg));
    }
    else if (gChipID == 0x5804)     //RDA5807PE,RDA5807SP
    {
        for (i=0;i<8;i++)
            RDA5807P_REGW[i] = RDA5807PE_initialization_reg[i];
     
        error_ind = OperationRDAFM_2w(WRITE, (uint8_t *)&RDA5807PE_initialization_reg[0], 2);
        Delay_ms(600); 
	    error_ind = OperationRDAFM_2w(WRITE, (uint8_t *)&RDA5807PE_initialization_reg[0], sizeof(RDA5807PE_initialization_reg));
    }
    else if (gChipID == 0x5801)     //RDA5807H,RDA5807HP
    {
        for (i=0;i<8;i++)
            RDA5807P_REGW[i] = RDA5807PH_initialization_reg[i];
     
        error_ind = OperationRDAFM_2w(WRITE, (uint8_t *)&RDA5807PH_initialization_reg[0], 2);
        Delay_ms(600); 
	    error_ind = OperationRDAFM_2w(WRITE, (uint8_t *)&RDA5807PH_initialization_reg[0], sizeof(RDA5807PH_initialization_reg));
        Delay_ms(100);

		do
		{
			i++;
			if(i>10) 
				return 0; 
			
		RDA5807P_SetFreq(8750);

		Delay_ms(10);   	

		//read REG0A&0B	
		OperationRDAFM_2w(READ,&(RDA5807P_REGR[0]), 4);
		if((RDA5807P_REGR[3]&0x80)==0)
			{
				RDA5807P_REGW[1] &= 0xFE;	
				error_ind = OperationRDAFM_2w(WRITE, (uint8_t *)&RDA5807P_REGW[0], 2);
				Delay_ms(50);	
				RDA5807P_REGW[1] |= 0x01;
				error_ind = OperationRDAFM_2w(WRITE, (uint8_t *)&RDA5807P_REGW[0], 2);
				Delay_ms(50);

		}
		}while((RDA5807P_REGR[3]&0x80)==0);
    }
    else
    {
        gChipID = RDA5807P_REGR[4];
        gChipID = ((gChipID<<8) | RDA5807P_REGR[5]);
    }
    
    if ((gChipID == 0x5802) || (gChipID == 0x5803))
    {
        gChipID = 0x5802;

        for (i=0;i<8;i++)
            RDA5807P_REGW[i] = RDA5807P_initialization_reg[i];


        error_ind = OperationRDAFM_2w(WRITE, (uint8_t *)&RDA5807P_initialization_reg[0], 2);
        Delay_ms(600); 
        error_ind = OperationRDAFM_2w(WRITE, (uint8_t *)&RDA5807P_initialization_reg[0], sizeof(RDA5807P_initialization_reg));

    }

	Delay_ms(50); 
	
	if (error_ind )
	   return 0;
	else
	   return 1;
}

#define TEA5767_ADDR_W      0xc0    // TEA5767 写地址
#define TEA5767_ADDR_R      0xc1    // TEA5767 读地址

uint8_t s_radioReadData[5];
uint8_t s_radioWriteData[5];

void TEA5767_Write(void)
{		
    uint8_t i;
    
    I2C_Start_PB6_PB7();
    I2C_Send_Byte_PB6_PB7(TEA5767_ADDR_W);
    I2C_Wait_Ack_PB6_PB7();
    for(i = 0; i < 5; i++)
    {
        I2C_Send_Byte_PB6_PB7(s_radioWriteData[i]);
        I2C_Ack_PB6_PB7();
    }
    I2C_Stop_PB6_PB7();
}

static uint32_t s_pll = 0;

void TEA5767_Read(void)
{
    uint8_t i;
    uint8_t tempLow;
    uint8_t tempHigh;
    s_pll = 0;
    
    I2C_Start_PB6_PB7();
    I2C_Send_Byte_PB6_PB7(TEA5767_ADDR_R); 
    I2C_Wait_Ack_PB6_PB7();
    for(i = 0; i < 5; i++)
    {
        s_radioReadData[i] = I2C_Read_Byte_PB6_PB7(1);
    }
    I2C_Stop_PB6_PB7();
    tempLow = s_radioReadData[1];
    tempHigh = s_radioReadData[0];
    tempHigh &= 0x3f;
    s_pll = tempHigh * 256 + tempLow;
}

uint8_t TeaReadStatus(void)
{
	uint8_t res = 0;
	uint8_t i = 0;
	I2C_Start_PB6_PB7();
	I2C_Send_Byte_PB6_PB7(TEA5767_ADDR_R); 
	res = I2C_Wait_Ack_PB6_PB7();
	printf("TeaReadStatus = %d.\n" , res);
	if(res)
	{
		return res;
	}
	for(i = 0; i < 5; i++)
	{
		s_radioReadData[i] = I2C_Read_Byte_PB6_PB7(1);
		if(i == 4) I2C_NAck_PB6_PB7();
		else I2C_Ack_PB6_PB7();
	}
	I2C_Stop_PB6_PB7();
	return 0;
}

void IWDG_Feed_Init(u16 prer, u16 rlr)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(prer);
    IWDG_SetReload(rlr);
    IWDG_ReloadCounter();
    IWDG_Enable();
}

int main(void)
{
	bool rec_start = false;
	bool rec_stop = false;
    uint32_t i;
	uint8_t ret , rda5807_write_data[10] , rda5807_read_data[10];

	float pres_bmp280 , temp_bmp280 , asl;
	uint8_t temp_dht11 , humi_dht11;

    short aacx , aacy , aacz;
	short gyrox , gyroy , gyroz;
	short magx , magy , magz;

	uint16_t bh1750_data;

	uint32_t timer_count = 0;
	bool flag_spo2_heart = false;

	uint32_t dat;
	uint16_t co2Data,TVOCData;

	uint16_t adc_val;

    Delay_Init();
    UART1_Init(115200);
	printf("SystemClk:%d\n",SystemCoreClock);
	IWDG_Feed_Init(IWDG_Prescaler_256 , 4000);
	// 3.2s IWDG reset

	printf("Brother Long is lihaility.\n");
	printf("Master XU is dalaoility.\n");

	UART2_Init(9600 , 1);
	//UART2_Send_String("UART2\n");

	UART4_Init(9600 , 1);
	//UART4_Send_String("UART4\n");

	ADC_Function_Init();
	DAC1_Init();

	SPI1_Init();
	SPI2_Init();
	RA02_GPIO_Init();
    RA02_EXTI_Int_Init();
	SX1276_Init(400 , 7);

#if BOARD_MODEL == BOARD_CH32_EVB_LORA
	I2C_Init_PB6_PB7();
	BMP280_Init();
	PAJ7620_Init();
	MPU9250_Init();
	BH1750_Init();

	printf("BSP_DHT11_Init = %d.\n" , BSP_DHT11_Init());

#elif BOARD_MODEL == BOARD_CH32_TOS_LORA

	TOS_LCD_Init();
	TOS_LCD_Fill(0 , 0 , TOS_LCD_W , TOS_LCD_W , BLACK);

#endif

	//I2C_Init_PD8_PD9();

 	while(1)
	{

		// if(mpu_dmp_get_data(&pitch , &roll , &yaw) == 0)
		// {
		// 	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
		// 	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);

		// 	printf("pitch = %d.%-3d roll = %d.%-3d yaw = %d.%-3d\n" ,
		// 	(int)pitch , abs(((int)(pitch * 1000)) % 1000) , 
		// 	(int)roll , abs(((int)(roll * 1000)) % 1000) , 
		// 	(int)yaw , abs(((int)(yaw * 1000)) % 1000)
		// 	);
		// }

#if BOARD_MODEL == BOARD_CH32_EVB_LORA
		ret = I2C_Read_Addr_PB6_PB7(PAJ7620_ID , 0x43);
		if(ret != 0)
		{
			switch(ret)
			{
				case 16:
				{
					printf("Approach.\n");

					break;
				}
				case 32:
				{
					printf("Away.\n");
					break;
				}
				case 4:
				{
					printf("Forward.\n");
					break;
				}
				case 8:
				{
					printf("Backword.\n");
					break;
				}
				case 2:
				{
					printf("Left.\n");
					break;
				}
				case 1:
				{
					printf("Right.\n");
					break;
				}
				case 64:
				{
					printf("Clockwise.\n");
					break;
				}
				case 128:
				{
					printf("Anti-clockwise.\n");
					break;
				}
			}
		}

		if(timer_count % 200 == 0)
		{
			adc_val = Get_ADC_Val(1);
			printf("adc_val = %d\n" , adc_val);
			lora_tx_buffer[2] = adc_val >> 8;
			lora_tx_buffer[3] = adc_val & 0xff;
			lora_tx_buffer[4] = 0;

			BMP280_Read_Pressure_Tempature(&pres_bmp280 , &temp_bmp280);
			asl = BMP280PressureToAltitude(&pres_bmp280);
			printf("pres_bmp280 = %d.%-3d , temp_bmp280 = %d.%-3d , asl = %d.%-3d\n" , 
			(int)pres_bmp280 , abs(((int)(pres_bmp280 * 1000)) % 1000) , 
			(int)temp_bmp280 , abs(((int)(temp_bmp280 * 1000)) % 1000) ,
			(int)asl , abs(((int)(asl * 1000)) % 1000)
			);

			lora_tx_buffer[6] = ((int)pres_bmp280) >> 8;
			lora_tx_buffer[7] = ((int)pres_bmp280) & 0xff;

			lora_tx_buffer[8] = (((int)(pres_bmp280 * 65536)) & 65536) >> 8;
			lora_tx_buffer[9] = ((int)(pres_bmp280 * 65536)) & 0xff;

			lora_tx_buffer[10] = ((int)temp_bmp280) >> 8;
			lora_tx_buffer[11] = ((int)temp_bmp280) & 0xff;

			printf("DHT11_Read_Data = %d.\n" , DHT11_Read_Data(&temp_dht11 , &humi_dht11));
			printf("temp_dht11 = %d , humi_dht11 = %d.\n" , temp_dht11 , humi_dht11);

			lora_tx_buffer[12] = ((int)humi_dht11) >> 8;
			lora_tx_buffer[13] = ((int)humi_dht11) & 0xff;
		
			aacx = I2C_Read_Addr_PB6_PB7(0x68 , ACCEL_XOUT_H) << 8 | I2C_Read_Addr_PB6_PB7(0x68 , ACCEL_XOUT_H + 1);
			aacx /= 164;
			aacy = I2C_Read_Addr_PB6_PB7(0x68 , ACCEL_YOUT_H) << 8 | I2C_Read_Addr_PB6_PB7(0x68 , ACCEL_YOUT_H + 1);
			aacy /= 164;
			aacz = I2C_Read_Addr_PB6_PB7(0x68 , ACCEL_ZOUT_H) << 8 | I2C_Read_Addr_PB6_PB7(0x68 , ACCEL_ZOUT_H + 1);
			aacz /= 164;
			printf("ACC:%d %d %d\n" , aacx , aacy , aacz);

			gyrox = I2C_Read_Addr_PB6_PB7(0x68 , GYRO_XOUT_H) << 8 | I2C_Read_Addr_PB6_PB7(0x68 , GYRO_XOUT_H + 1);
			gyrox /= 164;
			gyroy = I2C_Read_Addr_PB6_PB7(0x68 , GYRO_YOUT_H) << 8 | I2C_Read_Addr_PB6_PB7(0x68 , GYRO_YOUT_H + 1);
			gyroy /= 164;
			gyroz = I2C_Read_Addr_PB6_PB7(0x68 , GYRO_ZOUT_H) << 8 | I2C_Read_Addr_PB6_PB7(0x68 , GYRO_ZOUT_H + 1);
			gyroz /= 164;
			printf("GYRO:%d %d %d\n" , gyrox , gyroy , gyroz);

			I2C_Write_Reg_Data_PB6_PB7(0x68 , 0x37 , 0x02);//turn on Bypass Mode 
			Delay_ms(10);	
			I2C_Write_Reg_Data_PB6_PB7(0x0c , 0x0A , 0x01);
			Delay_ms(10);	

			magx = I2C_Read_Addr_PB6_PB7(0x0c , MAG_XOUT_H) << 8 | I2C_Read_Addr_PB6_PB7(0x0c , MAG_XOUT_H - 1);
			magy = I2C_Read_Addr_PB6_PB7(0x0c , MAG_YOUT_H) << 8 | I2C_Read_Addr_PB6_PB7(0x0c , MAG_YOUT_H - 1);
			magz = I2C_Read_Addr_PB6_PB7(0x0c , MAG_ZOUT_H) << 8 | I2C_Read_Addr_PB6_PB7(0x0c , MAG_ZOUT_H - 1);
			printf("MAG:%d %d %d\n" , magx , magy , magz);

			lora_tx_buffer[14] = aacx >> 8;
			lora_tx_buffer[15] = aacx & 0xff;

			lora_tx_buffer[16] = aacy >> 8;
			lora_tx_buffer[17] = aacy & 0xff;

			lora_tx_buffer[18] = aacz >> 8;
			lora_tx_buffer[19] = aacz & 0xff;

			lora_tx_buffer[20] = gyrox >> 8;
			lora_tx_buffer[21] = gyrox & 0xff;

			lora_tx_buffer[22] = gyroy >> 8;
			lora_tx_buffer[23] = gyroy & 0xff;

			lora_tx_buffer[24] = gyroz >> 8;
			lora_tx_buffer[25] = gyroz & 0xff;

			lora_tx_buffer[26] = magx >> 8;
			lora_tx_buffer[27] = magx & 0xff;

			lora_tx_buffer[28] = magy >> 8;
			lora_tx_buffer[29] = magy & 0xff;

			lora_tx_buffer[30] = magz >> 8;
			lora_tx_buffer[31] = magz & 0xff;

			bh1750_data = BH1750_Data_Read();
			printf("BH1750_Data_Read = %d\n" , bh1750_data);

			lora_tx_buffer[32] = bh1750_data >> 8;
			lora_tx_buffer[33] = bh1750_data & 0xff;
		}

		// if(timer_count % 250 == 0)
		// {
		// 	printf("timer_count = %d\n" , timer_count);
		// 	flag_spo2_heart = 1 - flag_spo2_heart;
		// 	if(flag_spo2_heart == true)
		// 		UART2_Send_String("AT+HEART\r\n");
		// 	else
		// 		UART2_Send_String("AT+SPO2\r\n");
		// }
		// if(timer_count % 300 == 0)
		// {
		// 	printf("spo2 = %d heart = %d\n" , spo2 , heart);
		// 	lora_tx_buffer[34] = spo2 >> 8;
		// 	lora_tx_buffer[35] = spo2 & 0xff;
		// 	lora_tx_buffer[36] = heart >> 8;
		// 	lora_tx_buffer[37] = heart & 0xff;
		// }
		// if(gyrox >= 80)
		// 	lora_tx_buffer[38] = 1;
		// if(gyroy >= 80)
		// 	lora_tx_buffer[39] = 1;

		if(timer_count % 1200 == 0)
		{
			printf("timer_count = %d\n" , timer_count);
			Lora_TxData(lora_tx_buffer , 50);
			IWDG_ReloadCounter();
			UART4_Send_String("AT+DMOVERQ\r\n");
		}

		if(timer_count % 10000 == 0)
		{
			lora_tx_buffer[4] = 0;
			lora_tx_buffer[38] = 0;
			lora_tx_buffer[39] = 0;
		}		

#elif BOARD_MODEL == BOARD_CH32_TOS_LORA

		if(timer_count % 120000000 == 0)
		{
			IWDG_ReloadCounter();
			UART4_Send_String("AT+DMOVERQ\r\n");
		}

#endif

	timer_count ++;

	if(timer_count >= 1000000000)
		timer_count = 0;

	} 
}
