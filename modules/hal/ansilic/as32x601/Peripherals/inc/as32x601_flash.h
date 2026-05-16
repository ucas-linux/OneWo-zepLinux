/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):   Xiamen Guoke Anxin Technology Co., LTD
 * FileName:        as32x601_gpio.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:			2024/08/14
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/08/14     1.0     build this module
 * Description:     This file contains all the functions prototypes for the GPIO firmware library.
 */

#ifndef AS32X601_FLASH_H
#define AS32X601_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601_flash_regs.h"

/*
 * @defgroup Flash controler unlock key definition
 */
#define FLASH_UNLOCK_KEY1 ((uint32_t)0x01020304)
#define FLASH_UNLOCK_KEY2 ((uint32_t)0x0A0B0C0D)

/*
 * @defgroup Flash information definition
 */
#define DFLASH_MAIN_SIZE       (512 * 1024UL)
#define DFLASH_INFO_SIZE       (4 * 1024UL)
#define PFLASH_MAIN_SIZE       (4 * 512 * 1024UL)
#define PFLASH_INFO_SIZE       (4 * 4 * 1024UL)
#define DFLASH_BLOCK_SIZE      (512 * 1024UL)
#define PFLASH_BLOCK_SIZE      (512 * 1024UL)
#define DFLASH_SECTOR_SIZE     (4 * 1024UL)
#define PFLASH_SECTOR_SIZE     (4 * 1024UL)
#define DFLASH_ROM_SIZE        (512UL)
#define PFLASH_ROM_SIZE        (512UL)
#define EFLASH_PROGRAM_MAXLEN  (64UL)
#define EFLASH_WRITE_UNIT_SIZE (8UL)

/*
 * @brief  Flash private state enumeration
 */
typedef enum 
{
    FLASH_SUCCESS = 0x00,	/* Success */
    FLASH_ERROR	  = 0x01,	/* Error */
    FLASH_BUSY    = 0x02 	/* Busy */
} FLASHStatus_TypeDef;

/*
 * @brief  Flash command enumeration
 */
typedef enum 
{
    FLASH_Cmd_SectorErase       = 0x01,     /* Sector erase */
    FLASH_Cmd_BlockErase        = 0x02,     /* blocks erase */
    FLASH_Cmd_WPInfoSectorErase = 0x03,     /* Write protection information sector erase */
    FLASH_Cmd_AllBlockErase     = 0x04,     /* All blocks erase */
    FLASH_Cmd_Program           = 0x10,     /* Program */
    FLASH_Cmd_SecKEYVerif       = 0x20      /* Security Key Verification */
} FLASHCmd_TypeDef;
#define IS_FLASH_CMD(CMD) (((CMD) == FLASH_Cmd_SectorErase)       || ((CMD) == FLASH_Cmd_BlockErase)    || \
                           ((CMD) == FLASH_Cmd_WPInfoSectorErase) || ((CMD) == FLASH_Cmd_AllBlockErase) || \
                           ((CMD) == FLASH_Cmd_Program)           || ((CMD) == FLASH_Cmd_SecKEYVerif))

/*
 * @defgroup Flash status definition
 */
#define FLASH_FLAG_LOCK         ((uint32_t)0x00000001)      /* Flash lock status flag */
#define FLASH_FLAG_BUSY         ((uint32_t)0x00000002)      /* Flash busy flag */
#define FLASH_FLAG_FINISH       ((uint32_t)0x00000004)      /* Flash command finish flag */
#define FLASH_FLAG_OPERR        ((uint32_t)0x00000008)      /* Flash operation error flag */
#define FLASH_FLAG_ECC1ERR      ((uint32_t)0x00000010)      /* Flash ECC 1-bit error flag */
#define FLASH_FLAG_ECC2ERR      ((uint32_t)0x00000020)      /* Flash ECC 2-bit error flag */
#define FLASH_FLAG_WPERR        ((uint32_t)0x00000040)      /* Flash write protection error flag */
#define FLASH_FLAG_SECKEYERR    ((uint32_t)0x00000080)      /* Flash security key verification error flag */
#define FLASH_FLAG_CLKSWRDY     ((uint32_t)0x00010000)      /* Flash clock switching status flag */
#define IS_FLASH_FLAG(FLAG) (((FLAG) == FLASH_FLAG_LOCK)    || ((FLAG) == FLASH_FLAG_BUSY)      || \
                             ((FLAG) == FLASH_FLAG_FINISH)  || ((FLAG) == FLASH_FLAG_OPERR)     || \
                             ((FLAG) == FLASH_FLAG_ECC1ERR) || ((FLAG) == FLASH_FLAG_ECC2ERR)   || \
                             ((FLAG) == FLASH_FLAG_WPERR)   || ((FLAG) == FLASH_FLAG_SECKEYERR) || \
                             ((FLAG) == FLASH_FLAG_CLKSWRDY))

/*
 * @defgroup Flash Clear Flags
 */
#define FLASH_CLEAR_FINISH      ((uint32_t)0x00000004)      /* Flash command finish clear flag */
#define FLASH_CLEAR_OPERR       ((uint32_t)0x00000008)      /* Flash operation error clear flag */
#define FLASH_CLEAR_ECC1ERR     ((uint32_t)0x00000010)      /* Flash ECC 1-bit error clear flag */
#define FLASH_CLEAR_ECC2ERR     ((uint32_t)0x00000020)      /* Flash ECC 2-bit error clear flag */
#define FLASH_CLEAR_WPERR       ((uint32_t)0x00000040)      /* Flash write protection error clear flag */
#define IS_FLASH_CLEAR(CLEAR) (((CLEAR) == FLASH_CLEAR_FINISH)  || ((CLEAR) == FLASH_CLEAR_OPERR)   || \
                               ((CLEAR) == FLASH_CLEAR_ECC1ERR) || ((CLEAR) == FLASH_CLEAR_ECC2ERR) || \
                               ((CLEAR) == FLASH_CLEAR_WPERR))

/*
 * @defgroup Flash interrupts definition
 */
#define FLASH_IT_FINIE          ((uint32_t)0x00000001)      /* Flash command finish interrupt enable */
#define FLASH_IT_OPEIE          ((uint32_t)0x00000002)      /* Flash operation error interrupt enable */
#define FLASH_IT_ECC2EIE        ((uint32_t)0x00000004)      /* Flash ECC 2-bit error interrupt enable */
#define IS_FLASH_IT(IT)	(((IT) == FLASH_IT_FINIE) || ((IT) == FLASH_IT_OPEIE) || \
                         ((IT) == FLASH_IT_ECC2EIE))

/*
 * @defgroup Flash Interruption Clear Flags
 */
#define FLASH_CLEAR_IT_FINISH   ((uint32_t)0x00000004)      /* Flash command finish clear flag */
#define FLASH_CLEAR_IT_OPERR    ((uint32_t)0x00000008)      /* Flash operation error clear flag */
#define FLASH_CLEAR_IT_ECC2ERR  ((uint32_t)0x00000020)      /* Flash ECC 2-bit error clear flag */
#define IS_FLASH_CLEAR_IT(CLEAR) (((CLEAR) == FLASH_CLEAR_IT_FINISH) || ((CLEAR) == FLASH_CLEAR_IT_OPERR) || \
                                  ((CLEAR) == FLASH_CLEAR_IT_ECC2ERR))

/* @defgroup FLASH status error bits */
#define FLASH_STATE_ERROR_BITS      (EFLASH_SR_OPERR | EFLASH_SR_ECC1ERR | \
                                     EFLASH_SR_ECC2ERR | EFLASH_SR_WPERR | \
                                     EFLASH_SR_SECKEYERR)

/*
 * @defgroup Flash memory protection definition
 */
#define FLASH_WP_AREA1          ((uint32_t)0x00000001)      /* Write protection area 1 */
#define FLASH_WP_AREA2          ((uint32_t)0x00000002)      /* Write protection area 2 */
#define FLASH_WP_AREA3          ((uint32_t)0x00000004)      /* Write protection area 3 */
#define FLASH_WP_AREA4          ((uint32_t)0x00000008)      /* Write protection area 4 */
#define FLASH_WP_AREA5          ((uint32_t)0x00000010)      /* Write protection area 5 */
#define FLASH_WP_AREA6          ((uint32_t)0x00000020)      /* Write protection area 6 */
#define FLASH_WP_AREA7          ((uint32_t)0x00000040)      /* Write protection area 7 */
#define FLASH_WP_AREA8          ((uint32_t)0x00000080)      /* Write protection area 8 */
#define FLASH_WP_AREA9          ((uint32_t)0x00000100)      /* Write protection area 9 */
#define FLASH_WP_AREA10         ((uint32_t)0x00000200)      /* Write protection area 10 */
#define FLASH_WP_AREA11         ((uint32_t)0x00000400)      /* Write protection area 11 */
#define FLASH_WP_AREA12         ((uint32_t)0x00000800)      /* Write protection area 12 */
#define FLASH_WP_AREA13         ((uint32_t)0x00001000)      /* Write protection area 13 */
#define FLASH_WP_AREA14         ((uint32_t)0x00002000)      /* Write protection area 14 */
#define FLASH_WP_AREA15         ((uint32_t)0x00004000)      /* Write protection area 15 */
#define FLASH_WP_AREA16         ((uint32_t)0x00008000)      /* Write protection area 16 */
#define FLASH_WP_AREA17         ((uint32_t)0x00010000)      /* Write protection area 17 */
#define FLASH_WP_AREA18         ((uint32_t)0x00020000)      /* Write protection area 18 */
#define FLASH_WP_AREA19         ((uint32_t)0x00040000)      /* Write protection area 19 */
#define FLASH_WP_AREA20         ((uint32_t)0x00080000)      /* Write protection area 20 */
#define FLASH_WP_AREA21         ((uint32_t)0x00100000)      /* Write protection area 21 */
#define FLASH_WP_AREA22         ((uint32_t)0x00200000)      /* Write protection area 22 */
#define FLASH_WP_AREA23         ((uint32_t)0x00400000)      /* Write protection area 23 */
#define FLASH_WP_AREA24         ((uint32_t)0x00800000)      /* Write protection area 24 */
#define FLASH_WP_AREA25         ((uint32_t)0x01000000)      /* Write protection area 25 */
#define FLASH_WP_AREA26         ((uint32_t)0x02000000)      /* Write protection area 26 */
#define FLASH_WP_AREA27         ((uint32_t)0x04000000)      /* Write protection area 27 */
#define FLASH_WP_AREA28         ((uint32_t)0x08000000)      /* Write protection area 28 */
#define FLASH_WP_AREA29         ((uint32_t)0x10000000)      /* Write protection area 29 */
#define FLASH_WP_AREA30         ((uint32_t)0x20000000)      /* Write protection area 30 */
#define FLASH_WP_AREA31         ((uint32_t)0x40000000)      /* Write protection area 31 */
#define FLASH_WP_AREA32         ((uint32_t)0x80000000)      /* Write protection area 32 */
#define IS_PFLASH_WP_AREAx(AREAx) (((AREAx) == FLASH_WP_AREA1)  || ((AREAx) == FLASH_WP_AREA2)  || \
                                   ((AREAx) == FLASH_WP_AREA3)  || ((AREAx) == FLASH_WP_AREA4)  || \
                                   ((AREAx) == FLASH_WP_AREA5)  || ((AREAx) == FLASH_WP_AREA6)  || \
                                   ((AREAx) == FLASH_WP_AREA7)  || ((AREAx) == FLASH_WP_AREA8)  || \
                                   ((AREAx) == FLASH_WP_AREA9)  || ((AREAx) == FLASH_WP_AREA10) || \
                                   ((AREAx) == FLASH_WP_AREA11) || ((AREAx) == FLASH_WP_AREA12) || \
                                   ((AREAx) == FLASH_WP_AREA13) || ((AREAx) == FLASH_WP_AREA14) || \
                                   ((AREAx) == FLASH_WP_AREA15) || ((AREAx) == FLASH_WP_AREA16) || \
                                   ((AREAx) == FLASH_WP_AREA17) || ((AREAx) == FLASH_WP_AREA18) || \
                                   ((AREAx) == FLASH_WP_AREA19) || ((AREAx) == FLASH_WP_AREA20) || \
                                   ((AREAx) == FLASH_WP_AREA21) || ((AREAx) == FLASH_WP_AREA22) || \
                                   ((AREAx) == FLASH_WP_AREA23) || ((AREAx) == FLASH_WP_AREA24) || \
                                   ((AREAx) == FLASH_WP_AREA25) || ((AREAx) == FLASH_WP_AREA26) || \
                                   ((AREAx) == FLASH_WP_AREA27) || ((AREAx) == FLASH_WP_AREA28) || \
                                   ((AREAx) == FLASH_WP_AREA29) || ((AREAx) == FLASH_WP_AREA30) || \
                                   ((AREAx) == FLASH_WP_AREA31) || ((AREAx) == FLASH_WP_AREA32))
#define IS_DFLASH_WP_AREAx(AREAx) (((AREAx) == FLASH_WP_AREA1) || ((AREAx) == FLASH_WP_AREA2) || \
                                   ((AREAx) == FLASH_WP_AREA3) || ((AREAx) == FLASH_WP_AREA4) || \
                                   ((AREAx) == FLASH_WP_AREA5) || ((AREAx) == FLASH_WP_AREA6) || \
                                   ((AREAx) == FLASH_WP_AREA7) || ((AREAx) == FLASH_WP_AREA8))

/*
 * @defgroup Flash security status definition
 */
#define FLASH_SECFLAG_KEY       ((uint32_t)0x0000000C)      /* Flash security Key flag */
#define FLASH_SECFLAG_RP        ((uint32_t)0x00000003)      /* Flash read protection flag */
#define IS_FLASH_SECFLAG(SECFLAG) (((SECFLAG) == FLASH_SECFLAG_KEY) || ((SECFLAG) == FLASH_SECFLAG_RP))

/*
 * @brief Function declaration
 */
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG);
void FLASH_ClearFlagStatus(uint32_t FLASH_FLAG);
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState);
void FLASH_ClearITPendingBit(uint32_t FLASH_IT);
void FLASH_UnlockCtrl(void);
void FLASH_LockCtrl(void);
void FLASH_SetCLKFreq(uint16_t CLK_Frequence);
FLASHStatus_TypeDef FLASH_EraseSector(uint32_t Addr, uint32_t Size);
FLASHStatus_TypeDef FLASH_EraseBlock(uint32_t Addr);
FLASHStatus_TypeDef FLASH_EraseWriteProtection(void);
FLASHStatus_TypeDef FLASH_EraseAllBlock(void);
FLASHStatus_TypeDef FLASH_ProgramRom(uint32_t Addr, uint32_t Size, const uint32_t *Data);
FLASHStatus_TypeDef FLASH_ProgramInfo(uint32_t Addr, uint32_t Size, const uint32_t *Data);
FLASHStatus_TypeDef FLASH_Read(uint32_t Addr, uint32_t Size, uint32_t *Data);
FlagStatus FLASH_GetPFlashWriteProtStatus(uint32_t ProtectArea);
FLASHStatus_TypeDef FLASH_SetPFlashWriteProtection(uint32_t ProtectArea);
FLASHStatus_TypeDef FLASH_ResetPFlashWriteProtection(uint32_t ProtectArea);
FlagStatus FLASH_GetDFlashWriteProtStatus(uint32_t ProtectArea);
FLASHStatus_TypeDef FLASH_SetDFlashWriteProtection(uint32_t ProtectArea);
FLASHStatus_TypeDef FLASH_ResetDFlashWriteProtection(uint32_t ProtectArea);
uint32_t FLASH_GetECCErrorAddress(void);
void FLASH_ClearECCErrorAddress(void);
FlagStatus FLASH_GetSECFlagStatus(uint32_t FLASH_SECFLAG);
FLASHStatus_TypeDef FLASH_InputSECKEY(uint32_t SEC_KEY0, uint32_t SEC_KEY1);

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
