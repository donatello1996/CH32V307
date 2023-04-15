#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <string.h>

#include "ch32v30x.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define DEBUG_UART1    1
#define DEBUG_UART2    2

#define DEBUG   DEBUG_UART1


void Delay_Init(void);
void Delay_us (uint32_t n);
void Delay_ms (uint32_t n);
void USART_Printf_Init(uint32_t baudrate);

#ifdef __cplusplus
}
#endif

#endif 



