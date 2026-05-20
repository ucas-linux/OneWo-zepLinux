/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_clint.c
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2024/08/19
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/08/19     1.0     build this module
 * Description:     This file provides firmware functions to manage the functionalities of the CLINT peripheral.
 */

#include "as32x601_clint.h"

/*
 * Function:        CLINT_SoftIntCmd
 * Description:     Enables or disables the soft interrupt.
 * Param:           NewState: new state of the CLINT peripheral.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void CLINT_SoftIntCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_EnState_STATE(NewState));

    if (NewState != DISABLE) 
    {
        CLINT->SOFTICR |= ((uint32_t)CLINT_SOFTICR_SOFT);
    } 
    else
    {
        CLINT->SOFTICR &= ~((uint32_t)CLINT_SOFTICR_SOFT);
    }
}

/*
 * Function:        CLINT_SetTimerComparator
 * Description:     Set the timer comparator.
 * Param:           TimerCompareLow: Low 32-bit value of timer comparator.
 *                  TimerCompareHigh: High 32-bit value of timer comparator.
 * Return:          None.
 */
void CLINT_SetTimerComparator(uint32_t TimerCompareLow, uint32_t TimerCompareHigh)
{
    CLINT->CMPLR = TimerCompareLow;
    CLINT->CMPHR = TimerCompareHigh;
}

/*
 * Function:        CLINT_ReadTimerVal
 * Description:     Read the timer.
 * Param:           None.
 * Return:          The value of timer.
 */
struct TimerValue CLINT_ReadTimerVal(void)
{
    struct TimerValue timerval;

    timerval.TimerValHigh = (uint32_t)(CLINT->RDTIMEHR);
    timerval.TimerValLow = (uint32_t)(CLINT->RDTIMELR);

    return timerval;
}

/*****END OF FILE*****/
