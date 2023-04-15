#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "i2c.h"

#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define GYRO_XOUT_H 0x43
#define GYRO_YOUT_H 0x45
#define GYRO_ZOUT_H 0x47

#define MAG_XOUT_H		0x04
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_H		0x08

#define PWR_MGMT_1                  0x6B
#define SMPLRT_DIV                  0x19
#define CONFIG                      0x1A
#define GYRO_CONFIG                 0x1B
#define	ACCEL_CONFIG	            0x1C
#define INT_ENABLE 0x38

void MPU6050_Init();

#endif
