/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_plic.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2023/12/14
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2023/12/14     1.0     build this module
 * Description:     This file contains all the functions prototypes for the PLIC firmware library.
 */

#ifndef AS32X601_PLIC_H
#define AS32X601_PLIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601_plic_regs.h"

/*  */
extern void SetMstatusMie(void);
extern void ResetMstatusMie(void);

/*
 * @brief Interrupt source ID numer enumeration
 */
typedef enum {
    None        = 0x00,
    EFLASH_IRQn = 0x01,
    FCU_IRQn    = 0x02,
    DMA0_IRQn   = 0x03,
    DMA1_IRQn   = 0x04,
    TIM0_IRQn   = 0x05,
    CANFD0_IRQn = 0x06,
    CANFD1_IRQn = 0x07,
    ADC0_IRQn   = 0x08,
    ADC1_IRQn   = 0x09,
    GPIOA_IRQn  = 0x0A,
    GPIOB_IRQn  = 0x0B,
    GPIOC_IRQn  = 0x0C,
    PKA_IRQn    = 0x0D,
    SPACC_IRQn  = 0x0E,
    TRNG_IRQn   = 0x0F,
    TIM1_IRQn   = 0x10,
    CANFD2_IRQn = 0x11,
    CANFD3_IRQn = 0x12,
    ADC2_IRQn   = 0x13,
    GPIOD_IRQn  = 0x14,
    GPIOE_IRQn  = 0x15,
    GPIOF_IRQn  = 0x16,
    GPIOG_IRQn  = 0x17,
    MAC_IRQn    = 0x18,
    SPI0_IRQn   = 0x19,
    SPI1_IRQn   = 0x1A,
    SPI2_IRQn   = 0x1B,
    I2C0_IRQn   = 0x1C,
    I2C1_IRQn   = 0x1D,
    USART0_IRQn = 0x1E,
    USART1_IRQn = 0x1F,
    USART2_IRQn = 0x20,
    USART3_IRQn = 0x21,
    TIM2_IRQn   = 0x22,
    TIM3_IRQn   = 0x23,
    TIM4_IRQn   = 0x24,
    SPI3_IRQn   = 0x25,
    SPI4_IRQn   = 0x26,
    SPI5_IRQn   = 0x27,
    I2C2_IRQn   = 0x28,
    I2C3_IRQn   = 0x29,
    USART4_IRQn = 0x2A,
    USART5_IRQn = 0x2B,
    USART6_IRQn = 0x2C,
    USART7_IRQn = 0x2D,
    TIM5_IRQn   = 0x2E,
    TIM6_IRQn   = 0x2F,
    TIM7_IRQn   = 0x30,
    GPIOH_IRQn  = 0x31,
    RTC_IRQn    = 0x32
} PLICIRQn_TypeDef;
#define IS_PLIC_IRQn(IRQN)  (((IRQN) == None        ) || ((IRQN) == EFLASH_IRQn ) || \
                             ((IRQN) == FCU_IRQn    ) || ((IRQN) == DMA0_IRQn   ) || \
                             ((IRQN) == DMA1_IRQn   ) || ((IRQN) == TIM0_IRQn   ) || \
                             ((IRQN) == CANFD0_IRQn ) || ((IRQN) == CANFD1_IRQn ) || \
                             ((IRQN) == ADC0_IRQn   ) || ((IRQN) == ADC1_IRQn   ) || \
                             ((IRQN) == GPIOA_IRQn  ) || ((IRQN) == GPIOB_IRQn  ) || \
                             ((IRQN) == GPIOC_IRQn  ) || ((IRQN) == PKA_IRQn    ) || \
                             ((IRQN) == SPACC_IRQn  ) || ((IRQN) == TRNG_IRQn   ) || \
                             ((IRQN) == TIM1_IRQn   ) || ((IRQN) == CANFD2_IRQn ) || \
                             ((IRQN) == CANFD3_IRQn ) || ((IRQN) == ADC2_IRQn   ) || \
                             ((IRQN) == GPIOD_IRQn  ) || ((IRQN) == GPIOE_IRQn  ) || \
                             ((IRQN) == GPIOF_IRQn  ) || ((IRQN) == GPIOG_IRQn  ) || \
                             ((IRQN) == MAC_IRQn    ) || ((IRQN) == SPI0_IRQn   ) || \
                             ((IRQN) == SPI1_IRQn   ) || ((IRQN) == SPI2_IRQn   ) || \
                             ((IRQN) == I2C0_IRQn   ) || ((IRQN) == I2C1_IRQn   ) || \
                             ((IRQN) == USART0_IRQn ) || ((IRQN) == USART1_IRQn ) || \
                             ((IRQN) == USART2_IRQn ) || ((IRQN) == USART3_IRQn ) || \
                             ((IRQN) == TIM2_IRQn   ) || ((IRQN) == TIM3_IRQn   ) || \
                             ((IRQN) == TIM4_IRQn   ) || ((IRQN) == SPI3_IRQn   ) || \
                             ((IRQN) == SPI4_IRQn   ) || ((IRQN) == SPI5_IRQn   ) || \
                             ((IRQN) == I2C2_IRQn   ) || ((IRQN) == I2C3_IRQn   ) || \
                             ((IRQN) == USART4_IRQn ) || ((IRQN) == USART5_IRQn ) || \
                             ((IRQN) == USART6_IRQn ) || ((IRQN) == USART7_IRQn ) || \
                             ((IRQN) == TIM5_IRQn   ) || ((IRQN) == TIM6_IRQn   ) || \
                             ((IRQN) == TIM7_IRQn   ) || ((IRQN) == GPIOH_IRQn  ) || \
                             ((IRQN) == RTC_IRQn    ))

/*
 * @brief PLIC init structure definition
 */
typedef struct
{
    PLICIRQn_TypeDef PLIC_IRQChannel;   /* Interrupt source selection */
    uint8_t PLIC_IRQPriority;           /* Interrupt source priority selection, this value can be 0x00 - 0x3F,
                                           the larger value has higher priority */
    FunctionalState PLIC_IRQChannelCmd; /* Interrupt channel enable */
} PLIC_InitTypeDef;

/*
 * @brief PLIC EXTI structure definition
 */
typedef struct
{
    uint8_t IDBuffer[64];
    uint8_t Index;
} PLIC_EXTITypeDef;

/*
 * @brief Interrupt handler virtual function declaration
 */
extern void Default_Int_Handler(void);
extern void EFLASH_IRQ_Handler(void) __attribute__((weak));
extern void FCU_IRQ_Handler(void) __attribute__((weak));
extern void DMA0_IRQ_Handler(void) __attribute__((weak));
extern void DMA1_IRQ_Handler(void) __attribute__((weak));
extern void TIM0_IRQ_Handler(void) __attribute__((weak));
extern void CANFD0_IRQ_Handler(void) __attribute__((weak));
extern void CANFD1_IRQ_Handler(void) __attribute__((weak));
extern void ADC0_IRQ_Handler(void) __attribute__((weak));
extern void ADC1_IRQ_Handler(void) __attribute__((weak));
extern void GPIOA_IRQ_Handler(void) __attribute__((weak));
extern void GPIOB_IRQ_Handler(void) __attribute__((weak));
extern void GPIOC_IRQ_Handler(void) __attribute__((weak));
extern void PKA_IRQ_Handler(void) __attribute__((weak));
extern void SPACC_IRQ_Handler(void) __attribute__((weak));
extern void TRNG_IRQ_Handler(void) __attribute__((weak));
extern void TIM1_IRQ_Handler(void) __attribute__((weak));
extern void CANFD2_IRQ_Handler(void) __attribute__((weak));
extern void CANFD3_IRQ_Handler(void) __attribute__((weak));
extern void ADC2_IRQ_Handler(void) __attribute__((weak));
extern void GPIOD_IRQ_Handler(void) __attribute__((weak));
extern void GPIOE_IRQ_Handler(void) __attribute__((weak));
extern void GPIOF_IRQ_Handler(void) __attribute__((weak));
extern void GPIOG_IRQ_Handler(void) __attribute__((weak));
extern void MAC_IRQ_Handler(void) __attribute__((weak));
extern void SPI0_IRQ_Handler(void) __attribute__((weak));
extern void SPI1_IRQ_Handler(void) __attribute__((weak));
extern void SPI2_IRQ_Handler(void) __attribute__((weak));
extern void I2C0_IRQ_Handler(void) __attribute__((weak));
extern void I2C1_IRQ_Handler(void) __attribute__((weak));
extern void USART0_IRQ_Handler(void) __attribute__((weak));
extern void USART1_IRQ_Handler(void) __attribute__((weak));
extern void USART2_IRQ_Handler(void) __attribute__((weak));
extern void USART3_IRQ_Handler(void) __attribute__((weak));
extern void TIM2_IRQ_Handler(void) __attribute__((weak));
extern void TIM3_IRQ_Handler(void) __attribute__((weak));
extern void TIM4_IRQ_Handler(void) __attribute__((weak));
extern void SPI3_IRQ_Handler(void) __attribute__((weak));
extern void SPI4_IRQ_Handler(void) __attribute__((weak));
extern void SPI5_IRQ_Handler(void) __attribute__((weak));
extern void I2C2_IRQ_Handler(void) __attribute__((weak));
extern void I2C3_IRQ_Handler(void) __attribute__((weak));
extern void USART4_IRQ_Handler(void) __attribute__((weak));
extern void USART5_IRQ_Handler(void) __attribute__((weak));
extern void USART6_IRQ_Handler(void) __attribute__((weak));
extern void USART7_IRQ_Handler(void) __attribute__((weak));
extern void TIM5_IRQ_Handler(void) __attribute__((weak));
extern void TIM6_IRQ_Handler(void) __attribute__((weak));
extern void TIM7_IRQ_Handler(void) __attribute__((weak));
extern void GPIOH_IRQ_Handler(void) __attribute__((weak));
extern void RTC_IRQ_Handler(void) __attribute__((weak));


extern void MSoftWare_IRQ_Handler(void) __attribute__((weak));
extern void MTimer_IRQ_Handler(void) __attribute__((weak));
extern void InstAddrMisalign_Handler(void) __attribute__((weak));
extern void InstAccessFault_Handler(void) __attribute__((weak));
extern void IllegalInst_Handler(void) __attribute__((weak));
extern void Breakpoint_Handler(void) __attribute__((weak));
extern void LoadAddrMisalign_Handler(void) __attribute__((weak));
extern void LoadAccessFault_Handler(void) __attribute__((weak));
extern void StoreAMOAddrMisalign_Handler(void) __attribute__((weak));
extern void StoreAMOAccessFault_Handler(void) __attribute__((weak));
extern void ECall_Handler(void) __attribute__((weak));
extern void InstPageFault_Handler(void) __attribute__((weak));
extern void LoadPageFault_Handler(void) __attribute__((weak));
extern void StoreAMOPageFalut_Handler(void) __attribute__((weak));


/*
 * @brief Function declaration
 */
void PLIC_StructInit(PLIC_InitTypeDef *PLIC_InitStruct);
void PLIC_Init(PLIC_InitTypeDef *PLIC_InitStruct);
void PLIC_TrapHandler(uint32_t Mcause);

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
