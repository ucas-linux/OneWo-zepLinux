/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_i2c.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:			2024/7/17
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/7/17     1.0     build this module
 * Description:     This file contains all the functions prototypes for the I2C firmware library.
 */

#ifndef AS32X601_I2C_H
#define AS32X601_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601_i2c_regs.h"

/*
 * @brief  Use by parameter examine.
 */
#define IS_I2C_ALL_PERIPH(PERIPH)  				(((PERIPH) == I2C0) || \
									((PERIPH) == I2C1) || \
									((PERIPH) == I2C2) || \
									((PERIPH) == I2C3))

/*
 * @brief I2C init structure definition
 */
typedef struct
{
	volatile uint8_t I2C_ClockScale;			/* Clock division selection. */
	volatile uint8_t I2C_ICLKDIVL;				/* Frequency division coefficient low. */
	volatile uint8_t I2C_ICLKDIVH;				/* Frequency division coefficient high. */
	volatile uint8_t I2C_EN;					/* I2C Enable. */
	volatile uint8_t I2C_SlaveAddress;			/* Slave address. */
	volatile uint8_t I2C_Ack;					/* Ack reply */
}I2C_InitTypeDef;

/*
 * @brief I2C interrupt structure definition
 */
typedef struct
{
	volatile uint8_t ERRN;						/* Transmission error interrupt enable */
	volatile uint8_t SIEN;						/* Sequential operation interrupt enable */
	volatile uint8_t TXEN;						/* Enable idle interrupt sending */
	volatile uint8_t RXEN;						/* Receive idle interrupt enable */
}I2C_IRQTypeDef;

/*
 * @brief I2C read/write commands
 */
#define I2C_READ								((uint8_t)0x01)							/* I2C read */
#define I2C_WRITE								((uint8_t)0x00)							/* I2C write */
#define IS_I2C_COMMAND(COMMAND) 				(((COMMAND) == I2C_READ) || ((COMMAND) == I2C_WRITE))

/*
 * @brief I2C clock selection
 */
#define I2C_PCLK_256							((uint8_t)0x00)							/* PCLK frequency/256 */
#define I2C_PCLK_224							I2C_CTRL_CLK_0							/* PCLK frequency/224 */
#define I2C_PCLK_192							I2C_CTRL_CLK_1							/* PCLK frequency/192 */
#define I2C_PCLK_160							(I2C_CTRL_CLK_1 | I2C_CTRL_CLK_0)		/* PCLK frequency/160 */
#define I2C_PCLK_960							I2C_CTRL_CLK_2							/* PCLK frequency/960 */
#define I2C_PCLK_120							(I2C_CTRL_CLK_2 | I2C_CTRL_CLK_0)		/* PCLK frequency/120 */
#define I2C_PCLK_60 							(I2C_CTRL_CLK_2 | I2C_CTRL_CLK_1)		/* PCLK frequency/60 */
#define I2C_PCLK_8  							I2C_CTRL_CLK							/* PCLK frequency/8/DIV */
#define IS_I2C_CLOCK(CLOCK)     				(((CLOCK) == I2C_PCLK_256) 	|| ((CLOCK) == I2C_PCLK_224) || \
												 ((CLOCK) == I2C_PCLK_192) 	|| ((CLOCK) == I2C_PCLK_160) || \
												 ((CLOCK) == I2C_PCLK_960) 	|| ((CLOCK) == I2C_PCLK_120) || \
												 ((CLOCK) == I2C_PCLK_60) 	|| ((CLOCK) == I2C_PCLK_8))

/*
 * @brief I2C The Assert Acknowledge flag
 */
#define I2C_IICAA_NACK							((uint8_t)0x00)							/* Send NACK when receiving data */
#define I2C_IICAA_ACK							I2C_CTRL_AA								/* Send ACK when receiving data */
#define IS_I2C_IICAA_FLAG(FLAG)					(((FLAG) == I2C_IICAA_NACK) || ((FLAG) == I2C_IICAA_ACK))

/*
 * @brief I2C state
 */
#define MASTER_START_READY                   	((uint8_t)0x01)							/* A START condition has been transmitted. */
#define MASTER_START_REPEAT                  	((uint8_t)0x02)							/* A repeated START condition has been transmitted. */
#define MSEND_WADDR_ACK                      	((uint8_t)0x03)							/* Slave sending address and write signal has been completed; ACK has been received. */
#define MSEND_WADDR_NACK                     	((uint8_t)0x04)							/* Slave sending address and write signal has been completed; NACK has been received. */
#define MSEND_DATA_ACK                       	((uint8_t)0x05)							/* The data byte transmission in the data register has been completed; ACK has been received. */
#define MSEND_DATA_NACK                      	((uint8_t)0x06)							/* The data byte transmission in the data register has been completed; NACK has been received. */
#define MSEND_ARBITRATION_LOST               	((uint8_t)0x07)							/* Arbitration lost in slave sending address,slave receiving address or data bytes. */
#define MSEND_RADDR_ACK                      	((uint8_t)0x08)							/* Slave receiving address and read signal has been completed; ACK has been received. */
#define MSEND_RADDR_NACK                     	((uint8_t)0x09)							/* Slave receiving address and read signal has been completed; NACK has been received. */
#define MREAD_DATA_ACK                       	((uint8_t)0x0A)							/* The data byte reception in the data register has been completed; ACK has been received. */
#define MREAD_DATA_NACK                      	((uint8_t)0x0B)							/* The data byte reception in the data register has been completed; NACK has been received. */
#define SREV_WADDR_ACK                       	((uint8_t)0x0C)							/* The address and write signal sent by the host has been received; ACK has been returned. */
#define SWADDR_ERROR_ACK                     	((uint8_t)0x0D)							/* After the host sends the address and read/write bits, arbitration is lost, and addresses 
																								and write signals sent by other hosts are received; ACK has been returned. */
#define SREV_BROADCAST_ACK                   	((uint8_t)0x0E)							/* General call address (0x00) has been received; ACK has been returned. */
#define SADDR_ERROR_REV_BROADCAST_ACK        	((uint8_t)0x0F)							/* Lost arbitration after sending address and read/write bits as the host,
																								and received broadcast addressing sent by other hosts; ACK has been returned */
#define SREV_DATA_ACK                        	((uint8_t)0x10)							/* Previously addressed with own slave address; DATA has been received; ACK returned. */
#define SREV_DATA_NACK                       	((uint8_t)0x11)							/* Previously addressed with own slave address; DATA has been received; NACK returned. */
#define SREV_BROADCASTDATA_ACK               	((uint8_t)0x12)							/* Previously addressed with general call address; DATA has been received; ACK returned. */
#define SREV_BROADCASTDATA_NACK              	((uint8_t)0x13)							/* Previously addressed with general call address; DATA has been received; NACK returned. */
#define SREV_STOP_MESSAGE                    	((uint8_t)0x14)							/* A STOP condition or repeated START condition has been received. */
#define SREV_RADDR_ACK                       	((uint8_t)0x15)							/* Received the address and read signal sent by the host; ACK has been received. */
#define SRADDR_ERROR_ACK                     	((uint8_t)0x16)							/* Arbitration loss after sending address and read/write bits as a host,
																								and receiving address and read signals from other hosts; ACK has been returned. */
#define SSEND_DATA_ACK                       	((uint8_t)0x17)							/* Data byte has been transmitted; ACK has been received. */
#define SSEND_DATA_NACK                      	((uint8_t)0x18)							/* Data byte has been transmitted; NACK has been received. */
#define SSEND_LASTDATA_ACK                   	((uint8_t)0x19)							/* Last data byte has transmitted; ACK has received. */
#define MASTER_STOP_OVER                     	((uint8_t)0x1C)							/* A STOP Condtion has been transmitted */
#define I2C_BUS_IDLE                         	((uint8_t)0x1F)							/* Idle state */
#define IS_I2C_STATE(STATE)    					(((STATE) == MASTER_START_READY) 			|| ((STATE) == MASTER_START_REPEAT) 	|| \
												 ((STATE) == MSEND_WADDR_ACK) 				|| ((STATE) == MSEND_WADDR_NACK) 		|| \
												 ((STATE) == MSEND_DATA_ACK) 				|| ((STATE) == MSEND_DATA_NACK) 		|| \
												 ((STATE) == MSEND_ARBITRATION_LOST) 		|| ((STATE) == MSEND_RADDR_ACK)			|| \
												 ((STATE) == MSEND_RADDR_NACK) 				|| ((STATE) == MREAD_DATA_ACK) 			|| \
												 ((STATE) == MREAD_DATA_NACK) 				|| ((STATE) == SREV_WADDR_ACK) 			|| \
												 ((STATE) == SWADDR_ERROR_ACK) 				|| ((STATE) == SREV_BROADCAST_ACK) 		|| \
												 ((STATE) == SADDR_ERROR_REV_BROADCAST_ACK) || ((STATE) == SREV_DATA_ACK) 			|| \
												 ((STATE) == SREV_DATA_NACK) 				|| ((STATE) == SREV_BROADCASTDATA_ACK) 	|| \
												 ((STATE) == SREV_BROADCASTDATA_NACK) 		|| ((STATE) == SREV_STOP_MESSAGE) 		|| \
												 ((STATE) == SREV_RADDR_ACK) 				|| ((STATE) == SRADDR_ERROR_ACK) 		|| \
												 ((STATE) == SSEND_DATA_ACK) 				|| ((STATE) == SSEND_DATA_NACK) 		|| \
												 ((STATE) == SSEND_LASTDATA_ACK) 			|| ((STATE) == MASTER_STOP_OVER) 		|| \
												 ((STATE) == I2C_BUS_IDLE))

/*
 * @brief Function declaration
 */
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct);
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct);
void I2C_ITStructInit(I2C_IRQTypeDef* I2C_IRQStruct);
void I2C_ITInit(I2C_TypeDef* I2Cx, I2C_IRQTypeDef* I2C_IRQStruct);
void I2C_GenerateStart(I2C_TypeDef* I2Cx);
void I2C_GenerateStop(I2C_TypeDef* I2Cx);
void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t Direction);
void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, uint8_t Ackflag);
void I2C_StartClear(I2C_TypeDef* I2Cx);
void I2C_ClearITPendingBit(I2C_TypeDef* I2Cx);
ITStatus I2C_GetITStatus(I2C_TypeDef* I2Cx);
uint8_t I2C_GetFlagstatus(I2C_TypeDef* I2Cx);
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data);
uint8_t I2C_ReceiveData(I2C_TypeDef* I2Cx);
FlagStatus I2C_CheckStatus(I2C_TypeDef* I2Cx, uint32_t Status);

/********************************************************************
			The follow functions is used to application.
********************************************************************/
FlagStatus I2C_Master_Transmit(I2C_TypeDef* I2Cx, uint8_t DevAddress, uint8_t *pData, uint32_t Size, uint32_t Timeout);
FlagStatus I2C_Master_Receive(I2C_TypeDef* I2Cx, uint8_t DevAddress, uint8_t* pData, uint32_t Size, uint32_t Timeout);
FlagStatus I2C_Slave_Transmit(I2C_TypeDef* I2Cx, uint8_t* pData, uint32_t Size, uint32_t Timeout);
FlagStatus I2C_Slave_Receive(I2C_TypeDef* I2Cx, uint8_t* pData, uint32_t Size, uint32_t Timeout);

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
