#include "i2c.h"

void I2C_Init_PD8_PD9(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);
		
	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN_PD8 | I2C_SDA_PIN_PD9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_GPIO_PD8_PD9 , &GPIO_InitStructure);

	I2C_IDLE_PD8_PD9();
}

void I2C_IDLE_PD8_PD9()
{
	I2C_SDA_OUT_PD9();
    I2C_SDA_H_PD9;
    I2C_SCL_H_PD8;
    Delay_us(4);
}

void I2C_Start_PD8_PD9(void)
{
	I2C_SDA_OUT_PD9();
	I2C_SDA_H_PD9;
	I2C_SCL_H_PD8;  	  
	Delay_us(4);
	I2C_SDA_L_PD9;
	Delay_us(4);
	I2C_SCL_L_PD8;
   	Delay_us(4);
}	

void I2C_Stop_PD8_PD9(void)
{
	I2C_SDA_OUT_PD9();
	I2C_SDA_L_PD9;
    I2C_SCL_H_PD8;
    Delay_us(4);
    I2C_SDA_H_PD9;
}

uint8_t I2C_Wait_Ack_PD8_PD9(void)
{
	uint8_t rvalue;
	I2C_SDA_OUT_PD9();
	I2C_SDA_H_PD9; 
	Delay_us(4);
	I2C_SCL_H_PD8;
	Delay_us(4);
	I2C_SDA_IN_PD9();
	if(I2C_READ_SDA_PD9)
	{
		rvalue = 1;
	}
	else
	{
		rvalue = 0;
	}
	I2C_SCL_L_PD8;
	Delay_us(4);
	return rvalue;
}

void I2C_Ack_PD8_PD9(void)
{
	I2C_SDA_OUT_PD9();
	I2C_SDA_L_PD9;
    Delay_us(4);
    I2C_SCL_H_PD8;
    Delay_us(4);
    I2C_SCL_L_PD8;
    Delay_us(4);
    I2C_SDA_H_PD9;
}
    
void I2C_NAck_PD8_PD9(void)
{
	I2C_SDA_OUT_PD9();
    I2C_SDA_H_PD9;
    Delay_us(4);
    I2C_SCL_H_PD8;
    Delay_us(4);
    I2C_SCL_L_PD8;
    Delay_us(4);
}	

void I2C_Send_Byte_PD8_PD9(uint8_t txd)
{                        
    uint8_t i;   
	I2C_SDA_OUT_PD9(); 
	I2C_SCL_L_PD8;
    for(i = 0 ; i < 8 ; i ++)
    {              
		if((txd&0x80)>>7)
			I2C_SDA_H_PD9;
		else
			I2C_SDA_L_PD9;
		txd<<=1; 	  
		Delay_us(20);   
		I2C_SCL_H_PD8;
		Delay_us(20); 
		I2C_SCL_L_PD8;
		Delay_us(20);
    }	
	Delay_us(20);
}

bool I2C_Get_Ack_PD8_PD9(void)
{
    bool ret;
    I2C_SDA_H_PD9;
	Delay_us(5); 
    I2C_SCL_H_PD8;
    Delay_us(5); 
    ret = I2C_READ_SDA_PD9;
    I2C_SCL_L_PD8;
    return ret;
}

bool I2C_Send_Byte_PD8_PD9_Ack(uint8_t txd)
{                        
    uint8_t i;   
	bool ack;
	I2C_SDA_OUT_PD9(); 
	I2C_SCL_L_PD8;
    for(i = 0 ; i < 8 ; i ++)
    {              
		if((txd & 0x80) >> 7)
			I2C_SDA_H_PD9;
		else
			I2C_SDA_L_PD9;
		Delay_us(5);   
		I2C_SCL_H_PD8;
		Delay_us(5); 
		I2C_SCL_L_PD8;
		txd <<= 1;
    }
	ack = I2C_Get_Ack_PD8_PD9();
	return ack;
} 	

uint16_t I2C_Read_Byte_PD8_PD9(uint8_t ack)
{
	uint8_t i;
	uint16_t dat = 0;
	I2C_SDA_IN_PD9();
    for(i = 0 ; i < 8 ; i++)
	{
        I2C_SCL_L_PD8;
        Delay_us(20);
        I2C_SCL_H_PD8;
        dat <<= 1;
        if(I2C_READ_SDA_PD9)	
			dat++;   
		Delay_us(20); 
    }		

    if (!ack)
        I2C_NAck_PD8_PD9();
    else
        I2C_Ack_PD8_PD9();
    return dat;
}

void I2C_Send_Ack_PD8_PD9(bool ack)
{
    if(ack)
		I2C_SDA_H_PD9;
	else I2C_SDA_L_PD9;
	Delay_us(5); 
    I2C_SCL_H_PD8;
    Delay_us(5); 
    I2C_SCL_L_PD8;
}

uint16_t I2C_Read_Byte_PD8_PD9_Ack(bool ack)
{
    uint8_t loop = 8 , ret = 0;
    I2C_SDA_H_PD9;
    while(loop--)
	{
		ret <<= 1;
		I2C_SCL_H_PD8;
		Delay_us(5); 
        if(I2C_READ_SDA_PD9)
		{
			ret++;
		}
        I2C_SCL_L_PD8;
        Delay_us(5);
    }
	
	I2C_Send_Ack_PD8_PD9(ack);
	
    return ret;
}

uint16_t I2C_Read_Addr_PD8_PD9_2bytes_Ack(uint8_t dev_addr , uint8_t reg)
{
	uint16_t res[2];
	I2C_Start_PD8_PD9(); 
	I2C_Send_Byte_PD8_PD9_Ack(0x22);
	I2C_Send_Byte_PD8_PD9_Ack(reg);
	I2C_Start_PD8_PD9();
	I2C_Send_Byte_PD8_PD9_Ack(0x23);
	res[0] = I2C_Read_Byte_PD8_PD9_Ack(0);
	res[1] = I2C_Read_Byte_PD8_PD9_Ack(1);
	I2C_Stop_PD8_PD9();
	return ((res[0] << 8) | res[1]);
}

uint8_t I2C_Read_Addr_PD8_PD9(uint8_t dev_addr , uint8_t reg)
{
	uint8_t res;
	I2C_Start_PD8_PD9(); 
	I2C_Send_Byte_PD8_PD9((dev_addr << 1) | 0);
	I2C_Wait_Ack_PD8_PD9();
	I2C_Send_Byte_PD8_PD9(reg);
	I2C_Wait_Ack_PD8_PD9();
	I2C_Start_PD8_PD9();
	I2C_Send_Byte_PD8_PD9((dev_addr << 1) | 1);
	I2C_Wait_Ack_PD8_PD9();
	res = I2C_Read_Byte_PD8_PD9(0);
	I2C_Stop_PD8_PD9();
	return res;		
}

void I2C_Read_Datas_PD8_PD9(uint8_t dev_addr , uint8_t reg , uint8_t data_len , uint8_t data[])
{
	while(data_len)
	{
        *data = I2C_Read_Addr_PD8_PD9(dev_addr , reg++);
        data ++;
        data_len --;
    }	
}

void I2C_Write_Reg_Data_PD8_PD9_2bytes_Ack(uint8_t Address, uint16_t Data)
{
    uint8_t Buf[2] = {0};
    Buf[0] = (Data & 0xff00) >> 8;
    Buf[1] = Data & 0x00ff;
    I2C_Start_PD8_PD9();
    I2C_Send_Byte_PD8_PD9_Ack(0x22);
    I2C_Send_Byte_PD8_PD9_Ack(Address);
    I2C_Send_Byte_PD8_PD9_Ack(Buf[0]);
    I2C_Send_Byte_PD8_PD9_Ack(Buf[1]);
    I2C_Stop_PD8_PD9();
}

void I2C_Write_Reg_Data_PD8_PD9(uint8_t dev_addr , uint8_t reg , uint8_t data)
{
    I2C_Start_PD8_PD9();
    I2C_Send_Byte_PD8_PD9((dev_addr << 1) | 0);
    I2C_Wait_Ack_PD8_PD9();
	I2C_Send_Byte_PD8_PD9(reg); 
    I2C_Wait_Ack_PD8_PD9();
    I2C_Send_Byte_PD8_PD9(data); 
    I2C_Wait_Ack_PD8_PD9();
    I2C_Stop_PD8_PD9();
	Delay_ms(10);
}

void I2C_Write_Reg_Datas_PD8_PD9(uint8_t dev_addr , uint8_t reg , uint8_t data_len , uint8_t data[])
{
	int i;
    I2C_Start_PD8_PD9();
    I2C_Send_Byte_PD8_PD9((dev_addr << 1) | 0);
    I2C_Wait_Ack_PD8_PD9();
	I2C_Send_Byte_PD8_PD9(reg); 
    I2C_Wait_Ack_PD8_PD9();
	for(i = 0 ; i < data_len ; i++)
	{
    	I2C_Send_Byte_PD8_PD9(data[i]); 
    	I2C_Wait_Ack_PD8_PD9();
	}
    I2C_Stop_PD8_PD9();
    Delay_ms(10);
}

//-------------------------------------------------------------------------------------

void I2C_Init_PB6_PB7(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		
	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN_PB6 | I2C_SDA_PIN_PB7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_GPIO_PB6_PB7 , &GPIO_InitStructure);

	I2C_IDLE_PB6_PB7();
}

void I2C_IDLE_PB6_PB7()
{
	I2C_SDA_OUT_PB7();
    I2C_SDA_H_PB7;
    I2C_SCL_H_PB6;
    Delay_us(4);
}

void I2C_Start_PB6_PB7(void)
{
	I2C_SDA_OUT_PB7();
	I2C_SDA_H_PB7;
	I2C_SCL_H_PB6;  	  
	Delay_us(4);
	I2C_SDA_L_PB7;
	Delay_us(4);
	I2C_SCL_L_PB6;
    Delay_us(4);
}	

void I2C_Stop_PB6_PB7(void)
{
	I2C_SDA_OUT_PB7();
	I2C_SDA_L_PB7;
    I2C_SCL_H_PB6;
    Delay_us(4);
    I2C_SDA_H_PB7;
}

uint8_t I2C_Wait_Ack_PB6_PB7(void)
{
	uint8_t rvalue;
	I2C_SDA_OUT_PB7();
	I2C_SDA_H_PB7; 
	Delay_us(4);
	I2C_SCL_H_PB6;
	Delay_us(4);
	I2C_SDA_IN_PB7();
	if(I2C_READ_SDA_PB7)
	{
		rvalue = 1;
	}
	else
	{
		rvalue = 0;
	}
	I2C_SCL_L_PB6;
	Delay_us(4);
	return rvalue;
}

void I2C_Ack_PB6_PB7(void)
{
	I2C_SDA_OUT_PB7();
	I2C_SDA_L_PB7;
    Delay_us(4);
    I2C_SCL_H_PB6;
    Delay_us(4);
    I2C_SCL_L_PB6;
    Delay_us(4);
    I2C_SDA_H_PB7;
}
    
void I2C_NAck_PB6_PB7(void)
{
	I2C_SDA_OUT_PB7();
    I2C_SDA_H_PB7;
    Delay_us(4);
    I2C_SCL_H_PB6;
    Delay_us(4);
    I2C_SCL_L_PB6;
    Delay_us(4);
}	

void I2C_Send_Byte_PB6_PB7(uint8_t txd)
{                        
    uint8_t i;   
	I2C_SDA_OUT_PB7(); 
	I2C_SCL_L_PB6;
    for(i = 0 ; i < 8 ; i ++)
    {              
		if((txd&0x80)>>7)
			I2C_SDA_H_PB7;
		else
			I2C_SDA_L_PB7;
		txd <<= 1; 	  
		Delay_us(20);   
		I2C_SCL_H_PB6;
		Delay_us(20); 
		I2C_SCL_L_PB6;
		Delay_us(20);
    }	
	Delay_us(20);
} 	    

uint16_t I2C_Read_Byte_PB6_PB7(uint8_t ack)
{
	uint8_t i;
	uint16_t dat = 0;
	I2C_SDA_IN_PB7();
    for(i = 0 ; i < 8 ; i++)
	{
        I2C_SCL_L_PB6;
        Delay_us(20);
        I2C_SCL_H_PB6;
        dat <<= 1;
        if(I2C_READ_SDA_PB7)	
			dat++;   
		Delay_us(20); 
    }		

    if (!ack)
        I2C_NAck_PB6_PB7();
    else
        I2C_Ack_PB6_PB7();
    return dat;
}

uint8_t I2C_Read_Addr_PB6_PB7(uint8_t dev_addr , uint8_t reg)
{
	uint8_t res;
	I2C_Start_PB6_PB7(); 
	I2C_Send_Byte_PB6_PB7((dev_addr << 1) | 0);
	I2C_Wait_Ack_PB6_PB7();
	I2C_Send_Byte_PB6_PB7(reg);
	I2C_Wait_Ack_PB6_PB7();
	I2C_Start_PB6_PB7();
	I2C_Send_Byte_PB6_PB7((dev_addr << 1) | 1);
	I2C_Wait_Ack_PB6_PB7();
	res = I2C_Read_Byte_PB6_PB7(0);
	I2C_Stop_PB6_PB7();
	return res;		
}

void I2C_Read_Datas_PB6_PB7(uint8_t dev_addr , uint8_t reg , uint8_t data_len , uint8_t data[])
{
	while(data_len)
	{
        *data = I2C_Read_Addr_PB6_PB7(dev_addr , reg++);
        data ++;
        data_len --;
    }	
}

void I2C_Write_Reg_Data_PB6_PB7(uint8_t dev_addr , uint8_t reg , uint8_t data)
{
    I2C_Start_PB6_PB7();
    I2C_Send_Byte_PB6_PB7((dev_addr << 1) | 0);
    I2C_Wait_Ack_PB6_PB7();
	I2C_Send_Byte_PB6_PB7(reg); 
    I2C_Wait_Ack_PB6_PB7();
    I2C_Send_Byte_PB6_PB7(data); 
    I2C_Wait_Ack_PB6_PB7();
    I2C_Stop_PB6_PB7();
}

void I2C_Write_Reg_Datas_PB6_PB7(uint8_t dev_addr , uint8_t reg , uint8_t data_len , uint8_t data[])
{
	int i;
    I2C_Start_PB6_PB7();
    I2C_Send_Byte_PB6_PB7((dev_addr << 1) | 0);
    I2C_Wait_Ack_PB6_PB7();
	I2C_Send_Byte_PB6_PB7(reg); 
    I2C_Wait_Ack_PB6_PB7();
	for(i = 0 ; i < data_len ; i++)
	{
    	I2C_Send_Byte_PB6_PB7(data[i]); 
    	I2C_Wait_Ack_PB6_PB7();
	}
    I2C_Stop_PB6_PB7();
    Delay_ms(10);
}
