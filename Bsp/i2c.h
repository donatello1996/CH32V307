#ifndef _I2C_H_
#define _I2C_H_
#include "ch32v30x.h"
#include <stdbool.h>

#define I2C_GPIO_PD8_PD9                GPIOD
#define I2C_SCL_PIN_PD8                 GPIO_Pin_8
#define I2C_SDA_PIN_PD9                 GPIO_Pin_9

#define I2C_SDA_IN_PD9()  		        {I2C_GPIO_PD8_PD9->CFGHR &= 0XFFFFFF0F ; I2C_GPIO_PD8_PD9->CFGHR |= 0X00000080;}
#define I2C_SDA_OUT_PD9() 		        {I2C_GPIO_PD8_PD9->CFGHR &= 0XFFFFFF0F ; I2C_GPIO_PD8_PD9->CFGHR |= 0X00000030;}

#define I2C_SCL_L_PD8    			    I2C_GPIO_PD8_PD9->BCR |= I2C_SCL_PIN_PD8
#define I2C_SCL_H_PD8    			    I2C_GPIO_PD8_PD9->BSHR |= I2C_SCL_PIN_PD8
#define I2C_SDA_L_PD9    			    I2C_GPIO_PD8_PD9->BCR |= I2C_SDA_PIN_PD9
#define I2C_SDA_H_PD9    			    I2C_GPIO_PD8_PD9->BSHR |= I2C_SDA_PIN_PD9
#define I2C_READ_SDA_PD9   		        GPIO_ReadInputDataBit(I2C_GPIO_PD8_PD9 , I2C_SDA_PIN_PD9)

void I2C_Start_PD8_PD9(void);
void I2C_Stop_PD8_PD9(void);
uint8_t I2C_Wait_Ack_PD8_PD9(void);
void I2C_Send_Byte_PD8_PD9(uint8_t txd);
uint16_t I2C_Read_Byte_PD8_PD9(uint8_t ack);

bool I2C_Get_Ack_PD8_PD9(void);
bool I2C_Send_Byte_PD8_PD9_Ack(uint8_t txd);
void I2C_Send_Ack_PD8_PD9(bool ack);
uint16_t I2C_Read_Byte_PD8_PD9_Ack(bool ack);
uint16_t I2C_Read_Addr_PD8_PD9_2bytes_Ack(uint8_t dev_addr , uint8_t reg);
void I2C_Write_Reg_Data_PD8_PD9(uint8_t dev_addr , uint8_t reg , uint8_t data);

//-------------------------------------------------------------------------------------

#define I2C_GPIO_PB6_PB7                GPIOB
#define I2C_SCL_PIN_PB6                 GPIO_Pin_6
#define I2C_SDA_PIN_PB7                 GPIO_Pin_7

#define I2C_SDA_IN_PB7()  		        {I2C_GPIO_PB6_PB7->CFGLR &= 0X0FFFFFFF ; I2C_GPIO_PB6_PB7->CFGLR |= 0X80000000;}
#define I2C_SDA_OUT_PB7() 		        {I2C_GPIO_PB6_PB7->CFGLR &= 0X0FFFFFFF ; I2C_GPIO_PB6_PB7->CFGLR |= 0X30000000;}

#define I2C_SCL_L_PB6    			    I2C_GPIO_PB6_PB7->BCR |= I2C_SCL_PIN_PB6
#define I2C_SCL_H_PB6    			    I2C_GPIO_PB6_PB7->BSHR |= I2C_SCL_PIN_PB6
#define I2C_SDA_L_PB7    			    I2C_GPIO_PB6_PB7->BCR |= I2C_SDA_PIN_PB7
#define I2C_SDA_H_PB7    			    I2C_GPIO_PB6_PB7->BSHR |= I2C_SDA_PIN_PB7
#define I2C_READ_SDA_PB7   		        GPIO_ReadInputDataBit(I2C_GPIO_PB6_PB7 , I2C_SDA_PIN_PB7)

void I2C_Init_PB6_PB7(void);
void I2C_IDLE_PB6_PB7();
void I2C_Start_PB6_PB7(void);
void I2C_Stop_PB6_PB7(void);
uint8_t I2C_Wait_Ack_PB6_PB7(void);
void I2C_Ack_PB6_PB7(void);
void I2C_NAck_PB6_PB7(void);
void I2C_Send_Byte_PB6_PB7(uint8_t txd);
uint16_t I2C_Read_Byte_PB6_PB7(uint8_t ack);
uint8_t I2C_Read_Addr_PB6_PB7(uint8_t dev_addr , uint8_t reg);
void I2C_Read_Datas_PB6_PB7(uint8_t dev_addr , uint8_t reg , uint8_t data_len , uint8_t data[]);
void I2C_Write_Reg_Data_PB6_PB7(uint8_t dev_addr , uint8_t reg , uint8_t data);
void I2C_Write_Reg_Datas_PB6_PB7(uint8_t dev_addr , uint8_t reg , uint8_t data_len , uint8_t data[]);
void I2C_Write_Reg_PB6_PB7_BH1750(uint8_t reg);
void Multiple_read_BH1750(void);

#endif
















