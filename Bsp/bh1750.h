#ifndef _BH1750_H_
#define _BH1750_H_

#include "ch32v30x.h"
#include "i2c.h"

#define BHAddWrite     0x46
#define BHAddRead      0x47
#define BHPowDown      0x00
#define BHPowOn        0x01
#define BHReset        0x07
#define BHModeH1       0x10
#define BHModeH2       0x11
#define BHModeL        0x13
#define BHSigModeH     0x20
#define BHSigModeH2    0x21
#define BHSigModeL     0x23

void BH1750_Init();
void BH1750_Command_Send(uint8_t command);
uint16_t BH1750_Data_Read(void);

#endif
