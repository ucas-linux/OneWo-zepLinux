/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_i2c_regs.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:			2024/7/17
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/7/17     1.0     build this module
 * Description:     This file contains all the register correlation defines for the I2C firmware library.
 */

#ifndef AS32X601_I2C_REG_H
#define AS32X601_I2C_REG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601.h"
#include "as32x601_addrmap.h"

/*
 * @brief General Purpose I/O registers structure
 */
typedef struct
{
	__IO uint8_t CTLR;                      /* I2C Control Register.                    			Address offset: 0x00 */
	uint8_t RESERVED0[3];                   /* I2C Reserved,										Address offset: 0x01 - 0x03 */
	__I  uint8_t STAT;                      /* I2C Status Register.                     			Address offset: 0x04 */
	uint8_t RESERVED1[3];                   /* I2C Reserved,										Address offset: 0x05 - 0x07 */
	__IO uint8_t DATA;                      /* I2C Data Register.                    				Address offset: 0x08 */
	uint8_t RESERVED2[3];                   /* I2C Reserved,										Address offset: 0x09 - 0x0B */
	__IO uint8_t ADDR;                      /* I2C Slave Address Register.							Address offset: 0x0C */
	uint8_t RESERVED3[3];                   /* I2C Reserved,										Address offset: 0x0D - 0x0F */
	__IO uint8_t DIV_L;						/* I2C Frequency division coefficient register low.		Address offset: 0x10 */
	uint8_t RESERVED4[3];                   /* I2C Reserved,										Address offset: 0x11 - 0x13 */
	__IO uint8_t IRQC;						/* I2C Interrupt control register.						Address offset: 0x14 */
	uint8_t RESERVED5[3];                   /* I2C Reserved,										Address offset: 0x15 - 0x17 */
	__IO uint8_t DIV_H;						/* I2C Frequency division coefficient register high.	Address offset: 0x18 */
}I2C_TypeDef;

/*
 * @addtogroup I2C Peripheral declaration
 */
#define I2C0	((I2C_TypeDef*)IIC_IIS0_BASE)
#define I2C1	((I2C_TypeDef*)IIC_IIS1_BASE)
#define I2C2	((I2C_TypeDef*)IIC_IIS2_BASE)
#define I2C3	((I2C_TypeDef*)IIC_IIS3_BASE)


/*******************  Bit definition for I2C_CTLR register  *******************/
#define I2C_CTRL_IICLK_DPos					(0U)
#define I2C_CTRL_CLKPos						I2C_CTRL_IICLK_DPos
#define I2C_CTRL_CLK_Msk						(0x83UL << I2C_CTRL_CLKPos)         /* 0x00000083 */
#define I2C_CTRL_CLK                   		I2C_CTRL_CLK_Msk                     /* II[2:0] bits (Serial clock rate) */
#define I2C_CTRL_CLK_0                 		(0x1UL << I2C_CTRL_CLKPos)          /* 0x00000001 */
#define I2C_CTRL_CLK_1                 		(0x2UL << I2C_CTRL_CLKPos)          /* 0x00000002 */
#define I2C_CTRL_CLK_2                 		(0x80UL<< I2C_CTRL_CLKPos)          /* 0x00000080 */


#define I2C_CTRL_IICAA_DPos					(2U)
#define I2C_CTRL_AA_Pos                 	I2C_CTRL_IICAA_DPos
#define I2C_CTRL_AA_Msk						(0x1UL << I2C_CTRL_AA_Pos)            /* 0x00000004 */
#define I2C_CTRL_AA                  		I2C_CTRL_AA_Msk                       /* The Assert Acknowledge flag */


#define I2C_CTRL_IICI_DPos					(3U)
#define I2C_CTRL_INT_Pos					I2C_CTRL_IICI_DPos
#define I2C_CTRL_INT_Msk					(0x1UL << I2C_CTRL_INT_Pos)            /* 0x00000008 */
#define I2C_CTRL_INT						I2C_CTRL_INT_Msk                       /* The Serial Interrupt flag */

#define I2C_CTRL_IICSTO_DPos				(4U) 
#define	I2C_CTRL_STO_Pos					I2C_CTRL_IICSTO_DPos
#define I2C_CTRL_STO_Msk					(0x1UL << I2C_CTRL_STO_Pos)         	  /* 0x00000010 */
#define I2C_CTRL_STO						I2C_CTRL_STO_Msk                    	  /* The STOP flag */

#define I2C_CTRL_IICSTA_DPos				(5U)
#define I2C_CTRL_STA_Pos                    I2C_CTRL_IICSTA_DPos
#define I2C_CTRL_IICSTA_Msk					(0x1UL << I2C_CTRL_STA_Pos)         	  /* 0x00000020 */
#define I2C_CTRL_IICSTA						I2C_CTRL_IICSTA_Msk                    	  /* The START flag */

#define I2C_CTRL_IICEN_DPos					(6U)
#define I2C_CTRL_EN_Pos						I2C_CTRL_IICEN_DPos  
#define I2C_CTRL_EN_Msk						(0x1UL << I2C_CTRL_EN_Pos)          	  /* 0x00000040 */
#define I2C_CTRL_EN							I2C_CTRL_EN_Msk                     	  /* Enable bit */

/*******************  Bit definition for I2C_STAT register  *******************/
#define I2C_STAT_RXNE_DPos					(0U)
#define I2C_STAT_RXNE_Pos            		I2C_STAT_RXNE_DPos
#define I2C_STAT_RXNE_Msk					(0x1UL << I2C_STAT_RXNE_Pos)           /* 0x00000001 */
#define I2C_STAT_RXNE						I2C_STAT_RXNE_Msk                      /* Receive idle status */

#define I2C_STAT_TXE_DPos					(1U)
#define I2C_STAT_TXE_Pos					I2C_STAT_TXE_DPos
#define I2C_STAT_TXE_Msk					(0x1UL << I2C_STAT_TXE_Pos)            /* 0x00000002 */
#define I2C_STAT_TXE						I2C_STAT_TXE_Msk                       /* Send idle status */


#define I2C_STAT_ERR_DPos					(2U)
#define I2C_STAT_ERR_Pos					I2C_STAT_ERR_DPos
#define I2C_STAT_ERR_Msk					(0x1UL << I2C_STAT_TERR_Pos)            /* 0x00000004 */
#define I2C_STAT_ERR						I2C_STAT_ERR_Msk                       /* Transmission TERRor status */

#define I2C_STAT_STATE_DPos					(3U)
#define I2C_STAT_STATE_Pos					I2C_STAT_STATE_DPos
#define I2C_STAT_STATE_Msk					(0x1FUL << I2C_STAT_STATE_Pos)         /* 0x000000F8 */
#define I2C_STAT_STATE                 		I2C_STAT_STATE_Msk                     /* STATE[4:0] bits (running state) */
#define I2C_STAT_STATE_0               		(0x1UL << I2C_STAT_STATE_Pos)          /* 0x00000008 */
#define I2C_STAT_STATE_1               		(0x2UL << I2C_STAT_STATE_Pos)          /* 0x00000010 */
#define I2C_STAT_STATE_2               		(0x4UL << I2C_STAT_STATE_Pos)          /* 0x00000020 */
#define I2C_STAT_STATE_3               		(0x8UL << I2C_STAT_STATE_Pos)          /* 0x00000040 */
#define I2C_STAT_STATE_4               		(0x10UL << I2C_STAT_STATE_Pos)         /* 0x00000080 */

/*******************  Bit definition for I2C_DATA register  *******************/
#define I2C_DATA_DATA_DPos					(0U)
#define I2C_DATA_DATA_Pos					I2C_DATA_DATA_DPos
#define I2C_DATA_DATA_Msk					(0xFFUL << I2C_DATA_DATA_Pos)          /* 0x000000FF */
#define I2C_DATA_DATA  						I2C_DATA_DATA_Msk                      /* DATA[7:0] bits (Serial data) */
#define I2C_DATA_DATA_0						(0x1UL << I2C_DATA_DATA_Pos)           /* 0x00000001 */
#define I2C_DATA_DATA_1						(0x2UL << I2C_DATA_DATA_Pos)           /* 0x00000002 */
#define I2C_DATA_DATA_2						(0x4UL << I2C_DATA_DATA_Pos)           /* 0x00000004 */
#define I2C_DATA_DATA_3						(0x8UL << I2C_DATA_DATA_Pos)           /* 0x00000008 */
#define I2C_DATA_DATA_4						(0x10UL << I2C_DATA_DATA_Pos)          /* 0x00000010 */
#define I2C_DATA_DATA_5						(0x20UL << I2C_DATA_DATA_Pos)          /* 0x00000020 */
#define I2C_DATA_DATA_6						(0x40UL << I2C_DATA_DATA_Pos)          /* 0x00000040 */
#define I2C_DATA_DATA_7						(0x80UL << I2C_DATA_DATA_Pos)          /* 0x00000080 */

/*******************  Bit definition for I2C_ADDR register  *******************/
#define I2C_ADDR_ADDR_DPos					(1U)
#define I2C_ADDR_ADDR_Pos                	I2C_ADDR_ADDR_DPos
#define I2C_ADDR_ADDR_Msk					(0x7FUL << I2C_ADDR_ADDR_Pos)          /* 0x000000FE */
#define I2C_ADDR_ADDR  						I2C_ADDR_ADDR_Msk                      /* ADDR[6:0] bits (Slave Address) */
#define I2C_ADDR_ADDR_0						(0x1UL << I2C_ADDR_ADDR_Pos)           /* 0x00000002 */
#define I2C_ADDR_ADDR_1						(0x2UL << I2C_ADDR_ADDR_Pos)           /* 0x00000004 */
#define I2C_ADDR_ADDR_2						(0x4UL << I2C_ADDR_ADDR_Pos)           /* 0x00000008 */
#define I2C_ADDR_ADDR_3						(0x8UL << I2C_ADDR_ADDR_Pos)           /* 0x00000010 */
#define I2C_ADDR_ADDR_4						(0x10UL << I2C_ADDR_ADDR_Pos)          /* 0x00000020 */
#define I2C_ADDR_ADDR_5						(0x20UL << I2C_ADDR_ADDR_Pos)          /* 0x00000040 */
#define I2C_ADDR_ADDR_6						(0x40UL << I2C_ADDR_ADDR_Pos)          /* 0x00000080 */

/******************  Bit definition for I2C_DIV_L register  *******************/
#define I2C_DIV_L_DIV_DPos					(0U)
#define I2C_DIV_L_DIV_Pos					I2C_DIV_L_DIV_DPos
#define I2C_DIV_L_DIV_Msk					(0xFFUL << I2C_DIV_L_DIV_Pos)          /* 0x000000FF */
#define I2C_DIV_L_DIV  						I2C_DIV_L_DIV_Msk                      /* DIV[7:0] bits (division factor) */
#define I2C_DIV_L_DIV_0						(0x1UL << I2C_DIV_L_DIV_Pos)           /* 0x00000001 */
#define I2C_DIV_L_DIV_1						(0x2UL << I2C_DIV_L_DIV_Pos)           /* 0x00000002 */
#define I2C_DIV_L_DIV_2						(0x4UL << I2C_DIV_L_DIV_Pos)           /* 0x00000004 */
#define I2C_DIV_L_DIV_3						(0x8UL << I2C_DIV_L_DIV_Pos)           /* 0x00000008 */
#define I2C_DIV_L_DIV_4						(0x10UL << I2C_DIV_L_DIV_Pos)          /* 0x00000010 */
#define I2C_DIV_L_DIV_5						(0x20UL << I2C_DIV_L_DIV_Pos)          /* 0x00000020 */
#define I2C_DIV_L_DIV_6						(0x40UL << I2C_DIV_L_DIV_Pos)          /* 0x00000040 */
#define I2C_DIV_L_DIV_7						(0x80UL << I2C_DIV_L_DIV_Pos)          /* 0x00000080 */

/*******************  Bit definition for I2C_IRQC register  *******************/
#define I2C_IRQC_RXNEN_DPos					(0U)
#define I2C_IRQC_RXNEN_Pos					I2C_IRQC_RXNEN_DPos
#define I2C_IRQC_RXNEN_Msk					(0x1UL << I2C_IRQC_RXNEN_Pos)          	 /* 0x00000001 */
#define I2C_IRQC_RXNEN                  	I2C_IRQC_RXNEN_Msk                      /* Receive idle interrupt enable */

#define I2C_IRQC_TXEN_DPos					(1U)
#define I2C_IRQC_TXEN_Pos              		I2C_IRQC_TXEN_DPos
#define I2C_IRQC_TXEN_Msk					(0x1UL << I2C_IRQC_TXEN_Pos)           /* 0x00000002 */
#define I2C_IRQC_TXEN						I2C_IRQC_TXEN_Msk                      /* Send idle interrupt enable */

#define I2C_IRQC_SIEN_DPos					(2U)
#define I2C_IRQC_SIEN_Pos               	I2C_IRQC_SIEN_DPos
#define I2C_IRQC_SIEN_Msk					(0x1UL << I2C_IRQC_SIEN_Pos)           /* 0x00000004 */
#define I2C_IRQC_SIEN						I2C_IRQC_SIEN_Msk                      /* Sequential operation interrupt enable */

#define I2C_IRQC_ERRN_DPos					(3U)
#define I2C_IRQC_ERRN_Pos               	I2C_IRQC_ERRN_DPos
#define I2C_IRQC_ERRN_Msk					(0x1UL << I2C_IRQC_ERRN_Pos)           	/* 0x00000008 */
#define I2C_IRQC_ERRN						I2C_IRQC_ERRN_Msk                      /* Transmission error interrupt enable */



/******************  Bit definition for I2C_DIV_H register  *******************/
#define I2C_DIV_H_DIV_DPos				(0U)
#define I2C_DIV_H_DIV_Pos               I2C_DIV_H_DIV_DPos
#define I2C_DIV_H_DIV_Msk				(0xFFUL << I2C_DIV_H_DIV_Pos)          /* 0x000000FF */
#define I2C_DIV_H_DIV  					I2C_DIV_H_DIV_Msk                      /* DIV[7:0] bits (division factor) */
#define I2C_DIV_H_DIV_0					(0x1UL << I2C_DIV_H_DIV_Pos)           /* 0x00000001 */
#define I2C_DIV_H_DIV_1					(0x2UL << I2C_DIV_H_DIV_Pos)           /* 0x00000002 */
#define I2C_DIV_H_DIV_2					(0x4UL << I2C_DIV_H_DIV_Pos)           /* 0x00000004 */
#define I2C_DIV_H_DIV_3					(0x8UL << I2C_DIV_H_DIV_Pos)           /* 0x00000008 */
#define I2C_DIV_H_DIV_4					(0x10UL << I2C_DIV_H_DIV_Pos)          /* 0x00000010 */
#define I2C_DIV_H_DIV_5					(0x20UL << I2C_DIV_H_DIV_Pos)          /* 0x00000020 */
#define I2C_DIV_H_DIV_6					(0x40UL << I2C_DIV_H_DIV_Pos)          /* 0x00000040 */
#define I2C_DIV_H_DIV_7					(0x80UL << I2C_DIV_H_DIV_Pos)          /* 0x00000080 */

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
