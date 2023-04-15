#include "debug.h"

static uint8_t  p_us = 0;
static uint16_t p_ms = 0;


void Delay_Init(void)
{
    p_us = SystemCoreClock / 7600000;
    p_ms = (uint16_t)p_us * 1000;

}

void Delay_us(uint32_t n)
{
    uint32_t i;

    SysTick->SR &= ~(1 << 0);
    i = (uint32_t)n * p_us;

    SysTick->CMP = i;
    SysTick->CTLR |= (1 << 4);
    SysTick->CTLR |= (1 << 5) | (1 << 0);

    while((SysTick->SR & (1 << 0)) != (1 << 0))
        ;
    SysTick->CTLR &= ~(1 << 0);
}

void Delay_ms(uint32_t n)
{
    uint32_t i;

    SysTick->SR &= ~(1 << 0);
    i = (uint32_t)n * p_ms;

    SysTick->CMP = i;
    SysTick->CTLR |= (1 << 4);
    SysTick->CTLR |= (1 << 5) | (1 << 0);

    while((SysTick->SR & (1 << 0)) != (1 << 0))
        ;
    SysTick->CTLR &= ~(1 << 0);
}

__IO uint32_t g_u32SystickCount = 0;

uint32_t GetTick(void)
{
    printf("GetTick = %d\n" , g_u32SystickCount);
	return g_u32SystickCount;
}

__attribute__((interrupt(), weak)) void SysTick_Handler(void)
{
    g_u32SystickCount ++;
}