#ifndef _BMP280_H_
#define _BMP280_H_

#include "ch32v30x.h"
#include "i2c.h"

#define ATH20_SLAVE_ADDRESS    				    0x38

#define BMP280_PRESSURE_OSR					    (BMP280_OVERSAMP_8X)
#define BMP280_TEMPERATURE_OSR				    (BMP280_OVERSAMP_16X)
#define BMP280_MODE							    (BMP280_PRESSURE_OSR << 2 | BMP280_TEMPERATURE_OSR << 5 | BMP280_NORMAL_MODE)

#define BMP280_SLAVE_ADDRESS_0x76		        0x76
#define BMP280_SLAVE_ADDRESS_0x77			    0x77		

/*calibration parameters */
#define BMP280_DIG_T1_LSB_REG                   0x88
#define BMP280_DIG_T1_MSB_REG                   0x89
#define BMP280_DIG_T2_LSB_REG                   0x8A
#define BMP280_DIG_T2_MSB_REG                   0x8B
#define BMP280_DIG_T3_LSB_REG                   0x8C
#define BMP280_DIG_T3_MSB_REG                   0x8D
#define BMP280_DIG_P1_LSB_REG                   0x8E
#define BMP280_DIG_P1_MSB_REG                   0x8F
#define BMP280_DIG_P2_LSB_REG                   0x90
#define BMP280_DIG_P2_MSB_REG                   0x91
#define BMP280_DIG_P3_LSB_REG                   0x92
#define BMP280_DIG_P3_MSB_REG                   0x93
#define BMP280_DIG_P4_LSB_REG                   0x94
#define BMP280_DIG_P4_MSB_REG                   0x95
#define BMP280_DIG_P5_LSB_REG                   0x96
#define BMP280_DIG_P5_MSB_REG                   0x97
#define BMP280_DIG_P6_LSB_REG                   0x98
#define BMP280_DIG_P6_MSB_REG                   0x99
#define BMP280_DIG_P7_LSB_REG                   0x9A
#define BMP280_DIG_P7_MSB_REG                   0x9B
#define BMP280_DIG_P8_LSB_REG                   0x9C
#define BMP280_DIG_P8_MSB_REG                   0x9D
#define BMP280_DIG_P9_LSB_REG                   0x9E
#define BMP280_DIG_P9_MSB_REG                   0x9F

#define BMP280_CHIPID_REG                    0xD0  /*Chip ID Register */
#define BMP280_RESET_REG                     0xE0  /*Softreset Register */
#define BMP280_STATUS_REG                    0xF3  /*Status Register */
#define BMP280_CTRLMEAS_REG                  0xF4  /*Ctrl Measure Register */
#define BMP280_CONFIG_REG                    0xF5  /*Configuration Register */
#define BMP280_PRESSURE_MSB_REG              0xF7  /*Pressure MSB Register */
#define BMP280_PRESSURE_LSB_REG              0xF8  /*Pressure LSB Register */
#define BMP280_PRESSURE_XLSB_REG             0xF9  /*Pressure XLSB Register */
#define BMP280_TEMPERATURE_MSB_REG           0xFA  /*Temperature MSB Reg */
#define BMP280_TEMPERATURE_LSB_REG           0xFB  /*Temperature LSB Reg */
#define BMP280_TEMPERATURE_XLSB_REG          0xFC  /*Temperature XLSB Reg */


#define BMP280_SLEEP_MODE				(0x00)
#define BMP280_FORCED_MODE				(0x01)
#define BMP280_NORMAL_MODE				(0x03)

#define BMP280_TEMPERATURE_CALIB_DIG_T1_LSB_REG             (0x88)
#define BMP280_PRESSURE_TEMPERATURE_CALIB_DATA_LENGTH       (24)
#define BMP280_DATA_FRAME_SIZE			(6)

#define BMP280_OVERSAMP_SKIPPED			(0x00)
#define BMP280_OVERSAMP_1X				(0x01)
#define BMP280_OVERSAMP_2X				(0x02)
#define BMP280_OVERSAMP_4X				(0x03)
#define BMP280_OVERSAMP_8X				(0x04)
#define BMP280_OVERSAMP_16X				(0x05)

typedef struct
{
    uint16_t dig_T1;/* calibration T1 data */
    int16_t dig_T2; /* calibration T2 data */
    int16_t dig_T3; /* calibration T3 data */
    uint16_t dig_P1;/* calibration P1 data */
    int16_t dig_P2; /* calibration P2 data */
    int16_t dig_P3; /* calibration P3 data */
    int16_t dig_P4; /* calibration P4 data */
    int16_t dig_P5; /* calibration P5 data */
    int16_t dig_P6; /* calibration P6 data */
    int16_t dig_P7; /* calibration P7 data */
    int16_t dig_P8; /* calibration P8 data */
    int16_t dig_P9; /* calibration P9 data */
    int t_fine; /* calibration t_fine data */
} bmp280Calib;

#define CONST_PF 0.1902630958	//(1/5.25588f) Pressure factor
#define FIX_TEMP 25				// Fixed Temperature. ASL is a function of pressure and temperature, but as the temperature changes so much (blow a little towards the flie and watch it drop 5 degrees) it corrupts the ASL estimates.
								// TLDR: Adjusting for temp changes does more harm than good.

uint8_t BMP280_Init(void);
float BMP280PressureToAltitude(float* pressure);
void BMP280_Read_Pressure_Tempature(float * pres , float * temp);

#endif