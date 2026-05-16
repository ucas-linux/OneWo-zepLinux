/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_usart.c
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:			2024/08/01
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/08/01     1.0     build this module
 * Description:     This file provides firmware functions to manage the functionalities of the USART peripheral.
 */

#include <stdlib.h>
#include "as32x601_usart.h"
#include "as32x601_smu.h"

/*
 * @brief  USART_Private_Constants USART Private Constants
 */
#define CPU_APB_HZ_TEMP		(20000000U)  		/* CPU APB frequency(20000000Hz) */
#define USART_DUMMY_DATA	((uint16_t) 0xFFFF)	/* USART transmitted dummy data */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* USART CR1 register clear Mask ((~(uint32_t)0xE9F3)) */
#define CR1_CLEAR_MASK            ((uint32_t)(USART_CR1_M | USART_CR1_PCE | \
                                              USART_CR1_PS | USART_CR1_TE | \
                                              USART_CR1_RE))

/* USART CR2 register clock bits clear Mask ((~(uint32_t)0xF0FF)) */
#define CR2_CLOCK_CLEAR_MASK      ((uint32_t)(USART_CR2_CLKEN | USART_CR2_CPOL | \
                                              USART_CR2_CPHA | USART_CR2_LBCL))

/* USART CR3 register clear Mask ((~(uint32_t)0xFCFF)) */
#define CR3_CLEAR_MASK            ((uint32_t)(USART_CR3_RTSE | USART_CR3_CTSE))

/*
 * Function:        USART_Init
 * Description:     Initialize the USART mode according to the specified.
 *					parameters in the USART_InitTypeDef and initialize the associated handle.
 * Param:           USARTx: where x can be (0..7) to select the USART peripheral.
 *                  USART_InitStruct: pointer to a USART_InitTypeDef structure that contains
 *                  the configuration information for the specified. USART peripheral.
 * Return:          None.
 */
void USART_Init(USART_TypeDef *USARTx, USART_InitTypeDef *USART_InitStruct)
{
    uint32_t tmpreg = 0x00;
    uint32_t apbclock = CPU_APB_HZ_TEMP;
    
    
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_BAUDRATE(USART_InitStruct->USART_BaudRate));
    assert_param(IS_USART_WORD_LENGTH(USART_InitStruct->USART_WordLength));
    assert_param(IS_USART_STOPBITS(USART_InitStruct->USART_StopBits));
    assert_param(IS_USART_PARITY(USART_InitStruct->USART_Parity));
    assert_param(IS_USART_MODE(USART_InitStruct->USART_Mode));
    assert_param(IS_USART_OVERSAMPLING(USART_InitStruct->USART_OverSampling));
    assert_param(IS_USART_HARDWARE_FLOW_CONTROL(USART_InitStruct->USART_HardwareFlowControl));

     /* Get the USART Bus Clock */
    if((USARTx == USART0) || (USARTx == USART1) || (USARTx == USART2) || (USARTx == USART3))
    {
        apbclock = SMU_ClocksStruct.APBBus0_Frequency;
    }
    else
    {
        apbclock = SMU_ClocksStruct.APBBus1_Frequency;
    }

    
    /* Disable the Peripheral */
    USARTx->CR1 &= ~((uint32_t)USART_CR1_UE);

    tmpreg = USARTx->CR2;

    /* Clear STOP[13:12] bits */
    tmpreg &= ~((uint32_t)USART_CR2_STOP);

    /* Configure the USART Stop Bits :
        Set STOP[13:12] bits according to USART_StopBits value */
    tmpreg |= ((uint32_t)USART_InitStruct->USART_StopBits);
  
    /* Write to USART CR2 */
    USARTx->CR2 = (uint32_t)tmpreg;

    tmpreg = 0;

    tmpreg = USARTx->CR1;

    /* Clear M, PCE, PS, TE and RE bits */
    tmpreg &= ~((uint32_t)CR1_CLEAR_MASK);

    /* Configure the USART Word Length, Parity and mode: 
        Set the M bits according to USART_WordLength value 
        Set PCE and PS bits according to USART_Parity value
        Set TE and RE bits according to USART_Mode value */
    tmpreg |= (uint32_t)USART_InitStruct->USART_WordLength | USART_InitStruct->USART_Parity |
            USART_InitStruct->USART_Mode;

    /* Select the Over Sampling */
	tmpreg &= ~((uint32_t)(USART_CR1_OVER8));
	tmpreg |= ((uint32_t)(USART_InitStruct->USART_OverSampling));

    /* Write to USART CR1 */
    USARTx->CR1 = (uint32_t)tmpreg;

    tmpreg = 0;

    tmpreg = USARTx->CR3;

    /* Clear CTSE and RTSE bits */
    tmpreg &= ~((uint32_t)CR3_CLEAR_MASK);

    /* Configure the USART HFC : 
        Set CTSE and RTSE bits according to USART_HardwareFlowControl value */
    tmpreg |= USART_InitStruct->USART_HardwareFlowControl;

    /* Write to USART CR3 */
    USARTx->CR3 = (uint32_t)tmpreg;

    USARTx->BRR = ((uint16_t)((apbclock)/(USART_InitStruct->USART_BaudRate)));

	if (USART_InitStruct->USART_AdvancedInit.USART_AdvFeatureInit != USART_ADVFeature_No_Init)
	{
		USART_AdvFeatureConfig(USARTx, &USART_InitStruct->USART_AdvancedInit);
	}
	else
	{
		/* Nothing to do */
	}
}

/*
 * Function:        USART_StructInit
 * Description:     Fills each USART_InitStruct member with its default value.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral.
 *                  USART_InitStruct: pointer to a USART_InitTypeDef structure that contains
 *                  the configuration information for the specified USART peripheral.
 * Return:          None.
 */
void USART_StructInit(USART_InitTypeDef* USART_InitStruct)
{
    /* USART_InitStruct members default value */
    USART_InitStruct->USART_BaudRate = 9600;
    USART_InitStruct->USART_WordLength = USART_WordLength_8b;
    USART_InitStruct->USART_StopBits = USART_StopBits_1;
    USART_InitStruct->USART_Parity = USART_Parity_No;
    USART_InitStruct->USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct->USART_HardwareFlowControl = USART_HWControl_No;
    USART_InitStruct->USART_OverSampling = USART_OverSampling_16;
    USART_InitStruct->USART_AdvancedInit.USART_AdvFeatureInit = USART_ADVFeature_No_Init;
}

/*
 * Function:        USART_ClockInit
 * Description:     Initializes the USARTx peripheral Clock according to the 
 *                  specified parameters in the USART_ClockInitStruct.
 * Param:           USARTx: where x can be (0..7) to select the USART peripheral.
 *                  USART_ClockInitStruct: pointer to a USART_ClockInitTypeDef structure that
 *                  contains the configuration information for the specified USART peripheral.
 * Return:          None.
 */
void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitTypeDef* USART_ClockInitStruct)
{
    uint32_t tmpreg = 0x00;
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_CLOCK(USART_ClockInitStruct->USART_Clock));
    assert_param(IS_USART_CPOL(USART_ClockInitStruct->USART_CPOL));
    assert_param(IS_USART_CPHA(USART_ClockInitStruct->USART_CPHA));
    assert_param(IS_USART_LASTBIT(USART_ClockInitStruct->USART_LastBit));

    tmpreg = USARTx->CR2;
    /* Clear CLKEN, CPOL, CPHA and LBCL bits */
    tmpreg &= ~((uint32_t)CR2_CLOCK_CLEAR_MASK);
    /* Configure the USART Clock, CPOL, CPHA and LastBit ------------*/
    /* Set CLKEN bit according to USART_Clock value */
    /* Set CPOL bit according to USART_CPOL value */
    /* Set CPHA bit according to USART_CPHA value */
    /* Set LBCL bit according to USART_LastBit value */
    tmpreg |= (uint32_t)USART_ClockInitStruct->USART_Clock | USART_ClockInitStruct->USART_CPOL | 
                    USART_ClockInitStruct->USART_CPHA | USART_ClockInitStruct->USART_LastBit;
    /* Write to USART CR2 */
    USARTx->CR2 = (uint32_t)tmpreg;
}

/*
 * Function:        USART_ClockStructInit
 * Description:     Fills each USART_ClockInitStruct member with its default value.
 * Param:           USART_ClockInitStruct: pointer to a USART_ClockInitTypeDef structure
 *                  which will be initialized.
 * Return:          None.
 */
void USART_ClockStructInit(USART_ClockInitTypeDef* USART_ClockInitStruct)
{
    /* USART_ClockInitStruct members default value */
    USART_ClockInitStruct->USART_Clock = USART_Clock_Disable;
    USART_ClockInitStruct->USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStruct->USART_CPHA = USART_CPHA_1Edge;
    USART_ClockInitStruct->USART_LastBit = USART_LastBit_Disable;
}

/*
 * Function:        USART_Cmd
 * Description:     Enables or disables the specified USART peripheral.
 * Param:           USARTx: where x can be (0..7) to select the USART peripheral.
 *                  NewState: new state of the USARTx peripheral.
 *                  This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_FunctionalState_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected USART by setting the UE bit in the CR1 register */
        USARTx->CR1 |= ((uint32_t)USART_CR1_UE);
    }
    else
    {
        /* Disable the selected USART by clearing the UE bit in the CR1 register */
        USARTx->CR1 &= ~((uint32_t)USART_CR1_UE);
    }
}

/*
 * Function:        USART_OneBitMethodCmd
 * Description:     Enables or disables the USART's one bit sampling method.
 * Param:           USARTx: where x can be (0..7) to select the USART peripheral.
 *                  NewState: new state of the USART one bit sampling method.
 *                  This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void USART_OneBitMethodCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_FunctionalState_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the one bit method by setting the ONEBITE bit in the CR3 register */
        USARTx->CR3 |= ((uint32_t)USART_CR3_ONEBIT);
    }
    else
    {
        /* Disable the one bit method by clearing the ONEBITE bit in the CR3 register */
        USARTx->CR3 &= ~((uint32_t)USART_CR3_ONEBIT);
    }
}

/*
 * Function:        USART_SendData
 * Description:     Transmits single data through the USARTx peripheral.
 * Param:           USARTx: where x can be (0..7) to select the USART peripheral.
 *                  Data: the data to transmit.
 * Return:          None.
 */
void USART_SendData(USART_TypeDef* USARTx, uint8_t Data)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_DATA(Data)); 

    /* Transmit Data */
    USARTx->TDR = (Data & (uint16_t)0x01FF);
}

/*
 * Function:        USART_ReceiveData
 * Description:     Returns the most recent received data by the USARTx peripheral.
 * Param:           USARTx: where x can be (0..7) to select the USART peripheral.
 * Return:          The received data.
 */
uint8_t USART_ReceiveData(USART_TypeDef* USARTx)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    /* Receive Data */
    return (USARTx->RDR & 0xFF);
}

/*
 * Function:        USART_LINBreakDetectLengthConfig
 * Description:     Sets the USART LIN Break detection length.
 * Param:           USARTx: where x can be (0..7) to select the USART peripheral.
 *                  USART_LINBreakDetectLength: specifies the LIN break detection length.
 *                   This parameter can be one of the following values:
 *                    USART_LINBreakDetectLength_11b: 11-bit break detection
 *                    USART_LINBreakDetectLength_12b: 12-bit break detection
 *                    USART_LINBreakDetectLength_13b: 13-bit break detection
 *                    USART_LINBreakDetectLength_14b: 14-bit break detection
 * Return:          None.
 */
void USART_LINBreakDetectLengthConfig(USART_TypeDef* USARTx, uint16_t USART_LINBreakDetectLength)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_LIN_BREAK_DETECT_LENGTH(USART_LINBreakDetectLength));

    USARTx->CR2 &= ~((uint32_t)USART_CR2_BKNUM);
    USARTx->CR2 |= USART_LINBreakDetectLength;  
}

/*
 * Function:        USART_LINCmd
 * Description:     Enables or disables the USART's LIN mode.
 * Param:           USARTx: where x can be (0..7) to select the USART peripheral.
 *                  NewState: new state of the USART LIN mode.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void USART_LINCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_FunctionalState_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the LIN mode by setting the LINEN bit in the CR2 register */
        USARTx->CR2 |= USART_CR2_LINEN;
    }
    else
    {
        /* Disable the LIN mode by clearing the LINEN bit in the CR2 register */
        USARTx->CR2 &= ~((uint32_t)USART_CR2_LINEN);
    }
}

/*
 * Function:        USART_SendBreak
 * Description:     Transmit break characters.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral.
 * Return:          None.
 */
void USART_SendBreak(USART_TypeDef *USARTx)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));

	/* Send break characters */
	USARTx->RQR |= ((uint16_t)(USART_SendBreak_Request));
}

/*
 * Function:        USART_AutoBaudRate
 * Description:     Auto baud rate request.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral.
 * Return:          None.
 */
void USART_AutoBaudRate(USART_TypeDef *USARTx)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));

	/* Send break characters */
	USARTx->RQR |= ((uint16_t)(USART_AutoBaud_Request));
}

/*
 * Function:        USART_RxDataFlush
 * Description:     Receive data flush request.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral.
 * Return:          None.
 */
void USART_RxDataFlush(USART_TypeDef *USARTx)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));

	/* Receive data flush request */
	USARTx->RQR |= ((uint16_t)(USART_RXData_Flush_Request));
}

/*
 * Function:        USART_TxDataFlush
 * Description:     Transmit data flush request.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral.
 * Return:          None.
 */
void USART_TxDataFlush(USART_TypeDef *USARTx)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));

	/* Transmit data flush Request */
	USARTx->RQR |= ((uint16_t)(USART_TXData_Flush_Request));
}

/*
 * Function:        USART_HalfDuplexCmd
 * Description:     Enables or disables the USART's Half Duplex communication.
 * Param:           USARTx: where x can be (0..7) to select the USART peripheral.
 *                  NewState: new state of the USART Communication.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void USART_HalfDuplexCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_FunctionalState_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the Half-Duplex mode by setting the HDSEL bit in the CR3 register */
        USARTx->CR3 |= USART_CR3_HDSEL;
    }
    else
    {
        /* Disable the Half-Duplex mode by clearing the HDSEL bit in the CR3 register */
        USARTx->CR3 &= ~((uint16_t)USART_CR3_HDSEL);
    }
}

/*
 * Function:        USART_DMACmd
 * Description:     Enables or disables the USART's DMA interface.
 * Param:           USARTx: where x can be (0..7) to select the USART peripheral.
 *                  USART_DMAReq: specifies the DMA request.
 *                   This parameter can be any combination of the following values:
 *                     USART_DMAReq_Tx: USART DMA transmit request
 *                     USART_DMAReq_Rx: USART DMA receive request
 *                  NewState: new state of the DMA Request sources.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void USART_DMACmd(USART_TypeDef* USARTx, uint16_t USART_DMAReq, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_DMAREQ(USART_DMAReq));  
    assert_param(IS_FunctionalState_STATE(NewState)); 

    if (NewState != DISABLE)
    {
        /* Enable the DMA transfer for selected requests by setting the DMAT and/or
            DMAR bits in the USART CR3 register */
        USARTx->CR3 |= USART_DMAReq;
    }
    else
    {
        /* Disable the DMA transfer for selected requests by clearing the DMAT and/or
            DMAR bits in the USART CR3 register */
        USARTx->CR3 &= ~((uint32_t)USART_DMAReq);
    }
}

/*
 * Function:        USART_EnableFifoMode
 * Description:     Enable the FIFO mode.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral.
 * Return:          None.
 */
void USART_EnableFifoMode(USART_TypeDef *USARTx)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    /* Disable the Peripheral */
    USARTx->CR1 &= ~((uint32_t)(USART_CR1_UE));

    /* Enable FIFO mode */
    USARTx->CR1 |= ((uint32_t)(USART_CR1_FIFOEN));
}

/*
 * Function:        USART_DisableFifoMode
 * Description:     Disable the FIFO mode.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral.
 * Return:          None.
 */
void USART_DisableFifoMode(USART_TypeDef *USARTx)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    /* Disable the Peripheral */
    USARTx->CR1 &= ~((uint32_t)(USART_CR1_UE));

    /* Disable FIFO mode */
    USARTx->CR1 &= ~((uint32_t)(USART_CR1_FIFOEN));
}

/*
 * Function:        USART_SetTxFifoThreshold
 * Description:     Set the TXFIFO threshold.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral.
 *                  Threshold: TX FIFO threshold value.
 *                   This parameter can be one of the following values:
 *                    USART_TXFIFO_Threshold_1_8: TXFIFO reaches 1/8 of its depth.
 *                    USART_TXFIFO_Threshold_1_4: TXFIFO reaches 1/4 of its depth.
 *                    USART_TXFIFO_Threshold_1_2: TXFIFO reaches 1/2 of its depth.
 *                    USART_TXFIFO_Threshold_3_4: TXFIFO reaches 3/4 of its depth.
 *                    USART_TXFIFO_Threshold_7_8: TXFIFO reaches 7/8 of its depth.
 *                    USART_TXFIFO_Threshold_8_8: TXFIFO becomes empty.
 * Return:          None.
 */
void USART_SetTxFifoThreshold(USART_TypeDef *USARTx, uint32_t Threshold)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_TXFIFO_THRESHOLD(Threshold));

    /* Disable the Peripheral */
    USARTx->CR1 &= ~((uint32_t)(USART_CR1_UE));

    /* Update TX threshold configuration */
    USARTx->CR3 &= ~((uint32_t)(USART_CR3_TXFTCFG));
    USARTx->CR3 |= ((uint32_t)(Threshold));
}

/*
 * Function:        USART_SetRxFifoThreshold
 * Description:     Set the RXFIFO threshold.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral.
 *                  Threshold: RX FIFO threshold value.
 *                   This parameter can be one of the following values:
 *                    USART_RXFIFO_Threshold_1_8: RXFIFO FIFO reaches 1/8 of its depth.
 *                    USART_RXFIFO_Threshold_1_4: RXFIFO FIFO reaches 1/4 of its depth.
 *                    USART_RXFIFO_Threshold_1_2: RXFIFO FIFO reaches 1/2 of its depth.
 *                    USART_RXFIFO_Threshold_3_4: RXFIFO FIFO reaches 3/4 of its depth.
 *                    USART_RXFIFO_Threshold_7_8: RXFIFO FIFO reaches 7/8 of its depth.
 *                    USART_RXFIFO_Threshold_8_8: RXFIFO FIFO becomes full.
 * Return:          None
 */
void USART_SetRxFifoThreshold(USART_TypeDef *USARTx, uint32_t Threshold)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_RXFIFO_THRESHOLD(Threshold));

	/* Disable the Peripheral */
	USARTx->CR1 &= ~((uint32_t)(USART_CR1_UE));

	/* Update RX threshold configuration */
	USARTx->CR3 &= ~((uint32_t)(USART_CR3_RXFTCFG));
	USARTx->CR3 |= ((uint32_t)(Threshold));
}

/*
 * Function:        USART_EnableSlaveMode
 * Description:     Enable the SPI slave mode.
 * Note:            When the USART operates in SPI slave mode, it handles data flow using
 *                  the serial interface clock derived from the external SCLK signal
 *                  provided by the external master SPI device.
 *                  In SPI slave mode, the USART must be enabled before starting the master
 *                  communications (or between frames while the clock is stable) Otherwise,
 *                  if the USART slave is enabled while the master is in the middle of a
 *                  frame, it will become desynchronized with the master.
 *                  The data register of the slave needs to be ready before the first edge
 *                  of the communication clock or before the end of the ongoing communication,
 *                  otherwise the SPI slave will transmit zeros.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral.
 * Return:          None.
 */
void USART_EnableSlaveMode(USART_TypeDef *USARTx)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    /* Synchronous slave mode limited to 8-bit data length */
    USARTx->CR1 &= ~((uint32_t)(USART_CR1_M));

    /* Disable the Peripheral */
    USARTx->CR1 &= ~((uint32_t)(USART_CR1_UE)); 

    /* In half-duplex mode, the following bits must be kept cleared:
    - LINEN and CLKEN bits in the USART_CR2 register.*/
    USARTx->CR2 &= ~((uint32_t)(USART_CR2_LINEN | USART_CR2_CLKEN));

    /* Enable the Synchronous slave mode by setting the SLVEN bit in the CR2 register */
    USARTx->CR2 |= ((uint32_t)(USART_CR2_SLVEN));
}

/*
 * Function:        USART_DisableSlaveMode
 * Description:     Disable the SPI slave mode.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral.
 * Return:          None.
 */
void USART_DisableSlaveMode(USART_TypeDef *USARTx)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    /* Disable the Peripheral */
    USARTx->CR1 &= ~((uint32_t)(USART_CR1_UE)); 

    /* Disable SPI slave mode */
    USARTx->CR2 &= ~((uint32_t)(USART_CR2_SLVEN));
}

/*
 * Function:        USART_ConfigNSS
 * Description:     Configure the Slave Select input pin (NSS)
 * Note:            Software NSS management: SPI slave will always be selected and NSS
 *                  input pin will be ignored.
 *                  Hardware NSS management: the SPI slave selection depends on NSS
 *                  input pin. The slave is selected when NSS is low and deselected when
 *                  NSS is high.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral.
 *                  NSSConfig: NSS configuration.
 *                   This parameter can be one of the following values:
 *                    USART_NSS_Hw.
 *                    USART_NSS_Sw.
 * Return:          None.
 */
void USART_ConfigNSS(USART_TypeDef *USARTx, uint32_t NSSConfig)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_NSS(NSSConfig));

    /* Disable the Peripheral */
    USARTx->CR1 &= ~((uint32_t)(USART_CR1_UE));

    /* Program DIS_NSS bit in the USART_CR2 register */
    USARTx->CR2 &= ~((uint32_t)USART_CR2_DIS_NSS);
    USARTx->CR2 |= ((uint32_t)(NSSConfig));
}

/*
 * Function:        USART_DeInit
 * Description:     DeInitialize the USART peripheral.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral
 * Return:          None
 */
void USART_DeInit(USART_TypeDef *USARTx)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    USARTx->CR1 = 0x00;
    USARTx->CR2 = 0x00;
    USARTx->CR3 = 0x00;
}

/*
 * Function:        USART_ITConfig
 * Description:     Wraps up transmission in non-blocking mode.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral.
 *                  ITType: USART interrupt trigger type.
 *                   This parameter can be one of the following values:
 *                    USART_IT_IDLE: USART idle interruption.
 *                    USART_IT_RXNE: USART read data register not empty interruption.
 *                    USART_IT_RXFNE: USART RXFIFO not empty interruption.
 *                    USART_IT_TC: USART transmission complete interruption.
 *                    USART_IT_TXE: USART transmit data register empty interruption.
 *                    USART_IT_TXFNF: USART TX FIFO not full interruption.
 *                    USART_IT_PE: USART parity error interruption.
 *                    USART_IT_TXFE: USART TXFIFO empty interruption.
 *                    USART_IT_RXFF: USART RXFIFO full interruption.
 *                    USART_IT_LBD: USART LIN break detection interruption.
 *                    USART_IT_ERR: USART error interruption.
 *                    USART_IT_CTS: USART CTS interruption.
 *                    USART_IT_TXFT: USART TXFIFO threshold reached interruption.
 *                    USART_IT_RXFT: USART RXFIFO threshold reached interruption.
 *                  NewState: new state of the Update disable.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None
 */
void USART_ITConfig(USART_TypeDef *USARTx, uint32_t ITType, EnState NewState)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_IT(ITType));
    assert_param(IS_EnState_STATE(NewState));

    if (NewState != DISABLE) 
    {
        /* Enable the interrupt */
        if(((ITType & USART_CR_MASK) >> USART_CR_POS) == 1U)
        {
            USARTx->CR1 |= ((uint32_t)(1U << (ITType & USART_IT_MASK)));
        }
        else if (((ITType & USART_CR_MASK) >> USART_CR_POS) == 2U)
        {
            USARTx->CR2 |= ((uint32_t)(1U << (ITType & USART_IT_MASK)));
        }
        else
        {
            USARTx->CR3 |= ((uint32_t)(1U << (ITType & USART_IT_MASK)));
        }
    } 
    else 
    {
        /* Disable the interrupt */
        if(((ITType & USART_CR_MASK) >> USART_CR_POS) == 1U)
        {
            USARTx->CR1 &= ~((uint32_t)(1U << (ITType & USART_IT_MASK)));
        }
        else if (((ITType & USART_CR_MASK) >> USART_CR_POS) == 2U)
        {
            USARTx->CR2 &= ~((uint32_t)(1U << (ITType & USART_IT_MASK)));
        }
        else
        {
            USARTx->CR3 &= ~((uint32_t)(1U << (ITType & USART_IT_MASK)));
        }
    }
}

/*
 * Function:        USART_GetFlagStatus
 * Description:     Check whether the specified USART interrupt has occurred or not.
 * param:           USARTx: where x can be (0..3) to select the USART peripheral.
 *                  USART_FLAG: Specifies the USART flag to check.
 *                   This parameter can be one of the following values:
 *                    USART_FLAG_TXFT: USART TXFIFO threshold flag.
 *                    USART_FLAG_RXFT: USART RXFIFO threshold flag.
 *                    USART_FLAG_RXFF: USART RXFIFO Full flag.
 *                    USART_FLAG_TXFE: USART TXFIFO Empty flag.
 *                    USART_FLAG_SBKF: USART send break flag.
 *                    USART_FLAG_BUSY: USART busy flag.
 *                    USART_FLAG_ABRF: USART auto Baud rate flag.
 *                    USART_FLAG_ABRE: USART auto Baud rate error.
 *                    USART_FLAG_CTS: USART CTS flag.
 *                    USART_FLAG_CTSIF: USART CTS interrupt flag.
 *                    USART_FLAG_LBDF: USART LIN break detection flag.
 *                    USART_FLAG_TXE: USART transmit data register empty.
 *                    USART_FLAG_TXFNF: USART TXFIFO not full.
 *                    USART_FLAG_TC: USART transmission complete.
 *                    USART_FLAG_RXNE: USART read data register not empty.
 *                    USART_FLAG_RXFNE: USART RXFIFO not empty.
 *                    USART_FLAG_IDLE: USART idle flag.
 *                    USART_FLAG_ORE: USART overrun error.
 *                    USART_FLAG_NE: USART noise error.
 *                    USART_FLAG_FE: USART frame error.
 *                    USART_FLAG_PE: USART parity error.
 * Return:          Usart IT status.
 *                   This parameter can be one of the following values:
 *                    SET or RESET.
 */
FlagStatus USART_GetFlagStatus(USART_TypeDef *USARTx, uint32_t USART_FLAG)
{
	FlagStatus bitstatus = RESET;

	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_FLAG(USART_FLAG));

	if ((USARTx->ISR & USART_FLAG) != RESET) 
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
 * Function:        USART_GetITStatus
 * Description:     Check whether the specified USART interrupt source is enabled or not.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral.
 *                  ITType: USART interrupt trigger type.
 *                   This parameter can be one of the following values:
 *                    USART_IT_IDLE: USART idle interruption.
 *                    USART_IT_RXNE: USART read data register not empty interruption.
 *                    USART_IT_RXFNE: USART RXFIFO not empty interruption.
 *                    USART_IT_TC: USART transmission complete interruption.
 *                    USART_IT_TXE: USART transmit data register empty interruption.
 *                    USART_IT_TXFNF: USART TX FIFO not full interruption.
 *                    USART_IT_PE: USART parity error interruption.
 *                    USART_IT_TXFE: USART TXFIFO empty interruption.
 *                    USART_IT_RXFF: USART RXFIFO full interruption.
 *                    USART_IT_LBD: USART LIN break detection interruption.
 *                    USART_IT_ERR: USART error interruption.
 *                    USART_IT_CTS: USART CTS interruption.
 *                    USART_IT_TXFT: USART TXFIFO threshold reached interruption.
 *                    USART_IT_RXFT: USART RXFIFO threshold reached interruption.
 * Return:          The new state of USART_IT (SET or RESET).
 */
FlagStatus USART_GetITStatus(USART_TypeDef *USARTx, uint32_t USART_IT)
{
	FlagStatus bitstatus = RESET;

	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_IT(USART_IT));

	if(((USART_IT & USART_CR_MASK) >> USART_CR_POS) == 1U)
	{
		if((USARTx->CR1 & (1U << (USART_IT & USART_IT_MASK))) != RESET)
		{
			bitstatus = SET;
		}
		else
		{
			bitstatus = RESET;
		}
	}
	else if (((USART_IT & USART_CR_MASK) >> USART_CR_POS) == 2U)
	{
		if((USARTx->CR2 & (1U << (USART_IT & USART_IT_MASK))) != RESET)
		{
			bitstatus = SET;
		}
		else
		{
			bitstatus = RESET;
		}
	}
	else
	{
		if((USARTx->CR3 & (1U << (USART_IT & USART_IT_MASK))) != RESET)
		{
			bitstatus = SET;
		}
		else
		{
			bitstatus = RESET;
		}
	}

	return bitstatus;
}

/*
 * Function:        USART_ClearITPendingBit
 * Description:     Clear the interrupt pending bits of the specified USART.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral.
 * 					ClearIT: USART interruption clear flags.
 * 					 This parameter can be one of the following values:
 * 					  USART_CLEAR_PEF: Parity error clear flag.
 * 					  USART_CLEAR_FEF: Framing error clear flag.
 * 					  USART_CLEAR_NEF: Noise error detected clear flag.
 * 					  USART_CLEAR_OREF: Overrun error clear flag.
 * 					  USART_CLEAR_IDLEF: Idle line detected clear flag.
 * 					  USART_CLEAR_TXFEF: Txfifo empty clear flag.
 * 					  USART_CLEAR_TCF: Transmission complete clear flag.
 * 					  USART_CLEAR_LBDF: Lin break detection clear flag.
 * 					  USART_CLEAR_CTSF: Cts clear flag.
 * 					  USART_CLEAR_RXFTF: Clear rxfifo threshold interrupt flag.
 * 					  USART_CLEAR_TXFTF: Clear txfifo threshold interrupt flag.
 * Return:          None
 */
void USART_ClearITPendingBit(USART_TypeDef *USARTx, uint32_t ClearIT)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_CLEAR(ClearIT));

	USARTx->ICR = ((uint32_t)ClearIT);
}

/*
 * Function:        USART_AdvFeatureConfig
 * Description:     Configure the USART peripheral advanced features.
 * Param:           USARTx: where x can be (0..3) to select the USART peripheral.
 *                  USART_AdvFeatureInitStruct: pointer to a USART_AdvFeatureInitTypeDef structure that contains
 * 					the configuration information for the specified. USART peripheral.
 * Return:          None.
 */
void USART_AdvFeatureConfig(USART_TypeDef *USARTx, USART_AdvFeatureInitTypeDef *USART_AdvFeatureInitStruct)
{
    /* Check whether the set of advanced features to configure is properly set */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_ADVFEATURE_INIT(USART_AdvFeatureInitStruct->AdvFeatureInit));

    /* if required, configure TX pin active level inversion */
    if(((USART_AdvFeatureInitStruct->USART_AdvFeatureInit) & (USART_ADVFeature_TXInvert_Init)) != RESET)
    {
        assert_param(IS_USART_ADVFEATURE_TXINV(USART_AdvFeatureInitStruct->USART_TxPinLevelInvert));

        USARTx->CR2 &= ~((uint32_t)(USART_CR2_TXINV));
        USARTx->CR2 |= ((uint32_t)(USART_AdvFeatureInitStruct->USART_TxPinLevelInvert));
    }
    else
	{
		/* Nothing to do */
	}

    /* if required, configure RX pin active level inversion */
    if(((USART_AdvFeatureInitStruct->USART_AdvFeatureInit) & (USART_ADVFeature_RXInvert_Init)) != RESET)
    {
        assert_param(IS_USART_ADVFEATURE_RXINV(USART_AdvFeatureInitStruct->USART_RxPinLevelInvert));

        USARTx->CR2 &= ~((uint32_t)(USART_CR2_RXINV));
        USARTx->CR2 |= ((uint32_t)(USART_AdvFeatureInitStruct->USART_RxPinLevelInvert));
    }
    else
	{
		/* Nothing to do */
	}

    /* if required, configure data inversion */
    if(((USART_AdvFeatureInitStruct->USART_AdvFeatureInit) & (USART_ADVFeature_DATAInvert_Init)) != RESET)
    {
        assert_param(IS_USART_ADVFEATURE_DATAINV(USART_AdvFeatureInitStruct->USART_DataInvert));

        USARTx->CR2 &= ~((uint32_t)(USART_CR2_DATAINV));
        USARTx->CR2 |= ((uint32_t)(USART_AdvFeatureInitStruct->USART_DataInvert));
    }
    else
	{
		/* Nothing to do */
	}

    /* if required, configure RX overrun detection disabling */
    if(((USART_AdvFeatureInitStruct->USART_AdvFeatureInit) & (USART_ADVFeature_RXOverrunDisable_Init)) != RESET)
    {
        assert_param(IS_USART_OVERRUN(USART_AdvFeatureInitStruct->USART_OverrunDisable));

        USARTx->CR3 &= ~((uint32_t)(USART_CR3_OVRDIS));
        USARTx->CR3 |= ((uint32_t)(USART_AdvFeatureInitStruct->USART_OverrunDisable));
    }
    else
	{
		/* Nothing to do */
	}

    /* if required, configure auto Baud rate detection scheme */
    if(((USART_AdvFeatureInitStruct->USART_AdvFeatureInit) & (USART_ADVFeature_AutoBaudrate_Init)) != RESET)
    {
        assert_param(IS_USART_ADVFEATURE_AUTOBAUDRATE(USART_AdvFeatureInitStruct->USART_AutoBaudRateEnable));

        USARTx->CR2 &= ~((uint32_t)(USART_CR2_ABREN));
        USARTx->CR2 |= ((uint32_t)(USART_AdvFeatureInitStruct->USART_AutoBaudRateEnable));

        /* set auto Baudrate detection parameters if detection is enabled */
        if(USART_AdvFeatureInitStruct->USART_AutoBaudRateEnable == USART_ADVFeature_AutoBaudRate_Enable)
        {
            assert_param(IS_USART_ADVFEATURE_AUTOBAUDRATEMODE(USART_AdvFeatureInitStruct->USART_AutoBaudRateMode));

            USARTx->CR2 &= ~((uint32_t)(USART_CR2_ABRMODE));
            USARTx->CR2 |= ((uint32_t)(USART_AdvFeatureInitStruct->USART_AutoBaudRateMode));
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        /* Nothing to do */
    }

    /* if required, configure MSB first on communication line */
    if(((USART_AdvFeatureInitStruct->USART_AdvFeatureInit) & (USART_ADVFeature_MSBFirst_Init)) != RESET)
    {
        assert_param(IS_USART_ADVFEATURE_MSBFIRST(USART_AdvFeatureInitStruct->USART_MSBFirst));

        USARTx->CR2 &= ~((uint32_t)(USART_CR2_MSBFIRST));
        USARTx->CR2 |= ((uint32_t)(USART_AdvFeatureInitStruct->USART_MSBFirst));
    }
    else
	{
		/* Nothing to do */
	}
}

/*****END OF FILE*****/
