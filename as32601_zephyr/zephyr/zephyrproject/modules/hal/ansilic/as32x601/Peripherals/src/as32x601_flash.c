/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_flash.c
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2024/08/14
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/08/14     1.0     build this module
 *     ANSILIC APPLICATION GROUP  2025/09/01     1.0     build this module
 * Description:     This file provides firmware functions to manage the functionalities of the FLASH peripheral.
 */

#include "as32x601_flash.h"

/*
 * @defgroup Flash private definition
 */
#define FLASH_CMD_TIMEOUT   (2UL) 	/* FLASH command timeout */
#define FLASH_ABORT_WAITCNT (0x40UL)

/* Flash info area base & end addresss */
#define FLASH_PF_INFO_BASE_ADDR         (0x01200000UL)
#define FLASH_WRITE_PROTECTION_ADDR     (FLASH_PF_INFO_BASE_ADDR)
#define FLASH_BACK_DOOR_KEY_ADDR        (FLASH_PF_INFO_BASE_ADDR + 0x1000UL)
#define FLASH_READ_PROTECTION_ADDR      (FLASH_PF_INFO_BASE_ADDR + 0x1000UL)

/* Private function prototypes -----------------------------------------------*/
static void FLASH_SetCommand(FLASHCmd_TypeDef Cmd);
static void FLASH_SetAddress(uint32_t Address);
static void FLASH_SetDataLength(uint32_t Length);
static void FLASH_SetLowData(uint32_t Data);
static void FLASH_SetHighData(uint32_t Data);
static FLASHStatus_TypeDef FLASH_StartCmd(void);

/*
 * Function:        FLASH_GetFlagStatus
 * Description:     Checks whether the specified flash flag is set or not.
 * Param:           FLASH_FLAG: specifies the flag to check.
 *                   This parameter can be one of the following values:
 *                      FLASH_FLAG_LOCK: Flash lock status flag
 *                      FLASH_FLAG_BUSY: Flash busy flag 
 *                      FLASH_FLAG_FINISH: Flash command finish flag
 *                      FLASH_FLAG_OPERR: Flash operation error flag
 *                      FLASH_FLAG_ECC1ERR: Flash ECC 1-bit error flag
 *                      FLASH_FLAG_ECC2ERR: Flash ECC 2-bit error flag
 *                      FLASH_FLAG_WPERR: Flash write protection error flag
 *                      FLASH_FLAG_SECKEYERR: Flash security key verification error flag
 *                      FLASH_FLAG_CLKSWRDY: Flash clock switching status flag
 * Return:          The state of FLASH_FLAG (SET or RESET).
 */
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_FLASH_FLAG(FLASH_FLAG));

    if ((EFLASH->SR & FLASH_FLAG) != (uint32_t)RESET) 
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
 * Function:        FLASH_ClearFlagStatus
 * Description:     Clears the the specified flash flag.
 * Param:           FLASH_FLAG: specifies the flag to clear.
 *                   This parameter can be one of the following values:
 *                      FLASH_FLAG_FINISH: Flash command finish flag
 *                      FLASH_FLAG_OPERR: Flash operation error flag
 *                      FLASH_FLAG_ECC1ERR: Flash a single-bit ECC error flag
 *                      FLASH_FLAG_ECC2ERR: Flash a multi-bit ECC error flag
 *                      FLASH_FLAG_WPERR: Flash write protection error flag
 * Return:          None.
 */
void FLASH_ClearFlagStatus(uint32_t FLASH_FLAG)
{
    /* Check the parameters */
    assert_param(IS_FLASH_CLEAR(FLASH_FLAG));

    EFLASH->SR = FLASH_FLAG;
}

/*
 * Function:        FLASH_ITConfig
 * Description:     Enables or disables the flash interrupts.
 * Param:           FLASH_IT: specifies the flash interrupt source to be enabled or disabled.
 *                   This parameter can be one of the following values:
 *                      FLASH_IT_FINIE: Flash command finish interrupt enable
 *                      FLASH_IT_OPEIE: Flash operation error interrupt enable
 *                      FLASH_IT_ECC2EIE: Flash ECC 2 bit error interrupt enable
 *                  NewState: new state of the specified EFLASH peripheral.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FLASH_IT(FLASH_IT));
    assert_param(IS_EnState_STATE(NewState));

    if (NewState != DISABLE) 
    {
        /* Enable the selected flash interrupt */
        EFLASH->CR |= FLASH_IT;
    } 
    else 
    {
        /* Disable the selected flash interrupt */
        EFLASH->CR &= ~((uint32_t)FLASH_IT);
    }
}

/*
 * Function:        FLASH_ClearITPendingBit
 * Description:     Clears the flash interrupt pending bits.
 * Param:           FLASH_IT: specifies the flash interrupt source to be enabled or disabled.
 *                   This parameter can be one of the following values:
 *                      FLASH_CLEAR_IT_FINISH: Flash command finish clear flag
 *                      FLASH_CLEAR_IT_OPERR: Flash operation error clear flag
 *                      FLASH_CLEAR_IT_ECC2ERR: Flash ECC 2 bit error clear flag
 * Return:          None.
 */
void FLASH_ClearITPendingBit(uint32_t FLASH_IT)
{
    /* Check the parameters */
    assert_param(IS_FLASH_CLEAR_IT(FLASH_IT));

    EFLASH->SR |= (uint32_t)FLASH_IT;
}

/*
 * Function:        FLASH_UnlockCtrl
 * Description:     Unlock flash register.
 * Param:           None.
 * Return:          None.
 */
void FLASH_UnlockCtrl(void)
{
    uint32_t statlockbit = FLASH_GetFlagStatus(FLASH_FLAG_LOCK);

    if (statlockbit == SET) 
    {
        EFLASH->UNLKR = FLASH_UNLOCK_KEY1;
        EFLASH->UNLKR = FLASH_UNLOCK_KEY2;
    }
    
    while (FLASH_GetFlagStatus(FLASH_FLAG_LOCK) == SET);
    while (FLASH_GetFlagStatus(FLASH_FLAG_BUSY) == SET);
}

/*
 * Function:        FLASH_LockCtrl
 * Description:     Lock flash register.
 * Param:           None.
 * Return:          None.
 */
void FLASH_LockCtrl(void)
{
    EFLASH->UNLKR = ~((uint32_t)FLASH_UNLOCK_KEY1);
    EFLASH->UNLKR = ~((uint32_t)FLASH_UNLOCK_KEY2);
}

/*
 * Function:        FLASH_SetCLKFreq
 * Description:     Set the FLASH clock frequency.
 * Param:           CLK_Frequence: FLASH clock frequency to be set，this parameter should be the AXIBus0's clock frequency.
 * Return:          None.
 */ 
void FLASH_SetCLKFreq(uint16_t CLK_Frequence)
{
   uint32_t tmpreg = 0x00;

    /* Waiting for idle state */
    while(FLASH_GetFlagStatus(FLASH_FLAG_BUSY) == SET);

    tmpreg = EFLASH->CR;
    
    if((tmpreg >> 16) == CLK_Frequence)
      return;
    tmpreg &= ~((uint32_t)EFLASH_CR_CLKFRQ);
    tmpreg |= ((uint32_t)(CLK_Frequence << EFLASH_CR_CLKFRQ_Pos));
    EFLASH->CR = tmpreg;

    /* Waiting for the flash controller to complete the preparation work */
    while(FLASH_GetFlagStatus(FLASH_FLAG_CLKSWRDY) == RESET);
}

/*
 * Function:        FLASH_EraseSector
 * Description:     Erase the specified flash sector.
 * Param:           Addr: Sector address to erase.
 *                  Size: The size of the sector to be erased
 * Return:          Operation status.
 *                   Can be one of the following values:
 *                      FLASH_SUCCESS: operation was successful
 *                      FLASH_ERROR: operation was failure
 *                      FLASH_BUSY: operation was busy
 */
FLASHStatus_TypeDef FLASH_EraseSector(uint32_t Addr, uint32_t Size)
{
    FLASHStatus_TypeDef ret = FLASH_SUCCESS;
    uint32_t sectorsize = 0x00;
    uint32_t tempsize = Size;

    /* Check the parameters */
    assert_param((Addr % 0x08) == 0);
    assert_param(Size > 0);

    /* Check if command is completed by BUSY */
    if(FLASH_GetFlagStatus(FLASH_FLAG_BUSY) == SET)
    {
        return FLASH_BUSY;
    }

    /* Determine the flash object of the operation */
    if ((Addr >= P_FLASH_Main_BASE) && ((Addr + tempsize) <= (P_FLASH_Main_BASE + PFLASH_MAIN_SIZE))) 
    {
        sectorsize = (uint32_t)PFLASH_SECTOR_SIZE;
    }
    else if ((Addr >= D_FLASH_Main_BASE) && ((Addr + tempsize) <= (D_FLASH_Main_BASE + DFLASH_MAIN_SIZE)))
    {
        sectorsize = (uint32_t)DFLASH_SECTOR_SIZE;
    }
    else
    {
        ret = FLASH_ERROR;
        tempsize = 0U;
        sectorsize = 0U;
    }

    while ((tempsize > 0U) && (FLASH_SUCCESS == ret)) 
    {
        /* Set flash sector erase command */
        FLASH_SetCommand(FLASH_Cmd_SectorErase);
        FLASH_SetAddress(Addr);

        ret = FLASH_StartCmd();

        /* Start command */
        if (ret == FLASH_ERROR) 
        {
            return FLASH_ERROR;
        }
        else if (ret == FLASH_BUSY)
        {
            return FLASH_BUSY;
        }
        else
        {
            /* Nothing to do */
        }

        /* Update size and address */
        tempsize -= sectorsize;
        Addr += sectorsize;
    }

    return ret;
}

/*
 * Function:        FLASH_EraseBlock
 * Description:     Erase specified flash blocks.
 * Param:           Addr: Block address to erase.
 * Return:          Operation status.
 *                   Can be one of the following values:
 *                      FLASH_SUCCESS: operation was successful
 *                      FLASH_ERROR: operation was failure
 *                      FLASH_BUSY: operation was busy
 */
FLASHStatus_TypeDef FLASH_EraseBlock(uint32_t Addr)
{
    FLASHStatus_TypeDef ret = FLASH_SUCCESS;

    /* Check the parameters */
    assert_param((Addr % 0x08) == 0);

    /* Check if command is completed by BUSY */
    if(FLASH_GetFlagStatus(FLASH_FLAG_BUSY) == SET)
    {
        return FLASH_BUSY;
    }

    /* Determine the flash object of the operation */
    if ((Addr >= P_FLASH_Main_BASE) && ((Addr) <= (P_FLASH_Main_BASE + PFLASH_MAIN_SIZE))) 
    {
        ret = FLASH_SUCCESS;
    }
    else if ((Addr >= D_FLASH_Main_BASE) && ((Addr) <= (D_FLASH_Main_BASE + DFLASH_MAIN_SIZE)))
    {
        ret = FLASH_SUCCESS;
    }
    else
    {
        ret = FLASH_ERROR;
    }

    if (FLASH_SUCCESS == ret) 
    {
        /* Set flash sector erase command */
        FLASH_SetCommand(FLASH_Cmd_BlockErase);
        FLASH_SetAddress(Addr);

        ret = FLASH_StartCmd();

        /* Start command */
        if (ret == FLASH_ERROR) 
        {
            return FLASH_ERROR;
        }
        else if (ret == FLASH_BUSY)
        {
            return FLASH_BUSY;
        }
        else
        {
            /* Nothing to do */
        }
    }

    return ret;
}

/*
 * Function:        FLASH_EraseWriteProtection
 * Description:     Erase write protection information flash sector.
 * Param:           None.
 * Return:          Operation status.
 *                   Can be one of the following values:
 *                      FLASH_SUCCESS: operation was successful
 *                      FLASH_ERROR: operation was failure
 *                      FLASH_BUSY: operation was busy
 */
FLASHStatus_TypeDef FLASH_EraseWriteProtection(void)
{
    FLASHStatus_TypeDef ret = FLASH_SUCCESS;

    FLASH_ClearFlagStatus(FLASH_CLEAR_FINISH);

    /* Check if command is completed by BUSY */
    if(FLASH_GetFlagStatus(FLASH_FLAG_BUSY) == SET)
    {
        return FLASH_BUSY;
    }
    
    /* Set flash bulk erase command */
    FLASH_SetCommand(FLASH_Cmd_WPInfoSectorErase);
    FLASH_SetAddress(FLASH_WRITE_PROTECTION_ADDR);

    /* Start command */
    ret = FLASH_StartCmd();

    return ret;
}

/*
 * Function:        FLASH_EraseAllBlock
 * Description:     Erase specified flash all blocks.
 * Param:           None.
 * Return:          Operation status.
 *                   Can be one of the following values:
 *                      FLASH_SUCCESS: operation was successful
 *                      FLASH_ERROR: operation was failure
 *                      FLASH_BUSY: operation was busy
 */
FLASHStatus_TypeDef FLASH_EraseAllBlock(void)
{
    FLASHStatus_TypeDef ret = FLASH_SUCCESS;

    FLASH_ClearFlagStatus(FLASH_CLEAR_FINISH);

    /* Check if command is completed by BUSY */
    if(FLASH_GetFlagStatus(FLASH_FLAG_BUSY) == SET)
    {
        return FLASH_BUSY;
    }
    
    /* Set flash bulk erase command */
    FLASH_SetCommand(FLASH_Cmd_AllBlockErase);

    /* Start command */
    ret = FLASH_StartCmd();

    return ret;
}

/*
 * Function:        FLASH_ProgramRom
 * Description:     Program specified flash.
 * Param:           Addr: The flash address to program.
 *                  Size: The data size to program(The unit is B).
 *                  Data: The data to program.
 * Return:          Operation status.
 *                   Can be one of the following values:
 *                      FLASH_SUCCESS: operation was successful
 *                      FLASH_ERROR: operation was failure
 *                      FLASH_BUSY: operation was busy
 */
FLASHStatus_TypeDef FLASH_ProgramRom(uint32_t Addr, uint32_t Size, const uint32_t *Data)
{
    FLASHStatus_TypeDef ret = FLASH_SUCCESS;
    uint32_t addrcount = 0x00, sizetemp = 0x00;
    uint32_t romsize = 0x00;

    /* Check the parameters */
    assert_param((0 < Size) && ((Size % EFLASH_WRITE_UNIT_SIZE) == 0));
    assert_param(NULL != Data);

    /* Check if command is completed by BUSY */
    if(FLASH_GetFlagStatus(FLASH_FLAG_BUSY) == SET)
    {
        return FLASH_BUSY;
    }

	/* Check address */
    if ((Addr >= P_FLASH_Main_BASE) && ((Addr + Size) <= (P_FLASH_Main_BASE + PFLASH_MAIN_SIZE))) 
    {
        romsize = PFLASH_ROM_SIZE;
    } 
    else if ((Addr >= D_FLASH_Main_BASE) && ((Addr + Size) <= (D_FLASH_Main_BASE + DFLASH_MAIN_SIZE)))
    {
        romsize = DFLASH_ROM_SIZE;
    }
	else
	{
		ret = FLASH_ERROR;
	}

    /* Judge size and feed write */
    addrcount = Size / romsize;
    if(0 != (Size % romsize))
    {
        addrcount += 1;
    }
    else
    {
        /* Nothing to do */
    }

	while ((FLASH_SUCCESS == ret) && (addrcount > 0U)) 
	{
		if (Size >= romsize) 
		{
			sizetemp  = romsize;
		} 
		else 
		{
			sizetemp  = Size;
		}
		/* Set address and data length */
		FLASH_SetAddress(Addr);
		FLASH_SetDataLength(sizetemp / EFLASH_WRITE_UNIT_SIZE);

		/* Set rom program command */
		FLASH_SetCommand(FLASH_Cmd_Program);

		while ((sizetemp > 0U) && (FLASH_SUCCESS == ret)) 
		{
            /* Write 64 bit data */
            FLASH_SetLowData(Data[0]);
            FLASH_SetHighData(Data[1]);

            /* Update data adddress*/
            Data += 2;
			sizetemp -= EFLASH_WRITE_UNIT_SIZE;
		}

        ret = FLASH_StartCmd();

        /* Start command */
        if (ret == FLASH_ERROR) 
        {
            return FLASH_ERROR;
        }
        else if (ret == FLASH_BUSY)
        {
            return FLASH_BUSY;
        }
        else
        {
            /* Nothing to do */
        }

		/* Update adddress*/
		Addr += romsize;
		addrcount--;
		Size -= romsize;
	}

    return ret;
}

/*
 * Function:        FLASH_ProgramInfo
 * Description:     Program information area flash.
 * Param:           Addr: the flash address to program.
 *                  Size: the data size to program(The unit is B).
 *                  Data: the data to program.
 * Return:          Operation status.
 *                   Can be one of the following values:
 *                      FLASH_SUCCESS: operation was successful
 *                      FLASH_ERROR: operation was failure
 *                      FLASH_BUSY: operation was busy
 */
FLASHStatus_TypeDef FLASH_ProgramInfo(uint32_t Addr, uint32_t Size, const uint32_t *Data)
{
    FLASHStatus_TypeDef ret = FLASH_SUCCESS;
    uint32_t addrcount = 0x00, sizetemp = 0x00;
	uint32_t romsize = 0x00;

    /* Check the parameters */
    assert_param((0 < Size) && ((Size % EFLASH_WRITE_UNIT_SIZE) == 0));
    assert_param(NULL != Data);

    /* Check if command is completed by BUSY */
    if(FLASH_GetFlagStatus(FLASH_FLAG_BUSY) == SET)
    {
        return FLASH_BUSY;
    }

	/* Check address */
    if ((Addr >= P_FLASH_Info_BASE) && ((Addr + Size) <= (P_FLASH_Info_BASE + PFLASH_INFO_SIZE))) 
    {
        romsize = PFLASH_ROM_SIZE;
    } 
    else if ((Addr >= D_FLASH_Info_BASE) && ((Addr + Size) <= (D_FLASH_Info_BASE + DFLASH_INFO_SIZE)))
    {
        romsize = DFLASH_ROM_SIZE;
    }
	else
	{
		ret = FLASH_ERROR;
	}

    /* Judge size and feed write */
    addrcount = Size / romsize;
    if(0 != (Size % romsize))
    {
        addrcount += 1;
    }
    else
    {
        /* Nothing to do */
    }

	while ((FLASH_SUCCESS == ret) && (addrcount > 0U)) 
	{
		if (Size >= romsize) 
		{
			sizetemp  = romsize;
		} 
		else 
		{
			sizetemp  = Size;
		}
		/* Set address and data length */
		FLASH_SetAddress(Addr);
		FLASH_SetDataLength(sizetemp / EFLASH_WRITE_UNIT_SIZE);

		/* Set rom program command */
		FLASH_SetCommand(FLASH_Cmd_Program);

		while ((sizetemp > 0U) && (FLASH_SUCCESS == ret)) 
		{
            /* Write 64 bit data */
            FLASH_SetLowData(Data[0]);
            FLASH_SetHighData(Data[1]);

            /* Update data adddress*/
            Data += 2;
			sizetemp -= EFLASH_WRITE_UNIT_SIZE;
		}

        ret = FLASH_StartCmd();

        /* Start command */
        if (ret == FLASH_ERROR) 
        {
            return FLASH_ERROR;
        }
        else if (ret == FLASH_BUSY)
        {
            return FLASH_BUSY;
        }
        else
        {
            /* Nothing to do */
        }

		/* Update adddress*/
		Addr += romsize;
		addrcount--;
		Size -= romsize;
	}

    return ret;
}

/*
 * Function:        FLASH_Read
 * Description:     Read data at the specified flash.
 * Param:           Addr: the flash address to read.
 *                  Size: the data size to read.
 *                  Data: the data of read.
 * Return:          Operation status.
 *                   Can be one of the following values:
 *                      FLASH_SUCCESS: operation was successful
 *                      FLASH_ERROR: operation was failure
 *                      FLASH_BUSY: operation was busy
 */
FLASHStatus_TypeDef FLASH_Read(uint32_t Addr, uint32_t Size, uint32_t *Data)
{
    FLASHStatus_TypeDef ret = FLASH_SUCCESS;
    uint32_t i = 0x00;

    /* Check the parameters */
    assert_param((0 < Size) && ((Size % EFLASH_WRITE_UNIT_SIZE) == 0));
    /* Check address */
    if ((Addr < P_FLASH_Main_BASE || Addr > P_FLASH_Info_BASE + PFLASH_INFO_SIZE) && (Addr < D_FLASH_Main_BASE || Addr > D_FLASH_Info_BASE + DFLASH_INFO_SIZE)) 
    {
        ret = FLASH_ERROR;
    }
    else
    {
        /* Nothing to do */
    }

    if (FLASH_SUCCESS == ret) 
    {
        for (i = 0; i < (Size/4); i++) 
        {
            if (SET == FLASH_GetFlagStatus(FLASH_FLAG_ECC1ERR | FLASH_FLAG_ECC2ERR)) 
            {
                return FLASH_ERROR;
            } 
            else 
            {
                Data[i] = *(uint32_t *)(Addr + (i * 4));
            }
        }
    }
    else
    {
        /* Nothing to do */
    }

    return ret;
}

/*
 * Function:        FLASH_GetPFlashWriteProtStatus
 * Description:     Checks whether the specified flash write protection area status.
 * Param:           ProtectArea: write protection area to check.
 *                   This parameter can be one of the following values:
 *                      FLASH_WP_AREA1: write protection area 1
 *                      FLASH_WP_AREA2: write protection area 2
 *                      FLASH_WP_AREA3: write protection area 3
 *                      FLASH_WP_AREA4: write protection area 4
 *                      FLASH_WP_AREA5: write protection area 5
 *                      FLASH_WP_AREA6: write protection area 6
 *                      FLASH_WP_AREA7: write protection area 7
 *                      FLASH_WP_AREA8: write protection area 8
 *                      FLASH_WP_AREA9: write protection area 9
 *                      FLASH_WP_AREA10: write protection area 10
 *                      FLASH_WP_AREA11: write protection area 11
 *                      FLASH_WP_AREA12: write protection area 12
 *                      FLASH_WP_AREA13: write protection area 13
 *                      FLASH_WP_AREA14: write protection area 14
 *                      FLASH_WP_AREA15: write protection area 15
 *                      FLASH_WP_AREA16: write protection area 16
 *                      FLASH_WP_AREA17: write protection area 17
 *                      FLASH_WP_AREA18: write protection area 18
 *                      FLASH_WP_AREA19: write protection area 19
 *                      FLASH_WP_AREA20: write protection area 20
 *                      FLASH_WP_AREA21: write protection area 21
 *                      FLASH_WP_AREA22: write protection area 22
 *                      FLASH_WP_AREA23: write protection area 23
 *                      FLASH_WP_AREA24: write protection area 24
 *                      FLASH_WP_AREA25: write protection area 25
 *                      FLASH_WP_AREA26: write protection area 26
 *                      FLASH_WP_AREA27: write protection area 27
 *                      FLASH_WP_AREA28: write protection area 28
 *                      FLASH_WP_AREA29: write protection area 29
 *                      FLASH_WP_AREA30: write protection area 30
 *                      FLASH_WP_AREA31: write protection area 31
 *                      FLASH_WP_AREA32: write protection area 32
 * Return:          The state of write protect area (SET or RESET).
 */
FlagStatus FLASH_GetPFlashWriteProtStatus(uint32_t ProtectArea)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_PFLASH_WP_AREAx(ProtectArea));

    if ((EFLASH->PWPR & (uint32_t)ProtectArea) != (uint32_t)RESET) 
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
 * Function:        FLASH_SetPFlashWriteProtection
 * Description:     Set the specified flash write protection area.
 * Param:           ProtectArea: write protection area.
 *                   This parameter can be one of the following values:
 *                      FLASH_WP_AREA1: write protection area 1
 *                      FLASH_WP_AREA2: write protection area 2
 *                      FLASH_WP_AREA3: write protection area 3
 *                      FLASH_WP_AREA4: write protection area 4
 *                      FLASH_WP_AREA5: write protection area 5
 *                      FLASH_WP_AREA6: write protection area 6
 *                      FLASH_WP_AREA7: write protection area 7
 *                      FLASH_WP_AREA8: write protection area 8
 *                      FLASH_WP_AREA9: write protection area 9
 *                      FLASH_WP_AREA10: write protection area 10
 *                      FLASH_WP_AREA11: write protection area 11
 *                      FLASH_WP_AREA12: write protection area 12
 *                      FLASH_WP_AREA13: write protection area 13
 *                      FLASH_WP_AREA14: write protection area 14
 *                      FLASH_WP_AREA15: write protection area 15
 *                      FLASH_WP_AREA16: write protection area 16
 *                      FLASH_WP_AREA17: write protection area 17
 *                      FLASH_WP_AREA18: write protection area 18
 *                      FLASH_WP_AREA19: write protection area 19
 *                      FLASH_WP_AREA20: write protection area 20
 *                      FLASH_WP_AREA21: write protection area 21
 *                      FLASH_WP_AREA22: write protection area 22
 *                      FLASH_WP_AREA23: write protection area 23
 *                      FLASH_WP_AREA24: write protection area 24
 *                      FLASH_WP_AREA25: write protection area 25
 *                      FLASH_WP_AREA26: write protection area 26
 *                      FLASH_WP_AREA27: write protection area 27
 *                      FLASH_WP_AREA28: write protection area 28
 *                      FLASH_WP_AREA29: write protection area 29
 *                      FLASH_WP_AREA30: write protection area 30
 *                      FLASH_WP_AREA31: write protection area 31
 *                      FLASH_WP_AREA32: write protection area 32
 * Return:          Operation status.
 *                   Can be one of the following values:
 *                      FLASH_SUCCESS: operation was successful
 *                      FLASH_ERROR: operation was failure
 *                      FLASH_BUSY: operation was busy
 */
FLASHStatus_TypeDef FLASH_SetPFlashWriteProtection(uint32_t ProtectArea)
{
    FLASHStatus_TypeDef ret = FLASH_SUCCESS;

    /* Check the parameters */
    assert_param(IS_PFLASH_WP_AREAx(ProtectArea));

    /* Write to WP register */
    EFLASH->PWPR &= ~(uint32_t)ProtectArea;

    /* Read the value of WP registers */
    if (FLASH_GetPFlashWriteProtStatus(ProtectArea) != RESET) 
    {
        ret = FLASH_ERROR;
    }
    else
    {
        /* Nothing to do */
    }

    return ret;
}

/*
 * Function:        FLASH_ResetPFlashWriteProtection
 * Description:     Reset the specified flash write protection area.
 * Param:           ProtectArea: write protection area.
 *                   This parameter can be one of the following values:
 *                      FLASH_WP_AREA1: write protection area 1
 *                      FLASH_WP_AREA2: write protection area 2
 *                      FLASH_WP_AREA3: write protection area 3
 *                      FLASH_WP_AREA4: write protection area 4
 *                      FLASH_WP_AREA5: write protection area 5
 *                      FLASH_WP_AREA6: write protection area 6
 *                      FLASH_WP_AREA7: write protection area 7
 *                      FLASH_WP_AREA8: write protection area 8
 *                      FLASH_WP_AREA9: write protection area 9
 *                      FLASH_WP_AREA10: write protection area 10
 *                      FLASH_WP_AREA11: write protection area 11
 *                      FLASH_WP_AREA12: write protection area 12
 *                      FLASH_WP_AREA13: write protection area 13
 *                      FLASH_WP_AREA14: write protection area 14
 *                      FLASH_WP_AREA15: write protection area 15
 *                      FLASH_WP_AREA16: write protection area 16
 *                      FLASH_WP_AREA17: write protection area 17
 *                      FLASH_WP_AREA18: write protection area 18
 *                      FLASH_WP_AREA19: write protection area 19
 *                      FLASH_WP_AREA20: write protection area 20
 *                      FLASH_WP_AREA21: write protection area 21
 *                      FLASH_WP_AREA22: write protection area 22
 *                      FLASH_WP_AREA23: write protection area 23
 *                      FLASH_WP_AREA24: write protection area 24
 *                      FLASH_WP_AREA25: write protection area 25
 *                      FLASH_WP_AREA26: write protection area 26
 *                      FLASH_WP_AREA27: write protection area 27
 *                      FLASH_WP_AREA28: write protection area 28
 *                      FLASH_WP_AREA29: write protection area 29
 *                      FLASH_WP_AREA30: write protection area 30
 *                      FLASH_WP_AREA31: write protection area 31
 *                      FLASH_WP_AREA32: write protection area 32
 * Return:          Operation status.
 *                   Can be one of the following values:
 *                      FLASH_SUCCESS: operation was successful
 *                      FLASH_ERROR: operation was failure
 *                      FLASH_BUSY: operation was busy
 */
FLASHStatus_TypeDef FLASH_ResetPFlashWriteProtection(uint32_t ProtectArea)
{
    FLASHStatus_TypeDef ret = FLASH_SUCCESS;

    /* Check the parameters */
    assert_param(IS_PFLASH_WP_AREAx(ProtectArea));

    /* Write to WP register */
    EFLASH->PWPR |= (uint32_t)ProtectArea;

    /* Read the value of WP registers */
    if (FLASH_GetPFlashWriteProtStatus(ProtectArea) != SET) 
    {
        ret = FLASH_ERROR;
    }
    else
    {
        /* Nothing to do */
    }

    return ret;
}


/*
 * Function:        FLASH_GetDFlashWriteProtStatus
 * Description:     Checks whether the specified flash write protection area status.
 * Param:           ProtectArea: write protection area to check.
 *                   This parameter can be one of the following values:
 *                      FLASH_WP_AREA1: write protection area 1
 *                      FLASH_WP_AREA2: write protection area 2
 *                      FLASH_WP_AREA3: write protection area 3
 *                      FLASH_WP_AREA4: write protection area 4
 *                      FLASH_WP_AREA5: write protection area 5
 *                      FLASH_WP_AREA6: write protection area 6
 *                      FLASH_WP_AREA7: write protection area 7
 *                      FLASH_WP_AREA8: write protection area 8
 * Return:          The state of write protect area (SET or RESET).
 */
FlagStatus FLASH_GetDFlashWriteProtStatus(uint32_t ProtectArea)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_DFLASH_WP_AREAx(ProtectArea));

    if ((EFLASH->DWPR & (uint32_t)ProtectArea) != (uint32_t)RESET) 
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
 * Function:        FLASH_SetDFlashWriteProtection
 * Description:     Set the specified flash write protection area.
 * Param:           ProtectArea: write protection area.
 *                   This parameter can be one of the following values:
 *                      FLASH_WP_AREA1: write protection area 1
 *                      FLASH_WP_AREA2: write protection area 2
 *                      FLASH_WP_AREA3: write protection area 3
 *                      FLASH_WP_AREA4: write protection area 4
 *                      FLASH_WP_AREA5: write protection area 5
 *                      FLASH_WP_AREA6: write protection area 6
 *                      FLASH_WP_AREA7: write protection area 7
 *                      FLASH_WP_AREA8: write protection area 8
 * Return:          Operation status.
 *                   Can be one of the following values:
 *                      FLASH_SUCCESS: operation was successful
 *                      FLASH_ERROR: operation was failure
 *                      FLASH_BUSY: operation was busy
 */
FLASHStatus_TypeDef FLASH_SetDFlashWriteProtection(uint32_t ProtectArea)
{
    FLASHStatus_TypeDef ret = FLASH_SUCCESS;

    /* Check the parameters */
    assert_param(IS_DFLASH_WP_AREAx(ProtectArea));

    /* Write to WP register */
    EFLASH->DWPR &= ~(uint32_t)ProtectArea;

    /* Read the value of WP registers */
    if (FLASH_GetDFlashWriteProtStatus(ProtectArea) != RESET) 
    {
        ret = FLASH_ERROR;
    }
    else
    {
        /* Nothing to do */
    }

    return ret;
}

/*
 * Function:        FLASH_ResetDFlashWriteProtection
 * Description:     Reset the specified flash write protection area.
 * Param:           ProtectArea: write protection area.
 *                   This parameter can be one of the following values:
 *                      FLASH_WP_AREA1: write protection area 1
 *                      FLASH_WP_AREA2: write protection area 2
 *                      FLASH_WP_AREA3: write protection area 3
 *                      FLASH_WP_AREA4: write protection area 4
 *                      FLASH_WP_AREA5: write protection area 5
 *                      FLASH_WP_AREA6: write protection area 6
 *                      FLASH_WP_AREA7: write protection area 7
 *                      FLASH_WP_AREA8: write protection area 8
 * Return:          Operation status.
 *                   Can be one of the following values:
 *                      FLASH_SUCCESS: operation was successful
 *                      FLASH_ERROR: operation was failure
 *                      FLASH_BUSY: operation was busy
 */
FLASHStatus_TypeDef FLASH_ResetDFlashWriteProtection(uint32_t ProtectArea)
{
    FLASHStatus_TypeDef ret = FLASH_SUCCESS;

    /* Check the parameters */
    assert_param(IS_DFLASH_WP_AREAx(ProtectArea));

    /* Write to WP register */
    EFLASH->DWPR |= (uint32_t)ProtectArea;

    /* Read the value of WP registers */
    if (FLASH_GetDFlashWriteProtStatus(ProtectArea) != SET) 
    {
        ret = FLASH_ERROR;
    }
    else
    {
        /* Nothing to do */
    }

    return ret;
}

/*
 * Function:        FLASH_GetECCErrorAddress
 * Description:     Return ECC 2-bit error address.
 * Param:           None.
 * Return:          ECC 2-bit error address.
 */
uint32_t FLASH_GetECCErrorAddress(void)
{
    return EFLASH->EAR;
}

/*
 * Function:        FLASH_ClearECCErrorAddress
 * Description:     Clear ECC 2-bit error address.
 * Param:           None.
 * Return:          None.
 */
void FLASH_ClearECCErrorAddress(void)
{
    EFLASH->EAR = 0x00;
}

/*
 * Function:        FLASH_GetSECFlagStatus
 * Description:     Checks whether the specified Security flash flag is set or not.
 * Param:           FLASH_SECFLAG: specifies the flag to check.
 *                   This parameter can be one of the following values:
 *                      FLASH_SECFLAG_KEY: Flash security Key flag
 *                      FLASH_SECFLAG_RP: Flash read protection flag
 * Return:          The state of FLASH_SECFLAG (SET or RESET).
 */
FlagStatus FLASH_GetSECFlagStatus(uint32_t FLASH_SECFLAG)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_FLASH_SECFLAG(FLASH_SECFLAG));

    if ((EFLASH->SECR & FLASH_SECFLAG) != FLASH_SECFLAG) 
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
 * Function:        FLASH_InputSECKEY
 * Description:     Enter flash security Key.
 * Param:           SEC_KEY0: EFlash security key 0.
 *                  SEC_KEY1: EFlash security key 1.
 * Return:          Operation status.
 *                   Can be one of the following values:
 *                      FLASH_SUCCESS: operation was successful
 *                      FLASH_ERROR: operation was failure
 *                      FLASH_BUSY: operation was busy
 */
FLASHStatus_TypeDef FLASH_InputSECKEY(uint32_t SEC_KEY0, uint32_t SEC_KEY1)
{
    FLASHStatus_TypeDef ret = FLASH_SUCCESS;

    if(FLASH_GetFlagStatus(FLASH_FLAG_BUSY) == SET)
    {
        return FLASH_BUSY;
    }

    /* Set flash bulk erase command */
    FLASH_SetCommand(FLASH_Cmd_SecKEYVerif);

    EFLASH->SECKEY0 = SEC_KEY0;
    EFLASH->SECKEY1 = SEC_KEY1;

    /* Start command */
    ret = FLASH_StartCmd();

    return ret;
}

/*
 * Function:        FLASH_SetCommand
 * Description:     Set flash operation command.
 * Param:           Cmd: Specify operation commands.
 * Return:          None.
 */
static void FLASH_SetCommand(FLASHCmd_TypeDef Cmd)
{
    /* Check the parameters */
    assert_param(IS_FLASH_CMD(Cmd));

    EFLASH->CIDR = (uint32_t)Cmd;
}

/*
 * Function:        FLASH_SetAddress
 * Description:     Set flash operation address.
 * Param:           Address: Enter the operation address.
 * Return:          None.
 */
static void FLASH_SetAddress(uint32_t Address)
{
    /* Check the parameters */
    assert_param((Address >= P_FLASH_Main_BASE) && (Address <= (D_FLASH_Info_BASE + DFLASH_INFO_SIZE)));
     assert_param((Address % 0x8) != 0);
    EFLASH->AR = Address;
}

/*
 * Function:        FLASH_SetDataLength
 * Description:     Set flash operation data length.
 * Param:           Length: Input operation data length.
 * Return:          None.
 */
static void FLASH_SetDataLength(uint32_t Length)
{
    /* Check the parameters */
    assert_param(Length > 0);

    EFLASH->DLR = Length;
}

/*
 * Function:        FLASH_SetLowData
 * Description:     Set low 32-bit data when program flash.
 * Param:           Data: Input low 32-bit data.
 * Return:          None.
 */
static void FLASH_SetLowData(uint32_t Data)
{
    EFLASH->DR0 = Data;
}

/*
 * Function:        FLASH_SetHighData
 * Description:     Set high 32-bit data when program flash.
 * Param:           Data: Input high 32-bit data.
 * Return:          None.
 */
static void FLASH_SetHighData(uint32_t Data)
{
    EFLASH->DR1 = Data;
}

/*
 * Function:        FLASH_StartCmd
 * Description:     Perform start command sequence on flash.
 * Param:           None.
 * Return:          Operation status.
 *                   Can be one of the following values:
 *                      FLASH_SUCCESS: operation was successful.
 *                      FLASH_ERROR: operation was failure.
 *                      FLASH_BUSY: operation was busy.
 */
static FLASHStatus_TypeDef FLASH_StartCmd(void)
{
    FLASHStatus_TypeDef ret = FLASH_SUCCESS;

    FLASH_ClearFlagStatus(FLASH_CLEAR_FINISH);

    if(FLASH_GetFlagStatus(FLASH_FLAG_BUSY) == SET)
    {
        return FLASH_BUSY;
    }

    /* Trigger command execute */
   EFLASH->CTR = EFLASH_CTR_CTR;

    /* Wait till FINISH bit is set */
    while (RESET == FLASH_GetFlagStatus(FLASH_FLAG_FINISH))
    {
        /* Check command execute error status */
        if (SET == FLASH_GetFlagStatus(EFLASH_SR_OPERR | EFLASH_SR_WPERR)) 
        {
            return FLASH_ERROR;
        }
        else
        {
            /* Nothing to do */
        }
    }

    return ret;
}

/*****END OF FILE*****/
