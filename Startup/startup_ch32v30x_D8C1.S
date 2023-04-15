/********************************** (C) COPYRIGHT *******************************
* File Name          : startup_ch32v30x_D8C.s
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : CH32V307-CH32V305 vector table for eclipse toolchain.
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

.section .vectors,"ax",@progbits
.global  _start
.align   4
_start:
    j Reset_Handler     /* Go! */

 .align 8
 .option push
 .option norvc
_vectors:
    .word Fault_Handler              /*   0: Exception                     */
    .word Default_Handler            /*   1: Reserved                      */
    .word NMI_Handler                /*   2: NMI                           */
    .word Default_Handler            /*   3: Reserved                      */
    .word Default_Handler            /*   4: Reserved                      */
    .word Ecall_M_Handler            /*   5: M mode Ecall                  */
    .word Default_Handler            /*   6: Reserved                      */
    .word Default_Handler            /*   7: Reserved                      */
    .word Ecall_U_Handler            /*   8: U mode Ecall                  */
    .word Default_Handler            /*   9: Reserved                      */
    .word Default_Handler            /*  10: Reserved                      */
    .word Default_Handler            /*  11: Reserved                      */
    .word SysTick_Handler            /*  12: SysTick                       */
    .word Default_Handler            /*  13: Reserved                      */
    .word SW_Handler                 /*  14: Software                      */
    .word Default_Handler            /*  15: Reserved                      */
    .word WWDG_IRQHandler            /*  16: Window Watchdog               */
    .word PVD_IRQHandler             /*  17: PVD through EXTI Line detect  */
    .word TAMPER_IRQHandler          /*  18: TAMPER                        */
    .word RTC_IRQHandler             /*  19: RTC                           */
    .word FLASH_IRQHandler           /*  20: Flash                         */
    .word RCC_IRQHandler             /*  21: RCC                           */
    .word EXTI0_IRQHandler           /*  22: EXTI Line 0                   */
    .word EXTI1_IRQHandler           /*  23: EXTI Line 1                   */
    .word EXTI2_IRQHandler           /*  24: EXTI Line 2                   */
    .word EXTI3_IRQHandler           /*  25: EXTI Line 3                   */
    .word EXTI4_IRQHandler           /*  26: EXTI Line 4                   */
    .word DMA1_Channel1_IRQHandler   /*  27: DMA1 Channel 1                */
    .word DMA1_Channel2_IRQHandler   /*  28: DMA1 Channel 2                */
    .word DMA1_Channel3_IRQHandler   /*  29: DMA1 Channel 3                */
    .word DMA1_Channel4_IRQHandler   /*  30: DMA1 Channel 4                */
    .word DMA1_Channel5_IRQHandler   /*  31: DMA1 Channel 5                */
    .word DMA1_Channel6_IRQHandler   /*  32: DMA1 Channel 6                */
    .word DMA1_Channel7_IRQHandler   /*  33: DMA1 Channel 7                */
    .word ADC1_2_IRQHandler          /*  34: ADC1_2                        */
    .word USB_HP_CAN1_TX_IRQHandler  /*  35: USB HP and CAN1 TX            */
    .word USB_LP_CAN1_RX0_IRQHandler /*  36: USB LP and CAN1RX0            */
    .word CAN1_RX1_IRQHandler        /*  37: CAN1 RX1                      */
    .word CAN1_SCE_IRQHandler        /*  38: CAN1 SCE                      */
    .word EXTI9_5_IRQHandler         /*  39: EXTI Line 9..5                */
    .word TIM1_BRK_IRQHandler        /*  40: TIM1 Break                    */
    .word TIM1_UP_IRQHandler         /*  41: TIM1 Update                   */
    .word TIM1_TRG_COM_IRQHandler    /*  42: TIM1 Trigger and Commutation  */
    .word TIM1_CC_IRQHandler         /*  43: TIM1 Capture Compare          */
    .word TIM2_IRQHandler            /*  44: TIM2                          */
    .word TIM3_IRQHandler            /*  45: TIM3                          */
    .word TIM4_IRQHandler            /*  46: TIM4                          */
    .word I2C1_EV_IRQHandler         /*  47: I2C1 Event                    */
    .word I2C1_ER_IRQHandler         /*  48: I2C1 Error                    */
    .word I2C2_EV_IRQHandler         /*  49: I2C2 Event                    */
    .word I2C2_ER_IRQHandler         /*  50: I2C2 Error                    */
    .word SPI1_IRQHandler            /*  51: SPI1                          */
    .word SPI2_IRQHandler            /*  52: SPI2                          */
    .word USART1_IRQHandler          /*  53: USART1                        */
    .word USART2_IRQHandler          /*  54: USART2                        */
    .word USART3_IRQHandler          /*  55: USART3                        */
    .word EXTI15_10_IRQHandler       /*  56: EXTI Line 15..10              */
    .word RTCAlarm_IRQHandler        /*  57: RTC Alarm through EXTI Line   */
    .word USBWakeUp_IRQHandler       /*  58: USB Wakeup from suspend       */
    .word TIM8_BRK_IRQHandler        /*  59: TIM8 Break                    */
    .word TIM8_UP_IRQHandler         /*  60: TIM8 Update                   */
    .word TIM8_TRG_COM_IRQHandler    /*  61: TIM8 Trigger and Commutation  */
    .word TIM8_CC_IRQHandler         /*  62: TIM8 Capture Compare          */
    .word RNG_IRQHandler             /*  63: RNG                           */
    .word FSMC_IRQHandler            /*  64: FSMC                          */
    .word SDIO_IRQHandler            /*  65: SDIO                          */
    .word TIM5_IRQHandler            /*  66: TIM5                          */
    .word SPI3_IRQHandler            /*  67: SPI3                          */
    .word UART4_IRQHandler           /*  68: UART4                         */
    .word UART5_IRQHandler           /*  69: UART5                         */
    .word TIM6_IRQHandler            /*  70: TIM6                          */
    .word TIM7_IRQHandler            /*  71: TIM7                          */
    .word DMA2_Channel1_IRQHandler   /*  72: DMA2 Channel 1                */
    .word DMA2_Channel2_IRQHandler   /*  73: DMA2 Channel 2                */
    .word DMA2_Channel3_IRQHandler   /*  74: DMA2 Channel 3                */
    .word DMA2_Channel4_IRQHandler   /*  75: DMA2 Channel 4                */
    .word DMA2_Channel5_IRQHandler   /*  76: DMA2 Channel 5                */
    .word ETH_IRQHandler             /*  77: ETH                           */
    .word ETH_WKUP_IRQHandler        /*  78: ETH WakeUp                    */
    .word CAN2_TX_IRQHandler         /*  79: CAN2 TX                       */
    .word CAN2_RX0_IRQHandler        /*  80: CAN2 RX0                      */
    .word CAN2_RX1_IRQHandler        /*  81: CAN2 RX1                      */
    .word CAN2_SCE_IRQHandler        /*  82: CAN2 SCE                      */
    .word OTG_FS_IRQHandler          /*  83: OTGFS                         */
    .word USBHSWakeup_IRQHandler     /*  84: USBHS Wakeup                  */
    .word USBHS_IRQHandler           /*  85: USBHS                         */
    .word DVP_IRQHandler             /*  86: DVP                           */
    .word UART6_IRQHandler           /*  87: UART6                         */
    .word UART7_IRQHandler           /*  88: UART7                         */
    .word UART8_IRQHandler           /*  89: UART8                         */
    .word TIM9_BRK_IRQHandler        /*  90: TIM9 Break                    */
    .word TIM9_UP_IRQHandler         /*  91: TIM9 Update                   */
    .word TIM9_TRG_COM_IRQHandler    /*  92: TIM9 Trigger and Commutation  */
    .word TIM9_CC_IRQHandler         /*  93: TIM9 Capture Compare          */
    .word TIM10_BRK_IRQHandler       /*  94: TIM10 Break                   */
    .word TIM10_UP_IRQHandler        /*  95: TIM10 Update                  */
    .word TIM10_TRG_COM_IRQHandler   /*  96: TIM10 Trigger and Commutation */
    .word TIM10_CC_IRQHandler        /*  97: TIM10 Capture Compare         */
    .word DMA2_Channel6_IRQHandler   /*  98: DMA2 Channel 6                */
    .word DMA2_Channel7_IRQHandler   /*  99: DMA2 Channel 7                */
    .word DMA2_Channel8_IRQHandler   /* 100: DMA2 Channel 8                */
    .word DMA2_Channel9_IRQHandler   /* 101: DMA2 Channel 9                */
    .word DMA2_Channel10_IRQHandler  /* 102: DMA2 Channel 10               */
    .word DMA2_Channel11_IRQHandler  /* 103: DMA2 Channel 11               */
.option pop

.section .text,"ax",@progbits
Reset_Handler:

.option push 
.option    norelax
    la gp, __global_pointer$
.option    pop 

    la sp, _eusrstack 
copy_data:
    /* Load data section from flash to RAM */
    la a0, _sidata
    la a1, _sdata
    la a2, _edata
    bgeu a1, a2, clear_bss

loop_copy_data:
    lw t0, (a0)
    sw t0, (a1)
    addi a0, a0, 4
    addi a1, a1, 4
    bltu a1, a2, loop_copy_data

clear_bss:
    /* Clear bss section */
    la a0, _sbss
    la a1, _ebss
    bgeu a0, a1, setup_interrupts
loop_clear_bss:
    sw zero, (a0)
    addi a0, a0, 4
    bltu a0, a1, loop_clear_bss

setup_interrupts:
    li t0, 0x1f
    csrw 0xbc0, t0

    /* Enable nested and hardware stack */
    /* li t0, 0x1f */ /* For MRS proprietary GCC compilers */

    /* Enable nested interrupt, disable hardware stack */
    li t0, 0x1e /* Refer to RISC-V4 PFIC manual */

    csrw 0x804, t0

    /* FS: Initial, MPP: M mode, MPIE: EN, MIE: EN */
    li t0, 0x3888
    csrs mstatus, t0

    la t0, _vectors
    ori t0, t0, 3  /* PFIC exception handling */
    csrw mtvec, t0

    jal SystemInit
    jal main

dead_loop:
    j dead_loop
