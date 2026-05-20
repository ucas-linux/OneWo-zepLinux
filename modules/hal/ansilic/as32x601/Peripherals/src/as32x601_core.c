/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_core.c
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2024/08/20
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/08/20     1.0     build this module
 * Description:     This file provides firmware functionality to manage core configuration information.
 */

#include <stdint.h>
#include "as32x601_core.h"
#include "as32x601_smu.h"
#include "as32x601_flash.h"

/*
 * Function:        SystemInit
 * Description:     Configuration the system.
 * Param:           None.
 * Return:          None.
 */
void SystemInit(void)
{
    SMU_PLLInitTypeDef SMU_PLLInitStruct;
    SMU_ClockInitTypeDef SMU_ClockInitStruct;

    /* Bus clock configure */
    SYS_BusClkInit();

    /* Dual core lockstep enable */
#if DCLS == 1
    SMU_DCLSCmd(ENABLE);
#endif

    /* PLL clock configure */
    SMU_PLLStructInit(&SMU_PLLInitStruct);
    SMU_PLLInitStruct.OscillatorType = SMU_OSCILLATORTYPE_FIRC; // Select FIRC as the PLL clock
    SMU_PLLInitStruct.FIRCOscState = ENABLE;                    // Enable FIRC
    SMU_PLLInitStruct.FIRCCalibrationValue = 0x00;              // FIRC oscillator frequency adjustment
    SMU_PLLInitStruct.PLLConfig.PLLState = ENABLE;              // Enable PLL
    SMU_PLLInitStruct.PLLConfig.PLLSource = SMU_PLLCLK_FIRC;    // FIRC serves as the PLL clock
    SMU_PLLInitStruct.PLLConfig.PLLDivR = 0x08;                 // For CAN Freq 40MHz
    SMU_PLLInitStruct.PLLConfig.PLLDivQ = 0x02;                 // For System Clock 160MHz
    SMU_PLLInitStruct.PLLConfig.PLLDivN = 0x10;                 // The clock after input frequency division is 1MHz
    SMU_PLLInitStruct.PLLConfig.PLLDivF = 0x140;                // The clock output by VCO is 320MHz
    SMU_PLLInit(&SMU_PLLInitStruct);

    EFLASH_CLK_ENABLE();                // Enable EFlash clock
    EFLASH_SET();                       // Release EFlash reset

    /* EFlash unlock */
    FLASH_UnlockCtrl();

    /* Configure EFlash clock frequency */
    FLASH_SetCLKFreq(160);

    /* Bus clock configure */
    SMU_ClockStructInit(&SMU_ClockInitStruct);
    SMU_ClockInitStruct.SYSCLKSelect = SMU_SYSCLK_PLL;          // Select PLL as the SYSCLK
    SMU_ClockInitStruct.AXI4Bus3CLKDiv = AXI4Bus3CLKDiv1;       // AXI4Bus3 Clock Division 1
    SMU_ClockInitStruct.APBBus0CLKDiv = APBBus0CLKDiv1;         // APBBus0 Clock Division 1
    SMU_ClockInitStruct.APBBus1CLKDiv = APBBus1CLKDiv1;         // APBBus1 Clock Division 1
    SMU_ClockInitStruct.CANX2CLKDiv = CANX2CLKDiv1;             // CANX2 Clock Division 1
    SMU_ClockInit(&SMU_ClockInitStruct);

    EFLASH_CLK_UPDATE_ENABLE();         // Enable EFlash clock update
    EFLASH_CLK_UPDATE_DISABLE();        // Disable EFlash clock update
    
    CLINT_CLK_ENABLE();                 // Enable CLINT clock
    CLINT_SET();                        // Set CLINT
}

/*
 * Function:        SYS_BusClkInit
 * Description:     Bus clock initialization.
 * Param:           None.
 * Return:          None.
 */
void SYS_BusClkInit(void)
{
#ifdef AX32x601_MCU
    AXILITEBUS1_ALL_RESET();
    AXILITEBUS2_ALL_RESET();
    APBBUS0_ALL_RESET();
    APBBUS1_ALL_RESET();

    APBBUS0_CLK_ENABLE();               // Enable APBBUS0 clock
    APBBUS0_SET();                      // Release APBBUS0 reset
    
    APBBUS1_CLK_ENABLE();               // Enable APBBUS1 clock
    APBBUS1_SET();                      // Release APBBUS1 reset

    AXIBUS3_CLK_ENABLE();               // Enable AXIBUS3 clock
    AXIBUS3_SET();                      // Release AXIBUS3 reset

    AXI4TOAPB0_CLK_ENABLE();            // Enable AXI4TOAPB0 clock
    AXI4TOAPB0_SET();                   // Release AXI4TOAPB0 reset
    
    AXI4TOAPB1_CLK_ENABLE();            // Enable AXI4TOAPB1 clock
    AXI4TOAPB1_SET();                   // Release AXI4TOAPB1 reset
    
    AXILITEBUS1_CLK_ENABLE();           // Enable AXILITEBUS1 clock
    AXILITEBUS1_SET();                  // Release AXILITEBUS1 reset
    
    AXILITEBUS2_CLK_ENABLE();           // Enable AXILITEBUS2 clock
    AXILITEBUS2_SET();                  // Release AXILITEBUS2 reset
#endif
}

/*
 * Function:        ClearCache
 * Description:     Clear cache.
 * Note:            Use this function with caution.
 * Param:           None.
 * Return:          None.
 */
void ClearCache(void)
{
    clear_cache();
}

/*
 * Function:        InstAddrMisalign_Handler
 * Description:     Instruction address misaligned.
 * Param:           None.
 * Return:          None.
 */
void InstAddrMisalign_Handler(void)
{
	while(1);
}

/*
 * Function:        InstAccessFault_Handler
 * Description:     Instruction access fault.
 * Param:           None.
 * Return:          None.
 */
void InstAccessFault_Handler(void)
{
	while(1);
}

/*
 * Function:        IllegalInst_Handler
 * Description:     Illegal instruction.
 * Param:           None.
 * Return:          None.
 */
void IllegalInst_Handler(void)
{
	while(1);
}

/*
 * Function:        Breakpoint_Handler
 * Description:     Breakpoint.
 * Param:           None.
 * Return:          None.
 */
void Breakpoint_Handler(void)
{
	while(1);
}

/*
 * Function:        LoadAddrMisalign_Handler
 * Description:     Load address misaligned.
 * Param:           None.
 * Return:          None.
 */
void LoadAddrMisalign_Handler(void)
{
	while(1);
}

/*
 * Function:        LoadAccessFault_Handler
 * Description:     Load access fault.
 * Param:           None.
 * Return:          None.
 */
void LoadAccessFault_Handler(void)
{
	while(1);
}

/*
 * Function:        StoreAMOAddrMisalign_Handler
 * Description:     Store/AMO address misaligned.
 * Param:           None.
 * Return:          None.
 */
void StoreAMOAddrMisalign_Handler(void)
{
	while(1);
}

/*
 * Function:        StoreAMOAccessFault_Handler
 * Description:     Store/AMO access fault.
 * Param:           None.
 * Return:          None.
 */
void StoreAMOAccessFault_Handler(void)
{
	while(1);
}

/*
 * Function:        ECall_Handler
 * Description:     Environment call from M-mode.
 * Param:           None.
 * Return:          None.
 */
void ECall_Handler(void)
{
	while(1);
}

/*
 * Function:        InstPageFault_Handler
 * Description:     Instruction page fault.
 * Param:           None.
 * Return:          None.
 */
void InstPageFault_Handler(void)
{
	while(1);
}

/*
 * Function:        LoadPageFault_Handler
 * Description:     Load page fault.
 * Param:           None.
 * Return:          None.
 */
void LoadPageFault_Handler(void)
{
	while(1);
}

/*
 * Function:        StoreAMOPageFalut_Handler
 * Description:     Store/AMO page fault.
 * Param:           None.
 * Return:          None.
 */
void StoreAMOPageFalut_Handler(void)
{
	while(1);
}

/*****END OF FILE*****/
