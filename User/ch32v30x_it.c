#include "ch32v30x_it.h"

#define __IRQ       __attribute__((interrupt()))
#define __IRQ_WEAK  __attribute__((interrupt(), weak))
#define __IRQ_NAKED __attribute__((naked))


/**
 * FreeRTOS supports both non-vectored and vectored exception model.
 * For non-vectored exception, use `freertos_risc_v_trap_handler`,
 * this function will determine the type of current exception.
 * For vectored exception, use `freertos_risc_v_exception_handler`,
 * and use `freertos_risc_v_mtimer_interrupt_handler` for timer interrupt.
 * 
 */

/**
 * @brief Default Handler for exceptions and interrupts
 * 
 */
__IRQ_WEAK void Default_Handler(void) {
    for(;;) {
        /* Where are you from? */
    }
}

/**
 * @brief Fault handler
 * 
 */
__IRQ_WEAK void Fault_Handler(void) {
    for(;;) {
        /* Emmmmmmmmm? */
    }
}

/**
 * @brief U mode ecall handler
 * 
 */
__IRQ_WEAK void Ecall_U_Handler(void) {
    for(;;) {
        /* Who called me? */
    }
}

/**
 * @brief M mode ecall handler
 * 
 */
__IRQ_WEAK void Ecall_M_Handler(void) {
  /* M mode ecall handler */
}

/**
 * @brief Non-maskable interrupt handler
 *  
 */
__IRQ_WEAK void NMI_Handler(void) {
    /* NMI handler */
}

__IRQ_WEAK void SW_Handler(void) {
    /* Software handler */
}

__IRQ_WEAK void WWDG_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void PVD_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TAMPER_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void RTC_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void FLASH_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void RCC_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void EXTI0_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void EXTI1_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void EXTI2_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void EXTI3_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void EXTI4_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA1_Channel1_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA1_Channel2_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA1_Channel3_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA1_Channel4_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA1_Channel5_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA1_Channel6_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA1_Channel7_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void ADC1_2_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void USB_HP_CAN1_TX_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void USB_LP_CAN1_RX0_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void CAN1_RX1_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void CAN1_SCE_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void EXTI9_5_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM1_BRK_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM1_UP_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM1_TRG_COM_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM1_CC_IRQHandler(void) {
    /**/
}

__IRQ_WEAK void TIM3_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM4_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void I2C1_EV_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void I2C1_ER_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void I2C2_EV_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void I2C2_ER_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void SPI1_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void SPI2_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void USART1_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void USART2_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void USART3_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void EXTI15_10_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void RTCAlarm_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void USBWakeUp_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM8_BRK_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM8_UP_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM8_TRG_COM_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM8_CC_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void RNG_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void FSMC_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void SDIO_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM5_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void SPI3_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void UART4_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void UART5_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM6_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM7_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA2_Channel1_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA2_Channel2_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA2_Channel3_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA2_Channel4_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA2_Channel5_IRQHandler(void) {
    /**/
}

__IRQ_WEAK void CAN2_TX_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void CAN2_RX0_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void CAN2_RX1_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void CAN2_SCE_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void OTG_FS_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void USBHSWakeup_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void USBHS_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DVP_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void UART6_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void UART7_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void UART8_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM9_BRK_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM9_UP_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM9_TRG_COM_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM9_CC_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM10_BRK_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM10_UP_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM10_TRG_COM_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void TIM10_CC_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA2_Channel6_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA2_Channel7_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA2_Channel8_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA2_Channel9_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA2_Channel10_IRQHandler(void) {
    /**/
}
__IRQ_WEAK void DMA2_Channel11_IRQHandler(void) {
    /**/
}

// void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
// void NMI_Handler(void)
// {
// }

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

void ETH_WKUP_IRQHandler()
{
    
}

void TIM2_IRQHandler()
{

}

void ETH_IRQHandler()
{
    
}