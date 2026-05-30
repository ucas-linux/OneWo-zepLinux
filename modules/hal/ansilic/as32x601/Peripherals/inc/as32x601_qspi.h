/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_qspi.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2024/08/20
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/08/20     1.0     build this module
 * Description:     This file contains all the functions prototypes for the GPIO firmware library.
 */

#ifndef AS32X601_QSPI_H
#define AS32X601_QSPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601_qspi_regs.h"

/*
 * @brief QSPI init structure definition
 */
typedef struct
{
    uint32_t PRESCALER;     /* Clock prescaler */
    uint32_t CSHT;          /* Chip select high time */
    uint32_t CKMODE;        /* Mode 0/mode 3 */
} QSPI_InitTypeDef;

/*
 * @brief QSPI communication configuration structure definition
 */
typedef struct
{
    uint32_t INST;          /* Instruction */
    uint32_t IMODE;         /* Instruction mode */
    uint32_t ADMODE;        /* Address mode */
    uint32_t ADSIZE;        /* Address size */
    uint32_t ABMODE;        /* Alternate bytes mode */
    uint32_t ABSIZE;        /* Alternate bytes size */
    uint32_t DUMMY;         /* Dummy cycles */
    uint32_t DMODE;         /* Data mode */
    uint32_t FMODE;         /* Functional mode */
} QSPI_CmdTypeDef;

/*
 * @brief QSPI functional mode enumeration
 */
typedef enum
{
    QSPI_FunMode_WriteIndirect = 0x00,      /* Indirect write mode */
    QSPI_FunMode_ReadIndirect  = 0x01,      /* Indirect read mode */
    QSPI_FunMode_AutoPoll      = 0x02,      /* Automatic polling mode */
    QSPI_FunMode_MemoryMap     = 0x03       /* Memory mapping mode */
} QSPIDataMode_TypeDef;
#define IS_QSPI_FUNMODE(FUNMODE) (((FUNMODE) == QSPI_FunMode_WriteIndirect) || ((FUNMODE) == QSPI_FunMode_ReadIndirect) || \
                                  ((FUNMODE) == QSPI_FunMode_AutoPoll)      || ((FUNMODE) == QSPI_FunMode_MemoryMap))

/*
 * @defgroup QSPI CK mode definition
 */
#define QSPI_CKMode_0               ((uint32_t)0x00)        /* When nCS is high, the QSPI clock signal CLK remains low */
#define QSPI_CKMode_3               ((uint32_t)0x04)        /* When nCS is high, the QSPI clock signal CLK remains high */
#define IS_QSPI_CKMODE(CKMODE) (((CKMODE) == QSPI_CKMode_0) || ((CKMODE) == QSPI_CKMode_3))

/*
 * @defgroup QSPI chip select high time definition
 */
#define QSPI_CSHT_1                 ((uint32_t)0x00)        /* The nCS signal remains high for at least 1 cycle between commands being sent */
#define QSPI_CSHT_2                 ((uint32_t)0x08)        /* The nCS signal remains high for at least 2 cycle between commands being sent */
#define QSPI_CSHT_3                 ((uint32_t)0x10)        /* The nCS signal remains high for at least 3 cycle between commands being sent */
#define QSPI_CSHT_4                 ((uint32_t)0x18)        /* The nCS signal remains high for at least 4 cycle between commands being sent */
#define QSPI_CSHT_5                 ((uint32_t)0x20)        /* The nCS signal remains high for at least 5 cycle between commands being sent */
#define QSPI_CSHT_6                 ((uint32_t)0x28)        /* The nCS signal remains high for at least 6 cycle between commands being sent */
#define QSPI_CSHT_7                 ((uint32_t)0x30)        /* The nCS signal remains high for at least 7 cycle between commands being sent */
#define QSPI_CSHT_8                 ((uint32_t)0x38)        /* The nCS signal remains high for at least 8 cycle between commands being sent */
#define IS_QSPI_CSHT(CSHT) (((CSHT) == QSPI_CSHT_1) || ((CSHT) == QSPI_CSHT_2) || \
                            ((CSHT) == QSPI_CSHT_3) || ((CSHT) == QSPI_CSHT_4) || \
                            ((CSHT) == QSPI_CSHT_5) || ((CSHT) == QSPI_CSHT_6) || \
                            ((CSHT) == QSPI_CSHT_7) || ((CSHT) == QSPI_CSHT_8))

/*
 * @defgroup QSPI instruction mode definition
 */
#define QSPI_InstMode_None          ((uint32_t)0x0000)      /* None instruction */
#define QSPI_InstMode_1Line         ((uint32_t)0x0100)      /* Single-wire transmission instruction */
#define QSPI_InstMode_2Lines        ((uint32_t)0x0200)      /* Dual-wire transmission instruction */
#define QSPI_InstMode_4Lines        ((uint32_t)0x0300)      /* Quad-wire transmission instruction */
#define IS_QSPI_INSTMODE(INSTMODE) (((INSTMODE) == QSPI_InstMode_None)   || ((INSTMODE) == QSPI_InstMode_1Line) || \
								    ((INSTMODE) == QSPI_InstMode_2Lines) || ((INSTMODE) == QSPI_InstMode_4Lines))

/*
 * @defgroup QSPI address mode definition
 */
#define QSPI_AddrMode_None          ((uint32_t)0x0000)      /* None address */
#define QSPI_AddrMode_1Line         ((uint32_t)0x0400)      /* Single-wire transmission address */
#define QSPI_AddrMode_2Lines        ((uint32_t)0x0800)      /* Dual-wire transmission address */
#define QSPI_AddrMode_4Lines        ((uint32_t)0x0C00)      /* Quad-wire transmission address */
#define IS_QSPI_ADDRMODE(ADDRMODE) (((ADDRMODE) == QSPI_AddrMode_None)   || ((ADDRMODE) == QSPI_AddrMode_1Line) || \
                                    ((ADDRMODE) == QSPI_AddrMode_2Lines) || ((ADDRMODE) == QSPI_AddrMode_4Lines))

/*
 * @defgroup QSPI address size definition
 */
#define QSPI_AddrSize_8b            ((uint32_t)0x0000)      /* 8-bit address */
#define QSPI_AddrSize_16b           ((uint32_t)0x1000)      /* 16-bit address */
#define QSPI_AddrSize_24b           ((uint32_t)0x2000)      /* 24-bit address */
#define QSPI_AddrSize_32b           ((uint32_t)0x3000)      /* 32-bit address */
#define IS_QSPI_ADDRSIZE(ADDRSIZE) (((ADDRSIZE) == QSPI_AddrSize_8b)  || ((ADDRSIZE) == QSPI_AddrSize_16b) || \
                                    ((ADDRSIZE) == QSPI_AddrSize_24b) || ((ADDRSIZE) == QSPI_AddrSize_32b))

/*
 * @defgroup QSPI alternate bytes mode definition
 */
#define QSPI_AltByteMode_None       ((uint32_t)0x0000)      /* None alternate bytes */
#define QSPI_AltByteMode_1Line      ((uint32_t)0x4000)      /* Single-wire transmission alternate bytes */
#define QSPI_AltByteMode_2Lines     ((uint32_t)0x8000)      /* Dual-wire transmission alternate bytes */
#define QSPI_AltByteMode_4Lines     ((uint32_t)0xC000)      /* Quad-wire transmission alternate bytes */
#define IS_QSPI_ALTBYTEMODE(ALTBYTEMODE) (((ALTBYTEMODE) == QSPI_AltByteMode_None)   || ((ALTBYTEMODE) == QSPI_AltByteMode_1Line) || \
                                          ((ALTBYTEMODE) == QSPI_AltByteMode_2Lines) || ((ALTBYTEMODE) == QSPI_AltByteMode_4Lines))

/*
 * @defgroup QSPI alternate bytes size definition
 */
#define QSPI_AltByteSize_8b         ((uint32_t)0x00000000)  /* 8-bit alternate bytes*/
#define QSPI_AltByteSize_16b        ((uint32_t)0x00010000)  /* 16-bit alternate bytes */
#define QSPI_AltByteSize_24b        ((uint32_t)0x00020000)  /* 24-bit alternate bytes */
#define QSPI_AltByteSize_32b        ((uint32_t)0x00030000)  /* 32-bit alternate bytes */
#define IS_QSPI_ALTBYTESIZE(ALTBYTESIZE) (((ALTBYTESIZE) == QSPI_AltByteSize_8b)  || ((ALTBYTESIZE) == QSPI_AltByteSize_16b) || \
                                          ((ALTBYTESIZE) == QSPI_AltByteSize_24b) || ((ALTBYTESIZE) == QSPI_AltByteSize_32b))

/*
 * @defgroup QSPI data mode definition
 */
#define QSPI_DataMode_None          ((uint32_t)0x00000000)  /* None data */
#define QSPI_DataMode_1Line         ((uint32_t)0x00800000)  /* Single-wire transmission data */
#define QSPI_DataMode_2Lines        ((uint32_t)0x01000000)  /* Dual-wire transmission data */
#define QSPI_DataMode_4Lines        ((uint32_t)0x01800000)  /* Quad-wire transmission data */
#define IS_QSPI_DATAMODE(DATAMODE) (((DATAMODE) == QSPI_DataMode_None)   || ((DATAMODE) == QSPI_DataMode_1Line) || \
                                    ((DATAMODE) == QSPI_DataMode_2Lines) || ((DATAMODE) == QSPI_DataMode_4Lines))

/*
 * @defgroup QSPI flag definition
 */
#define QSPI_FLAG_BUSY              ((uint32_t)0x0001)      /* Busy */
#define QSPI_FLAG_TCF               ((uint32_t)0x0080)      /* Transmission completion flag */
#define QSPI_FLAG_TOF               ((uint32_t)0x0100)      /* Timeout flag */
#define QSPI_FLAG_PMF               ((uint32_t)0x0200)      /* Polling for matching flag */
#define IS_QSPI_FLAG(FLAG) (((FLAG) == QSPI_FLAG_BUSY) || ((FLAG) == QSPI_FLAG_TCF) || \
                            ((FLAG) == QSPI_FLAG_TOF)  || ((FLAG) == QSPI_FLAG_PMF))

/*
 * @defgroup QSPI Flag bit clear
 */
#define QSPI_CLEAR_CTCF             ((uint32_t)0x01)        /* Clear the transfer complete flag */
#define QSPI_CLEAR_CTOF             ((uint32_t)0x02)        /* Clear timeout flag */
#define QSPI_CLEAR_CPMF             ((uint32_t)0x04)        /* Clear the polling match flag */
#define IS_QSPI_CLEAR(CLEAR) (((CLEAR) == QSPI_CLEAR_CTCF) || ((CLEAR) == QSPI_CLEAR_CTOF) || \
                              ((CLEAR) == QSPI_CLEAR_CPMF))

/*
 * @brief Function declaration
 */
void QSPI_StructInit(QSPI_InitTypeDef *QSPI_InitStruct);
void QSPI_Init(QSPI_InitTypeDef *QSPI_InitStruct);
void QSPI_Cmd(FunctionalState NewState);
void QSPI_AbortRequest(FunctionalState NewState);
void QSPI_CmdStructInit(QSPI_CmdTypeDef *QSPI_CmdStruct);
void QSPI_CmdConfig(QSPI_CmdTypeDef *QSPI_CmdStruct);
FlagStatus QSPI_GetFlagStatus(uint32_t QSPI_FLAG);
void QSPI_ClearFlag(uint32_t QSPI_FLAG_CLEAR);
uint8_t QSPI_GetFIFOLevel(void);
void QSPI_SetAddr(uint32_t QSPI_Addr);
void QSPI_SetAltBytes(uint32_t QSPI_AltBytes);
void QSPI_SetData(uint32_t QSPI_Data);
uint32_t QSPI_ReadData(void);
void QSPI_SetDataLength(uint32_t QSPI_DataLength);
void QSPI_SetTimeout(uint16_t QSPI_Timeout);
void QSPI_SetPollingInterval(uint16_t QSPI_PollingInterval);
void QSPI_SetPollingStaMatch(uint32_t QSPI_MatchData);
void QSPI_PollingStaMask(uint32_t QSPI_MatchDataMask);

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
