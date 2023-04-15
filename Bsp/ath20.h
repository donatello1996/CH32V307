#ifndef _ATH20_H_
#define _ATH20_H_
#include "i2c.h"

#define ATH20_SLAVE_ADDRESS     0x38

#define	INIT		            0xBE
#define	SoftReset		        0xBA
#define	StartTest		        0xAC

uint8_t ATH20_Init(void);
uint8_t ATH20_Read_Cal_Enable(void); 
void ATH20_Read_CTdata(uint32_t *ct);

#endif
