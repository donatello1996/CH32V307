#ifndef _UART_H_
#define _UART_H_

#include <stdio.h>
#include <string.h>

#include "ch32v30x.h"

void UART1_Init(uint32_t baud);
void UART1_Send_Char(char c);
void UART1_Send_String(char s[]);

void UART2_Init(int baud , int flag_idle_dma);
void UART2_Send_Char(char c);
void UART2_Send_String(char s[]);

#endif