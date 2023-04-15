#ifndef AD7606_H
#define AD7606_H

#include <stdio.h>
#include <stdbool.h>

#include "ch32v30x.h"

void AD7606_PWM_ModeConfig(uint32_t frequency);

void AD7606_RD_CS_RST_Dataport_Init(void);

void AD7606_EXTI5_Int_Init(void);

#endif