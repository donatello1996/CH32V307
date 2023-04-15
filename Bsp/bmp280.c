#include "bmp280.h"

uint8_t msb , lsb , xlsb;
float var1 , var2;
bmp280Calib  bmp280Cal;

uint8_t BMP280_Init(void)
{
    uint8_t bmp280_id;
    uint8_t tmp[1];

    I2C_Read_Datas_PB6_PB7(BMP280_SLAVE_ADDRESS_0x76, BMP280_CHIPID_REG, 1, &bmp280_id);

	printf("bmp280_id = 0x%x\n" , bmp280_id);

    I2C_Read_Datas_PB6_PB7(BMP280_SLAVE_ADDRESS_0x76, BMP280_DIG_T1_LSB_REG , 24 , (u8 *)&bmp280Cal);
	
	//printf("%d %d %d %d %d %d %d %d %d %d %d %d\n" ,
	// bmp280Cal.dig_T1 , bmp280Cal.dig_T2 , bmp280Cal.dig_T3 ,
	// bmp280Cal.dig_P1 , bmp280Cal.dig_P2 , bmp280Cal.dig_P3 , 
	// bmp280Cal.dig_P4 , bmp280Cal.dig_P5 , bmp280Cal.dig_P6 , 
	// bmp280Cal.dig_P7 , bmp280Cal.dig_P8 , bmp280Cal.dig_P9);

    tmp[0] = BMP280_MODE;
    I2C_Write_Reg_Datas_PB6_PB7(BMP280_SLAVE_ADDRESS_0x76 , BMP280_CTRLMEAS_REG , 1 , tmp);

    tmp[0] = 5 << 2;
    I2C_Write_Reg_Datas_PB6_PB7(BMP280_SLAVE_ADDRESS_0x76 , BMP280_CONFIG_REG , 1 , tmp);

    return bmp280_id;
}

float BMP280PressureToAltitude(float* pressure)
{
    if (*pressure > 0)
    {
        return ((pow((1015.7f / (*pressure)), CONST_PF) - 1.0f) * (FIX_TEMP + 273.15f)) / 0.0065f;
    }
    else
    {
        return 0;
    }
}

void BMP280_Read_Pressure_Tempature(float * pres , float * temp)
{
	msb = I2C_Read_Addr_PB6_PB7(BMP280_SLAVE_ADDRESS_0x76 , BMP280_PRESSURE_MSB_REG);
	lsb = I2C_Read_Addr_PB6_PB7(BMP280_SLAVE_ADDRESS_0x76 , BMP280_PRESSURE_LSB_REG);
	xlsb = I2C_Read_Addr_PB6_PB7(BMP280_SLAVE_ADDRESS_0x76 , BMP280_PRESSURE_XLSB_REG);
    

    *pres = (msb * 65536 | lsb * 256 | xlsb) >> 4;

	msb = I2C_Read_Addr_PB6_PB7(BMP280_SLAVE_ADDRESS_0x76 , BMP280_TEMPERATURE_MSB_REG);
	lsb = I2C_Read_Addr_PB6_PB7(BMP280_SLAVE_ADDRESS_0x76 , BMP280_TEMPERATURE_LSB_REG);
	xlsb = I2C_Read_Addr_PB6_PB7(BMP280_SLAVE_ADDRESS_0x76 , BMP280_TEMPERATURE_XLSB_REG);

    *temp = (msb * 65536 | lsb * 256 | xlsb) >> 4;

    var1 = ((*temp) / 16384.0 - bmp280Cal.dig_T1 / 1024.0)*(bmp280Cal.dig_T2);
    var2 = (((*temp) / 131072.0 - bmp280Cal.dig_T1 / 8192.0) *
	 ((*temp) / 131072.0 - bmp280Cal.dig_T1 / 8192.0)) * bmp280Cal.dig_T3;
    *temp = var1 + var2;
    *temp /= 5120.0;

    var1 = ((*temp) / 2.0) - 64000.0;
    var2 = var1 * var1*(bmp280Cal.dig_P6) / 32768.0;
    var2 = var2 + var1 * (bmp280Cal.dig_P5)*2.0;
    var2 = (var2 / 4.0) + ((bmp280Cal.dig_P4)*65536.0);
    var1 = (bmp280Cal.dig_P3)*var1*var1 / 524288.0 + (bmp280Cal.dig_P2)*var1 / 524288.0;
    var1 = (1.0 + var1 / 32768.0)*(bmp280Cal.dig_P1);

    *pres = 1048576.0 - (*pres);
    *pres = ((*pres) - (var2 / 4096.0))*6250.0 / var1;
    var1 = (bmp280Cal.dig_P9) * (*pres) * (*pres) / 2147483648.0;
    var2 = (*pres) * (bmp280Cal.dig_P8) / 32768.0;
    *pres = (*pres) + (var1 + var2 + (bmp280Cal.dig_P7)) / 16.0;
}
