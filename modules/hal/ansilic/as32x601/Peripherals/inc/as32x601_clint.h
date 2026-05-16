/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):    Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	 as32x601_clint.h
 * Author:           ANSILIC APPLICATION GROUP
 * Version:          V1.0
 * Date:             2024/08/19
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/08/19     1.0     build this module
 * Description:     This file contains all the functions prototypes for the CLINT firmware library.
 */

#ifndef AS32X601_CLINT_H
#define AS32X601_CLINT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601_clint_regs.h"

/* 
 * @brief Timer value parameter definition  
 */ 
struct TimerValue
{
    uint32_t TimerValHigh;
    uint32_t TimerValLow;
};

/*
 * @brief Function declaration
 */
void CLINT_SoftIntCmd(FunctionalState NewState);
void CLINT_SetTimerComparator(uint32_t TimerCompareLow, uint32_t TimerCompareHigh);
struct TimerValue CLINT_ReadTimerVal(void);

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
