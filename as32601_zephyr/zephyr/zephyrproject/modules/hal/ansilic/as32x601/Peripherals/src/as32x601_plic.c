/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_plic.c
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2023/12/14
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2023/12/14     1.0     build this module
 * Description:     This file provides firmware functions to manage the functionalities of the PLIC module.
 */

#include "as32x601_plic.h"
#include "as32x601_core.h"

static void (*PLIC_IRQHandlers[65])() =
{
    Default_Int_Handler,
    EFLASH_IRQ_Handler,
    FCU_IRQ_Handler,
    DMA0_IRQ_Handler,
    DMA1_IRQ_Handler,
    TIM0_IRQ_Handler,
    CANFD0_IRQ_Handler,
    CANFD1_IRQ_Handler,
    ADC0_IRQ_Handler,
    ADC1_IRQ_Handler,
    GPIOA_IRQ_Handler,
    GPIOB_IRQ_Handler,
    GPIOC_IRQ_Handler,
    PKA_IRQ_Handler,
    SPACC_IRQ_Handler,
    TRNG_IRQ_Handler,
    TIM1_IRQ_Handler,
    CANFD2_IRQ_Handler,
    CANFD3_IRQ_Handler,
    ADC2_IRQ_Handler,
    GPIOD_IRQ_Handler,
    GPIOE_IRQ_Handler,
    GPIOF_IRQ_Handler,
    GPIOG_IRQ_Handler,
    MAC_IRQ_Handler,
    SPI0_IRQ_Handler,
    SPI1_IRQ_Handler,
    SPI2_IRQ_Handler,
    I2C0_IRQ_Handler,
    I2C1_IRQ_Handler,
    USART0_IRQ_Handler,
    USART1_IRQ_Handler,
    USART2_IRQ_Handler,
    USART3_IRQ_Handler,
    TIM2_IRQ_Handler,
    TIM3_IRQ_Handler,
    TIM4_IRQ_Handler,
    SPI3_IRQ_Handler,
    SPI4_IRQ_Handler,
    SPI5_IRQ_Handler,
    I2C2_IRQ_Handler,
    I2C3_IRQ_Handler,
    USART4_IRQ_Handler,
    USART5_IRQ_Handler,
    USART6_IRQ_Handler,
    USART7_IRQ_Handler,
    TIM5_IRQ_Handler,
    TIM6_IRQ_Handler,
    TIM7_IRQ_Handler,
    GPIOH_IRQ_Handler,
    RTC_IRQ_Handler,
    Default_Int_Handler,
    Default_Int_Handler,
    Default_Int_Handler,
    Default_Int_Handler,
    Default_Int_Handler,
    Default_Int_Handler,
    Default_Int_Handler,
    Default_Int_Handler,
    Default_Int_Handler,
    Default_Int_Handler,
    Default_Int_Handler,
    Default_Int_Handler,
    Default_Int_Handler,
    Default_Int_Handler
};

/*
 * Function:        PLIC_StructInit
 * Description:     Fills each PLIC_InitStruct member with its default value.
 * Param:           PLIC_InitStruct : pointer to a PLIC_InitTypeDef structure which will be initialized.
 * Return:          None
 */
void PLIC_StructInit(PLIC_InitTypeDef *PLIC_InitStruct)
{
    /* Reset PLIC init structure parameters values */

    PLIC_InitStruct->PLIC_IRQChannel    = None;    /* Interrupt source selection */
    PLIC_InitStruct->PLIC_IRQPriority   = 0x00;    /* Interrupt source priority selection */
    PLIC_InitStruct->PLIC_IRQChannelCmd = DISABLE; /* Interrupt channel enale */
}

/*
 * Function:        PLIC_Init
 * Description:     Initializes the PLIC peripheral according to the specified parameters in the PLIC_InitStruct.
 * Param:           PLIC_InitStruct: pointer to a PLIC_InitTypeDef structure that contains the configuration information for the specified PLIC peripheral.
 * Return:          None
 */
void PLIC_Init(PLIC_InitTypeDef *PLIC_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_PLIC_IRQn((PLIC_InitTypeDef)PLIC_InitStruct->PLIC_IRQChannel));

    if (PLIC_InitStruct->PLIC_IRQChannelCmd != DISABLE) 
    {
        /* Interrupt source priority selection */
        PLIC->AXI_PLIC_PRIORITY[PLIC_InitStruct->PLIC_IRQChannel - 1] = PLIC_InitStruct->PLIC_IRQPriority;
        /* Enable the interrupt channel */
        if (PLIC_InitStruct->PLIC_IRQChannel < 33) 
        {
            PLIC->AXI_PLIC_EN[0] |= ((uint32_t)0x01 << (PLIC_InitStruct->PLIC_IRQChannel - 1));
        } 
        else 
        {
            PLIC->AXI_PLIC_EN[1] |= ((uint32_t)0x01 << (PLIC_InitStruct->PLIC_IRQChannel - 33));
        }
    } 
    else 
    {
         /* Disable the interrupt channel */
        if (PLIC_InitStruct->PLIC_IRQChannel < 33) 
        {
            PLIC->AXI_PLIC_EN[0] &= ~((uint32_t)0x01 << (PLIC_InitStruct->PLIC_IRQChannel - 1));
        } 
        else 
        {
            PLIC->AXI_PLIC_EN[1] &= ~((uint32_t)0x01 << (PLIC_InitStruct->PLIC_IRQChannel - 33));
        }
    }
}
// #include "myprintf.h"
/*
 * Function:        PLIC_TrapHandler
 * Description:     Interrupt handler type selection.
 * Param:           Mcause: determine the type of exception or interrupt based on the value of the mcause register.
 * Return:          None
 */
void PLIC_TrapHandler(uint32_t Mcause)
{
    if((Mcause & 0x80000000) != 0)
    {
        switch (Mcause & 0x0fff) 
        {
            case 3: /* Machine software interrupt */
                MSoftWare_IRQ_Handler();
                break;
            case 7: /* Machine timer interrupt */
                MTimer_IRQ_Handler();
                break;
            case 11: /* Machine external interrupt */
                /* Zephyr handles machine external interrupts through _isr_wrapper. */
                break;
            default:
                break;
        }
    }
    else
    {
        switch (Mcause & 0xfff) 
        {   
            case 0: /* Instruction address misaligned */
                InstAddrMisalign_Handler();
                break;
            case 1: /* Instruction access fault */
                InstAccessFault_Handler();
                break;
            case 2: /* Illegal instruction */
                IllegalInst_Handler();
                break;
            case 3: /* Breakpoint */
                Breakpoint_Handler();
                break;
            case 4: /* Load address misaligned */
                LoadAddrMisalign_Handler();
                break;
            case 5: /* Load access fault */
                LoadAccessFault_Handler();
                break;
            case 6: /* Store/AMO address misaligned */
                StoreAMOAddrMisalign_Handler();
                break;
            case 7: /* Store/AMO access fault */
                StoreAMOAccessFault_Handler();
                break;
            case 11: /* Environment call from M-mode */
                ECall_Handler();
                break;
            case 12: /* Instruction page fault */
                InstPageFault_Handler();
                break;
            case 13: /* Load page fault */
                LoadPageFault_Handler();
                break;
            case 15: /* Store/AMO page fault */
                StoreAMOPageFalut_Handler();
                break;
            default:
                break;
        }
    }
}

/*****END OF FILE*****/
