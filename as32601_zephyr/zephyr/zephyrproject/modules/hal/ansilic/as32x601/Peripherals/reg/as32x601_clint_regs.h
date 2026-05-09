/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_clint_regs.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:            2024/08/19
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/08/19     1.0     build this module
 * Description:     This file contains all the register correlation defines for the CLINT firmware library.
 */

#ifndef AS32X601_CLINT_REG_H
#define AS32X601_CLINT_REG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601.h"
#include "as32x601_addrmap.h"

/*
 * @brief CLINT registers structure
 */
typedef struct
{
    __IO uint32_t SOFTICR;          /* CLINT_SOFT       Soft interrupt control register,        Address offset: 0x0000 */
    uint32_t RESERVED0[4095];       /* Reserved,                                                Address offset: 0x04 - 0x3FFC */
    __O  uint32_t CMPLR;            /* CLINT_CMP_L      Compare registers with lower 32 bits,   Address offset: 0x4000 */
    __O  uint32_t CMPHR;            /* CLINT_CMP_H      Compare registers with 32 higher bits,  Address offset: 0x4004 */
    uint32_t RESERVED1[8188];       /* Reserved,                                                Address offset: 0x4008 - 0xBFF4 */
    __I  uint32_t RDTIMELR;         /* CLINT_DR_TIME_L  Real-time counter low 32 bit register,  Address offset: 0XBFF8 */
    __I  uint32_t RDTIMEHR;         /* CLINT_DR_TIME_H  Real-time counter high 32 bit register, Address offset: 0XBFFC */
} CLINT_TypeDef;

/*
 * @brief CLINT peripheral declaration
 */
#define CLINT   ((CLINT_TypeDef *)CLINT_BASE)

/******************  Bits definition for CLINT_SOFTICR register  *****************/
#define CLINT_SOFTICR_SOFT_DPos     (0U)
#define CLINT_SOFTICR_SOFT_Pos      CLINT_SOFTICR_SOFT_DPos
#define CLINT_SOFTICR_SOFT_Msk      (0x1UL << CLINT_SOFTICR_SOFT_Pos)               /* 0x00000001 */
#define CLINT_SOFTICR_SOFT          CLINT_SOFTICR_SOFT_Msk                          /* Writing 1 will trigger a soft interrupt. */

/******************  Bits definition for CLINT_CMPLR register  *****************/
#define CLINT_CMPLR_CMP_DPos        (0U)
#define CLINT_CMPLR_CMP_Pos         CLINT_CMPLR_CMP_DPos
#define CLINT_CMPLR_CMP_Msk         (0xFFFFFFFFUL << CLINT_CMPLR_CMP_Pos)           /* 0xFFFFFFFF */
#define CLINT_CMPLR_CMP             CLINT_CMPLR_CMP_Msk                             /* CMP[31:0] bits (Comparator value that generates a timer interrupt when the real-time counter is equal to this value.) */

/******************  Bits definition for CLINT_CMPHR register  *****************/
#define CLINT_CMPHR_CMP_DPos        (0U)
#define CLINT_CMPHR_CMP_Pos         CLINT_CMPHR_CMP_DPos
#define CLINT_CMPHR_CMP_Msk         (0xFFFFFFFFUL << CLINT_CMPHR_CMP_Pos)           /* 0xFFFFFFFF */
#define CLINT_CMPHR_CMP             CLINT_CMPHR_CMP_Msk                             /* CMP[31:0] bits (Comparator value that generates a timer interrupt when the real-time counter is equal to this value.) */

/******************  Bits definition for CLINT_RDTIMELR register  *****************/
#define CLINT_RDTIMELR_DRTIME_DPos  (0U)
#define CLINT_RDTIMELR_DRTIME_Pos   CLINT_RDTIMELR_DRTIME_DPos
#define CLINT_RDTIMELR_DRTIME_Msk   (0xFFFFFFFFUL << CLINT_RDTIMELR_DRTIME_Pos)     /* 0xFFFFFFFF */
#define CLINT_RDTIMELR_DRTIME       CLINT_RDTIMELR_DRTIME_Msk                       /* DRTIME[31:0] bits (Real-time counter low 32 bit) */

/******************  Bits definition for CLINT_RDTIMEHR register  *****************/
#define CLINT_RDTIMEHR_DRTIME_DPos  (0U)
#define CLINT_RDTIMEHR_DRTIME_Pos   CLINT_RDTIMEHR_DRTIME_DPos
#define CLINT_RDTIMEHR_DRTIME_Msk   (0xFFFFFFFFUL << CLINT_RDTIMEHR_DRTIME_Pos)     /* 0xFFFFFFFF */
#define CLINT_RDTIMEHR_DRTIME       CLINT_RDTIMEHR_DRTIME_Msk                       /* DRTIME[31:0] bits (Real-time counter high 32 bit) */

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
