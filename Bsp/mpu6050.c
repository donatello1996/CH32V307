#include "mpu6050.h"

void MPU6050_Init()
{
	int ret;
	ret = I2C_Read_Addr_PB6_PB7(0x68 , 0x75);
	printf("MPU6050_Init = 0x%x\n" , ret);

	I2C_Write_Reg_Data_PB6_PB7(0x68 , SMPLRT_DIV , 7);
	//Write to sample rate register
	printf("SMPLRT_DIV = 0x%x\n" , I2C_Read_Addr_PB6_PB7(0x68 , SMPLRT_DIV));

	I2C_Write_Reg_Data_PB6_PB7(0x68 , PWR_MGMT_1 , 1);
	// Write to power management register
	printf("PWR_MGMT_1 = 0x%x\n" , I2C_Read_Addr_PB6_PB7(0x68 , PWR_MGMT_1));

	I2C_Write_Reg_Data_PB6_PB7(0x68 , CONFIG , 0);
	// Write to Configuration register 
	printf("CONFIG = 0x%x\n" , I2C_Read_Addr_PB6_PB7(0x68 , CONFIG));

	I2C_Write_Reg_Data_PB6_PB7(0x68 , GYRO_CONFIG , 24);
	// Write to Gyro Configuration register
	printf("GYRO_CONFIG = 0x%x\n" , I2C_Read_Addr_PB6_PB7(0x68 , GYRO_CONFIG));

	I2C_Write_Reg_Data_PB6_PB7(0x68 , INT_ENABLE , 1);
	//Write to interrupt enable register
	printf("INT_ENABLE = 0x%x\n" , I2C_Read_Addr_PD8_PD9(0x68 , INT_ENABLE));

}

void MPU9250_Init()
{
	int ret;
	ret = I2C_Read_Addr_PB6_PB7(0x68 , 0x75);
	printf("MPU9250_Init = 0x%x\n" , ret);

	I2C_Write_Reg_Data_PB6_PB7(0x68 , PWR_MGMT_1 , 0);
	//Write to power management register
	printf("PWR_MGMT_1 = 0x%x\n" , I2C_Read_Addr_PB6_PB7(0x68 , PWR_MGMT_1));

	I2C_Write_Reg_Data_PB6_PB7(0x68 , SMPLRT_DIV , 0x07);
	//Write to sample rate register
	printf("SMPLRT_DIV = 0x%x\n" , I2C_Read_Addr_PB6_PB7(0x68 , SMPLRT_DIV));

	I2C_Write_Reg_Data_PB6_PB7(0x68 , CONFIG , 0x06);
	// Write to Configuration register 
	printf("CONFIG = 0x%x\n" , I2C_Read_Addr_PB6_PB7(0x68 , CONFIG));

	I2C_Write_Reg_Data_PB6_PB7(0x68 , GYRO_CONFIG , 0x18);
	// Write to Gyro Configuration register
	printf("GYRO_CONFIG = 0x%x\n" , I2C_Read_Addr_PB6_PB7(0x68 , GYRO_CONFIG));

	I2C_Write_Reg_Data_PB6_PB7(0x68 , ACCEL_CONFIG , 0x01);
	// Write to Gyro Configuration register
	printf("ACCEL_CONFIG = 0x%x\n" , I2C_Read_Addr_PB6_PB7(0x68 , ACCEL_CONFIG));
	
}
