/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_plic_regs.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:            2023/11/21
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2023/11/21     1.0     build this module
 * Description:     This file contains all the register correlation defines for the PLIC firmware library.
 */

#ifndef AS32X601_PLIC_REG_H
#define AS32X601_PLIC_REG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601_addrmap.h"
#include "as32x601.h"

/*
 * @brief PLIC registers structure
 */
typedef struct
{
    __IO uint32_t AXI_PLIC_ID;              /* Interrupt source ID numer registers,         Address offset: 0x00 */
    __IO uint32_t AXI_PLIC_PRIORITY[64];    /* Interrupt source priority register 0 - 63,   Address offset: 0x04 - 0x100 */
    __IO uint32_t AXI_PLIC_EN[2];           /* Interrupt source enable registe 0 - 1,       Address offset: 0x0104 - 0x0108 */
} PLIC_TypeDef;

/*
 * @addtogroup PLIC peripheral declaration
 */
#define PLIC    ((PLIC_TypeDef *)PLIC_BASE)

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
