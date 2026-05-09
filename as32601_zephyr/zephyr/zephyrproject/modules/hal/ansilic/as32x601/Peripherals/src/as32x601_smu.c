/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_smu.c
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2023/12/06
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2023/12/06     1.0     build this module
 * Description:     This file provides firmware functions to manage the functionalities of the Reset and clock control.
 */

#include "as32x601_smu.h"
#include "as32x601_flash.h"

/*Global Clock Struct Types---------------------------------------------------*/
SMU_ClocksTypeDef   SMU_ClocksStruct;

/* Private function prototypes -----------------------------------------------*/
static uint32_t SMU_GetPLLQCLK(void);
static uint32_t SMU_GetPLLRCLK(void);

/*
 * This section provides the clock frequencies that can be configured.
 *   +-------------------------------------------------------------------------------------------+
 *   |                                   Clock Frequency (MHz)                                   |
 *   |-------------------------------------------------------------------------------------------|
 *   |    Symbol     |       Parameter      |      MIN       |       TYP       |       MAX       |
 *   |---------------|----------------------|----------------|-----------------|-----------------|
 *   |    FCLKIN     |    Input frequency   |      0.95      |        NA       |      132.3      |
 *   |---------------|----------------------|----------------|-----------------|-----------------|
 *   |    FCLK_PFD   |  PFD input frequency |      0.95      |        1        |       2.1       |
 *   |---------------|----------------------|----------------|-----------------|-----------------|
 *   |    FCLKVCO    | VCO output frequency |      100       |        NA       |       480       |
 *   |---------------|----------------------|----------------|-----------------|-----------------|
 *   |    FCLK_Q     |CLK_Q output frequency|      16        |        NA       |       480       |
 *   |---------------|----------------------|----------------|-----------------|-----------------|
 *   |    FCLK_R     |CLK_R output frequency|      30        |        NA       |       240       |
 *   +-------------------------------------------------------------------------------------------+
 * Note: FCLK_VCO=FCLKIN*DIVF/DIVN; FCLK_Q=FCLK_VCO /DIVQ; FCLK_R=FCLK_VCO /DIVR
 *
 * This section describes the value ranges of the parameters.
 *   +-----------------------------------------------+
 *   |             Parameter value range             |
 *   |-----------------------------------------------|
 *   |    Symbol     |      MIN      |      MAX      |
 *   |---------------|---------------|---------------|
 *   |   DIVN[5:0]   |       1       |      63       |
 *   |---------------|---------------|---------------|
 *   |   DIVF[8:0]   |       50      |      511      |
 *   |---------------|---------------|---------------|
 *   |   DIVQ[3:0]   |       1       |      15       |
 *   |---------------|---------------|---------------|
 *   |   DIVR[2:0]   |       1       |       7       |
 *   +-----------------------------------------------+
 */
/*
 * Function:        SMU_PLLStructInit
 * Description:     Fills each SMU_PLLInitTypeDef member with its default value.
 * Param:           SMU_PLLInitStruct : pointer to a SMU_PLLInitTypeDef structure which will be initialized.
 * Return:          None
 */
void SMU_PLLStructInit(SMU_PLLInitTypeDef *SMU_PLLInitStruct)
{
    /* Reset PLL init structure parameters values */
    SMU_PLLInitStruct->OscillatorType = SMU_OSCILLATORTYPE_OSC;
    SMU_PLLInitStruct->FIRCOscState = DISABLE;
    SMU_PLLInitStruct->FIRCCalibrationValue = 0x00;
    SMU_PLLInitStruct->PLLConfig.PLLState = ENABLE;
    SMU_PLLInitStruct->PLLConfig.PLLSource = SMU_PLLCLK_OSC;
    SMU_PLLInitStruct->PLLConfig.PLLDivR = 0x01;
    SMU_PLLInitStruct->PLLConfig.PLLDivQ = 0x01;
    SMU_PLLInitStruct->PLLConfig.PLLDivN = 0x08;
    SMU_PLLInitStruct->PLLConfig.PLLDivF = 0x32;
}

/*
 * Function:        SMU_PLLInit
 * Description:     Configuration the Osc according to the specified parameters in the SMU_PLLInitStruct.
 * Param:           SMU_PLLInitStruct: pointer to a SMU_PLLInitTypeDef structure that contains the configuration information for the osc.
 * Return:          Error status.
 */
ErrorStatus SMU_PLLInit(SMU_PLLInitTypeDef *SMU_PLLInitStruct)
{
    /* Check Null pointer */
    if(SMU_PLLInitStruct == NULL)
    {
        return ERROR;
    }

    /* Check the parameters */
    assert_param(IS_SMU_OSCILLATOR(SMU_PLLInitStruct->OscillatorType));

    /* OSC Configuration */
    if(SMU_PLLInitStruct->OscillatorType == SMU_OSCILLATORTYPE_OSC)
    {
        SMU_FIRCRc16mPdCmd(ENABLE);
        if(SMU_PLLInitStruct->FIRCOscState == ENABLE)
        {
            //SMU_PLLInitStruct->FIRCOscState = DISABLE;
            //SMU_FIRCRc16mPdCmd(DISABLE);
        }
        else
        {
            ;
        }
    }

    /* FIRC Configuration */
    if(SMU_PLLInitStruct->OscillatorType == SMU_OSCILLATORTYPE_FIRC)
    {
        if(SMU_PLLInitStruct->FIRCOscState == ENABLE)
        {
            SMU_FIRCRc16mFreConfig(SMU_PLLInitStruct->FIRCCalibrationValue);
            SMU_FIRCRc16mPdCmd(ENABLE);
            while(SMU_GetFIRCReadyStatus() == RESET)
            {
                ;
            }
        }
        else
        {
            SMU_FIRCRc16mPdCmd(DISABLE);
            return ERROR;
        }
    }

    /* PLL Configuration */
    if(SMU_PLLInitStruct->PLLConfig.PLLState == ENABLE)
    {
        /* Check the parameters */
        assert_param(IS_SMU_PLLCLKSELECT(SMU_PLLInitStruct->PLLConfig.PLLSource));
        assert_param(IS_SMU_PLLR_VALUE(SMU_PLLInitStruct->PLLConfig.PLLDivR));
        assert_param(IS_SMU_PLLQ_VALUE(SMU_PLLInitStruct->PLLConfig.PLLDivQ));
        assert_param(IS_SMU_PLLN_VALUE(SMU_PLLInitStruct->PLLConfig.PLLDivN));
        assert_param(IS_SMU_PLLF_VALUE(SMU_PLLInitStruct->PLLConfig.PLLDivF));
         if(SMU_PLLConfig(&SMU_PLLInitStruct->PLLConfig) == ERROR)
         {
             return ERROR;
         }
       
        
        while(SMU_GetFlagStatus(SMU_FLAG_PLLLOCK) == RESET)
        {
            ;
        }
    }
    else
    {
        SMU_PLLCmd(DISABLE);
        SMU_PLLRstnCmd(ENABLE);
    }

    return SUCCESS;
}

/*
 * Function:        SMU_ClockStructInit
 * Description:     Fills each SMU_ClockInitStruct member with its default value.
 * Param:           SMU_ClockInitStruct: pointer to a SMU_ClockInitTypeDef structure which will be initialized.
 * Return:          None.
 */
void SMU_ClockStructInit(SMU_ClockInitTypeDef *SMU_ClockInitStruct)
{
    /* Reset GPIO init structure parameters values */
    SMU_ClockInitStruct->SYSCLKSelect = SMU_SYSCLK_PLL;
    SMU_ClockInitStruct->AXI4Bus3CLKDiv = AXI4Bus3CLKDiv1;
    SMU_ClockInitStruct->APBBus0CLKDiv = APBBus0CLKDiv1;
    SMU_ClockInitStruct->APBBus1CLKDiv = APBBus1CLKDiv1;
    SMU_ClockInitStruct->CANX2CLKDiv = CANX2CLKDiv1;
}

/*
 * Function:        SMU_ClockInit
 * Description:     Configer the core clock.
 * Param:           SMU_ClockInitStruct: pointer to a SMU_ClockInitTypeDef structure that contains the configuration information for the core clock.
 * Return:          Error status.
 */
ErrorStatus SMU_ClockInit(SMU_ClockInitTypeDef *SMU_ClockInitStruct)
{
    uint32_t tmpreg = 0x00;
    /* Check Null pointer */
    if(SMU_ClockInitStruct == NULL)
    {
        return ERROR;
    }
    /* Check the parameters */
    assert_param(IS_SMU_SYSCLKSELECT(SMU_ClockInitStruct->SYSCLKSelect));
    assert_param(IS_SMU_AXI4BUS3CLKDIV(SMU_ClockInitStruct->AXI4Bus3CLKDiv));
    assert_param(IS_SMU_APBBUS0CLKDIV(SMU_ClockInitStruct->APBBus0CLKDiv));
    assert_param(IS_SMU_APBBUS1CLKDIV(SMU_ClockInitStruct->APBBus1CLKDiv));
    assert_param(IS_SMU_CANX2CLKDIV(SMU_ClockInitStruct->CANX2CLKDiv));

    /* Set the core frequency division */
    tmpreg = SMU->BUSCFGR;

    /* Set the SYSSEL */
    if(SMU_ClockInitStruct->SYSCLKSelect == SMU_SYSCLK_FIRC)
    {
      SMU_FIRCRc16mPdCmd(ENABLE);
      while(SMU_GetFIRCReadyStatus() == RESET)
      {
          ;
      }
    }
    if(SMU_ClockInitStruct->SYSCLKSelect == SMU_SYSCLK_PLL)
    {
       while(SMU_GetFlagStatus(SMU_FLAG_PLLLOCK) == RESET)
       {
          ;
       }
    }
    tmpreg &= ~((uint32_t)SMU_BUSCFGR_SYSSEL);
    tmpreg |= ((uint32_t)SMU_ClockInitStruct->SYSCLKSelect << SMU_BUSCFGR_SYSSEL_Pos);

    /* Set the AXI4BUS3DIV */
    tmpreg &= ~((uint32_t)SMU_BUSCFGR_AXI4BUS3DIV);
    tmpreg |= ((uint32_t)SMU_ClockInitStruct->AXI4Bus3CLKDiv << SMU_BUSCFGR_AXI4BUS3DIV_Pos);

    /* Set the APBBUS0DIV */
    tmpreg &= ~((uint32_t)SMU_BUSCFGR_APBBUS0DIV);
    tmpreg |= ((uint32_t)SMU_ClockInitStruct->APBBus0CLKDiv << SMU_BUSCFGR_APBBUS0DIV_Pos);

    /* Set the APBBUS1DIV */
    tmpreg &= ~((uint32_t)SMU_BUSCFGR_APBBUS1DIV);
    tmpreg |= ((uint32_t)SMU_ClockInitStruct->APBBus1CLKDiv << SMU_BUSCFGR_APBBUS1DIV_Pos);

    /* Set the CANX2DIV */
    tmpreg &= ~((uint32_t)SMU_BUSCFGR_CANX2DIV);
    tmpreg |= ((uint32_t)SMU_ClockInitStruct->CANX2CLKDiv << SMU_BUSCFGR_CANX2DIV_Pos);
	
    /* Write to SMU BUSCFGR */
    SMU->BUSCFGR = tmpreg;

    return SUCCESS;
}

/*
 * This section provides the operating frequency range of the kernel and bus.
 *   +------------------------------------------------------------------------+
 *   |                          Clock Frequency (MHz)                         |
 *   |------------------------------------------------------------------------|
 *   |      Symbol       |      MIN       |       TYP       |       MAX       |
 *   |-------------------|----------------|-----------------|-----------------|
 *   |      coreClk      |       16       |        NA       |       180       |
 *   |-------------------|----------------|-----------------|-----------------|
 *   |    axi4Bus1Clk    |       16       |        NA       |        90       |
 *   |-------------------|----------------|-----------------|-----------------|
 *   |    axi4Bus3Clk    |        8       |        NA       |        90       |
 *   |-------------------|----------------|-----------------|-----------------|
 *   |     apbBus0Clk    |        8       |        NA       |        45       |
 *   |-------------------|----------------|-----------------|-----------------|
 *   |     apbBus1Clk    |        8       |        NA       |        45       |
 *   |-------------------|----------------|-----------------|-----------------|
 *   |      canClk       |        8       |        NA       |        80       |
 *   |-------------------|----------------|-----------------|-----------------|
 *   |      ADCClk       |        1       |        NA       |        45       |
 *   |-------------------|----------------|-----------------|-----------------|
 *   |      DACClk       |        1       |        NA       |        45       |
 *   +------------------------------------------------------------------------+
 */
/*
* Function:        SMU_GetClocksFreq
* Description:     Returns the frequencies of different clocks.
* Param:           pointer to a SMU_ClocksTypeDef structure which will hold
*                  the clocks frequencies.
* Return:          Error status.
*/
ErrorStatus SMU_GetClocksFreq(SMU_ClocksTypeDef* SMU_Clocks)
{
    uint32_t tmpclk = 0x00;
    uint32_t tmpreg = 0x00;

    /* If select the OSC clock as the clock source */
    if (((SMU->BUSCFGR & SMU_BUSCFGR_SYSSEL) >> SMU_BUSCFGR_SYSSEL_Pos) == SMU_SYSCLK_FIRC)
    {
        tmpclk = CLK_FIRC_HZ;
    }
    else if (((SMU->BUSCFGR & SMU_BUSCFGR_SYSSEL) >> SMU_BUSCFGR_SYSSEL_Pos) == SMU_SYSCLK_PLL)
    {
        tmpclk = SMU_GetPLLQCLK();
    }
    else if (((SMU->BUSCFGR & SMU_BUSCFGR_SYSSEL) >> SMU_BUSCFGR_SYSSEL_Pos) == SMU_SYSCLK_OSC)
    {
        tmpclk = CLK_OSC_HZ;
    }
    else
    {
        return ERROR;
    }

    /* Compute clock frequency */
    SMU_Clocks->AXIBus0_Frequency = tmpclk;

    /* AXIBus0 clock used as AXIBus1 clock source */
    SMU_Clocks->AXIBus1_Frequency = SMU_Clocks->AXIBus0_Frequency / 2;

    /* AXIBus2 clock used as sys clock source */
    SMU_Clocks->AXIBus2_Frequency = SMU_Clocks->AXIBus1_Frequency;

    /* AXIBus1 clock used as AXIBus3 clock source */
    tmpreg = (uint32_t)((SMU->BUSCFGR & SMU_BUSCFGR_AXI4BUS3DIV) >> SMU_BUSCFGR_AXI4BUS3DIV_Pos);
    if(((tmpreg & (tmpreg - 1)) == 0) && (tmpreg <= 0x02))
    {
        SMU_Clocks->AXIBus3_Frequency = SMU_Clocks->AXIBus1_Frequency / tmpreg;
    }
    else
    {
        SMU_Clocks->AXIBus3_Frequency = SMU_Clocks->AXIBus1_Frequency / 2;
    }

    /* AXIBus2 clock used as AXILiteBus0 clock source */
    SMU_Clocks->AXILite4Bus0_Frequency = SMU_Clocks->AXIBus2_Frequency;

    /* AXIBus3 clock used as AXILiteBus1 clock source */
    SMU_Clocks->AXILite4Bus1_Frequency = SMU_Clocks->AXIBus3_Frequency;

    /* AXIBus3 clock used as AXILiteBus2 clock source */
    SMU_Clocks->AXILite4Bus2_Frequency = SMU_Clocks->AXIBus3_Frequency;

    /* AXIBus3 clock used as APBBus0 clock source */
    tmpreg = (uint32_t)((SMU->BUSCFGR & SMU_BUSCFGR_APBBUS0DIV) >> SMU_BUSCFGR_APBBUS0DIV_Pos);
    if(((tmpreg & (tmpreg - 1)) == 0) && (tmpreg <= 0x08))
    {
        SMU_Clocks->APBBus0_Frequency = SMU_Clocks->AXIBus3_Frequency / tmpreg;
    }
    else
    {
        SMU_Clocks->APBBus0_Frequency = SMU_Clocks->AXIBus3_Frequency / 8;
    }

    /* AXIBus3 clock used as APBBus1 clock source */
    tmpreg = (uint32_t)((SMU->BUSCFGR & SMU_BUSCFGR_APBBUS1DIV) >> SMU_BUSCFGR_APBBUS1DIV_Pos);
    if(((tmpreg & (tmpreg - 1)) == 0) && (tmpreg <= 0x08))
    {
        SMU_Clocks->APBBus1_Frequency = SMU_Clocks->AXIBus3_Frequency / tmpreg;
    }
    else
    {
        SMU_Clocks->APBBus1_Frequency = SMU_Clocks->AXIBus3_Frequency / 8;
    }

    /* PLLR clock used as CANCLKx2 clock source */
    tmpclk = SMU_GetPLLRCLK(); 
    tmpreg = (uint32_t)((SMU->BUSCFGR & SMU_BUSCFGR_CANX2DIV) >> SMU_BUSCFGR_CANX2DIV_Pos);
    if(((tmpreg & (tmpreg - 1)) == 0) && (tmpreg <= 0x08))
    {
        SMU_Clocks->CANCLKx2_Frequency = tmpclk / tmpreg;
    }
    else
    {
        SMU_Clocks->CANCLKx2_Frequency = tmpclk / 8;
    }

    return SUCCESS;
}

/*
 * Function:        SMU_FIRCRc16mPdCmd
 * Description:     Enables or disables the 16M RC oscillator.
 * Param:           NewState: new state of the 16M RC oscillator.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void SMU_FIRCRc16mPdCmd(EnState NewState)
{
    /* Check the parameters */
    assert_param(IS_EnState_STATE(NewState));

    if (NewState != (uint32_t)DISABLE) 
    {
        SMU->FIRCCFGR &= ~((uint32_t)SMU_FIRCCFGR_FIRCON);
    } 
    else 
    {
        SMU->FIRCCFGR |= ((uint32_t)SMU_FIRCCFGR_FIRCON);
    }
}

/*
 * Function:        SMU_FIRCRc16mPdAdj
 * Description:     Adjust the 16M RC oscillator frequency.
 * Param:           FIRCRc16mPdFrequency: specifies the 16M RC oscillator frequency.
 * Return:          None.
 */
void SMU_FIRCRc16mFreConfig(uint8_t FIRCRc16mPdFrequency)
{
	uint32_t tmpreg = 0x00;
	
	assert_param(FIRCRc16mPdFrequency < 0x40);
	
	tmpreg = SMU->FIRCCFGR;
	
	tmpreg &= ~((uint32_t)SMU_FIRCCFGR_FIRCTF);

    tmpreg |= ((uint32_t)(FIRCRc16mPdFrequency << SMU_FIRCCFGR_FIRCTF_Pos));
	
    SMU->FIRCCFGR = tmpreg;
}

/*
 * Function:        SMU_PLLCmd
 * Description:     Enables or disables the PLL clock.
 * Param:           NewState: new state of the PLL clock.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void SMU_PLLCmd(EnState NewState)
{
    /* Check the parameters */
    assert_param(IS_EnState_STATE(NewState));

    if (NewState != (uint32_t)DISABLE) 
    {
        SMU->PLLCFGR |= (uint32_t)SMU_PLLCFGR_PLLEN;
    } 
    else 
    {
        SMU->PLLCFGR &= ~(uint32_t)SMU_PLLCFGR_PLLEN;
    }
}

/*
 * Function:        SMU_PLLRstnCmd
 * Description:     Reset or not reset the PLL clock.
 * Param:           NewState: new state of the PLL clock reset.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void SMU_PLLRstnCmd(EnState NewState)
{
    /* Check the parameters */
    assert_param(IS_EnState_STATE(NewState));

    if (NewState != (uint32_t)DISABLE) 
    {
        SMU->PLLCFGR &= ~(uint32_t)SMU_PLLCFGR_PLLRST;
    } 
    else 
    {
        SMU->PLLCFGR |= (uint32_t)SMU_PLLCFGR_PLLRST;
    }
}

/*
 * Function:        SMU_PLLUpdateCmd
 * Description:     Enables or disables the Update PLL frequency.
 * Param:           NewState: new state of the Update PLL frequency.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void SMU_PLLUpdateCmd(EnState NewState)
{
    /* Check the parameters */
    assert_param(IS_EnState_STATE(NewState));

    if (NewState != (uint32_t)DISABLE) 
    {
        SMU->PLLCFGR |= (uint32_t)SMU_PLLCFGR_PLLUF;
    } 
    else 
    {
        SMU->PLLCFGR &= ~(uint32_t)SMU_PLLCFGR_PLLUF;
    }
}

/*
 * Function:        SMU_PLLConfig
 * Description:     Configer the PLL clock.
 * Param:           SMU_PLLConfigStruct: pointer to a SMU_PLLConfigTypeDef structure that contains the configuration information for the PLL clock.
 * Return:          Error status.
 */
ErrorStatus SMU_PLLConfig(SMU_PLLConfigTypeDef *SMU_PLLConfigStruct)
{
    uint32_t tmpreg = 0x00;

    /* Check Null pointer */
    if(SMU_PLLConfigStruct == NULL)
    {
        return ERROR;
    }

   tmpreg = SMU->PLLCFGR;
 
   /* Set the PLLN */
   tmpreg &= ~((uint32_t)SMU_PLLCFGR_PLLN);
   tmpreg |= ((uint32_t)(SMU_PLLConfigStruct->PLLDivN << SMU_PLLCFGR_PLLN_Pos));
 
   /* Set the PLLF */
   tmpreg &= ~((uint32_t)SMU_PLLCFGR_PLLF);
   tmpreg |= ((uint32_t)(SMU_PLLConfigStruct->PLLDivF << SMU_PLLCFGR_PLLF_Pos));
 
   /* Set the PLLQ */
   tmpreg &= ~((uint32_t)SMU_PLLCFGR_PLLQ);
   tmpreg |= ((uint32_t)(SMU_PLLConfigStruct->PLLDivQ << SMU_PLLCFGR_PLLQ_Pos));
 
   /* Set the PLLQ */
   tmpreg &= ~((uint32_t)SMU_PLLCFGR_PLLR);
   tmpreg |= ((uint32_t)(SMU_PLLConfigStruct->PLLDivR << SMU_PLLCFGR_PLLR_Pos));
 
    /* Set the PLLEN */
   tmpreg |= (uint32_t)SMU_PLLCFGR_PLLEN;
   
   /* Set the PLLRst */
   tmpreg |= (uint32_t)SMU_PLLCFGR_PLLRST;
   
   /* Set the PLLUpdate */
   tmpreg |= (uint32_t)SMU_PLLCFGR_PLLUF;
   
   /* Write to SMU PLLCFGR */
   SMU->PLLCFGR = tmpreg;
    
    tmpreg = SMU->BUSCFGR;

    tmpreg &= ~((uint32_t)SMU_BUSCFGR_PLLSEL);
    /* Set the value of the DIVR and DIVQ and DIVN */
    tmpreg |= ((uint32_t)(SMU_PLLConfigStruct->PLLSource << SMU_BUSCFGR_PLLSEL_Pos));

    /* Write to SMU BUSCFGR */
    SMU->BUSCFGR = tmpreg;

    return SUCCESS;
}

/*
 * Function:        SMU_GetFIRCReadyStatus
 * Description:     Get 16M RC oscillator readiness status.
 * Param:           None.
 * Return:          Status of FIRC oscillator ready.
 */
FlagStatus SMU_GetFIRCReadyStatus(void)
{
    FlagStatus bitstatus = RESET;

    if ((SMU->FIRCCFGR & SMU_FIRCCFGR_FIRCRDY) != (uint32_t)RESET) 
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
 * Function:        SMU_GetFlagStatus
 * Description:     Check if the specified SMU status has occurred.
 * Param:           SMU_FLAG: Specifies the SMU flag to check.
 *                   This parameter can be one of the following values:
 *                    SMU_FLAG_PLLLOCK: PLL clock lock flag.
 *                    SMU_FLAG_EFLASHICS: EFLASH initialization completion signal flag.
 *                    SMU_FLAG_PWNSTART: Power on start flag.
 *                    SMU_FLAG_DSSTART: Deep sleep activation flag.
 *                    SMU_FLAG_SLEEPSTART: Sleep start flag.
 *                    SMU_FLAG_LPWR: Power saving mode flag.
 *                    SMU_FLAG_AXI4Bus0: AXI4Bus0 bus status flag.
 *                    SMU_FLAG_AXI4Bus1: AXI4Bus1 bus status flag.
 *                    SMU_FLAG_AXI4Bus2: AXI4Bus2 bus status flag.
 *                    SMU_FLAG_SRAM0RDY: SRAM0 readiness flag.
 *                    SMU_FLAG_SRAM1RDY: SRAM1 readiness flag.
 *                    SMU_FLAG_SRAM2RDY: SRAM2 readiness flag.
 *                    SMU_FLAG_SRAM3RDY: SRAM3 readiness flag.
 * Return:          SMU status.
 *                   This parameter can be one of the following values:
 *                    SET or RESET.
 */
FlagStatus SMU_GetFlagStatus(uint32_t SMU_FLAG)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
	assert_param(IS_SMU_FLAG(SMU_FLAG));

    if ((SMU->SR & SMU_FLAG) != (uint32_t)RESET) 
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
 * Function:        SMU_DCLSCmd
 * Description:     Enables or disables the double core lock step.
 * Param:           NewState: new state of the double core lock step.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void SMU_DCLSCmd(EnState NewState)
{
    /* Check the parameters */
    assert_param(IS_EnState_STATE(NewState));

    if (NewState != (uint32_t)DISABLE) 
    {
        SMU->OTHCFGR |= ((uint32_t)SMU_OTHCFGR_DCLSEN);
    } 
    else 
    {
        SMU->OTHCFGR &= ~((uint32_t)SMU_OTHCFGR_DCLSEN);
    }
}

/*
 * Function:        SMU_ECCCmd
 * Description:     Enables or disables the ECC.
 * Param:           NewState: new state of the ECC.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void SMU_ECCCmd(EnState NewState)
{
    /* Check the parameters */
    assert_param(IS_EnState_STATE(NewState));

    if (NewState != (uint32_t)DISABLE) 
    {
        SMU->OTHCFGR |= ((uint32_t)SMU_OTHCFGR_ECCEN);
    } 
    else 
    {
        SMU->OTHCFGR &= ~((uint32_t)SMU_OTHCFGR_ECCEN);
    }
}

/*
 * Function:        SMU_TimerBreak
 * Description:     Set the specify high timer break output.
 * Param:           TimerSelect: Select the timer to be break outputted.
 *                   This parameter can be one of the following values:
 *                    Timer0Break: Timer 0 break bit.
 *                    Timer1Break: Timer 1 break bit.
 *                    Timer2Break: Timer 2 break bit.
 *                    Timer5Break: Timer 5 break bit.
 * Return:          None.
 */
void SMU_TimerBreak(SMUTimerBreak_TypeDef TimerSelect)
{
   uint32_t tmpreg = 0x00;

   /* Check the parameters */
   assert_param(IS_SMU_TIMEBREAK(TimerSelect));

   /* Select the timer to be break outputted */
   tmpreg = SMU->OTHCFGR;

   tmpreg &= ~((uint32_t)SMU_OTHCFGR_TIMBKEN);
   tmpreg |= ((uint32_t)TimerSelect << SMU_OTHCFGR_TIMBKEN_Pos);

   SMU->OTHCFGR = tmpreg;
}

/*
 * Function:        SMU_ClearTimeOutFlag
 * Description:     Clear timeout flag.
 * Param:           None.
 * Return:          None.
 */
void SMU_ClearTimeOutFlag(void)
{
    SMU->OTHCFGR |= ((uint32_t)SMU_OTHCFGR_TIMEOUTCF);
}

/*
 * Function:        SMU_SetSRAMLowPower
 * Description:     Enable SRAM low power consumption.
 * Param:           SRAMSelect: SRAM low power enabling.
 *                   This parameter can be one of the following values:
 *                    Sram0LowPower: SRAM0 low-power bit.
 *                    Sram1LowPower: SRAM1 low-power bit.
 *                    Sram2LowPower: SRAM2 low-power bit.
 *                    Sram3LowPower: SRAM3 low-power bit.
 *                  NewState: new state of the SRAM low power.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void SMU_SetSRAMLowPower(SMUSramLowPower_TypeDef SRAMSelect, EnState NewState)
{
   /* Check the parameters */
   assert_param(IS_EnState_STATE(NewState));
   assert_param(IS_SMU_SRAMLOWPOWER(SRAMSelect));

   /* Select SRAM low-power enable */
   if (NewState != (uint32_t)DISABLE) 
    {
        SMU->OTHCFGR |= ((uint32_t)SRAMSelect << SMU_OTHCFGR_SRAMLPEN_Pos);
    } 
    else 
    {
        SMU->OTHCFGR &= ~((uint32_t)SRAMSelect << SMU_OTHCFGR_SRAMLPEN_Pos);
    }
}

/*
 * Function:        SMU_SetSysClockOut
 * Description:     Set system clock output.
 * Param:           CLKSelect: system clock output.
 *                   This parameter can be one of the following values:
 *                    CoreCLKDiv8Out: CoreClk's 8-division clock output.
 *                    AxiBus1ClkDiv8Out: AxiBus1Clk's 8-division clock output.
 *                    AxiBus3ClkDiv8Out: AxiBus3Clk's 8-division clock output.
 *                    ApbBus0ClkDiv8Out: ApbBus0Clk's 8-division clock output.
 *                    ApbBus1ClkDiv8Out: ApbBus1Clk's 8-division clock output.
 *                    FircClkDiv8Out: FircClk's 8-division clock output.
 *                    OscClkDiv8Out: OscClk's 8-division clock output.
 *                    SircClkOut: SircClk clock output.
 * Return:          None.
 */
void SMU_SetSysClockOut(SMUCLKOutMux_TypeDef CLKSelect)
{
    uint32_t tmpreg = 0x00;

   /* Check the parameters */
   assert_param(IS_SMU_CLKOUTMUX(CLKSelect));

   /* Select Clock Output Mux */
   tmpreg = SMU->OTHCFGR;

   tmpreg &= ~((uint32_t)SMU_OTHCFGR_CLKOUTMUX);
   tmpreg |= ((uint32_t)(CLKSelect << SMU_OTHCFGR_CLKOUTMUX_Pos));

   SMU->OTHCFGR = tmpreg;
}

/*
 * Function:        SMU_BUSTimeOutCmd
 * Description:     Set bus timeout.
 * Param:           NewState: new state of the bus timeout.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void SMU_BUSTimeOutCmd(EnState NewState)
{
    /* Check the parameters */
    assert_param(IS_EnState_STATE(NewState));

    if (NewState != (uint32_t)DISABLE) 
    {
        SMU->BUSTCFGR |= ((uint32_t)SMU_BUSTCFGR_BUSTIMEOUTEN);
    } 
    else 
    {
        SMU->BUSTCFGR &= ~((uint32_t)SMU_BUSTCFGR_BUSTIMEOUTEN);
    }
}

/*
 * Function:        SMU_SetBUSTimeOutValue
 * Description:     Set bus timeout counter.
 * Param:           TimeOutValue: Bus timeout counter value.
 * Return:          None.
 */
void SMU_SetBUSTimeOutValue(uint32_t TimeOutValue)
{
    /* Check the parameters */
    assert_param((TimeOutValue >= 30000) &&  (TimeOutValue <= 0xFFFF));

    SMU->BUSTCFGR &= ~((uint32_t)SMU_BUSTCFGR_BUSTIMEOUTNUM);
    SMU->BUSTCFGR |= ((uint32_t)TimeOutValue);
}

/*
 * Function:        SMU_GetPLLQCLK
 * Description:     Obtain PLLQ clock frequency.
 * Param:           None.
 * Return:          PLLQ clock frequency.
 */
static uint32_t SMU_GetPLLQCLK(void)
{
    uint32_t pllqclk = 0x00;
    uint32_t pllclk_sel = 0x00;

    /* If select the OSC clock as the clock source */
    if ((SMU->BUSCFGR & SMU_BUSCFGR_PLLSEL)!= RESET) 
    {
        pllclk_sel = CLK_OSC_HZ;
    }
    else
    {
        pllclk_sel = CLK_FIRC_HZ;
    }

    pllqclk = (uint32_t)((pllclk_sel / ((SMU->PLLCFGR & SMU_PLLCFGR_PLLN) >> SMU_PLLCFGR_PLLN_Pos)) *
                                    ((SMU->PLLCFGR & SMU_PLLCFGR_PLLF) >> SMU_PLLCFGR_PLLF_Pos)) /
                                    ((SMU->PLLCFGR & SMU_PLLCFGR_PLLQ) >> SMU_PLLCFGR_PLLQ_Pos);

    return pllqclk;
}

/*
 * Function:        SMU_GetPLLRCLK
 * Description:     Obtain PLLR clock frequency.
 * Param:           None.
 * Return:          PLLR clock frequency.
 */
static uint32_t SMU_GetPLLRCLK(void)
{
    uint32_t pllrclk = 0x00;
    uint32_t pllclk_sel = 0x00;

    /* If select the OSC clock as the clock source */
    if ((SMU->BUSCFGR & SMU_BUSCFGR_PLLSEL) != RESET) 
    {
        pllclk_sel = CLK_OSC_HZ;
    }
    else
    {
        pllclk_sel = CLK_FIRC_HZ;
    }

    pllrclk = (uint32_t)((pllclk_sel / ((SMU->PLLCFGR & SMU_PLLCFGR_PLLN) >> SMU_PLLCFGR_PLLN_Pos)) *
                                    ((SMU->PLLCFGR & SMU_PLLCFGR_PLLF) >> SMU_PLLCFGR_PLLF_Pos)) /
                                    ((SMU->PLLCFGR & SMU_PLLCFGR_PLLR) >> SMU_PLLCFGR_PLLR_Pos);

    return pllrclk;
}

/*****END OF FILE*****/
