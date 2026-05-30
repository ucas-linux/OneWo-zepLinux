/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_qspi.c
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2024/08/20
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/08/20     1.0     build this module
 * Description:     This file provides firmware functions to manage the functionalities of the QSPI peripheral.
 */

#include "as32x601_qspi.h"

/*
 * Function:        QSPI_StructInit
 * Description:     Fills each QSPI_InitStruct member with its default value.
 * Param:           QSPI_InitStruct : pointer to a QSPI_InitTypeDef structure which will be initialized.
 * Return:          None
 */
void QSPI_StructInit(QSPI_InitTypeDef *QSPI_InitStruct)
{
    QSPI_InitStruct->PRESCALER = 0x00;          /* Clock prescaler,this value can be (2~256)*/
    QSPI_InitStruct->CSHT      = QSPI_CSHT_7;   /* Chip select high time */
    QSPI_InitStruct->CKMODE    = QSPI_CKMode_0; /* Mode 0/mode 3 */
}

/*
 * Function:        QSPI_Init
 * Description:     Initializes the QSPI peripheral according to the specified parameters in the QSPI_InitStruct.
 * Param:           QSPI_InitStruct: pointer to a QSPI_InitTypeDef structure that contains the configuration
 *					information for the specified QSPI peripheral.
 * Return:          None
 */
void QSPI_Init(QSPI_InitTypeDef *QSPI_InitStruct)
{
    uint32_t tmpreg = 0x00;

    /* Check the parameters */
    assert_param((QSPI_InitStruct->PRESCALER >= 2U) && (QSPI_InitStruct->PRESCALER <= 256U));
    assert_param(IS_QSPI_CSHT(QSPI_InitStruct->CSHT));
    assert_param(IS_QSPI_CKMODE(QSPI_InitStruct->CKMODE));

    tmpreg = QSPI->CR;

    /* Set PRESCALER ,CSHT and CKMODE bits */
    tmpreg &= ~((uint32_t)(QSPI_CR_PRESCALER | QSPI_CR_CSHT | QSPI_CR_CKMODE));
    tmpreg |= (uint32_t)((QSPI_InitStruct->PRESCALER - 1) << QSPI_CR_PRESCALER_Pos) |
                QSPI_InitStruct->CSHT | QSPI_InitStruct->CKMODE;

    /* Write to QSPI CR */
    QSPI->CR = tmpreg;
}

/*
 * Function:        QSPI_Cmd
 * Description:     Enables or disables the QSPI.
 * Param:           NewState: new state of the QSPI peripheral.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void QSPI_Cmd(FunctionalState NewState)
{
    /* Check the parameter */
    assert_param(IS_EnState_STATE(NewState));

    if (NewState != DISABLE)
    {
        QSPI->CR |= ((uint32_t)QSPI_CR_EN);
    }
    else
    {
        QSPI->CR &= ~((uint32_t)QSPI_CR_EN);
    }
}

/*
 * Function:        QSPI_AbortRequest
 * Description:     Abort or not abort the request.
 * Param:           NewState: new state of the QSPI abort request.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void QSPI_AbortRequest(FunctionalState NewState)
{
    /* Check the parameter */
    assert_param(IS_EnState_STATE(NewState));

    if (NewState != DISABLE)
    {
        QSPI->CR |= ((uint32_t)QSPI_CR_ABORT);
    }
    else
    {
        QSPI->CR &= ~((uint32_t)QSPI_CR_ABORT);
    }
}

/*
 * Function:        QSPI_CmdStructInit
 * Description:     Fills each SPI_InitStruct member with its default value.
 * Param:           QSPI_CmdStruct : pointer to a SPI_InitTypeDef structure which will be initialized.
 * Return:          None
 */
void QSPI_CmdStructInit(QSPI_CmdTypeDef *QSPI_CmdStruct)
{
    QSPI_CmdStruct->INST   = 0x00;                       /* Instruction */
    QSPI_CmdStruct->IMODE  = QSPI_InstMode_None;         /* Instruction mode */
    QSPI_CmdStruct->ADMODE = QSPI_AddrMode_None;         /* Address mode */
    QSPI_CmdStruct->ADSIZE = QSPI_AddrSize_8b;           /* Address size */
    QSPI_CmdStruct->ABMODE = QSPI_AltByteMode_None;      /* Alternate bytes mode */
    QSPI_CmdStruct->ABSIZE = QSPI_AltByteSize_8b;        /* Alternate bytes size */
    QSPI_CmdStruct->DUMMY  = 0x00;                       /* Dummy cycles */
    QSPI_CmdStruct->DMODE  = QSPI_DataMode_None;         /* Data mode */
    QSPI_CmdStruct->FMODE  = QSPI_FunMode_WriteIndirect; /* Functional mode */
}

/*
 * Function:        QSPI_CmdConfig
 * Description:     Communicate with flash by configuring the communication register.
 * Param:           QSPI_CmdStruct : pointer to a QSPI_CmdTypeDef structure that contains
 *					the configuration information for the specified QSPI peripheral.
 * Return:          None
 */
void QSPI_CmdConfig(QSPI_CmdTypeDef *QSPI_CmdStruct)
{
    uint32_t tmpreg = 0x00;

    /* Check the parameters */
    assert_param(QSPI_CmdStruct->INST <= 0xFF);
    assert_param(IS_QSPI_INSTMODE(QSPI_CmdStruct->IMODE));
    assert_param(IS_QSPI_ADDRMODE(QSPI_CmdStruct->ADMODE));
    assert_param(IS_QSPI_ADDRSIZE(QSPI_CmdStruct->ADSIZE));
    assert_param(IS_QSPI_ALTBYTEMODE(QSPI_CmdStruct->ABMODE));
    assert_param(IS_QSPI_ALTBYTESIZE(QSPI_CmdStruct->ABSIZE));
    assert_param(QSPI_CmdStruct->DUMMY <= 0x1F);
    assert_param(IS_QSPI_DATAMODE(QSPI_CmdStruct->DMODE));
    assert_param(IS_QSPI_FUNMODE(QSPI_CmdStruct->FMODE));

    tmpreg = QSPI->CCR;

    tmpreg = (uint32_t)(QSPI_CmdStruct->INST | QSPI_CmdStruct->IMODE |
                        QSPI_CmdStruct->ADMODE | QSPI_CmdStruct->ADSIZE |
                        QSPI_CmdStruct->ABMODE | QSPI_CmdStruct->ABSIZE |
                        (QSPI_CmdStruct->DUMMY << QSPI_CCR_DUMMY_Pos) | QSPI_CmdStruct->DMODE |
                        (QSPI_CmdStruct->FMODE) << QSPI_CCR_FMODE_Pos);

    /* Write to QSPI CCR */
    QSPI->CCR = tmpreg;
}

/*
 * Function:        QSPI_GetFlagStatus
 * Description:     Checks whether the QSPI flag is set or not.
 * Param:           QSPI_FLAG: specifies the flag to check.
 *                   This parameter can be one of the following values:
 *                      QSPI_FLAG_BUSY: busy
 *                      QSPI_FLAG_TCF: transmission completion flag
 *                      QSPI_FLAG_TOF: timeout flag
 *                      QSPI_FLAG_PMF: polling for matching flags
 * Return:          The state of QSPI_FLAG (SET or RESET).
 */
FlagStatus QSPI_GetFlagStatus(uint32_t QSPI_FLAG)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_QSPI_FLAG(QSPI_FLAG));

    if ((QSPI->SR & QSPI_FLAG) != (uint32_t)RESET)
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
 * Function:        QSPI_GetFlagStatus
 * Description:     Clear whether the QSPI flag.
 * Param:           QSPI_FLAG: the flag which to be cleared.
 *                   This parameter can be one of the following values:
 *                      QSPI_FLAG_BUSY: busy
 *                      QSPI_FLAG_TCF: transmission completion flag
 *                      QSPI_FLAG_TOF: timeout flag
 *                      QSPI_FLAG_PMF: Polling for matching flag
 * Return:          None.
 */
void QSPI_ClearFlag(uint32_t QSPI_FLAG)
{
    /* Check the parameters */
    assert_param(IS_QSPI_CLEAR(QSPI_FLAG));

    QSPI->FCR = QSPI_FLAG;
}

/*
 * Function:        QSPI_GetFIFOLevel
 * Description:     Get the QSPI FIFO level.
 * Param:           None.
 * Return:          Number of valid bytes in FIFO.
 */
uint8_t QSPI_GetFIFOLevel(void)
{
    uint8_t value = 0x00;

    value = (uint8_t)((QSPI->SR & QSPI_SR_FLEVEL) >> QSPI_SR_FLEVEL_Pos);

    return value;
}

/*
 * Function:        QSPI_SetAddr
 * Description:     Set the address that sent to the QSPI Flash.
 * Param:           QSPI_Addr: the address sent to the QSPI Flash.
 * Return:          None.
 */
void QSPI_SetAddr(uint32_t QSPI_Addr)
{
    QSPI->AR = QSPI_Addr;
}

/*
 * Function:        QSPI_SetAltBytes
 * Description:     Set the alternate bytes that sent to the QSPI Flash.
 * Param:           QSPI_AltBytes: the alternate bytes that sent to the QSPI Flash.
 * Return:          None.
 */
void QSPI_SetAltBytes(uint32_t QSPI_AltBytes)
{
    QSPI->ABR = QSPI_AltBytes;
}

/*
 * Function:        QSPI_SetData
 * Description:     Set the data that sent to the QSPI Flash.
 * Param:           QSPI_Data: the alternate bytes that sent to the QSPI Flash.
 * Return:          None.
 */
void QSPI_SetData(uint32_t QSPI_Data)
{
    QSPI->DR = QSPI_Data;
}

/*
 * Function:        QSPI_ReadData
 * Description:     Get the data received from the QSPI Flash.
 * Param:           None.
 * Return:          The data received from the QSPI Flash.
 */
uint32_t QSPI_ReadData(void)
{
    return QSPI->DR;
}

/*
 * Function:        QSPI_SetDataLength
 * Description:     Set the number of bytes that need to be transferred when communicating with the QSPI Flash.
 * Param:           QSPI_DataLength: the number of bytes(0x00000000 - 0xFFFFFFFF),set 0x00000000 is 1 byte.
 * Note:            In polling mode, this field is set to a maximum of 3 (transfer 4 bytes of data).
 * Return:          None.
 */
void QSPI_SetDataLength(uint32_t QSPI_DataLength)
{
    QSPI->DLR = QSPI_DataLength;
}

/*
 * Function:        QSPI_SetTimeout
 * Description:     Set the timeout period in memory mapping mode.
 * Param:           QSPI_Timeout: timeout wait value.
 * Return:          None.
 */
void QSPI_SetTimeout(uint16_t QSPI_Timeout)
{
    QSPI->TOR = (uint32_t)QSPI_Timeout;
}

/*
 * Function:        QSPI_SetPollingInterval
 * Description:     Set the polling interval of read operations in auto polling mode.
 * Param:           QSPI_PollingInterval: polling interval value.
 * Return:          None.
 */
void QSPI_SetPollingInterval(uint16_t QSPI_PollingInterval)
{
    QSPI->PIR = (uint32_t)QSPI_PollingInterval;
}

/*
 * Function:        QSPI_SetPollingStaMatch
 * Description:     Set the polling status match value in auto polling mode.
 * Param:           QSPI_MatchData: polling status match value.
 * Return:          None.
 */
void QSPI_SetPollingStaMatch(uint32_t QSPI_MatchData)
{
    QSPI->PSMAR = QSPI_MatchData;
}

/*
 * Function:        QSPI_PollingStaMask
 * Description:     Set the polling mask match value in auto polling mode.
 * Param:           QSPI_MatchDataMask: polling mask match value.
 * Return:          None.
 */
void QSPI_PollingStaMask(uint32_t QSPI_MatchDataMask)
{
    QSPI->PSMKR = QSPI_MatchDataMask;
}

/*****END OF FILE*****/
