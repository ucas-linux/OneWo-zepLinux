/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_i2c.c
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:			2024/7/17
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/7/17     1.0     build this module
 * Description:     This file provides firmware functions to manage the functionalities of the TIMER peripheral.
 */

#include "as32x601_i2c.h"

/* Private function prototypes -----------------------------------------------*/
static void I2C_Delay_us(uint32_t Delayus);

/*
 * Function:        I2C_StructInit
 * Description:     Fills each I2C_InitStruct member with its default value.
 * Param:           I2C_InitStruct: pointer to a I2C_InitTypeDef structure which will be initialized.
 * Return:          None.
 */
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct)
{
    I2C_InitStruct->I2C_ClockScale = I2C_PCLK_256;
    I2C_InitStruct->I2C_EN = DISABLE;
    I2C_InitStruct->I2C_SlaveAddress = 0x00;
    I2C_InitStruct->I2C_Ack = DISABLE;
}

/*
 * Function:        I2C_Init
 * Description:     Initializes the I2Cx peripheral according to the specified parameters in the I2C_InitStruct.
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 *                  I2C_InitStruct: pointer to a I2C_InitTypeDef structure that contains the configuration .
 *					information for the specified I2C peripheral.
 * Return:          None
 */
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct)
{
    uint8_t tmpreg = 0x00;

    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_I2C_CLOCK(I2C_InitStruct->I2C_ClockScale));
    assert_param(IS_EnState_STATE(I2C_InitStruct->I2C_EN));
    assert_param(IS_I2C_IICAA_FLAG(I2C_InitStruct->I2C_Ack));
    assert_param(I2C_InitStruct->I2C_SlaveAddress <= 0x7FU);

    tmpreg = I2Cx->CTLR;

    /* Clear Interrupt Bits by Sequential Operation */
    tmpreg &= ~((uint8_t)(I2C_CTRL_INT));

    /* Set clock selection */
    tmpreg &= ~((uint8_t)(I2C_CTRL_CLK));
    tmpreg |= ((uint8_t)I2C_InitStruct->I2C_ClockScale);

    /* Set I2C enable */
    tmpreg &= ~((uint8_t)(I2C_CTRL_EN));
    tmpreg |= ((uint8_t)I2C_InitStruct->I2C_EN << I2C_CTRL_EN_Pos);

    /* Set Assert Acknowledge flag */
    tmpreg &= ~((uint8_t)(I2C_CTRL_AA));
    tmpreg |= ((uint8_t)I2C_InitStruct->I2C_Ack);

    I2Cx->CTLR = tmpreg;

    if (I2C_InitStruct->I2C_ClockScale == I2C_PCLK_8)
    {
        /* Check the parameters */
        assert_param(I2C_InitStruct->I2C_ICLKDIVL <= 0xFFU);
        assert_param(I2C_InitStruct->I2C_ICLKDIVH <= 0xFFU);

        /* Set frequency division coefficient */
        I2Cx->DIV_L = I2C_InitStruct->I2C_ICLKDIVL;
        I2Cx->DIV_H = I2C_InitStruct->I2C_ICLKDIVH;
    }

    /* Set Slave address. */
    I2Cx->ADDR = ((uint8_t)(I2C_InitStruct->I2C_SlaveAddress << 1U));
}

/*
 * Function:        I2C_ITStructInit
 * Description:     Fills each I2C_IRQStruct member with its default value.
 * param:           I2C_IRQStruct: pointer to a I2C_IRQTypeDef structure which will be initialized.
 * Return:          None.
 */
void I2C_ITStructInit(I2C_IRQTypeDef* I2C_IRQStruct)
{
    I2C_IRQStruct->ERRN = DISABLE;
    I2C_IRQStruct->SIEN = DISABLE;
    I2C_IRQStruct->TXEN = DISABLE;
    I2C_IRQStruct->RXEN = DISABLE;
}

/*
 * Function:        I2C_ITInit
 * Description:     Initializes the I2Cx interrupt according to the specified parameters in the I2C_InitStruct.
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 *                  I2C_IRQStruct: pointer to a I2C_IRQStruct structure that contains the configuration information for the specified I2C interrupt.
 * Return:          None.
 */
void I2C_ITInit(I2C_TypeDef* I2Cx, I2C_IRQTypeDef* I2C_IRQStruct)
{
    uint8_t tmpreg = 0x00;

    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_EnState_STATE(I2C_IRQStruct->ERRN));
	assert_param(IS_EnState_STATE(I2C_IRQStruct->SIEN));
	assert_param(IS_EnState_STATE(I2C_IRQStruct->TXEN));
	assert_param(IS_EnState_STATE(I2C_IRQStruct->RXEN));

    tmpreg = I2Cx->IRQC;

    /* Set transmission error interrupt */
    tmpreg &= ~((uint8_t)(I2C_IRQC_ERRN));
    tmpreg |= ((uint8_t)(I2C_IRQStruct->ERRN << I2C_IRQC_ERRN_Pos));

	/* Set sequential operation interrupt */
    tmpreg &= ~((uint8_t)(I2C_IRQC_SIEN));
    tmpreg |= ((uint8_t)(I2C_IRQStruct->SIEN << I2C_IRQC_SIEN_Pos));

	/* Set send idle interrupt */
    tmpreg &= ~((uint8_t)(I2C_IRQC_TXEN));
    tmpreg |= ((uint8_t)(I2C_IRQStruct->TXEN << I2C_IRQC_TXEN_Pos));

	/* Set receive idle interrupt */
    tmpreg &= ~((uint8_t)(I2C_IRQC_RXNEN));
    tmpreg |= ((uint8_t)(I2C_IRQStruct->RXEN << I2C_IRQC_RXNEN_Pos));

    I2Cx->IRQC = tmpreg;
}


/*
 * Function:        I2C_GenerateStart
 * Description:     I2C transmission start signal.
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 * Return:          None
 */
void I2C_GenerateStart(I2C_TypeDef* I2Cx)
{
    uint8_t tmpreg = 0x00;

	/* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    tmpreg = I2Cx->CTLR;

    tmpreg |= ((uint8_t)I2C_CTRL_IICSTA);

    I2Cx->CTLR = tmpreg;
}

/*
 * Function:        I2C_GenerateStop
 * Description:     I2C transmission stop signal.
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 * Return:          None
 */
void I2C_GenerateStop(I2C_TypeDef* I2Cx)
{
    uint8_t tmpreg = 0x00;

	/* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    tmpreg = I2Cx->CTLR;

    tmpreg |= ((uint8_t)I2C_CTRL_STO);

    I2Cx->CTLR = tmpreg;
}

/*
 * Function:        I2C_Send7bitAddress
 * Description:     Set the device 7 bits address value.
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 * 					Address: The device 7 bits address value.
 * 					Direction: Reading and writing direction.
 *					 This parameter can be one of the following values:
 * 						I2C_READ: I2C Read.
 * 						I2C_WRITE: I2C Write.
 * Return:          None.
 */
void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t Direction)
{
	/* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(Address <= 0x7FU);
	assert_param(IS_I2C_COMMAND(Direction));

    I2Cx->DATA = ((uint8_t)(Direction | (Address << 1U)));
}

/*
 * Function:        I2C_AcknowledgeConfig
 * Description:     Set I2C reply.
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 * 					ackflag: Reply to ack or reply to nack.
 *					 This parameter can be one of the following values:
 *						I2C_IICAA_NACK: Send NACK when receiving data.
 *						I2C_IICAA_ACK: Send ACK when receiving data.
 * Return:          None.
 */
void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, uint8_t Ackflag)
{
	uint8_t tmpreg = 0x00;

	/* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_I2C_IICAA_FLAG(Ackflag));

	tmpreg = I2Cx->CTLR;

	/* Set Assert Acknowledge flag */
    tmpreg &= ~((uint8_t)(I2C_CTRL_AA));
    tmpreg |= ((uint8_t)Ackflag);

    I2Cx->CTLR = tmpreg;
}

/*
 * Function:        I2C_StartClear
 * Description:     I2C clear start signal.
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 * Return:          None.
 */
void I2C_StartClear(I2C_TypeDef* I2Cx)
{
    uint8_t tmpreg = 0x00;

	/* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    tmpreg = I2Cx->CTLR;

    tmpreg &= ~((uint8_t)(I2C_CTRL_IICSTA));

    I2Cx->CTLR = tmpreg;
}

/*
 * Function:        I2C_ClearITPendingBit
 * Description:     Clear the Serial Interrupt flag.
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 * Return:          None.
 */
void I2C_ClearITPendingBit(I2C_TypeDef* I2Cx)
{
    uint8_t tmpreg = 0x00;

	/* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    tmpreg = I2Cx->CTLR;

    tmpreg &= ~((uint8_t)(I2C_CTRL_INT));

    I2Cx->CTLR = tmpreg;
}

/*
 * Function:        I2C_GetITStatus
 * Description:     Get the Serial Interrupt flag.
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 * Return:          The Serial Interrupt flag (SET or RESET).
 */
ITStatus I2C_GetITStatus(I2C_TypeDef* I2Cx)
{
	ITStatus bitstatus = RESET;

	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    if ((I2Cx->CTLR & (I2C_CTRL_INT)) != (uint8_t)RESET)
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
 * Function:        I2C_GetFlagstatus
 * Description:     Get I2C status.
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 * Return:          I2C status.
 */
uint8_t I2C_GetFlagstatus(I2C_TypeDef* I2Cx)
{
    uint8_t tmpstat = 0x00;

	/* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    tmpstat = I2Cx->STAT;

    return ((uint8_t)(tmpstat >> I2C_STAT_STATE_Pos));
}

/*
 * Function:        I2C_SendData
 * Description:     Write data
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 * 					Data: Set input data.
 * Return:          None.
 */
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data)
{
	/* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(Data <= 0xFFU);

    I2Cx->DATA = Data;
}

/*
 * Function:        I2C_DataGet
 * Description:     Read single byte data.
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 * Return:          Single byte data.
 */
uint8_t I2C_ReceiveData(I2C_TypeDef* I2Cx)
{
    uint8_t value = 0x00;

	/* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    value = I2Cx->DATA;

    return value;
}

/*
 * Function:        I2C_CheckStatus
 * Description:     Determine the status of i2c.
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 * 					Status: Select the state to be judged.
 *					 This parameter can be one of the following values:
 *						MASTER_START_READY.
 *					  	MASTER_START_REPEAT.
 *					  	MSEND_WADDR_ACK.
 *					  	MSEND_WADDR_NACK.
 *					  	MSEND_DATA_ACK.
 *					  	MSEND_DATA_NACK.
 *					  	MSEND_ARBITRATION_LOST.
 *					  	MSEND_RADDR_ACK.
 *					  	MSEND_RADDR_NACK.
 *					  	MREAD_DATA_ACK.
 *					  	MREAD_DATA_NACK.
 *					  	SREV_WADDR_ACK.
 *					  	SWADDR_ERROR_ACK.
 *					  	SREV_BROADCAST_ACK.
 *					  	SADDR_ERROR_REV_BROADCAST_ACK.
 *					  	SREV_DATA_ACK.
 *					  	SREV_DATA_NACK.
 *					  	SREV_BROADCASTDATA_ACK.
 *					  	SREV_BROADCASTDATA_NACK.
 *					  	SREV_STOP_MESSAGE.
 *					  	SREV_RADDR_ACK.
 *					  	SRADDR_ERROR_ACK.
 *					  	SSEND_DATA_ACK.
 *					  	SSEND_DATA_NACK.
 *					  	SSEND_LASTDATA_ACK.
 *					  	MASTER_STOP_OVER.
 *					  	I2C_BUS_IDLE.
 * Return:          The state of I2C_status (SET or RESET).
 */
FlagStatus I2C_CheckStatus(I2C_TypeDef* I2Cx, uint32_t Status)
{
	FlagStatus bitstatus = RESET;

	/* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_I2C_STATE(Status));

    if (I2C_GetFlagstatus(I2Cx) == Status)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

	return bitstatus;
}

/********************************************************************
			The follow functions is used to application.
********************************************************************/
 
/*
 * Function:        I2C_Master_Transmit
 * Description:     Transmits in master mode an amount of data.
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 * 					DevAddress: The device 7 bits address value.
 * 					pData: Pointer to data buffer.
 * 					Size: Amount of data to be sent.
 * 					Timeout: Timeout duration.
 * Return:          Sent state (SET or RESET).
 */
FlagStatus I2C_Master_Transmit(I2C_TypeDef* I2Cx, uint8_t DevAddress, uint8_t *pData, uint32_t Size, uint32_t Timeout)
{
	uint8_t num = 0x00;

	/* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(DevAddress <= 0x7FU);
	assert_param(pData != 0);
	assert_param(Size > 0);
	assert_param(Timeout > 0);

	/* Waiting for bus release */
	while (!I2C_CheckStatus(I2Cx, I2C_BUS_IDLE))
	{
		I2C_StartClear(I2Cx);
		I2C_GenerateStop(I2Cx);
		I2C_ClearITPendingBit(I2Cx);
		if ((Timeout--) == 0)
		{
			return RESET;
		}
		I2C_Delay_us(500);
	}
	I2C_GenerateStart(I2Cx);

	/* Waiting for the start signal to complete */
	while (!I2C_CheckStatus(I2Cx, MASTER_START_READY))
	{
		if ((Timeout--) == 0)
		{
            I2C_StartClear(I2Cx);
            I2C_GenerateStop(I2Cx);
            I2C_ClearITPendingBit(I2Cx);
			return RESET;
		}
		I2C_Delay_us(500);
	}
	/* Write address and clear IICI bit */
	I2C_Send7bitAddress(I2Cx, DevAddress, I2C_WRITE);
	I2C_StartClear(I2Cx); 
	I2C_ClearITPendingBit(I2Cx);

	/* Waiting for the slave to receive the completed address and send an ack */
	while (!I2C_CheckStatus(I2Cx, MSEND_WADDR_ACK))
	{
		if ((Timeout--) == 0)
		{
            I2C_StartClear(I2Cx);
			I2C_GenerateStop(I2Cx);
			I2C_ClearITPendingBit(I2Cx);
			return RESET;
		}
		I2C_Delay_us(500);
	}
	for (num = 0; num < Size; num++)
	{
		I2C_SendData(I2Cx, pData[num]);
		I2C_ClearITPendingBit(I2Cx);
		/* Wait for the slave to receive the completed data and send an ack */
		while (!I2C_CheckStatus(I2Cx, MSEND_DATA_ACK))
		{
			if ((Timeout--) == 0)
			{
                I2C_StartClear(I2Cx);
				I2C_GenerateStop(I2Cx);
				I2C_ClearITPendingBit(I2Cx);
				return RESET;
			}
			I2C_Delay_us(500);
		}
	}
	/* Send stop signal and clear IICI bit */
	I2C_GenerateStop(I2Cx);
	I2C_ClearITPendingBit(I2Cx);
	return SET;
}

/*
 * Function:        I2C_Master_Receive
 * Description:     Receive in master mode an amount of data.
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 * 					DevAddress: The device 7 bits address value.
 * 					pData: Pointer to data buffer.
 * 					Size: Amount of data to be receive.
 * 					Timeout: Timeout duration.
 * Return:          Receive state (SET or RESET).
 */
FlagStatus I2C_Master_Receive(I2C_TypeDef* I2Cx, uint8_t DevAddress, uint8_t* pData, uint32_t Size, uint32_t Timeout)
{
	uint32_t num = 0x00;

	/* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(DevAddress <= 0x7FU);
	assert_param(Size > 0);
	assert_param(Timeout > 0);

	/* Waiting for bus release */
	while (!I2C_CheckStatus(I2Cx, I2C_BUS_IDLE))
	{
		I2C_StartClear(I2Cx);
		I2C_GenerateStop(I2Cx);
		I2C_ClearITPendingBit(I2Cx);
		if ((Timeout--) == 0)
		{
			return RESET;
		}
		I2C_Delay_us(1000);
	}
	I2C_AcknowledgeConfig(I2Cx, I2C_IICAA_ACK);
	I2C_GenerateStart(I2Cx);

	/* Waiting for the start signal to complete */
	while (!I2C_CheckStatus(I2Cx, MASTER_START_READY))
	{
		if ((Timeout--) == 0)
		{
            I2C_StartClear(I2Cx);
            I2C_GenerateStop(I2Cx);
            I2C_ClearITPendingBit(I2Cx);
			return RESET;
		}
		I2C_Delay_us(1000);
	}
	I2C_Send7bitAddress(I2Cx, DevAddress, I2C_READ);
	I2C_ClearITPendingBit(I2Cx);

	/* Waiting for the slave to receive the completed address and send an ack */
	while (!I2C_CheckStatus(I2Cx, MSEND_RADDR_ACK))
	{
		if ((Timeout--) == 0)
		{
            I2C_StartClear(I2Cx);
			I2C_GenerateStop(I2Cx);
			I2C_ClearITPendingBit(I2Cx);
			return RESET;
		}
		I2C_Delay_us(1000);
	}

	for (num = 0; num < Size; num++)
	{
		if (num == (Size - 1))
		{
			/* IIC sends NACK */
			I2C_AcknowledgeConfig(I2Cx, I2C_IICAA_NACK);
		}
		else
		{
			I2C_AcknowledgeConfig(I2Cx, I2C_IICAA_ACK);
		}

		I2C_StartClear(I2Cx);
		I2C_ClearITPendingBit(I2Cx);

		/* Wait for the slave to send the completed data, and the host will send an ack */
		while (!(I2C_CheckStatus(I2Cx, MREAD_DATA_ACK) || I2C_CheckStatus(I2Cx, MREAD_DATA_NACK)))
		{
			if ((Timeout--) == 0)
			{
                I2C_StartClear(I2Cx);
				I2C_GenerateStop(I2Cx);
				I2C_ClearITPendingBit(I2Cx);
				return RESET;
			}
			I2C_Delay_us(1000);
		}
		*pData++ = I2C_ReceiveData(I2Cx);
	}

	/* Send stop signal and clear IICI bit */
    I2C_StartClear(I2Cx);
	I2C_GenerateStop(I2Cx);
	I2C_ClearITPendingBit(I2Cx);
	return SET;
}

/*
 * Function:        I2C_Slave_Transmit
 * Description:     Transmits in slave mode an amount of data.
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 * 					pData: Pointer to data buffer.
 * 					Size: Amount of data to be sent.
 * 					Timeout: Timeout duration.
 * Return:          Sent state (SET or RESET).
 */
FlagStatus I2C_Slave_Transmit(I2C_TypeDef* I2Cx, uint8_t* pData, uint32_t Size, uint32_t Timeout)
{
	uint32_t num = 0x00;

	/* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(pData != 0);
	assert_param(Size > 0);
	assert_param(Timeout > 0);

	/* Waiting for bus release */
	while (!I2C_CheckStatus(I2Cx, I2C_BUS_IDLE))
	{
		I2C_StartClear(I2Cx);
		I2C_GenerateStop(I2Cx);
		I2C_ClearITPendingBit(I2Cx);
		if ((Timeout--) == 0)
		{
			return RESET;
		}
		I2C_Delay_us(500);
	}
	I2C_AcknowledgeConfig(I2Cx, I2C_IICAA_ACK);

	/* Waiting for address reception judgment to be completed */
	while (!I2C_CheckStatus(I2Cx, SREV_RADDR_ACK))
	{
		if ((Timeout--) == 0)
		{
			return RESET;
		}
		I2C_Delay_us(500);
	}

	for (num = 0; num < Size; num++)
	{
		I2C_SendData(I2Cx, pData[num]);

		if (num == (Size - 1))
		{
			/* IIC sends NACK */
			I2C_AcknowledgeConfig(I2Cx, I2C_IICAA_NACK);
		}
		else
		{
			I2C_AcknowledgeConfig(I2Cx, I2C_IICAA_ACK);
		}
		I2C_ClearITPendingBit(I2Cx);

		/* Wait for the master to receive the completed data and send an ack */
		while (!(I2C_CheckStatus(I2Cx, SSEND_DATA_ACK) || I2C_CheckStatus(I2Cx, SSEND_DATA_NACK)))
		{
			if ((Timeout--) == 0)
			{
				I2C_GenerateStop(I2Cx);
				I2C_ClearITPendingBit(I2Cx);
				return RESET;
			}
			I2C_Delay_us(500);
		}
	}

	I2C_ClearITPendingBit(I2Cx);
	return SET;
}

/*
 * Function:        I2C_Slave_Receive
 * Description:     Receive in slave mode an amount of data.
 * Param:           I2Cx: where x can be (0..3) to select the I2C peripheral.
 * 					pData: Pointer to data buffer.
 * 					Size: Amount of data to be receive.
 * 					Timeout: Timeout duration.
 * Return:          Receive state (SET or RESET).
 */
FlagStatus I2C_Slave_Receive(I2C_TypeDef* I2Cx, uint8_t* pData, uint32_t Size, uint32_t Timeout)
{
	uint32_t num = 0x00;

	/* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(Size > 0);
	assert_param(Timeout > 0);

	/* Waiting for bus release */
	while (!I2C_CheckStatus(I2Cx, I2C_BUS_IDLE))
	{
		I2C_StartClear(I2Cx);
		I2C_GenerateStop(I2Cx);
		I2C_ClearITPendingBit(I2Cx);
		if ((Timeout--) == 0)
		{
			return RESET;
		}
		I2C_Delay_us(1000);
	}

	/* Waiting for address reception judgment to be completed */
	while (!I2C_CheckStatus(I2Cx, SREV_WADDR_ACK))
	{
		if ((Timeout--) == 0)
		{
			return RESET;
		}
		I2C_Delay_us(1000);
	}

	for (num = 0; num < Size; num++)
	{
		/* IIC sends ACK */
		I2C_AcknowledgeConfig(I2Cx, I2C_IICAA_ACK);

		I2C_ClearITPendingBit(I2Cx);
		/* Wait for the master to send the completed data, and the slave to send the ack */
		while (!(I2C_CheckStatus(I2Cx, SREV_DATA_ACK) || I2C_CheckStatus(I2Cx, SREV_STOP_MESSAGE)))
		{
			if ((Timeout--) == 0)
			{
				I2C_GenerateStop(I2Cx);
				I2C_ClearITPendingBit(I2Cx);
				return RESET;
			}
			I2C_Delay_us(1000);
		}
		*pData++ = I2C_ReceiveData(I2Cx);	
	}
	/* IIC sends NACK */
	I2C_AcknowledgeConfig(I2Cx, I2C_IICAA_NACK);
	I2C_GenerateStop(I2Cx);
	I2C_ClearITPendingBit(I2Cx);
	return SET;
}

/*
 * Function:        I2C_Delay_us
 * Description:     I2C initialization delay.
 * param:           Delayus: delay duration.
 * Return:          None,
 */
static void I2C_Delay_us(uint32_t Delayus)
{
    int i = 0U;
    /* Set this value based on the CPU clock (MHz)*/
    const int count = 40U;

    /* Check the parameters */
    assert_param(Delayus > 0);

    for (i = 0; i < count * Delayus; i++)
    {
        __asm volatile("nop");
    }
}

/*****END OF FILE*****/
