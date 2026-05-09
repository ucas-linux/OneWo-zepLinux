/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_spi.c
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:			2023/12/24
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2023/12/24     1.0     build this module
 * Description:     This file provides firmware functions to manage the functionalities of the SPI peripheral.
 */

#include "as32x601_spi.h"

/*
 * Function:        SPI_StructInit
 * Description:     Fills each SPI_InitStruct member with its default value.
 * Param:           SPI_InitStruct : pointer to a SPI_InitTypeDef structure which will be initialized.
 * Return:          None
 */
void SPI_StructInit(SPI_InitTypeDef *SPI_InitStruct)
{
    /* Reset SPI init structure parameters values */
    SPI_InitStruct->SPI_CLKDiv       = SPI_CLKDiv2;         /* Specifies the clock frequency division */
    SPI_InitStruct->SPI_Mode         = SPI_Mode_Master;     /* Specifies the SPI operating mode. */
    SPI_InitStruct->SPI_FrameSize    = SPI_FrameSize_8b;    /* Specifies the SPI data size. */
    SPI_InitStruct->SPI_MotorolaMode = SPI_Motorola_Mode0;  /* Select the specifies SPI motorola mode */
    SPI_InitStruct->SPI_TransMode    = SPI_TransNor;        /* Specifies the SPI transmission mode. */
}

/*
 * Function:        SPI_Init
 * Description:     Initializes the SPIx peripheral according to the specified parameters in the SPI_InitStruct.
 * Param:           SPIx: where x can be (0..5) to select the SPI peripheral.
 *                  SPI_InitStruct: pointer to a SPI_InitTypeDef structure that contains the configuration information for the specified SPI peripheral.
 * Return:          None
 */
void SPI_Init(SPI_TypeDef *SPIx, SPI_InitTypeDef *SPI_InitStruct)
{
    uint32_t tmpreg = 0x00;

    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_CLKDIV(SPI_InitStruct->SPI_CLKDiv));
    assert_param(IS_SPI_Mode(SPI_InitStruct->SPI_Mode));
    assert_param(IS_SPI_FRAMESIZE(SPI_InitStruct->SPI_FrameSize));
    assert_param(IS_SPI_MOTOROLAMODE(SPI_InitStruct->SPI_MotorolaMode));
    assert_param(IS_SPI_TRANSMODE(SPI_InitStruct->SPI_TransMode));

    /* Set the SPI clock frequency division */
    tmpreg = SPIx->CLKCR;
    tmpreg &= ~((uint32_t)(SPI_CLKCR_CLKCR));
    tmpreg |= ((uint32_t)(SPI_InitStruct->SPI_CLKDiv));
    SPIx->CLKCR = tmpreg;

    tmpreg = 0;
    /* Set the SPI mode */
    tmpreg = SPIx->CR0;
    tmpreg &= ~((uint32_t)(SPI_CR0_MSTR));
    tmpreg |= ((uint32_t)(SPI_InitStruct->SPI_Mode));
    SPIx->CR0 = tmpreg;

    /* Set the SPI frame size,and  motorola mode and trans mode */
    SPIx->CFG = ((uint32_t)(SPI_InitStruct->SPI_FrameSize | SPI_InitStruct->SPI_MotorolaMode |
                            SPI_InitStruct->SPI_TransMode));
}

/*
 * Function:        SPI_Cmd
 * Description:     Enables or disables the specified SPIx.
 * Param:           SPIx: where x can be (0..5) to select the SPI peripheral.
 *                  NewState: new state of the SPIx peripheral.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void SPI_Cmd(SPI_TypeDef *SPIx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_EnState_STATE(NewState));

    if (NewState != DISABLE) 
    {
        SPIx->CR0 |= ((uint32_t)(SPI_CR0_SPE));
    }
    else
    {
        SPIx->CR0 &= ~((uint32_t)(SPI_CR0_SPE));
    }
}

/*
 * Function:        SPI_ResetFIFO
 * Description:     Resets FIFO of the specified SPIx.
 * Param:           SPIx: where x can be (0..5) to select the SPI peripheral.
 *                  SPI_FIFOType: Reset the type of specified SPI FIFO.
 *                   This parameter can be: SPI_ResetFIFO_Rx or SPI_ResetFIFO_Tx or SPI_ResetFIFO_TxandRx.
 * Return:          None.
 */
void SPI_ResetFIFO(SPI_TypeDef *SPIx, SPIResetFIFO_TypeDef SPI_FIFOType)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_RESETFIFO(SPI_FIFOType));

    SPIx->CR1 = SPI_FIFOType;
}

/*
 * Function:        SPI_NSSConfig
 * Description:     Sets or resets the specified SPI slave selection.
 * Param:           SPIx: where x can be (0..5) to select the SPI peripheral.
 *                  Slavex: select the SPI slaves.
 *                   This parameter can be one of the following values:
 *                      SPI_Slave0: Selection slave 0
 *                      SPI_Slave1: Selection slave 1
 *                      SPI_Slave2: Selection slave 2
 *                      SPI_Slave3: Selection slave 3
 *                  NewState: new state of the specified SPIx slave selection.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void SPI_NSSConfig(SPI_TypeDef *SPIx, SPISlaveSel_TypeDef Slavex, FunctionalState NewState)
{
    uint32_t tmpreg = 0x00;

    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_SLAVESEL(Slavex));
    assert_param(IS_EnState_STATE(NewState));

    tmpreg = SPIx->SSR;
    tmpreg &= ~((uint32_t)(SPI_SSR_SSR));

    if (NewState != DISABLE) 
    {
        tmpreg |= ((uint32_t)Slavex);
    } 
    else 
    {
        tmpreg &= ~((uint32_t)Slavex);
    }
    SPIx->SSR = tmpreg;
}

/*
 * Function:        SPI_SendData
 * Description:     Transmits single data through the SPIx peripheral.
 * Param:           SPIx: where x can be (0..5) to select the SPI peripheral.
 *                  Data: the data to transmit.
 * Return:          None.
 */
void SPI_SendData(SPI_TypeDef *SPIx, uint32_t Data)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    /* Transmit data */
    SPIx->TXDR = Data;
}

/*
 * Function:        SPI_SendDataLast
 * Description:     Transmits last data through the SPIx peripheral.
 * Param:           SPIx: where x can be (0..5) to select the SPI peripheral.
 *                  Data: the data to transmit.
 * Return:          None.
 */
void SPI_SendDataLast(SPI_TypeDef *SPIx, uint32_t DataLast)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    /* Transmit last data */
    SPIx->TXEDR = DataLast;
}

/*
 * Function:        SPI_ReceiveData
 * Description:     Returns the most recent received data by the SPIx peripheral.
 * Param:           SPIx: where x can be (0..5) to select the SPI peripheral.
 * Return:          The received data.
 */
uint32_t SPI_ReceiveData(SPI_TypeDef *SPIx)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    /* Receive data */
    return (uint32_t)SPIx->RXDR;
}

/*
 * Function:        SPI_ITConfig
 * Description:     Enables or disables the specified SPI interrupts.
 * Param:           SPIx: where x can be (0..5) to select the SPI peripheral.
 *                  SPI_IT: specifies the SPI interrupt source to be enabled or disabled.
 *                   This parameter can be one of the following values:
 *                      SPI_IT_TD: Tx done interrupt
 *                      SPI_IT_RXO:Rx buffer overflow interrupt
 *                      SPI_IT_TXE:Tx buffer empty interrupt
 *                      SPI_IT_NSSV:NSS Voh interrupt
 *                      SPI_IT_RXNE:Rx buffer non-empty interrupt
 *                      SPI_IT_TXNE:Tx buffer non-full interrupt
 *                  NewState: new state of the specified SPIx peripheral.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void SPI_ITConfig(SPI_TypeDef *SPIx, uint32_t SPI_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_CONFIG_IT(SPI_IT));
    assert_param(IS_EnState_STATE(NewState));

    /* Select the register of the operation according to the interrupt type */
    if (NewState != DISABLE) 
    {
        /* Enable the interrupt */
        if(((SPI_IT & SPI_CR_MASK) >> SPI_CR_POS) == 1U)
        {
            SPIx->CR0 |= ((uint32_t)(1U << (SPI_IT & SPI_IT_MASK)));
        }
        else
        {
            SPIx->IER |= ((uint32_t)(1U << (SPI_IT & SPI_IT_MASK)));
        }
    } 
    else 
    {
        /* Disable the interrupt */
        if(((SPI_IT & SPI_CR_MASK) >> SPI_CR_POS) == 1U)
        {
            SPIx->CR0 &= ~((uint32_t)(1U << (SPI_IT & SPI_IT_MASK)));
        }
        else
        {
            SPIx->IER &= ~((uint32_t)(1U << (SPI_IT & SPI_IT_MASK)));
        }
    }
}

/*
 * Function:        SPI_GetITStatus
 * Description:     Checks whether the specified SPI interrupt status is set or not.
 * Param:           SPIx: where x can be (0..5) to select the SPI peripheral.
 *                  SPI_ITSTA: specifies the interrupt status to check.
 *                   This parameter can be one of the following values:
 *                      SPI_ITStatus_TD: Tx done interrupt status
 *                      SPI_ITStatus_RXO: Rx buffer overflow interrupt status
 *                      SPI_ITStatus_TXE: Tx buffer empty interrupt status
 *                      SPI_ITStatus_NSSV: NSS Voh interrupt status
 *                      SPI_ITStatus_RXNE: Rx buffer non-empty interrupt status
 *                      SPI_ITStatus_TXNF: Tx buffer non-full interrupt status
 * Return:          The state of SPI_ITSTA (SET or RESET).
 */
ITStatus SPI_GetITStatus(SPI_TypeDef *SPIx, uint32_t SPI_ITSTA)
{
    ITStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_GETSTA_IT(SPI_ITSTA));

    if ((SPIx->ISR & SPI_ITSTA) != (uint32_t)RESET) 
    {
        bitstatus = SET;
    } 
    else 
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/*
 * Function:        SPI_GetITRawStatus
 * Description:     Checks whether the specified SPI interrupt raw status is set or not.
 * Param:           SPIx: where x can be (0..5) to select the SPI peripheral.
 *                  SPI_ITRaw: specifies the interrupt raw status to check.
 *                   This parameter can be one of the following values:
 *                      SPI_ITRaw_TD: Tx done interrupt raw status
 *                      SPI_ITRaw_RXO: Rx buffer overflow interrupt raw status
 *                      SPI_ITRaw_TXE: Tx buffer empty interrupt raw status
 *                      SPI_ITRaw_NSSV: NSS Voh interrupt raw status
 *                      SPI_ITRaw_RXNE: Rx buffer non-empty interrupt raw status
 *                      SPI_ITRaw_TXNF: Tx buffer non-full interrupt raw status
 * Return:          The state of SPI_ITRaw (SET or RESET).
 */
ITStatus SPI_GetITRawStatus(SPI_TypeDef *SPIx, uint32_t SPI_ITRaw)
{
    ITStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_GETRAW_IT(SPI_ITRaw));

    if ((SPIx->IRSR & SPI_ITRaw) != (uint32_t)RESET) 
    {
        bitstatus = SET;
    } 
    else 
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/*
 * Function:        SPI_GetFlagStatus
 * Description:     Checks whether the specified SPI flag is set or not.
 * Param:           SPIx: where x can be (0..5) to select the SPI peripheral.
 *                  SPI_FLAG: specifies the flag to check.
 *                   This parameter can be one of the following values:
 *                      SPI_Flag_NFRX: NSS is effective, the next frame in the receiving buffer is received for the first time
 *                      SPI_Flag_DONE: No request for receiving and sending
 *                      SPI_Flag_RXE: Rx buffer is empty
 *                      SPI_Flag_TXF: Tx buffer is full
 *                      SPI_Flag_RXO: Rx buffer is overflow
 *                      SPI_Flag_TXE: Tx buffer is empty
 *                      SPI_Flag_NSSS: The state of select slave
 *                      SPI_Flag_BUSS: SPI is still transmitting data
 * Return:          The state of SPI_FLAG (SET or RESET).
 */
FlagStatus SPI_GetFlagStatus(SPI_TypeDef *SPIx, uint32_t SPI_FLAG)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_FLAG(SPI_FLAG));

    if ((SPIx->SR & SPI_FLAG) != (uint32_t)RESET) 
    {
        bitstatus = SET;
    } 
    else 
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/*
 * Function:        SPI_ClearITPendingBit
 * Description:     Clears the SPIx's interrupt pending bits.
 * Param:           SPIx: where x can be (0..5) to select the SPI peripheral.
 *                  SPI_ITSTA: specifies the interrupt pending bit to clear.
 *                   This parameter can be one of the following values:
 *                      SPI_CLEAR_TD: Tx done clear interrupt status
 *                      SPI_CLEAR_RXO: Rx buffer overflow clear interrupt status
 *                      SPI_CLEAR_TXE: Tx buffer empty clear interrupt status
 *                      SPI_CLEAR_NSSV: NSS Voh clear interrupt status
 *                      SPI_CLEAR_RXNE: Rx buffer non-empty clear interrupt status
 *                      SPI_CLEAR_TXNF: Tx buffer non-full clear interrupt status
 * Return:          None.
 */
void SPI_ClearITPendingBit(SPI_TypeDef *SPIx, uint32_t SPI_ITSTA)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_CLEAR_IT(SPI_ITSTA));

    SPIx->IFCR = (uint32_t)SPI_ITSTA;
}

/*****END OF FILE*****/
