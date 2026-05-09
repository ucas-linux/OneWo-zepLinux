/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_usart_reg.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:			2024/08/01
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/08/01     1.0     build this module
 * Description:     This file contains all the register correlation defines for the USART firmware library.
 */

#ifndef AS32X601_USART_REGS_H
#define AS32X601_USART_REGS_H

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
	__IO uint32_t CR1;      /* USART_CTRL1  USART control register 1,			    Address offset: 0x00 */
	__IO uint32_t CR2;      /* USART_CTRL2  USART control register 2,			    Address offset: 0x04 */
	__IO uint32_t CR3;      /* USART_CTRL3  USART control register 3,			    Address offset: 0x08 */
	__IO uint32_t BRR;      /* USART_BRR    USART baud rate register,			    Address offset: 0x0C */
	uint32_t RESERVED[2];   /* USART Reserved,						                Address offset: 0x10 - 0x14 */
	__O  uint32_t RQR;      /* USART_RQR    USART request register,				    Address offset: 0x18 */
	__I  uint32_t ISR;      /* USART_ISR    USART interrupt and status register,	Address offset: 0x1C */
	__O  uint32_t ICR;      /* USART_ICR    USART interrupt flag clear register,	Address offset: 0x20 */
	__I  uint32_t RDR;      /* USART_RDR    USART receive data register,			Address offset: 0x24 */
	__IO uint32_t TDR;      /* USART_TDR    USART transmit data register,		    Address offset: 0x28 */
} USART_TypeDef;

/*
 * @addtogroup USART Peripheral declaration
 */
#define USART0	((USART_TypeDef*)USART0_BASE)
#define USART1	((USART_TypeDef*)USART1_BASE)
#define USART2	((USART_TypeDef*)USART2_BASE)
#define USART3	((USART_TypeDef*)USART3_BASE)
#define USART4	((USART_TypeDef*)USART4_BASE)
#define USART5	((USART_TypeDef*)USART5_BASE)
#define USART6	((USART_TypeDef*)USART6_BASE)
#define USART7	((USART_TypeDef*)USART7_BASE)

/******************  Bit definition for USART_CR1 register  *******************/
#define USART_CR1_UE_DPos               (0U)
#define USART_CR1_UE_Pos                USART_CR1_UE_DPos
#define USART_CR1_UE_Msk                (0x1UL << USART_CR1_UE_Pos)             /* 0x00000001 */
#define USART_CR1_UE                    USART_CR1_UE_Msk                        /* USART Enable */

#define USART_CR1_RE_DPos               (2U)
#define USART_CR1_RE_Pos                USART_CR1_RE_DPos
#define USART_CR1_RE_Msk                (0x1UL << USART_CR1_RE_Pos)             /* 0x00000004 */
#define USART_CR1_RE                    USART_CR1_RE_Msk                        /* Receiver Enable */

#define USART_CR1_TE_DPos               (3U)
#define USART_CR1_TE_Pos                USART_CR1_TE_DPos
#define USART_CR1_TE_Msk                (0x1UL << USART_CR1_TE_Pos)             /* 0x00000008 */
#define USART_CR1_TE                    USART_CR1_TE_Msk                        /* Transmitter Enable */

#define USART_CR1_IDLEIE_DPos           (4U)
#define USART_CR1_IDLEIE_Pos            USART_CR1_IDLEIE_DPos
#define USART_CR1_IDLEIE_Msk            (0x1U << USART_CR1_IDLEIE_Pos)          /* 0x00000010 */
#define USART_CR1_IDLEIE                USART_CR1_IDLEIE_Msk                    /* IDLE Interrupt Enable */

#define USART_CR1_RXNEIE_RXFNEIE_DPos   (5U)
#define USART_CR1_RXNEIE_RXFNEIE_Pos    USART_CR1_RXNEIE_RXFNEIE_DPos
#define USART_CR1_RXNEIE_RXFNEIE_Msk    (0x1UL << USART_CR1_RXNEIE_RXFNEIE_Pos) /* 0x00000020 */
#define USART_CR1_RXNEIE_RXFNEIE        USART_CR1_RXNEIE_RXFNEIE_Msk            /* RXNE and RX FIFO Not Empty Interrupt Enable */

#define USART_CR1_TCIE_DPos             (6U)
#define USART_CR1_TCIE_Pos              USART_CR1_TCIE_DPos
#define USART_CR1_TCIE_Msk              (0x1UL << USART_CR1_TCIE_Pos)           /* 0x00000040 */
#define USART_CR1_TCIE                  USART_CR1_TCIE_Msk                      /* Transmission Complete Interrupt Enable */

#define USART_CR1_TXEIE_TXFNFIE_DPos    (7U)
#define USART_CR1_TXEIE_TXFNFIE_Pos     USART_CR1_TXEIE_TXFNFIE_DPos
#define USART_CR1_TXEIE_TXFNFIE_Msk     (0x1UL << USART_CR1_TXEIE_TXFNFIE_Pos)  /* 0x00000080 */
#define USART_CR1_TXEIE_TXFNFIE         USART_CR1_TXEIE_TXFNFIE_Msk             /* TXE and TX FIFO Not Full Interrupt Enable */

#define USART_CR1_PEIE_DPos             (8U)
#define USART_CR1_PEIE_Pos              USART_CR1_PEIE_DPos
#define USART_CR1_PEIE_Msk              (0x1UL << USART_CR1_PEIE_Pos)           /* 0x00000100 */
#define USART_CR1_PEIE                  USART_CR1_PEIE_Msk                      /* PE Interrupt Enable */

#define USART_CR1_PS_DPos               (9U)
#define USART_CR1_PS_Pos                USART_CR1_PS_DPos
#define USART_CR1_PS_Msk                (0x1UL << USART_CR1_PS_Pos)             /* 0x00000200 */
#define USART_CR1_PS                    USART_CR1_PS_Msk                        /* Parity Selection */

#define USART_CR1_PCE_DPos              (10U)
#define USART_CR1_PCE_Pos               USART_CR1_PCE_DPos
#define USART_CR1_PCE_Msk               (0x1UL << USART_CR1_PCE_Pos)            /* 0x00000400 */
#define USART_CR1_PCE                   USART_CR1_PCE_Msk                       /* Parity Control Enable */

#define USART_CR1_M_DPos                (12U)
#define USART_CR1_M_Pos                 USART_CR1_M_DPos
#define USART_CR1_M_Msk                 (0x10001UL << USART_CR1_M_Pos)          /* 0x10001000 */
#define USART_CR1_M                     USART_CR1_M_Msk                         /* Word length */

#define USART_CR1_M0_DPos               (12U)
#define USART_CR1_M0_Pos                USART_CR1_M0_DPos
#define USART_CR1_M0_Msk                (0x1UL << USART_CR1_M0_Pos)             /* 0x00001000 */
#define USART_CR1_M0                    USART_CR1_M0_Msk                        /* Word length - Bit 0 */

#define USART_CR1_OVER8_DPos            (15U)
#define USART_CR1_OVER8_Pos             USART_CR1_OVER8_DPos
#define USART_CR1_OVER8_Msk             (0x1UL << USART_CR1_OVER8_Pos)          /* 0x00008000 */
#define USART_CR1_OVER8                 USART_CR1_OVER8_Msk                     /* Oversampling by 8-bit or 16-bit mode */

#define USART_CR1_M1_DPos               (28U)
#define USART_CR1_M1_Pos                USART_CR1_M1_DPos
#define USART_CR1_M1_Msk                (0x1UL << USART_CR1_M1_Pos)             /* 0x10000000 */
#define USART_CR1_M1                    USART_CR1_M1_Msk                        /* Word length - Bit 1 */

#define USART_CR1_FIFOEN_DPos           (29U)
#define USART_CR1_FIFOEN_Pos            USART_CR1_FIFOEN_DPos
#define USART_CR1_FIFOEN_Msk            (0x1UL << USART_CR1_FIFOEN_Pos)         /* 0x20000000 */
#define USART_CR1_FIFOEN                USART_CR1_FIFOEN_Msk                    /* FIFO mode enable */

#define USART_CR1_TXFEIE_DPos           (30U)
#define USART_CR1_TXFEIE_Pos            USART_CR1_TXFEIE_DPos
#define USART_CR1_TXFEIE_Msk            (0x1UL << USART_CR1_TXFEIE_Pos)         /* 0x40000000 */
#define USART_CR1_TXFEIE                USART_CR1_TXFEIE_Msk                    /* TXFIFO empty interrupt enable */

#define USART_CR1_RXFFIE_DPos           (31U)
#define USART_CR1_RXFFIE_Pos            USART_CR1_RXFFIE_DPos
#define USART_CR1_RXFFIE_Msk            (0x1UL << USART_CR1_RXFFIE_Pos)         /* 0x80000000 */
#define USART_CR1_RXFFIE                USART_CR1_RXFFIE_Msk                    /* RXFIFO Full interrupt enable */

/* Legacy define */
#define  USART_CR1_RXNEIE  USART_CR1_RXNEIE_RXFNEIE
#define  USART_CR1_TXEIE   USART_CR1_TXEIE_TXFNFIE

/******************  Bit definition for USART_CR2 register  *******************/
#define USART_CR2_SLVEN_DPos            (0U)
#define USART_CR2_SLVEN_Pos             USART_CR2_SLVEN_DPos
#define USART_CR2_SLVEN_Msk             (0x1UL << USART_CR2_SLVEN_Pos)          /* 0x00000001 */
#define USART_CR2_SLVEN                 USART_CR2_SLVEN_Msk                     /* Synchronous Slave mode Enable */

#define USART_CR2_BKNUM_DPos            (1U)
#define USART_CR2_BKNUM_Pos             USART_CR2_BKNUM_DPos
#define USART_CR2_BKNUM_Msk             (0x3UL << USART_CR2_BKNUM_Pos)          /* 0x00000006 */
#define USART_CR2_BKNUM                 USART_CR2_BKNUM_Msk                     /* BKNUM[1:0] bits (Send the length of Break characters) */
#define USART_CR2_BKNUM_0               (0x1UL << USART_CR2_BKNUM_Pos)          /* 0x00000002 */
#define USART_CR2_BKNUM_1               (0x2UL << USART_CR2_BKNUM_Pos)          /* 0x00000004 */

#define USART_CR2_DIS_NSS_DPos          (3U)
#define USART_CR2_DIS_NSS_Pos           USART_CR2_DIS_NSS_DPos
#define USART_CR2_DIS_NSS_Msk           (0x1UL << USART_CR2_DIS_NSS_Pos)        /* 0x00000008 */
#define USART_CR2_DIS_NSS               USART_CR2_DIS_NSS_Msk                   /* Negative Slave Select (NSS) pin management */

#define USART_CR2_LBDIE_DPos            (6U)
#define USART_CR2_LBDIE_Pos             USART_CR2_LBDIE_DPos
#define USART_CR2_LBDIE_Msk             (0x1UL << USART_CR2_LBDIE_Pos)          /* 0x00000040 */
#define USART_CR2_LBDIE                 USART_CR2_LBDIE_Msk                     /* LIN Break Detection Interrupt Enable */

#define USART_CR2_LBCL_DPos             (8U)
#define USART_CR2_LBCL_Pos              USART_CR2_LBCL_DPos
#define USART_CR2_LBCL_Msk              (0x1U << USART_CR2_LBCL_Pos)            /* 0x00000100 */
#define USART_CR2_LBCL                  USART_CR2_LBCL_Msk                      /* Last Bit Clock pulse */

#define USART_CR2_CPHA_DPos             (9U)
#define USART_CR2_CPHA_Pos              USART_CR2_CPHA_DPos
#define USART_CR2_CPHA_Msk              (0x1UL << USART_CR2_CPHA_Pos)           /* 0x00000200 */
#define USART_CR2_CPHA                  USART_CR2_CPHA_Msk                      /* Clock Phase */

#define USART_CR2_CPOL_DPos             (10U)
#define USART_CR2_CPOL_Pos              USART_CR2_CPOL_DPos
#define USART_CR2_CPOL_Msk              (0x1UL << USART_CR2_CPOL_Pos)           /* 0x00000400 */
#define USART_CR2_CPOL                  USART_CR2_CPOL_Msk                      /* Clock Polarity */

#define USART_CR2_CLKEN_DPos            (11U)
#define USART_CR2_CLKEN_Pos             USART_CR2_CLKEN_DPos
#define USART_CR2_CLKEN_Msk             (0x1UL << USART_CR2_CLKEN_Pos)          /* 0x00000800 */
#define USART_CR2_CLKEN                 USART_CR2_CLKEN_Msk                     /* Clock Enable */

#define USART_CR2_STOP_DPos             (12U)
#define USART_CR2_STOP_Pos              USART_CR2_STOP_DPos
#define USART_CR2_STOP_Msk              (0x3UL << USART_CR2_STOP_Pos)           /* 0x00003000 */
#define USART_CR2_STOP                  USART_CR2_STOP_Msk                      /* STOP[1:0] bits (STOP bits) */
#define USART_CR2_STOP_0                (0x1UL << USART_CR2_STOP_Pos)           /* 0x00001000 */
#define USART_CR2_STOP_1                (0x2UL << USART_CR2_STOP_Pos)           /* 0x00002000 */

#define USART_CR2_LINEN_DPos            (14U)
#define USART_CR2_LINEN_Pos             USART_CR2_LINEN_DPos
#define USART_CR2_LINEN_Msk             (0x1UL << USART_CR2_LINEN_Pos)          /* 0x00004000 */
#define USART_CR2_LINEN                 USART_CR2_LINEN_Msk                     /* LIN mode enable */

#define USART_CR2_RXINV_DPos            (16U)
#define USART_CR2_RXINV_Pos             USART_CR2_RXINV_DPos
#define USART_CR2_RXINV_Msk             (0x1U << USART_CR2_RXINV_Pos)           /* 0x00010000 */
#define USART_CR2_RXINV                 USART_CR2_RXINV_Msk                     /* RX pin active level inversion */

#define USART_CR2_TXINV_DPos            (17U)
#define USART_CR2_TXINV_Pos             USART_CR2_TXINV_DPos
#define USART_CR2_TXINV_Msk             (0x1U << USART_CR2_TXINV_Pos)           /* 0x00020000 */
#define USART_CR2_TXINV                 USART_CR2_TXINV_Msk                     /* TX pin active level inversion */

#define USART_CR2_DATAINV_DPos          (18U)
#define USART_CR2_DATAINV_Pos           USART_CR2_DATAINV_DPos
#define USART_CR2_DATAINV_Msk           (0x1U << USART_CR2_DATAINV_Pos)         /* 0x00040000 */
#define USART_CR2_DATAINV               USART_CR2_DATAINV_Msk                   /* Binary data inversion */

#define USART_CR2_MSBFIRST_DPos         (19U)
#define USART_CR2_MSBFIRST_Pos          USART_CR2_MSBFIRST_DPos
#define USART_CR2_MSBFIRST_Msk          (0x1UL << USART_CR2_MSBFIRST_Pos)       /* 0x00080000 */
#define USART_CR2_MSBFIRST              USART_CR2_MSBFIRST_Msk                  /* Most Significant Bit First */

#define USART_CR2_ABREN_DPos            (20U)
#define USART_CR2_ABREN_Pos             USART_CR2_ABREN_DPos
#define USART_CR2_ABREN_Msk             (0x1UL << USART_CR2_ABREN_Pos)          /* 0x00100000 */
#define USART_CR2_ABREN                 USART_CR2_ABREN_Msk                     /* Auto Baud-Rate Enable*/

#define USART_CR2_ABRMOD_DPos           (21U)
#define USART_CR2_ABRMODE_Pos           USART_CR2_ABRMOD_DPos
#define USART_CR2_ABRMODE_Msk           (0x3U << USART_CR2_ABRMODE_Pos)         /* 0x00600000 */
#define USART_CR2_ABRMODE               USART_CR2_ABRMODE_Msk                   /* ABRMOD[1:0] bits (Auto Baud-Rate Mode) */
#define USART_CR2_ABRMODE_0             (0x1U << USART_CR2_ABRMODE_Pos)         /* 0x00200000 */
#define USART_CR2_ABRMODE_1             (0x2U << USART_CR2_ABRMODE_Pos)         /* 0x00400000 */


/******************  Bit definition for USART_CR3 register  *******************/
#define USART_CR3_EIE_DPos              (0U)
#define USART_CR3_EIE_Pos               USART_CR3_EIE_DPos
#define USART_CR3_EIE_Msk               (0x1UL << USART_CR3_EIE_Pos)            /* 0x00000001 */
#define USART_CR3_EIE                   USART_CR3_EIE_Msk                       /* Error Interrupt Enable */

#define USART_CR3_HDSEL_DPos            (1U)
#define USART_CR3_HDSEL_Pos             USART_CR3_HDSEL_DPos
#define USART_CR3_HDSEL_Msk             (0x1U << USART_CR3_HDSEL_Pos)           /* 0x00000002 */
#define USART_CR3_HDSEL                 USART_CR3_HDSEL_Msk                     /* Half-Duplex Selection */

#define USART_CR3_DMAR_DPos             (4U)
#define USART_CR3_DMAR_Pos              USART_CR3_DMAR_DPos
#define USART_CR3_DMAR_Msk              (0x1U << USART_CR3_DMAR_Pos)            /* 0x00000010 */
#define USART_CR3_DMAR                  USART_CR3_DMAR_Msk                      /* DMA Enable Receiver */

#define USART_CR3_DMAT_DPos             (5U)
#define USART_CR3_DMAT_Pos              USART_CR3_DMAT_DPos
#define USART_CR3_DMAT_Msk              (0x1U << USART_CR3_DMAT_Pos)            /* 0x00000020 */
#define USART_CR3_DMAT                  USART_CR3_DMAT_Msk                      /* DMA Enable Transmitter */

#define USART_CR3_RTSE_DPos             (6U)
#define USART_CR3_RTSE_Pos              USART_CR3_RTSE_DPos
#define USART_CR3_RTSE_Msk              (0x1UL << USART_CR3_RTSE_Pos)           /* 0x00000040 */
#define USART_CR3_RTSE                  USART_CR3_RTSE_Msk                      /* RTS Enable */

#define USART_CR3_CTSE_DPos             (7U)
#define USART_CR3_CTSE_Pos              USART_CR3_CTSE_DPos
#define USART_CR3_CTSE_Msk              (0x1UL << USART_CR3_CTSE_Pos)           /* 0x00000080 */
#define USART_CR3_CTSE                  USART_CR3_CTSE_Msk                      /* CTS Enable */

#define USART_CR3_CTSIE_DPos            (8U)
#define USART_CR3_CTSIE_Pos             USART_CR3_CTSIE_DPos
#define USART_CR3_CTSIE_Msk             (0x1UL << USART_CR3_CTSIE_Pos)          /* 0x00000100 */
#define USART_CR3_CTSIE                 USART_CR3_CTSIE_Msk                     /* CTS Interrupt Enable */

#define USART_CR3_ONEBIT_DPos           (9U)
#define USART_CR3_ONEBIT_Pos            USART_CR3_ONEBIT_DPos
#define USART_CR3_ONEBIT_Msk            (0x1U << USART_CR3_ONEBIT_Pos)          /* 0x00000200 */
#define USART_CR3_ONEBIT                USART_CR3_ONEBIT_Msk                    /* One sample bit method enable */

#define USART_CR3_OVRDIS_DPos           (10U)
#define USART_CR3_OVRDIS_Pos            USART_CR3_OVRDIS_DPos
#define USART_CR3_OVRDIS_Msk            (0x1U << USART_CR3_OVRDIS_Pos)          /* 0x00000400 */
#define USART_CR3_OVRDIS                USART_CR3_OVRDIS_Msk                    /* Overrun Disable */

#define USART_CR3_TXFTIE_DPos           (20U)
#define USART_CR3_TXFTIE_Pos            USART_CR3_TXFTIE_DPos
#define USART_CR3_TXFTIE_Msk            (0x1UL << USART_CR3_TXFTIE_Pos)         /* 0x00100000 */
#define USART_CR3_TXFTIE                USART_CR3_TXFTIE_Msk                    /* TXFIFO threshold interrupt enable */

#define USART_CR3_RXFTCFG_DPos          (22U)
#define USART_CR3_RXFTCFG_Pos           USART_CR3_RXFTCFG_DPos
#define USART_CR3_RXFTCFG_Msk           (0x7UL << USART_CR3_RXFTCFG_Pos)        /* 0x01C00000 */
#define USART_CR3_RXFTCFG               USART_CR3_RXFTCFG_Msk                   /* RXFTCFG [2:0]Receive FIFO threshold configuration */
#define USART_CR3_RXFTCFG_0             (0x1UL << USART_CR3_RXFTCFG_Pos)        /* 0x00400000 */
#define USART_CR3_RXFTCFG_1             (0x2UL << USART_CR3_RXFTCFG_Pos)        /* 0x00800000 */
#define USART_CR3_RXFTCFG_2             (0x4UL << USART_CR3_RXFTCFG_Pos)        /* 0x01000000 */

#define USART_CR3_RXFTIE_DPos           (25U)
#define USART_CR3_RXFTIE_Pos            USART_CR3_RXFTIE_DPos
#define USART_CR3_RXFTIE_Msk            (0x1UL << USART_CR3_RXFTIE_Pos)         /* 0x02000000 */
#define USART_CR3_RXFTIE                USART_CR3_RXFTIE_Msk                    /* RXFIFO threshold interrupt enable */

#define USART_CR3_TXFTCFG_DPos          (26U)
#define USART_CR3_TXFTCFG_Pos           USART_CR3_TXFTCFG_DPos
#define USART_CR3_TXFTCFG_Msk           (0x7UL << USART_CR3_TXFTCFG_Pos)        /* 0x1C000000 */
#define USART_CR3_TXFTCFG               USART_CR3_TXFTCFG_Msk                   /* TXFIFO [2:0] threshold configuration */
#define USART_CR3_TXFTCFG_0             (0x1UL << USART_CR3_TXFTCFG_Pos)        /* 0x04000000 */
#define USART_CR3_TXFTCFG_1             (0x2UL << USART_CR3_TXFTCFG_Pos)        /* 0x08000000 */
#define USART_CR3_TXFTCFG_2             (0x4UL << USART_CR3_TXFTCFG_Pos)        /* 0x10000000 */


/******************  Bit definition for USART_BRR register  *******************/
#define USART_BRR_BRR_DPos              (0U)
#define USART_BRR_DIV_Pos               USART_BRR_BRR_DPos
#define USART_BRR_DIV_Msk               (0xFFFFUL << USART_BRR_DIV_Pos)         /* 0x0000FFFF */
#define USART_BRR_DIV                   USART_BRR_DIV_Msk                       /* USARTDIV */

/*******************  Bit definition for USART_RQR register  ******************/
#define USART_RQR_ABRRQ_DPos            (0U)
#define USART_RQR_ABRRQ_Pos             USART_RQR_ABRRQ_DPos
#define USART_RQR_ABRRQ_Msk             (0x1U << USART_RQR_ABRRQ_Pos)           /* 0x00000001 */
#define USART_RQR_ABRRQ                 USART_RQR_ABRRQ_Msk                     /* Auto-Baud Rate Request */

#define USART_RQR_SBKRQ_DPos            (1U)
#define USART_RQR_SBKRQ_Pos             USART_RQR_SBKRQ_DPos
#define USART_RQR_SBKRQ_Msk             (0x1UL << USART_RQR_SBKRQ_Pos)          /* 0x00000002 */
#define USART_RQR_SBKRQ                 USART_RQR_SBKRQ_Msk                     /* Send Break Request */

#define USART_RQR_RXFRQ_DPos            (3U)
#define USART_RQR_RXFRQ_Pos             USART_RQR_RXFRQ_DPos
#define USART_RQR_RXFRQ_Msk             (0x1UL << USART_RQR_RXFRQ_Pos)          /* 0x00000008 */
#define USART_RQR_RXFRQ                 USART_RQR_RXFRQ_Msk                     /* Receive Data flush Request */

#define USART_RQR_TXFRQ_DPos            (4U)
#define USART_RQR_TXFRQ_Pos             USART_RQR_TXFRQ_DPos
#define USART_RQR_TXFRQ_Msk             (0x1UL << USART_RQR_TXFRQ_Pos)          /* 0x00000010 */
#define USART_RQR_TXFRQ                 USART_RQR_TXFRQ_Msk                     /* Transmit data flush Request */

/*******************  Bit definition for USART_ISR register  ******************/
#define USART_ISR_PE_DPos               (0U)
#define USART_ISR_PE_Pos                USART_ISR_PE_DPos
#define USART_ISR_PE_Msk                (0x1UL << USART_ISR_PE_Pos)             /* 0x00000001 */
#define USART_ISR_PE                    USART_ISR_PE_Msk                        /* Parity Error */

#define USART_ISR_FE_DPos               (1U)
#define USART_ISR_FE_Pos                USART_ISR_FE_DPos
#define USART_ISR_FE_Msk                (0x1UL << USART_ISR_FE_Pos)             /* 0x00000002 */
#define USART_ISR_FE                    USART_ISR_FE_Msk                        /* Framing Error */

#define USART_ISR_NE_DPos               (2U)
#define USART_ISR_NE_Pos                USART_ISR_NE_DPos
#define USART_ISR_NE_Msk                (0x1UL << USART_ISR_NE_Pos)             /* 0x00000004 */
#define USART_ISR_NE                    USART_ISR_NE_Msk                        /* Noise detected Flag */

#define USART_ISR_ORE_DPos              (3U)
#define USART_ISR_ORE_Pos               USART_ISR_ORE_DPos
#define USART_ISR_ORE_Msk               (0x1UL << USART_ISR_ORE_Pos)            /* 0x00000008 */
#define USART_ISR_ORE                   USART_ISR_ORE_Msk                       /* OverRun Error */

#define USART_ISR_IDLE_DPos             (4U)
#define USART_ISR_IDLE_Pos              USART_ISR_IDLE_DPos
#define USART_ISR_IDLE_Msk              (0x1UL << USART_ISR_IDLE_Pos)           /* 0x00000010 */
#define USART_ISR_IDLE                  USART_ISR_IDLE_Msk                      /* IDLE line detected */

#define USART_ISR_RXNE_DPos             (5U)
#define USART_ISR_RXNE_RXFNE_Pos        USART_ISR_RXNE_DPos
#define USART_ISR_RXNE_RXFNE_Msk        (0x1UL << USART_ISR_RXNE_RXFNE_Pos)     /* 0x00000020 */
#define USART_ISR_RXNE_RXFNE            USART_ISR_RXNE_RXFNE_Msk                /* Read Data Register or RX FIFO Not Empty */

#define USART_ISR_TC_DPos               (6U)
#define USART_ISR_TC_Pos                USART_ISR_TC_DPos
#define USART_ISR_TC_Msk                (0x1UL << USART_ISR_TC_Pos)             /* 0x00000040 */
#define USART_ISR_TC                    USART_ISR_TC_Msk                        /* Transmission Complete */

#define USART_ISR_TXE_DPos              (7U)
#define USART_ISR_TXE_TXFNF_Pos         USART_ISR_TXE_DPos
#define USART_ISR_TXE_TXFNF_Msk         (0x1UL << USART_ISR_TXE_TXFNF_Pos)      /* 0x00000080 */
#define USART_ISR_TXE_TXFNF             USART_ISR_TXE_TXFNF_Msk                 /* Transmit Data Register Empty or TX FIFO Not Full Flag */

#define USART_ISR_LBDF_DPos             (8U)
#define USART_ISR_LBDF_Pos              USART_ISR_LBDF_DPos
#define USART_ISR_LBDF_Msk              (0x1UL << USART_ISR_LBDF_Pos)           /* 0x00000100 */
#define USART_ISR_LBDF                  USART_ISR_LBDF_Msk                      /* LIN Break Detection Flag */

#define USART_ISR_CTSIF_DPos            (9U)
#define USART_ISR_CTSIF_Pos             USART_ISR_CTSIF_DPos
#define USART_ISR_CTSIF_Msk             (0x1UL << USART_ISR_CTSIF_Pos)          /* 0x00000200 */
#define USART_ISR_CTSIF                 USART_ISR_CTSIF_Msk                     /* CTS interrupt flag */

#define USART_ISR_CTS_DPos              (10U)
#define USART_ISR_CTS_Pos               USART_ISR_CTS_DPos
#define USART_ISR_CTS_Msk               (0x1UL << USART_ISR_CTS_Pos)            /* 0x00000400 */
#define USART_ISR_CTS                   USART_ISR_CTS_Msk                       /* CTS flag */

#define USART_ISR_ABRE_DPos             (14U)
#define USART_ISR_ABRE_Pos              USART_ISR_ABRE_DPos
#define USART_ISR_ABRE_Msk              (0x1U << USART_ISR_ABRE_Pos)            /* 0x00004000 */
#define USART_ISR_ABRE                  USART_ISR_ABRE_Msk                      /* Auto-Baud Rate Error */

#define USART_ISR_ABRF_DPos             (15U)
#define USART_ISR_ABRF_Pos              USART_ISR_ABRF_DPos
#define USART_ISR_ABRF_Msk              (0x1U << USART_ISR_ABRF_Pos)            /* 0x00008000 */
#define USART_ISR_ABRF                  USART_ISR_ABRF_Msk                      /* Auto-Baud Rate Flag */

#define USART_ISR_BUSY_DPos             (16U)
#define USART_ISR_BUSY_Pos              USART_ISR_BUSY_DPos
#define USART_ISR_BUSY_Msk              (0x1UL << USART_ISR_BUSY_Pos)           /* 0x00010000 */
#define USART_ISR_BUSY                  USART_ISR_BUSY_Msk                      /* Busy Flag */

#define USART_ISR_SBKF_DPos             (18U)
#define USART_ISR_SBKF_Pos              USART_ISR_SBKF_DPos
#define USART_ISR_SBKF_Msk              (0x1U << USART_ISR_SBKF_Pos)            /* 0x00040000 */
#define USART_ISR_SBKF                  USART_ISR_SBKF_Msk                      /* Send Break Flag */

#define USART_ISR_TXFE_DPos             (23U)
#define USART_ISR_TXFE_Pos              USART_ISR_TXFE_DPos
#define USART_ISR_TXFE_Msk              (0x1UL << USART_ISR_TXFE_Pos)           /* 0x00800000 */
#define USART_ISR_TXFE                  USART_ISR_TXFE_Msk                      /* TXFIFO Empty */

#define USART_ISR_RXFF_DPos             (24U)
#define USART_ISR_RXFF_Pos              USART_ISR_RXFF_DPos
#define USART_ISR_RXFF_Msk              (0x1UL << USART_ISR_RXFF_Pos)           /* 0x01000000 */
#define USART_ISR_RXFF                  USART_ISR_RXFF_Msk                      /* RXFIFO Full Flag */

#define USART_ISR_RXFT_DPos             (26U)
#define USART_ISR_RXFT_Pos              USART_ISR_RXFT_DPos
#define USART_ISR_RXFT_Msk              (0x1UL << USART_ISR_RXFT_Pos)           /* 0x04000000 */
#define USART_ISR_RXFT                  USART_ISR_RXFT_Msk                      /* RXFIFO threshold Flag */

#define USART_ISR_TXFT_DPos             (27U)
#define USART_ISR_TXFT_Pos              USART_ISR_TXFT_DPos
#define USART_ISR_TXFT_Msk              (0x1UL << USART_ISR_TXFT_Pos)           /* 0x08000000 */
#define USART_ISR_TXFT                  USART_ISR_TXFT_Msk                      /* TXFIFO threshold Flag */


/*******************  Bit definition for USART_ICR register  ******************/
#define USART_ICR_PECF_DPos             (0U)
#define USART_ICR_PECF_Pos              USART_ICR_PECF_DPos
#define USART_ICR_PECF_Msk              (0x1UL << USART_ICR_PECF_Pos)           /* 0x00000001 */
#define USART_ICR_PECF                  USART_ICR_PECF_Msk                      /* Parity Error Clear Flag */

#define USART_ICR_FECF_DPos             (1U)
#define USART_ICR_FECF_Pos              USART_ICR_FECF_DPos
#define USART_ICR_FECF_Msk              (0x1UL << USART_ICR_FECF_Pos)           /* 0x00000002 */
#define USART_ICR_FECF                  USART_ICR_FECF_Msk                      /* Framing Error Clear Flag */

#define USART_ICR_NECF_DPos             (2U)
#define USART_ICR_NECF_Pos              USART_ICR_NECF_DPos
#define USART_ICR_NECF_Msk              (0x1UL << USART_ICR_NECF_Pos)           /* 0x00000004 */
#define USART_ICR_NECF                  USART_ICR_NECF_Msk                      /* Noise detected Clear Flag */

#define USART_ICR_ORECF_DPos            (3U)
#define USART_ICR_ORECF_Pos             USART_ICR_ORECF_DPos
#define USART_ICR_ORECF_Msk             (0x1UL << USART_ICR_ORECF_Pos)          /* 0x00000008 */
#define USART_ICR_ORECF                 USART_ICR_ORECF_Msk                     /* OverRun Error Clear Flag */

#define USART_ICR_IDLECF_DPos           (4U)
#define USART_ICR_IDLECF_Pos            USART_ICR_IDLECF_DPos
#define USART_ICR_IDLECF_Msk            (0x1UL << USART_ICR_IDLECF_Pos)         /* 0x00000010 */
#define USART_ICR_IDLECF                USART_ICR_IDLECF_Msk                    /* IDLE line detected Clear Flag */

#define USART_ICR_TXFECF_DPos           (5U)
#define USART_ICR_TXFECF_Pos            USART_ICR_TXFECF_DPos
#define USART_ICR_TXFECF_Msk            (0x1UL << USART_ICR_TXFECF_Pos)         /* 0x00000020 */
#define USART_ICR_TXFECF                USART_ICR_TXFECF_Msk                    /* TXFIFO empty clear flag */

#define USART_ICR_TCCF_DPos             (6U)
#define USART_ICR_TCCF_Pos              USART_ICR_TCCF_DPos
#define USART_ICR_TCCF_Msk              (0x1UL << USART_ICR_TCCF_Pos)           /* 0x00000040 */
#define USART_ICR_TCCF                  USART_ICR_TCCF_Msk                      /* Transmission Complete Clear Flag */

#define USART_ICR_LBDCF_DPos            (8U)
#define USART_ICR_LBDCF_Pos             USART_ICR_LBDCF_DPos
#define USART_ICR_LBDCF_Msk             (0x1UL << USART_ICR_LBDCF_Pos)          /* 0x00000100 */
#define USART_ICR_LBDCF                 USART_ICR_LBDCF_Msk                     /* LIN Break Detection Clear Flag */

#define USART_ICR_CTSCF_DPos            (9U)
#define USART_ICR_CTSCF_Pos             USART_ICR_CTSCF_DPos
#define USART_ICR_CTSCF_Msk             (0x1UL << USART_ICR_CTSCF_Pos)          /* 0x00000200 */
#define USART_ICR_CTSCF                 USART_ICR_CTSCF_Msk                     /* CTS Interrupt Clear Flag */

#define USART_ICR_RXFTCF_DPos           (26U)
#define USART_ICR_RXFTCF_Pos            USART_ICR_RXFTCF_DPos
#define USART_ICR_RXFTCF_Msk            (0x1UL << USART_ICR_RXFTCF_Pos)         /* 0x04000000 */
#define USART_ICR_RXFTCF                USART_ICR_RXFTCF_Msk                    /* Clear RXFIFO threshold interrupt flag */

#define USART_ICR_TXFTCF_DPos           (27U)
#define USART_ICR_TXFTCF_Pos            USART_ICR_TXFTCF_DPos
#define USART_ICR_TXFTCF_Msk            (0x1UL << USART_ICR_TXFTCF_Pos)         /* 0x08000000 */
#define USART_ICR_TXFTCF                USART_ICR_TXFTCF_Msk                    /* Clear TXFIFO threshold interrupt flag */


/*******************  Bit definition for USART_RDR register  ******************/
#define USART_RDR_RDR_DPos              (0U)
#define USART_RDR_RDR_Pos               USART_RDR_RDR_DPos
#define USART_RDR_RDR_Msk               (0x1FFUL << USART_RDR_RDR_Pos)          /* 0x000001FF */
#define USART_RDR_RDR                   USART_RDR_RDR_Msk                       /* RDR[8:0] bits (Receive Data value) */

/*******************  Bit definition for USART_TDR register  ******************/
#define USART_TDR_TDR_DPos              (0U)
#define USART_TDR_TDR_Pos               USART_TDR_TDR_DPos
#define USART_TDR_TDR_Msk               (0x1FFUL << USART_TDR_TDR_Pos)          /* 0x000001FF */
#define USART_TDR_TDR                   USART_TDR_TDR_Msk                       /* TDR[8:0] bits (Transmit Data value) */

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
