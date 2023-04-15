#ifndef _PAJ7620_H_
#define _PAJ7620_H_
#include "ch32v30x.h"

#define PAJ7620_ADDR_BASE				0x00
#define PAJ7620_REGITER_BANK_SEL		(PAJ7620_ADDR_BASE + 0xEF)
#define PAJ7620_ID                      0x73
#define PAJ7620_VAL(val, maskbit)		( val << maskbit )
#define PAJ7620_BANK0					PAJ7620_VAL(0,0)

#endif
