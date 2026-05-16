/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_smu_regs.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2023/11/21
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2023/11/21     1.0     build this module
 * Description:     This file contains all the register correlation defines for the SMU firmware library.
 */

#ifndef AS32X601_SMU_REG_H
#define AS32X601_SMU_REG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601_addrmap.h"
#include "as32x601.h"
#include <stdio.h>

/*
 * @brief System Management Unit registers structure
 */
typedef struct
{
    __IO uint32_t PLLCFGR;      /* SMU_pllCfgReg        PLL divide and reset configuration registers,       Address offset: 0x00 */
    __IO uint32_t FIRCCFGR;     /* SMU_fircCfgReg       Internal clock and reset configuration registers,   Address offset: 0x04 */
    __IO uint32_t BUSCFGR;      /* SMU_busClkDivReg     BUS clock and reset configuration registers,        Address offset: 0x08 */
    __I  uint32_t SR;           /* SMU_smuState         SMU state registers,                                Address offset: 0x0C */
    __IO uint32_t AB0CFGR;      /* SMU_ab0CfgReg        AB0 bus clock and reset configuration register,     Address offset: 0x10 */
    __IO uint32_t AB1CFGR;      /* SMU_ab1CfgReg        AB1 bus clock and reset configuration register,     Address offset: 0x14 */
    __IO uint32_t AB3CFGR;      /* SMU_ab3CfgReg        AB3 bus clock and reset configuration registers,    Address offset: 0x18 */
    __IO uint32_t ALB0CFGR;     /* SMU_alb0CfgReg       ALB0 bus clock and reset configuration registers,   Address offset: 0x1C */
    __IO uint32_t ALB1CFGR;     /* SMU_alb1CfgReg       ALB1 bus clock and reset configuration registers,   Address offset: 0x20 */
    __IO uint32_t ALB2CFGR;     /* SMU_alb2CfgReg       ALB2 bus clock and reset configuration registers,   Address offset: 0x24 */
    __IO uint32_t APB0CFGR;     /* SMU_apb0CfgReg       APB0 bus clock and reset configuration registers,   Address offset: 0x28 */
    __IO uint32_t APB1CFGR;     /* SMU_apb1CfgReg       APB1 bus clock and reset configuration registers,   Address offset: 0x2c */
    __IO uint32_t OTHCFGR;      /* SMU_othCfgReg        Other modules configuration registers,              Address offset: 0x30 */
    __IO uint32_t BUSTCFGR;     /* SMU_busMonReg        BUS timeout configuration registers,                Address offset: 0x34 */
} SMU_TypeDef;

/*
 * @addtogroup SMU peripheral declaration
 */
#define SMU ((SMU_TypeDef *)SMU_BASE)


/******************  Bits definition for SMU_PLLCFGR register  *****************/
#define SMU_PLLCFGR_pllDivN_DPos        (0U)
#define SMU_PLLCFGR_PLLN_Pos            SMU_PLLCFGR_pllDivN_DPos
#define SMU_PLLCFGR_PLLN_Msk            (0x3FUL << SMU_PLLCFGR_PLLN_Pos)        /* 0x0000003F */
#define SMU_PLLCFGR_PLLN                SMU_PLLCFGR_PLLN_Msk                    /* PLLN[5:0] bits (Input frequency division control signal) */
#define SMU_PLLCFGR_PLLN_0              (0x01UL << SMU_PLLCFGR_PLLN_Pos)        /* 0x00000001 */
#define SMU_PLLCFGR_PLLN_1              (0x02UL << SMU_PLLCFGR_PLLN_Pos)        /* 0x00000002 */
#define SMU_PLLCFGR_PLLN_2              (0x04UL << SMU_PLLCFGR_PLLN_Pos)        /* 0x00000004 */
#define SMU_PLLCFGR_PLLN_3              (0x08UL << SMU_PLLCFGR_PLLN_Pos)        /* 0x00000008 */
#define SMU_PLLCFGR_PLLN_4              (0x10UL << SMU_PLLCFGR_PLLN_Pos)        /* 0x00000010 */
#define SMU_PLLCFGR_PLLN_5              (0x20UL << SMU_PLLCFGR_PLLN_Pos)        /* 0x00000020 */

#define SMU_PLLCFGR_pllDivF_DPos        (6U)
#define SMU_PLLCFGR_PLLF_Pos            SMU_PLLCFGR_pllDivF_DPos
#define SMU_PLLCFGR_PLLF_Msk            (0x1FFUL << SMU_PLLCFGR_PLLF_Pos)       /* 0x00007FC0 */
#define SMU_PLLCFGR_PLLF                SMU_PLLCFGR_PLLF_Msk                    /* PLLF[8:0] bits (Feedback clock control signal) */
#define SMU_PLLCFGR_PLLF_0              (0x01UL << SMU_PLLCFGR_PLLF_Pos)        /* 0x00000040 */
#define SMU_PLLCFGR_PLLF_1              (0x02UL << SMU_PLLCFGR_PLLF_Pos)        /* 0x00000080 */
#define SMU_PLLCFGR_PLLF_2              (0x04UL << SMU_PLLCFGR_PLLF_Pos)        /* 0x00000100 */
#define SMU_PLLCFGR_PLLF_3              (0x08UL << SMU_PLLCFGR_PLLF_Pos)        /* 0x00000200 */
#define SMU_PLLCFGR_PLLF_4              (0x10UL << SMU_PLLCFGR_PLLF_Pos)        /* 0x00000400 */
#define SMU_PLLCFGR_PLLF_5              (0x20UL << SMU_PLLCFGR_PLLF_Pos)        /* 0x00000800 */
#define SMU_PLLCFGR_PLLF_6              (0x40UL << SMU_PLLCFGR_PLLF_Pos)        /* 0x00001000 */
#define SMU_PLLCFGR_PLLF_7              (0x80UL << SMU_PLLCFGR_PLLF_Pos)        /* 0x00002000 */
#define SMU_PLLCFGR_PLLF_8              (0x100UL << SMU_PLLCFGR_PLLF_Pos)       /* 0x00004000 */

#define SMU_PLLCFGR_pllDivQ_DPos        (15U)
#define SMU_PLLCFGR_PLLQ_Pos            SMU_PLLCFGR_pllDivQ_DPos
#define SMU_PLLCFGR_PLLQ_Msk            (0xFUL << SMU_PLLCFGR_PLLQ_Pos)         /* 0x00078000 */
#define SMU_PLLCFGR_PLLQ                SMU_PLLCFGR_PLLQ_Msk                    /* PLLQ[4:0] bits (Output frequency division control signal of CLK_Q) */
#define SMU_PLLCFGR_PLLQ_0              (0x01UL << SMU_PLLCFGR_PLLQ_Pos)        /* 0x00008000 */
#define SMU_PLLCFGR_PLLQ_1              (0x02UL << SMU_PLLCFGR_PLLQ_Pos)        /* 0x00010000 */
#define SMU_PLLCFGR_PLLQ_2              (0x04UL << SMU_PLLCFGR_PLLQ_Pos)        /* 0x00020000 */
#define SMU_PLLCFGR_PLLQ_3              (0x08UL << SMU_PLLCFGR_PLLQ_Pos)        /* 0x00040000 */

#define SMU_PLLCFGR_pllDivR_DPos        (19U)
#define SMU_PLLCFGR_PLLR_Pos            SMU_PLLCFGR_pllDivR_DPos
#define SMU_PLLCFGR_PLLR_Msk            (0x7UL << SMU_PLLCFGR_PLLR_Pos)         /* 0x00380000 */
#define SMU_PLLCFGR_PLLR                SMU_PLLCFGR_PLLR_Msk                    /* PLLQ[3:0] bits (Output frequency division control signal of CLK_R) */
#define SMU_PLLCFGR_PLLR_0              (0x01UL << SMU_PLLCFGR_PLLR_Pos)        /* 0x00080000 */
#define SMU_PLLCFGR_PLLR_1              (0x02UL << SMU_PLLCFGR_PLLR_Pos)        /* 0x00100000 */
#define SMU_PLLCFGR_PLLR_2              (0x04UL << SMU_PLLCFGR_PLLR_Pos)        /* 0x00200000 */

#define SMU_PLLCFGR_pllEn_DPos          (22U)
#define SMU_PLLCFGR_PLLEN_Pos           SMU_PLLCFGR_pllEn_DPos
#define SMU_PLLCFGR_PLLEN_Msk           (0x1UL << SMU_PLLCFGR_PLLEN_Pos)        /* 0x00400000 */
#define SMU_PLLCFGR_PLLEN               SMU_PLLCFGR_PLLEN_Msk                   /* PLL clock enable */

#define SMU_PLLCFGR_pllRstn_DPos        (23U)
#define SMU_PLLCFGR_PLLRST_Pos          SMU_PLLCFGR_pllRstn_DPos
#define SMU_PLLCFGR_PLLRST_Msk          (0x1UL << SMU_PLLCFGR_PLLRST_Pos)       /* 0x00800000 */
#define SMU_PLLCFGR_PLLRST              SMU_PLLCFGR_PLLRST_Msk                  /* PLL clock reset signal */

#define SMU_PLLCFGR_pllUpdate_DPos      (24U)
#define SMU_PLLCFGR_PLLUF_Pos            SMU_PLLCFGR_pllUpdate_DPos
#define SMU_PLLCFGR_PLLUF_Msk           (0x1UL << SMU_PLLCFGR_PLLUF_Pos)        /* 0x01000000 */
#define SMU_PLLCFGR_PLLUF               SMU_PLLCFGR_PLLUF_Msk                   /* Update PLL frequency enable */


/******************  Bits definition for SMU_FIRCCFGR register  *****************/
#define SMU_FIRCCFGR_fircTF_DPos        (0U)
#define SMU_FIRCCFGR_FIRCTF_Pos         SMU_FIRCCFGR_fircTF_DPos
#define SMU_FIRCCFGR_FIRCTF_Msk         (0x3FUL << SMU_FIRCCFGR_FIRCTF_Pos)     /* 0x0000003F */
#define SMU_FIRCCFGR_FIRCTF             SMU_FIRCCFGR_FIRCTF_Msk                 /* FIRCTF[5:0] bits (FIRC oscillator frequency adjustment) */
#define SMU_FIRCCFGR_FIRCTF_0           (0x01UL << SMU_FIRCCFGR_FIRCTF_Pos)     /* 0x00000001 */
#define SMU_FIRCCFGR_FIRCTF_1           (0x02UL << SMU_FIRCCFGR_FIRCTF_Pos)     /* 0x00000002 */
#define SMU_FIRCCFGR_FIRCTF_2           (0x04UL << SMU_FIRCCFGR_FIRCTF_Pos)     /* 0x00000004 */
#define SMU_FIRCCFGR_FIRCTF_3           (0x08UL << SMU_FIRCCFGR_FIRCTF_Pos)     /* 0x00000008 */
#define SMU_FIRCCFGR_FIRCTF_4           (0x10UL << SMU_FIRCCFGR_FIRCTF_Pos)     /* 0x00000010 */
#define SMU_FIRCCFGR_FIRCTF_5           (0x20UL << SMU_FIRCCFGR_FIRCTF_Pos)     /* 0x00000020 */

#define SMU_FIRCCFGR_fircPdOsc_DPos     (6U)
#define SMU_FIRCCFGR_FIRCON_Pos         SMU_FIRCCFGR_fircPdOsc_DPos
#define SMU_FIRCCFGR_FIRCON_Msk         (0x1UL << SMU_FIRCCFGR_FIRCON_Pos)      /* 0x00000040 */
#define SMU_FIRCCFGR_FIRCON             SMU_FIRCCFGR_FIRCON_Msk                 /* FIRC oscillator switch */

#define SMU_FIRCCFGR_fircRdy_DPos       (7U)
#define SMU_FIRCCFGR_FIRCRDY_Pos        SMU_FIRCCFGR_fircRdy_DPos
#define SMU_FIRCCFGR_FIRCRDY_Msk        (0x1UL << SMU_FIRCCFGR_FIRCRDY_Pos)     /* 0x00000080 */
#define SMU_FIRCCFGR_FIRCRDY            SMU_FIRCCFGR_FIRCRDY_Msk                /* FIRC oscillator preparation position */


/******************  Bits definition for SMU_BUSCFGR register  *****************/
#define SMU_BUSCFGR_pllClkSel_DPos      (0U)
#define SMU_BUSCFGR_PLLSEL_Pos          SMU_BUSCFGR_pllClkSel_DPos
#define SMU_BUSCFGR_PLLSEL_Msk          (0x1UL << SMU_BUSCFGR_PLLSEL_Pos)       /* 0x00000001 */
#define SMU_BUSCFGR_PLLSEL              SMU_BUSCFGR_PLLSEL_Msk                  /* PLL clock selection */

#define SMU_BUSCFGR_sysClkSel_DPos      (1U)
#define SMU_BUSCFGR_SYSSEL_Pos          SMU_BUSCFGR_sysClkSel_DPos
#define SMU_BUSCFGR_SYSSEL_Msk          (0x3UL << SMU_BUSCFGR_SYSSEL_Pos)       /* 0x00000006 */
#define SMU_BUSCFGR_SYSSEL              SMU_BUSCFGR_SYSSEL_Msk                  /* SYSSEL[1:0] bits (System clock selection) */
#define SMU_BUSCFGR_SYSSEL_0            (0x01UL << SMU_BUSCFGR_SYSSEL_Pos)      /* 0x00000002 */
#define SMU_BUSCFGR_SYSSEL_1            (0x02UL << SMU_BUSCFGR_SYSSEL_Pos)      /* 0x00000004 */

#define SMU_BUSCFGR_axi4Bus3Div_DPos    (9U)
#define SMU_BUSCFGR_AXI4BUS3DIV_Pos     SMU_BUSCFGR_axi4Bus3Div_DPos
#define SMU_BUSCFGR_AXI4BUS3DIV_Msk     (0x3UL << SMU_BUSCFGR_AXI4BUS3DIV_Pos)  /* 0x00000600 */
#define SMU_BUSCFGR_AXI4BUS3DIV         SMU_BUSCFGR_AXI4BUS3DIV_Msk             /* AXI4BUS3DIV[1:0] bits (Axi4Bus3 clock division ratio) */
#define SMU_BUSCFGR_AXI4BUS3DIV_0       (0x01UL << SMU_BUSCFGR_AXI4BUS3DIV_Pos) /* 0x00000200 */
#define SMU_BUSCFGR_AXI4BUS3DIV_1       (0x02UL << SMU_BUSCFGR_AXI4BUS3DIV_Pos) /* 0x00000400 */

#define SMU_BUSCFGR_apbBus0Div_DPos     (11U)
#define SMU_BUSCFGR_APBBUS0DIV_Pos      SMU_BUSCFGR_apbBus0Div_DPos
#define SMU_BUSCFGR_APBBUS0DIV_Msk      (0xFUL << SMU_BUSCFGR_APBBUS0DIV_Pos)   /* 0x00007800 */
#define SMU_BUSCFGR_APBBUS0DIV          SMU_BUSCFGR_APBBUS0DIV_Msk              /* APBBUS0DIV[3:0] bits (ApbBus0 clock division ratio) */
#define SMU_BUSCFGR_APBBUS0DIV_0        (0x01UL << SMU_BUSCFGR_APBBUS0DIV_Pos)  /* 0x00000800 */
#define SMU_BUSCFGR_APBBUS0DIV_1        (0x02UL << SMU_BUSCFGR_APBBUS0DIV_Pos)  /* 0x00001000 */
#define SMU_BUSCFGR_APBBUS0DIV_2        (0x04UL << SMU_BUSCFGR_APBBUS0DIV_Pos)  /* 0x00002000 */
#define SMU_BUSCFGR_APBBUS0DIV_3        (0x08UL << SMU_BUSCFGR_APBBUS0DIV_Pos)  /* 0x00004000 */

#define SMU_BUSCFGR_apbBus1Div_DPos     (15U)
#define SMU_BUSCFGR_APBBUS1DIV_Pos      SMU_BUSCFGR_apbBus1Div_DPos
#define SMU_BUSCFGR_APBBUS1DIV_Msk      (0xFUL << SMU_BUSCFGR_APBBUS1DIV_Pos)   /* 0x00078000 */
#define SMU_BUSCFGR_APBBUS1DIV          SMU_BUSCFGR_APBBUS1DIV_Msk              /* APBBUS1DIV[3:0] bits (ApbBus1 clock division ratio) */
#define SMU_BUSCFGR_APBBUS1DIV_0        (0x01UL << SMU_BUSCFGR_APBBUS1DIV_Pos)  /* 0x00008000 */
#define SMU_BUSCFGR_APBBUS1DIV_1        (0x02UL << SMU_BUSCFGR_APBBUS1DIV_Pos)  /* 0x00010000 */
#define SMU_BUSCFGR_APBBUS1DIV_2        (0x04UL << SMU_BUSCFGR_APBBUS1DIV_Pos)  /* 0x00020000 */
#define SMU_BUSCFGR_APBBUS1DIV_3        (0x08UL << SMU_BUSCFGR_APBBUS1DIV_Pos)  /* 0x00040000 */

#define SMU_BUSCFGR_canClkX2Div_DPos    (19U)
#define SMU_BUSCFGR_CANX2DIV_Pos        SMU_BUSCFGR_canClkX2Div_DPos
#define SMU_BUSCFGR_CANX2DIV_Msk        (0xFUL << SMU_BUSCFGR_CANX2DIV_Pos)     /* 0x00780000 */
#define SMU_BUSCFGR_CANX2DIV            SMU_BUSCFGR_CANX2DIV_Msk                /* CANX2DIV[3:0] bits (CANX2 clock division ratio) */
#define SMU_BUSCFGR_CANX2DIV_0          (0x01UL << SMU_BUSCFGR_CANX2DIV_Pos)    /* 0x00080000 */
#define SMU_BUSCFGR_CANX2DIV_1          (0x02UL << SMU_BUSCFGR_CANX2DIV_Pos)    /* 0x00100000 */
#define SMU_BUSCFGR_CANX2DIV_2          (0x04UL << SMU_BUSCFGR_CANX2DIV_Pos)    /* 0x00200000 */
#define SMU_BUSCFGR_CANX2DIV_3          (0x08UL << SMU_BUSCFGR_CANX2DIV_Pos)    /* 0x00400000 */


/******************  Bits definition for SR register  *****************/
#define SMU_SR_pllLock_DPos             (0U)
#define SMU_SR_PLLLOCK_Pos              SMU_SR_pllLock_DPos
#define SMU_SR_PLLLOCK_Msk              (0x1UL << SMU_SR_PLLLOCK_Pos)           /* 0x00000001 */
#define SMU_SR_PLLLOCK                  SMU_SR_PLLLOCK_Msk                      /* PLL clock lock */

#define SMU_SR_eflashInitDone_DPos      (1U)
#define SMU_SR_EFLASHICS_Pos            SMU_SR_eflashInitDone_DPos
#define SMU_SR_EFLASHICS_Msk            (0x1UL << SMU_SR_EFLASHICS_Pos)         /* 0x00000002 */
#define SMU_SR_EFLASHICS                SMU_SR_EFLASHICS_Msk                    /* EFLASH initialization completion signal */

#define SMU_SR_workState_DPos           (2U)
#define SMU_SR_MCUSM_Pos                SMU_SR_workState_DPos
#define SMU_SR_MCUSM_Msk                (0x3UL << SMU_SR_MCUSM_Pos)             /* 0x0000000C */
#define SMU_SR_MCUSM                    SMU_SR_MCUSM_Msk                        /* MCUSM[1:0] bits (MCU startup mode) */
#define SMU_SR_MCUSM_0                  (0x01UL << SMU_SR_MCUSM_Pos)            /* 0x00000004 */
#define SMU_SR_MCUSM_1                  (0x02UL << SMU_SR_MCUSM_Pos)            /* 0x00000008 */

#define SMU_SR_lowPower_DPos            (4U)
#define SMU_SR_LPWRF_Pos                SMU_SR_lowPower_DPos
#define SMU_SR_LPWRF_Msk                (0x1UL << SMU_SR_LPWRF_Pos)             /* 0x00000010 */
#define SMU_SR_LPWRF                    SMU_SR_LPWRF_Msk                        /* Low power mode flag */

#define SMU_SR_busState_DPos            (5U)
#define SMU_SR_BUSS_Pos                 SMU_SR_busState_DPos
#define SMU_SR_BUSS_Msk                 (0x7UL << SMU_SR_BUSS_Pos)              /* 0x000000E0 */
#define SMU_SR_BUSS                     SMU_SR_BUSS_Msk                         /* BUSS[2:0] bits (Bus state) */
#define SMU_SR_BUSS_0                   (0x01UL << SMU_SR_BUSS_Pos)             /* 0x00000020 */
#define SMU_SR_BUSS_1                   (0x02UL << SMU_SR_BUSS_Pos)             /* 0x00000040 */
#define SMU_SR_BUSS_2                   (0x04UL << SMU_SR_BUSS_Pos)             /* 0x00000080 */

#define SMU_SR_sramRdy_DPos             (8U)
#define SMU_SR_SRAMRDY_Pos              SMU_SR_sramRdy_DPos
#define SMU_SR_SRAMRDY_Msk              (0xFUL << SMU_SR_SRAMRDY_Pos)           /* 0x00000F00 */
#define SMU_SR_SRAMRDY                  SMU_SR_SRAMRDY_Msk                      /* SRAMRDY[3:0] bits (SRAM ready) */
#define SMU_SR_SRAMRDY_0                (0x01UL << SMU_SR_SRAMRDY_Pos)          /* 0x00000100 */
#define SMU_SR_SRAMRDY_1                (0x02UL << SMU_SR_SRAMRDY_Pos)          /* 0x00000200 */
#define SMU_SR_SRAMRDY_2                (0x04UL << SMU_SR_SRAMRDY_Pos)          /* 0x00000400 */
#define SMU_SR_SRAMRDY_3                (0x08UL << SMU_SR_SRAMRDY_Pos)          /* 0x00000800 */


/******************  Bits definition for SMU_AB0CFGR register  *****************/
#define SMU_AB0CFGR_QSPIEN_DPos         (0U)
#define SMU_AB0CFGR_QSPIEN_Pos          SMU_AB0CFGR_QSPIEN_DPos
#define SMU_AB0CFGR_QSPIEN_Msk          (0x1UL << SMU_AB0CFGR_QSPIEN_Pos)       /* 0x00000001 */
#define SMU_AB0CFGR_QSPIEN              SMU_AB0CFGR_QSPIEN_Msk                  /* QSPI clock enable */

#define SMU_AB0CFGR_QSPIRST_DPos        (1U)
#define SMU_AB0CFGR_QSPIRST_Pos         SMU_AB0CFGR_QSPIRST_DPos
#define SMU_AB0CFGR_QSPIRST_Msk         (0x1UL << SMU_AB0CFGR_QSPIRST_Pos)      /* 0x00000002 */
#define SMU_AB0CFGR_QSPIRST             SMU_AB0CFGR_QSPIRST_Msk                 /* QSPI reset */

#define SMU_AB0CFGR_EFLASHEN_DPos       (2U)
#define SMU_AB0CFGR_EFLASHEN_Pos        SMU_AB0CFGR_EFLASHEN_DPos
#define SMU_AB0CFGR_EFLASHEN_Msk        (0x1UL << SMU_AB0CFGR_EFLASHEN_Pos)     /* 0x00000004 */
#define SMU_AB0CFGR_EFLASHEN            SMU_AB0CFGR_EFLASHEN_Msk                /* EFLASH clock enable */

#define SMU_AB0CFGR_EFLASHRST_DPos      (3U)
#define SMU_AB0CFGR_EFLASHRST_Pos       SMU_AB0CFGR_EFLASHRST_DPos
#define SMU_AB0CFGR_EFLASHRST_Msk       (0x1UL << SMU_AB0CFGR_EFLASHRST_Pos)    /* 0x00000008 */
#define SMU_AB0CFGR_EFLASHRST           SMU_AB0CFGR_EFLASHRST_Msk               /* EFLASH reset */

#define SMU_AB0CFGR_QSPIUEN_DPos        (4U)
#define SMU_AB0CFGR_QSPIUEN_Pos         SMU_AB0CFGR_QSPIUEN_DPos
#define SMU_AB0CFGR_QSPIUEN_Msk         (0x1UL << SMU_AB0CFGR_QSPIUEN_Pos)      /* 0x00000010 */
#define SMU_AB0CFGR_QSPIUEN             SMU_AB0CFGR_QSPIUEN_Msk                 /* QSPI Clock update Enable */

#define SMU_AB0CFGR_EFLASHUEN_DPos      (5U)
#define SMU_AB0CFGR_EFLASHUEN_Pos       SMU_AB0CFGR_EFLASHUEN_DPos
#define SMU_AB0CFGR_EFLASHUEN_Msk       (0x1UL << SMU_AB0CFGR_EFLASHUEN_Pos)    /* 0x00000020 */
#define SMU_AB0CFGR_EFLASHUEN           SMU_AB0CFGR_EFLASHUEN_Msk               /* EFLASH clock update enable */


/******************  Bits definition for SMU_AB1CFGR register  *****************/
#define SMU_AB1CFGR_DMA0EN_DPos         (0U)
#define SMU_AB1CFGR_DMA0EN_Pos          SMU_AB1CFGR_DMA0EN_DPos
#define SMU_AB1CFGR_DMA0EN_Msk          (0x1UL << SMU_AB1CFGR_DMA0EN_Pos)       /* 0x00000001 */
#define SMU_AB1CFGR_DMA0EN              SMU_AB1CFGR_DMA0EN_Msk                  /* DMA0 clock enable */

#define SMU_AB1CFGR_DMA0RST_DPos        (1U)
#define SMU_AB1CFGR_DMA0RST_Pos         SMU_AB1CFGR_DMA0RST_DPos
#define SMU_AB1CFGR_DMA0RST_Msk         (0x1UL << SMU_AB1CFGR_DMA0RST_Pos)      /* 0x00000002 */
#define SMU_AB1CFGR_DMA0RST             SMU_AB1CFGR_DMA0RST_Msk                 /* DMA0 reset */

#define SMU_AB1CFGR_DMA1EN_DPos         (2U)
#define SMU_AB1CFGR_DMA1EN_Pos          SMU_AB1CFGR_DMA1EN_DPos
#define SMU_AB1CFGR_DMA1EN_Msk          (0x1UL << SMU_AB1CFGR_DMA1EN_Pos)       /* 0x00000004 */
#define SMU_AB1CFGR_DMA1EN              SMU_AB1CFGR_DMA1EN_Msk                  /* DMA1 clock enable */

#define SMU_AB1CFGR_DMA1RST_DPos        (3U)
#define SMU_AB1CFGR_DMA1RST_Pos         SMU_AB1CFGR_DMA1RST_DPos
#define SMU_AB1CFGR_DMA1RST_Msk         (0x1UL << SMU_AB1CFGR_DMA1RST_Pos)      /* 0x00000008 */
#define SMU_AB1CFGR_DMA1RST             SMU_AB1CFGR_DMA1RST_Msk                 /* DMA1 reset */

#define SMU_AB1CFGR_SRAM0EN_DPos        (4U)
#define SMU_AB1CFGR_SRAM0EN_Pos         SMU_AB1CFGR_SRAM0EN_DPos
#define SMU_AB1CFGR_SRAM0EN_Msk         (0x1UL << SMU_AB1CFGR_SRAM0EN_Pos)      /* 0x00000010 */
#define SMU_AB1CFGR_SRAM0EN             SMU_AB1CFGR_SRAM0EN_Msk                 /* SRAM0 clock enable */

#define SMU_AB1CFGR_SRAM0RST_DPos       (5U)
#define SMU_AB1CFGR_SRAM0RST_Pos        SMU_AB1CFGR_SRAM0RST_DPos
#define SMU_AB1CFGR_SRAM0RST_Msk        (0x1UL << SMU_AB1CFGR_SRAM0RST_Pos)     /* 0x00000020 */
#define SMU_AB1CFGR_SRAM0RST            SMU_AB1CFGR_SRAM0RST_Msk                /* SRAM0 reset */

#define SMU_AB1CFGR_SRAM1EN_DPos        (6U)
#define SMU_AB1CFGR_SRAM1EN_Pos         SMU_AB1CFGR_SRAM1EN_DPos
#define SMU_AB1CFGR_SRAM1EN_Msk         (0x1UL << SMU_AB1CFGR_SRAM1EN_Pos)      /* 0x00000040 */
#define SMU_AB1CFGR_SRAM1EN             SMU_AB1CFGR_SRAM1EN_Msk                 /* SRAM1 clock enable */

#define SMU_AB1CFGR_SRAM1RST_DPos       (7U)
#define SMU_AB1CFGR_SRAM1RST_Pos        SMU_AB1CFGR_SRAM1RST_DPos
#define SMU_AB1CFGR_SRAM1RST_Msk        (0x1UL << SMU_AB1CFGR_SRAM1RST_Pos)     /* 0x00000080 */
#define SMU_AB1CFGR_SRAM1RST            SMU_AB1CFGR_SRAM1RST_Msk                /* SRAM1 reset */

#define SMU_AB1CFGR_SRAM2EN_DPos        (8U)
#define SMU_AB1CFGR_SRAM2EN_Pos         SMU_AB1CFGR_SRAM2EN_DPos
#define SMU_AB1CFGR_SRAM2EN_Msk         (0x1UL << SMU_AB1CFGR_SRAM2EN_Pos)      /* 0x00000100 */
#define SMU_AB1CFGR_SRAM2EN             SMU_AB1CFGR_SRAM2EN_Msk                 /* SRAM2 clock enable */

#define SMU_AB1CFGR_SRAM2RST_DPos       (9U)
#define SMU_AB1CFGR_SRAM2RST_Pos        SMU_AB1CFGR_SRAM2RST_DPos
#define SMU_AB1CFGR_SRAM2RST_Msk        (0x1UL << SMU_AB1CFGR_SRAM2RST_Pos)     /* 0x00000200 */
#define SMU_AB1CFGR_SRAM2RST            SMU_AB1CFGR_SRAM2RST_Msk                /* SRAM2 reset */

#define SMU_AB1CFGR_SRAM3EN_DPos        (10U)
#define SMU_AB1CFGR_SRAM3EN_Pos         SMU_AB1CFGR_SRAM3EN_DPos
#define SMU_AB1CFGR_SRAM3EN_Msk         (0x1UL << SMU_AB1CFGR_SRAM3EN_Pos)      /* 0x00000400 */
#define SMU_AB1CFGR_SRAM3EN             SMU_AB1CFGR_SRAM3EN_Msk                 /* SRAM3 clock enable */

#define SMU_AB1CFGR_SRAM3RST_DPos       (11U)
#define SMU_AB1CFGR_SRAM3RST_Pos        SMU_AB1CFGR_SRAM3RST_DPos
#define SMU_AB1CFGR_SRAM3RST_Msk        (0x1UL << SMU_AB1CFGR_SRAM3RST_Pos)     /* 0x00000800 */
#define SMU_AB1CFGR_SRAM3RST            SMU_AB1CFGR_SRAM3RST_Msk                /* SRAM3 reset */


/******************  Bits definition for SMU_AB3CFGR register  *****************/
#define SMU_AB3CFGR_AXI4TOAPB0EN_DPos   (2U)
#define SMU_AB3CFGR_AXI4TOAPB0EN_Pos    SMU_AB3CFGR_AXI4TOAPB0EN_DPos
#define SMU_AB3CFGR_AXI4TOAPB0EN_Msk    (0x1UL << SMU_AB3CFGR_AXI4TOAPB0EN_Pos) /* 0x00000004 */
#define SMU_AB3CFGR_AXI4TOAPB0EN        SMU_AB3CFGR_AXI4TOAPB0EN_Msk            /* AXI4 to APB0 clock enable */

#define SMU_AB3CFGR_AXI4TOAPB0RST_DPos  (3U)
#define SMU_AB3CFGR_AXI4TOAPB0RST_Pos   SMU_AB3CFGR_AXI4TOAPB0RST_DPos
#define SMU_AB3CFGR_AXI4TOAPB0RST_Msk   (0x1UL << SMU_AB3CFGR_AXI4TOAPB0RST_Pos)/* 0x00000008 */
#define SMU_AB3CFGR_AXI4TOAPB0RST       SMU_AB3CFGR_AXI4TOAPB0RST_Msk           /* AXI4 to APB0 reset */

#define SMU_AB3CFGR_AXI4TOAPB1EN_DPos   (4U)
#define SMU_AB3CFGR_AXI4TOAPB1EN_Pos    SMU_AB3CFGR_AXI4TOAPB1EN_DPos
#define SMU_AB3CFGR_AXI4TOAPB1EN_Msk    (0x1UL << SMU_AB3CFGR_AXI4TOAPB1EN_Pos) /* 0x00000010 */
#define SMU_AB3CFGR_AXI4TOAPB1EN        SMU_AB3CFGR_AXI4TOAPB1EN_Msk            /* AXI4 to APB1 clock enable */

#define SMU_AB3CFGR_AXI4TOAPB1RST_DPos  (5U)
#define SMU_AB3CFGR_AXI4TOAPB1RST_Pos   SMU_AB3CFGR_AXI4TOAPB1RST_DPos
#define SMU_AB3CFGR_AXI4TOAPB1RST_Msk   (0x1UL << SMU_AB3CFGR_AXI4TOAPB1RST_Pos)/* 0x00000020 */
#define SMU_AB3CFGR_AXI4TOAPB1RST       SMU_AB3CFGR_AXI4TOAPB1RST_Msk           /* AXI4 to APB1 reset */

#define SMU_AB3CFGR_AXIBUS3EN_DPos      (6U)
#define SMU_AB3CFGR_AXIBUS3EN_Pos       SMU_AB3CFGR_AXIBUS3EN_DPos
#define SMU_AB3CFGR_AXIBUS3EN_Msk       (0x1UL << SMU_AB3CFGR_AXIBUS3EN_Pos)    /* 0x00000040 */
#define SMU_AB3CFGR_AXIBUS3EN           SMU_AB3CFGR_AXIBUS3EN_Msk               /* AxiBux3 clock enable */

#define SMU_AB3CFGR_AXIBUS3RST_DPos     (7U)
#define SMU_AB3CFGR_AXIBUS3RST_Pos      SMU_AB3CFGR_AXIBUS3RST_DPos
#define SMU_AB3CFGR_AXIBUS3RST_Msk      (0x1UL << SMU_AB3CFGR_AXIBUS3RST_Pos)   /* 0x00000080 */
#define SMU_AB3CFGR_AXIBUS3RST          SMU_AB3CFGR_AXIBUS3RST_Msk              /* AxiBux3 reset */


/******************  Bits definition for SMU_ALB0CFGR register  *****************/
#define SMU_ALB0CFGR_FCUEN_DPos         (0U)
#define SMU_ALB0CFGR_FCUEN_Pos          SMU_ALB0CFGR_FCUEN_DPos
#define SMU_ALB0CFGR_FCUEN_Msk          (0x1UL << SMU_ALB0CFGR_FCUEN_Pos)       /* 0x00000001 */
#define SMU_ALB0CFGR_FCUEN              SMU_ALB0CFGR_FCUEN_Msk                  /* FCU clock enable */

#define SMU_ALB0CFGR_FCURST_DPos        (1U)
#define SMU_ALB0CFGR_FCURST_Pos         SMU_ALB0CFGR_FCURST_DPos
#define SMU_ALB0CFGR_FCURST_Msk         (0x1UL << SMU_ALB0CFGR_FCURST_Pos)      /* 0x00000002 */
#define SMU_ALB0CFGR_FCURST             SMU_ALB0CFGR_FCURST_Msk                 /* FCU reset */

#define SMU_ALB0CFGR_CMU0EN_DPos        (2U)
#define SMU_ALB0CFGR_CMU0EN_Pos         SMU_ALB0CFGR_CMU0EN_DPos
#define SMU_ALB0CFGR_CMU0EN_Msk         (0x1UL << SMU_ALB0CFGR_CMU0EN_Pos)      /* 0x00000004 */
#define SMU_ALB0CFGR_CMU0EN             SMU_ALB0CFGR_CMU0EN_Msk                 /* CMU0 clock enable */

#define SMU_ALB0CFGR_CMU0RST_DPos       (3U)
#define SMU_ALB0CFGR_CMU0RST_Pos        SMU_ALB0CFGR_CMU0RST_DPos
#define SMU_ALB0CFGR_CMU0RST_Msk        (0x1UL << SMU_ALB0CFGR_CMU0RST_Pos)     /* 0x00000008 */
#define SMU_ALB0CFGR_CMU0RST            SMU_ALB0CFGR_CMU0RST_Msk                /* CMU0 reset */

#define SMU_ALB0CFGR_CMU1EN_DPos        (4U)
#define SMU_ALB0CFGR_CMU1EN_Pos         SMU_ALB0CFGR_CMU1EN_DPos
#define SMU_ALB0CFGR_CMU1EN_Msk         (0x1UL << SMU_ALB0CFGR_CMU1EN_Pos)      /* 0x00000010 */
#define SMU_ALB0CFGR_CMU1EN             SMU_ALB0CFGR_CMU1EN_Msk                 /* CMU1 clock enable */

#define SMU_ALB0CFGR_CMU1RST_DPos       (5U)
#define SMU_ALB0CFGR_CMU1RST_Pos        SMU_ALB0CFGR_CMU1RST_DPos
#define SMU_ALB0CFGR_CMU1RST_Msk        (0x1UL << SMU_ALB0CFGR_CMU1RST_Pos)     /* 0x00000020 */
#define SMU_ALB0CFGR_CMU1RST            SMU_ALB0CFGR_CMU1RST_Msk                /* CMU1 reset */

#define SMU_ALB0CFGR_CMU2EN_DPos        (6U)
#define SMU_ALB0CFGR_CMU2EN_Pos         SMU_ALB0CFGR_CMU2EN_DPos
#define SMU_ALB0CFGR_CMU2EN_Msk         (0x1UL << SMU_ALB0CFGR_CMU2EN_Pos)      /* 0x00000040 */
#define SMU_ALB0CFGR_CMU2EN             SMU_ALB0CFGR_CMU2EN_Msk                 /* CMU2 clock enable */

#define SMU_ALB0CFGR_CMU2RST_DPos       (7U)
#define SMU_ALB0CFGR_CMU2RST_Pos        SMU_ALB0CFGR_CMU2RST_DPos
#define SMU_ALB0CFGR_CMU2RST_Msk        (0x1UL << SMU_ALB0CFGR_CMU2RST_Pos)     /* 0x00000080 */
#define SMU_ALB0CFGR_CMU2RST            SMU_ALB0CFGR_CMU2RST_Msk                /* CMU2 reset */

#define SMU_ALB0CFGR_CMU3EN_DPos        (8U)
#define SMU_ALB0CFGR_CMU3EN_Pos         SMU_ALB0CFGR_CMU3EN_DPos
#define SMU_ALB0CFGR_CMU3EN_Msk         (0x1UL << SMU_ALB0CFGR_CMU3EN_Pos)      /* 0x00000100 */
#define SMU_ALB0CFGR_CMU3EN             SMU_ALB0CFGR_CMU3EN_Msk                 /* CMU3 clock enable */

#define SMU_ALB0CFGR_CMU3RST_DPos       (9U)
#define SMU_ALB0CFGR_CMU3RST_Pos        SMU_ALB0CFGR_CMU3RST_DPos
#define SMU_ALB0CFGR_CMU3RST_Msk        (0x1UL << SMU_ALB0CFGR_CMU3RST_Pos)     /* 0x00000200 */
#define SMU_ALB0CFGR_CMU3RST            SMU_ALB0CFGR_CMU3RST_Msk                /* CMU3 reset */

#define SMU_ALB0CFGR_PLICEN_DPos        (10U)
#define SMU_ALB0CFGR_PLICEN_Pos         SMU_ALB0CFGR_PLICEN_DPos
#define SMU_ALB0CFGR_PLICEN_Msk         (0x1UL << SMU_ALB0CFGR_PLICEN_Pos)      /* 0x00000400 */
#define SMU_ALB0CFGR_PLICEN             SMU_ALB0CFGR_PLICEN_Msk                 /* PLIC clock enable */

#define SMU_ALB0CFGR_PLICRST_DPos       (11U)
#define SMU_ALB0CFGR_PLICRST_Pos        SMU_ALB0CFGR_PLICRST_DPos
#define SMU_ALB0CFGR_PLICRST_Msk        (0x1UL << SMU_ALB0CFGR_PLICRST_Pos)     /* 0x00000800 */
#define SMU_ALB0CFGR_PLICRST            SMU_ALB0CFGR_PLICRST_Msk                /* PLIC reset */

#define SMU_ALB0CFGR_CLINTEN_DPos       (12U)
#define SMU_ALB0CFGR_CLINTEN_Pos        SMU_ALB0CFGR_CLINTEN_DPos
#define SMU_ALB0CFGR_CLINTEN_Msk        (0x1UL << SMU_ALB0CFGR_CLINTEN_Pos)     /* 0x00001000 */
#define SMU_ALB0CFGR_CLINTEN            SMU_ALB0CFGR_CLINTEN_Msk                /* CLINT clock enable */

#define SMU_ALB0CFGR_CLINTRST_DPos      (13U)
#define SMU_ALB0CFGR_CLINTRST_Pos       SMU_ALB0CFGR_CLINTRST_DPos
#define SMU_ALB0CFGR_CLINTRST_Msk       (0x1UL << SMU_ALB0CFGR_CLINTRST_Pos)    /* 0x00002000 */
#define SMU_ALB0CFGR_CLINTRST           SMU_ALB0CFGR_CLINTRST_Msk               /* CLINT reset */


/******************  Bits definition for SMU_ALB1CFGR register  *****************/
#define SMU_ALB1CFGR_HTIM0EN_DPos       (0U)
#define SMU_ALB1CFGR_HTIM0EN_Pos        SMU_ALB1CFGR_HTIM0EN_DPos
#define SMU_ALB1CFGR_HTIM0EN_Msk        (0x1UL << SMU_ALB1CFGR_HTIM0EN_Pos)     /* 0x00000001 */
#define SMU_ALB1CFGR_HTIM0EN            SMU_ALB1CFGR_HTIM0EN_Msk                /* HTIM0 clock enable */

#define SMU_ALB1CFGR_HTIM0RST_DPos      (1U)
#define SMU_ALB1CFGR_HTIM0RST_Pos       SMU_ALB1CFGR_HTIM0RST_DPos
#define SMU_ALB1CFGR_HTIM0RST_Msk       (0x1UL << SMU_ALB1CFGR_HTIM0RST_Pos)    /* 0x00000002 */
#define SMU_ALB1CFGR_HTIM0RST           SMU_ALB1CFGR_HTIM0RST_Msk               /* HTIM0 reset */

#define SMU_ALB1CFGR_CANFD0EN_DPos      (2U)
#define SMU_ALB1CFGR_CANFD0EN_Pos       SMU_ALB1CFGR_CANFD0EN_DPos
#define SMU_ALB1CFGR_CANFD0EN_Msk       (0x1UL << SMU_ALB1CFGR_CANFD0EN_Pos)    /* 0x00000004 */
#define SMU_ALB1CFGR_CANFD0EN           SMU_ALB1CFGR_CANFD0EN_Msk               /* CANFD0 clock enable */

#define SMU_ALB1CFGR_CANFD0RST_DPos     (3U)
#define SMU_ALB1CFGR_CANFD0RST_Pos      SMU_ALB1CFGR_CANFD0RST_DPos
#define SMU_ALB1CFGR_CANFD0RST_Msk      (0x1UL << SMU_ALB1CFGR_CANFD0RST_Pos)   /* 0x00000008 */
#define SMU_ALB1CFGR_CANFD0RST          SMU_ALB1CFGR_CANFD0RST_Msk              /* CANFD0 reset */

#define SMU_ALB1CFGR_CANFD1EN_DPos      (4U)
#define SMU_ALB1CFGR_CANFD1EN_Pos       SMU_ALB1CFGR_CANFD1EN_DPos
#define SMU_ALB1CFGR_CANFD1EN_Msk       (0x1UL << SMU_ALB1CFGR_CANFD1EN_Pos)    /* 0x00000010 */
#define SMU_ALB1CFGR_CANFD1EN           SMU_ALB1CFGR_CANFD1EN_Msk               /* CANFD1 clock enable */

#define SMU_ALB1CFGR_CANFD1RST_DPos     (5U)
#define SMU_ALB1CFGR_CANFD1RST_Pos      SMU_ALB1CFGR_CANFD1RST_DPos
#define SMU_ALB1CFGR_CANFD1RST_Msk      (0x1UL << SMU_ALB1CFGR_CANFD1RST_Pos)   /* 0x00000020 */
#define SMU_ALB1CFGR_CANFD1RST          SMU_ALB1CFGR_CANFD1RST_Msk              /* CANFD1 reset */

#define SMU_ALB1CFGR_ADC0EN_DPos        (6U)
#define SMU_ALB1CFGR_ADC0EN_Pos         SMU_ALB1CFGR_ADC0EN_DPos
#define SMU_ALB1CFGR_ADC0EN_Msk         (0x1UL << SMU_ALB1CFGR_ADC0EN_Pos)      /* 0x00000040 */
#define SMU_ALB1CFGR_ADC0EN             SMU_ALB1CFGR_ADC0EN_Msk                 /* ADC0 clock enable */

#define SMU_ALB1CFGR_ADC0RST_DPos       (7U)
#define SMU_ALB1CFGR_ADC0RST_Pos        SMU_ALB1CFGR_ADC0RST_DPos
#define SMU_ALB1CFGR_ADC0RST_Msk        (0x1UL << SMU_ALB1CFGR_ADC0RST_Pos)     /* 0x00000080 */
#define SMU_ALB1CFGR_ADC0RST            SMU_ALB1CFGR_ADC0RST_Msk                /* ADC0 reset */

#define SMU_ALB1CFGR_ADC1EN_DPos        (8U)
#define SMU_ALB1CFGR_ADC1EN_Pos         SMU_ALB1CFGR_ADC1EN_DPos
#define SMU_ALB1CFGR_ADC1EN_Msk         (0x1UL << SMU_ALB1CFGR_ADC1EN_Pos)      /* 0x00000100 */
#define SMU_ALB1CFGR_ADC1EN             SMU_ALB1CFGR_ADC1EN_Msk                 /* ADC1 clock enable */

#define SMU_ALB1CFGR_ADC1RST_DPos       (9U)
#define SMU_ALB1CFGR_ADC1RST_Pos        SMU_ALB1CFGR_ADC1RST_DPos
#define SMU_ALB1CFGR_ADC1RST_Msk        (0x1UL << SMU_ALB1CFGR_ADC1RST_Pos)     /* 0x00000200 */
#define SMU_ALB1CFGR_ADC1RST            SMU_ALB1CFGR_ADC1RST_Msk                /* ADC1 reset */

#define SMU_ALB1CFGR_DAC0EN_DPos        (10U)
#define SMU_ALB1CFGR_DAC0EN_Pos         SMU_ALB1CFGR_DAC0EN_DPos
#define SMU_ALB1CFGR_DAC0EN_Msk         (0x1UL << SMU_ALB1CFGR_DAC0EN_Pos)      /* 0x00000400 */
#define SMU_ALB1CFGR_DAC0EN             SMU_ALB1CFGR_DAC0EN_Msk                 /* DAC0 clock enable */

#define SMU_ALB1CFGR_DAC0RST_DPos       (11U)
#define SMU_ALB1CFGR_DAC0RST_Pos        SMU_ALB1CFGR_DAC0RST_DPos
#define SMU_ALB1CFGR_DAC0RST_Msk        (0x1UL << SMU_ALB1CFGR_DAC0RST_Pos)     /* 0x00000800 */
#define SMU_ALB1CFGR_DAC0RST            SMU_ALB1CFGR_DAC0RST_Msk                /* DAC0 reset */

#define SMU_ALB1CFGR_GPIOAEN_DPos       (12U)
#define SMU_ALB1CFGR_GPIOAEN_Pos        SMU_ALB1CFGR_GPIOAEN_DPos
#define SMU_ALB1CFGR_GPIOAEN_Msk        (0x1UL << SMU_ALB1CFGR_GPIOAEN_Pos)     /* 0x00001000 */
#define SMU_ALB1CFGR_GPIOAEN            SMU_ALB1CFGR_GPIOAEN_Msk                /* GPIOA clock enable */

#define SMU_ALB1CFGR_GPIOARST_DPos      (13U)
#define SMU_ALB1CFGR_GPIOARST_Pos       SMU_ALB1CFGR_GPIOARST_DPos
#define SMU_ALB1CFGR_GPIOARST_Msk       (0x1UL << SMU_ALB1CFGR_GPIOARST_Pos)    /* 0x00002000 */
#define SMU_ALB1CFGR_GPIOARST            SMU_ALB1CFGR_GPIOARST_Msk              /* GPIOA reset */

#define SMU_ALB1CFGR_GPIOBEN_DPos       (14U)
#define SMU_ALB1CFGR_GPIOBEN_Pos        SMU_ALB1CFGR_GPIOBEN_DPos
#define SMU_ALB1CFGR_GPIOBEN_Msk        (0x1UL << SMU_ALB1CFGR_GPIOBEN_Pos)     /* 0x00004000 */
#define SMU_ALB1CFGR_GPIOBEN            SMU_ALB1CFGR_GPIOBEN_Msk                /* GPIOB clock enable */

#define SMU_ALB1CFGR_GPIOBRST_DPos      (15U)
#define SMU_ALB1CFGR_GPIOBRST_Pos       SMU_ALB1CFGR_GPIOBRST_DPos
#define SMU_ALB1CFGR_GPIOBRST_Msk       (0x1UL << SMU_ALB1CFGR_GPIOBRST_Pos)    /* 0x00008000 */
#define SMU_ALB1CFGR_GPIOBRST           SMU_ALB1CFGR_GPIOBRST_Msk               /* GPIOB reset */

#define SMU_ALB1CFGR_GPIOCEN_DPos       (16U)
#define SMU_ALB1CFGR_GPIOCEN_Pos        SMU_ALB1CFGR_GPIOCEN_DPos
#define SMU_ALB1CFGR_GPIOCEN_Msk        (0x1UL << SMU_ALB1CFGR_GPIOCEN_Pos)     /* 0x00010000 */
#define SMU_ALB1CFGR_GPIOCEN            SMU_ALB1CFGR_GPIOCEN_Msk                /* GPIOC clock enable */

#define SMU_ALB1CFGR_GPIOCRST_DPos      (17U)
#define SMU_ALB1CFGR_GPIOCRST_Pos       SMU_ALB1CFGR_GPIOCRST_DPos
#define SMU_ALB1CFGR_GPIOCRST_Msk       (0x1UL << SMU_ALB1CFGR_GPIOCRST_Pos)    /* 0x00020000 */
#define SMU_ALB1CFGR_GPIOCRST           SMU_ALB1CFGR_GPIOCRST_Msk               /* GPIOC reset */

#define SMU_ALB1CFGR_DSEEN_DPos         (18U)
#define SMU_ALB1CFGR_DSEEN_Pos          SMU_ALB1CFGR_DSEEN_DPos
#define SMU_ALB1CFGR_DSEEN_Msk          (0x01UL << SMU_ALB1CFGR_DSEEN_Pos)      /* 0x00040000 */
#define SMU_ALB1CFGR_DSEEN              SMU_ALB1CFGR_DSEEN_Msk                  /* DSE clock enable */

#define SMU_ALB1CFGR_DSERST_DPos        (19U)
#define SMU_ALB1CFGR_DSERST_Pos         SMU_ALB1CFGR_DSERST_DPos
#define SMU_ALB1CFGR_DSERST_Msk         (0x01UL << SMU_ALB1CFGR_DSERST_Pos)     /* 0x00080000 */
#define SMU_ALB1CFGR_DSERST             SMU_ALB1CFGR_DSERST_Msk                 /* DSE reset */

#define SMU_ALB1CFGR_AXILITEBUS1EN_DPos (20U)
#define SMU_ALB1CFGR_AXILITEBUS1EN_Pos  SMU_ALB1CFGR_AXILITEBUS1EN_DPos
#define SMU_ALB1CFGR_AXILITEBUS1EN_Msk  (0x1UL << SMU_ALB1CFGR_AXILITEBUS1EN_Pos)   /* 0x00100000 */
#define SMU_ALB1CFGR_AXILITEBUS1EN      SMU_ALB1CFGR_AXILITEBUS1EN_Msk              /* AXILiteBUS1 clock enable */

#define SMU_ALB1CFGR_AXILITEBUS1RST_DPos    (21U)
#define SMU_ALB1CFGR_AXILITEBUS1RST_Pos     SMU_ALB1CFGR_AXILITEBUS1RST_DPos
#define SMU_ALB1CFGR_AXILITEBUS1RST_Msk     (0x1UL << SMU_ALB1CFGR_AXILITEBUS1RST_Pos)  /* 0x00200000 */
#define SMU_ALB1CFGR_AXILITEBUS1RST         SMU_ALB1CFGR_AXILITEBUS1RST_Msk             /* AXILiteBUS1 reset */


/******************  Bits definition for SMU_ALB2CFGR register  *****************/
#define SMU_ALB2CFGR_HTIM1EN_DPos       (0U)
#define SMU_ALB2CFGR_HTIM1EN_Pos        SMU_ALB2CFGR_HTIM1EN_DPos
#define SMU_ALB2CFGR_HTIM1EN_Msk        (0x1UL << SMU_ALB2CFGR_HTIM1EN_Pos)     /* 0x00000001 */
#define SMU_ALB2CFGR_HTIM1EN            SMU_ALB2CFGR_HTIM1EN_Msk                /* HTIM1 clock enable */

#define SMU_ALB2CFGR_HTIM1RST_DPos      (1U)
#define SMU_ALB2CFGR_HTIM1RST_Pos       SMU_ALB2CFGR_HTIM1RST_DPos
#define SMU_ALB2CFGR_HTIM1RST_Msk       (0x1UL << SMU_ALB2CFGR_HTIM1RST_Pos)    /* 0x00000002 */
#define SMU_ALB2CFGR_HTIM1RST           SMU_ALB2CFGR_HTIM1RST_Msk               /* HTIM1 reset */

#define SMU_ALB2CFGR_CANFD2EN_DPos      (2U)
#define SMU_ALB2CFGR_CANFD2EN_Pos       SMU_ALB2CFGR_CANFD2EN_DPos
#define SMU_ALB2CFGR_CANFD2EN_Msk       (0x1UL << SMU_ALB2CFGR_CANFD2EN_Pos)    /* 0x00000004 */
#define SMU_ALB2CFGR_CANFD2EN           SMU_ALB2CFGR_CANFD2EN_Msk               /* CANFD2 clock enable */

#define SMU_ALB2CFGR_CANFD2RST_DPos     (3U)
#define SMU_ALB2CFGR_CANFD2RST_Pos      SMU_ALB2CFGR_CANFD2RST_DPos
#define SMU_ALB2CFGR_CANFD2RST_Msk      (0x1UL << SMU_ALB2CFGR_CANFD2RST_Pos)   /* 0x00000008 */
#define SMU_ALB2CFGR_CANFD2RST          SMU_ALB2CFGR_CANFD2RST_Msk              /* CANFD2 reset */

#define SMU_ALB2CFGR_CANFD3EN_DPos      (4U)
#define SMU_ALB2CFGR_CANFD3EN_Pos       SMU_ALB2CFGR_CANFD3EN_DPos
#define SMU_ALB2CFGR_CANFD3EN_Msk       (0x1UL << SMU_ALB2CFGR_CANFD3EN_Pos)    /* 0x00000010 */
#define SMU_ALB2CFGR_CANFD3EN           SMU_ALB2CFGR_CANFD3EN_Msk               /* CANFD3 clock enable */

#define SMU_ALB2CFGR_CANFD3RST_DPos     (5U)
#define SMU_ALB2CFGR_CANFD3RST_Pos      SMU_ALB2CFGR_CANFD3RST_DPos
#define SMU_ALB2CFGR_CANFD3RST_Msk      (0x1UL << SMU_ALB2CFGR_CANFD3RST_Pos)   /* 0x00000020 */
#define SMU_ALB2CFGR_CANFD3RST          SMU_ALB2CFGR_CANFD3RST_Msk              /* CANFD3 reset */

#define SMU_ALB2CFGR_ADC2EN_DPos        (6U)
#define SMU_ALB2CFGR_ADC2EN_Pos         SMU_ALB2CFGR_ADC2EN_DPos
#define SMU_ALB2CFGR_ADC2EN_Msk         (0x1UL << SMU_ALB2CFGR_ADC2EN_Pos)      /* 0x00000040 */
#define SMU_ALB2CFGR_ADC2EN             SMU_ALB2CFGR_ADC2EN_Msk                 /* ADC2 clock enable */

#define SMU_ALB2CFGR_ADC2RST_DPos       (7U)
#define SMU_ALB2CFGR_ADC2RST_Pos        SMU_ALB2CFGR_ADC2RST_DPos
#define SMU_ALB2CFGR_ADC2RST_Msk        (0x1UL << SMU_ALB2CFGR_ADC2RST_Pos)     /* 0x00000080 */
#define SMU_ALB2CFGR_ADC2RST            SMU_ALB2CFGR_ADC2RST_Msk                /* ADC2 reset */

#define SMU_ALB2CFGR_DAC1EN_DPos        (8U)
#define SMU_ALB2CFGR_DAC1EN_Pos         SMU_ALB2CFGR_DAC1EN_DPos
#define SMU_ALB2CFGR_DAC1EN_Msk         (0x1UL << SMU_ALB2CFGR_DAC1EN_Pos)      /* 0x00000100 */
#define SMU_ALB2CFGR_DAC1EN             SMU_ALB2CFGR_DAC1EN_Msk                 /* DAC1 clock enable */

#define SMU_ALB2CFGR_DAC1RST_DPos       (9U)
#define SMU_ALB2CFGR_DAC1RST_Pos        SMU_ALB2CFGR_DAC1RST_DPos
#define SMU_ALB2CFGR_DAC1RST_Msk        (0x1UL << SMU_ALB2CFGR_DAC1RST_Pos)     /* 0x00000200 */
#define SMU_ALB2CFGR_DAC1RST            SMU_ALB2CFGR_DAC1RST_Msk                /* DAC1 reset */

#define SMU_ALB2CFGR_GPIODEN_DPos       (10U)
#define SMU_ALB2CFGR_GPIODEN_Pos        SMU_ALB2CFGR_GPIODEN_DPos
#define SMU_ALB2CFGR_GPIODEN_Msk        (0x1UL << SMU_ALB2CFGR_GPIODEN_Pos)     /* 0x00000400 */
#define SMU_ALB2CFGR_GPIODEN            SMU_ALB2CFGR_GPIODEN_Msk                /* GPIOD clock enable */

#define SMU_ALB2CFGR_GPIODRST_DPos      (11U)
#define SMU_ALB2CFGR_GPIODRST_Pos       SMU_ALB2CFGR_GPIODRST_DPos
#define SMU_ALB2CFGR_GPIODRST_Msk       (0x1UL << SMU_ALB2CFGR_GPIODRST_Pos)    /* 0x00000800 */
#define SMU_ALB2CFGR_GPIODRST           SMU_ALB2CFGR_GPIODRST_Msk               /* GPIOD reset */

#define SMU_ALB2CFGR_GPIOEEN_DPos       (12U)
#define SMU_ALB2CFGR_GPIOEEN_Pos        SMU_ALB2CFGR_GPIOEEN_DPos
#define SMU_ALB2CFGR_GPIOEEN_Msk        (0x1UL << SMU_ALB2CFGR_GPIOEEN_Pos)     /* 0x00001000 */
#define SMU_ALB2CFGR_GPIOEEN            SMU_ALB2CFGR_GPIOEEN_Msk                /* GPIOE clock enable */

#define SMU_ALB2CFGR_GPIOERST_DPos      (13U)
#define SMU_ALB2CFGR_GPIOERST_Pos       SMU_ALB2CFGR_GPIOERST_DPos
#define SMU_ALB2CFGR_GPIOERST_Msk       (0x1UL << SMU_ALB2CFGR_GPIOERST_Pos)    /* 0x00002000 */
#define SMU_ALB2CFGR_GPIOERST           SMU_ALB2CFGR_GPIOERST_Msk               /* GPIOE reset */

#define SMU_ALB2CFGR_GPIOFEN_DPos       (14U)
#define SMU_ALB2CFGR_GPIOFEN_Pos        SMU_ALB2CFGR_GPIOFEN_DPos
#define SMU_ALB2CFGR_GPIOFEN_Msk        (0x1UL << SMU_ALB2CFGR_GPIOFEN_Pos)     /* 0x00004000 */
#define SMU_ALB2CFGR_GPIOFEN            SMU_ALB2CFGR_GPIOFEN_Msk                /* GPIOF clock enable */

#define SMU_ALB2CFGR_GPIOFRST_DPos      (15U)
#define SMU_ALB2CFGR_GPIOFRST_Pos       SMU_ALB2CFGR_GPIOFRST_DPos
#define SMU_ALB2CFGR_GPIOFRST_Msk       (0x1UL << SMU_ALB2CFGR_GPIOFRST_Pos)    /* 0x00008000 */
#define SMU_ALB2CFGR_GPIOFRST           SMU_ALB2CFGR_GPIOFRST_Msk               /* GPIOF reset */

#define SMU_ALB2CFGR_GPIOGEN_DPos       (16U)
#define SMU_ALB2CFGR_GPIOGEN_Pos        SMU_ALB2CFGR_GPIOGEN_DPos
#define SMU_ALB2CFGR_GPIOGEN_Msk        (0x1UL << SMU_ALB2CFGR_GPIOGEN_Pos)     /* 0x00010000 */
#define SMU_ALB2CFGR_GPIOGEN            SMU_ALB2CFGR_GPIOGEN_Msk                /* GPIOG clock enable */

#define SMU_ALB2CFGR_GPIOGRST_DPos      (17U)
#define SMU_ALB2CFGR_GPIOGRST_Pos       SMU_ALB2CFGR_GPIOGRST_DPos
#define SMU_ALB2CFGR_GPIOGRST_Msk       (0x1UL << SMU_ALB2CFGR_GPIOGRST_Pos)    /* 0x00020000 */
#define SMU_ALB2CFGR_GPIOGRST           SMU_ALB2CFGR_GPIOGRST_Msk               /* GPIOG reset */

#define SMU_ALB2CFGR_AXILITEBUS2EN_DPos (18U)
#define SMU_ALB2CFGR_AXILITEBUS2EN_Pos  SMU_ALB2CFGR_AXILITEBUS2EN_DPos
#define SMU_ALB2CFGR_AXILITEBUS2EN_Msk  (0x1UL << SMU_ALB2CFGR_AXILITEBUS2EN_Pos)   /* 0x00040000 */
#define SMU_ALB2CFGR_AXILITEBUS2EN      SMU_ALB2CFGR_AXILITEBUS2EN_Msk              /* AXILite Bus2 clock enable */

#define SMU_ALB2CFGR_AXILITEBUS2RST_DPos    (19U)
#define SMU_ALB2CFGR_AXILITEBUS2RST_Pos     SMU_ALB2CFGR_AXILITEBUS2RST_DPos
#define SMU_ALB2CFGR_AXILITEBUS2RST_Msk     (0x1UL << SMU_ALB2CFGR_AXILITEBUS2RST_Pos)  /* 0x00080000 */
#define SMU_ALB2CFGR_AXILITEBUS2RST         SMU_ALB2CFGR_AXILITEBUS2RST_Msk             /* AXILite Bus2 reset */


/******************  Bits definition for SMU_APB0CFGR register  *****************/
#define SMU_APB0CFGR_MACEN_DPos         (0U)
#define SMU_APB0CFGR_MACEN_Pos          SMU_APB0CFGR_MACEN_DPos
#define SMU_APB0CFGR_MACEN_Msk          (0x1UL << SMU_APB0CFGR_MACEN_Pos)       /* 0x00000001 */
#define SMU_APB0CFGR_MACEN              SMU_APB0CFGR_MACEN_Msk                  /* MAC clock enable */

#define SMU_APB0CFGR_MACRST_DPos        (1U)
#define SMU_APB0CFGR_MACRST_Pos         SMU_APB0CFGR_MACRST_DPos
#define SMU_APB0CFGR_MACRST_Msk         (0x1UL << SMU_APB0CFGR_MACRST_Pos)      /* 0x00000002 */
#define SMU_APB0CFGR_MACRST             SMU_APB0CFGR_MACRST_Msk                 /* MAC reset */

#define SMU_APB0CFGR_SPI0EN_DPos        (2U)
#define SMU_APB0CFGR_SPI0EN_Pos         SMU_APB0CFGR_SPI0EN_DPos
#define SMU_APB0CFGR_SPI0EN_Msk         (0x1UL << SMU_APB0CFGR_SPI0EN_Pos)      /* 0x00000004 */
#define SMU_APB0CFGR_SPI0EN             SMU_APB0CFGR_SPI0EN_Msk                 /* SPI0 clock enable */

#define SMU_APB0CFGR_SPI0RST_DPos       (3U)
#define SMU_APB0CFGR_SPI0RST_Pos        SMU_APB0CFGR_SPI0RST_DPos
#define SMU_APB0CFGR_SPI0RST_Msk        (0x1UL << SMU_APB0CFGR_SPI0RST_Pos)     /* 0x00000008 */
#define SMU_APB0CFGR_SPI0RST            SMU_APB0CFGR_SPI0RST_Msk                /* SPI0 reset */

#define SMU_APB0CFGR_SPI1EN_DPos        (4U)
#define SMU_APB0CFGR_SPI1EN_Pos         SMU_APB0CFGR_SPI1EN_DPos
#define SMU_APB0CFGR_SPI1EN_Msk         (0x1UL << SMU_APB0CFGR_SPI1EN_Pos)      /* 0x00000010 */
#define SMU_APB0CFGR_SPI1EN             SMU_APB0CFGR_SPI1EN_Msk                 /* SPI1 clock enable */

#define SMU_APB0CFGR_SPI1RST_DPos       (5U)
#define SMU_APB0CFGR_SPI1RST_Pos        SMU_APB0CFGR_SPI1RST_DPos
#define SMU_APB0CFGR_SPI1RST_Msk        (0x1UL << SMU_APB0CFGR_SPI1RST_Pos)     /* 0x00000020 */
#define SMU_APB0CFGR_SPI1RST            SMU_APB0CFGR_SPI1RST_Msk                /* SPI1 reset */

#define SMU_APB0CFGR_SPI2EN_DPos        (6U)
#define SMU_APB0CFGR_SPI2EN_Pos         SMU_APB0CFGR_SPI2EN_DPos
#define SMU_APB0CFGR_SPI2EN_Msk         (0x1UL << SMU_APB0CFGR_SPI2EN_Pos)      /* 0x00000040 */
#define SMU_APB0CFGR_SPI2EN             SMU_APB0CFGR_SPI2EN_Msk                 /* SPI2 clock enable */

#define SMU_APB0CFGR_SPI2RST_DPos       (7U)
#define SMU_APB0CFGR_SPI2RST_Pos        SMU_APB0CFGR_SPI2RST_DPos
#define SMU_APB0CFGR_SPI2RST_Msk        (0x1UL << SMU_APB0CFGR_SPI2RST_Pos)     /* 0x00000080 */
#define SMU_APB0CFGR_SPI2RST            SMU_APB0CFGR_SPI2RST_Msk                /* SPI2 reset */

#define SMU_APB0CFGR_IIC0EN_DPos        (8U)
#define SMU_APB0CFGR_IIC0EN_Pos         SMU_APB0CFGR_IIC0EN_DPos
#define SMU_APB0CFGR_IIC0EN_Msk         (0x1UL << SMU_APB0CFGR_IIC0EN_Pos)      /* 0x00000100 */
#define SMU_APB0CFGR_IIC0EN             SMU_APB0CFGR_IIC0EN_Msk                 /* IIC0 clock enable */

#define SMU_APB0CFGR_IIC0RST_DPos       (9U)
#define SMU_APB0CFGR_IIC0RST_Pos        SMU_APB0CFGR_IIC0RST_DPos
#define SMU_APB0CFGR_IIC0RST_Msk        (0x1UL << SMU_APB0CFGR_IIC0RST_Pos)     /* 0x00000200 */
#define SMU_APB0CFGR_IIC0RST            SMU_APB0CFGR_IIC0RST_Msk                /* IIC0 reset */

#define SMU_APB0CFGR_IIC1EN_DPos        (10U)
#define SMU_APB0CFGR_IIC1EN_Pos         SMU_APB0CFGR_IIC1EN_DPos
#define SMU_APB0CFGR_IIC1EN_Msk         (0x1UL << SMU_APB0CFGR_IIC1EN_Pos)      /* 0x00000400 */
#define SMU_APB0CFGR_IIC1EN             SMU_APB0CFGR_IIC1EN_Msk                 /* IIC1 clock enable */

#define SMU_APB0CFGR_IIC1RST_DPos       (11U)
#define SMU_APB0CFGR_IIC1RST_Pos        SMU_APB0CFGR_IIC1RST_DPos
#define SMU_APB0CFGR_IIC1RST_Msk        (0x1UL << SMU_APB0CFGR_IIC1RST_Pos)     /* 0x00000800 */
#define SMU_APB0CFGR_IIC1RST            SMU_APB0CFGR_IIC1RST_Msk                /* IIC1 reset */

#define SMU_APB0CFGR_USART0EN_DPos      (12U)
#define SMU_APB0CFGR_USART0EN_Pos       SMU_APB0CFGR_USART0EN_DPos
#define SMU_APB0CFGR_USART0EN_Msk       (0x1UL << SMU_APB0CFGR_USART0EN_Pos)    /* 0x00001000 */
#define SMU_APB0CFGR_USART0EN           SMU_APB0CFGR_USART0EN_Msk               /* USART0 clock enable */

#define SMU_APB0CFGR_USART0RST_DPos     (13U)
#define SMU_APB0CFGR_USART0RST_Pos      SMU_APB0CFGR_USART0RST_DPos
#define SMU_APB0CFGR_USART0RST_Msk      (0x1UL << SMU_APB0CFGR_USART0RST_Pos)   /* 0x00002000 */
#define SMU_APB0CFGR_USART0RST          SMU_APB0CFGR_USART0RST_Msk              /* USART0 reset */

#define SMU_APB0CFGR_USART1EN_DPos      (14U)
#define SMU_APB0CFGR_USART1EN_Pos       SMU_APB0CFGR_USART1EN_DPos
#define SMU_APB0CFGR_USART1EN_Msk       (0x1UL << SMU_APB0CFGR_USART1EN_Pos)    /* 0x00004000 */
#define SMU_APB0CFGR_USART1EN           SMU_APB0CFGR_USART1EN_Msk               /* USART1 clock enable */

#define SMU_APB0CFGR_USART1RST_DPos     (15U)
#define SMU_APB0CFGR_USART1RST_Pos      SMU_APB0CFGR_USART1RST_DPos
#define SMU_APB0CFGR_USART1RST_Msk      (0x1UL << SMU_APB0CFGR_USART1RST_Pos)   /* 0x00008000 */
#define SMU_APB0CFGR_USART1RST          SMU_APB0CFGR_USART1RST_Msk              /* USART1 reset */

#define SMU_APB0CFGR_USART2EN_DPos      (16U)
#define SMU_APB0CFGR_USART2EN_Pos       SMU_APB0CFGR_USART2EN_DPos
#define SMU_APB0CFGR_USART2EN_Msk       (0x1UL << SMU_APB0CFGR_USART2EN_Pos)    /* 0x00010000 */
#define SMU_APB0CFGR_USART2EN           SMU_APB0CFGR_USART2EN_Msk               /* USART2 clock enable */

#define SMU_APB0CFGR_USART2RST_DPos     (17U)
#define SMU_APB0CFGR_USART2RST_Pos      SMU_APB0CFGR_USART2RST_DPos
#define SMU_APB0CFGR_USART2RST_Msk      (0x1UL << SMU_APB0CFGR_USART2RST_Pos)   /* 0x00020000 */
#define SMU_APB0CFGR_USART2RST          SMU_APB0CFGR_USART2RST_Msk              /* USART2 reset */

#define SMU_APB0CFGR_USART3EN_DPos      (18U)
#define SMU_APB0CFGR_USART3EN_Pos       SMU_APB0CFGR_USART3EN_DPos
#define SMU_APB0CFGR_USART3EN_Msk       (0x1UL << SMU_APB0CFGR_USART3EN_Pos)    /* 0x00040000 */
#define SMU_APB0CFGR_USART3EN           SMU_APB0CFGR_USART3EN_Msk               /* USART3 clock enable */

#define SMU_APB0CFGR_USART3RST_DPos     (19U)
#define SMU_APB0CFGR_USART3RST_Pos      SMU_APB0CFGR_USART3RST_DPos
#define SMU_APB0CFGR_USART3RST_Msk      (0x1UL << SMU_APB0CFGR_USART3RST_Pos)   /* 0x00080000 */
#define SMU_APB0CFGR_USART3RST          SMU_APB0CFGR_USART3RST_Msk              /* USART3 reset */

#define SMU_APB0CFGR_HTIM2EN_DPos       (20U)
#define SMU_APB0CFGR_HTIM2EN_Pos        SMU_APB0CFGR_HTIM2EN_DPos
#define SMU_APB0CFGR_HTIM2EN_Msk        (0x1UL << SMU_APB0CFGR_HTIM2EN_Pos)     /* 0x00100000 */
#define SMU_APB0CFGR_HTIM2EN            SMU_APB0CFGR_HTIM2EN_Msk                /* HTIM2 clock enable */

#define SMU_APB0CFGR_HTIM2RST_DPos      (21U)
#define SMU_APB0CFGR_HTIM2RST_Pos       SMU_APB0CFGR_HTIM2RST_DPos
#define SMU_APB0CFGR_HTIM2RST_Msk       (0x1UL << SMU_APB0CFGR_HTIM2RST_Pos)    /* 0x00200000 */
#define SMU_APB0CFGR_HTIM2RST           SMU_APB0CFGR_HTIM2RST_Msk               /* HTIM2 reset */

#define SMU_APB0CFGR_TIM3EN_DPos        (22U)
#define SMU_APB0CFGR_TIM3EN_Pos         SMU_APB0CFGR_TIM3EN_DPos
#define SMU_APB0CFGR_TIM3EN_Msk         (0x1UL << SMU_APB0CFGR_TIM3EN_Pos)      /* 0x00400000 */
#define SMU_APB0CFGR_TIM3EN             SMU_APB0CFGR_TIM3EN_Msk                 /* TIM3 clock enable */

#define SMU_APB0CFGR_TIM3RST_DPos       (23U)
#define SMU_APB0CFGR_TIM3RST_Pos        SMU_APB0CFGR_TIM3RST_DPos
#define SMU_APB0CFGR_TIM3RST_Msk        (0x1UL << SMU_APB0CFGR_TIM3RST_Pos)     /* 0x00800000 */
#define SMU_APB0CFGR_TIM3RST           SMU_APB0CFGR_TIM3RST_Msk                 /* TIM3 reset */

#define SMU_APB0CFGR_TIM4EN_DPos        (24U)
#define SMU_APB0CFGR_TIM4EN_Pos         SMU_APB0CFGR_TIM4EN_DPos
#define SMU_APB0CFGR_TIM4EN_Msk         (0x1UL << SMU_APB0CFGR_TIM4EN_Pos)      /* 0x01000000 */
#define SMU_APB0CFGR_TIM4EN             SMU_APB0CFGR_TIM4EN_Msk                 /* TIM4 clock enable */

#define SMU_APB0CFGR_TIM4RST_DPos       (25U)
#define SMU_APB0CFGR_TIM4RST_Pos        SMU_APB0CFGR_TIM4RST_DPos
#define SMU_APB0CFGR_TIM4RST_Msk        (0x1UL << SMU_APB0CFGR_TIM4RST_Pos)     /* 0x02000000 */
#define SMU_APB0CFGR_TIM4RST            SMU_APB0CFGR_TIM4RST_Msk                /* TIM4 reset */

#define SMU_APB0CFGR_WWDG0EN_DPos       (26U)
#define SMU_APB0CFGR_WWDG0EN_Pos        SMU_APB0CFGR_WWDG0EN_DPos
#define SMU_APB0CFGR_WWDG0EN_Msk        (0x1UL << SMU_APB0CFGR_WWDG0EN_Pos)     /* 0x04000000 */
#define SMU_APB0CFGR_WWDG0EN            SMU_APB0CFGR_WWDG0EN_Msk                /* WWDG0 clock enable */

#define SMU_APB0CFGR_WWDG0RST_DPos      (27U)
#define SMU_APB0CFGR_WWDG0RST_Pos       SMU_APB0CFGR_WWDG0RST_DPos
#define SMU_APB0CFGR_WWDG0RST_Msk       (0x1UL << SMU_APB0CFGR_WWDG0RST_Pos)    /* 0x08000000 */
#define SMU_APB0CFGR_WWDG0RST           SMU_APB0CFGR_WWDG0RST_Msk               /* WWDG0 reset */

#define SMU_APB0CFGR_APBBUS0EN_DPos     (30U)
#define SMU_APB0CFGR_APBBUS0EN_Pos      SMU_APB0CFGR_APBBUS0EN_DPos
#define SMU_APB0CFGR_APBBUS0EN_Msk      (0x1UL << SMU_APB0CFGR_APBBUS0EN_Pos)   /* 0x40000000 */
#define SMU_APB0CFGR_APBBUS0EN          SMU_APB0CFGR_APBBUS0EN_Msk              /* APBBUS0 clock enable */

#define SMU_APB0CFGR_APBBUS0RST_DPos    (31U)
#define SMU_APB0CFGR_APBBUS0RST_Pos     SMU_APB0CFGR_APBBUS0RST_DPos
#define SMU_APB0CFGR_APBBUS0RST_Msk     (0x1UL << SMU_APB0CFGR_APBBUS0RST_Pos)  /* 0x80000000 */
#define SMU_APB0CFGR_APBBUS0RST         SMU_APB0CFGR_APBBUS0RST_Msk             /* APBBUS0 reset */


/******************  Bits definition for SMU_APB1CFGR register  *****************/
#define SMU_APB1CFGR_SPI3EN_DPos        (0U)
#define SMU_APB1CFGR_SPI3EN_Pos         SMU_APB1CFGR_SPI3EN_DPos
#define SMU_APB1CFGR_SPI3EN_Msk         (0x1UL << SMU_APB1CFGR_SPI3EN_Pos)      /* 0x00000001 */
#define SMU_APB1CFGR_SPI3EN             SMU_APB1CFGR_SPI3EN_Msk                 /* SPI3 clock enable */

#define SMU_APB1CFGR_SPI3RST_DPos       (1U)
#define SMU_APB1CFGR_SPI3RST_Pos        SMU_APB1CFGR_SPI3RST_DPos
#define SMU_APB1CFGR_SPI3RST_Msk        (0x1UL << SMU_APB1CFGR_SPI3RST_Pos)     /* 0x00000002 */
#define SMU_APB1CFGR_SPI3RST            SMU_APB1CFGR_SPI3RST_Msk                /* SPI3 reset */

#define SMU_APB1CFGR_SPI4EN_DPos        (2U)
#define SMU_APB1CFGR_SPI4EN_Pos         SMU_APB1CFGR_SPI4EN_DPos
#define SMU_APB1CFGR_SPI4EN_Msk         (0x1UL << SMU_APB1CFGR_SPI4EN_Pos)      /* 0x00000004 */
#define SMU_APB1CFGR_SPI4EN             SMU_APB1CFGR_SPI4EN_Msk                 /* SPI4 clock enable */

#define SMU_APB1CFGR_SPI4RST_DPos       (3U)
#define SMU_APB1CFGR_SPI4RST_Pos        SMU_APB1CFGR_SPI4RST_DPos
#define SMU_APB1CFGR_SPI4RST_Msk        (0x1UL << SMU_APB1CFGR_SPI4RST_Pos)     /* 0x00000008 */
#define SMU_APB1CFGR_SPI4RST            SMU_APB1CFGR_SPI4RST_Msk                /* SPI4 reset */

#define SMU_APB1CFGR_SPI5EN_DPos        (4U)
#define SMU_APB1CFGR_SPI5EN_Pos         SMU_APB1CFGR_SPI5EN_DPos
#define SMU_APB1CFGR_SPI5EN_Msk         (0x1UL << SMU_APB1CFGR_SPI5EN_Pos)      /* 0x00000010 */
#define SMU_APB1CFGR_SPI5EN             SMU_APB1CFGR_SPI5EN_Msk                 /* SPI5 clock enable */

#define SMU_APB1CFGR_SPI5RST_DPos       (5U)
#define SMU_APB1CFGR_SPI5RST_Pos        SMU_APB1CFGR_SPI5RST_DPos
#define SMU_APB1CFGR_SPI5RST_Msk        (0x1UL << SMU_APB1CFGR_SPI5RST_Pos)     /* 0x00000020 */
#define SMU_APB1CFGR_SPI5RST            SMU_APB1CFGR_SPI5RST_Msk                /* SPI5 reset */

#define SMU_APB1CFGR_IIC2EN_DPos        (6U)
#define SMU_APB1CFGR_IIC2EN_Pos         SMU_APB1CFGR_IIC2EN_DPos
#define SMU_APB1CFGR_IIC2EN_Msk         (0x1UL << SMU_APB1CFGR_IIC2EN_Pos)      /* 0x00000040 */
#define SMU_APB1CFGR_IIC2EN             SMU_APB1CFGR_IIC2EN_Msk                 /* IIC2 clock enable */

#define SMU_APB1CFGR_IIC2RST_DPos       (7U)
#define SMU_APB1CFGR_IIC2RST_Pos        SMU_APB1CFGR_IIC2RST_DPos
#define SMU_APB1CFGR_IIC2RST_Msk        (0x1UL << SMU_APB1CFGR_IIC2RST_Pos)     /* 0x00000080 */
#define SMU_APB1CFGR_IIC2RST            SMU_APB1CFGR_IIC2RST_Msk                /* IIC2 reset */

#define SMU_APB1CFGR_IIC3EN_DPos        (8U)
#define SMU_APB1CFGR_IIC3EN_Pos         SMU_APB1CFGR_IIC3EN_DPos
#define SMU_APB1CFGR_IIC3EN_Msk         (0x1UL << SMU_APB1CFGR_IIC3EN_Pos)      /* 0x00000100 */
#define SMU_APB1CFGR_IIC3EN             SMU_APB1CFGR_IIC3EN_Msk                 /* IIC3 clock enable */

#define SMU_APB1CFGR_IIC3RST_DPos       (9U)
#define SMU_APB1CFGR_IIC3RST_Pos        SMU_APB1CFGR_IIC3RST_DPos
#define SMU_APB1CFGR_IIC3RST_Msk        (0x1UL << SMU_APB1CFGR_IIC3RST_Pos)     /* 0x00000200 */
#define SMU_APB1CFGR_IIC3RST            SMU_APB1CFGR_IIC3RST_Msk                /* IIC3 reset */

#define SMU_APB1CFGR_USART4EN_DPos      (10U)
#define SMU_APB1CFGR_USART4EN_Pos       SMU_APB1CFGR_USART4EN_DPos
#define SMU_APB1CFGR_USART4EN_Msk       (0x1UL << SMU_APB1CFGR_USART4EN_Pos)    /* 0x00000400 */
#define SMU_APB1CFGR_USART4EN            SMU_APB1CFGR_USART4EN_Msk              /* USART4 clock enable */

#define SMU_APB1CFGR_USART4RST_DPos     (11U)
#define SMU_APB1CFGR_USART4RST_Pos      SMU_APB1CFGR_USART4RST_DPos
#define SMU_APB1CFGR_USART4RST_Msk      (0x1UL << SMU_APB1CFGR_USART4RST_Pos)   /* 0x00000800 */
#define SMU_APB1CFGR_USART4RST            SMU_APB1CFGR_USART4RST_Msk            /* USART4 reset */

#define SMU_APB1CFGR_USART5EN_DPos      (12U)
#define SMU_APB1CFGR_USART5EN_Pos       SMU_APB1CFGR_USART5EN_DPos
#define SMU_APB1CFGR_USART5EN_Msk       (0x1UL << SMU_APB1CFGR_USART5EN_Pos)    /* 0x00001000 */
#define SMU_APB1CFGR_USART5EN            SMU_APB1CFGR_USART5EN_Msk              /* USART5 clock enable */

#define SMU_APB1CFGR_USART5RST_DPos     (13U)
#define SMU_APB1CFGR_USART5RST_Pos      SMU_APB1CFGR_USART5RST_DPos
#define SMU_APB1CFGR_USART5RST_Msk      (0x1UL << SMU_APB1CFGR_USART5RST_Pos)   /* 0x00002000 */
#define SMU_APB1CFGR_USART5RST          SMU_APB1CFGR_USART5RST_Msk              /* USART5 reset */

#define SMU_APB1CFGR_USART6EN_DPos      (14U)
#define SMU_APB1CFGR_USART6EN_Pos       SMU_APB1CFGR_USART6EN_DPos
#define SMU_APB1CFGR_USART6EN_Msk       (0x1UL << SMU_APB1CFGR_USART6EN_Pos)    /* 0x00004000 */
#define SMU_APB1CFGR_USART6EN           SMU_APB1CFGR_USART6EN_Msk               /* USART6 clock enable */

#define SMU_APB1CFGR_USART6RST_DPos     (15U)
#define SMU_APB1CFGR_USART6RST_Pos      SMU_APB1CFGR_USART6RST_DPos
#define SMU_APB1CFGR_USART6RST_Msk      (0x1UL << SMU_APB1CFGR_USART6RST_Pos)   /* 0x00008000 */
#define SMU_APB1CFGR_USART6RST          SMU_APB1CFGR_USART6RST_Msk              /* USART6 reset */

#define SMU_APB1CFGR_USART7EN_DPos      (16U)
#define SMU_APB1CFGR_USART7EN_Pos       SMU_APB1CFGR_USART7EN_DPos
#define SMU_APB1CFGR_USART7EN_Msk       (0x1UL << SMU_APB1CFGR_USART7EN_Pos)    /* 0x00010000 */
#define SMU_APB1CFGR_USART7EN           SMU_APB1CFGR_USART7EN_Msk               /* USART7 clock enable */

#define SMU_APB1CFGR_USART7RST_DPos     (17U)
#define SMU_APB1CFGR_USART7RST_Pos      SMU_APB1CFGR_USART7RST_DPos
#define SMU_APB1CFGR_USART7RST_Msk      (0x1UL << SMU_APB1CFGR_USART7RST_Pos)   /* 0x00020000 */
#define SMU_APB1CFGR_USART7RST          SMU_APB1CFGR_USART7RST_Msk              /* USART7 reset */

#define SMU_APB1CFGR_HTIM5EN_DPos       (18U)
#define SMU_APB1CFGR_HTIM5EN_Pos        SMU_APB1CFGR_HTIM5EN_DPos
#define SMU_APB1CFGR_HTIM5EN_Msk        (0x1UL << SMU_APB1CFGR_HTIM5EN_Pos)     /* 0x00040000 */
#define SMU_APB1CFGR_HTIM5EN            SMU_APB1CFGR_HTIM5EN_Msk                /* HTIM5 clock enable */

#define SMU_APB1CFGR_HTIM5RST_DPos      (19U)
#define SMU_APB1CFGR_HTIM5RST_Pos       SMU_APB1CFGR_HTIM5RST_DPos
#define SMU_APB1CFGR_HTIM5RST_Msk       (0x1UL << SMU_APB1CFGR_HTIM5RST_Pos)    /* 0x00080000 */
#define SMU_APB1CFGR_HTIM5RST           SMU_APB1CFGR_HTIM5RST_Msk               /* HTIM5 reset */

#define SMU_APB1CFGR_TIM6EN_DPos        (20U)
#define SMU_APB1CFGR_TIM6EN_Pos         SMU_APB1CFGR_TIM6EN_DPos
#define SMU_APB1CFGR_TIM6EN_Msk         (0x1UL << SMU_APB1CFGR_TIM6EN_Pos)      /* 0x00100000 */
#define SMU_APB1CFGR_TIM6EN             SMU_APB1CFGR_TIM6EN_Msk                 /* TIM6 clock enable */

#define SMU_APB1CFGR_TIM6RST_DPos       (21U)
#define SMU_APB1CFGR_TIM6RST_Pos        SMU_APB1CFGR_TIM6RST_DPos
#define SMU_APB1CFGR_TIM6RST_Msk        (0x1UL << SMU_APB1CFGR_TIM6RST_Pos)     /* 0x00200000 */
#define SMU_APB1CFGR_TIM6RST            SMU_APB1CFGR_TIM6RST_Msk                /* TIM6 reset */

#define SMU_APB1CFGR_TIM7EN_DPos        (22U)
#define SMU_APB1CFGR_TIM7EN_Pos         SMU_APB1CFGR_TIM7EN_DPos
#define SMU_APB1CFGR_TIM7EN_Msk         (0x1UL << SMU_APB1CFGR_TIM7EN_Pos)      /* 0x00400000 */
#define SMU_APB1CFGR_TIM7EN             SMU_APB1CFGR_TIM7EN_Msk                 /* TIM7 clock enable */

#define SMU_APB1CFGR_TIM7RST_DPos       (23U)
#define SMU_APB1CFGR_TIM7RST_Pos        SMU_APB1CFGR_TIM7RST_DPos
#define SMU_APB1CFGR_TIM7RST_Msk        (0x1UL << SMU_APB1CFGR_TIM7RST_Pos)     /* 0x00800000 */
#define SMU_APB1CFGR_TIM7RST            SMU_APB1CFGR_TIM7RST_Msk                /* TIM7 reset */

#define SMU_APB1CFGR_WWDG1EN_DPos       (24U)
#define SMU_APB1CFGR_WWDG1EN_Pos        SMU_APB1CFGR_WWDG1EN_DPos
#define SMU_APB1CFGR_WWDG1EN_Msk        (0x1UL << SMU_APB1CFGR_WWDG1EN_Pos)     /* 0x01000000 */
#define SMU_APB1CFGR_WWDG1EN            SMU_APB1CFGR_WWDG1EN_Msk                /* WWDG1 clock enable */

#define SMU_APB1CFGR_WWDG1RST_DPos      (25U)
#define SMU_APB1CFGR_WWDG1RST_Pos       SMU_APB1CFGR_WWDG1RST_DPos
#define SMU_APB1CFGR_WWDG1RST_Msk       (0x1UL << SMU_APB1CFGR_WWDG1RST_Pos)    /* 0x02000000 */
#define SMU_APB1CFGR_WWDG1RST           SMU_APB1CFGR_WWDG1RST_Msk               /* WWDG1 reset */

#define SMU_APB1CFGR_CRCEN_DPos         (26U)
#define SMU_APB1CFGR_CRCEN_Pos          SMU_APB1CFGR_CRCEN_DPos
#define SMU_APB1CFGR_CRCEN_Msk          (0x1UL << SMU_APB1CFGR_CRCEN_Pos)       /* 0x04000000 */
#define SMU_APB1CFGR_CRCEN              SMU_APB1CFGR_CRCEN_Msk                  /* CRC clock enable */

#define SMU_APB1CFGR_CRCRST_DPos        (27U)
#define SMU_APB1CFGR_CRCRST_Pos         SMU_APB1CFGR_CRCRST_DPos
#define SMU_APB1CFGR_CRCRST_Msk         (0x1UL << SMU_APB1CFGR_CRCRST_Pos)      /* 0x08000000 */
#define SMU_APB1CFGR_CRCRST             SMU_APB1CFGR_CRCRST_Msk                 /* CRC reset */

#define SMU_APB1CFGR_APBBUS1EN_DPos     (28U)
#define SMU_APB1CFGR_APBBUS1EN_Pos      SMU_APB1CFGR_APBBUS1EN_DPos
#define SMU_APB1CFGR_APBBUS1EN_Msk      (0x1UL << SMU_APB1CFGR_APBBUS1EN_Pos)   /* 0x10000000 */
#define SMU_APB1CFGR_APBBUS1EN          SMU_APB1CFGR_APBBUS1EN_Msk              /* APB1BUS1 clock enable */

#define SMU_APB1CFGR_APBBUS1RST_DPos    (29U)
#define SMU_APB1CFGR_APBBUS1RST_Pos     SMU_APB1CFGR_APBBUS1RST_DPos
#define SMU_APB1CFGR_APBBUS1RST_Msk     (0x1UL << SMU_APB1CFGR_APBBUS1RST_Pos)  /* 0x20000000 */
#define SMU_APB1CFGR_APBBUS1RST         SMU_APB1CFGR_APBBUS1RST_Msk             /* APB1BUS1 reset */


/******************  Bits definition for SMU_OTHCFGR register  *****************/
#define SMU_OTHCFGR_dclsEnable_DPos     (0U)
#define SMU_OTHCFGR_DCLSEN_Pos          SMU_OTHCFGR_dclsEnable_DPos
#define SMU_OTHCFGR_DCLSEN_Msk          (0x1UL << SMU_OTHCFGR_DCLSEN_Pos)       /* 0x00000001 */
#define SMU_OTHCFGR_DCLSEN              SMU_OTHCFGR_DCLSEN_Msk                  /* Dual core enable */

#define SMU_OTHCFGR_eccEnable_DPos      (1U)
#define SMU_OTHCFGR_ECCEN_Pos           SMU_OTHCFGR_eccEnable_DPos
#define SMU_OTHCFGR_ECCEN_Msk           (0x1UL << SMU_OTHCFGR_ECCEN_Pos)        /* 0x00000002 */
#define SMU_OTHCFGR_ECCEN               SMU_OTHCFGR_ECCEN_Msk                   /* ECC enable */

#define SMU_OTHCFGR_timerBreak_DPos     (2U)
#define SMU_OTHCFGR_TIMBKEN_Pos         SMU_OTHCFGR_timerBreak_DPos
#define SMU_OTHCFGR_TIMBKEN_Msk         (0xFUL << SMU_OTHCFGR_TIMBKEN_Pos)      /* 0x0000003C */
#define SMU_OTHCFGR_TIMBKEN             SMU_OTHCFGR_TIMBKEN_Msk                 /* TIMBKEN[3:0] bits (Advanced Timer Brake Enable) */
#define SMU_OTHCFGR_TIMBKEN_0           (0x01UL << SMU_OTHCFGR_TIMBKEN_Pos)     /* 0x00000004 */
#define SMU_OTHCFGR_TIMBKEN_1           (0x02UL << SMU_OTHCFGR_TIMBKEN_Pos)     /* 0x00000008 */
#define SMU_OTHCFGR_TIMBKEN_2           (0x04UL << SMU_OTHCFGR_TIMBKEN_Pos)     /* 0x00000010 */
#define SMU_OTHCFGR_TIMBKEN_3           (0x08UL << SMU_OTHCFGR_TIMBKEN_Pos)     /* 0x00000020 */

#define SMU_OTHCFGR_clrTimeOutFlag_DPos (6U)
#define SMU_OTHCFGR_TIMEOUTCF_Pos       SMU_OTHCFGR_clrTimeOutFlag_DPos
#define SMU_OTHCFGR_TIMEOUTCF_Msk       (0x1UL << SMU_OTHCFGR_TIMEOUTCF_Pos)    /* 0x00000040 */
#define SMU_OTHCFGR_TIMEOUTCF           SMU_OTHCFGR_TIMEOUTCF_Msk               /* Clear timeout flag */

#define SMU_OTHCFGR_sramLowPowerEn_DPos (8U)
#define SMU_OTHCFGR_SRAMLPEN_Pos        SMU_OTHCFGR_sramLowPowerEn_DPos
#define SMU_OTHCFGR_SRAMLPEN_Msk        (0xFUL << SMU_OTHCFGR_SRAMLPEN_Pos)     /* 0x00000F00 */
#define SMU_OTHCFGR_SRAMLPEN            SMU_OTHCFGR_SRAMLPEN_Msk                /* SRAMLPEN[3:0] bits (SRAM Low Power Enable Signal) */
#define SMU_OTHCFGR_SRAMLPEN_0          (0x01UL << SMU_OTHCFGR_SRAMLPEN_Pos)    /* 0x00000100 */
#define SMU_OTHCFGR_SRAMLPEN_1          (0x02UL << SMU_OTHCFGR_SRAMLPEN_Pos)    /* 0x00000200 */
#define SMU_OTHCFGR_SRAMLPEN_2          (0x04UL << SMU_OTHCFGR_SRAMLPEN_Pos)    /* 0x00000400 */
#define SMU_OTHCFGR_SRAMLPEN_3          (0x08UL << SMU_OTHCFGR_SRAMLPEN_Pos)    /* 0x00000800 */

#define SMU_OTHCFGR_clkOutMux_DPos      (12U)
#define SMU_OTHCFGR_CLKOUTMUX_Pos       SMU_OTHCFGR_clkOutMux_DPos
#define SMU_OTHCFGR_CLKOUTMUX_Msk       (0x7UL << SMU_OTHCFGR_CLKOUTMUX_Pos)    /* 0x00007000 */
#define SMU_OTHCFGR_CLKOUTMUX           SMU_OTHCFGR_CLKOUTMUX_Msk               /* CLKOUTMUX[2:0] bits (Clock Output Mux Select) */
#define SMU_OTHCFGR_CLKOUTMUX_0         (0x01UL << SMU_OTHCFGR_CLKOUTMUX_Pos)   /* 0x00001000 */
#define SMU_OTHCFGR_CLKOUTMUX_1         (0x02UL << SMU_OTHCFGR_CLKOUTMUX_Pos)   /* 0x00002000 */
#define SMU_OTHCFGR_CLKOUTMUX_2         (0x04UL << SMU_OTHCFGR_CLKOUTMUX_Pos)   /* 0x00004000 */


/******************  Bits definition for SMU_BUSTCFGR register  *****************/
#define SMU_BUSTCFGR_busTimeOutNum_DPos (0U)
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos  SMU_BUSTCFGR_busTimeOutNum_DPos
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_Msk  (0xFFFFUL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x0000FFFF */
#define SMU_BUSTCFGR_BUSTIMEOUTNUM      SMU_BUSTCFGR_BUSTIMEOUTNUM_Msk                  /* BUSTIMEOUTNUM[15:0] bits (Bus Timeout Counter) */
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_0    (0x0001UL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x00000001*/
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_1    (0x0002UL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x00000002*/
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_2    (0x0004UL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x00000004*/
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_3    (0x0008UL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x00000008*/
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_4    (0x0010UL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x00000010*/
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_5    (0x0020UL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x00000020*/
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_6    (0x0040UL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x00000040*/
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_7    (0x0080UL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x00000080*/
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_8    (0x0100UL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x00000100*/
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_9    (0x0200UL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x00000200*/
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_10   (0x0400UL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x00000400*/
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_11   (0x0800UL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x00000800*/
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_12   (0x1000UL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x00001000*/
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_13   (0x2000UL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x00002000*/
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_14   (0x4000UL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x00004000*/
#define SMU_BUSTCFGR_BUSTIMEOUTNUM_15   (0x8000UL << SMU_BUSTCFGR_BUSTIMEOUTNUM_Pos)    /* 0x00008000*/

#define SMU_BUSTCFGR_busMonEn_DPos      (16U)
#define SMU_BUSTCFGR_BUSTIMEOUTEN_Pos   SMU_BUSTCFGR_busMonEn_DPos
#define SMU_BUSTCFGR_BUSTIMEOUTEN_Msk   (0x1UL << SMU_BUSTCFGR_BUSTIMEOUTEN_Pos)    /* 0x00010000 */
#define SMU_BUSTCFGR_BUSTIMEOUTEN       SMU_BUSTCFGR_BUSTIMEOUTEN_Msk               /* Bus Timeout Enable */


#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
