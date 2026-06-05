/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_canfd.c
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:			2024/07/30
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/07/30     1.0     build this module
 * Description:     This file provides firmware functions to manage the functionalities of the CANFD peripheral.
 */

#include "as32x601_canfd.h"

static uint8_t Can_Dlc2Reglen[16] = {0U, 1U, 1U, 1U, 1U, 2U, 2U, 2U, 2U, 3U, 4U, 5U, 6U, 8U, 12U, 16U};
static uint8_t Can_Dlc2DataLen[16] = {0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U};
static uint8_t Can_DataLen2Dlc[65] = {0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U,
                                        9U, 9U, 9U, 9U, 10U, 10U, 10U, 10U,
                                        11U, 11U, 11U, 11U, 12U, 12U, 12U, 12U,
                                        13U, 13U, 13U, 13U, 13U, 13U, 13U, 13U,
                                        14U, 14U, 14U, 14U, 14U, 14U, 14U, 14U,
                                        14U, 14U, 14U, 14U, 14U, 14U, 14U, 14U,
                                        15U, 15U, 15U, 15U, 15U, 15U, 15U, 15U,
                                        15U, 15U, 15U, 15U, 15U, 15U, 15U, 15U};

/*
 * Function:        CANFD_StructInit
 * Description:     Fills each CANFD_InitStruct member with its default value.
 * Param:           CANFD_InitStruct: pointer to a CANFD_StructInit structure which will be initialized.
 * Return:          None.
 */
void CANFD_StructInit(CANFD_InitTypeDef* CANFD_InitStruct)
{
	CANFD_InitStruct->CANFD_SRR = CANFD_SRR_RESET;
	CANFD_InitStruct->CANFD_APBRPR = CANFD_APBRPR_1tp;
	CANFD_InitStruct->CANFD_APBTR_APTS1 = CANFD_APBTR_TS1_1tp;
	CANFD_InitStruct->CANFD_APBTR_APTS2 = CANFD_APBTR_TS2_1tp;
	CANFD_InitStruct->CANFD_APBTR_APSJW = CANFD_APBTR_SJW_1tp;
	CANFD_InitStruct->CANFD_DPBRPR = CANFD_DPBRPR_1tp;
	CANFD_InitStruct->CANFD_DPBTR_DPTS1 = CANFD_DPBTR_TS1_1tp;
	CANFD_InitStruct->CANFD_DPBTR_DPTS2 = CANFD_DPBTR_TS2_1tp;
	CANFD_InitStruct->CANFD_DPBTR_DPSJW = CANFD_DPBTR_SJW_1tp;
}

/*
 * Function:        CANFD_Init
 * Description:     Initializes the CANFDx peripheral according to the specified parameters in the CANFD_InitStruct.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  CANFD_InitStruct: pointer to a CANFD_InitTypeDef structure that contains.
 * Return:          None.
 */
void CANFD_Init(CANFD_TypeDef* CANFDx , CANFD_InitTypeDef* CANFD_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
    assert_param(IS_CANFD_SRR(CANFD_InitStruct->CANFD_SRR));
    assert_param(IS_CANFD_APBRPR(CANFD_InitStruct->CANFD_APBRPR));
    assert_param(IS_CANFD_APBTR_TS1(CANFD_InitStruct->CANFD_APBTR_APTS1));
    assert_param(IS_CANFD_APBTR_TS2(CANFD_InitStruct->CANFD_APBTR_APTS2));
    assert_param(IS_CANFD_APBTR_SJW(CANFD_InitStruct->CANFD_APBTR_APSJW));
    assert_param(IS_CANFD_DPBRPR(CANFD_InitStruct->CANFD_DPBRPR));
    assert_param(IS_CANFD_DPBTR_TS1(CANFD_InitStruct->CANFD_DPBTR_DPTS1));
    assert_param(IS_CANFD_DPBTR_TS2(CANFD_InitStruct->CANFD_DPBTR_DPTS2));
    assert_param(IS_CANFD_DPBTR_SJW(CANFD_InitStruct->CANFD_DPBTR_DPSJW));

    CANFDx->SRR = CANFD_InitStruct->CANFD_SRR;
    CANFDx->APBRPR = CANFD_InitStruct->CANFD_APBRPR;
    CANFDx->APBTR = ((uint32_t)(((CANFD_InitStruct->CANFD_APBTR_APTS1) << CANFD_APBTR_TS1_Pos) |
                                                        ((CANFD_InitStruct->CANFD_APBTR_APTS2) << CANFD_APBTR_TS2_Pos) |
                                                        ((CANFD_InitStruct->CANFD_APBTR_APSJW) << CANFD_APBTR_SJW_Pos)));
    CANFDx->DPBRPR = CANFD_InitStruct->CANFD_DPBRPR;
    CANFDx->DPBTR = ((uint32_t)(((CANFD_InitStruct->CANFD_DPBTR_DPTS1) << CANFD_DPBTR_DP_TS1_Pos) |
                                                            ((CANFD_InitStruct->CANFD_DPBTR_DPTS2) << CANFD_DPBTR_DP_TS2_Pos) |
                                                            ((CANFD_InitStruct->CANFD_DPBTR_DPSJW) << CANFD_DPBTR_DP_SJW_Pos)));
    
}			

/*
 * Function:        CANFD_Sleep
 * Description:     CANFDx request core to be in Sleep mode.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          None.
 */
void CANFD_Sleep(CANFD_TypeDef* CANFDx)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	CANFDx->SRR &= ~((uint32_t)CANFD_SRR_CEN);
	CANFDx->MSR &= ~((uint32_t)CANFD_MSR_SNOOP | (uint32_t)CANFD_MSR_LBACK);
	CANFDx->MSR |= ((uint32_t)CANFD_MSR_SLEEP);
	CANFDx->SRR |= CANFD_SRR_CEN;
}

/*
 * Function:        CANFD_WakeUp
 * Description:     CANFDx core wakes up from Sleep mode.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          None.
 */
void CANFD_WakeUp(CANFD_TypeDef* CANFDx)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	CANFDx->SRR &= ~((uint32_t)CANFD_SRR_CEN);
	CANFDx->MSR &= ~((uint32_t)CANFD_MSR_SLEEP);
	CANFDx->SRR |= CANFD_SRR_CEN;
}

/*
 * Function:        CANFD_SetLoopback
 * Description:     CANFDx request core to be in Loopback mode.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          None.
 */
void CANFD_SetLoopback(CANFD_TypeDef* CANFDx)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	CANFDx->SRR &= ~((uint32_t)CANFD_SRR_CEN);
	CANFDx->MSR &= ~((uint32_t)CANFD_MSR_SNOOP | (uint32_t)CANFD_MSR_SLEEP);
	CANFDx->MSR |= ((uint32_t)CANFD_MSR_LBACK);
	CANFDx->SRR |= CANFD_SRR_CEN;
}

/*
 * Function:        CANFD_CloseLoopback
 * Description:     CANFDx no such request core to be in Loopback mode.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          None.
 */
void CANFD_CloseLoopback(CANFD_TypeDef* CANFDx)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	CANFDx->SRR &= ~((uint32_t)CANFD_SRR_CEN);
	CANFDx->MSR &= ~((uint32_t)CANFD_MSR_LBACK);
	CANFDx->SRR |= CANFD_SRR_CEN;
}

/*
 * Function:        CANFD_SetSnoop
 * Description:     CANFDx request core to be in Snoop mode.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          None.
 */
void CANFD_SetSnoop(CANFD_TypeDef* CANFDx)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	CANFDx->SRR &= ~((uint32_t)CANFD_SRR_CEN);
	CANFDx->MSR &= ~((uint32_t)CANFD_MSR_LBACK | (uint32_t)CANFD_MSR_SLEEP);
	CANFDx->MSR |= ((uint32_t)CANFD_MSR_SNOOP);
	CANFDx->SRR |= CANFD_SRR_CEN;
}

/*
 * Function:        CANFD_CloseSnoop
 * Description:     CANFDx no such request core to be in Snoop mode.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          None.
 */
void CANFD_CloseSnoop(CANFD_TypeDef* CANFDx)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	CANFDx->SRR &= ~((uint32_t)CANFD_SRR_CEN);
	CANFDx->MSR &= ~((uint32_t)CANFD_MSR_SNOOP);
	CANFDx->SRR |= CANFD_SRR_CEN;
}

/*
 * Function:        CANFD_BitRateConfig
 * Description:     CANFDx Bit Rate Switch Disable Override.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  NewState: new state of the CANFD Bit Rate Switch.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void CANFD_BitRateConfig(CANFD_TypeDef* CANFDx, EnState NewState)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	assert_param(IS_EnState_STATE(NewState));

	CANFDx->SRR &= ~((uint32_t)CANFD_SRR_CEN);
	if (NewState != DISABLE)
	{
		CANFDx->MSR &= ~((uint32_t)CANFD_MSR_BRSD);
	}
	else
	{
		CANFDx->MSR |= CANFD_MSR_BRSD;
	}
	CANFDx->SRR |= CANFD_SRR_CEN;
}

/*
 * Function:        CANFD_AutoRetransConfig
 * Description:     CANFDx Auto-Retransmission.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  NewState: new state of the CANFD Auto-Retransmission.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void CANFD_AutoRetransConfig(CANFD_TypeDef* CANFDx, EnState NewState)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	assert_param(IS_EnState_STATE(NewState));

	CANFDx->SRR &= ~((uint32_t)CANFD_SRR_CEN);
	if (NewState != DISABLE)
	{
		CANFDx->MSR &= ~((uint32_t)CANFD_MSR_DAR);
	}
	else
	{
		CANFDx->MSR |= CANFD_MSR_DAR;
	}
	CANFDx->SRR |= CANFD_SRR_CEN;
}

/*
 * Function:        CANFD_TDCConfig
 * Description:     CANFDx transmitter Delay Compensation.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  NewState: new state of the CANFD transmitter Delay Compensation.
 *                   This parameter can be: ENABLE or DISABLE.
 *                  TDC_OFFSET: TDC offset value.
 * Return:          None.
 */
void CANFD_TDCConfig(CANFD_TypeDef* CANFDx, EnState NewState, uint8_t TDC_OFFSET)
{
	
	CANFDx->DPBRPR &= ~(CANFD_DPBRPR_TDCOFF | CANFD_DPBRPR_TDC);
    if(NewState == ENABLE)
    {
        CANFDx->DPBRPR |= ((0x01 << CANFD_DPBRPR_TDC_Pos) | (TDC_OFFSET << CANFD_DPBRPR_TDCOFF_Pos));
    }
    else
    {
      /*nothing to do*/
    }
}
/*
 * Function:        CANFD_ProtExcConfig
 * Description:     CANFDx Protocol Exception Event Detection/Generation.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  NewState: new state of the CANFD Protocol Exception Event Detection/Generation.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void CANFD_ProtExcConfig(CANFD_TypeDef* CANFDx, EnState NewState)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	assert_param(IS_EnState_STATE(NewState));

	CANFDx->SRR &= ~((uint32_t)CANFD_SRR_CEN);
	if (NewState != DISABLE)
	{
		CANFDx->MSR &= ~((uint32_t)CANFD_MSR_DPEE);
	}
	else
	{
		CANFDx->MSR |= CANFD_MSR_DPEE;
	}
	CANFDx->SRR |= CANFD_SRR_CEN;
}

/*
 * Function:        CANFD_SetBusOff
 * Description:     CANFDx start Bus-Off Recovery Request.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          None.
 */
void CANFD_SetBusOff(CANFD_TypeDef* CANFDx)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	CANFDx->SRR &= ~((uint32_t)CANFD_SRR_CEN);
	CANFDx->MSR |= CANFD_MSR_SBR;
	CANFDx->SRR |= CANFD_SRR_CEN;
}

/*
 * Function:        CANFD_AutoBusOffRecoConfig
 * Description:     CANFDx Auto Bus-Off Recovery Request.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  NewState: new state of the CANFD Auto Bus-Off Recovery Request.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void CANFD_AutoBusOffRecoConfig(CANFD_TypeDef* CANFDx, EnState NewState)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	assert_param(IS_EnState_STATE(NewState));

	CANFDx->SRR &= ~((uint32_t)CANFD_SRR_CEN);
	if (NewState != DISABLE)
	{
		CANFDx->MSR |= CANFD_MSR_ABR;
	}
	else
	{
		CANFDx->MSR &= ~((uint32_t)CANFD_MSR_ABR);
	}
	CANFDx->SRR |= CANFD_SRR_CEN;
}

/*
 * Function:        CANFD_ReceiveALL
 * Description:     Set CANFDx Receive all message.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  NewState: This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void CANFD_ReceiveALL(CANFD_TypeDef* CANFDx,EnState NewState)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
        if(NewState == ENABLE)
          CANFDx->AFR = 0xffffffff;
        else
          CANFDx->AFR = 0x00000000;
        
        for(int RB = 0; RB < 32; RB++)
        {
          CANFDx->Rxfiltering[RB].AFMR = 0x00000000;
          CANFDx->Rxfiltering[RB].AFIR = 0x00000000;
        }
        
}
/*
 * Function:        CANFD_FilterInit
 * Description:     Set CANFDx Receive filter.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  RSTBx: choose which set of configurations to set.
 * 					 This parameter can be one of TBx where x can be (0..31).
 * 					Afm: setting the Acceptance Filter Mask registers.
 * 					Afi: setting the Acceptance Filter ID registers.
 * Return:          None.
 */
void CANFD_FilterInit(CANFD_TypeDef* CANFDx, uint32_t CANFD_Filterx, uint32_t Afm, uint32_t Afi)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	assert_param(IS_CANFD_Filter_SPACE(CANFD_Filterx));
	assert_param(Afm > 0U);
	assert_param(Afi > 0U);

	CANFDx->Rxfiltering[CANFD_Filterx].AFMR = Afm;
	CANFDx->Rxfiltering[CANFD_Filterx].AFIR = Afi;
	CANFDx->AFR |= ((uint32_t)(0x01 << CANFD_Filterx));
}

/*
 * Function:        CANFD_FrameStructInit
 * Description:     Fills each CANFD_TXFrameStruct member with its default value.
 * Param:           CANFD_TXFrameStruct : pointer to a CANFD_TXFrameStruct structure which will be initialized.
 * Return:          None.
 */

void CANFD_FrameStructInit(CANFD_TXFrameTypeDef* CANFD_TXFrameStruct)
{
	CANFD_TXFrameStruct->CANFD_TX_PROTOCOL = CAN;
	CANFD_TXFrameStruct->CANFD_TX_FORMAT = STANDARD;
	CANFD_TXFrameStruct->CANFD_TX_TYPE = DATA;
	
}

/*
 * Function:        CANFD_FrameInit
 * Description:     CANFDx Frame settings
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  RSTBx: choose which set of configurations to set.
 * 					 This parameter can be one of TBx where x can be (0..31).
 * 					CANFD_TXFrameStruct: pointer to a CANFD_TXFrameStruct structure that contains.
 * Return:          None.
 */
void CANFD_FrameInit(CANFD_TypeDef* CANFDx, uint32_t RSTBx, CANFD_TXFrameTypeDef* CANFD_TXFrameStruct)
{
    uint32_t tmpreg = 0x00;
    
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	assert_param(IS_CANFD_TB_NUMBER(RSTBx));
	assert_param(IS_CANFD_TXFORMAT(CANFD_TXFrameStruct->CANFD_TX_FORMAT));
	assert_param(IS_CANFD_TXPROTOCOL(CANFD_TXFrameStruct->CANFD_TX_PROTOCOL));
    assert_param(IS_CANFD_TXTYPE(CANFD_TXFrameStruct->CANFD_TX_TYPE));
    
    if(CANFD_TXFrameStruct->CANFD_TX_FORMAT == EXTENDED)
    {
        tmpreg = CANFDx->TxMessage[RSTBx].TB_ID;
        tmpreg &= ~(CANFD_TB_ID_ID | CANFD_TB_ID_EM_ID);
        tmpreg |= (CANFD_TXFrameStruct->CANFD_Frame_ID & 0x3FFFF) << CANFD_TB_ID_EM_ID_Pos;
        tmpreg |= ((CANFD_TXFrameStruct->CANFD_Frame_ID >> 18) & 0x7FF) << CANFD_TB_ID_ID_Pos;
        CANFDx->TxMessage[RSTBx].TB_ID = tmpreg;
        
        if(CANFD_TXFrameStruct->CANFD_TX_PROTOCOL == CAN)
        {
            tmpreg = CANFDx->TxMessage[RSTBx].TB_ID;
            tmpreg &= ~(CANFD_TB_ID_SRR | CANFD_TB_ID_IDE | CANFD_TB_ID_RTR);
            tmpreg |= (CANFD_TB_ID_SRR | CANFD_TB_ID_IDE | (CANFD_TXFrameStruct->CANFD_TX_TYPE << CANFD_TB_ID_RTR_Pos));
            CANFDx->TxMessage[RSTBx].TB_ID = tmpreg;
        }
        else
        {
            tmpreg = CANFDx->TxMessage[RSTBx].TB_ID;
            tmpreg &= ~(CANFD_TB_ID_SRR | CANFD_TB_ID_IDE | CANFD_TB_ID_RTR);
            tmpreg |= (CANFD_TB_ID_SRR | CANFD_TB_ID_IDE);
            CANFDx->TxMessage[RSTBx].TB_ID = tmpreg;
        }
        
    }
    else
    {
        tmpreg = CANFDx->TxMessage[RSTBx].TB_ID;
        tmpreg &= ~CANFD_TB_ID_ID;
        tmpreg |= CANFD_TXFrameStruct->CANFD_Frame_ID << CANFD_TB_ID_ID_Pos;
        CANFDx->TxMessage[RSTBx].TB_ID = tmpreg;
        
        if(CANFD_TXFrameStruct->CANFD_TX_PROTOCOL == CAN)
        {
            tmpreg = CANFDx->TxMessage[RSTBx].TB_ID;
            tmpreg &= ~(CANFD_TB_ID_SRR | CANFD_TB_ID_IDE | CANFD_TB_ID_RTR);
            tmpreg |= (CANFD_TXFrameStruct->CANFD_TX_TYPE << CANFD_TB_ID_SRR_Pos);
            CANFDx->TxMessage[RSTBx].TB_ID = tmpreg;
        }
        else
        {
            tmpreg = CANFDx->TxMessage[RSTBx].TB_ID;
            tmpreg &= ~(CANFD_TB_ID_SRR | CANFD_TB_ID_IDE | CANFD_TB_ID_RTR);
            CANFDx->TxMessage[RSTBx].TB_ID = tmpreg;
        }
    }
	CANFDx->TxMessage[RSTBx].TB_DLC &= ~(CANFD_TB_DLC_BRS | CANFD_TB_DLC_EDL);
	CANFDx->TxMessage[RSTBx].TB_DLC = ((CANFD_TXFrameStruct->CANFD_TX_PROTOCOL) << CANFD_TB_DLC_BRS_Pos);
}

/*
 * Function:        CANFD_Transmit
 * Description:     CANFDx Write send data to send register.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  RSTBx: choose which set of configurations to set
 * 					 This parameter can be one of TBx where x can be (0..31)
 * 					Canfdbuf: Input an eight digit array of data.
 * 					Buflength: The number of bytes in the input array.
 * 					 This parameter can be one of the following values:
 * 					  Byte_0,Byte_1,Byte_2,Byte_3,Byte_4,Byte_5,Byte_6,Byte_7,
 * 					  Byte_8,Byte_12,Byte_16,Byte_20,Byte_24,Byte_32,Byte_48,Byte_64.
 * Return:          None.
 */
void CANFD_Transmit(CANFD_TypeDef* CANFDx, uint32_t RSTBx, uint8_t* Canfdbuf, uint32_t Buflength)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	assert_param(IS_CANFD_TB_NUMBER(RSTBx));
	assert_param(Canfdbuf != 0U);
	assert_param(IS_CANFD_DATA_LENGHT(Buflength));

	uint32_t* txaddress = (uint32_t*)&CANFDx->TxMessage[RSTBx].TB_DW0;
	uint32_t ByteCounter;

	CANFDx->TxMessage[RSTBx].TB_DLC &= ~CANFD_TB_DLC_DLC;
	CANFDx->TxMessage[RSTBx].TB_DLC |= (Can_DataLen2Dlc[Buflength] << CANFD_TB_DLC_DLC_Pos);

	for(ByteCounter = 0; ByteCounter < Buflength; ByteCounter += 4)
	{
		*txaddress++ = ((Canfdbuf[ByteCounter] << 24) |
					(Canfdbuf[ByteCounter + 1] << 16) |
					(Canfdbuf[ByteCounter + 2] << 8) |
					(Canfdbuf[ByteCounter + 3]));
	}
	CANFD_NormalStatus(CANFDx);
	CANFD_SetSendChannel(CANFDx, 1 << RSTBx);
}

/*
 * Function:        CANFD_LoopbackTransmit
 * Description:     CANFDx Write send data to send register.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  RSTBx: choose which set of configurations to set
 * 					 This parameter can be one of TBx where x can be (0..31)
 * 					Canfdbuf: Input an eight digit array of data.
 * 					Buflength: The number of bytes in the input array.
 * 					 This parameter can be one of the following values:
 * 					  Byte_0,Byte_1,Byte_2,Byte_3,Byte_4,Byte_5,Byte_6,Byte_7,
 * 					  Byte_8,Byte_12,Byte_16,Byte_20,Byte_24,Byte_32,Byte_48,Byte_64.
 * Return:          None.
 */
void CANFD_LoopbackTransmit(CANFD_TypeDef* CANFDx, uint32_t RSTBx, uint8_t* Canfdbuf, uint32_t Buflength)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	assert_param(IS_CANFD_TB_NUMBER(RSTBx));
	assert_param(Canfdbuf != 0U);
	assert_param(IS_CANFD_DATA_LENGHT(Buflength));

	uint32_t* txaddress = (uint32_t*)&CANFDx->TxMessage[RSTBx].TB_DW0;
	uint32_t ByteCounter;

	CANFDx->TxMessage[RSTBx].TB_DLC &= ~CANFD_TB_DLC_DLC;
	CANFDx->TxMessage[RSTBx].TB_DLC |= (Can_DataLen2Dlc[Buflength] << CANFD_TB_DLC_DLC_Pos);

	for(ByteCounter = 0; ByteCounter < Buflength; ByteCounter += 4)
	{
		*txaddress++ = ((Canfdbuf[ByteCounter] << 24) |
					(Canfdbuf[ByteCounter + 1] << 16) |
					(Canfdbuf[ByteCounter + 2] << 8) |
					(Canfdbuf[ByteCounter + 3]));
	}
	CANFD_SetSendChannel(CANFDx, 1 << RSTBx);
}

/*
 * Function:        CANFD_NormalStatus
 * Description:     CANFDx wait the core is in Normal mode.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          None.
 */
void CANFD_NormalStatus(CANFD_TypeDef* CANFDx)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	while (1)
	{
		if (CANFD_GetStatus(CANFDx, CANFD_SR_NORMAL))
		break;
	}
}

/*
 * Function:        CANFD_GetStatus
 * Description:     CANFDx get corresponding status.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * 					State: Select the state of judgment.
 * 					 This parameter can be one of the following values:
 * 					  CANFD_SR_CONFIG: Configuration Mode Indicator.
 * 					  CANFD_SR_LBACK: Loopback Mode.
 * 					  CANFD_SR_SLEEP: Sleep Mode.
 * 					  CANFD_SR_NORMAL: Normal Mode.
 * 					  CANFD_SR_BIDLE: Bus Idle.
 * 					  CANFD_SR_BBSY: Indicates the CAN bus status.
 * 					  CANFD_SR_ERRWRN: Error Warning.
 * 					  CANFD_SR_ESTAT: Error Status.
 * 					  CANFD_SR_PEE_CONFIG: PEE Mode Indicator.
 * 					  CANFD_SR_BSFR_CONFIG: Bus-Off Recovery Mode Indicator.
 * 					  CANFD_SR_SNOOP: Snoop Mode.
 * 					  CANFD_SR_TDCV: Transmitter Delay Compensation Value.
 * Return:          Canfd status.
 *					 Can be one of the following values:
 *                    SET or RESET.
 *                    CANFD_SR_ESTAT/CANFD_SR_TDCV: return values.
 */
uint32_t CANFD_GetStatus(CANFD_TypeDef* CANFDx, uint32_t State)
{
	uint32_t state_data = 0x00;

	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	assert_param(State & 0x007F17FF);

	if ((CANFDx->SR) & State & 0x0000167fu)
	{
		state_data = (uint32_t)SET;
	}
	else
	{
		state_data = (uint32_t)RESET;
	}

	if (State & 0x00000180u)
	{
		state_data = (((CANFDx->SR) >> 7) & 0x03);
	}
	if (State & 0x007f0000u)
	{
		state_data = (((CANFDx->SR) >> 16) & 0x7f);
	}
	return state_data;
}

/*
 * Function:        CANFD_ITConfig
 * Description:     Configer the CANFD interrupt trigger type.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * 					ITType: CANFD interrupt trigger type.
 * 					 This parameter can be one of the following values:
 * 					 CANFD_IT_ALL: ALL Interrupt Enable.
 * 					 CANFD_IT_EARBLST: Arbitration Lost Interrupt Enable.
 * 					 CANFD_IT_ETXOK: Transmission Successful Interrupt Enable.
 * 					 CANFD_IT_EPEE: Protocol Exception Event Interrupt Enable.
 * 					 CANFD_IT_EBSFRD: Bus-Off Recovery Done Interrupt Enable.
 * 					 CANFD_IT_ERXOK: New Message Received Interrupt Enable.
 * 					 CANFD_IT_ETSCNT_OFLW: Timestamp Counter Overflow Interrupt Enable.
 * 					 CANFD_IT_ERXFOFLW: RX FIFO-0 Overflow Interrupt Enable (Sequential/FIFO Mode).
 * 					 CANFD_IT_EERROR: Error Interrupt Enable.
 * 					 CANFD_IT_EBSOFF: Bus-Off Interrupt Enable.
 * 					 CANFD_IT_ESLP: Sleep Interrupt Enable.
 * 					 CANFD_IT_EWKUP: Wake-Up Interrupt Enable.
 * 					 CANFD_IT_ETXRRS: TX Buffer Ready Request Served Interrupt Enable.
 * 					 CANFD_IT_ETXCRS: TX Cancellation Request Served Interrupt Enable.
 *                  NewState: new state of the CANFD Interrupt Enable.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void CANFD_ITConfig(CANFD_TypeDef* CANFDx, uint32_t ITType, EnState NewState)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	assert_param(IS_CANFD_IT(ITType));
	assert_param(IS_EnState_STATE(NewState));

	if (NewState != DISABLE)
	{
		CANFDx->IER |= ITType;
	}
	else
	{
		CANFDx->IER &= ~((uint32_t)ITType);
	}
}

/*
 * Function:        CANFD_SetSendChannel
 * Description:     CANFDx send data.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  SendTb: select the channel for sending.
 * 					 This parameter can be one of SEND_TBx where x can be (0..31) and SEND_ALL.
 * Return:          None.
 */
void CANFD_SetSendChannel(CANFD_TypeDef* CANFDx , uint32_t SendTb)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	assert_param(IS_CANFD_TB_SEND(SendTb));

	CANFDx->TRR = SendTb;
}

/*
 * Function:        CANFD_GetSendCmpStatus
 * Description:     CANFDx send data completion judgment.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  SendTb: select the channel for sending
 * 					 This parameter can be one of SEND_TBx where x can be (0..31) and SEND_ALL.
 * Return:          The state of CANFD send completion (SET or RESET).
 */
FlagStatus CANFD_GetSendCmpStatus(CANFD_TypeDef* CANFDx, uint32_t SendTb)
{
	FlagStatus bitstatus = RESET;

	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	assert_param(IS_CANFD_TB_SEND(SendTb));

	if (((CANFDx->TRR) & SendTb) != (uint32_t)RESET)
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
 * Function:        CANFD_GetTimtampValue
 * Description:     Return CANFDx Timestamp Counter Value.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          CANFD Timestamp Counter Value.
 */
uint32_t CANFD_GetTimtampValue(CANFD_TypeDef* CANFDx)
{
	uint32_t tmpreg = 0x00;

	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	tmpreg = ((uint32_t)(CANFDx->TSR >> CANFD_TSR_TIMESTAMP_CNT_Pos));

	return tmpreg;
}

/*
 * Function:        CANFD_ClearTimtamp
 * Description:     CANFDx Clear Timestamp Counter.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          None.
 */
void CANFD_ClearTimtamp(CANFD_TypeDef* CANFDx)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	CANFDx->TSR |= CANFD_TSR_CTS;
}

/*
 * Function:        CANFD_GetErrorStatus
 * Description:     CANFDx get Error Status.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * 					ErrorState: Select Error Status.
 * 					 This parameter can be one of the following values:
 * 					  CANFD_ERROR_FLAG_CRCER: CANFD CRC error flag.
 * 					  CANFD_ERROR_FLAG_FMER: CANFD form error flag.
 * 					  CANFD_ERROR_FLAG_STER: CANFD stuff error flag.
 * 					  CANFD_ERROR_FLAG_BERR: CANFD bit error flag.
 * 					  CANFD_ERROR_FLAG_ACKER: CANFD ACK error flag.
 * 					  CANFD_ERROR_FLAG_F_CRCER: CANFD CRC error in CAN FD data phase flag.
 * 					  CANFD_ERROR_FLAG_F_FMER: CANFD form error in CAN FD data phase flag.
 * 					  CANFD_ERROR_FLAG_F_STER: CANFD stuff error in CAN FD data phase flag.
 * 					  CANFD_ERROR_FLAG_F_BERR: CANFD bit error in CAN FD data phase flag.
 * Return:          The state of CANFD Error Flag(SET or RESET).
 */
FlagStatus CANFD_GetErrorStatus(CANFD_TypeDef* CANFDx, uint32_t ErrorState)
{
	FlagStatus bitstatus = RESET;

	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	assert_param(IS_CANFD_ESR_STATE(ErrorState));

	if (((CANFDx->ESR) & ErrorState) != (uint32_t)RESET)
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
 * Function:        CANFD_GetRXErrorCount
 * Description:     CANFDx Receive Error Count.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          CANFD Receive Error number
 */
uint32_t CANFD_GetRXErrorCount(CANFD_TypeDef* CANFDx)
{
	uint32_t tmpreg = 0x00;

	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	tmpreg = ((uint32_t)((CANFDx->ECR & CANFD_ECR_REC) >> CANFD_ECR_REC_Pos));

	return tmpreg;
}

/*
 * Function:        CANFD_GetTXErrorCount
 * Description:     CANFDx Transmit Error count.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          CANFD Transmit Error number.
 */
uint32_t CANFD_GetTXErrorCount(CANFD_TypeDef* CANFDx)
{
	uint32_t tmpreg = 0x00;

	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	tmpreg = ((uint32_t)((CANFDx->ECR & CANFD_ECR_TEC) >> CANFD_ECR_TEC_Pos));

	return tmpreg;
}

/*
 * Function:        CANFD_SetBufferReadyIT
 * Description:     CANFDx TX Buffer Ready Req Served/Cleared Interrupt Enable.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  Cleartb: select the channel for sending.
 * 					 This parameter can be one of CANFD_TBx where x can be (0..31,ALL).
 * 					NewState: new state of the CANFD TX Buffer Ready Req Served/Cleared Interrupt Enable.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void CANFD_SetBufferReadyIT(CANFD_TypeDef* CANFDx, uint32_t Cleartb, EnState NewState)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	assert_param(IS_CANFD_TB_SPACE(Cleartb));
	assert_param(IS_EnState_STATE(NewState));
	
	if (NewState != DISABLE)
	{
		CANFDx->ERRS |= Cleartb;
	}
	else
	{
		CANFDx->ERRS &= ~((uint32_t)Cleartb);
	}
}

/*
 * Function:        CANFD_CancelTransmit
 * Description:     CANFDx TX Buffer Cancel Request.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  TcrTb: select the channel for sending.
 * 					 This parameter can be one of CANFD_TBx where x can be (0..31,ALL).
 * Return:          None.
 */
void CANFD_CancelTransmit(CANFD_TypeDef* CANFDx, uint32_t TcrTb)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	assert_param(IS_CANFD_TB_SPACE(TcrTb));

	CANFDx->TCR |= TcrTb;
}

/*
 * Function:        CANFD_TxBufferCancelIT
 * Description:     CANFDx TX Buffer Transmission Served/Cleared Interrupt Enable.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  EcrsTb: select the channel for sending.
 * 					 This parameter can be one of CANFD_TBx where x can be (0..31,ALL).
 * 					NewState: new state of the CANFD TX Buffer Transmission Served/Cleared Interrupt Enable.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void CANFD_TxBufferCancelIT(CANFD_TypeDef* CANFDx, uint32_t EcrsTb, EnState NewState)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	assert_param(IS_CANFD_TB_SPACE(EcrsTb));
	assert_param(IS_EnState_STATE(NewState));

	if (NewState != DISABLE)
	{
		CANFDx->ECRS |= EcrsTb;
	}
	else
	{
		CANFDx->ECRS &= ~((uint32_t)EcrsTb);
	}
}


/*
 * Function:        CANFD_GetRxFillLevel
 * Description:     Return CANFDx RX FIFO Fill Level (0 to 64).
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          RX FIFO Fill Level (0 to 64).
 */
uint32_t CANFD_GetRxFillLevel(CANFD_TypeDef* CANFDx)
{
	uint32_t tmpreg = 0x00;

	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	tmpreg = ((uint32_t)(((CANFDx->RFSR) & CANFD_RFSR_FL) >> CANFD_RFSR_FL_Pos));

	return tmpreg;
}

/*
 * Function:        CANFD_GetRxReadIndex
 * Description:     Return CANFDx RX FIFO Read Index (0 to 63).
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          RX FIFO Read Index (0 to 63).
 */
uint32_t CANFD_GetRxReadIndex(CANFD_TypeDef* CANFDx)
{
	uint32_t tmpreg = 0x00;

	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	tmpreg = ((uint32_t)(((CANFDx->RFSR) & CANFD_RFSR_RI) >> CANFD_RFSR_RI_Pos));

	return tmpreg;
}

/*
 * Function:        CANFD_SetRxIncReadIndex
 * Description:     CANFDx RX FIFO Increment Read Index.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          None.
 */
void CANFD_SetRxIncReadIndex(CANFD_TypeDef* CANFDx)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	CANFDx->RFSR |= CANFD_RFSR_IRI;
}

/*
 * Function:        CANFD_Enable
 * Description:     CANFDx Enable.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          None.
 */
void CANFD_Enable(CANFD_TypeDef* CANFDx)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	CANFDx->SRR = CANFD_SRR_CEN;
}

/*
 * Function:        CANFD_GetITStatus
 * Description:     Gets the specified CANFD interrupt status.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 *                  Flag: Specifies the CANFD flag to check.
 *                   This parameter can be one of the following values:
 * 					  CANFD_FLAG_ARBLST: Arbitration lost interrupt flag.
 * 					  CANFD_FLAG_TXOK: Transmission successful interrupt flag.
 * 					  CANFD_FLAG_PEE: Protocol exception event interrupt flag.
 * 					  CANFD_FLAG_BSFRD: Bus-off recovery done interrupt flag.
 * 					  CANFD_FLAG_RXOK: New message received interrupt flag.
 * 					  CANFD_FLAG_TSCNT_OFLW: Timestamp counter overflow interrupt flag.
 * 					  CANFD_FLAG_RXFOFLW: Rx fifo-0 overflow interrupt flag (sequential/fifo mode).
 * 					  CANFD_FLAG_ERROE: Error interrupt flag.
 * 					  CANFD_FLAG_BSOFF: Bus-off interrupt flag.
 * 					  CANFD_FLAG_SLP: Sleep interrupt flag.
 * 					  CANFD_FLAG_WKUP: Wake-up interrupt flag.
 * 					  CANFD_FLAG_TXRRS: Tx buffer ready request served interrupt flag.
 * 					  CANFD_FLAG_TXCRS: Tx cancellation request served interrupt flag.
 * Return:          CANFD IT status.
 * 					 This parameter can be one of the following values:
 * 					  SET or RESET.
 */
ITStatus CANFD_GetITStatus(CANFD_TypeDef *CANFDx, uint32_t Flag)
{
    ITStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
    assert_param(IS_CANFD_FLAG(Flag));

    if ((CANFDx->ISR & Flag) != RESET) 
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
 * Function:        CANFD_ClearITPendingBit
 * Description:     Clear the interrupt pending bits of the specified CANFD.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * 					ClearIT: CANFD interruption clear flags.
 * 					 This parameter can be one of the following values:
 * 					  CANFD_CLEAR_ARBLST: Clears arbitration lost interrupt.
 * 					  CANFD_CLEAR_TXOK: Clears transmission successful interrupt.
 * 					  CANFD_CLEAR_PEE: Clears protocol exception event interrupt.
 * 					  CANFD_CLEAR_BSFRD: Clears bus-off recovery done interrupt.
 * 					  CANFD_CLEAR_RXOK: Clears new message received interrupt.
 * 					  CANFD_CLEAR_TSCNT_OFLW: Clears timestamp counter overflow interrupt.
 * 					  CANFD_CLEAR_RXFOFLW: Clears rx fifo-0 overflow interrupt (sequential/fifo mode).
 * 					  CANFD_CLEAR_ERROE: Clears error interrupt.
 * 					  CANFD_CLEAR_BSOFF: Clears bus-off interrupt.
 * 					  CANFD_CLEAR_SLP: Clears sleep interrupt.
 * 					  CANFD_CLEAR_WKUP: Clears wake-up interrupt.
 * 					  CANFD_CLEAR_TXRRS: Clears tx buffer ready request served interrupt.
 * 					  CANFD_CLEAR_TXCRS: Clears tx cancellation request served interrupt.
 * Return:          None
 */
void CANFD_ClearITPendingBit(CANFD_TypeDef *CANFDx, uint32_t ClearIT)
{
	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
    assert_param(IS_CANFD_CLEAR(ClearIT));

	CANFDx->ICR = ((uint32_t)ClearIT);
}

/*
 * Function:        CANFD_GetReceiveBytes
 * Description:     Return CANFDx Bytes received.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * Return:          Bytes received.
 */
uint32_t CANFD_GetReceiveBytes(CANFD_TypeDef* CANFDx)
{
	uint32_t tmpreg = 0x00;
	uint8_t rxri = 0x00;
	uint32_t value = 0x00;

	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	rxri = (CANFDx->RFSR & CANFD_RFSR_RI);
	tmpreg = ((uint32_t)(((CANFDx->RxMessage[rxri].RB_DLC) & CANFD_RB_DLC_DLC) >> CANFD_RB_DLC_DLC_Pos));
	switch (tmpreg)
	{
		case 0:
			value = Byte_0;
			break;
		case 1:
			value = Byte_1;
			break;
		case 2:
			value = Byte_2;
			break;
		case 3:
			value = Byte_3;
			break;
		case 4:
			value = Byte_4;
			break;
		case 5:
			value = Byte_5;
			break;
		case 6:
			value = Byte_6;
			break;
		case 7:
			value = Byte_7;
			break;
		case 8:
			value = Byte_8;
			break;
		case 9:
			value = Byte_12;
			break;
		case 10:
			value = Byte_16;
			break;
		case 11:
			value = Byte_20;
			break;
		case 12:
			value = Byte_24;
			break;
		case 13:
			value = Byte_32;
			break;
		case 14:
			value = Byte_48;
			break;
		case 15:
			value = Byte_64;
			break;
		default:
			break;
	}

	return value;
}

/*
 * Function:        CANFD_GetReceiveData
 * Description:     CANFDx Read received data and store it in an eight bit array.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * 					Data: The user inputs an eight bit array for storing data
 * Return:          None.
 */
uint32_t CANFD_GetReceiveData(CANFD_TypeDef* CANFDx, uint8_t* Data)
{
	uint8_t rxfl = 0x00;
	uint8_t rxri = 0x00;
	uint8_t datalenght = 0x00;
	uint8_t mdata = 0x00;
	uint32_t receive_len = 0x00;
	uint32_t RXDataBuff[16] = { 0 };

	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));

	rxfl = ((uint8_t)(CANFDx->RFSR >> CANFD_RFSR_FL_Pos));
	if (rxfl != 0)
	{
		rxri = ((uint8_t)((CANFDx->RFSR & CANFD_RFSR_RI) >> CANFD_RFSR_RI_Pos));
		datalenght = Can_Dlc2Reglen[((CANFDx->RxMessage[rxri].RB_DLC) >> 28) & 0xF];
		for (uint8_t rxdatax = 0; rxdatax < datalenght; rxdatax++)
		{
			switch (rxdatax)
			{
				case 0:
					RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW0;
					break;
				case 1:
					RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW1;
					break;
				case 2:
					RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW2;
					break;
				case 3:
					RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW3;
					break;
				case 4:
					RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW4;
					break;
				case 5:
					RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW5;
					break;
				case 6:
					RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW6;
					break;
				case 7:
					RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW7;
					break;
				case 8:
					RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW8;
					break;
				case 9:
					RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW9;
					break;
				case 10:
					RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW10;
					break;
				case 11:
					RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW11;
					break;
				case 12:
					RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW12;
					break;
				case 13:
					RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW13;
					break;
				case 14:
					RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW14;
					break;
				case 15:
					RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW15;
					break;
				default:
					break;
			}
		}
		for (uint8_t buffx = 0; buffx < datalenght; buffx++)
		{
			mdata = RXDataBuff[buffx] >> 24;
			*Data++ = mdata;
			mdata = RXDataBuff[buffx] >> 16;
			*Data++ = mdata;
			mdata = RXDataBuff[buffx] >> 8;
			*Data++ = mdata;
			mdata = RXDataBuff[buffx];
			*Data++ = mdata;
		}
        receive_len = (uint32_t)Can_Dlc2DataLen[((CANFDx->RxMessage[rxri].RB_DLC) >> 28) & 0xF];
		CANFDx->RFSR |= CANFD_RFSR_IRI;
		rxfl = (uint8_t)((CANFDx->RFSR & CANFD_RFSR_FL) >> CANFD_RFSR_FL_Pos);
	}
    
    return receive_len;
}

/*
 * Function:        CANFD_GetReceiveInfo
 * Description:     CANFDx Read received data and store it in an eight bit array.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * 					Data: The user inputs an eight bit array for storing data
 * Return:          None.
 */
CANFD_RXInfoTypeDef CANFD_GetReceiveInfo(CANFD_TypeDef* CANFDx)
{
	CANFD_RXInfoTypeDef CANFD_RXInfoStruct;
        uint8_t rxri = 0x00;
        uint32_t tempreg= 0x00;
    
        /* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	
	/* Get the CANFD message count*/
	CANFD_RXInfoStruct.CANFD_Message_Count = ((uint8_t)(CANFDx->RFSR >> CANFD_RFSR_FL_Pos));
	
	rxri = ((uint8_t)((CANFDx->RFSR & CANFD_RFSR_RI) >> CANFD_RFSR_RI_Pos));
	/* Get the current CANFD message length*/
	tempreg = ((uint32_t)(((CANFDx->RxMessage[rxri].RB_DLC) & CANFD_RB_DLC_DLC) >> CANFD_RB_DLC_DLC_Pos));
	switch (tempreg)
	{
		case 0:
			CANFD_RXInfoStruct.CANFD_CurrentMessage_DLC = Byte_0;
			break;
		case 1:
			CANFD_RXInfoStruct.CANFD_CurrentMessage_DLC = Byte_1;
			break;
		case 2:
			CANFD_RXInfoStruct.CANFD_CurrentMessage_DLC = Byte_2;
			break;
		case 3:
			CANFD_RXInfoStruct.CANFD_CurrentMessage_DLC = Byte_3;
			break;
		case 4:
			CANFD_RXInfoStruct.CANFD_CurrentMessage_DLC = Byte_4;
			break;
		case 5:
			CANFD_RXInfoStruct.CANFD_CurrentMessage_DLC = Byte_5;
			break;
		case 6:
			CANFD_RXInfoStruct.CANFD_CurrentMessage_DLC = Byte_6;
			break;
		case 7:
			CANFD_RXInfoStruct.CANFD_CurrentMessage_DLC = Byte_7;
			break;
		case 8:
			CANFD_RXInfoStruct.CANFD_CurrentMessage_DLC = Byte_8;
			break;
		case 9:
			CANFD_RXInfoStruct.CANFD_CurrentMessage_DLC = Byte_12;
			break;
		case 10:
			CANFD_RXInfoStruct.CANFD_CurrentMessage_DLC = Byte_16;
			break;
		case 11:
			CANFD_RXInfoStruct.CANFD_CurrentMessage_DLC = Byte_20;
			break;
		case 12:
			CANFD_RXInfoStruct.CANFD_CurrentMessage_DLC = Byte_24;
			break;
		case 13:
			CANFD_RXInfoStruct.CANFD_CurrentMessage_DLC = Byte_32;
			break;
		case 14:
			CANFD_RXInfoStruct.CANFD_CurrentMessage_DLC = Byte_48;
			break;
		case 15:
			CANFD_RXInfoStruct.CANFD_CurrentMessage_DLC = Byte_64;
			break;
		default:
			break;
	}
	
	if((CANFDx->RxMessage[rxri].RB_ID >> CANFD_RB_ID_IDE_Pos) & 0x01)
	{
		CANFD_RXInfoStruct.CANFD_CurrentMessage_FORMAT = EXTENDED;
		CANFD_RXInfoStruct.CANFD_CurrentFrame_ID = (((CANFDx->RxMessage[rxri].RB_ID >> CANFD_RB_ID_ID_Pos) & 0x7FF) << 18) + ((CANFDx->RxMessage[rxri].RB_ID >> CANFD_RB_ID_EM_ID_Pos) & 0x3FFFF);
		if((CANFDx->RxMessage[rxri].RB_ID >> CANFD_RB_ID_RTR_DPos) & 0x01)
			CANFD_RXInfoStruct.CANFD_CurrentMessage_TYPE  = REMOTE;
		else
			CANFD_RXInfoStruct.CANFD_CurrentMessage_TYPE  = DATA;
	}
	else
	{
		CANFD_RXInfoStruct.CANFD_CurrentMessage_FORMAT = STANDARD;
		CANFD_RXInfoStruct.CANFD_CurrentFrame_ID = (CANFDx->RxMessage[rxri].RB_ID >> CANFD_RB_ID_ID_Pos) & 0x7FF;
		if((CANFDx->RxMessage[rxri].RB_ID >> CANFD_RB_ID_SRR_Pos) & 0x01)
			CANFD_RXInfoStruct.CANFD_CurrentMessage_TYPE  = REMOTE;
		else
			CANFD_RXInfoStruct.CANFD_CurrentMessage_TYPE  = DATA;	
	}
	
	if(((CANFDx->RxMessage[rxri].RB_DLC >> CANFD_RB_DLC_BRS_Pos) & 0x03) == 0x03)
	{
		CANFD_RXInfoStruct.CANFD_CurrentMessage_PROTOCOL = CANFD_BRS;
	}
	else if(((CANFDx->RxMessage[rxri].RB_DLC >> CANFD_RB_DLC_BRS_Pos) & 0x03) == 0x02)
	{
		CANFD_RXInfoStruct.CANFD_CurrentMessage_PROTOCOL = CANFD;
	}
	else
	{
		CANFD_RXInfoStruct.CANFD_CurrentMessage_PROTOCOL = CAN;
	}
    
    return CANFD_RXInfoStruct;
}

/*
 * Function:        CANFD_GetReceiveMessage
 * Description:     CANFDx Read received data and store it in an eight bit array.
 * Param:           CANFDx: where x can be (0..3) to select the CANFD peripheral.
 * 					Data: The user inputs an eight bit array for storing data
 * Return:          None.
 */
void CANFD_GetReceiveMessage(CANFD_TypeDef* CANFDx, uint8_t* Data)
{
	uint8_t rxri = 0x00;
	uint8_t datalenght = 0x00;
	uint8_t mdata = 0x00;
	uint32_t RXDataBuff[16] = { 0 };

	/* Check the parameters */
	assert_param(IS_CANFD_ALL_PERIPH(CANFDx));
	rxri = ((uint8_t)((CANFDx->RFSR & CANFD_RFSR_RI) >> CANFD_RFSR_RI_Pos));
	datalenght = Can_Dlc2Reglen[((CANFDx->RxMessage[rxri].RB_DLC) >> 28) & 0xF];
	for (uint8_t rxdatax = 0; rxdatax < datalenght; rxdatax++)
	{
		switch (rxdatax)
		{
			case 0:
				RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW0;
				break;
			case 1:
				RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW1;
				break;
			case 2:
				RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW2;
				break;
			case 3:
				RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW3;
				break;
			case 4:
				RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW4;
				break;
			case 5:
				RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW5;
				break;
			case 6:
				RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW6;
				break;
			case 7:
				RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW7;
				break;
			case 8:
				RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW8;
				break;
			case 9:
				RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW9;
				break;
			case 10:
				RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW10;
				break;
			case 11:
				RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW11;
				break;
			case 12:
				RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW12;
				break;
			case 13:
				RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW13;
				break;
			case 14:
				RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW14;
				break;
			case 15:
				RXDataBuff[rxdatax] = CANFDx->RxMessage[rxri].RB_DW15;
				break;
			default:
				break;
		}
	}
	for (uint8_t buffx = 0; buffx < datalenght; buffx++)
	{
		mdata = RXDataBuff[buffx] >> 24;
		*Data++ = mdata;
		mdata = RXDataBuff[buffx] >> 16;
		*Data++ = mdata;
		mdata = RXDataBuff[buffx] >> 8;
		*Data++ = mdata;
		mdata = RXDataBuff[buffx];
		*Data++ = mdata;
	}
	CANFDx->RFSR |= CANFD_RFSR_IRI;
}


/*****END OF FILE*****/ 
