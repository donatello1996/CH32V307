#include "bh1750.h"

void BH1750_Init()
{
    BH1750_Command_Send(BHPowOn);   
	BH1750_Command_Send(BHReset);
	BH1750_Command_Send(BHModeL);
}

void BH1750_Command_Send(uint8_t command)
{
    do
    {
    	I2C_Start_PB6_PB7();
    	I2C_Send_Byte_PB6_PB7(0x46);
    }
    while(I2C_Wait_Ack_PB6_PB7());
    I2C_Send_Byte_PB6_PB7(command); 
    I2C_Wait_Ack_PB6_PB7();
    I2C_Stop_PB6_PB7(); 
}

uint16_t BH1750_Data_Read(void)
{
    uint16_t buf;
    I2C_Start_PB6_PB7(); 
    I2C_Send_Byte_PB6_PB7(0x47);
    I2C_Wait_Ack_PB6_PB7();
    buf = I2C_Read_Byte_PB6_PB7(1);
    buf = buf << 8;
    buf += 0x00ff & I2C_Read_Byte_PB6_PB7(0);
    I2C_Stop_PB6_PB7();
    return buf; 
}