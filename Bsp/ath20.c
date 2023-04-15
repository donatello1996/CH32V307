#include "ath20.h"

uint8_t ATH20_Read_Status(void)
{
    uint8_t ret = I2C_Read_Addr_PB6_PB7(ATH20_SLAVE_ADDRESS, 0x00);
    printf("ATH20_Read_Status = 0x%x\n" , ret);
	return ret;
}

uint8_t ATH20_Read_Cal_Enable(void)
{
    uint8_t val = 0;//ret = 0,

    val = ATH20_Read_Status();
    if((val & 0x68) == 0x08)
        return 1;
    else
        return 0;
}

void ATH20_Read_CTdata(uint32_t *ct)
{
    uint32_t RetuData = 0;
	uint16_t cnt = 0;
    uint8_t Data[10];
    uint8_t tmp[10];

    tmp[0] = 0x33;
    tmp[1] = 0x00;
    I2C_Write_Reg_Data_PB6_PB7(ATH20_SLAVE_ADDRESS , StartTest , tmp[0]);
    I2C_Write_Reg_Data_PB6_PB7(ATH20_SLAVE_ADDRESS , StartTest + 1, tmp[1]);
    //I2C_Write_Reg_Datas_PB6_PB7(ATH20_SLAVE_ADDRESS , StartTest , 2 , tmp);
	Delay_ms(75);

    cnt = 0;
	while(((ATH20_Read_Status()&0x80) == 0x80))
	{
        Delay_ms(1);
        if(cnt++ >= 100)
        {
            break;
        }
	}

    I2C_Read_Datas_PB6_PB7(ATH20_SLAVE_ADDRESS , 0x00 , 7 , Data);

	RetuData = 0;
    RetuData = (RetuData|Data[1]) << 8;
	RetuData = (RetuData|Data[2]) << 8;
	RetuData = (RetuData|Data[3]);
	RetuData = RetuData >> 4;
	ct[0] = RetuData;
    printf("ct[0] = %d\n" , ct[0]);

    RetuData = 0;
	RetuData = (RetuData|Data[3]) << 8;
	RetuData = (RetuData|Data[4]) << 8;
	RetuData = (RetuData|Data[5]);
	RetuData = RetuData&0xfffff;
	ct[1] = RetuData;
    printf("ct[1] = %d\n" , ct[1]);
}

uint8_t count;
uint8_t ATH20_Init(void)
{
    uint8_t tmp[10];

    Delay_ms(40);

    tmp[0] = 0x08;
    tmp[1] = 0x00;

    I2C_Write_Reg_Data_PB6_PB7(ATH20_SLAVE_ADDRESS , INIT , tmp[0]);
    I2C_Write_Reg_Data_PB6_PB7(ATH20_SLAVE_ADDRESS , INIT + 1, tmp[1]);
    //I2C_Write_Reg_Datas_PB6_PB7(ATH20_SLAVE_ADDRESS , INIT , 2 , tmp);

    Delay_ms(500);
    count = 0;

    while(ATH20_Read_Cal_Enable() == 0)
    {
        I2C_Write_Reg_Data_PB6_PB7(ATH20_SLAVE_ADDRESS , INIT , tmp[0]);
        I2C_Write_Reg_Data_PB6_PB7(ATH20_SLAVE_ADDRESS , INIT + 1, tmp[1]);
        //I2C_Write_Reg_Datas_PB6_PB7(ATH20_SLAVE_ADDRESS , INIT , 2 , tmp);
        Delay_ms(200);

        count++;
        if(count >= 10)
            return 0;
        Delay_ms(500);
    }
    return 1;
}
