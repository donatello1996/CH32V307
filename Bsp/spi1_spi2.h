#ifndef _SPI1_SPI2_H_
#define _SPI1_SPI2_H_

#include "ch32v30x.h"

void SPI1_Init();
void SPI2_Init();

uint8_t SPI1_ReadWriteByte(uint8_t data);
uint8_t SPI2_ReadWriteByte(uint8_t data);

#endif