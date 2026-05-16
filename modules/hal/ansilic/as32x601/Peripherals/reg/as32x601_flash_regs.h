/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_flash_regs.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2024/08/14
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/08/14     1.0     build this module
 * Description:     This file contains all the register correlation defines for the Flash firmware library.
 */

#ifndef AS32X601_FLASH_REG_H
#define AS32X601_FLASH_REG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601.h"
#include "as32x601_addrmap.h"

/*
 * @brief General Purpose I/O registers structure
 */
typedef struct
{
    __IO uint32_t SR;           /* EFLASH_STATE     EFlash state register,                      Address offset: 0x00 */
    __IO uint32_t UNLKR;        /* EFLASH_KEY       EFlash Unlock register,                     Address offset: 0x04 */
    __IO uint32_t CR;           /* EFLASH_CNFG      EFlash configuration register,              Address offset: 0x08 */
    __IO uint32_t CIDR;         /* EFLASH_CMD       EFlash command ID register,                 Address offset: 0x0C */
    __IO uint32_t AR;           /* EFLASH_ADDR      EFlash address register,                    Address offset: 0x10 */
    __IO uint32_t DLR;          /* EFLASH_LEN       EFlash Data length register,                Address offset: 0x14 */
    uint32_t RESERVED0[2];      /* Reserved,                                                    Address offset: 0x18 - 0x1C */
    __IO uint32_t DR0;          /* EFLASH_DATA0     EFlash data register 0,                     Address offset: 0x20 */
    __IO uint32_t DR1;          /* EFLASH_DATA1     EFlash data register 1,                     Address offset: 0x24 */
    uint32_t RESERVED1[2];      /* Reserved,                                                    Address offset: 0x28 - 0x2C */
    __IO uint32_t CTR;          /* EFLASH_START     EFlash command triggers register,           Address offset: 0x30 */
    __IO uint32_t PWPR;         /* EFLASH_PPROT     EFlash P-Flash Write protected register,    Address offset: 0x34 */
    __IO uint32_t DWPR;         /* EFLASH_DPROT     EFlash D-Flash Write protected register,    Address offset: 0x38 */
    __IO uint32_t EAR;          /* EFLASH_ECC_ADDR  EFlash ECC 2-bit error address register,    Address offset: 0x3C */
    __I  uint32_t SECR;         /* EFLASH_SEC       EFlash security register,                   Address offset: 0x40 */
    __IO uint32_t SECKEY0;      /* EFLASH_SECKEY0   EFlash security key register 0,             Address offset: 0x44 */
    __IO uint32_t SECKEY1;      /* EFLASH_SECKEY1   EFlash security key register 1,             Address offset: 0x48 */
} FLASH_TypeDef;

/*
 * @addtogroup FLASH Peripheral declaration
 */
#define EFLASH ((FLASH_TypeDef *)FLASH_CTRL_BASE)

/******************  Bits definition for EFLASH_SR register  *****************/
#define EFLASH_SR_LOCK_DPos             (0U)
#define EFLASH_SR_LOCK_Pos              EFLASH_SR_LOCK_DPos
#define EFLASH_SR_LOCK_Msk              (0x01UL << EFLASH_SR_LOCK_Pos)          /* 0x00000001 */
#define EFLASH_SR_LOCK                  EFLASH_SR_LOCK_Msk                      /* Lock status flag */

#define EFLASH_SR_BUSY_DPos             (1U)
#define EFLASH_SR_BUSY_Pos              EFLASH_SR_BUSY_DPos
#define EFLASH_SR_BUSY_Msk              (0x01UL << EFLASH_SR_BUSY_Pos)          /* 0x00000002 */
#define EFLASH_SR_BUSY                  EFLASH_SR_BUSY_Msk                      /* Busy flag */

#define EFLASH_SR_FINISH_DPos           (2U)
#define EFLASH_SR_FINISH_Pos            EFLASH_SR_FINISH_DPos
#define EFLASH_SR_FINISH_Msk            (0x01UL << EFLASH_SR_FINISH_Pos)        /* 0x00000004 */
#define EFLASH_SR_FINISH                EFLASH_SR_FINISH_Msk                    /* Command finish flag */

#define EFLASH_SR_OPERR_DPos            (3U)
#define EFLASH_SR_OPERR_Pos             EFLASH_SR_OPERR_DPos
#define EFLASH_SR_OPERR_Msk             (0x01UL << EFLASH_SR_OPERR_Pos)         /* 0x00000008 */
#define EFLASH_SR_OPERR                 EFLASH_SR_OPERR_Msk                     /* Operation error flag */

#define EFLASH_SR_ECC1ERR_DPos          (4U)
#define EFLASH_SR_ECC1ERR_Pos           EFLASH_SR_ECC1ERR_DPos
#define EFLASH_SR_ECC1ERR_Msk           (0x01UL << EFLASH_SR_ECC1ERR_Pos)       /* 0x00000010 */
#define EFLASH_SR_ECC1ERR               EFLASH_SR_ECC1ERR_Msk                   /* ECC 1-bit error flag */

#define EFLASH_SR_ECC2ERR_DPos          (5U)
#define EFLASH_SR_ECC2ERR_Pos           EFLASH_SR_ECC2ERR_DPos
#define EFLASH_SR_ECC2ERR_Msk           (0x01UL << EFLASH_SR_ECC2ERR_Pos)       /* 0x00000020 */
#define EFLASH_SR_ECC2ERR               EFLASH_SR_ECC2ERR_Msk                   /* ECC 2-bit error flag */

#define EFLASH_SR_WPERR_DPos            (6U)
#define EFLASH_SR_WPERR_Pos             EFLASH_SR_WPERR_DPos
#define EFLASH_SR_WPERR_Msk             (0x01UL << EFLASH_SR_WPERR_Pos)         /* 0x00000040 */
#define EFLASH_SR_WPERR                 EFLASH_SR_WPERR_Msk                     /* Write protection error flag */

#define EFLASH_SR_SECKEYERR_DPos        (7U)
#define EFLASH_SR_SECKEYERR_Pos         EFLASH_SR_SECKEYERR_DPos
#define EFLASH_SR_SECKEYERR_Msk         (0x01UL << EFLASH_SR_SECKEYERR_Pos)     /* 0x00000080 */
#define EFLASH_SR_SECKEYERR             EFLASH_SR_SECKEYERR_Msk                 /* Flash Security Key verification error flag */

#define EFLASH_SR_CLKSWRDY_DPos         (16U)
#define EFLASH_SR_CLKSWRDY_Pos          EFLASH_SR_CLKSWRDY_DPos
#define EFLASH_SR_CLKSWRDY_Msk          (0x01UL << EFLASH_SR_CLKSWRDY_Pos)      /* 0x00010000 */
#define EFLASH_SR_CLKSWRDY              EFLASH_SR_CLKSWRDY_Msk                  /* Flash clock switching status flag */


/******************  Bits definition for EFLASH_UNLKR register  *****************/
#define EFLASH_UNLKR_KEY_DPos           (0U)
#define EFLASH_UNLKR_UNLKR_Pos          EFLASH_UNLKR_KEY_DPos
#define EFLASH_UNLKR_UNLKR_Msk          (0xFFFFFFFFUL << EFLASH_UNLKR_UNLKR_Pos)    /* 0xFFFFFFFF */
#define EFLASH_UNLKR_UNLKR              EFLASH_UNLKR_UNLKR_Msk                      /* UNLKR[31:0] bits (Unlock flash) */

/******************  Bits definition for EFLASH_CR register  *****************/
#define EFLASH_CR_FINIE_DPos            (0U)
#define EFLASH_CR_FINIE_Pos             EFLASH_CR_FINIE_DPos
#define EFLASH_CR_FINIE_Msk             (0x01UL << EFLASH_CR_FINIE_Pos)         /* 0x00000001 */
#define EFLASH_CR_FINIE                 EFLASH_CR_FINIE_Msk                     /* command finish interrupt enable */

#define EFLASH_CR_OPEIE_DPos            (1U)
#define EFLASH_CR_OPEIE_Pos             EFLASH_CR_OPEIE_DPos
#define EFLASH_CR_OPEIE_Msk             (0x01UL << EFLASH_CR_OPEIE_Pos)         /* 0x00000002 */
#define EFLASH_CR_OPEIE                 EFLASH_CR_OPEIE_Msk                     /* operation error interrupt enable */

#define EFLASH_CR_ECC2EIE_DPos          (2U)
#define EFLASH_CR_ECC2EIE_Pos           EFLASH_CR_ECC2EIE_DPos
#define EFLASH_CR_ECC2EIE_Msk           (0x01UL << EFLASH_CR_ECC2EIE_Pos)       /* 0x00000004 */
#define EFLASH_CR_ECC2EIE               EFLASH_CR_ECC2EIE_Msk                   /* ECC 2 bit error interrupt enable */

#define EFLASH_CR_CLKFRQ_DPos           (16U)
#define EFLASH_CR_CLKFRQ_Pos            EFLASH_CR_CLKFRQ_DPos
#define EFLASH_CR_CLKFRQ_Msk            (0xFFFFUL << EFLASH_CR_CLKFRQ_Pos)      /* 0xFFFF0000 */
#define EFLASH_CR_CLKFRQ                EFLASH_CR_CLKFRQ_Msk                    /* CLKFRQ[15:0] bits (Clock frequency) */

/******************  Bits definition for EFLASH_CIDR register  *****************/
#define EFLASH_CIDR_CMD_DPos            (0U)
#define EFLASH_CIDR_CIDR_Pos            EFLASH_CIDR_CMD_DPos
#define EFLASH_CIDR_CIDR_Msk            (0xFFUL << EFLASH_CIDR_CIDR_Pos)        /* 0x000000FF */
#define EFLASH_CIDR_CIDR                EFLASH_CIDR_CIDR_Msk                    /* CIDR[7:0] bits (Command ID) */

/******************  Bits definition for EFLASH_AR register  *****************/
#define EFLASH_AR_ADDR_DPos             (0U)
#define EFLASH_AR_AR_Pos                EFLASH_AR_ADDR_DPos
#define EFLASH_AR_AR_Msk                (0xFFFFFFFFUL << EFLASH_AR_AR_Pos)      /* 0xFFFFFFFF */
#define EFLASH_AR_AR                    EFLASH_AR_AR_Msk                        /* AR[31:0] bits (Operation address) */

/******************  Bits definition for EFLASH_DLR register  *****************/
#define EFLASH_DLR_LEN_DPos             (0U)
#define EFLASH_DLR_DLR_Pos              EFLASH_DLR_LEN_DPos
#define EFLASH_DLR_DLR_Msk              (0xFFUL << EFLASH_DLR_DLR_Pos)          /* 0x000000FF */
#define EFLASH_DLR_DLR                  EFLASH_DLR_DLR_Msk                      /* DLR[31:0] bits (Data length) */

/******************  Bits definition for EFLASH_DR0 register  *****************/
#define EFLASH_DR0_DATA0_DPos           (0U)
#define EFLASH_DR0_DR0_Pos              EFLASH_DR0_DATA0_DPos
#define EFLASH_DR0_DR0_Msk              (0xFFFFFFFFUL << EFLASH_DR0_DR0_Pos)    /* 0xFFFFFFFF */
#define EFLASH_DR0_DR0                  EFLASH_DR0_DR0_Msk                      /* DR0[31:0] bits (Data register low 32 bit) */

/******************  Bits definition for EFLASH_DR1 register  *****************/
#define EFLASH_DR1_DATA1_DPos           (0U)
#define EFLASH_DR1_DR1_Pos              EFLASH_DR1_DATA1_DPos
#define EFLASH_DR1_DR1_Msk              (0xFFFFFFFFUL << EFLASH_DR1_DR1_Pos)    /* 0xFFFFFFFF */
#define EFLASH_DR1_DR1                  EFLASH_DR1_DR1_Msk                      /* DR1[31:0] bits (Data register high 32 bit) */

/******************  Bits definition for EFLASH_CTR register  *****************/
#define EFLASH_CTR_START_DPos           (0U)
#define EFLASH_CTR_CTR_Pos              EFLASH_CTR_START_DPos
#define EFLASH_CTR_CTR_Msk              (0x01UL << EFLASH_CTR_CTR_Pos)          /* 0x00000000 */
#define EFLASH_CTR_CTR                  EFLASH_CTR_CTR_Msk                      /* Cmmand start */

/******************  Bits definition for EFLASH_PWPR register  *****************/
#define EFLASH_PWPR_PPROT_DPos          (0U)
#define EFLASH_PWPR_PWPR_Pos            EFLASH_PWPR_PPROT_DPos
#define EFLASH_PWPR_PWPR_Msk            (0xFFFFFFFFUL << EFLASH_PWPR_PWPR_Pos)  /* 0xFFFFFFFF */
#define EFLASH_PWPR_PWPR                EFLASH_PWPR_PWPR_Msk                    /* PWPR[31:0] bits (PFlash write protection status value) */

/******************  Bits definition for EFLASH_DWPR register  *****************/
#define EFLASH_DWPR_DPROT_DPos          (0U)
#define EFLASH_DWPR_DWPR_Pos            EFLASH_DWPR_DPROT_DPos
#define EFLASH_DWPR_DWPR_Msk            (0xFFFFFFFFUL << EFLASH_DWPR_DWPR_Pos)  /* 0xFFFFFFFF */
#define EFLASH_DWPR_DWPR                EFLASH_DWPR_DWPR_Msk                    /* DWPR[31:0] bits (DFlash write protection status value) */

/******************  Bits definition for EFLASH_EAR register  *****************/
#define EFLASH_EAR_ECC_ADDR_DPos        (0U)
#define EFLASH_EAR_EAR_Pos              EFLASH_EAR_ECC_ADDR_DPos
#define EFLASH_EAR_EAR_Msk              (0xFFFFFFFFUL << EFLASH_EAR_EAR_Pos)    /* 0xFFFFFFFF */
#define EFLASH_EAR_EAR                  EFLASH_EAR_EAR_Msk                      /* EAR[31:0] bits (ECC 2bit error address) */

/******************  Bits definition for EFLASH_SECR register  *****************/
#define EFLASH_SECR_RPROT_DPos          (0U)
#define EFLASH_SECR_RPROT_Pos           EFLASH_SECR_RPROT_DPos
#define EFLASH_SECR_RPROT_Msk           (0x03UL << EFLASH_SECR_RPROT_Pos)       /* 0x00000003 */
#define EFLASH_SECR_RPROT               EFLASH_SECR_RPROT_Msk                   /* RPROT[1:0] bits (Flash read protection enable flag) */

#define EFLASH_SECR_SECKEYEN_DPos       (0U)
#define EFLASH_SECR_SECKEYEN_Pos        EFLASH_SECR_SECKEYEN_DPos
#define EFLASH_SECR_SECKEYEN_Msk        (0x03UL << EFLASH_SECR_SECKEYEN_Pos)    /* 0x0000000C */
#define EFLASH_SECR_SECKEYEN            EFLASH_SECR_SECKEYEN_Msk                /* RPROT[3:2] bits (Flash security key enable flag) */


/******************  Bits definition for EFLASH_SECKEY0 register  *****************/
#define EFLASH_SECKEY0_SECKEY0_DPos     (0U)
#define EFLASH_SECKEY0_SECKEY0_Pos      EFLASH_SECKEY0_SECKEY0_DPos
#define EFLASH_SECKEY0_SECKEY0_Msk      (0xFFFFFFFFUL << EFLASH_SECKEY0_SECKEY0_Pos)    /* 0xFFFFFFFF */
#define EFLASH_SECKEY0_SECKEY0          EFLASH_SECKEY0_SECKEY0_Msk                      /* SECKEY0[31:0] bits (Flash security key low 32 bits) */

/******************  Bits definition for EFLASH_SECKEY0 register  *****************/
#define EFLASH_SECKEY1_SECKEY1_DPos     (0U)
#define EFLASH_SECKEY1_SECKEY1_Pos      EFLASH_SECKEY1_SECKEY1_DPos
#define EFLASH_SECKEY1_SECKEY1_Msk      (0xFFFFFFFFUL << EFLASH_SECKEY1_SECKEY1_Pos)    /* 0xFFFFFFFF */
#define EFLASH_SECKEY1_SECKEY1          EFLASH_SECKEY1_SECKEY1_Msk                      /* SECKEY1[31:0] bits (Flash security key high 32 bits) */

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
