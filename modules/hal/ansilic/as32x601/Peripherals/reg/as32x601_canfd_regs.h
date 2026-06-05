/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_canfd_reg.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:			2024/07/30
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/07/30     1.0     build this module
 * Description:     This file contains all the register correlation defines for the CANFD firmware library.
 */

#ifndef AS32X601_CANFD_REGS_H
#define AS32X601_CANFD_REGS_H

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
	__IO uint32_t TB_ID;	/* TB*-ID Register.  	Address offset: 0x100, 0x148 ... */
	__IO uint32_t TB_DLC;	/* TB*-DLC Register. 	Address offset: 0x104, 0x14C ... */
	__IO uint32_t TB_DW0;	/* TB*-DW0 Register. 	Address offset: 0x108, 0x150 ... */
	__IO uint32_t TB_DW1;	/* TB*-DW1 Register. 	Address offset: 0x10C, 0x154 ... */
	__IO uint32_t TB_DW2;	/* TB*-DW2 Register. 	Address offset: 0x110, 0x158 ... */
	__IO uint32_t TB_DW3;	/* TB*-DW3 Register. 	Address offset: 0x114, 0x15C ... */
	__IO uint32_t TB_DW4;	/* TB*-DW4 Register. 	Address offset: 0x118, 0x160 ... */
	__IO uint32_t TB_DW5;	/* TB*-DW5 Register. 	Address offset: 0x11C, 0x164 ... */
	__IO uint32_t TB_DW6;	/* TB*-DW6 Register. 	Address offset: 0x120, 0x168 ... */
	__IO uint32_t TB_DW7;	/* TB*-DW7 Register. 	Address offset: 0x124, 0x16C ... */
	__IO uint32_t TB_DW8;	/* TB*-DW8 Register. 	Address offset: 0x128, 0x170 ... */
	__IO uint32_t TB_DW9;	/* TB*-DW9 Register. 	Address offset: 0x12C, 0x174 ... */
	__IO uint32_t TB_DW10;	/* TB*-DW10 Register.	Address offset: 0x130, 0x178 ... */
	__IO uint32_t TB_DW11;	/* TB*-DW11 Register.	Address offset: 0x134, 0x17C ... */
	__IO uint32_t TB_DW12;	/* TB*-DW12 Register.	Address offset: 0x138, 0x180 ... */
	__IO uint32_t TB_DW13;	/* TB*-DW13 Register.	Address offset: 0x13C, 0x184 ... */
	__IO uint32_t TB_DW14;	/* TB*-DW14 Register.	Address offset: 0x140, 0x188 ... */
	__IO uint32_t TB_DW15;	/* TB*-DW15 Register.	Address offset: 0x144, 0x18C ... */
} CANFD_TxMessageTypeDef;

/*
 * @brief General Purpose I/O registers structure
 */
typedef struct
{
	__IO uint32_t AFMR;	/* Acceptance Filter Mask registers.	Address offset: 0xA00, 0xA08 ... */
	__IO uint32_t AFIR;	/* Acceptance Filter ID registers.		Address offset: 0xA04, 0xA0C ... */
} CANFD_RxfilterTypeDef;


/*
 * @brief General Purpose I/O registers structure
 */
typedef struct
{
	__I  uint32_t RB_ID;	/* RB*-ID Register.  	Address offset: 0x2100, 0x2148 ... */
	__I  uint32_t RB_DLC;	/* RB*-DLC Register. 	Address offset: 0x2104, 0x214C ... */
	__I  uint32_t RB_DW0;	/* RB*-DW0 Register. 	Address offset: 0x2108, 0x2150 ... */
	__I  uint32_t RB_DW1;	/* RB*-DW1 Register. 	Address offset: 0x210C, 0x2154 ... */
	__I  uint32_t RB_DW2;	/* RB*-DW2 Register. 	Address offset: 0x2110, 0x2158 ... */
	__I  uint32_t RB_DW3;	/* RB*-DW3 Register. 	Address offset: 0x2114, 0x215C ... */
	__I  uint32_t RB_DW4;	/* RB*-DW4 Register. 	Address offset: 0x2118, 0x2160 ... */
	__I  uint32_t RB_DW5;	/* RB*-DW5 Register. 	Address offset: 0x211C, 0x2164 ... */
	__I  uint32_t RB_DW6;	/* RB*-DW6 Register. 	Address offset: 0x2120, 0x2168 ... */
	__I  uint32_t RB_DW7;	/* RB*-DW7 Register. 	Address offset: 0x2124, 0x216C ... */
	__I  uint32_t RB_DW8;	/* RB*-DW8 Register. 	Address offset: 0x2128, 0x2170 ... */
	__I  uint32_t RB_DW9;	/* RB*-DW9 Register. 	Address offset: 0x212C, 0x2174 ... */
	__I  uint32_t RB_DW10;	/* RB*-DW10 Register.	Address offset: 0x2130, 0x2178 ... */
	__I  uint32_t RB_DW11;	/* RB*-DW11 Register.	Address offset: 0x2134, 0x217C ... */
	__I  uint32_t RB_DW12;	/* RB*-DW12 Register.	Address offset: 0x2138, 0x2180 ... */
	__I  uint32_t RB_DW13;	/* RB*-DW13 Register.	Address offset: 0x213C, 0x2184 ... */
	__I  uint32_t RB_DW14;	/* RB*-DW14 Register.	Address offset: 0x2140, 0x2188 ... */
	__I  uint32_t RB_DW15;	/* RB*-DW15 Register.	Address offset: 0x2144, 0x218C ... */
} CANFD_RxMessageTypeDef;

/*
 * @brief General Purpose I/O registers structure
 */
typedef struct
{
	__IO uint32_t 		SRR;						/* Software Reset Register.															Address offset: 0x00 */
	__IO uint32_t 		MSR;						/* Mode Select Register.   															Address offset: 0x04 */
	__IO uint32_t 		APBRPR;						/* Arbitration Phase (Nominal) Baud Rate Prescaler Register.						Address offset: 0x08 */
	__IO uint32_t 		APBTR;						/* Arbitration Phase (Nominal) Bit Timing Register.           						Address offset: 0x0C */
	__I  uint32_t 		ECR;						/* Error Count Register.                											Address offset: 0x10 */
	__IO uint32_t 		ESR;						/* Error Status Register.               											Address offset: 0x14 */
	__I  uint32_t 		SR;							/* Status Register.                  												Address offset: 0x18 */
	__I  uint32_t 		ISR;						/* Interrupt Status Register.           											Address offset: 0x1C */
	__IO uint32_t 		IER;						/* Interrupt Enable Register.           											Address offset: 0x20 */
	__O  uint32_t 		ICR;						/* Interrupt Clear Register.            											Address offset: 0x24 */
	__IO uint32_t 		TSR;						/* Timestamp Register.                  											Address offset: 0x28 */
	uint32_t            RESERVED0[23];				/* Reserved.                  														Address offset: 0x2C - 0x84 */
	__IO uint32_t 		DPBRPR;						/* Data Phase Baud Rate Prescaler Register.                  						Address offset: 0x88 */
	__IO uint32_t 		DPBTR;						/* Data Phase Bit Timing Register.      											Address offset: 0x8C */
	__IO uint32_t 		TRR;						/* TX Buffer Ready Request Register.    											Address offset: 0x90 */
	__IO uint32_t 		ERRS;						/* Interrupt Enable TX Buffer Ready Request Served/Cleared.     					Address offset: 0x94 */
	__IO uint32_t 		TCR;						/* TX Buffer Cancel Request.          												Address offset: 0x98 */
	__IO uint32_t 		ECRS;						/* Interrupt Enable TX Buffer Cancellation Served/Cleared.     						Address offset: 0x9C */
	uint32_t            RESERVED6;					/* Reserved.                  														Address offset: 0xA0 */
	uint32_t            RESERVED1[15];				/* Reserved.                  														Address offset: 0xA4 - 0xDC */
	__IO uint32_t 		AFR;						/* Acceptance Filter (Control) Register.                  							Address offset: 0xE0 */
	uint32_t            RESERVED2;					/* Reserved.                  														Address offset: 0xE4 */
	__IO uint32_t 		RFSR;						/* RX FIFO Status Register.            												Address offset: 0xE8 */
	uint32_t              		RESERVED3[5];		/* Reserved.                  														Address offset: 0xEC - 0xFC */
	CANFD_TxMessageTypeDef		TxMessage[32];		/* CAN FD TX Message Space Register Descriptions.            						Address offset: 0x100 - 0x9FC */
	CANFD_RxfilterTypeDef		Rxfiltering[32];	/* ID Filter-Mask Pairs.            												Address offset: 0xA00 - 0xAFC */
	uint32_t              		RESERVED4[1344];	/* Reserved.                  														Address offset: 0xB00 - 0x1FFC */
	uint32_t              		RESERVED5[64];		/* Reserved.                  														Address offset: 0x2000 - 0x20FC */
	CANFD_RxMessageTypeDef		RxMessage[64];		/* CAN FD RX Message Space (Sequential/FIFO Buffers-RX FIFO) Register Descriptions.	Address offset: 0x2100 - 0x32FC */
} CANFD_TypeDef;

/*
 * @addtogroup CANFD Peripheral declaration
 */
#define CANFD0                ((CANFD_TypeDef *) CANFD0_BASE)
#define CANFD1                ((CANFD_TypeDef *) CANFD1_BASE)
#define CANFD2                ((CANFD_TypeDef *) CANFD2_BASE)
#define CANFD3                ((CANFD_TypeDef *) CANFD3_BASE)

/******************  Bit definition for CANFD_SRR register  *******************/
#define CANFD_SRR_SRST_DPos       (0U)
#define CANFD_SRR_SRST_Pos        CANFD_SRR_SRST_DPos
#define CANFD_SRR_SRST_Msk        (0x1UL << CANFD_SRR_SRST_Pos)                /* 0x00000001 */
#define CANFD_SRR_SRST            CANFD_SRR_SRST_Msk                           /* Reset. */

#define CANFD_SRR_CEN_DPos        (1U)
#define CANFD_SRR_CEN_Pos         CANFD_SRR_CEN_DPos
#define CANFD_SRR_CEN_Msk         (0x1UL << CANFD_SRR_CEN_Pos)                 /* 0x00000002 */
#define CANFD_SRR_CEN             CANFD_SRR_CEN_Msk                            /* CAN Enable. */

/******************  Bit definition for CANFD_MSR register  *******************/
#define CANFD_MSR_SLEEP_DPos      (0U)
#define CANFD_MSR_SLEEP_Pos       CANFD_MSR_SLEEP_DPos
#define CANFD_MSR_SLEEP_Msk       (0x1UL << CANFD_MSR_SLEEP_Pos)               /* 0x00000001 */
#define CANFD_MSR_SLEEP           CANFD_MSR_SLEEP_Msk                          /* Sleep Mode Select/Request. */

#define CANFD_MSR_LBACK_DPos      (1U)
#define CANFD_MSR_LBACK_Pos       CANFD_MSR_LBACK_DPos
#define CANFD_MSR_LBACK_Msk       (0x1UL << CANFD_MSR_LBACK_Pos)               /* 0x00000002 */
#define CANFD_MSR_LBACK           CANFD_MSR_LBACK_Msk                          /* Loopback Mode Select/Request. */

#define CANFD_MSR_SNOOP_DPos      (2U)
#define CANFD_MSR_SNOOP_Pos       CANFD_MSR_SNOOP_DPos
#define CANFD_MSR_SNOOP_Msk       (0x1UL << CANFD_MSR_SNOOP_Pos)               /* 0x00000004 */
#define CANFD_MSR_SNOOP           CANFD_MSR_SNOOP_Msk                          /* SNOOP Mode Select/Request. */

#define CANFD_MSR_BRSD_DPos       (3U)
#define CANFD_MSR_BRSD_Pos        CANFD_MSR_BRSD_DPos
#define CANFD_MSR_BRSD_Msk        (0x1UL << CANFD_MSR_BRSD_Pos)                /* 0x00000008 */
#define CANFD_MSR_BRSD            CANFD_MSR_BRSD_Msk                           /* CAN FD Bit Rate Switch Disable Override. */

#define CANFD_MSR_DAR_DPos        (4U)
#define CANFD_MSR_DAR_Pos         CANFD_MSR_DAR_DPos
#define CANFD_MSR_DAR_Msk         (0x1UL << CANFD_MSR_DAR_Pos)                 /* 0x00000010 */
#define CANFD_MSR_DAR             CANFD_MSR_DAR_Msk                            /* Disable Auto-Retransmission. */

#define CANFD_MSR_DPEE_DPos       (5U)
#define CANFD_MSR_DPEE_Pos        CANFD_MSR_DPEE_DPos
#define CANFD_MSR_DPEE_Msk        (0x1UL << CANFD_MSR_DPEE_Pos)                /* 0x00000020 */
#define CANFD_MSR_DPEE            CANFD_MSR_DPEE_Msk                           /* Disable Protocol Exception Event Detection/Generation. */

#define CANFD_MSR_SBR_DPos        (6U)
#define CANFD_MSR_SBR_Pos         CANFD_MSR_SBR_DPos
#define CANFD_MSR_SBR_Msk         (0x1UL << CANFD_MSR_SBR_Pos)                 /* 0x00000040 */
#define CANFD_MSR_SBR             CANFD_MSR_SBR_Msk                            /* Start Bus-Off Recovery Request. */

#define CANFD_MSR_ABR_DPos        (7U)
#define CANFD_MSR_ABR_Pos         CANFD_MSR_ABR_DPos
#define CANFD_MSR_ABR_Msk         (0x1UL << CANFD_MSR_ABR_Pos)                 /* 0x00000080 */
#define CANFD_MSR_ABR             CANFD_MSR_ABR_Msk                            /* Auto Bus-Off Recovery Request. */

/****************  Bit definition for CANFD_APBRPR register  ******************/
#define CANFD_APBRPR_BRP_DPos     (0U)
#define CANFD_APBRPR_BRP_Pos      CANFD_APBRPR_BRP_DPos
#define CANFD_APBRPR_BRP_Msk      (0xFFUL << CANFD_APBRPR_BRP_Pos)             /* 0x000000FF */
#define CANFD_APBRPR_BRP          CANFD_APBRPR_BRP_Msk                         /* BRP[7:0] bits (Arbitration Phase (Nominal) Baud Rate Prescaler.) */
#define CANFD_APBRPR_BRP_0        (0x1UL << CANFD_APBRPR_BRP_Pos)              /* 0x00000001 */
#define CANFD_APBRPR_BRP_1        (0x2UL << CANFD_APBRPR_BRP_Pos)              /* 0x00000002 */
#define CANFD_APBRPR_BRP_2        (0x4UL << CANFD_APBRPR_BRP_Pos)              /* 0x00000004 */
#define CANFD_APBRPR_BRP_3        (0x8UL << CANFD_APBRPR_BRP_Pos)              /* 0x00000008 */
#define CANFD_APBRPR_BRP_4        (0x10UL << CANFD_APBRPR_BRP_Pos)             /* 0x00000010 */
#define CANFD_APBRPR_BRP_5        (0x20UL << CANFD_APBRPR_BRP_Pos)             /* 0x00000020 */
#define CANFD_APBRPR_BRP_6        (0x40UL << CANFD_APBRPR_BRP_Pos)             /* 0x00000040 */
#define CANFD_APBRPR_BRP_7        (0x80UL << CANFD_APBRPR_BRP_Pos)             /* 0x00000080 */

/****************  Bit definition for CANFD_APBTR register  *******************/
#define CANFD_APBTR_TS1_DPos      (0U)
#define CANFD_APBTR_TS1_Pos       CANFD_APBTR_TS1_DPos
#define CANFD_APBTR_TS1_Msk       (0xFFUL << CANFD_APBTR_TS1_Pos)              /* 0x000000FF */
#define CANFD_APBTR_TS1           CANFD_APBTR_TS1_Msk                          /* TS1[7:0] bits (Time Segment 1) */
#define CANFD_APBTR_TS1_0         (0x1UL << CANFD_APBTR_TS1_Pos)               /* 0x00000001 */
#define CANFD_APBTR_TS1_1         (0x2UL << CANFD_APBTR_TS1_Pos)               /* 0x00000002 */
#define CANFD_APBTR_TS1_2         (0x4UL << CANFD_APBTR_TS1_Pos)               /* 0x00000004 */
#define CANFD_APBTR_TS1_3         (0x8UL << CANFD_APBTR_TS1_Pos)               /* 0x00000008 */
#define CANFD_APBTR_TS1_4         (0x10UL << CANFD_APBTR_TS1_Pos)              /* 0x00000010 */
#define CANFD_APBTR_TS1_5         (0x20UL << CANFD_APBTR_TS1_Pos)              /* 0x00000020 */
#define CANFD_APBTR_TS1_6         (0x40UL << CANFD_APBTR_TS1_Pos)              /* 0x00000040 */
#define CANFD_APBTR_TS1_7         (0x80UL << CANFD_APBTR_TS1_Pos)              /* 0x00000080 */

#define CANFD_APBTR_TS2_DPos      (8U)
#define CANFD_APBTR_TS2_Pos       CANFD_APBTR_TS2_DPos
#define CANFD_APBTR_TS2_Msk       (0x7FUL << CANFD_APBTR_TS2_Pos)              /* 0x00007F00 */
#define CANFD_APBTR_TS2           CANFD_APBTR_TS2_Msk                          /* TS2[6:0] bits (Time Segment 2) */
#define CANFD_APBTR_TS2_0         (0x1UL << CANFD_APBTR_TS2_Pos)               /* 0x00000100 */
#define CANFD_APBTR_TS2_1         (0x2UL << CANFD_APBTR_TS2_Pos)               /* 0x00000200 */
#define CANFD_APBTR_TS2_2         (0x4UL << CANFD_APBTR_TS2_Pos)               /* 0x00000400 */
#define CANFD_APBTR_TS2_3         (0x8UL << CANFD_APBTR_TS2_Pos)               /* 0x00000800 */
#define CANFD_APBTR_TS2_4         (0x10UL << CANFD_APBTR_TS2_Pos)              /* 0x00001000 */
#define CANFD_APBTR_TS2_5         (0x20UL << CANFD_APBTR_TS2_Pos)              /* 0x00002000 */
#define CANFD_APBTR_TS2_6         (0x40UL << CANFD_APBTR_TS2_Pos)              /* 0x00004000 */

#define CANFD_APBTR_SJW_DPos      (16U)
#define CANFD_APBTR_SJW_Pos       CANFD_APBTR_SJW_DPos
#define CANFD_APBTR_SJW_Msk       (0x7FUL << CANFD_APBTR_SJW_Pos)              /* 0x007F0000 */
#define CANFD_APBTR_SJW           CANFD_APBTR_SJW_Msk                          /* SJW[6:0] bits (Synchronization Jump Width) */
#define CANFD_APBTR_SJW_0         (0x1UL << CANFD_APBTR_SJW_Pos)               /* 0x00010000 */
#define CANFD_APBTR_SJW_1         (0x2UL << CANFD_APBTR_SJW_Pos)               /* 0x00020000 */
#define CANFD_APBTR_SJW_2         (0x4UL << CANFD_APBTR_SJW_Pos)               /* 0x00040000 */
#define CANFD_APBTR_SJW_3         (0x8UL << CANFD_APBTR_SJW_Pos)               /* 0x00080000 */
#define CANFD_APBTR_SJW_4         (0x10UL << CANFD_APBTR_SJW_Pos)              /* 0x00100000 */
#define CANFD_APBTR_SJW_5         (0x20UL << CANFD_APBTR_SJW_Pos)              /* 0x00200000 */
#define CANFD_APBTR_SJW_6         (0x40UL << CANFD_APBTR_SJW_Pos)              /* 0x00400000 */

/*****************  Bit definition for CANFD_ECR register  ********************/
#define CANFD_ECR_TEC_DPos         (0U)
#define CANFD_ECR_TEC_Pos         CANFD_ECR_TEC_DPos
#define CANFD_ECR_TEC_Msk         (0xFFUL << CANFD_ECR_TEC_Pos)                /* 0x000000FF */
#define CANFD_ECR_TEC             CANFD_ECR_TEC_Msk                            /* TEC[7:0] bits (Transmit Error Count) */
#define CANFD_ECR_TEC_0           (0x1UL << CANFD_ECR_TEC_Pos)                 /* 0x00000001 */
#define CANFD_ECR_TEC_1           (0x2UL << CANFD_ECR_TEC_Pos)                 /* 0x00000002 */
#define CANFD_ECR_TEC_2           (0x4UL << CANFD_ECR_TEC_Pos)                 /* 0x00000004 */
#define CANFD_ECR_TEC_3           (0x8UL << CANFD_ECR_TEC_Pos)                 /* 0x00000008 */
#define CANFD_ECR_TEC_4           (0x10UL << CANFD_ECR_TEC_Pos)                /* 0x00000010 */
#define CANFD_ECR_TEC_5           (0x20UL << CANFD_ECR_TEC_Pos)                /* 0x00000020 */
#define CANFD_ECR_TEC_6           (0x40UL << CANFD_ECR_TEC_Pos)                /* 0x00000040 */
#define CANFD_ECR_TEC_7           (0x80UL << CANFD_ECR_TEC_Pos)                /* 0x00000080 */

#define CANFD_ECR_REC_DPos        (8U)
#define CANFD_ECR_REC_Pos         CANFD_ECR_REC_DPos
#define CANFD_ECR_REC_Msk         (0xFFUL << CANFD_ECR_REC_Pos)                /* 0x0000FF00 */
#define CANFD_ECR_REC             CANFD_ECR_REC_Msk                            /* REC[7:0] bits (Receive Error Count) */
#define CANFD_ECR_REC_0           (0x1UL << CANFD_ECR_REC_Pos)                 /* 0x00000100 */
#define CANFD_ECR_REC_1           (0x2UL << CANFD_ECR_REC_Pos)                 /* 0x00000200 */
#define CANFD_ECR_REC_2           (0x4UL << CANFD_ECR_REC_Pos)                 /* 0x00000400 */
#define CANFD_ECR_REC_3           (0x8UL << CANFD_ECR_REC_Pos)                 /* 0x00000800 */
#define CANFD_ECR_REC_4           (0x10UL << CANFD_ECR_REC_Pos)                /* 0x00001000 */
#define CANFD_ECR_REC_5           (0x20UL << CANFD_ECR_REC_Pos)                /* 0x00002000 */
#define CANFD_ECR_REC_6           (0x40UL << CANFD_ECR_REC_Pos)                /* 0x00004000 */
#define CANFD_ECR_REC_7           (0x80UL << CANFD_ECR_REC_Pos)                /* 0x00008000 */

/******************  Bit definition for CANFD_ESR register  *******************/
#define CANFD_ESR_CRCER_DPos      (0U)
#define CANFD_ESR_CRCER_Pos       CANFD_ESR_CRCER_DPos
#define CANFD_ESR_CRCER_Msk       (0x1UL << CANFD_ESR_CRCER_Pos)               /* 0x00000001 */
#define CANFD_ESR_CRCER           CANFD_ESR_CRCER_Msk                          /* CRC Error. */

#define CANFD_ESR_FMER_DPos       (1U)
#define CANFD_ESR_FMER_Pos        CANFD_ESR_FMER_DPos
#define CANFD_ESR_FMER_Msk        (0x1UL << CANFD_ESR_FMER_Pos)                /* 0x00000002 */
#define CANFD_ESR_FMER            CANFD_ESR_FMER_Msk                           /* Form Error. */

#define CANFD_ESR_STER_DPos       (2U)
#define CANFD_ESR_STER_Pos        CANFD_ESR_STER_DPos
#define CANFD_ESR_STER_Msk        (0x1UL << CANFD_ESR_STER_Pos)                /* 0x00000004 */
#define CANFD_ESR_STER            CANFD_ESR_STER_Msk                           /* Stuff Error */

#define CANFD_ESR_BERR_DPos       (3U)
#define CANFD_ESR_BERR_Pos        CANFD_ESR_BERR_DPos
#define CANFD_ESR_BERR_Msk        (0x1UL << CANFD_ESR_BERR_Pos)                /* 0x00000008 */
#define CANFD_ESR_BERR            CANFD_ESR_BERR_Msk                           /* Bit Error. */

#define CANFD_ESR_ACKER_DPos      (4U)
#define CANFD_ESR_ACKER_Pos       CANFD_ESR_ACKER_DPos
#define CANFD_ESR_ACKER_Msk       (0x1UL << CANFD_ESR_ACKER_Pos)               /* 0x00000010 */
#define CANFD_ESR_ACKER           CANFD_ESR_ACKER_Msk                          /* ACK Error. */

#define CANFD_ESR_F_CRCER_DPos    (8U)
#define CANFD_ESR_F_CRCER_Pos     CANFD_ESR_F_CRCER_DPos
#define CANFD_ESR_F_CRCER_Msk     (0x1UL << CANFD_ESR_F_CRCER_Pos)             /* 0x00000100 */
#define CANFD_ESR_F_CRCER         CANFD_ESR_F_CRCER_Msk                        /* CRC Error in CAN FD Data Phase. */

#define CANFD_ESR_F_FMER_DPos     (9U)
#define CANFD_ESR_F_FMER_Pos      CANFD_ESR_F_FMER_DPos
#define CANFD_ESR_F_FMER_Msk      (0x1UL << CANFD_ESR_F_FMER_Pos)              /* 0x00000200 */
#define CANFD_ESR_F_FMER          CANFD_ESR_F_FMER_Msk                         /* Form Error in CAN FD Data Phase. */

#define CANFD_ESR_F_STER_DPos     (10U)
#define CANFD_ESR_F_STER_Pos      CANFD_ESR_F_STER_DPos
#define CANFD_ESR_F_STER_Msk      (0x1UL << CANFD_ESR_F_STER_Pos)              /* 0x00000400 */
#define CANFD_ESR_F_STER          CANFD_ESR_F_STER_Msk                         /* Stuff Error in CAN FD Data Phase. */

#define CANFD_ESR_F_BERR_DPos     (11U)
#define CANFD_ESR_F_BERR_Pos      CANFD_ESR_F_BERR_DPos
#define CANFD_ESR_F_BERR_Msk      (0x1UL << CANFD_ESR_F_BERR_Pos)              /* 0x00000800 */
#define CANFD_ESR_F_BERR          CANFD_ESR_F_BERR_Msk                         /* Bit Error in CAN FD Data Phase. */

/*******************  Bit definition for CANFD_SR register  *******************/
#define CANFD_SR_CONFIG_DPos      (0U)
#define CANFD_SR_CONFIG_Pos       CANFD_SR_CONFIG_DPos
#define CANFD_SR_CONFIG_Msk       (0x1UL << CANFD_SR_CONFIG_Pos)               /* 0x00000001 */
#define CANFD_SR_CONFIG           CANFD_SR_CONFIG_Msk                          /* Configuration Mode Indicator. */

#define CANFD_SR_LBACK_DPos       (1U)
#define CANFD_SR_LBACK_Pos        CANFD_SR_LBACK_DPos
#define CANFD_SR_LBACK_Msk        (0x1UL << CANFD_SR_LBACK_Pos)                /* 0x00000002 */
#define CANFD_SR_LBACK            CANFD_SR_LBACK_Msk                           /* Loopback Mode. */

#define CANFD_SR_SLEEP_DPos       (2U)
#define CANFD_SR_SLEEP_Pos        CANFD_SR_SLEEP_DPos
#define CANFD_SR_SLEEP_Msk        (0x1UL << CANFD_SR_SLEEP_DPos)                /* 0x00000004 */
#define CANFD_SR_SLEEP            CANFD_SR_SLEEP_Msk                           /* Sleep Mode. */

#define CANFD_SR_NORMAL_DPos       (3U)
#define CANFD_SR_NORMAL_Pos       CANFD_SR_NORMAL_DPos
#define CANFD_SR_NORMAL_Msk       (0x1UL << CANFD_SR_NORMAL_Pos)               /* 0x00000008 */
#define CANFD_SR_NORMAL           CANFD_SR_NORMAL_Msk                          /* Normal Mode. */

#define CANFD_SR_BIDLE_DPos       (4U)
#define CANFD_SR_BIDLE_Pos        CANFD_SR_BIDLE_DPos
#define CANFD_SR_BIDLE_Msk        (0x1UL << CANFD_SR_BIDLE_DPos)                /* 0x00000010 */
#define CANFD_SR_BIDLE            CANFD_SR_BIDLE_Msk                           /* Bus Idle. */

#define CANFD_SR_BBSY_DPos        (5U)
#define CANFD_SR_BBSY_Pos        CANFD_SR_BBSY_DPos
#define CANFD_SR_BBSY_Pos         CANFD_SR_BBSY_DPos
#define CANFD_SR_BBSY_Msk         (0x1UL << CANFD_SR_BBSY_Pos)                 /* 0x00000020 */
#define CANFD_SR_BBSY             CANFD_SR_BBSY_Msk                            /* Indicates the CAN bus status. */

#define CANFD_SR_ERRWRN_DPos      (6U)
#define CANFD_SR_ERRWRN_Pos       CANFD_SR_ERRWRN_DPos
#define CANFD_SR_ERRWRN_Msk       (0x1UL << CANFD_SR_ERRWRN_Pos)               /* 0x00000040 */
#define CANFD_SR_ERRWRN           CANFD_SR_ERRWRN_Msk                          /* Error Warning. */

#define CANFD_SR_ESTAT_DPos       (7U)
#define CANFD_SR_ESTAT_Pos        CANFD_SR_ESTAT_DPos
#define CANFD_SR_ESTAT_Msk        (0x3UL << CANFD_SR_ESTAT_Pos)                /* 0x00000180 */
#define CANFD_SR_ESTAT            CANFD_SR_ESTAT_Msk                           /* ESTAT[1:0] bits (Error Status.) */
#define CANFD_SR_ESTAT_0          (0x1UL << CANFD_SR_ESTAT_Pos)                /* 0x00000100 */
#define CANFD_SR_ESTAT_1          (0x2UL << CANFD_SR_ESTAT_Pos)                /* 0x00000200 */

#define CANFD_SR_PEE_CONFIG_DPos  (9U)
#define CANFD_SR_PEE_CONFIG_Pos   CANFD_SR_PEE_CONFIG_DPos
#define CANFD_SR_PEE_CONFIG_Msk   (0x1UL << CANFD_SR_PEE_CONFIG_Pos)           /* 0x00000200 */
#define CANFD_SR_PEE_CONFIG       CANFD_SR_PEE_CONFIG_Msk                      /* PEE Mode Indicator. */

#define CANFD_SR_BSFR_CONFIG_DPos (10U)
#define CANFD_SR_BSFR_CONFIG_Pos  CANFD_SR_BSFR_CONFIG_DPos
#define CANFD_SR_BSFR_CONFIG_Msk  (0x1UL << CANFD_SR_BSFR_CONFIG_Pos)          /* 0x00000400 */
#define CANFD_SR_BSFR_CONFIG      CANFD_SR_BSFR_CONFIG_Msk                     /* Bus-Off Recovery Mode Indicator. */

#define CANFD_SR_SNOOP_DPos       (12U)
#define CANFD_SR_SNOOP_Pos        CANFD_SR_SNOOP_DPos
#define CANFD_SR_SNOOP_Msk        (0x1UL << CANFD_SR_SNOOP_Pos)                /* 0x00001000 */
#define CANFD_SR_SNOOP            CANFD_SR_SNOOP_Msk                           /* Snoop Mode. */
#define CANFD_SR_TDCV_Pos         (16U)
#define CANFD_SR_TDCV_Msk         (0x7FUL << CANFD_SR_TDCV_Pos)                /* 0x007F0000 */
#define CANFD_SR_TDCV             CANFD_SR_TDCV_Msk                            /* TDCV[6:0] bits (Transmitter Delay Compensation Value.) */
#define CANFD_SR_TDCV_0           (0x1UL << CANFD_SR_TDCV_Pos)                 /* 0x00010000 */
#define CANFD_SR_TDCV_1           (0x2UL << CANFD_SR_TDCV_Pos)                 /* 0x00020000 */
#define CANFD_SR_TDCV_2           (0x4UL << CANFD_SR_TDCV_Pos)                 /* 0x00040000 */
#define CANFD_SR_TDCV_3           (0x8UL << CANFD_SR_TDCV_Pos)                 /* 0x00080000 */
#define CANFD_SR_TDCV_4           (0x10UL << CANFD_SR_TDCV_Pos)                /* 0x00100000 */
#define CANFD_SR_TDCV_5           (0x20UL << CANFD_SR_TDCV_Pos)                /* 0x00200000 */
#define CANFD_SR_TDCV_6           (0x40UL << CANFD_SR_TDCV_Pos)                /* 0x00400000 */

/******************  Bit definition for CANFD_ISR register  *******************/
#define CANFD_ISR_ARBLST_DPos     (0U)
#define CANFD_ISR_ARBLST_Pos      CANFD_ISR_ARBLST_DPos
#define CANFD_ISR_ARBLST_Msk      (0x1UL << CANFD_ISR_ARBLST_Pos)              /* 0x00000001 */
#define CANFD_ISR_ARBLST          CANFD_ISR_ARBLST_Msk                         /* Arbitration Lost Interrupt. */

#define CANFD_ISR_TXOK_DPos       (1U)
#define CANFD_ISR_TXOK_Pos        CANFD_ISR_TXOK_DPos
#define CANFD_ISR_TXOK_Msk        (0x1UL << CANFD_ISR_TXOK_Pos)                /* 0x00000002 */
#define CANFD_ISR_TXOK            CANFD_ISR_TXOK_Msk                           /* Transmission Successful Interrupt. */

#define CANFD_ISR_PEE_DPos        (2U)
#define CANFD_ISR_PEE_Pos         CANFD_ISR_PEE_DPos
#define CANFD_ISR_PEE_Msk         (0x1UL << CANFD_ISR_PEE_Pos)                 /* 0x00000004 */
#define CANFD_ISR_PEE             CANFD_ISR_PEE_Msk                            /* Protocol Exception Event Interrupt. */

#define CANFD_ISR_BSFRD_DPos      (3U)
#define CANFD_ISR_BSFRD_Pos       CANFD_ISR_BSFRD_DPos
#define CANFD_ISR_BSFRD_Msk       (0x1UL << CANFD_ISR_BSFRD_Pos)               /* 0x00000008 */
#define CANFD_ISR_BSFRD           CANFD_ISR_BSFRD_Msk                          /* Bus-Off Recovery Done Interrupt. */

#define CANFD_ISR_RXOK_DPos       (4U)
#define CANFD_ISR_RXOK_Pos        CANFD_ISR_RXOK_DPos
#define CANFD_ISR_RXOK_Msk        (0x1UL << CANFD_ISR_RXOK_Pos)                /* 0x00000010 */
#define CANFD_ISR_RXOK            CANFD_ISR_RXOK_Msk                           /* New Message Received Interrupt */

#define CANFD_ISR_TSCNT_OFLW_DPos (5U)
#define CANFD_ISR_TSCNT_OFLW_Pos  CANFD_ISR_TSCNT_OFLW_DPos
#define CANFD_ISR_TSCNT_OFLW_Msk  (0x1UL << CANFD_ISR_TSCNT_OFLW_Pos)          /* 0x00000020 */
#define CANFD_ISR_TSCNT_OFLW      CANFD_ISR_TSCNT_OFLW_Msk                     /* Timestamp Counter Overflow Interrupt. */

#define CANFD_ISR_RXFOFLW_DPos    (6U)
#define CANFD_ISR_RXFOFLW_Pos     CANFD_ISR_RXFOFLW_DPos
#define CANFD_ISR_RXFOFLW_Msk     (0x1UL << CANFD_ISR_RXFOFLW_Pos)             /* 0x00000040 */
#define CANFD_ISR_RXFOFLW         CANFD_ISR_RXFOFLW_Msk                        /* RX FIFO-0 Overflow Interrupt (Sequential/FIFO Mode). */

#define CANFD_ISR_ERROE_DPos      (8U)
#define CANFD_ISR_ERROE_Pos       CANFD_ISR_ERROE_DPos
#define CANFD_ISR_ERROE_Msk       (0x1UL << CANFD_ISR_ERROE_Pos)               /* 0x00000100 */
#define CANFD_ISR_ERROE           CANFD_ISR_ERROE_Msk                          /* Error Interrupt */

#define CANFD_ISR_BSOFF_DPos      (9U)
#define CANFD_ISR_BSOFF_Pos       CANFD_ISR_BSOFF_DPos
#define CANFD_ISR_BSOFF_Msk       (0x1UL << CANFD_ISR_BSOFF_Pos)               /* 0x00000200 */
#define CANFD_ISR_BSOFF           CANFD_ISR_BSOFF_Msk                          /* Bus-Off Interrupt. */

#define CANFD_ISR_SLP_DPos        (10U)
#define CANFD_ISR_SLP_Pos         CANFD_ISR_SLP_DPos
#define CANFD_ISR_SLP_Msk         (0x1UL << CANFD_ISR_SLP_Pos)                 /* 0x00000400 */
#define CANFD_ISR_SLP             CANFD_ISR_SLP_Msk                            /* Sleep Interrupt. */

#define CANFD_ISR_WKUP_DPos       (11U)
#define CANFD_ISR_WKUP_Pos        CANFD_ISR_WKUP_DPos
#define CANFD_ISR_WKUP_Msk        (0x1UL << CANFD_ISR_WKUP_Pos)                /* 0x00000800 */
#define CANFD_ISR_WKUP            CANFD_ISR_WKUP_Msk                           /* Wake-Up Interrupt */

#define CANFD_ISR_TXRRS_DPos      (13U)
#define CANFD_ISR_TXRRS_Pos       CANFD_ISR_TXRRS_DPos
#define CANFD_ISR_TXRRS_Msk       (0x1UL << CANFD_ISR_TXRRS_Pos)               /* 0x00002000 */
#define CANFD_ISR_TXRRS           CANFD_ISR_TXRRS_Msk                          /* TX Buffer Ready Request Served Interrupt. */

#define CANFD_ISR_TXCRS_DPos      (14U)
#define CANFD_ISR_TXCRS_Pos       CANFD_ISR_TXCRS_DPos
#define CANFD_ISR_TXCRS_Msk       (0x1UL << CANFD_ISR_TXCRS_Pos)               /* 0x00004000 */
#define CANFD_ISR_TXCRS           CANFD_ISR_TXCRS_Msk                          /* TX Cancellation Request Served Interrupt. */

/******************  Bit definition for CANFD_IER register  *******************/
#define CANFD_IER_EARBLST_DPos    (0U)
#define CANFD_IER_EARBLST_Pos     CANFD_IER_EARBLST_DPos
#define CANFD_IER_EARBLST_Msk     (0x1UL << CANFD_IER_EARBLST_Pos)             /* 0x00000001 */
#define CANFD_IER_EARBLST         CANFD_IER_EARBLST_Msk                        /* Arbitration Lost Interrupt Enable */

#define CANFD_IER_ETXOK_DPos      (1U)
#define CANFD_IER_ETXOK_Pos       CANFD_IER_ETXOK_DPos
#define CANFD_IER_ETXOK_Msk       (0x1UL << CANFD_IER_ETXOK_Pos)               /* 0x00000002 */
#define CANFD_IER_ETXOK           CANFD_IER_ETXOK_Msk                          /* Transmission Successful Interrupt Enable. */

#define CANFD_IER_EPEE_DPos       (2U)
#define CANFD_IER_EPEE_Pos        CANFD_IER_EPEE_DPos
#define CANFD_IER_EPEE_Msk        (0x1UL << CANFD_IER_EPEE_Pos)                /* 0x00000004 */
#define CANFD_IER_EPEE            CANFD_IER_EPEE_Msk                           /* Protocol Exception Event Interrupt Enable. */

#define CANFD_IER_EBSFRD_DPos     (3U)
#define CANFD_IER_EBSFRD_Pos      CANFD_IER_EBSFRD_DPos
#define CANFD_IER_EBSFRD_Msk      (0x1UL << CANFD_IER_EBSFRD_Pos)              /* 0x00000008 */
#define CANFD_IER_EBSFRD          CANFD_IER_EBSFRD_Msk                         /* Bus-Off Recovery Done Interrupt Enable. */

#define CANFD_IER_ERXOK_DPos      (4U)
#define CANFD_IER_ERXOK_Pos       CANFD_IER_ERXOK_DPos
#define CANFD_IER_ERXOK_Msk       (0x1UL << CANFD_IER_ERXOK_Pos)               /* 0x00000010 */
#define CANFD_IER_ERXOK           CANFD_IER_ERXOK_Msk                          /* New Message Received Interrupt Enable. */

#define CANFD_IER_ETSCNT_OFLW_DPos (5U)
#define CANFD_IER_ETSCNT_OFLW_Pos  CANFD_IER_ETSCNT_OFLW_DPos
#define CANFD_IER_ETSCNT_OFLW_Msk (0x1UL << CANFD_IER_ETSCNT_OFLW_Pos)         /* 0x00000020 */
#define CANFD_IER_ETSCNT_OFLW     CANFD_IER_ETSCNT_OFLW_Msk                    /* Timestamp Counter Overflow Interrupt Enable. */

#define CANFD_IER_ERXFOFLW_DPos   (6U)
#define CANFD_IER_ERXFOFLW_Pos    CANFD_IER_ERXFOFLW_DPos
#define CANFD_IER_ERXFOFLW_Msk    (0x1UL << CANFD_IER_ERXFOFLW_Pos)            /* 0x00000040 */
#define CANFD_IER_ERXFOFLW        CANFD_IER_ERXFOFLW_Msk                       /* RX FIFO-0 Overflow Interrupt Enable (Sequential/FIFO Mode). */

#define CANFD_IER_EERROR_DPos     (8U)
#define CANFD_IER_EERROR_Pos      CANFD_IER_EERROR_DPos
#define CANFD_IER_EERROR_Msk      (0x1UL << CANFD_IER_EERROR_Pos)              /* 0x00000100 */
#define CANFD_IER_EERROR          CANFD_IER_EERROR_Msk                         /* Error Interrupt Enable. */

#define CANFD_IER_EBSOFF_DPos     (9U)
#define CANFD_IER_EBSOFF_Pos      CANFD_IER_EBSOFF_DPos
#define CANFD_IER_EBSOFF_Msk      (0x1UL << CANFD_IER_EBSOFF_Pos)              /* 0x00000200 */
#define CANFD_IER_EBSOFF          CANFD_IER_EBSOFF_Msk                         /* Bus-Off Interrupt Enable. */

#define CANFD_IER_ESLP_DPos       (10U)
#define CANFD_IER_ESLP_Pos        CANFD_IER_ESLP_DPos
#define CANFD_IER_ESLP_Msk        (0x1UL << CANFD_IER_ESLP_Pos)                /* 0x00000400 */
#define CANFD_IER_ESLP            CANFD_IER_ESLP_Msk                           /* Sleep Interrupt Enable. */

#define CANFD_IER_EWKUP_DPos      (11U)
#define CANFD_IER_EWKUP_Pos       CANFD_IER_EWKUP_DPos
#define CANFD_IER_EWKUP_Msk       (0x1UL << CANFD_IER_EWKUP_Pos)               /* 0x00000800 */
#define CANFD_IER_EWKUP           CANFD_IER_EWKUP_Msk                          /* Wake-Up Interrupt Enable. */

#define CANFD_IER_ETXRRS_DPos     (13U)
#define CANFD_IER_ETXRRS_Pos      CANFD_IER_ETXRRS_DPos
#define CANFD_IER_ETXRRS_Msk      (0x1UL << CANFD_IER_ETXRRS_Pos)              /* 0x00002000 */
#define CANFD_IER_ETXRRS          CANFD_IER_ETXRRS_Msk                         /* TX Buffer Ready Request Served Interrupt Enable. */

#define CANFD_IER_ETXCRS_DPos     (14U)
#define CANFD_IER_ETXCRS_Pos      CANFD_IER_ETXCRS_DPos
#define CANFD_IER_ETXCRS_Msk      (0x1UL << CANFD_IER_ETXCRS_Pos)              /* 0x00004000 */
#define CANFD_IER_ETXCRS          CANFD_IER_ETXCRS_Msk                         /* TX Cancellation Request Served Interrupt Enable. */

/******************  Bit definition for CANFD_ICR register  *******************/
#define CANFD_ICR_CARBLST_DPos    (0U)
#define CANFD_ICR_CARBLST_Pos     CANFD_ICR_CARBLST_DPos
#define CANFD_ICR_CARBLST_Msk     (0x1UL << CANFD_ICR_CARBLST_Pos)             /* 0x00000001 */
#define CANFD_ICR_CARBLST         CANFD_ICR_CARBLST_Msk                        /* Clears Arbitration Lost interrupt */

#define CANFD_ICR_CTXOK_DPos      (1U)
#define CANFD_ICR_CTXOK_Pos       CANFD_ICR_CTXOK_DPos
#define CANFD_ICR_CTXOK_Msk       (0x1UL << CANFD_ICR_CTXOK_Pos)               /* 0x00000002 */
#define CANFD_ICR_CTXOK           CANFD_ICR_CTXOK_Msk                          /* Clears Transmission Successful interrupt */

#define CANFD_ICR_CPEE_DPos       (2U)
#define CANFD_ICR_CPEE_Pos        CANFD_ICR_CPEE_DPos
#define CANFD_ICR_CPEE_Msk        (0x1UL << CANFD_ICR_CPEE_Pos)                /* 0x00000004 */
#define CANFD_ICR_CPEE            CANFD_ICR_CPEE_Msk                           /* Clears Protocol Exception Event interrupt */

#define CANFD_ICR_CBSFRD_DPos     (3U)
#define CANFD_ICR_CBSFRD_Pos      CANFD_ICR_CBSFRD_DPos
#define CANFD_ICR_CBSFRD_Msk      (0x1UL << CANFD_ICR_CBSFRD_Pos)              /* 0x00000008 */
#define CANFD_ICR_CBSFRD          CANFD_ICR_CBSFRD_Msk                         /* Clears Bus-Off Recovery Done interrupt */

#define CANFD_ICR_CRXOK_DPos       (4U)
#define CANFD_ICR_CRXOK_Pos       CANFD_ICR_CRXOK_DPos
#define CANFD_ICR_CRXOK_Msk       (0x1UL << CANFD_ICR_CRXOK_Pos)               /* 0x00000010 */
#define CANFD_ICR_CRXOK           CANFD_ICR_CRXOK_Msk                          /* Clears New Message Received interrupt */

#define CANFD_ICR_CTSCNT_OFLW_DPos (5U)
#define CANFD_ICR_CTSCNT_OFLW_Pos CANFD_ICR_CTSCNT_OFLW_DPos
#define CANFD_ICR_CTSCNT_OFLW_Msk (0x1UL << CANFD_ICR_CTSCNT_OFLW_Pos)         /* 0x00000020 */
#define CANFD_ICR_CTSCNT_OFLW     CANFD_ICR_CTSCNT_OFLW_Msk                    /* Clears Timestamp Counter Overflow Interrupt */

#define CANFD_ICR_CRXFOFLW_DPos    (6U)
#define CANFD_ICR_CRXFOFLW_Pos     CANFD_ICR_CRXFOFLW_DPos
#define CANFD_ICR_CRXFOFLW_Msk    (0x1UL << CANFD_ICR_CRXFOFLW_Pos)            /* 0x00000040 */
#define CANFD_ICR_CRXFOFLW        CANFD_ICR_CRXFOFLW_Msk                       /* Clears RX FIFO-0 Overflow interrupt (Sequential/FIFO Mode). */

#define CANFD_ICR_CERROE_DPos     (8U)
#define CANFD_ICR_CERROE_Pos      CANFD_ICR_CERROE_DPos
#define CANFD_ICR_CERROE_Msk      (0x1UL << CANFD_ICR_CERROE_Pos)              /* 0x00000100 */
#define CANFD_ICR_CERROE          CANFD_ICR_CERROE_Msk                         /* Clears Error interrupt */

#define CANFD_ICR_CBSOFF_DPos     (9U)
#define CANFD_ICR_CBSOFF_Pos      CANFD_ICR_CBSOFF_DPos
#define CANFD_ICR_CBSOFF_Msk      (0x1UL << CANFD_ICR_CBSOFF_Pos)              /* 0x00000200 */
#define CANFD_ICR_CBSOFF          CANFD_ICR_CBSOFF_Msk                         /* Clears Bus-Off interrupt */

#define CANFD_ICR_CSLP_DPos       (10U)
#define CANFD_ICR_CSLP_Pos        CANFD_ICR_CSLP_DPos
#define CANFD_ICR_CSLP_Msk        (0x1UL << CANFD_ICR_CSLP_Pos)                /* 0x00000400 */
#define CANFD_ICR_CSLP            CANFD_ICR_CSLP_Msk                           /* Clears Sleep interrupt */

#define CANFD_ICR_CWKUP_DPos      (11U)
#define CANFD_ICR_CWKUP_Pos       CANFD_ICR_CWKUP_DPos
#define CANFD_ICR_CWKUP_Msk       (0x1UL << CANFD_ICR_CWKUP_Pos)               /* 0x00000800 */
#define CANFD_ICR_CWKUP           CANFD_ICR_CWKUP_Msk                          /* Clears Wake-Up interrupt */

#define CANFD_ICR_CTXRRS_DPos     (13U)
#define CANFD_ICR_CTXRRS_Pos      CANFD_ICR_CTXRRS_DPos
#define CANFD_ICR_CTXRRS_Msk      (0x1UL << CANFD_ICR_CTXRRS_Pos)              /* 0x00002000 */
#define CANFD_ICR_CTXRRS          CANFD_ICR_CTXRRS_Msk                         /* Clears TX Buffer Ready Request Served Interrupt */

#define CANFD_ICR_CTXCRS_DPos     (14U)
#define CANFD_ICR_CTXCRS_Pos      CANFD_ICR_CTXCRS_DPos
#define CANFD_ICR_CTXCRS_Msk      (0x1UL << CANFD_ICR_CTXCRS_Pos)              /* 0x00004000 */
#define CANFD_ICR_CTXCRS          CANFD_ICR_CTXCRS_Msk                         /* Clears TX Cancellation Request Served Interrupt */

/******************  Bit definition for CANFD_TSR register  *******************/
#define CANFD_TSR_CTS_DPos         (0U)
#define CANFD_TSR_CTS_Pos          CANFD_TSR_CTS_DPos
#define CANFD_TSR_CTS_Msk         (0x1UL << CANFD_TSR_CTS_Pos)                 /* 0x00000001 */
#define CANFD_TSR_CTS              CANFD_TSR_CTS_Msk                            /* Clear Timestamp Counter. */

#define CANFD_TSR_TIMESTAMP_CNT_DPos (16U)
#define CANFD_TSR_TIMESTAMP_CNT_Pos  CANFD_TSR_TIMESTAMP_CNT_DPos
#define CANFD_TSR_TIMESTAMP_CNT_Msk (0xFFFFUL << CANFD_TSR_TIMESTAMP_CNT_Pos)  /* 0xFFFF0000 */
#define CANFD_TSR_TIMESTAMP_CNT     CANFD_TSR_TIMESTAMP_CNT_Msk                /* TIMESTAMP_CNT[15:0] bits (Timestamp Counter Value.) */
#define CANFD_TSR_TIMESTAMP_CNT_0   (0x1UL << CANFD_TSR_TIMESTAMP_CNT_Pos)     /* 0x00010000 */
#define CANFD_TSR_TIMESTAMP_CNT_1   (0x2UL << CANFD_TSR_TIMESTAMP_CNT_Pos)     /* 0x00020000 */
#define CANFD_TSR_TIMESTAMP_CNT_2   (0x4UL << CANFD_TSR_TIMESTAMP_CNT_Pos)     /* 0x00040000 */
#define CANFD_TSR_TIMESTAMP_CNT_3   (0x8UL << CANFD_TSR_TIMESTAMP_CNT_Pos)     /* 0x00080000 */
#define CANFD_TSR_TIMESTAMP_CNT_4   (0x10UL << CANFD_TSR_TIMESTAMP_CNT_Pos)    /* 0x00100000 */
#define CANFD_TSR_TIMESTAMP_CNT_5   (0x20UL << CANFD_TSR_TIMESTAMP_CNT_Pos)    /* 0x00200000 */
#define CANFD_TSR_TIMESTAMP_CNT_6   (0x40UL << CANFD_TSR_TIMESTAMP_CNT_Pos)    /* 0x00400000 */
#define CANFD_TSR_TIMESTAMP_CNT_7   (0x80UL << CANFD_TSR_TIMESTAMP_CNT_Pos)    /* 0x00800000 */
#define CANFD_TSR_TIMESTAMP_CNT_8   (0x100UL << CANFD_TSR_TIMESTAMP_CNT_Pos)   /* 0x01000000 */
#define CANFD_TSR_TIMESTAMP_CNT_9   (0x200UL << CANFD_TSR_TIMESTAMP_CNT_Pos)   /* 0x02000000 */
#define CANFD_TSR_TIMESTAMP_CNT_10  (0x400UL << CANFD_TSR_TIMESTAMP_CNT_Pos)   /* 0x04000000 */
#define CANFD_TSR_TIMESTAMP_CNT_11  (0x800UL << CANFD_TSR_TIMESTAMP_CNT_Pos)   /* 0x08000000 */
#define CANFD_TSR_TIMESTAMP_CNT_12  (0x1000UL << CANFD_TSR_TIMESTAMP_CNT_Pos)  /* 0x10000000 */
#define CANFD_TSR_TIMESTAMP_CNT_13  (0x2000UL << CANFD_TSR_TIMESTAMP_CNT_Pos)  /* 0x20000000 */
#define CANFD_TSR_TIMESTAMP_CNT_14  (0x4000UL << CANFD_TSR_TIMESTAMP_CNT_Pos)  /* 0x40000000 */
#define CANFD_TSR_TIMESTAMP_CNT_15  (0x8000UL << CANFD_TSR_TIMESTAMP_CNT_Pos)  /* 0x80000000 */

/****************  Bit definition for CANFD_DPBRPR register  ******************/
#define CANFD_DPBRPR_DP_BRP_DPos 	(0U)
#define CANFD_DPBRPR_DP_BRP_Pos  	CANFD_DPBRPR_DP_BRP_DPos
#define CANFD_DPBRPR_DP_BRP_Msk  	(0xFFUL << CANFD_DPBRPR_DP_BRP_Pos)         /* 0x000000FF */
#define CANFD_DPBRPR_DP_BRP      	CANFD_DPBRPR_DP_BRP_Msk                     /* DP_BRP[7:0] bits (Data Phase Baud Rate Prescaler) */
#define CANFD_DPBRPR_DP_BRP_0    	(0x1UL << CANFD_DPBRPR_DP_BRP_Pos)          /* 0x00000001 */
#define CANFD_DPBRPR_DP_BRP_1    	(0x2UL << CANFD_DPBRPR_DP_BRP_Pos)          /* 0x00000002 */
#define CANFD_DPBRPR_DP_BRP_2    	(0x4UL << CANFD_DPBRPR_DP_BRP_Pos)          /* 0x00000004 */
#define CANFD_DPBRPR_DP_BRP_3    	(0x8UL << CANFD_DPBRPR_DP_BRP_Pos)          /* 0x00000008 */
#define CANFD_DPBRPR_DP_BRP_4    	(0x10UL << CANFD_DPBRPR_DP_BRP_Pos)         /* 0x00000010 */
#define CANFD_DPBRPR_DP_BRP_5    	(0x20UL << CANFD_DPBRPR_DP_BRP_Pos)         /* 0x00000020 */
#define CANFD_DPBRPR_DP_BRP_6    	(0x40UL << CANFD_DPBRPR_DP_BRP_Pos)         /* 0x00000040 */
#define CANFD_DPBRPR_DP_BRP_7    	(0x80UL << CANFD_DPBRPR_DP_BRP_Pos)         /* 0x00000080 */

#define CANFD_DPBRPR_TDCOFF_DPos 	(8U)
#define CANFD_DPBRPR_TDCOFF_Pos  	CANFD_DPBRPR_TDCOFF_DPos
#define CANFD_DPBRPR_TDCOFF_Msk  	(0x3FUL << CANFD_DPBRPR_TDCOFF_Pos)         /* 0x00003F00 */
#define CANFD_DPBRPR_TDCOFF      	CANFD_DPBRPR_TDCOFF_Msk                     /* TDCOFF[5:0] bits (Transmitter Delay Compensation Offset) */
#define CANFD_DPBRPR_TDCOFF_0    	(0x1UL << CANFD_DPBRPR_TDCOFF_Pos)          /* 0x00000100 */
#define CANFD_DPBRPR_TDCOFF_1    	(0x2UL << CANFD_DPBRPR_TDCOFF_Pos)          /* 0x00000200 */
#define CANFD_DPBRPR_TDCOFF_2    	(0x4UL << CANFD_DPBRPR_TDCOFF_Pos)          /* 0x00000400 */
#define CANFD_DPBRPR_TDCOFF_3    	(0x8UL << CANFD_DPBRPR_TDCOFF_Pos)          /* 0x00000800 */
#define CANFD_DPBRPR_TDCOFF_4    	(0x10UL << CANFD_DPBRPR_TDCOFF_Pos)         /* 0x00001000 */
#define CANFD_DPBRPR_TDCOFF_5    	(0x20UL << CANFD_DPBRPR_TDCOFF_Pos)         /* 0x00002000 */

#define CANFD_DPBRPR_TDC_DPos       (16U)
#define CANFD_DPBRPR_TDC_Pos        CANFD_DPBRPR_TDC_DPos
#define CANFD_DPBRPR_TDC_Msk        (0x1UL << CANFD_DPBRPR_TDC_Pos)             /* 0x00010000 */
#define CANFD_DPBRPR_TDC            CANFD_DPBRPR_TDC_Msk                        /* Transmitter Delay Compensation (TDC) Enable */

/*****************  Bit definition for CANFD_DPBTR register  ******************/
#define CANFD_DPBTR_DP_TS1_DPos   (0U)
#define CANFD_DPBTR_DP_TS1_Pos    CANFD_DPBTR_DP_TS1_DPos
#define CANFD_DPBTR_DP_TS1_Msk    (0x1FUL << CANFD_DPBTR_DP_TS1_Pos)           /* 0x0000001F */
#define CANFD_DPBTR_DP_TS1        CANFD_DPBTR_DP_TS1_Msk                       /* DP_TS1[4:0] bits (Data Phase Time Segment 1) */
#define CANFD_DPBTR_DP_TS1_0      (0x1UL << CANFD_DPBTR_DP_TS1_Pos)            /* 0x00000001 */
#define CANFD_DPBTR_DP_TS1_1      (0x2UL << CANFD_DPBTR_DP_TS1_Pos)            /* 0x00000002 */
#define CANFD_DPBTR_DP_TS1_2      (0x4UL << CANFD_DPBTR_DP_TS1_Pos)            /* 0x00000004 */
#define CANFD_DPBTR_DP_TS1_3      (0x8UL << CANFD_DPBTR_DP_TS1_Pos)            /* 0x00000008 */
#define CANFD_DPBTR_DP_TS1_4      (0x10UL << CANFD_DPBTR_DP_TS1_Pos)           /* 0x00000010 */

#define CANFD_DPBTR_DP_TS2_DPos   (8U)
#define CANFD_DPBTR_DP_TS2_Pos    CANFD_DPBTR_DP_TS2_DPos
#define CANFD_DPBTR_DP_TS2_Msk    (0xFUL << CANFD_DPBTR_DP_TS2_Pos)            /* 0x00000F00 */
#define CANFD_DPBTR_DP_TS2        CANFD_DPBTR_DP_TS2_Msk                       /* DP_TS2[3:0] bits (Data Phase Time Segment 2) */
#define CANFD_DPBTR_DP_TS2_0      (0x1UL << CANFD_DPBTR_DP_TS2_Pos)            /* 0x00000100 */
#define CANFD_DPBTR_DP_TS2_1      (0x2UL << CANFD_DPBTR_DP_TS2_Pos)            /* 0x00000200 */
#define CANFD_DPBTR_DP_TS2_2      (0x4UL << CANFD_DPBTR_DP_TS2_Pos)            /* 0x00000400 */
#define CANFD_DPBTR_DP_TS2_3      (0x8UL << CANFD_DPBTR_DP_TS2_Pos)            /* 0x00000800 */

#define CANFD_DPBTR_DP_SJW_DPos   (16U)
#define CANFD_DPBTR_DP_SJW_Pos    CANFD_DPBTR_DP_SJW_DPos
#define CANFD_DPBTR_DP_SJW_Msk    (0xFUL << CANFD_DPBTR_DP_SJW_Pos)            /* 0x000F0000 */
#define CANFD_DPBTR_DP_SJW        CANFD_DPBTR_DP_SJW_Msk                       /* DP_SJW[3:0] bits (Data Phase Synchronization Jump Width) */
#define CANFD_DPBTR_DP_SJW_0      (0x1UL << CANFD_DPBTR_DP_SJW_Pos)            /* 0x00010000 */
#define CANFD_DPBTR_DP_SJW_1      (0x2UL << CANFD_DPBTR_DP_SJW_Pos)            /* 0x00020000 */
#define CANFD_DPBTR_DP_SJW_2      (0x4UL << CANFD_DPBTR_DP_SJW_Pos)            /* 0x00040000 */
#define CANFD_DPBTR_DP_SJW_3      (0x8UL << CANFD_DPBTR_DP_SJW_Pos)            /* 0x00080000 */

/******************  Bit definition for CANFD_TRR register  *******************/
#define CANFD_TRR_RR_DPos         (0U)
#define CANFD_TRR_RR_Pos          CANFD_TRR_RR_DPos
#define CANFD_TRR_RR_Msk          (0xFFFFFFFFUL << CANFD_TRR_RR_Pos)           /* 0xFFFFFFFF */
#define CANFD_TRR_RR              CANFD_TRR_RR_Msk                             /* RR[31:0] bits (TX Buffer Ready Request) */
#define CANFD_TRR_RR_0            (0x1UL << CANFD_TRR_RR_Pos)                  /* 0x00000001 */
#define CANFD_TRR_RR_1            (0x2UL << CANFD_TRR_RR_Pos)                  /* 0x00000002 */
#define CANFD_TRR_RR_2            (0x4UL << CANFD_TRR_RR_Pos)                  /* 0x00000004 */
#define CANFD_TRR_RR_3            (0x8UL << CANFD_TRR_RR_Pos)                  /* 0x00000008 */
#define CANFD_TRR_RR_4            (0x10UL << CANFD_TRR_RR_Pos)                 /* 0x00000010 */
#define CANFD_TRR_RR_5            (0x20UL << CANFD_TRR_RR_Pos)                 /* 0x00000020 */
#define CANFD_TRR_RR_6            (0x40UL << CANFD_TRR_RR_Pos)                 /* 0x00000040 */
#define CANFD_TRR_RR_7            (0x80UL << CANFD_TRR_RR_Pos)                 /* 0x00000080 */
#define CANFD_TRR_RR_8            (0x100UL << CANFD_TRR_RR_Pos)                /* 0x00000100 */
#define CANFD_TRR_RR_9            (0x200UL << CANFD_TRR_RR_Pos)                /* 0x00000200 */
#define CANFD_TRR_RR_10           (0x400UL << CANFD_TRR_RR_Pos)                /* 0x00000400 */
#define CANFD_TRR_RR_11           (0x800UL << CANFD_TRR_RR_Pos)                /* 0x00000800 */
#define CANFD_TRR_RR_12           (0x1000UL << CANFD_TRR_RR_Pos)               /* 0x00001000 */
#define CANFD_TRR_RR_13           (0x2000UL << CANFD_TRR_RR_Pos)               /* 0x00002000 */
#define CANFD_TRR_RR_14           (0x4000UL << CANFD_TRR_RR_Pos)               /* 0x00004000 */
#define CANFD_TRR_RR_15           (0x8000UL << CANFD_TRR_RR_Pos)               /* 0x00008000 */
#define CANFD_TRR_RR_16           (0x10000UL << CANFD_TRR_RR_Pos)              /* 0x00010000 */
#define CANFD_TRR_RR_17           (0x20000UL << CANFD_TRR_RR_Pos)              /* 0x00020000 */
#define CANFD_TRR_RR_18           (0x40000UL << CANFD_TRR_RR_Pos)              /* 0x00040000 */
#define CANFD_TRR_RR_19           (0x80000UL << CANFD_TRR_RR_Pos)              /* 0x00080000 */
#define CANFD_TRR_RR_20           (0x100000UL << CANFD_TRR_RR_Pos)             /* 0x00100000 */
#define CANFD_TRR_RR_21           (0x200000UL << CANFD_TRR_RR_Pos)             /* 0x00200000 */
#define CANFD_TRR_RR_22           (0x400000UL << CANFD_TRR_RR_Pos)             /* 0x00400000 */
#define CANFD_TRR_RR_23           (0x800000UL << CANFD_TRR_RR_Pos)             /* 0x00800000 */
#define CANFD_TRR_RR_24           (0x1000000UL << CANFD_TRR_RR_Pos)            /* 0x01000000 */
#define CANFD_TRR_RR_25           (0x2000000UL << CANFD_TRR_RR_Pos)            /* 0x02000000 */
#define CANFD_TRR_RR_26           (0x4000000UL << CANFD_TRR_RR_Pos)            /* 0x04000000 */
#define CANFD_TRR_RR_27           (0x8000000UL << CANFD_TRR_RR_Pos)            /* 0x08000000 */
#define CANFD_TRR_RR_28           (0x10000000UL << CANFD_TRR_RR_Pos)           /* 0x10000000 */
#define CANFD_TRR_RR_29           (0x20000000UL << CANFD_TRR_RR_Pos)           /* 0x20000000 */
#define CANFD_TRR_RR_30           (0x40000000UL << CANFD_TRR_RR_Pos)           /* 0x40000000 */
#define CANFD_TRR_RR_31           (0x80000000UL << CANFD_TRR_RR_Pos)           /* 0x80000000 */

/******************  Bit definition for CANFD_ERRS register  ******************/
#define CANFD_ERRS_ERRS_DPos      (0U)
#define CANFD_ERRS_ERRS_Pos       CANFD_ERRS_ERRS_DPos
#define CANFD_ERRS_ERRS_Msk       (0xFFFFFFFFUL << CANFD_ERRS_ERRS_Pos)        /* 0xFFFFFFFF */
#define CANFD_ERRS_ERRS           CANFD_ERRS_ERRS_Msk                          /* ERRS[31:0] bits (TX Buffer Ready Req Served/Cleared Interrupt Enable) */
#define CANFD_ERRS_ERRS_0         (0x1UL << CANFD_ERRS_ERRS_Pos)               /* 0x00000001 */
#define CANFD_ERRS_ERRS_1         (0x2UL << CANFD_ERRS_ERRS_Pos)               /* 0x00000002 */
#define CANFD_ERRS_ERRS_2         (0x4UL << CANFD_ERRS_ERRS_Pos)               /* 0x00000004 */
#define CANFD_ERRS_ERRS_3         (0x8UL << CANFD_ERRS_ERRS_Pos)               /* 0x00000008 */
#define CANFD_ERRS_ERRS_4         (0x10UL << CANFD_ERRS_ERRS_Pos)              /* 0x00000010 */
#define CANFD_ERRS_ERRS_5         (0x20UL << CANFD_ERRS_ERRS_Pos)              /* 0x00000020 */
#define CANFD_ERRS_ERRS_6         (0x40UL << CANFD_ERRS_ERRS_Pos)              /* 0x00000040 */
#define CANFD_ERRS_ERRS_7         (0x80UL << CANFD_ERRS_ERRS_Pos)              /* 0x00000080 */
#define CANFD_ERRS_ERRS_8         (0x100UL << CANFD_ERRS_ERRS_Pos)             /* 0x00000100 */
#define CANFD_ERRS_ERRS_9         (0x200UL << CANFD_ERRS_ERRS_Pos)             /* 0x00000200 */
#define CANFD_ERRS_ERRS_10        (0x400UL << CANFD_ERRS_ERRS_Pos)             /* 0x00000400 */
#define CANFD_ERRS_ERRS_11        (0x800UL << CANFD_ERRS_ERRS_Pos)             /* 0x00000800 */
#define CANFD_ERRS_ERRS_12        (0x1000UL << CANFD_ERRS_ERRS_Pos)            /* 0x00001000 */
#define CANFD_ERRS_ERRS_13        (0x2000UL << CANFD_ERRS_ERRS_Pos)            /* 0x00002000 */
#define CANFD_ERRS_ERRS_14        (0x4000UL << CANFD_ERRS_ERRS_Pos)            /* 0x00004000 */
#define CANFD_ERRS_ERRS_15        (0x8000UL << CANFD_ERRS_ERRS_Pos)            /* 0x00008000 */
#define CANFD_ERRS_ERRS_16        (0x10000UL << CANFD_ERRS_ERRS_Pos)           /* 0x00010000 */
#define CANFD_ERRS_ERRS_17        (0x20000UL << CANFD_ERRS_ERRS_Pos)           /* 0x00020000 */
#define CANFD_ERRS_ERRS_18        (0x40000UL << CANFD_ERRS_ERRS_Pos)           /* 0x00040000 */
#define CANFD_ERRS_ERRS_19        (0x80000UL << CANFD_ERRS_ERRS_Pos)           /* 0x00080000 */
#define CANFD_ERRS_ERRS_20        (0x100000UL << CANFD_ERRS_ERRS_Pos)          /* 0x00100000 */
#define CANFD_ERRS_ERRS_21        (0x200000UL << CANFD_ERRS_ERRS_Pos)          /* 0x00200000 */
#define CANFD_ERRS_ERRS_22        (0x400000UL << CANFD_ERRS_ERRS_Pos)          /* 0x00400000 */
#define CANFD_ERRS_ERRS_23        (0x800000UL << CANFD_ERRS_ERRS_Pos)          /* 0x00800000 */
#define CANFD_ERRS_ERRS_24        (0x1000000UL << CANFD_ERRS_ERRS_Pos)         /* 0x01000000 */
#define CANFD_ERRS_ERRS_25        (0x2000000UL << CANFD_ERRS_ERRS_Pos)         /* 0x02000000 */
#define CANFD_ERRS_ERRS_26        (0x4000000UL << CANFD_ERRS_ERRS_Pos)         /* 0x04000000 */
#define CANFD_ERRS_ERRS_27        (0x8000000UL << CANFD_ERRS_ERRS_Pos)         /* 0x08000000 */
#define CANFD_ERRS_ERRS_28        (0x10000000UL << CANFD_ERRS_ERRS_Pos)        /* 0x10000000 */
#define CANFD_ERRS_ERRS_29        (0x20000000UL << CANFD_ERRS_ERRS_Pos)        /* 0x20000000 */
#define CANFD_ERRS_ERRS_30        (0x40000000UL << CANFD_ERRS_ERRS_Pos)        /* 0x40000000 */
#define CANFD_ERRS_ERRS_31        (0x80000000UL << CANFD_ERRS_ERRS_Pos)        /* 0x80000000 */

/*******************  Bit definition for CANFD_TCR register  ******************/
#define CANFD_TCR_CR_DPos         (0U)
#define CANFD_TCR_CR_Pos          CANFD_TCR_CR_DPos
#define CANFD_TCR_CR_Msk          (0xFFFFFFFFUL << CANFD_TCR_CR_Pos)           /* 0xFFFFFFFF */
#define CANFD_TCR_CR              CANFD_TCR_CR_Msk                             /* CR[31:0] bits (TX Buffer Cancel Request) */
#define CANFD_TCR_CR_0            (0x1UL << CANFD_TCR_CR_Pos)                  /* 0x00000001 */
#define CANFD_TCR_CR_1            (0x2UL << CANFD_TCR_CR_Pos)                  /* 0x00000002 */
#define CANFD_TCR_CR_2            (0x4UL << CANFD_TCR_CR_Pos)                  /* 0x00000004 */
#define CANFD_TCR_CR_3            (0x8UL << CANFD_TCR_CR_Pos)                  /* 0x00000008 */
#define CANFD_TCR_CR_4            (0x10UL << CANFD_TCR_CR_Pos)                 /* 0x00000010 */
#define CANFD_TCR_CR_5            (0x20UL << CANFD_TCR_CR_Pos)                 /* 0x00000020 */
#define CANFD_TCR_CR_6            (0x40UL << CANFD_TCR_CR_Pos)                 /* 0x00000040 */
#define CANFD_TCR_CR_7            (0x80UL << CANFD_TCR_CR_Pos)                 /* 0x00000080 */
#define CANFD_TCR_CR_8            (0x100UL << CANFD_TCR_CR_Pos)                /* 0x00000100 */
#define CANFD_TCR_CR_9            (0x200UL << CANFD_TCR_CR_Pos)                /* 0x00000200 */
#define CANFD_TCR_CR_10           (0x400UL << CANFD_TCR_CR_Pos)                /* 0x00000400 */
#define CANFD_TCR_CR_11           (0x800UL << CANFD_TCR_CR_Pos)                /* 0x00000800 */
#define CANFD_TCR_CR_12           (0x1000UL << CANFD_TCR_CR_Pos)               /* 0x00001000 */
#define CANFD_TCR_CR_13           (0x2000UL << CANFD_TCR_CR_Pos)               /* 0x00002000 */
#define CANFD_TCR_CR_14           (0x4000UL << CANFD_TCR_CR_Pos)               /* 0x00004000 */
#define CANFD_TCR_CR_15           (0x8000UL << CANFD_TCR_CR_Pos)               /* 0x00008000 */
#define CANFD_TCR_CR_16           (0x10000UL << CANFD_TCR_CR_Pos)              /* 0x00010000 */
#define CANFD_TCR_CR_17           (0x20000UL << CANFD_TCR_CR_Pos)              /* 0x00020000 */
#define CANFD_TCR_CR_18           (0x40000UL << CANFD_TCR_CR_Pos)              /* 0x00040000 */
#define CANFD_TCR_CR_19           (0x80000UL << CANFD_TCR_CR_Pos)              /* 0x00080000 */
#define CANFD_TCR_CR_20           (0x100000UL << CANFD_TCR_CR_Pos)             /* 0x00100000 */
#define CANFD_TCR_CR_21           (0x200000UL << CANFD_TCR_CR_Pos)             /* 0x00200000 */
#define CANFD_TCR_CR_22           (0x400000UL << CANFD_TCR_CR_Pos)             /* 0x00400000 */
#define CANFD_TCR_CR_23           (0x800000UL << CANFD_TCR_CR_Pos)             /* 0x00800000 */
#define CANFD_TCR_CR_24           (0x1000000UL << CANFD_TCR_CR_Pos)            /* 0x01000000 */
#define CANFD_TCR_CR_25           (0x2000000UL << CANFD_TCR_CR_Pos)            /* 0x02000000 */
#define CANFD_TCR_CR_26           (0x4000000UL << CANFD_TCR_CR_Pos)            /* 0x04000000 */
#define CANFD_TCR_CR_27           (0x8000000UL << CANFD_TCR_CR_Pos)            /* 0x08000000 */
#define CANFD_TCR_CR_28           (0x10000000UL << CANFD_TCR_CR_Pos)           /* 0x10000000 */
#define CANFD_TCR_CR_29           (0x20000000UL << CANFD_TCR_CR_Pos)           /* 0x20000000 */
#define CANFD_TCR_CR_30           (0x40000000UL << CANFD_TCR_CR_Pos)           /* 0x40000000 */
#define CANFD_TCR_CR_31           (0x80000000UL << CANFD_TCR_CR_Pos)           /* 0x80000000 */

/******************  Bit definition for CANFD_ECRS register  ******************/
#define CANFD_ECRS_ECRS_DPos      (0U)
#define CANFD_ECRS_ECRS_Pos       CANFD_ECRS_ECRS_DPos
#define CANFD_ECRS_ECRS_Msk       (0xFFFFFFFFUL << CANFD_ECRS_ECRS_Pos)        /* 0xFFFFFFFF */
#define CANFD_ECRS_ECRS           CANFD_ECRS_ECRS_Msk                          /* ECRS[31:0] bits (TX Buffer Transmission Served/Cleared Interrupt Enable) */
#define CANFD_ECRS_ECRS_0         (0x1UL << CANFD_ECRS_ECRS_Pos)               /* 0x00000001 */
#define CANFD_ECRS_ECRS_1         (0x2UL << CANFD_ECRS_ECRS_Pos)               /* 0x00000002 */
#define CANFD_ECRS_ECRS_2         (0x4UL << CANFD_ECRS_ECRS_Pos)               /* 0x00000004 */
#define CANFD_ECRS_ECRS_3         (0x8UL << CANFD_ECRS_ECRS_Pos)               /* 0x00000008 */
#define CANFD_ECRS_ECRS_4         (0x10UL << CANFD_ECRS_ECRS_Pos)              /* 0x00000010 */
#define CANFD_ECRS_ECRS_5         (0x20UL << CANFD_ECRS_ECRS_Pos)              /* 0x00000020 */
#define CANFD_ECRS_ECRS_6         (0x40UL << CANFD_ECRS_ECRS_Pos)              /* 0x00000040 */
#define CANFD_ECRS_ECRS_7         (0x80UL << CANFD_ECRS_ECRS_Pos)              /* 0x00000080 */
#define CANFD_ECRS_ECRS_8         (0x100UL << CANFD_ECRS_ECRS_Pos)             /* 0x00000100 */
#define CANFD_ECRS_ECRS_9         (0x200UL << CANFD_ECRS_ECRS_Pos)             /* 0x00000200 */
#define CANFD_ECRS_ECRS_10        (0x400UL << CANFD_ECRS_ECRS_Pos)             /* 0x00000400 */
#define CANFD_ECRS_ECRS_11        (0x800UL << CANFD_ECRS_ECRS_Pos)             /* 0x00000800 */
#define CANFD_ECRS_ECRS_12        (0x1000UL << CANFD_ECRS_ECRS_Pos)            /* 0x00001000 */
#define CANFD_ECRS_ECRS_13        (0x2000UL << CANFD_ECRS_ECRS_Pos)            /* 0x00002000 */
#define CANFD_ECRS_ECRS_14        (0x4000UL << CANFD_ECRS_ECRS_Pos)            /* 0x00004000 */
#define CANFD_ECRS_ECRS_15        (0x8000UL << CANFD_ECRS_ECRS_Pos)            /* 0x00008000 */
#define CANFD_ECRS_ECRS_16        (0x10000UL << CANFD_ECRS_ECRS_Pos)           /* 0x00010000 */
#define CANFD_ECRS_ECRS_17        (0x20000UL << CANFD_ECRS_ECRS_Pos)           /* 0x00020000 */
#define CANFD_ECRS_ECRS_18        (0x40000UL << CANFD_ECRS_ECRS_Pos)           /* 0x00040000 */
#define CANFD_ECRS_ECRS_19        (0x80000UL << CANFD_ECRS_ECRS_Pos)           /* 0x00080000 */
#define CANFD_ECRS_ECRS_20        (0x100000UL << CANFD_ECRS_ECRS_Pos)          /* 0x00100000 */
#define CANFD_ECRS_ECRS_21        (0x200000UL << CANFD_ECRS_ECRS_Pos)          /* 0x00200000 */
#define CANFD_ECRS_ECRS_22        (0x400000UL << CANFD_ECRS_ECRS_Pos)          /* 0x00400000 */
#define CANFD_ECRS_ECRS_23        (0x800000UL << CANFD_ECRS_ECRS_Pos)          /* 0x00800000 */
#define CANFD_ECRS_ECRS_24        (0x1000000UL << CANFD_ECRS_ECRS_Pos)         /* 0x01000000 */
#define CANFD_ECRS_ECRS_25        (0x2000000UL << CANFD_ECRS_ECRS_Pos)         /* 0x02000000 */
#define CANFD_ECRS_ECRS_26        (0x4000000UL << CANFD_ECRS_ECRS_Pos)         /* 0x04000000 */
#define CANFD_ECRS_ECRS_27        (0x8000000UL << CANFD_ECRS_ECRS_Pos)         /* 0x08000000 */
#define CANFD_ECRS_ECRS_28        (0x10000000UL << CANFD_ECRS_ECRS_Pos)        /* 0x10000000 */
#define CANFD_ECRS_ECRS_29        (0x20000000UL << CANFD_ECRS_ECRS_Pos)        /* 0x20000000 */
#define CANFD_ECRS_ECRS_30        (0x40000000UL << CANFD_ECRS_ECRS_Pos)        /* 0x40000000 */
#define CANFD_ECRS_ECRS_31        (0x80000000UL << CANFD_ECRS_ECRS_Pos)        /* 0x80000000 */


/******************  Bit definition for CANFD_AFR register  *******************/
#define CANFD_AFR_UAF_DPos        (0U)
#define CANFD_AFR_UAF_Pos         CANFD_AFR_UAF_DPos
#define CANFD_AFR_UAF_Msk         (0xFFFFFFFFUL << CANFD_AFR_UAF_Pos)  	       /* 0xFFFFFFFF */
#define CANFD_AFR_UAF             CANFD_AFR_UAF_Msk                		       /* UAF[31:0] bits (Use Acceptance Filter Mask Pair) */
#define CANFD_AFR_UAF_0           (0x1UL << CANFD_AFR_UAF_Pos)     		       /* 0x00000001 */
#define CANFD_AFR_UAF_1           (0x2UL << CANFD_AFR_UAF_Pos)     		       /* 0x00000002 */
#define CANFD_AFR_UAF_2           (0x4UL << CANFD_AFR_UAF_Pos)     		       /* 0x00000004 */
#define CANFD_AFR_UAF_3           (0x8UL << CANFD_AFR_UAF_Pos)     		       /* 0x00000008 */
#define CANFD_AFR_UAF_4           (0x10UL << CANFD_AFR_UAF_Pos)    		       /* 0x00000010 */
#define CANFD_AFR_UAF_5           (0x20UL << CANFD_AFR_UAF_Pos)    		       /* 0x00000020 */
#define CANFD_AFR_UAF_6           (0x40UL << CANFD_AFR_UAF_Pos)    		       /* 0x00000040 */
#define CANFD_AFR_UAF_7           (0x80UL << CANFD_AFR_UAF_Pos)    		       /* 0x00000080 */
#define CANFD_AFR_UAF_8           (0x100UL << CANFD_AFR_UAF_Pos)   		       /* 0x00000100 */
#define CANFD_AFR_UAF_9           (0x200UL << CANFD_AFR_UAF_Pos)   		       /* 0x00000200 */
#define CANFD_AFR_UAF_10          (0x400UL << CANFD_AFR_UAF_Pos)   		       /* 0x00000400 */
#define CANFD_AFR_UAF_11          (0x800UL << CANFD_AFR_UAF_Pos)   		       /* 0x00000800 */
#define CANFD_AFR_UAF_12          (0x1000UL << CANFD_AFR_UAF_Pos)  		       /* 0x00001000 */
#define CANFD_AFR_UAF_13          (0x2000UL << CANFD_AFR_UAF_Pos)  		       /* 0x00002000 */
#define CANFD_AFR_UAF_14          (0x4000UL << CANFD_AFR_UAF_Pos)  		       /* 0x00004000 */
#define CANFD_AFR_UAF_15          (0x8000UL << CANFD_AFR_UAF_Pos)  		       /* 0x00008000 */
#define CANFD_AFR_UAF_16          (0x10000UL << CANFD_AFR_UAF_Pos)     	       /* 0x00010000 */
#define CANFD_AFR_UAF_17          (0x20000UL << CANFD_AFR_UAF_Pos)     	       /* 0x00020000 */
#define CANFD_AFR_UAF_18          (0x40000UL << CANFD_AFR_UAF_Pos)     	       /* 0x00040000 */
#define CANFD_AFR_UAF_19          (0x80000UL << CANFD_AFR_UAF_Pos)     	       /* 0x00080000 */
#define CANFD_AFR_UAF_20          (0x100000UL << CANFD_AFR_UAF_Pos)    	       /* 0x00100000 */
#define CANFD_AFR_UAF_21          (0x200000UL << CANFD_AFR_UAF_Pos)    	       /* 0x00200000 */
#define CANFD_AFR_UAF_22          (0x400000UL << CANFD_AFR_UAF_Pos)    	       /* 0x00400000 */
#define CANFD_AFR_UAF_23          (0x800000UL << CANFD_AFR_UAF_Pos)    	       /* 0x00800000 */
#define CANFD_AFR_UAF_24          (0x1000000UL << CANFD_AFR_UAF_Pos)   	       /* 0x01000000 */
#define CANFD_AFR_UAF_25          (0x2000000UL << CANFD_AFR_UAF_Pos)   	       /* 0x02000000 */
#define CANFD_AFR_UAF_26          (0x4000000UL << CANFD_AFR_UAF_Pos)   	       /* 0x04000000 */
#define CANFD_AFR_UAF_27          (0x8000000UL << CANFD_AFR_UAF_Pos)   	       /* 0x08000000 */
#define CANFD_AFR_UAF_28          (0x10000000UL << CANFD_AFR_UAF_Pos)  	       /* 0x10000000 */
#define CANFD_AFR_UAF_29          (0x20000000UL << CANFD_AFR_UAF_Pos)  	       /* 0x20000000 */
#define CANFD_AFR_UAF_30          (0x40000000UL << CANFD_AFR_UAF_Pos)  	       /* 0x40000000 */
#define CANFD_AFR_UAF_31          (0x80000000UL << CANFD_AFR_UAF_Pos)  	       /* 0x80000000 */

/******************  Bit definition for CANFD_RFSR register  ******************/
#define CANFD_RFSR_RI_DPos        (0U)
#define CANFD_RFSR_RI_Pos         CANFD_RFSR_RI_DPos
#define CANFD_RFSR_RI_Msk         (0x3FUL << CANFD_RFSR_RI_Pos)                /* 0x0000001F */
#define CANFD_RFSR_RI             CANFD_RFSR_RI_Msk                            /* RI[5:0] bits (RX FIFO Read Index (0 to 63).) */
#define CANFD_RFSR_RI_0           (0x1UL << CANFD_RFSR_RI_Pos)                 /* 0x00000001 */
#define CANFD_RFSR_RI_1           (0x2UL << CANFD_RFSR_RI_Pos)                 /* 0x00000002 */
#define CANFD_RFSR_RI_2           (0x4UL << CANFD_RFSR_RI_Pos)                 /* 0x00000004 */
#define CANFD_RFSR_RI_3           (0x8UL << CANFD_RFSR_RI_Pos)                 /* 0x00000008 */
#define CANFD_RFSR_RI_4           (0x10UL << CANFD_RFSR_RI_Pos)                /* 0x00000010 */
#define CANFD_RFSR_RI_5           (0x20UL << CANFD_RFSR_RI_Pos)                /* 0x00000020 */

#define CANFD_RFSR_IRI_DPos       (7U)
#define CANFD_RFSR_IRI_Pos        CANFD_RFSR_IRI_DPos
#define CANFD_RFSR_IRI_Msk        (0x1UL << CANFD_RFSR_IRI_Pos)                /* 0x00000080 */
#define CANFD_RFSR_IRI            CANFD_RFSR_IRI_Msk                           /* RX FIFO Increment Read Index by 1. */

#define CANFD_RFSR_FL_DPos        (8U)
#define CANFD_RFSR_FL_Pos         CANFD_RFSR_FL_DPos
#define CANFD_RFSR_FL_Msk         (0x7FUL << CANFD_RFSR_FL_Pos)                /* 0x00003F00 */
#define CANFD_RFSR_FL             CANFD_RFSR_FL_Msk                            /* FL[5:0] bits (RX FIFO Fill Level (0 to 64).) */
#define CANFD_RFSR_FL_0           (0x1UL << CANFD_RFSR_FL_Pos)                 /* 0x00000100 */
#define CANFD_RFSR_FL_1           (0x2UL << CANFD_RFSR_FL_Pos)                 /* 0x00000200 */
#define CANFD_RFSR_FL_2           (0x4UL << CANFD_RFSR_FL_Pos)                 /* 0x00000400 */
#define CANFD_RFSR_FL_3           (0x8UL << CANFD_RFSR_FL_Pos)                 /* 0x00000800 */
#define CANFD_RFSR_FL_4           (0x10UL << CANFD_RFSR_FL_Pos)                /* 0x00001000 */
#define CANFD_RFSR_FL_5           (0x20UL << CANFD_RFSR_FL_Pos)                /* 0x00002000 */
#define CANFD_RFSR_FL_6           (0x40UL << CANFD_RFSR_FL_Pos)                /* 0x00004000 */

/******************  Bit definition for CANFD_TB_ID register  *****************/
#define CANFD_TB_ID_RTR_DPos      (0U)
#define CANFD_TB_ID_RTR_Pos       CANFD_TB_ID_RTR_DPos
#define CANFD_TB_ID_RTR_Msk       (0x1UL << CANFD_TB_ID_RTR_Pos)               /* 0x00000001 */
#define CANFD_TB_ID_RTR           CANFD_TB_ID_RTR_Msk                          /* Remote Transmission Request. */

#define CANFD_TB_ID_EM_ID_DPos    (1U)
#define CANFD_TB_ID_EM_ID_Pos     CANFD_TB_ID_EM_ID_DPos
#define CANFD_TB_ID_EM_ID_Msk 	  (0x3FFFFUL << CANFD_TB_ID_EM_ID_Pos)         /* 0x0007FFFE */
#define CANFD_TB_ID_EM_ID     	  CANFD_TB_ID_EM_ID_Msk                        /* ID[17:0] bits (Extended Message ID.) */
#define CANFD_TB_ID_EM_ID_0   	  (0x1UL << CANFD_TB_ID_EM_ID_Pos)             /* 0x00000002 */
#define CANFD_TB_ID_EM_ID_1   	  (0x2UL << CANFD_TB_ID_EM_ID_Pos)             /* 0x00000004 */
#define CANFD_TB_ID_EM_ID_2   	  (0x4UL << CANFD_TB_ID_EM_ID_Pos)             /* 0x00000008 */
#define CANFD_TB_ID_EM_ID_3   	  (0x8UL << CANFD_TB_ID_EM_ID_Pos)             /* 0x00000010 */
#define CANFD_TB_ID_EM_ID_4   	  (0x10UL << CANFD_TB_ID_EM_ID_Pos)            /* 0x00000020 */
#define CANFD_TB_ID_EM_ID_5   	  (0x20UL << CANFD_TB_ID_EM_ID_Pos)            /* 0x00000040 */
#define CANFD_TB_ID_EM_ID_6   	  (0x40UL << CANFD_TB_ID_EM_ID_Pos)            /* 0x00000080 */
#define CANFD_TB_ID_EM_ID_7   	  (0x80UL << CANFD_TB_ID_EM_ID_Pos)            /* 0x00000100 */
#define CANFD_TB_ID_EM_ID_8   	  (0x100UL << CANFD_TB_ID_EM_ID_Pos)           /* 0x00000200 */
#define CANFD_TB_ID_EM_ID_9   	  (0x200UL << CANFD_TB_ID_EM_ID_Pos)           /* 0x00000400 */
#define CANFD_TB_ID_EM_ID_10  	  (0x400UL << CANFD_TB_ID_EM_ID_Pos)           /* 0x00000800 */
#define CANFD_TB_ID_EM_ID_11  	  (0x800UL << CANFD_TB_ID_EM_ID_Pos)           /* 0x00001000 */
#define CANFD_TB_ID_EM_ID_12  	  (0x1000UL << CANFD_TB_ID_EM_ID_Pos)          /* 0x00002000 */
#define CANFD_TB_ID_EM_ID_13  	  (0x2000UL << CANFD_TB_ID_EM_ID_Pos)          /* 0x00004000 */
#define CANFD_TB_ID_EM_ID_14  	  (0x4000UL << CANFD_TB_ID_EM_ID_Pos)          /* 0x00008000 */
#define CANFD_TB_ID_EM_ID_15  	  (0x8000UL << CANFD_TB_ID_EM_ID_Pos)          /* 0x00010000 */
#define CANFD_TB_ID_EM_ID_16  	  (0x10000UL << CANFD_TB_ID_EM_ID_Pos)         /* 0x00020000 */
#define CANFD_TB_ID_EM_ID_17  	  (0x20000UL << CANFD_TB_ID_EM_ID_Pos)         /* 0x00040000 */

#define CANFD_TB_ID_IDE_DPos      (19U)
#define CANFD_TB_ID_IDE_Pos       CANFD_TB_ID_IDE_DPos
#define CANFD_TB_ID_IDE_Msk       (0x1UL << CANFD_TB_ID_IDE_Pos)               /* 0x00080000 */
#define CANFD_TB_ID_IDE           CANFD_TB_ID_IDE_Msk                          /* Identifier Extension. */

#define CANFD_TB_ID_SRR_DPos      (20U)
#define CANFD_TB_ID_SRR_Pos       CANFD_TB_ID_SRR_DPos
#define CANFD_TB_ID_SRR_Msk       (0x1UL << CANFD_TB_ID_SRR_Pos)               /* 0x00100000 */
#define CANFD_TB_ID_SRR           CANFD_TB_ID_SRR_Msk                          /* Substitute Remote Transmission Request. */

#define CANFD_TB_ID_ID_DPos       (21U)
#define CANFD_TB_ID_ID_Pos        CANFD_TB_ID_ID_DPos
#define CANFD_TB_ID_ID_Msk 	      (0x7FFUL << CANFD_TB_ID_ID_Pos)              /* 0xFFE00000 */
#define CANFD_TB_ID_ID     	      CANFD_TB_ID_ID_Msk                           /* ID[28:18] bits (Standard Message ID.) */
#define CANFD_TB_ID_ID_0   	      (0x1UL << CANFD_TB_ID_ID_Pos)                /* 0x00200000 */
#define CANFD_TB_ID_ID_1   	      (0x2UL << CANFD_TB_ID_ID_Pos)                /* 0x00400000 */
#define CANFD_TB_ID_ID_2   	      (0x4UL << CANFD_TB_ID_ID_Pos)                /* 0x00800000 */
#define CANFD_TB_ID_ID_3   	      (0x8UL << CANFD_TB_ID_ID_Pos)                /* 0x01000000 */
#define CANFD_TB_ID_ID_4   	      (0x10UL << CANFD_TB_ID_ID_Pos)               /* 0x02000000 */
#define CANFD_TB_ID_ID_5   	      (0x20UL << CANFD_TB_ID_ID_Pos)               /* 0x04000000 */
#define CANFD_TB_ID_ID_6   	      (0x40UL << CANFD_TB_ID_ID_Pos)               /* 0x08000000 */
#define CANFD_TB_ID_ID_7   	      (0x80UL << CANFD_TB_ID_ID_Pos)               /* 0x10000000 */
#define CANFD_TB_ID_ID_8   	      (0x100UL << CANFD_TB_ID_ID_Pos)              /* 0x20000000 */
#define CANFD_TB_ID_ID_9   	      (0x200UL << CANFD_TB_ID_ID_Pos)              /* 0x40000000 */
#define CANFD_TB_ID_ID_10  	      (0x400UL << CANFD_TB_ID_ID_Pos)              /* 0x80000000 */

/*****************  Bit definition for CANFD_TB_DLC register  *****************/
#define CANFD_TB_DLC_MM_DPos      (16U)
#define CANFD_TB_DLC_MM_Pos       CANFD_TB_DLC_MM_DPos
#define CANFD_TB_DLC_MM_Msk 	  (0xFFUL << CANFD_TB_DLC_MM_Pos)              /* 0x00FF0000 */
#define CANFD_TB_DLC_MM     	  CANFD_TB_DLC_MM_Msk                          /* MM[7:0] bits (Written by CPU during TX Buffer configuration.) */
#define CANFD_TB_DLC_MM_0   	  (0x1UL << CANFD_TB_DLC_MM_Pos)               /* 0x00010000 */
#define CANFD_TB_DLC_MM_1   	  (0x2UL << CANFD_TB_DLC_MM_Pos)               /* 0x00020000 */
#define CANFD_TB_DLC_MM_2   	  (0x4UL << CANFD_TB_DLC_MM_Pos)               /* 0x00040000 */
#define CANFD_TB_DLC_MM_3   	  (0x8UL << CANFD_TB_DLC_MM_Pos)               /* 0x00080000 */
#define CANFD_TB_DLC_MM_4   	  (0x10UL << CANFD_TB_DLC_MM_Pos)              /* 0x00100000 */
#define CANFD_TB_DLC_MM_5   	  (0x20UL << CANFD_TB_DLC_MM_Pos)              /* 0x00200000 */
#define CANFD_TB_DLC_MM_6   	  (0x40UL << CANFD_TB_DLC_MM_Pos)              /* 0x00400000 */
#define CANFD_TB_DLC_MM_7   	  (0x80UL << CANFD_TB_DLC_MM_Pos)              /* 0x00800000 */

#define CANFD_TB_DLC_EFC_DPos     (24U)
#define CANFD_TB_DLC_EFC_Pos      CANFD_TB_DLC_EFC_DPos
#define CANFD_TB_DLC_EFC_Msk      (0x1UL << CANFD_TB_DLC_EFC_Pos)              /* 0x01000000 */
#define CANFD_TB_DLC_EFC          CANFD_TB_DLC_EFC_Msk                         /* Event FIFO Control. */

#define CANFD_TB_DLC_BRS_DPos     (26U)
#define CANFD_TB_DLC_BRS_Pos      CANFD_TB_DLC_BRS_DPos
#define CANFD_TB_DLC_BRS_Msk      (0x1UL << CANFD_TB_DLC_BRS_Pos)              /* 0x04000000 */
#define CANFD_TB_DLC_BRS          CANFD_TB_DLC_BRS_Msk                         /* Bit Rate Switch */

#define CANFD_TB_DLC_EDL_DPos     (27U)
#define CANFD_TB_DLC_EDL_Pos      CANFD_TB_DLC_EDL_DPos
#define CANFD_TB_DLC_EDL_Msk      (0x1UL << CANFD_TB_DLC_EDL_Pos)              /* 0x08000000 */
#define CANFD_TB_DLC_EDL          CANFD_TB_DLC_EDL_Msk                         /* Extended Data Length/FD Frame Format. */

#define CANFD_TB_DLC_DLC_DPos     (28U)
#define CANFD_TB_DLC_DLC_Pos      CANFD_TB_DLC_DLC_DPos
#define CANFD_TB_DLC_DLC_Msk 	  (0xFUL << CANFD_TB_DLC_DLC_Pos)              /* 0xF0000000 */
#define CANFD_TB_DLC_DLC     	  CANFD_TB_DLC_DLC_Msk                         /* DLC[3:0] bits (Data Length Code.) */
#define CANFD_TB_DLC_DLC_0   	  (0x1UL << CANFD_TB_DLC_DLC_Pos)              /* 0x10000000 */
#define CANFD_TB_DLC_DLC_1   	  (0x2UL << CANFD_TB_DLC_DLC_Pos)              /* 0x20000000 */
#define CANFD_TB_DLC_DLC_2   	  (0x4UL << CANFD_TB_DLC_DLC_Pos)              /* 0x40000000 */
#define CANFD_TB_DLC_DLC_3   	  (0x8UL << CANFD_TB_DLC_DLC_Pos)              /* 0x80000000 */

/*****************  Bit definition for CANFD_TB_DW register  ******************/
#define CANFD_TB_DW_DB_DPos       (0U)
#define CANFD_TB_DW_DB_Pos        CANFD_TB_DW_DB_DPos
#define CANFD_TB_DW_DB_Msk 	      (0xFFUL << CANFD_TB_DW_DB_Pos)               /* 0x000000FF */
#define CANFD_TB_DW_DB     	      CANFD_TB_DW_DB_Msk                           /* Data bytes[7:0] bits (Data Byte.) */
#define CANFD_TB_DW_DB_0   	      (0x1UL << CANFD_TB_DW_DB_Pos)                /* 0x00000001 */
#define CANFD_TB_DW_DB_1   	      (0x2UL << CANFD_TB_DW_DB_Pos)                /* 0x00000002 */
#define CANFD_TB_DW_DB_2   	      (0x4UL << CANFD_TB_DW_DB_Pos)                /* 0x00000004 */
#define CANFD_TB_DW_DB_3   	      (0x8UL << CANFD_TB_DW_DB_Pos)                /* 0x00000008 */
#define CANFD_TB_DW_DB_4   	      (0x10UL << CANFD_TB_DW_DB_Pos)               /* 0x00000010 */
#define CANFD_TB_DW_DB_5   	      (0x20UL << CANFD_TB_DW_DB_Pos)               /* 0x00000020 */
#define CANFD_TB_DW_DB_6   	      (0x40UL << CANFD_TB_DW_DB_Pos)               /* 0x00000040 */
#define CANFD_TB_DW_DB_7   	      (0x80UL << CANFD_TB_DW_DB_Pos)               /* 0x00000080 */


/*******************  Bit definition for CANFD_AFMR register  *****************/
#define CANFD_AFMR_AMRTR_DPos     (0U)
#define CANFD_AFMR_AMRTR_Pos      CANFD_AFMR_AMRTR_DPos
#define CANFD_AFMR_AMRTR_Msk      (0x1UL << CANFD_AFMR_AMRTR_Pos)              /* 0x00000001 */
#define CANFD_AFMR_AMRTR          CANFD_AFMR_AMRTR_Msk                         /* Remote Transmission Request Mask. */

#define CANFD_AFMR_EM_AMID_DPos   (1U)
#define CANFD_AFMR_EM_AMID_Pos    CANFD_AFMR_EM_AMID_DPos
#define CANFD_AFMR_EM_AMID_Msk 	  (0x3FFFFUL << CANFD_AFMR_EM_AMID_Pos)        /* 0x0007FFFE */
#define CANFD_AFMR_EM_AMID     	  CANFD_AFMR_EM_AMID_Msk                       /* AMID[17:0] bits (Extended Message ID Mask) */
#define CANFD_AFMR_EM_AMID_0   	  (0x1UL << CANFD_AFMR_EM_AMID_Pos)            /* 0x00000002 */
#define CANFD_AFMR_EM_AMID_1   	  (0x2UL << CANFD_AFMR_EM_AMID_Pos)            /* 0x00000004 */
#define CANFD_AFMR_EM_AMID_2   	  (0x4UL << CANFD_AFMR_EM_AMID_Pos)            /* 0x00000008 */
#define CANFD_AFMR_EM_AMID_3   	  (0x8UL << CANFD_AFMR_EM_AMID_Pos)            /* 0x00000010 */
#define CANFD_AFMR_EM_AMID_4   	  (0x10UL << CANFD_AFMR_EM_AMID_Pos)           /* 0x00000020 */
#define CANFD_AFMR_EM_AMID_5   	  (0x20UL << CANFD_AFMR_EM_AMID_Pos)           /* 0x00000040 */
#define CANFD_AFMR_EM_AMID_6   	  (0x40UL << CANFD_AFMR_EM_AMID_Pos)           /* 0x00000080 */
#define CANFD_AFMR_EM_AMID_7   	  (0x80UL << CANFD_AFMR_EM_AMID_Pos)           /* 0x00000100 */
#define CANFD_AFMR_EM_AMID_8   	  (0x100UL << CANFD_AFMR_EM_AMID_Pos)          /* 0x00000200 */
#define CANFD_AFMR_EM_AMID_9   	  (0x200UL << CANFD_AFMR_EM_AMID_Pos)          /* 0x00000400 */
#define CANFD_AFMR_EM_AMID_10  	  (0x400UL << CANFD_AFMR_EM_AMID_Pos)          /* 0x00000800 */
#define CANFD_AFMR_EM_AMID_11  	  (0x800UL << CANFD_AFMR_EM_AMID_Pos)          /* 0x00001000 */
#define CANFD_AFMR_EM_AMID_12  	  (0x1000UL << CANFD_AFMR_EM_AMID_Pos)         /* 0x00002000 */
#define CANFD_AFMR_EM_AMID_13  	  (0x2000UL << CANFD_AFMR_EM_AMID_Pos)         /* 0x00004000 */
#define CANFD_AFMR_EM_AMID_14  	  (0x4000UL << CANFD_AFMR_EM_AMID_Pos)         /* 0x00008000 */
#define CANFD_AFMR_EM_AMID_15  	  (0x8000UL << CANFD_AFMR_EM_AMID_Pos)         /* 0x00010000 */
#define CANFD_AFMR_EM_AMID_16  	  (0x10000UL << CANFD_AFMR_EM_AMID_Pos)        /* 0x00020000 */
#define CANFD_AFMR_EM_AMID_17  	  (0x20000UL << CANFD_AFMR_EM_AMID_Pos)        /* 0x00040000 */

#define CANFD_AFMR_AMIDE_DPos     (19U)
#define CANFD_AFMR_AMIDE_Pos      CANFD_AFMR_AMIDE_DPos
#define CANFD_AFMR_AMIDE_Msk      (0x1UL << CANFD_AFMR_AMIDE_Pos)              /* 0x00080000 */
#define CANFD_AFMR_AMIDE          CANFD_AFMR_AMIDE_Msk                         /* Identifier Extension Mask */

#define CANFD_AFMR_AMSRR_DPos     (20U)
#define CANFD_AFMR_AMSRR_Pos      CANFD_AFMR_AMSRR_DPos
#define CANFD_AFMR_AMSRR_Msk      (0x1UL << CANFD_AFMR_AMSRR_Pos)              /* 0x00100000 */
#define CANFD_AFMR_AMSRR          CANFD_AFMR_AMSRR_Msk                         /* Substitute Remote Transmission Request Mask. */

#define CANFD_AFMR_AMID_DPos      (21U)
#define CANFD_AFMR_AMID_Pos       CANFD_AFMR_AMID_DPos
#define CANFD_AFMR_AMID_Msk 	  (0x7FFUL << CANFD_AFMR_AMID_Pos)             /* 0xFFE00000 */
#define CANFD_AFMR_AMID     	  CANFD_AFMR_AMID_Msk                          /* AMID[28:18] bits (Standard Message ID Mask .) */
#define CANFD_AFMR_AMID_0   	  (0x1UL << CANFD_AFMR_AMID_Pos)               /* 0x00200000 */
#define CANFD_AFMR_AMID_1   	  (0x2UL << CANFD_AFMR_AMID_Pos)               /* 0x00400000 */
#define CANFD_AFMR_AMID_2   	  (0x4UL << CANFD_AFMR_AMID_Pos)               /* 0x00800000 */
#define CANFD_AFMR_AMID_3   	  (0x8UL << CANFD_AFMR_AMID_Pos)               /* 0x01000000 */
#define CANFD_AFMR_AMID_4   	  (0x10UL << CANFD_AFMR_AMID_Pos)              /* 0x02000000 */
#define CANFD_AFMR_AMID_5   	  (0x20UL << CANFD_AFMR_AMID_Pos)              /* 0x04000000 */
#define CANFD_AFMR_AMID_6   	  (0x40UL << CANFD_AFMR_AMID_Pos)              /* 0x08000000 */
#define CANFD_AFMR_AMID_7   	  (0x80UL << CANFD_AFMR_AMID_Pos)              /* 0x10000000 */
#define CANFD_AFMR_AMID_8   	  (0x100UL << CANFD_AFMR_AMID_Pos)             /* 0x20000000 */
#define CANFD_AFMR_AMID_9   	  (0x200UL << CANFD_AFMR_AMID_Pos)             /* 0x40000000 */
#define CANFD_AFMR_AMID_10  	  (0x400UL << CANFD_AFMR_AMID_Pos)             /* 0x80000000 */

/*******************  Bit definition for CANFD_AFIR register  *****************/
#define CANFD_AFIR_AIRTR_DPos     (0U)
#define CANFD_AFIR_AIRTR_Pos      CANFD_AFIR_AIRTR_DPos
#define CANFD_AFIR_AIRTR_Msk      (0x1UL << CANFD_AFIR_AIRTR_Pos)              /* 0x00000001 */
#define CANFD_AFIR_AIRTR          CANFD_AFIR_AIRTR_Msk                         /* Remote Transmission Request. */

#define CANFD_AFIR_EM_AIID_DPos   (1U)
#define CANFD_AFIR_EM_AIID_Pos    CANFD_AFIR_EM_AIID_DPos
#define CANFD_AFIR_EM_AIID_Msk 	  (0x3FFFFUL << CANFD_AFIR_EM_AIID_Pos)        /* 0x0007FFFE */
#define CANFD_AFIR_EM_AIID     	  CANFD_AFIR_EM_AIID_Msk                       /* AIID[17:0] bits (Extended Message ID.) */
#define CANFD_AFIR_EM_AIID_0   	  (0x1UL << CANFD_AFIR_EM_AIID_Pos)            /* 0x00000002 */
#define CANFD_AFIR_EM_AIID_1   	  (0x2UL << CANFD_AFIR_EM_AIID_Pos)            /* 0x00000004 */
#define CANFD_AFIR_EM_AIID_2   	  (0x4UL << CANFD_AFIR_EM_AIID_Pos)            /* 0x00000008 */
#define CANFD_AFIR_EM_AIID_3   	  (0x8UL << CANFD_AFIR_EM_AIID_Pos)            /* 0x00000010 */
#define CANFD_AFIR_EM_AIID_4   	  (0x10UL << CANFD_AFIR_EM_AIID_Pos)           /* 0x00000020 */
#define CANFD_AFIR_EM_AIID_5   	  (0x20UL << CANFD_AFIR_EM_AIID_Pos)           /* 0x00000040 */
#define CANFD_AFIR_EM_AIID_6   	  (0x40UL << CANFD_AFIR_EM_AIID_Pos)           /* 0x00000080 */
#define CANFD_AFIR_EM_AIID_7   	  (0x80UL << CANFD_AFIR_EM_AIID_Pos)           /* 0x00000100 */
#define CANFD_AFIR_EM_AIID_8   	  (0x100UL << CANFD_AFIR_EM_AIID_Pos)          /* 0x00000200 */
#define CANFD_AFIR_EM_AIID_9   	  (0x200UL << CANFD_AFIR_EM_AIID_Pos)          /* 0x00000400 */
#define CANFD_AFIR_EM_AIID_10  	  (0x400UL << CANFD_AFIR_EM_AIID_Pos)          /* 0x00000800 */
#define CANFD_AFIR_EM_AIID_11  	  (0x800UL << CANFD_AFIR_EM_AIID_Pos)          /* 0x00001000 */
#define CANFD_AFIR_EM_AIID_12  	  (0x1000UL << CANFD_AFIR_EM_AIID_Pos)         /* 0x00002000 */
#define CANFD_AFIR_EM_AIID_13  	  (0x2000UL << CANFD_AFIR_EM_AIID_Pos)         /* 0x00004000 */
#define CANFD_AFIR_EM_AIID_14  	  (0x4000UL << CANFD_AFIR_EM_AIID_Pos)         /* 0x00008000 */
#define CANFD_AFIR_EM_AIID_15  	  (0x8000UL << CANFD_AFIR_EM_AIID_Pos)         /* 0x00010000 */
#define CANFD_AFIR_EM_AIID_16  	  (0x10000UL << CANFD_AFIR_EM_AIID_Pos)        /* 0x00020000 */
#define CANFD_AFIR_EM_AIID_17  	  (0x20000UL << CANFD_AFIR_EM_AIID_Pos)        /* 0x00040000 */

#define CANFD_AFIR_AIIDE_DPos     (19U)
#define CANFD_AFIR_AIIDE_Pos      CANFD_AFIR_AIIDE_DPos
#define CANFD_AFIR_AIIDE_Msk      (0x1UL << CANFD_AFIR_AIIDE_Pos)              /* 0x00080000 */
#define CANFD_AFIR_AIIDE          CANFD_AFIR_AIIDE_Msk                         /* Identifier Extension. */

#define CANFD_AFIR_AISRR_DPos     (20U)
#define CANFD_AFIR_AISRR_Pos      CANFD_AFIR_AISRR_DPos
#define CANFD_AFIR_AISRR_Msk      (0x1UL << CANFD_AFIR_AISRR_Pos)              /* 0x00100000 */
#define CANFD_AFIR_AISRR          CANFD_AFIR_AISRR_Msk                         /* Substitute Remote Transmission Request. */

#define CANFD_AFIR_AIID_DPos      (21U)
#define CANFD_AFIR_AIID_Pos       CANFD_AFIR_AIID_DPos
#define CANFD_AFIR_AIID_Msk 	  (0x7FFUL << CANFD_AFIR_AIID_Pos)             /* 0xFFE00000 */
#define CANFD_AFIR_AIID     	  CANFD_AFIR_AIID_Msk                          /* AIID[28:18] bits (Standard Message ID.) */
#define CANFD_AFIR_AIID_0   	  (0x1UL << CANFD_AFIR_AIID_Pos)               /* 0x00200000 */
#define CANFD_AFIR_AIID_1   	  (0x2UL << CANFD_AFIR_AIID_Pos)               /* 0x00400000 */
#define CANFD_AFIR_AIID_2   	  (0x4UL << CANFD_AFIR_AIID_Pos)               /* 0x00800000 */
#define CANFD_AFIR_AIID_3   	  (0x8UL << CANFD_AFIR_AIID_Pos)               /* 0x01000000 */
#define CANFD_AFIR_AIID_4   	  (0x10UL << CANFD_AFIR_AIID_Pos)              /* 0x02000000 */
#define CANFD_AFIR_AIID_5   	  (0x20UL << CANFD_AFIR_AIID_Pos)              /* 0x04000000 */
#define CANFD_AFIR_AIID_6   	  (0x40UL << CANFD_AFIR_AIID_Pos)              /* 0x08000000 */
#define CANFD_AFIR_AIID_7   	  (0x80UL << CANFD_AFIR_AIID_Pos)              /* 0x10000000 */
#define CANFD_AFIR_AIID_8   	  (0x100UL << CANFD_AFIR_AIID_Pos)             /* 0x20000000 */
#define CANFD_AFIR_AIID_9   	  (0x200UL << CANFD_AFIR_AIID_Pos)             /* 0x40000000 */
#define CANFD_AFIR_AIID_10  	  (0x400UL << CANFD_AFIR_AIID_Pos)             /* 0x80000000 */

/******************  Bit definition for CANFD_RB_ID register  *****************/
#define CANFD_RB_ID_RTR_DPos      (0U)
#define CANFD_RB_ID_RTR_Pos       CANFD_RB_ID_RTR_DPos
#define CANFD_RB_ID_RTR_Msk       (0x1UL << CANFD_RB_ID_RTR_Pos)               /* 0x00000001 */
#define CANFD_RB_ID_RTR           CANFD_RB_ID_RTR_Msk                          /* Remote Transmission Request. */

#define CANFD_RB_ID_EM_ID_DPos   (1U)
#define CANFD_RB_ID_EM_ID_Pos    CANFD_RB_ID_EM_ID_DPos
#define CANFD_RB_ID_EM_ID_Msk 	  (0x3FFFFUL << CANFD_RB_ID_EM_ID_Pos)         /* 0x0007FFFE */
#define CANFD_RB_ID_EM_ID     	  CANFD_RB_ID_EM_ID_Msk                        /* ID[17:0] bits (Extended Message ID.) */
#define CANFD_RB_ID_EM_ID_0   	  (0x1UL << CANFD_RB_ID_EM_ID_Pos)             /* 0x00000002 */
#define CANFD_RB_ID_EM_ID_1   	  (0x2UL << CANFD_RB_ID_EM_ID_Pos)             /* 0x00000004 */
#define CANFD_RB_ID_EM_ID_2   	  (0x4UL << CANFD_RB_ID_EM_ID_Pos)             /* 0x00000008 */
#define CANFD_RB_ID_EM_ID_3   	  (0x8UL << CANFD_RB_ID_EM_ID_Pos)             /* 0x00000010 */
#define CANFD_RB_ID_EM_ID_4   	  (0x10UL << CANFD_RB_ID_EM_ID_Pos)            /* 0x00000020 */
#define CANFD_RB_ID_EM_ID_5   	  (0x20UL << CANFD_RB_ID_EM_ID_Pos)            /* 0x00000040 */
#define CANFD_RB_ID_EM_ID_6   	  (0x40UL << CANFD_RB_ID_EM_ID_Pos)            /* 0x00000080 */
#define CANFD_RB_ID_EM_ID_7   	  (0x80UL << CANFD_RB_ID_EM_ID_Pos)            /* 0x00000100 */
#define CANFD_RB_ID_EM_ID_8   	  (0x100UL << CANFD_RB_ID_EM_ID_Pos)           /* 0x00000200 */
#define CANFD_RB_ID_EM_ID_9   	  (0x200UL << CANFD_RB_ID_EM_ID_Pos)           /* 0x00000400 */
#define CANFD_RB_ID_EM_ID_10  	  (0x400UL << CANFD_RB_ID_EM_ID_Pos)           /* 0x00000800 */
#define CANFD_RB_ID_EM_ID_11  	  (0x800UL << CANFD_RB_ID_EM_ID_Pos)           /* 0x00001000 */
#define CANFD_RB_ID_EM_ID_12  	  (0x1000UL << CANFD_RB_ID_EM_ID_Pos)          /* 0x00002000 */
#define CANFD_RB_ID_EM_ID_13  	  (0x2000UL << CANFD_RB_ID_EM_ID_Pos)          /* 0x00004000 */
#define CANFD_RB_ID_EM_ID_14  	  (0x4000UL << CANFD_RB_ID_EM_ID_Pos)          /* 0x00008000 */
#define CANFD_RB_ID_EM_ID_15  	  (0x8000UL << CANFD_RB_ID_EM_ID_Pos)          /* 0x00010000 */
#define CANFD_RB_ID_EM_ID_16  	  (0x10000UL << CANFD_RB_ID_EM_ID_Pos)         /* 0x00020000 */
#define CANFD_RB_ID_EM_ID_17  	  (0x20000UL << CANFD_RB_ID_EM_ID_Pos)         /* 0x00040000 */
#define CANFD_RB_ID_IDE_Pos       (19U)
#define CANFD_RB_ID_IDE_Msk       (0x1UL << CANFD_RB_ID_IDE_Pos)               /* 0x00080000 */
#define CANFD_RB_ID_IDE           CANFD_RB_ID_IDE_Msk                          /* Identifier Extension. */
#define CANFD_RB_ID_SRR_Pos       (20U)
#define CANFD_RB_ID_SRR_Msk       (0x1UL << CANFD_RB_ID_SRR_Pos)               /* 0x00100000 */
#define CANFD_RB_ID_SRR           CANFD_RB_ID_SRR_Msk                          /* Substitute Remote Transmission Request. */
#define CANFD_RB_ID_ID_Pos 	      (21U)
#define CANFD_RB_ID_ID_Msk 	      (0x7FFUL << CANFD_RB_ID_ID_Pos)              /* 0xFFE00000 */
#define CANFD_RB_ID_ID     	      CANFD_RB_ID_ID_Msk                           /* ID[28:18] bits (Standard Message ID.) */
#define CANFD_RB_ID_ID_0   	      (0x1UL << CANFD_RB_ID_ID_Pos)                /* 0x00200000 */
#define CANFD_RB_ID_ID_1   	      (0x2UL << CANFD_RB_ID_ID_Pos)                /* 0x00400000 */
#define CANFD_RB_ID_ID_2   	      (0x4UL << CANFD_RB_ID_ID_Pos)                /* 0x00800000 */
#define CANFD_RB_ID_ID_3   	      (0x8UL << CANFD_RB_ID_ID_Pos)                /* 0x01000000 */
#define CANFD_RB_ID_ID_4   	      (0x10UL << CANFD_RB_ID_ID_Pos)               /* 0x02000000 */
#define CANFD_RB_ID_ID_5   	      (0x20UL << CANFD_RB_ID_ID_Pos)               /* 0x04000000 */
#define CANFD_RB_ID_ID_6   	      (0x40UL << CANFD_RB_ID_ID_Pos)               /* 0x08000000 */
#define CANFD_RB_ID_ID_7   	      (0x80UL << CANFD_RB_ID_ID_Pos)               /* 0x10000000 */
#define CANFD_RB_ID_ID_8   	      (0x100UL << CANFD_RB_ID_ID_Pos)              /* 0x20000000 */
#define CANFD_RB_ID_ID_9   	      (0x200UL << CANFD_RB_ID_ID_Pos)              /* 0x40000000 */
#define CANFD_RB_ID_ID_10  	      (0x400UL << CANFD_RB_ID_ID_Pos)              /* 0x80000000 */

/*****************  Bit definition for CANFD_RB_DLC register  *****************/
#define CANFD_RB_DLC_TS_DPos      (0U)
#define CANFD_RB_DLC_TS_Pos       CANFD_RB_DLC_TS_DPos
#define CANFD_RB_DLC_TS_Msk       (0xFFFFUL << CANFD_RB_DLC_TS_Pos)  	       /* 0x0000FFFF */
#define CANFD_RB_DLC_TS           CANFD_RB_DLC_TS_Msk                	       /* Timestamp[15:0] bits (Timestamp captured after SOF bit.) */
#define CANFD_RB_DLC_TS_0         (0x1UL << CANFD_RB_DLC_TS_Pos)     	       /* 0x00000001 */
#define CANFD_RB_DLC_TS_1         (0x2UL << CANFD_RB_DLC_TS_Pos)     	       /* 0x00000002 */
#define CANFD_RB_DLC_TS_2         (0x4UL << CANFD_RB_DLC_TS_Pos)     	       /* 0x00000004 */
#define CANFD_RB_DLC_TS_3         (0x8UL << CANFD_RB_DLC_TS_Pos)     	       /* 0x00000008 */
#define CANFD_RB_DLC_TS_4         (0x10UL << CANFD_RB_DLC_TS_Pos)    	       /* 0x00000010 */
#define CANFD_RB_DLC_TS_5         (0x20UL << CANFD_RB_DLC_TS_Pos)    	       /* 0x00000020 */
#define CANFD_RB_DLC_TS_6         (0x40UL << CANFD_RB_DLC_TS_Pos)    	       /* 0x00000040 */
#define CANFD_RB_DLC_TS_7         (0x80UL << CANFD_RB_DLC_TS_Pos)    	       /* 0x00000080 */
#define CANFD_RB_DLC_TS_8         (0x100UL << CANFD_RB_DLC_TS_Pos)   	       /* 0x00000100 */
#define CANFD_RB_DLC_TS_9         (0x200UL << CANFD_RB_DLC_TS_Pos)   	       /* 0x00000200 */
#define CANFD_RB_DLC_TS_10        (0x400UL << CANFD_RB_DLC_TS_Pos)   	       /* 0x00000400 */
#define CANFD_RB_DLC_TS_11        (0x800UL << CANFD_RB_DLC_TS_Pos)   	       /* 0x00000800 */
#define CANFD_RB_DLC_TS_12        (0x1000UL << CANFD_RB_DLC_TS_Pos)  	       /* 0x00001000 */
#define CANFD_RB_DLC_TS_13        (0x2000UL << CANFD_RB_DLC_TS_Pos)  	       /* 0x00002000 */
#define CANFD_RB_DLC_TS_14        (0x4000UL << CANFD_RB_DLC_TS_Pos)  	       /* 0x00004000 */
#define CANFD_RB_DLC_TS_15        (0x8000UL << CANFD_RB_DLC_TS_Pos)  	       /* 0x00008000 */

#define CANFD_RB_DLC_MFI_DPos     (16U)
#define CANFD_RB_DLC_MFI_Pos      CANFD_RB_DLC_MFI_DPos
#define CANFD_RB_DLC_MFI_Msk 	  (0x1FUL << CANFD_RB_DLC_MFI_Pos)              /* 0x001F0000 */
#define CANFD_RB_DLC_MFI     	  CANFD_RB_DLC_MFI_Msk                          /* Matched_Filter_Index[4:0] bits (provide the matched filter index for received message.) */
#define CANFD_RB_DLC_MFI_0   	  (0x1UL << CANFD_RB_DLC_MFI_Pos)               /* 0x00010000 */
#define CANFD_RB_DLC_MFI_1   	  (0x2UL << CANFD_RB_DLC_MFI_Pos)               /* 0x00020000 */
#define CANFD_RB_DLC_MFI_2   	  (0x4UL << CANFD_RB_DLC_MFI_Pos)               /* 0x00040000 */
#define CANFD_RB_DLC_MFI_3   	  (0x8UL << CANFD_RB_DLC_MFI_Pos)               /* 0x00080000 */
#define CANFD_RB_DLC_MFI_4   	  (0x10UL << CANFD_RB_DLC_MFI_Pos)              /* 0x00100000 */

#define CANFD_RB_DLC_ESI_DPos     (25U)
#define CANFD_RB_DLC_ESI_Pos      CANFD_RB_DLC_ESI_DPos
#define CANFD_RB_DLC_ESI_Msk      (0x1UL << CANFD_RB_DLC_ESI_Pos)              /* 0x02000000 */
#define CANFD_RB_DLC_ESI          CANFD_RB_DLC_ESI_Msk                         /* Error State Indicator. */

#define CANFD_RB_DLC_BRS_DPos      (26U)
#define CANFD_RB_DLC_BRS_Pos       CANFD_RB_DLC_BRS_DPos
#define CANFD_RB_DLC_BRS_Msk      (0x1UL << CANFD_RB_DLC_BRS_Pos)              /* 0x04000000 */
#define CANFD_RB_DLC_BRS          CANFD_RB_DLC_BRS_Msk                         /* Bit Rate Switch */

#define CANFD_RB_DLC_EDL_DPos      (27U)
#define CANFD_RB_DLC_EDL_Pos       CANFD_RB_DLC_EDL_DPos
#define CANFD_RB_DLC_EDL_Msk      (0x1UL << CANFD_RB_DLC_EDL_Pos)              /* 0x08000000 */
#define CANFD_RB_DLC_EDL          CANFD_RB_DLC_EDL_Msk                         /* Extended Data Length/FD Frame Format. */


#define CANFD_RB_DLC_DLC_DPos     (28U)
#define CANFD_RB_DLC_DLC_Pos      CANFD_RB_DLC_DLC_DPos
#define CANFD_RB_DLC_DLC_Msk 	  (0xFUL << CANFD_RB_DLC_DLC_Pos)              /* 0xF0000000 */
#define CANFD_RB_DLC_DLC     	  CANFD_RB_DLC_DLC_Msk                         /* DLC[3:0] bits (Data Length Code.) */
#define CANFD_RB_DLC_DLC_0   	  (0x1UL << CANFD_RB_DLC_DLC_Pos)              /* 0x10000000 */
#define CANFD_RB_DLC_DLC_1   	  (0x2UL << CANFD_RB_DLC_DLC_Pos)              /* 0x20000000 */
#define CANFD_RB_DLC_DLC_2   	  (0x4UL << CANFD_RB_DLC_DLC_Pos)              /* 0x40000000 */
#define CANFD_RB_DLC_DLC_3   	  (0x8UL << CANFD_RB_DLC_DLC_Pos)              /* 0x80000000 */

/*****************  Bit definition for CANFD_RB_DW register  ******************/
#define CANFD_RB_DW_DB_DPos       (0U)
#define CANFD_RB_DW_DB_Pos        CANFD_RB_DW_DB_DPos
#define CANFD_RB_DW_DB_Msk 	      (0xFFUL << CANFD_RB_DW_DB_Pos)               /* 0x000000FF */
#define CANFD_RB_DW_DB     	      CANFD_RB_DW_DB_Msk                           /* Data bytes[7:0] bits (Data Byte.) */
#define CANFD_RB_DW_DB_0   	      (0x1UL << CANFD_RB_DW_DB_Pos)                /* 0x00000001 */
#define CANFD_RB_DW_DB_1   	      (0x2UL << CANFD_RB_DW_DB_Pos)                /* 0x00000002 */
#define CANFD_RB_DW_DB_2   	      (0x4UL << CANFD_RB_DW_DB_Pos)                /* 0x00000004 */
#define CANFD_RB_DW_DB_3   	      (0x8UL << CANFD_RB_DW_DB_Pos)                /* 0x00000008 */
#define CANFD_RB_DW_DB_4   	      (0x10UL << CANFD_RB_DW_DB_Pos)               /* 0x00000010 */
#define CANFD_RB_DW_DB_5   	      (0x20UL << CANFD_RB_DW_DB_Pos)               /* 0x00000020 */
#define CANFD_RB_DW_DB_6   	      (0x40UL << CANFD_RB_DW_DB_Pos)               /* 0x00000040 */
#define CANFD_RB_DW_DB_7   	      (0x80UL << CANFD_RB_DW_DB_Pos)               /* 0x00000080 */

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/