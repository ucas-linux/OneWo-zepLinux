/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_timer_reg.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:			2024/08/08
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/08/08     1.0     build this module
 * Description:     This file contains all the register correlation defines for the TIMER firmware library.
 */

#ifndef AS32X601_TIMER_REGS_H
#define AS32X601_TIMER_REGS_H

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
	__IO uint32_t CR1;  	/* TIMx control register 1,						Address offset: 0x00 */
	__IO uint32_t CR2;  	/* TIMx control register 2,						Address offset: 0x04 */
	__IO uint32_t SMCR; 	/* TIMx slave mode control register,			Address offset: 0x08 */
	__IO uint32_t DIER; 	/* TIMx DMA/interrupt enable register,			Address offset: 0x0C */
	__IO uint32_t SR;   	/* TIMx status register,						Address offset: 0x10 */
	__IO uint32_t EGR;  	/* TIMx event generation register,				Address offset: 0x14 */
	__IO uint32_t CCMR1;	/* TIMx capture/compare mode register 1,		Address offset: 0x18 */
	__IO uint32_t CCMR2;	/* TIMx capture/compare mode register 2,		Address offset: 0x1C */
	__IO uint32_t CCMR3;	/* TIMx capture/compare mode register 3,		Address offset: 0x20 */
	__IO uint32_t CCER; 	/* TIMx capture/compare enable register,		Address offset: 0x24 */
	__IO uint32_t CNT;  	/* TIMx counter,								Address offset: 0x28 */
	__IO uint32_t PSC;  	/* TIMx prescaler,								Address offset: 0x2C */
	__IO uint32_t ARR;  	/* TIMx auto-reload register,					Address offset: 0x30 */
	__IO uint32_t RCR;  	/* TIMx repetition counter register,			Address offset: 0x34 */
	__IO uint32_t CCR1; 	/* TIMx capture/compare register 1,				Address offset: 0x38 */
	__IO uint32_t CCR2; 	/* TIMx capture/compare register 2,				Address offset: 0x3C */
	__IO uint32_t CCR3; 	/* TIMx capture/compare register 3,				Address offset: 0x40 */
	__IO uint32_t CCR4; 	/* TIMx capture/compare register 4,				Address offset: 0x44 */
	__IO uint32_t CCR5; 	/* TIMx capture/compare register 5,				Address offset: 0x48 */
	__IO uint32_t CCR6; 	/* TIMx capture/compare register 6,				Address offset: 0x4C */
	__IO uint32_t BDTR; 	/* TIMx break and dead-time register,			Address offset: 0x50 */
	__IO uint32_t DCR;  	/* TIMx DMA control register,					Address offset: 0x54 */
	__IO uint32_t DMAR; 	/* TIMx DMA address for full transfer,			Address offset: 0x58 */
	__IO uint32_t AF1;  	/* TIMx alternate function option register 1,	Address offset: 0x5C */
} TIM_TypeDef;

/*
 * @addtogroup TIM Peripheral declaration
 */
#define TIM0	((TIM_TypeDef*)TIM0_BASE)
#define TIM1	((TIM_TypeDef*)TIM1_BASE)
#define TIM2	((TIM_TypeDef*)TIM2_BASE)
#define TIM3	((TIM_TypeDef*)TIM3_BASE)
#define TIM4	((TIM_TypeDef*)TIM4_BASE)
#define TIM5	((TIM_TypeDef*)TIM5_BASE)
#define TIM6	((TIM_TypeDef*)TIM6_BASE)
#define TIM7	((TIM_TypeDef*)TIM7_BASE)

/*******************  Bit definition for TIM_CR1 register  ********************/
#define TIM_CR1_CEN_DPos           (0U)
#define TIM_CR1_CEN_Pos           TIM_CR1_CEN_DPos
#define TIM_CR1_CEN_Msk           (0x1UL << TIM_CR1_CEN_Pos)                   /* 0x00000001 */
#define TIM_CR1_CEN               TIM_CR1_CEN_Msk                              /* Counter enable */

#define TIM_CR1_UDIS_DPos         (1U)
#define TIM_CR1_UDIS_Pos          TIM_CR1_UDIS_DPos
#define TIM_CR1_UDIS_Msk          (0x1UL << TIM_CR1_UDIS_Pos)                  /* 0x00000002 */
#define TIM_CR1_UDIS              TIM_CR1_UDIS_Msk                             /* Update disable */

#define TIM_CR1_URS_DPos          (2U)
#define TIM_CR1_URS_Pos           TIM_CR1_URS_DPos
#define TIM_CR1_URS_Msk           (0x1UL << TIM_CR1_URS_Pos)                   /* 0x00000004 */
#define TIM_CR1_URS               TIM_CR1_URS_Msk                              /* Update request source */

#define TIM_CR1_OPM_DPos          (3U)
#define TIM_CR1_OPM_Pos           TIM_CR1_OPM_DPos
#define TIM_CR1_OPM_Msk           (0x1UL << TIM_CR1_OPM_Pos)                   /* 0x00000008 */
#define TIM_CR1_OPM               TIM_CR1_OPM_Msk                              /* One pulse mode */

#define TIM_CR1_DIR_DPos          (4U)
#define TIM_CR1_DIR_Pos           TIM_CR1_DIR_DPos
#define TIM_CR1_DIR_Msk           (0x1UL << TIM_CR1_DIR_Pos)                   /* 0x00000010 */
#define TIM_CR1_DIR               TIM_CR1_DIR_Msk                              /* Direction */

#define TIM_CR1_CMS_DPos          (5U)
#define TIM_CR1_CMS_Pos           TIM_CR1_CMS_DPos
#define TIM_CR1_CMS_Msk           (0x3UL << TIM_CR1_CMS_Pos)                   /* 0x00000060 */
#define TIM_CR1_CMS               TIM_CR1_CMS_Msk                              /*CMS[1:0] bits (Center-aligned mode selection) */
#define TIM_CR1_CMS_0             (0x1UL << TIM_CR1_CMS_Pos)                   /* 0x00000020 */
#define TIM_CR1_CMS_1             (0x2UL << TIM_CR1_CMS_Pos)                   /* 0x00000040 */

#define TIM_CR1_ARPE_DPos         (7U)
#define TIM_CR1_ARPE_Pos          TIM_CR1_ARPE_DPos
#define TIM_CR1_ARPE_Msk          (0x1UL << TIM_CR1_ARPE_Pos)                  /* 0x00000080 */
#define TIM_CR1_ARPE              TIM_CR1_ARPE_Msk                             /* Auto-reload preload enable */

#define TIM_CR1_CKD_DPos          (8U)
#define TIM_CR1_CKD_Pos           TIM_CR1_CKD_DPos
#define TIM_CR1_CKD_Msk           (0x3UL << TIM_CR1_CKD_Pos)                   /* 0x00000300 */
#define TIM_CR1_CKD               TIM_CR1_CKD_Msk                              /*CKD[1:0] bits (Clock division) */
#define TIM_CR1_CKD_0             (0x1UL << TIM_CR1_CKD_Pos)                   /* 0x00000100 */
#define TIM_CR1_CKD_1             (0x2UL << TIM_CR1_CKD_Pos)                   /* 0x00000200 */

/*******************  Bit definition for TIM_CR2 register  ********************/
#define TIM_CR2_CCPC_DPos         (0U)
#define TIM_CR2_CCPC_Pos          TIM_CR2_CCPC_DPos
#define TIM_CR2_CCPC_Msk          (0x1UL << TIM_CR2_CCPC_Pos)                  /* 0x00000001 */
#define TIM_CR2_CCPC              TIM_CR2_CCPC_Msk                             /* Capture/compare preloaded control */

#define TIM_CR2_CCUS_DPos         (2U)
#define TIM_CR2_CCUS_Pos         TIM_CR2_CCUS_DPos
#define TIM_CR2_CCUS_Msk          (0x1UL << TIM_CR2_CCUS_Pos)                  /* 0x00000004 */
#define TIM_CR2_CCUS              TIM_CR2_CCUS_Msk                             /* Capture/compare control update selection */

#define TIM_CR2_CCDS_DPos         (3U)
#define TIM_CR2_CCDS_Pos          TIM_CR2_CCDS_DPos
#define TIM_CR2_CCDS_Msk          (0x1UL << TIM_CR2_CCDS_Pos)                  /* 0x00000008 */
#define TIM_CR2_CCDS              TIM_CR2_CCDS_Msk                             /* Capture/compare DMA selection */

#define TIM_CR2_MMS_DPos          (4U)
#define TIM_CR2_MMS_Pos           TIM_CR2_MMS_DPos
#define TIM_CR2_MMS_Msk           (0x7UL << TIM_CR2_MMS_Pos)                   /* 0x00000070 */
#define TIM_CR2_MMS               TIM_CR2_MMS_Msk                              /*MMS[2:0] bits (Master mode selection) */
#define TIM_CR2_MMS_0             (0x1UL << TIM_CR2_MMS_Pos)                   /* 0x00000010 */
#define TIM_CR2_MMS_1             (0x2UL << TIM_CR2_MMS_Pos)                   /* 0x00000020 */
#define TIM_CR2_MMS_2             (0x4UL << TIM_CR2_MMS_Pos)                   /* 0x00000040 */

#define TIM_CR2_TI1S_DPos         (7U)
#define TIM_CR2_TI1S_Pos          TIM_CR2_TI1S_DPos
#define TIM_CR2_TI1S_Pos          TIM_CR2_TI1S_DPos
#define TIM_CR2_TI1S_Msk          (0x1UL << TIM_CR2_TI1S_Pos)                  /* 0x00000080 */
#define TIM_CR2_TI1S              TIM_CR2_TI1S_Msk                             /* TI1 selection */

#define TIM_CR2_OIS1_DPos         (8U)
#define TIM_CR2_OIS1_Pos          TIM_CR2_OIS1_DPos
#define TIM_CR2_OIS1_Msk          (0x1UL << TIM_CR2_OIS1_Pos)                  /* 0x00000100 */
#define TIM_CR2_OIS1              TIM_CR2_OIS1_Msk                             /* Output Idle state 1 (OC1 output) */

#define TIM_CR2_OIS1N_DPos        (9U)
#define TIM_CR2_OIS1N_Pos         TIM_CR2_OIS1N_DPos
#define TIM_CR2_OIS1N_Msk         (0x1UL << TIM_CR2_OIS1N_Pos)                 /* 0x00000200 */
#define TIM_CR2_OIS1N             TIM_CR2_OIS1N_Msk                            /* Output Idle state 1 (OC1N output) */

#define TIM_CR2_OIS2_DPos         (10U)
#define TIM_CR2_OIS2_Pos          TIM_CR2_OIS2_DPos
#define TIM_CR2_OIS2_Msk          (0x1UL << TIM_CR2_OIS2_Pos)                  /* 0x00000400 */
#define TIM_CR2_OIS2              TIM_CR2_OIS2_Msk                             /* Output Idle state 2 (OC2 output) */

#define TIM_CR2_OIS2N_DPos        (11U)
#define TIM_CR2_OIS2N_Pos         TIM_CR2_OIS2N_DPos
#define TIM_CR2_OIS2N_Msk         (0x1UL << TIM_CR2_OIS2N_Pos)                 /* 0x00000800 */
#define TIM_CR2_OIS2N             TIM_CR2_OIS2N_Msk                            /* Output Idle state 2 (OC2N output) */

#define TIM_CR2_OIS3_DPos         (12U)
#define TIM_CR2_OIS3_Pos          TIM_CR2_OIS3_DPos
#define TIM_CR2_OIS3_Msk          (0x1UL << TIM_CR2_OIS3_Pos)                  /* 0x00001000 */
#define TIM_CR2_OIS3              TIM_CR2_OIS3_Msk                             /* Output Idle state 3 (OC3 output) */

#define TIM_CR2_OIS3N_DPos        (13U)
#define TIM_CR2_OIS3N_Pos         TIM_CR2_OIS3N_DPos
#define TIM_CR2_OIS3N_Msk         (0x1UL << TIM_CR2_OIS3N_Pos)                 /* 0x00002000 */
#define TIM_CR2_OIS3N             TIM_CR2_OIS3N_Msk                            /* Output Idle state 3 (OC3N output) */

#define TIM_CR2_OIS4_DPos         (14U)
#define TIM_CR2_OIS4_Pos          TIM_CR2_OIS4_DPos
#define TIM_CR2_OIS4_Msk          (0x1UL << TIM_CR2_OIS4_Pos)                  /* 0x00004000 */
#define TIM_CR2_OIS4              TIM_CR2_OIS4_Msk                             /* Output Idle state 4 (OC4 output) */

#define TIM_CR2_OIS4N_DPos        (15U)
#define TIM_CR2_OIS4N_Pos         TIM_CR2_OIS4N_DPos
#define TIM_CR2_OIS4N_Msk         (0x1UL << TIM_CR2_OIS4N_Pos)                 /* 0x00008000 */
#define TIM_CR2_OIS4N             TIM_CR2_OIS4N_Msk                            /* Output Idle state 4 (OC4N output) */

#define TIM_CR2_OIS5_DPos         (16U)
#define TIM_CR2_OIS5_Pos          TIM_CR2_OIS5_DPos
#define TIM_CR2_OIS5_Msk          (0x1UL << TIM_CR2_OIS5_Pos)                  /* 0x00010000 */
#define TIM_CR2_OIS5              TIM_CR2_OIS5_Msk                             /* Output Idle state 5 (OC5 output) */

#define TIM_CR2_OIS5N_DPos        (17U)
#define TIM_CR2_OIS5N_Pos         TIM_CR2_OIS5N_DPos
#define TIM_CR2_OIS5N_Msk         (0x1UL << TIM_CR2_OIS5N_Pos)                 /* 0x00020000 */
#define TIM_CR2_OIS5N             TIM_CR2_OIS5N_Msk                            /* Output Idle state 5 (OC5N output) */

#define TIM_CR2_OIS6_DPos         (18U)
#define TIM_CR2_OIS6_Pos          TIM_CR2_OIS6_DPos
#define TIM_CR2_OIS6_Msk          (0x1UL << TIM_CR2_OIS6_Pos)                  /* 0x00040000 */
#define TIM_CR2_OIS6              TIM_CR2_OIS6_Msk                             /* Output Idle state 6 (OC6 output) */

#define TIM_CR2_OIS6N_DPos        (19U)
#define TIM_CR2_OIS6N_Pos         TIM_CR2_OIS6N_DPos
#define TIM_CR2_OIS6N_Msk         (0x1UL << TIM_CR2_OIS6N_Pos)                 /* 0x00080000 */
#define TIM_CR2_OIS6N             TIM_CR2_OIS6N_Msk                            /* Output Idle state 6 (OC6N output) */

#define TIM_CR2_MMS2_DPos         (20U)
#define TIM_CR2_MMS2_Pos          TIM_CR2_MMS2_DPos
#define TIM_CR2_MMS2_Msk          (0xFUL << TIM_CR2_MMS2_Pos)                  /* 0x00F00000 */
#define TIM_CR2_MMS2              TIM_CR2_MMS2_Msk                             /*MMS[2:0] bits (Master mode selection 2) */
#define TIM_CR2_MMS2_0            (0x1UL << TIM_CR2_MMS2_Pos)                  /* 0x00100000 */
#define TIM_CR2_MMS2_1            (0x2UL << TIM_CR2_MMS2_Pos)                  /* 0x00200000 */
#define TIM_CR2_MMS2_2            (0x4UL << TIM_CR2_MMS2_Pos)                  /* 0x00400000 */
#define TIM_CR2_MMS2_3            (0x8UL << TIM_CR2_MMS2_Pos)                  /* 0x00800000 */


/*******************  Bit definition for TIM_SMCR register  *******************/
#define TIM_SMCR_SMS_DPos         (0U)
#define TIM_SMCR_SMS_Pos          TIM_SMCR_SMS_DPos
#define TIM_SMCR_SMS_Msk          (0xFUL << TIM_SMCR_SMS_Pos)                  /* 0x0000000F */
#define TIM_SMCR_SMS              TIM_SMCR_SMS_Msk                             /* SMS[3:0] bits (Slave mode selection) */
#define TIM_SMCR_SMS_0            (0x1UL << TIM_SMCR_SMS_Pos)                  /* 0x00000001 */
#define TIM_SMCR_SMS_1            (0x2UL << TIM_SMCR_SMS_Pos)                  /* 0x00000002 */
#define TIM_SMCR_SMS_2            (0x4UL << TIM_SMCR_SMS_Pos)                  /* 0x00000004 */
#define TIM_SMCR_SMS_3            (0x8UL << TIM_SMCR_SMS_Pos)                  /* 0x00000008 */

#define TIM_SMCR_TS_DPos          (4U)
#define TIM_SMCR_TS_Pos           TIM_SMCR_TS_DPos
#define TIM_SMCR_TS_Msk           (0x7UL << TIM_SMCR_TS_Pos)                   /* 0x00000070 */
#define TIM_SMCR_TS               TIM_SMCR_TS_Msk                              /* TS[2:0] bits (Trigger selection) */
#define TIM_SMCR_TS_0             (0x1UL << TIM_SMCR_TS_Pos)                   /* 0x00000010 */
#define TIM_SMCR_TS_1             (0x2UL << TIM_SMCR_TS_Pos)                   /* 0x00000020 */
#define TIM_SMCR_TS_2             (0x4UL << TIM_SMCR_TS_Pos)                   /* 0x00000040 */

#define TIM_SMCR_ETF_DPos         (8U)
#define TIM_SMCR_ETF_Pos          TIM_SMCR_ETF_DPos
#define TIM_SMCR_ETF_Msk          (0xFUL << TIM_SMCR_ETF_Pos)                  /* 0x00000F00 */
#define TIM_SMCR_ETF              TIM_SMCR_ETF_Msk                             /* ETF[3:0] bits (External trigger filter) */
#define TIM_SMCR_ETF_0            (0x1UL << TIM_SMCR_ETF_Pos)                  /* 0x00000100 */
#define TIM_SMCR_ETF_1            (0x2UL << TIM_SMCR_ETF_Pos)                  /* 0x00000200 */
#define TIM_SMCR_ETF_2            (0x4UL << TIM_SMCR_ETF_Pos)                  /* 0x00000400 */
#define TIM_SMCR_ETF_3            (0x8UL << TIM_SMCR_ETF_Pos)                  /* 0x00000800 */

#define TIM_SMCR_ETPS_DPos        (12U)
#define TIM_SMCR_ETPS_Pos         TIM_SMCR_ETPS_DPos
#define TIM_SMCR_ETPS_Msk         (0x3UL << TIM_SMCR_ETPS_Pos)                 /* 0x00003000 */
#define TIM_SMCR_ETPS             TIM_SMCR_ETPS_Msk                            /* ETPS[1:0] bits (External trigger prescaler) */
#define TIM_SMCR_ETPS_0           (0x1UL << TIM_SMCR_ETPS_Pos)  
#define TIM_SMCR_ETPS_1           (0x2UL << TIM_SMCR_ETPS_Pos)                 /* 0x00002000 */

#define TIM_SMCR_ECE_DPos         (14U)
#define TIM_SMCR_ECE_Pos          TIM_SMCR_ECE_DPos
#define TIM_SMCR_ECE_Msk          (0x1UL << TIM_SMCR_ECE_Pos)                  /* 0x00004000 */
#define TIM_SMCR_ECE              TIM_SMCR_ECE_Msk                             /* External clock enable */

#define TIM_SMCR_ETP_DPos         (15U)
#define TIM_SMCR_ETP_Pos          TIM_SMCR_ETP_DPos
#define TIM_SMCR_ETP_Msk          (0x1UL << TIM_SMCR_ETP_Pos)                  /* 0x00008000 */
#define TIM_SMCR_ETP              TIM_SMCR_ETP_Msk                             /* External trigger polarity */


/*******************  Bit definition for TIM_DIER register  *******************/
#define TIM_DIER_UIE_DPos         (0U)
#define TIM_DIER_UIE_Pos          TIM_DIER_UIE_DPos
#define TIM_DIER_UIE_Msk          (0x1UL << TIM_DIER_UIE_Pos)                  /* 0x00000001 */
#define TIM_DIER_UIE              TIM_DIER_UIE_Msk                             /* Update interrupt enable */

#define TIM_DIER_TIE_DPos         (2U)
#define TIM_DIER_TIE_Pos          TIM_DIER_TIE_DPos
#define TIM_DIER_TIE_Msk          (0x1UL << TIM_DIER_TIE_Pos)                  /* 0x00000004 */
#define TIM_DIER_TIE              TIM_DIER_TIE_Msk                             /* Trigger interrupt enable */

#define TIM_DIER_BIE_DPos         (3U)
#define TIM_DIER_BIE_Pos          TIM_DIER_BIE_DPos
#define TIM_DIER_BIE_Msk          (0x1UL << TIM_DIER_BIE_Pos)                  /* 0x00000008 */
#define TIM_DIER_BIE              TIM_DIER_BIE_Msk                             /* Break interrupt enable */

#define TIM_DIER_UDE_DPos         (8U)
#define TIM_DIER_UDE_Pos          TIM_DIER_UDE_DPos
#define TIM_DIER_UDE_Msk          (0x1UL << TIM_DIER_UDE_Pos)                  /* 0x00000100 */
#define TIM_DIER_UDE              TIM_DIER_UDE_Msk                             /* Update DMA request enable */

#define TIM_DIER_CC1IE_DPos       (16U)
#define TIM_DIER_CC1IE_Pos        TIM_DIER_CC1IE_DPos
#define TIM_DIER_CC1IE_Msk        (0x1UL << TIM_DIER_CC1IE_Pos)                /* 0x00010000 */
#define TIM_DIER_CC1IE            TIM_DIER_CC1IE_Msk                           /* Capture/Compare 1 interrupt enable */

#define TIM_DIER_CC2IE_DPos       (17U)
#define TIM_DIER_CC2IE_Pos        TIM_DIER_CC2IE_DPos
#define TIM_DIER_CC2IE_Msk        (0x1UL << TIM_DIER_CC2IE_Pos)                /* 0x00020000 */
#define TIM_DIER_CC2IE            TIM_DIER_CC2IE_Msk                           /* Capture/Compare 2 interrupt enable */

#define TIM_DIER_CC3IE_DPos       (18U)
#define TIM_DIER_CC3IE_Pos        TIM_DIER_CC3IE_DPos
#define TIM_DIER_CC3IE_Msk        (0x1UL << TIM_DIER_CC3IE_Pos)                /* 0x00040000 */
#define TIM_DIER_CC3IE            TIM_DIER_CC3IE_Msk   

#define TIM_DIER_CC4IE_DPos       (19U)
#define TIM_DIER_CC4IE_Pos        TIM_DIER_CC4IE_DPos
#define TIM_DIER_CC4IE_Msk        (0x1UL << TIM_DIER_CC4IE_Pos)                /* 0x00080000 */
#define TIM_DIER_CC4IE            TIM_DIER_CC4IE_Msk                           /* Capture/Compare 4 interrupt enable */

#define TIM_DIER_CC5IE_DPos       (20U)
#define TIM_DIER_CC5IE_Pos        TIM_DIER_CC5IE_DPos
#define TIM_DIER_CC5IE_Msk        (0x1UL << TIM_DIER_CC5IE_Pos)                /* 0x00100000 */
#define TIM_DIER_CC5IE            TIM_DIER_CC5IE_Msk                           /* Capture/Compare 5 interrupt enable */

#define TIM_DIER_CC6IE_DPos       (21U)
#define TIM_DIER_CC6IE_Pos        TIM_DIER_CC6IE_DPos
#define TIM_DIER_CC6IE_Msk        (0x1UL << TIM_DIER_CC6IE_Pos)                 /* 0x00200000 */
#define TIM_DIER_CC6IE            TIM_DIER_CC6IE_Msk                           /* Capture/Compare 6 interrupt enable */

#define TIM_DIER_CC1DE_DPos       (24U)
#define TIM_DIER_CC1DE_Pos        TIM_DIER_CC1DE_DPos
#define TIM_DIER_CC1DE_Msk        (0x1UL << TIM_DIER_CC1DE_Pos)                /* 0x01000000 */
#define TIM_DIER_CC1DE            TIM_DIER_CC1DE_Msk                           /* Capture/Compare 1 DMA request enable */

#define TIM_DIER_CC2DE_DPos       (25U)
#define TIM_DIER_CC2DE_Pos        TIM_DIER_CC2DE_DPos
#define TIM_DIER_CC2DE_Msk        (0x1UL << TIM_DIER_CC2DE_Pos)                /* 0x02000000 */
#define TIM_DIER_CC2DE            TIM_DIER_CC2DE_Msk                           /* Capture/Compare 2 DMA request enable */

#define TIM_DIER_CC3DE_DPos       (26U)
#define TIM_DIER_CC3DE_Pos        TIM_DIER_CC3DE_DPos
#define TIM_DIER_CC3DE_Msk        (0x1UL << TIM_DIER_CC3DE_Pos)                /* 0x04000000 */
#define TIM_DIER_CC3DE            TIM_DIER_CC3DE_Msk                           /* Capture/Compare 3 DMA request enable */

#define TIM_DIER_CC4DE_DPos       (27U)
#define TIM_DIER_CC4DE_Pos        TIM_DIER_CC4DE_DPos
#define TIM_DIER_CC4DE_Msk        (0x1UL << TIM_DIER_CC4DE_Pos)                /* 0x08000000 */
#define TIM_DIER_CC4DE            TIM_DIER_CC4DE_Msk                           /* Capture/Compare 4 DMA request enable */

#define TIM_DIER_CC5DE_DPos       (28U)
#define TIM_DIER_CC5DE_Pos        TIM_DIER_CC5DE_DPos
#define TIM_DIER_CC5DE_Msk        (0x1UL << TIM_DIER_CC5DE_Pos)                /* 0x10000000 */
#define TIM_DIER_CC5DE            TIM_DIER_CC5DE_Msk                           /* Capture/Compare 5 DMA request enable */

#define TIM_DIER_CC6DE_DPos       (29U)
#define TIM_DIER_CC6DE_Pos        TIM_DIER_CC6DE_DPos
#define TIM_DIER_CC6DE_Msk        (0x1UL << TIM_DIER_CC6DE_Pos)                /* 0x20000000 */
#define TIM_DIER_CC6DE            TIM_DIER_CC6DE_Msk                           /* Capture/Compare 6 DMA request enable */


/********************  Bit definition for TIM_SR register  ********************/
#define TIM_SR_UIF_DPos           (0U)
#define TIM_SR_UIF_Pos            TIM_SR_UIF_DPos
#define TIM_SR_UIF_Msk            (0x1UL << TIM_SR_UIF_Pos)                    /* 0x00000001 */
#define TIM_SR_UIF                TIM_SR_UIF_Msk                               /* Update interrupt flag */

#define TIM_SR_TIF_DPos           (2U)
#define TIM_SR_TIF_Pos            TIM_SR_TIF_DPos
#define TIM_SR_TIF_Msk            (0x1UL << TIM_SR_TIF_Pos)                    /* 0x00000004 */
#define TIM_SR_TIF                TIM_SR_TIF_Msk                               /* Trigger interrupt flag */

#define TIM_SR_BIF_DPos           (3U)
#define TIM_SR_BIF_Pos            TIM_SR_BIF_DPos
#define TIM_SR_BIF_Msk            (0x1UL << TIM_SR_BIF_Pos)                    /* 0x00000008 */
#define TIM_SR_BIF                TIM_SR_BIF_Msk                               /* Break interrupt flag */

#define TIM_SR_CC1IF_DPos         (16U)
#define TIM_SR_CC1IF_Pos          TIM_SR_CC1IF_DPos
#define TIM_SR_CC1IF_Msk          (0x1UL << TIM_SR_CC1IF_Pos)                  /* 0x00010000 */
#define TIM_SR_CC1IF              TIM_SR_CC1IF_Msk                             /* Capture/Compare 1 interrupt flag */

#define TIM_SR_CC2IF_DPos         (17U)
#define TIM_SR_CC2IF_Pos          TIM_SR_CC2IF_DPos
#define TIM_SR_CC2IF_Msk          (0x1UL << TIM_SR_CC2IF_Pos)                  /* 0x00020000 */
#define TIM_SR_CC2IF              TIM_SR_CC2IF_Msk                             /* Capture/Compare 2 interrupt flag */

#define TIM_SR_CC3IF_DPos         (18U)
#define TIM_SR_CC3IF_Pos          TIM_SR_CC3IF_DPos
#define TIM_SR_CC3IF_Msk          (0x1UL << TIM_SR_CC3IF_Pos)                  /* 0x00040000 */
#define TIM_SR_CC3IF              TIM_SR_CC3IF_Msk                             /* Capture/Compare 3 interrupt flag */

#define TIM_SR_CC4IF_DPos         (19U)
#define TIM_SR_CC4IF_Pos          TIM_SR_CC4IF_DPos
#define TIM_SR_CC4IF_Msk          (0x1UL << TIM_SR_CC4IF_Pos)                  /* 0x00080000 */
#define TIM_SR_CC4IF              TIM_SR_CC4IF_Msk                             /* Capture/Compare 4 interrupt flag */

#define TIM_SR_CC5IF_DPos         (20U)
#define TIM_SR_CC5IF_Pos          TIM_SR_CC5IF_DPos
#define TIM_SR_CC5IF_Msk          (0x1UL << TIM_SR_CC5IF_Pos)                  /* 0x00100000 */
#define TIM_SR_CC5IF              TIM_SR_CC5IF_Msk                             /* Capture/Compare 5 interrupt flag */

#define TIM_SR_CC6IF_DPos         (21U)
#define TIM_SR_CC6IF_Pos          TIM_SR_CC6IF_DPos
#define TIM_SR_CC6IF_Msk          (0x1UL << TIM_SR_CC6IF_Pos)                  /* 0x00200000 */
#define TIM_SR_CC6IF              TIM_SR_CC6IF_Msk                             /* Capture/Compare 6 interrupt flag */

#define TIM_SR_CC1OF_DPos         (24U)
#define TIM_SR_CC1OF_Pos          TIM_SR_CC1OF_DPos
#define TIM_SR_CC1OF_Msk          (0x1UL << TIM_SR_CC1OF_Pos)                  /* 0x01000000 */
#define TIM_SR_CC1OF              TIM_SR_CC1OF_Msk                             /* Capture/Compare 1 overcapture flag */

#define TIM_SR_CC2OF_DPos         (25U)
#define TIM_SR_CC2OF_Pos          TIM_SR_CC2OF_DPos
#define TIM_SR_CC2OF_Msk          (0x1UL << TIM_SR_CC2OF_Pos)                  /* 0x02000000 */
#define TIM_SR_CC2OF              TIM_SR_CC2OF_Msk                             /* Capture/Compare 2 overcapture flag */

#define TIM_SR_CC3OF_DPos         (26U)
#define TIM_SR_CC3OF_Pos          TIM_SR_CC3OF_DPos
#define TIM_SR_CC3OF_Msk          (0x1UL << TIM_SR_CC3OF_Pos)                  /* 0x04000000 */
#define TIM_SR_CC3OF              TIM_SR_CC3OF_Msk                             /* Capture/Compare 3 overcapture flag */

#define TIM_SR_CC4OF_DPos         (27U)
#define TIM_SR_CC4OF_Pos          TIM_SR_CC4OF_DPos
#define TIM_SR_CC4OF_Msk          (0x1UL << TIM_SR_CC4OF_Pos)                  /* 0x08000000 */
#define TIM_SR_CC4OF              TIM_SR_CC4OF_Msk                             /* Capture/Compare 4 overcapture flag */

#define TIM_SR_CC5OF_DPos         (28U)
#define TIM_SR_CC5OF_Pos          TIM_SR_CC5OF_DPos
#define TIM_SR_CC5OF_Msk          (0x1UL << TIM_SR_CC5OF_Pos)                  /* 0x10000000 */
#define TIM_SR_CC5OF              TIM_SR_CC5OF_Msk                             /* Capture/Compare 5 overcapture flag */

#define TIM_SR_CC6OF_DPos         (29U)
#define TIM_SR_CC6OF_Pos          TIM_SR_CC6OF_DPos
#define TIM_SR_CC6OF_Msk          (0x1UL << TIM_SR_CC6OF_Pos)                  /* 0x20000000 */
#define TIM_SR_CC6OF              TIM_SR_CC6OF_Msk                             /* Capture/Compare 6 overcapture flag */

/*******************  Bit definition for TIM_EGR register  ********************/
#define TIM_EGR_UG_DPos           (0U)
#define TIM_EGR_UG_Pos            TIM_EGR_UG_DPos
#define TIM_EGR_UG_Msk            (0x1UL << TIM_EGR_UG_Pos)                    /* 0x00000001 */
#define TIM_EGR_UG                TIM_EGR_UG_Msk                               /* Update generation */

#define TIM_EGR_COMG_DPos         (1U)
#define TIM_EGR_COMG_Pos          TIM_EGR_COMG_DPos
#define TIM_EGR_COMG_Msk          (0x1UL << TIM_EGR_COMG_Pos)                  /* 0x00000002 */
#define TIM_EGR_COMG              TIM_EGR_COMG_Msk                             /* Capture/Compare control update generation */

#define TIM_EGR_TG_DPos           (2U)
#define TIM_EGR_TG_Pos            TIM_EGR_TG_DPos
#define TIM_EGR_TG_Msk            (0x1UL << TIM_EGR_TG_Pos)                    /* 0x00000004 */
#define TIM_EGR_TG                TIM_EGR_TG_Msk                               /* Trigger generation */

#define TIM_EGR_BG_DPos           (3U)
#define TIM_EGR_BG_Pos            TIM_EGR_BG_DPos
#define TIM_EGR_BG_Msk            (0x1UL << TIM_EGR_BG_Pos)                    /* 0x00000008 */
#define TIM_EGR_BG                TIM_EGR_BG_Msk                               /* Break generation */

#define TIM_EGR_CC1G_DPos         (16U)
#define TIM_EGR_CC1G_Pos          TIM_EGR_CC1G_DPos
#define TIM_EGR_CC1G_Msk          (0x1UL << TIM_EGR_CC1G_Pos)                  /* 0x00010000 */
#define TIM_EGR_CC1G              TIM_EGR_CC1G_Msk                             /* Capture/Compare 1 generation */

#define TIM_EGR_CC2G_DPos         (17U)
#define TIM_EGR_CC2G_Pos          TIM_EGR_CC2G_DPos
#define TIM_EGR_CC2G_Msk          (0x1UL << TIM_EGR_CC2G_Pos)                  /* 0x00020000 */
#define TIM_EGR_CC2G              TIM_EGR_CC2G_Msk                             /* Capture/Compare 2 generation */

#define TIM_EGR_CC3G_DPos         (18U)
#define TIM_EGR_CC3G_Pos          TIM_EGR_CC3G_DPos
#define TIM_EGR_CC3G_Msk          (0x1UL << TIM_EGR_CC3G_Pos)                  /* 0x00040000 */
#define TIM_EGR_CC3G              TIM_EGR_CC3G_Msk                             /* Capture/Compare 3 generation */

#define TIM_EGR_CC4G_DPos         (19U)
#define TIM_EGR_CC4G_Pos          TIM_EGR_CC4G_DPos
#define TIM_EGR_CC4G_Msk          (0x1UL << TIM_EGR_CC4G_Pos)                  /* 0x00080000 */
#define TIM_EGR_CC4G              TIM_EGR_CC4G_Msk                             /* Capture/Compare 4 generation */

#define TIM_EGR_CC5G_DPos         (20U)
#define TIM_EGR_CC5G_Pos          TIM_EGR_CC5G_DPos
#define TIM_EGR_CC5G_Msk          (0x1UL << TIM_EGR_CC5G_Pos)                  /* 0x00100000 */
#define TIM_EGR_CC5G              TIM_EGR_CC5G_Msk                             /* Capture/Compare 5 generation */

#define TIM_EGR_CC6G_DPos         (21U)
#define TIM_EGR_CC6G_Pos          TIM_EGR_CC6G_DPos
#define TIM_EGR_CC6G_Msk          (0x1UL << TIM_EGR_CC6G_Pos)                  /* 0x00200000 */
#define TIM_EGR_CC6G              TIM_EGR_CC6G_Msk                             /* Capture/Compare 6 generation */



/******************  Bit definition for TIM_CCMR1 register  *******************/
#define TIM_CCMR1_CC1S_DPos       (0U)
#define TIM_CCMR1_CC1S_Pos        TIM_CCMR1_CC1S_DPos
#define TIM_CCMR1_CC1S_Msk        (0x3UL << TIM_CCMR1_CC1S_Pos)                /* 0x00000003 */
#define TIM_CCMR1_CC1S            TIM_CCMR1_CC1S_Msk                           /* CC1S[1:0] bits (Capture/Compare 1 selection) */
#define TIM_CCMR1_CC1S_0          (0x1UL << TIM_CCMR1_CC1S_Pos)                /* 0x00000001 */
#define TIM_CCMR1_CC1S_1          (0x2UL << TIM_CCMR1_CC1S_Pos)                /* 0x00000002 */

#define TIM_CCMR1_OC1FE_DPos      (2U)
#define TIM_CCMR1_OC1FE_Pos       TIM_CCMR1_OC1FE_DPos
#define TIM_CCMR1_OC1FE_Msk       (0x1UL << TIM_CCMR1_OC1FE_Pos)               /* 0x00000004 */
#define TIM_CCMR1_OC1FE           TIM_CCMR1_OC1FE_Msk                          /* Output Compare 1 fast enable */

#define TIM_CCMR1_OC1PE_DPos      (3U)
#define TIM_CCMR1_OC1PE_Pos       TIM_CCMR1_OC1PE_DPos
#define TIM_CCMR1_OC1PE_Msk       (0x1UL << TIM_CCMR1_OC1PE_Pos)               /* 0x00000008 */
#define TIM_CCMR1_OC1PE           TIM_CCMR1_OC1PE_Msk                          /* Output Compare 1 preload enable */

#define TIM_CCMR1_OC1M_DPos       (4U)
#define TIM_CCMR1_OC1M_Pos        TIM_CCMR1_OC1M_DPos
#define TIM_CCMR1_OC1M_Msk        (0x1007UL << TIM_CCMR1_OC1M_Pos)             /* 0x00010070 */
#define TIM_CCMR1_OC1M            TIM_CCMR1_OC1M_Msk                           /* OC1M[2:0] bits (Output Compare 1 mode) */
#define TIM_CCMR1_OC1M_0          (0x0001UL << TIM_CCMR1_OC1M_Pos)             /* 0x00000010 */
#define TIM_CCMR1_OC1M_1          (0x0002UL << TIM_CCMR1_OC1M_Pos)             /* 0x00000020 */
#define TIM_CCMR1_OC1M_2          (0x0004UL << TIM_CCMR1_OC1M_Pos)             /* 0x00000040 */
#define TIM_CCMR1_OC1M_3          (0x1000UL << TIM_CCMR1_OC1M_Pos)             /* 0x00010000 */

#define TIM_CCMR1_OC1CE_DPos      (7U)
#define TIM_CCMR1_OC1CE_Pos       TIM_CCMR1_OC1CE_DPos
#define TIM_CCMR1_OC1CE_Msk       (0x1UL << TIM_CCMR1_OC1CE_Pos)               /* 0x00000080 */
#define TIM_CCMR1_OC1CE           TIM_CCMR1_OC1CE_Msk                          /* Output Compare 1 clear enable */

#define TIM_CCMR1_CC2S_DPos       (8U)
#define TIM_CCMR1_CC2S_Pos        TIM_CCMR1_CC2S_DPos
#define TIM_CCMR1_CC2S_Msk        (0x3UL << TIM_CCMR1_CC2S_Pos)                /* 0x00000300 */
#define TIM_CCMR1_CC2S            TIM_CCMR1_CC2S_Msk                           /* CC2S[1:0] bits (Capture/Compare 2 selection) */
#define TIM_CCMR1_CC2S_0          (0x1UL << TIM_CCMR1_CC2S_Pos)                /* 0x00000100 */
#define TIM_CCMR1_CC2S_1          (0x2UL << TIM_CCMR1_CC2S_Pos)                /* 0x00000200 */


#define TIM_CCMR1_OC2FE_DPos      (10U)
#define TIM_CCMR1_OC2FE_Pos       TIM_CCMR1_OC2FE_DPos
#define TIM_CCMR1_OC2FE_Msk       (0x1UL << TIM_CCMR1_OC2FE_Pos)               /* 0x00000400 */
#define TIM_CCMR1_OC2FE           TIM_CCMR1_OC2FE_Msk                          /* Output Compare 2 fast enable */

#define TIM_CCMR1_OC2PE_DPos      (11U)
#define TIM_CCMR1_OC2PE_Pos       TIM_CCMR1_OC2PE_DPos
#define TIM_CCMR1_OC2PE_Msk       (0x1UL << TIM_CCMR1_OC2PE_Pos)               /* 0x00000800 */
#define TIM_CCMR1_OC2PE           TIM_CCMR1_OC2PE_Msk                          /* Output Compare 2 preload enable */

#define TIM_CCMR1_OC2M_DPos       (12U)
#define TIM_CCMR1_OC2M_Pos        TIM_CCMR1_OC2M_DPos
#define TIM_CCMR1_OC2M_Msk        (0x1007UL << TIM_CCMR1_OC2M_Pos)             /* 0x01007000 */
#define TIM_CCMR1_OC2M            TIM_CCMR1_OC2M_Msk                           /* OC2M[2:0] bits (Output Compare 2 mode) */
#define TIM_CCMR1_OC2M_0          (0x0001UL << TIM_CCMR1_OC2M_Pos)             /* 0x00001000 */
#define TIM_CCMR1_OC2M_1          (0x0002UL << TIM_CCMR1_OC2M_Pos)             /* 0x00002000 */
#define TIM_CCMR1_OC2M_2          (0x0004UL << TIM_CCMR1_OC2M_Pos)             /* 0x00004000 */
#define TIM_CCMR1_OC2M_3          (0x1000UL << TIM_CCMR1_OC2M_Pos)             /* 0x01000000 */

#define TIM_CCMR1_OC2CE_DPos      (15U)
#define TIM_CCMR1_OC2CE_Pos       TIM_CCMR1_OC2CE_DPos
#define TIM_CCMR1_OC2CE_Msk       (0x1UL << TIM_CCMR1_OC2CE_Pos)               /* 0x00008000 */
#define TIM_CCMR1_OC2CE           TIM_CCMR1_OC2CE_Msk                          /* Output Compare 2 clear enable */


/*----------------------------------------------------------------------------*/

#define TIM_CCMR1_IC1PSC_DPos     (2U)
#define TIM_CCMR1_IC1PSC_Pos      TIM_CCMR1_IC1PSC_DPos
#define TIM_CCMR1_IC1PSC_Msk      (0x3UL << TIM_CCMR1_IC1PSC_Pos)              /* 0x0000000C */
#define TIM_CCMR1_IC1PSC          TIM_CCMR1_IC1PSC_Msk                         /* IC1PSC[1:0] bits (Input capture 1 prescaler) */
#define TIM_CCMR1_IC1PSC_0        (0x1UL << TIM_CCMR1_IC1PSC_Pos)              /* 0x00000004 */
#define TIM_CCMR1_IC1PSC_1        (0x2UL << TIM_CCMR1_IC1PSC_Pos)              /* 0x00000008 */

#define TIM_CCMR1_IC1F_DPos       (4U)
#define TIM_CCMR1_IC1F_Pos        TIM_CCMR1_IC1F_DPos
#define TIM_CCMR1_IC1F_Msk        (0xFUL << TIM_CCMR1_IC1F_Pos)                /* 0x000000F0 */
#define TIM_CCMR1_IC1F            TIM_CCMR1_IC1F_Msk                           /* IC1F[3:0] bits (Input capture 1 filter) */
#define TIM_CCMR1_IC1F_0          (0x1UL << TIM_CCMR1_IC1F_Pos)                /* 0x00000010 */
#define TIM_CCMR1_IC1F_1          (0x2UL << TIM_CCMR1_IC1F_Pos)                /* 0x00000020 */
#define TIM_CCMR1_IC1F_2          (0x4UL << TIM_CCMR1_IC1F_Pos)                /* 0x00000040 */
#define TIM_CCMR1_IC1F_3          (0x8UL << TIM_CCMR1_IC1F_Pos)                /* 0x00000080 */

#define TIM_CCMR1_IC2PSC_DPos     (10U)
#define TIM_CCMR1_IC2PSC_Pos      TIM_CCMR1_IC2PSC_DPos
#define TIM_CCMR1_IC2PSC_Msk      (0x3UL << TIM_CCMR1_IC2PSC_Pos)              /* 0x00000C00 */
#define TIM_CCMR1_IC2PSC          TIM_CCMR1_IC2PSC_Msk                         /* IC2PSC[1:0] bits (Input capture 2 prescaler) */
#define TIM_CCMR1_IC2PSC_0        (0x1UL << TIM_CCMR1_IC2PSC_Pos)              /* 0x00000400 */
#define TIM_CCMR1_IC2PSC_1        (0x2UL << TIM_CCMR1_IC2PSC_Pos)              /* 0x00000800 */

#define TIM_CCMR1_IC2F_DPos       (12U)
#define TIM_CCMR1_IC2F_Pos        TIM_CCMR1_IC2F_DPos
#define TIM_CCMR1_IC2F_Msk        (0xFUL << TIM_CCMR1_IC2F_Pos)                /* 0x0000F000 */
#define TIM_CCMR1_IC2F            TIM_CCMR1_IC2F_Msk                           /* IC2F[3:0] bits (Input capture 2 filter) */
#define TIM_CCMR1_IC2F_0          (0x1UL << TIM_CCMR1_IC2F_Pos)                /* 0x00001000 */
#define TIM_CCMR1_IC2F_1          (0x2UL << TIM_CCMR1_IC2F_Pos)                /* 0x00002000 */
#define TIM_CCMR1_IC2F_2          (0x4UL << TIM_CCMR1_IC2F_Pos)                /* 0x00004000 */
#define TIM_CCMR1_IC2F_3          (0x8UL << TIM_CCMR1_IC2F_Pos)                /* 0x00008000 */

/******************  Bit definition for TIM_CCMR2 register  *******************/
#define TIM_CCMR2_CC3S_DPos       (0U)
#define TIM_CCMR2_CC3S_Pos        TIM_CCMR2_CC3S_DPos
#define TIM_CCMR2_CC3S_Msk        (0x3UL << TIM_CCMR2_CC3S_Pos)                /* 0x00000003 */
#define TIM_CCMR2_CC3S            TIM_CCMR2_CC3S_Msk                           /* CC3S[1:0] bits (Capture/Compare 3 selection) */
#define TIM_CCMR2_CC3S_0          (0x1UL << TIM_CCMR2_CC3S_Pos)                /* 0x00000001 */
#define TIM_CCMR2_CC3S_1          (0x2UL << TIM_CCMR2_CC3S_Pos)                /* 0x00000002 */

#define TIM_CCMR2_OC3FE_DPos      (2U)
#define TIM_CCMR2_OC3FE_Pos       TIM_CCMR2_OC3FE_DPos
#define TIM_CCMR2_OC3FE_Msk       (0x1UL << TIM_CCMR2_OC3FE_Pos)               /* 0x00000004 */
#define TIM_CCMR2_OC3FE           TIM_CCMR2_OC3FE_Msk                          /* Output compare 3 fast enable */

#define TIM_CCMR2_OC3PE_DPos      (3U)
#define TIM_CCMR2_OC3PE_Pos       TIM_CCMR2_OC3PE_DPos
#define TIM_CCMR2_OC3PE_Msk       (0x1UL << TIM_CCMR2_OC3PE_Pos)               /* 0x00000008 */
#define TIM_CCMR2_OC3PE           TIM_CCMR2_OC3PE_Msk                          /* Output compare 3 preload enable */

#define TIM_CCMR2_OC3M_DPos       (4U)
#define TIM_CCMR2_OC3M_Pos        TIM_CCMR2_OC3M_DPos
#define TIM_CCMR2_OC3M_Msk        (0x1007UL << TIM_CCMR2_OC3M_Pos)             /* 0x00010070 */
#define TIM_CCMR2_OC3M            TIM_CCMR2_OC3M_Msk                           /* OC3M[2:0] bits (Output compare 3 mode) */
#define TIM_CCMR2_OC3M_0          (0x0001UL << TIM_CCMR2_OC3M_Pos)             /* 0x00000010 */
#define TIM_CCMR2_OC3M_1          (0x0002UL << TIM_CCMR2_OC3M_Pos)             /* 0x00000020 */
#define TIM_CCMR2_OC3M_2          (0x0004UL << TIM_CCMR2_OC3M_Pos)             /* 0x00000040 */
#define TIM_CCMR2_OC3M_3          (0x1000UL << TIM_CCMR2_OC3M_Pos)             /* 0x00010000 */

#define TIM_CCMR2_OC3CE_DPos      (7U)
#define TIM_CCMR2_OC3CE_Pos       TIM_CCMR2_OC3CE_DPos
#define TIM_CCMR2_OC3CE_Msk       (0x1UL << TIM_CCMR2_OC3CE_Pos)               /* 0x00000080 */
#define TIM_CCMR2_OC3CE           TIM_CCMR2_OC3CE_Msk   

#define TIM_CCMR2_CC4S_DPos       (8U)
#define TIM_CCMR2_CC4S_Pos        TIM_CCMR2_CC4S_DPos
#define TIM_CCMR2_CC4S_Msk        (0x3UL << TIM_CCMR2_CC4S_Pos)                /* 0x00000300 */
#define TIM_CCMR2_CC4S            TIM_CCMR2_CC4S_Msk                           /* CC4S[1:0] bits (Capture/Compare 4 selection) */
#define TIM_CCMR2_CC4S_0          (0x1UL << TIM_CCMR2_CC4S_Pos)                /* 0x00000100 */
#define TIM_CCMR2_CC4S_1          (0x2UL << TIM_CCMR2_CC4S_Pos)                /* 0x00000200 */

#define TIM_CCMR2_OC4FE_DPos      (10U)
#define TIM_CCMR2_OC4FE_Pos       TIM_CCMR2_OC4FE_DPos
#define TIM_CCMR2_OC4FE_Msk       (0x1UL << TIM_CCMR2_OC4FE_Pos)               /* 0x00000400 */
#define TIM_CCMR2_OC4FE           TIM_CCMR2_OC4FE_Msk                          /* Output compare 4 fast enable */

#define TIM_CCMR2_OC4PE_DPos      (11U)
#define TIM_CCMR2_OC4PE_Pos       TIM_CCMR2_OC4PE_DPos
#define TIM_CCMR2_OC4PE_Msk       (0x1UL << TIM_CCMR2_OC4PE_Pos)               /* 0x00000800 */
#define TIM_CCMR2_OC4PE           TIM_CCMR2_OC4PE_Msk                          /* Output compare 4 preload enable */

#define TIM_CCMR2_OC4M_DPos       (12U)
#define TIM_CCMR2_OC4M_Pos        TIM_CCMR2_OC4M_DPos
#define TIM_CCMR2_OC4M_Msk        (0x1007UL << TIM_CCMR2_OC4M_Pos)             /* 0x01007000 */
#define TIM_CCMR2_OC4M            TIM_CCMR2_OC4M_Msk                           /* OC4M[2:0] bits (Output compare 4 mode) */
#define TIM_CCMR2_OC4M_0          (0x0001UL << TIM_CCMR2_OC4M_Pos)             /* 0x00001000 */
#define TIM_CCMR2_OC4M_1          (0x0002UL << TIM_CCMR2_OC4M_Pos)             /* 0x00002000 */
#define TIM_CCMR2_OC4M_2          (0x0004UL << TIM_CCMR2_OC4M_Pos)             /* 0x00004000 */
#define TIM_CCMR2_OC4M_3          (0x1000UL << TIM_CCMR2_OC4M_Pos)             /* 0x01000000 */

#define TIM_CCMR2_OC4CE_DPos      (15U)
#define TIM_CCMR2_OC4CE_Pos       TIM_CCMR2_OC4CE_DPos
#define TIM_CCMR2_OC4CE_Msk       (0x1UL << TIM_CCMR2_OC4CE_Pos)               /* 0x00008000 */
#define TIM_CCMR2_OC4CE           TIM_CCMR2_OC4CE_Msk                          /* Output compare 4 clear enable */


/*----------------------------------------------------------------------------*/

#define TIM_CCMR2_IC3PSC_DPos     (2U)
#define TIM_CCMR2_IC3PSC_Pos      TIM_CCMR2_IC3PSC_DPos
#define TIM_CCMR2_IC3PSC_Msk      (0x3UL << TIM_CCMR2_IC3PSC_Pos)              /* 0x0000000C */
#define TIM_CCMR2_IC3PSC          TIM_CCMR2_IC3PSC_Msk                         /* IC3PSC[1:0] bits (Input capture 3 prescaler) */
#define TIM_CCMR2_IC3PSC_0        (0x1UL << TIM_CCMR2_IC3PSC_Pos)              /* 0x00000004 */
#define TIM_CCMR2_IC3PSC_1        (0x2UL << TIM_CCMR2_IC3PSC_Pos)              /* 0x00000008 */

#define TIM_CCMR2_IC3F_DPos       (4U)
#define TIM_CCMR2_IC3F_Pos        TIM_CCMR2_IC3F_DPos
#define TIM_CCMR2_IC3F_Msk        (0xFUL << TIM_CCMR2_IC3F_Pos)                /* 0x000000F0 */
#define TIM_CCMR2_IC3F            TIM_CCMR2_IC3F_Msk                           /* IC3F[3:0] bits (Input capture 3 filter) */
#define TIM_CCMR2_IC3F_0          (0x1UL << TIM_CCMR2_IC3F_Pos)                /* 0x00000010 */
#define TIM_CCMR2_IC3F_1          (0x2UL << TIM_CCMR2_IC3F_Pos)                /* 0x00000020 */
#define TIM_CCMR2_IC3F_2          (0x4UL << TIM_CCMR2_IC3F_Pos)                /* 0x00000040 */
#define TIM_CCMR2_IC3F_3          (0x8UL << TIM_CCMR2_IC3F_Pos)                /* 0x00000080 */

#define TIM_CCMR2_IC4PSC_DPos     (10U)
#define TIM_CCMR2_IC4PSC_Pos      TIM_CCMR2_IC4PSC_DPos
#define TIM_CCMR2_IC4PSC_Msk      (0x3UL << TIM_CCMR2_IC4PSC_Pos)              /* 0x00000C00 */
#define TIM_CCMR2_IC4PSC          TIM_CCMR2_IC4PSC_Msk                         /* IC4PSC[1:0] bits (Input capture 4 prescaler) */
#define TIM_CCMR2_IC4PSC_0        (0x1UL << TIM_CCMR2_IC4PSC_Pos)              /* 0x00000400 */
#define TIM_CCMR2_IC4PSC_1        (0x2UL << TIM_CCMR2_IC4PSC_Pos)     

#define TIM_CCMR2_IC4F_DPos       (12U)
#define TIM_CCMR2_IC4F_Pos        TIM_CCMR2_IC4F_DPos
#define TIM_CCMR2_IC4F_Msk        (0xFUL << TIM_CCMR2_IC4F_Pos)                /* 0x0000F000 */
#define TIM_CCMR2_IC4F            TIM_CCMR2_IC4F_Msk                           /* IC4F[3:0] bits (Input capture 4 filter) */
#define TIM_CCMR2_IC4F_0          (0x1UL << TIM_CCMR2_IC4F_Pos)                /* 0x00001000 */
#define TIM_CCMR2_IC4F_1          (0x2UL << TIM_CCMR2_IC4F_Pos)                /* 0x00002000 */
#define TIM_CCMR2_IC4F_2          (0x4UL << TIM_CCMR2_IC4F_Pos)                /* 0x00004000 */
#define TIM_CCMR2_IC4F_3          (0x8UL << TIM_CCMR2_IC4F_Pos)                /* 0x00008000 */

/******************  Bit definition for TIM_CCMR3 register  *******************/
#define TIM_CCMR3_CC5S_DPos       (0U)
#define TIM_CCMR3_CC5S_Pos        TIM_CCMR3_CC5S_DPos
#define TIM_CCMR3_CC5S_Msk        (0x3UL << TIM_CCMR3_CC5S_Pos)                /* 0x00000003 */
#define TIM_CCMR3_CC5S            TIM_CCMR3_CC5S_Msk                           /* CC5S[1:0] bits (Capture/Compare 5 selection) */
#define TIM_CCMR3_CC5S_0          (0x1UL << TIM_CCMR3_CC5S_Pos)                /* 0x00000001 */
#define TIM_CCMR3_CC5S_1          (0x2UL << TIM_CCMR3_CC5S_Pos)                /* 0x00000002 */

#define TIM_CCMR3_OC5FE_DPos      (2U)
#define TIM_CCMR3_OC5FE_Pos       TIM_CCMR3_OC5FE_DPos
#define TIM_CCMR3_OC5FE_Msk       (0x1UL << TIM_CCMR3_OC5FE_Pos)               /* 0x00000004 */
#define TIM_CCMR3_OC5FE           TIM_CCMR3_OC5FE_Msk                          /* Output compare 5 fast enable */

#define TIM_CCMR3_OC5PE_DPos      (3U)
#define TIM_CCMR3_OC5PE_Pos       TIM_CCMR3_OC5PE_DPos
#define TIM_CCMR3_OC5PE_Msk       (0x1UL << TIM_CCMR3_OC5PE_Pos)               /* 0x00000008 */
#define TIM_CCMR3_OC5PE           TIM_CCMR3_OC5PE_Msk                          /* Output compare 5 preload enable */

#define TIM_CCMR3_OC5M_DPos       (4U)
#define TIM_CCMR3_OC5M_Pos        TIM_CCMR3_OC5M_DPos
#define TIM_CCMR3_OC5M_Msk        (0x1007UL << TIM_CCMR3_OC5M_Pos)             /* 0x00010070 */
#define TIM_CCMR3_OC5M            TIM_CCMR3_OC5M_Msk                           /* OC5M[2:0] bits (Output compare 5 mode) */
#define TIM_CCMR3_OC5M_0          (0x0001UL << TIM_CCMR3_OC5M_Pos)             /* 0x00000010 */
#define TIM_CCMR3_OC5M_1          (0x0002UL << TIM_CCMR3_OC5M_Pos)             /* 0x00000020 */
#define TIM_CCMR3_OC5M_2          (0x0004UL << TIM_CCMR3_OC5M_Pos)             /* 0x00000040 */
#define TIM_CCMR3_OC5M_3          (0x1000UL << TIM_CCMR3_OC5M_Pos)             /* 0x00010000 */

#define TIM_CCMR3_OC5CE_DPos      (7U)
#define TIM_CCMR3_OC5CE_Pos       TIM_CCMR3_OC5CE_DPos
#define TIM_CCMR3_OC5CE_Msk       (0x1UL << TIM_CCMR3_OC5CE_Pos)               /* 0x00000080 */
#define TIM_CCMR3_OC5CE           TIM_CCMR3_OC5CE_Msk  

#define TIM_CCMR3_CC6S_DPos       (8U)
#define TIM_CCMR3_CC6S_Pos        TIM_CCMR3_CC6S_DPos
#define TIM_CCMR3_CC6S_Msk        (0x3UL << TIM_CCMR3_CC6S_Pos)                /* 0x00000300 */
#define TIM_CCMR3_CC6S            TIM_CCMR3_CC6S_Msk                           /* CC6S[1:0] bits (Capture/Compare 6 selection) */
#define TIM_CCMR3_CC6S_0          (0x1UL << TIM_CCMR3_CC6S_Pos)                /* 0x00000100 */
#define TIM_CCMR3_CC6S_1          (0x2UL << TIM_CCMR3_CC6S_Pos)                /* 0x00000200 */

#define TIM_CCMR3_OC6FE_DPos      (10U)
#define TIM_CCMR3_OC6FE_Pos       TIM_CCMR3_OC6FE_DPos
#define TIM_CCMR3_OC6FE_Msk       (0x1UL << TIM_CCMR3_OC6FE_Pos)               /* 0x00000400 */
#define TIM_CCMR3_OC6FE           TIM_CCMR3_OC6FE_Msk                          /* Output compare 6 fast enable */

#define TIM_CCMR3_OC6PE_DPos      (11U)
#define TIM_CCMR3_OC6PE_Pos       TIM_CCMR3_OC6PE_DPos
#define TIM_CCMR3_OC6PE_Msk       (0x1UL << TIM_CCMR3_OC6PE_Pos)               /* 0x00000800 */
#define TIM_CCMR3_OC6PE           TIM_CCMR3_OC6PE_Msk                          /* Output compare 6 preload enable */

#define TIM_CCMR3_OC6M_DPos       (12U)
#define TIM_CCMR3_OC6M_Pos        TIM_CCMR3_OC6M_DPos
#define TIM_CCMR3_OC6M_Msk        (0x1007UL << TIM_CCMR3_OC6M_Pos)             /* 0x01007000 */
#define TIM_CCMR3_OC6M            TIM_CCMR3_OC6M_Msk                           /* OC6M[2:0] bits (Output compare 6 mode) */
#define TIM_CCMR3_OC6M_0          (0x0001UL << TIM_CCMR3_OC6M_Pos)             /* 0x00001000 */
#define TIM_CCMR3_OC6M_1          (0x0002UL << TIM_CCMR3_OC6M_Pos)             /* 0x00002000 */
#define TIM_CCMR3_OC6M_2          (0x0004UL << TIM_CCMR3_OC6M_Pos)             /* 0x00004000 */
#define TIM_CCMR3_OC6M_3          (0x1000UL << TIM_CCMR3_OC6M_Pos)             /* 0x01000000 */

#define TIM_CCMR3_OC6CE_DPos      (15U)
#define TIM_CCMR3_OC6CE_Pos       TIM_CCMR3_OC6CE_DPos
#define TIM_CCMR3_OC6CE_Msk       (0x1UL << TIM_CCMR3_OC6CE_Pos)               /* 0x00008000 */
#define TIM_CCMR3_OC6CE           TIM_CCMR3_OC6CE_Msk                          /* Output compare 6 clear enable */

/*----------------------------------------------------------------------------*/

#define TIM_CCMR3_IC5PSC_DPos     (2U)
#define TIM_CCMR3_IC5PSC_Pos      TIM_CCMR3_IC5PSC_DPos
#define TIM_CCMR3_IC5PSC_Msk      (0x3UL << TIM_CCMR3_IC5PSC_Pos)              /* 0x0000000C */
#define TIM_CCMR3_IC5PSC          TIM_CCMR3_IC5PSC_Msk                         /*IC5PSC[1:0] bits (Input capture 5 prescaler) */
#define TIM_CCMR3_IC5PSC_0        (0x1UL << TIM_CCMR3_IC5PSC_Pos)              /* 0x00000004 */
#define TIM_CCMR3_IC5PSC_1        (0x2UL << TIM_CCMR3_IC5PSC_Pos)              /* 0x00000008 */

#define TIM_CCMR3_IC5F_DPos       (4U)
#define TIM_CCMR3_IC5F_Pos        TIM_CCMR3_IC5F_DPos
#define TIM_CCMR3_IC5F_Msk        (0xFUL << TIM_CCMR3_IC5F_Pos)                /* 0x000000F0 */
#define TIM_CCMR3_IC5F            TIM_CCMR3_IC5F_Msk                           /*IC5F[3:0] bits (Input capture 5 filter) */
#define TIM_CCMR3_IC5F_0          (0x1UL << TIM_CCMR3_IC5F_Pos)                /* 0x00000010 */
#define TIM_CCMR3_IC5F_1          (0x2UL << TIM_CCMR3_IC5F_Pos)                /* 0x00000020 */
#define TIM_CCMR3_IC5F_2          (0x4UL << TIM_CCMR3_IC5F_Pos)                /* 0x00000040 */
#define TIM_CCMR3_IC5F_3          (0x8UL << TIM_CCMR3_IC5F_Pos)                /* 0x00000080 */

#define TIM_CCMR3_IC6PSC_DPos     (10U)
#define TIM_CCMR3_IC6PSC_Pos      TIM_CCMR3_IC6PSC_DPos
#define TIM_CCMR3_IC6PSC_Msk      (0x3UL << TIM_CCMR3_IC6PSC_Pos)              /* 0x00000C00 */
#define TIM_CCMR3_IC6PSC          TIM_CCMR3_IC6PSC_Msk                         /*IC6PSC[1:0] bits (Input capture 6 prescaler) */
#define TIM_CCMR3_IC6PSC_0        (0x1UL << TIM_CCMR3_IC6PSC_Pos)              /* 0x00000400 */
#define TIM_CCMR3_IC6PSC_1        (0x2UL << TIM_CCMR3_IC6PSC_Pos)              /* 0x00000800 */

#define TIM_CCMR3_IC6F_DPos       (12U)
#define TIM_CCMR3_IC6F_Pos        TIM_CCMR3_IC6F_DPos
#define TIM_CCMR3_IC6F_Msk        (0xFUL << TIM_CCMR3_IC6F_Pos)                /* 0x0000F000 */
#define TIM_CCMR3_IC6F            TIM_CCMR3_IC6F_Msk                           /* IC6F[3:0] bits (Input capture 6 filter) */
#define TIM_CCMR3_IC6F_0          (0x1UL << TIM_CCMR3_IC6F_Pos)                /* 0x00001000 */
#define TIM_CCMR3_IC6F_1          (0x2UL << TIM_CCMR3_IC6F_Pos)                /* 0x00002000 */
#define TIM_CCMR3_IC6F_2          (0x4UL << TIM_CCMR3_IC6F_Pos)                /* 0x00004000 */
#define TIM_CCMR3_IC6F_3          (0x8UL << TIM_CCMR3_IC6F_Pos)                /* 0x00008000 */

/*******************  Bit definition for TIM_CCER register  *******************/
#define TIM_CCER_CC1E_DPos        (0U)
#define TIM_CCER_CC1E_Pos         TIM_CCER_CC1E_DPos
#define TIM_CCER_CC1E_Msk         (0x1UL << TIM_CCER_CC1E_Pos)                 /* 0x00000001 */
#define TIM_CCER_CC1E             TIM_CCER_CC1E_Msk                            /* Capture/Compare 1 output enable */

#define TIM_CCER_CC1P_DPos        (1U)
#define TIM_CCER_CC1P_Pos         TIM_CCER_CC1P_DPos
#define TIM_CCER_CC1P_Msk         (0x1UL << TIM_CCER_CC1P_Pos)                 /* 0x00000002 */
#define TIM_CCER_CC1P             TIM_CCER_CC1P_Msk                            /* Capture/Compare 1 output polarity */

#define TIM_CCER_CC1NE_DPos       (2U)
#define TIM_CCER_CC1NE_Pos        TIM_CCER_CC1NE_DPos
#define TIM_CCER_CC1NE_Msk        (0x1UL << TIM_CCER_CC1NE_Pos)                /* 0x00000004 */
#define TIM_CCER_CC1NE            TIM_CCER_CC1NE_Msk                           /* Capture/Compare 1 complementary output enable */

#define TIM_CCER_CC1NP_DPos       (3U)
#define TIM_CCER_CC1NP_Pos        TIM_CCER_CC1NP_DPos
#define TIM_CCER_CC1NP_Msk        (0x1UL << TIM_CCER_CC1NP_Pos)                /* 0x00000008 */
#define TIM_CCER_CC1NP            TIM_CCER_CC1NP_Msk                           /* Capture/Compare 1 complementary output polarity */

#define TIM_CCER_CC2E_DPos        (4U)
#define TIM_CCER_CC2E_Pos         TIM_CCER_CC2E_DPos
#define TIM_CCER_CC2E_Msk         (0x1UL << TIM_CCER_CC2E_Pos)                 /* 0x00000010 */
#define TIM_CCER_CC2E             TIM_CCER_CC2E_Msk                            /* Capture/Compare 2 output enable */

#define TIM_CCER_CC2P_DPos        (5U)
#define TIM_CCER_CC2P_Pos         TIM_CCER_CC2P_DPos
#define TIM_CCER_CC2P_Msk         (0x1UL << TIM_CCER_CC2P_Pos)                 /* 0x00000020 */
#define TIM_CCER_CC2P             TIM_CCER_CC2P_Msk                            /* Capture/Compare 2 output polarity */

#define TIM_CCER_CC2NE_DPos        (6U)
#define TIM_CCER_CC2NE_Pos         TIM_CCER_CC2NE_DPos
#define TIM_CCER_CC2NE_Msk         (0x1UL << TIM_CCER_CC2NE_Pos)                /* 0x00000040 */
#define TIM_CCER_CC2NE             TIM_CCER_CC2NE_Msk     

#define TIM_CCER_CC2NP_DPos        (7U)
#define TIM_CCER_CC2NP_Pos         TIM_CCER_CC2NP_DPos
#define TIM_CCER_CC2NP_Msk         (0x1UL << TIM_CCER_CC2NP_Pos)                /* 0x00000080 */
#define TIM_CCER_CC2NP             TIM_CCER_CC2NP_Msk                           /* Capture/Compare 2 complementary output polarity */

#define TIM_CCER_CC3E_DPos         (8U)
#define TIM_CCER_CC3E_Pos          TIM_CCER_CC3E_DPos
#define TIM_CCER_CC3E_Msk          (0x1UL << TIM_CCER_CC3E_Pos)                 /* 0x00000100 */
#define TIM_CCER_CC3E              TIM_CCER_CC3E_Msk                            /* Capture/Compare 3 output enable */

#define TIM_CCER_CC3P_DPos         (9U)
#define TIM_CCER_CC3P_Pos          TIM_CCER_CC3P_DPos
#define TIM_CCER_CC3P_Msk          (0x1UL << TIM_CCER_CC3P_Pos)                 /* 0x00000200 */
#define TIM_CCER_CC3P              TIM_CCER_CC3P_Msk                            /* Capture/Compare 3 output polarity */

#define TIM_CCER_CC3NE_DPos        (10U)
#define TIM_CCER_CC3NE_Pos         TIM_CCER_CC3NE_DPos
#define TIM_CCER_CC3NE_Msk         (0x1UL << TIM_CCER_CC3NE_Pos)                /* 0x00000400 */
#define TIM_CCER_CC3NE             TIM_CCER_CC3NE_Msk                           /* Capture/Compare 3 complementary output enable */

#define TIM_CCER_CC3NP_DPos        (11U)
#define TIM_CCER_CC3NP_Pos         TIM_CCER_CC3NP_DPos
#define TIM_CCER_CC3NP_Msk         (0x1UL << TIM_CCER_CC3NP_Pos)                /* 0x00000800 */
#define TIM_CCER_CC3NP             TIM_CCER_CC3NP_Msk                           /* Capture/Compare 3 complementary output polarity */

#define TIM_CCER_CC4E_DPos         (12U)
#define TIM_CCER_CC4E_Pos          TIM_CCER_CC4E_DPos
#define TIM_CCER_CC4E_Msk          (0x1UL << TIM_CCER_CC4E_Pos)                 /* 0x00001000 */
#define TIM_CCER_CC4E              TIM_CCER_CC4E_Msk                            /* Capture/Compare 4 output enable */

#define TIM_CCER_CC4P_DPos         (13U)
#define TIM_CCER_CC4P_Pos          TIM_CCER_CC4P_DPos
#define TIM_CCER_CC4P_Msk          (0x1UL << TIM_CCER_CC4P_Pos)                 /* 0x00002000 */
#define TIM_CCER_CC4P              TIM_CCER_CC4P_Msk

#define TIM_CCER_CC4NE_DPos        (14U)
#define TIM_CCER_CC4NE_Pos         TIM_CCER_CC4NE_DPos
#define TIM_CCER_CC4NE_Msk         (0x1UL << TIM_CCER_CC4NE_Pos)                /* 0x00004000 */
#define TIM_CCER_CC4NE             TIM_CCER_CC4NE_Msk                           /* Capture/Compare 4 complementary output enable */

#define TIM_CCER_CC4NP_DPos        (15U)
#define TIM_CCER_CC4NP_Pos         TIM_CCER_CC4NP_DPos
#define TIM_CCER_CC4NP_Msk         (0x1UL << TIM_CCER_CC4NP_Pos)                /* 0x00008000 */
#define TIM_CCER_CC4NP             TIM_CCER_CC4NP_Msk                           /* Capture/Compare 4 complementary output polarity */

#define TIM_CCER_CC5E_DPos         (16U)
#define TIM_CCER_CC5E_Pos          TIM_CCER_CC5E_DPos
#define TIM_CCER_CC5E_Msk          (0x1UL << TIM_CCER_CC5E_Pos)                 /* 0x00010000 */
#define TIM_CCER_CC5E              TIM_CCER_CC5E_Msk                            /* Capture/Compare 5 output enable */

#define TIM_CCER_CC5P_DPos         (17U)
#define TIM_CCER_CC5P_Pos          TIM_CCER_CC5P_DPos
#define TIM_CCER_CC5P_Msk          (0x1UL << TIM_CCER_CC5P_Pos)                 /* 0x00020000 */
#define TIM_CCER_CC5P              TIM_CCER_CC5P_Msk                            /* Capture/Compare 5 output polarity */

#define TIM_CCER_CC5NE_DPos        (18U)
#define TIM_CCER_CC5NE_Pos         TIM_CCER_CC5NE_DPos
#define TIM_CCER_CC5NE_Msk         (0x1UL << TIM_CCER_CC5NE_Pos)                /* 0x00040000 */
#define TIM_CCER_CC5NE             TIM_CCER_CC5NE_Msk                           /* Capture/Compare 5 complementary output enable */

#define TIM_CCER_CC5NP_DPos        (19U)
#define TIM_CCER_CC5NP_Pos         TIM_CCER_CC5NP_DPos
#define TIM_CCER_CC5NP_Msk         (0x1UL << TIM_CCER_CC5NP_Pos)                /* 0x00080000 */
#define TIM_CCER_CC5NP             TIM_CCER_CC5NP_Msk                           /* Capture/Compare 5 complementary output polarity */

#define TIM_CCER_CC6E_DPos         (20U)
#define TIM_CCER_CC6E_Pos          TIM_CCER_CC6E_DPos
#define TIM_CCER_CC6E_Msk          (0x1UL << TIM_CCER_CC6E_Pos)                 /* 0x00100000 */
#define TIM_CCER_CC6E              TIM_CCER_CC6E_Msk     

#define TIM_CCER_CC6P_DPos         (21U)
#define TIM_CCER_CC6P_Pos          TIM_CCER_CC6P_DPos
#define TIM_CCER_CC6P_Msk          (0x1UL << TIM_CCER_CC6P_Pos)                 /* 0x00200000 */
#define TIM_CCER_CC6P              TIM_CCER_CC6P_Msk                            /* Capture/Compare 6 output polarity */

#define TIM_CCER_CC6NE_DPos        (22U)
#define TIM_CCER_CC6NE_Pos         TIM_CCER_CC6NE_DPos
#define TIM_CCER_CC6NE_Msk         (0x1UL << TIM_CCER_CC6NE_Pos)                /* 0x00400000 */
#define TIM_CCER_CC6NE             TIM_CCER_CC6NE_Msk                           /* Capture/Compare 6 complementary output enable */

#define TIM_CCER_CC6NP_DPos        (23U)
#define TIM_CCER_CC6NP_Pos         TIM_CCER_CC6NP_DPos
#define TIM_CCER_CC6NP_Msk         (0x1UL << TIM_CCER_CC6NP_Pos)                /* 0x00800000 */
#define TIM_CCER_CC6NP             TIM_CCER_CC6NP_Msk                           /* Capture/Compare 6 complementary output polarity */


/*******************  Bit definition for TIM_CNT register  ********************/
#define TIM_CNT_CNT_DPos          (0U)
#define TIM_CNT_CNT_Pos           TIM_CNT_CNT_DPos
#define TIM_CNT_CNT_Msk           (0xFFFFFFFFUL << TIM_CNT_CNT_Pos)            /* 0xFFFFFFFF */
#define TIM_CNT_CNT               TIM_CNT_CNT_Msk                              /* Counter value */

/*******************  Bit definition for TIM_PSC register  ********************/
#define TIM_PSC_PSC_DPos          (0U)
#define TIM_PSC_PSC_Pos           TIM_PSC_PSC_DPos
#define TIM_PSC_PSC_Msk           (0xFFFFFFFFUL << TIM_PSC_PSC_Pos)            /* 0xFFFFFFFF */
#define TIM_PSC_PSC               TIM_PSC_PSC_Msk                              /* Prescaler value */

/*******************  Bit definition for TIM_ARR register  ********************/
#define TIM_ARR_ARR_DPos          (0U)
#define TIM_ARR_ARR_Pos           TIM_ARR_ARR_DPos
#define TIM_ARR_ARR_Msk           (0xFFFFFFFFUL << TIM_ARR_ARR_Pos)            /* 0xFFFFFFFF */
#define TIM_ARR_ARR               TIM_ARR_ARR_Msk                              /* Auto-reload value */


/*******************  Bit definition for TIM_RCR register  ********************/
#define TIM_RCR_REP_DPos         (0U)
#define TIM_RCR_REP_Pos          TIM_RCR_REP_DPos
#define TIM_RCR_REP_Msk          (0xFFFFUL << TIM_RCR_REP_Pos)                 /* 0x0000FFFF */
#define TIM_RCR_REP              TIM_RCR_REP_Msk                              /* Repetition counter value */

/*******************  Bit definition for TIM_CCR1 register  *******************/
#define TIM_CCR1_CCR1_DPos        (0U)
#define TIM_CCR1_CCR1_Pos         TIM_CCR1_CCR1_DPos
#define TIM_CCR1_CCR1_Msk         (0xFFFFFFFFUL << TIM_CCR1_CCR1_Pos)          /* 0xFFFFFFFF */
#define TIM_CCR1_CCR1             TIM_CCR1_CCR1_Msk                            /* Capture/Compare 1 value */

/*******************  Bit definition for TIM_CCR2 register  *******************/
#define TIM_CCR2_CCR2_DPos        (0U)
#define TIM_CCR2_CCR2_Pos         TIM_CCR2_CCR2_DPos
#define TIM_CCR2_CCR2_Msk         (0xFFFFFFFFUL << TIM_CCR2_CCR2_Pos)          /* 0xFFFFFFFF */
#define TIM_CCR2_CCR2             TIM_CCR2_CCR2_Msk                            /* Capture/Compare 2 value */

/*******************  Bit definition for TIM_CCR3 register  *******************/
#define TIM_CCR3_CCR3_DPos        (0U)
#define TIM_CCR3_CCR3_Pos         TIM_CCR3_CCR3_DPos
#define TIM_CCR3_CCR3_Msk         (0xFFFFFFFFUL << TIM_CCR3_CCR3_Pos)          /* 0xFFFFFFFF */
#define TIM_CCR3_CCR3             TIM_CCR3_CCR3_Msk                            /* Capture/Compare 3 value */

/*******************  Bit definition for TIM_CCR4 register  *******************/
#define TIM_CCR4_CCR4_DPos        (0U)
#define TIM_CCR4_CCR4_Pos         TIM_CCR4_CCR4_DPos
#define TIM_CCR4_CCR4_Msk         (0xFFFFFFFFUL << TIM_CCR4_CCR4_Pos)          /* 0xFFFFFFFF */
#define TIM_CCR4_CCR4             TIM_CCR4_CCR4_Msk                            /* Capture/Compare 4 value */

/*******************  Bit definition for TIM_CCR5 register  *******************/
#define TIM_CCR5_CCR5_DPos        (0U)
#define TIM_CCR5_CCR5_Pos         TIM_CCR5_CCR5_DPos
#define TIM_CCR5_CCR5_Msk         (0xFFFFFFFFUL << TIM_CCR5_CCR5_Pos)          /* 0xFFFFFFFF */
#define TIM_CCR5_CCR5             TIM_CCR5_CCR5_Msk                            /* Capture/Compare 5 value */

/*******************  Bit definition for TIM_CCR6 register  *******************/
#define TIM_CCR6_CCR6_DPos        (0U)
#define TIM_CCR6_CCR6_Pos         TIM_CCR6_CCR6_DPos
#define TIM_CCR6_CCR6_Msk         (0xFFFFFFFFUL << TIM_CCR6_CCR6_Pos)          /* 0xFFFFFFFF */
#define TIM_CCR6_CCR6             TIM_CCR6_CCR6_Msk                            /* Capture/Compare 6 value */

/*******************  Bit definition for TIM_BDTR register  *******************/

#define TIM_BDTR_DTG_DPos         (0U)
#define TIM_BDTR_DTG_Pos          TIM_BDTR_DTG_DPos
#define TIM_BDTR_DTG_Msk          (0xFFUL << TIM_BDTR_DTG_Pos)                 /* 0x000000FF */
#define TIM_BDTR_DTG              TIM_BDTR_DTG_Msk                             /* DTG[0:7] bits (Dead-time generator setup) */
#define TIM_BDTR_DTG_0            (0x01UL << TIM_BDTR_DTG_Pos)                 /* 0x00000001 */
#define TIM_BDTR_DTG_1            (0x02UL << TIM_BDTR_DTG_Pos)                 /* 0x00000002 */
#define TIM_BDTR_DTG_2            (0x04UL << TIM_BDTR_DTG_Pos)                 /* 0x00000004 */
#define TIM_BDTR_DTG_3            (0x08UL << TIM_BDTR_DTG_Pos)                 /* 0x00000008 */
#define TIM_BDTR_DTG_4            (0x10UL << TIM_BDTR_DTG_Pos)                 /* 0x00000010 */
#define TIM_BDTR_DTG_5            (0x20UL << TIM_BDTR_DTG_Pos)                 /* 0x00000020 */
#define TIM_BDTR_DTG_6            (0x40UL << TIM_BDTR_DTG_Pos)                 /* 0x00000040 */
#define TIM_BDTR_DTG_7            (0x80UL << TIM_BDTR_DTG_Pos)                 /* 0x00000080 */

#define TIM_BDTR_OSSI_DPos        (10U)
#define TIM_BDTR_OSSI_Pos         TIM_BDTR_OSSI_DPos
#define TIM_BDTR_OSSI_Msk         (0x1UL << TIM_BDTR_OSSI_Pos)                 /* 0x00000400 */
#define TIM_BDTR_OSSI             TIM_BDTR_OSSI_Msk                            /* Off-state selection for Idle mode */

#define TIM_BDTR_OSSR_DPos        (11U)
#define TIM_BDTR_OSSR_Pos         TIM_BDTR_OSSR_DPos
#define TIM_BDTR_OSSR_Msk         (0x1UL << TIM_BDTR_OSSR_Pos)                 /* 0x00000800 */
#define TIM_BDTR_OSSR             TIM_BDTR_OSSR_Msk                            /* Off-state selection for Run mode */

#define TIM_BDTR_BKE_DPos         (12U)
#define TIM_BDTR_BKE_Pos          TIM_BDTR_BKE_DPos
#define TIM_BDTR_BKE_Msk          (0x1UL << TIM_BDTR_BKE_Pos)                  /* 0x00001000 */
#define TIM_BDTR_BKE              TIM_BDTR_BKE_Msk                             /* Break enable */

#define TIM_BDTR_BKP_DPos         (13U)
#define TIM_BDTR_BKP_Pos          TIM_BDTR_BKP_DPos
#define TIM_BDTR_BKP_Msk          (0x1UL << TIM_BDTR_BKP_Pos)                  /* 0x00002000 */
#define TIM_BDTR_BKP              TIM_BDTR_BKP_Msk      

#define TIM_BDTR_AOE_DPos         (14U)
#define TIM_BDTR_AOE_Pos          TIM_BDTR_AOE_DPos
#define TIM_BDTR_AOE_Msk          (0x1UL << TIM_BDTR_AOE_Pos)                  /* 0x00004000 */
#define TIM_BDTR_AOE              TIM_BDTR_AOE_Msk                             /* Automatic output enable */

#define TIM_BDTR_MOE_DPos         (15U)
#define TIM_BDTR_MOE_Pos          TIM_BDTR_MOE_DPos
#define TIM_BDTR_MOE_Msk          (0x1UL << TIM_BDTR_MOE_Pos)                  /* 0x00008000 */
#define TIM_BDTR_MOE              TIM_BDTR_MOE_Msk                             /* Main output enable */

#define TIM_BDTR_BKF_DPos         (16U)
#define TIM_BDTR_BKF_Pos          TIM_BDTR_BKF_DPos
#define TIM_BDTR_BKF_Msk          (0xFUL << TIM_BDTR_BKF_Pos)                  /* 0x000F0000 */
#define TIM_BDTR_BKF              TIM_BDTR_BKF_Msk                             /* Break filter */


/*******************  Bit definition for TIM_DCR register  ********************/
#define TIM_DCR_DBA_Pos           (0U)
#define TIM_DCR_DBA_Msk           (0x1FUL << TIM_DCR_DBA_Pos)                  /* 0x0000001F */
#define TIM_DCR_DBA               TIM_DCR_DBA_Msk                              /*DBA[4:0] bits (DMA base address) */
#define TIM_DCR_DBA_0             (0x01UL << TIM_DCR_DBA_Pos)                  /* 0x00000001 */
#define TIM_DCR_DBA_1             (0x02UL << TIM_DCR_DBA_Pos)                  /* 0x00000002 */
#define TIM_DCR_DBA_2             (0x04UL << TIM_DCR_DBA_Pos)                  /* 0x00000004 */
#define TIM_DCR_DBA_3             (0x08UL << TIM_DCR_DBA_Pos)                  /* 0x00000008 */
#define TIM_DCR_DBA_4             (0x10UL << TIM_DCR_DBA_Pos)                  /* 0x00000010 */

#define TIM_DCR_DBL_Pos           (8U)
#define TIM_DCR_DBL_Msk           (0x1FUL << TIM_DCR_DBL_Pos)                  /* 0x00001F00 */
#define TIM_DCR_DBL               TIM_DCR_DBL_Msk                              /*DBL[4:0] bits (DMA burst length) */
#define TIM_DCR_DBL_0             (0x01UL << TIM_DCR_DBL_Pos)                  /* 0x00000100 */
#define TIM_DCR_DBL_1             (0x02UL << TIM_DCR_DBL_Pos)                  /* 0x00000200 */
#define TIM_DCR_DBL_2             (0x04UL << TIM_DCR_DBL_Pos)                  /* 0x00000400 */
#define TIM_DCR_DBL_3             (0x08UL << TIM_DCR_DBL_Pos)                  /* 0x00000800 */
#define TIM_DCR_DBL_4             (0x10UL << TIM_DCR_DBL_Pos)                  /* 0x00001000 */

/*******************  Bit definition for TIM_DMAR register  *******************/
#define TIM_DMAR_DMAB_DPos        (0U)
#define TIM_DMAR_DMAB_Pos         TIM_DMAR_DMAB_DPos 
#define TIM_DMAR_DMAB_Msk         (0xFFFFFFFFUL << TIM_DMAR_DMAB_Pos)          /* 0xFFFFFFFF */
#define TIM_DMAR_DMAB             TIM_DMAR_DMAB_Msk                            /* DMA register for burst accesses */


/*******************  Bit definition for TIM_AF1 register  ********************/

#define TIM_AF1_ETRSEL_DPos       (0U)
#define TIM_AF1_ETRSEL_Pos        TIM_AF1_ETRSEL_DPos
#define TIM_AF1_ETRSEL_Msk        (0xFUL << TIM_AF1_ETRSEL_Pos)                /* 0x0000000F */
#define TIM_AF1_ETRSEL            TIM_AF1_ETRSEL_Msk                           /*ETRSEL[3:0] bits (ETR source selection) */
#define TIM_AF1_ETRSEL_0          (0x1UL << TIM_AF1_ETRSEL_Pos)                /* 0x00000001 */
#define TIM_AF1_ETRSEL_1          (0x2UL << TIM_AF1_ETRSEL_Pos)                /* 0x00000002 */
#define TIM_AF1_ETRSEL_2          (0x4UL << TIM_AF1_ETRSEL_Pos)                /* 0x00000004 */
#define TIM_AF1_ETRSEL_3          (0x8UL << TIM_AF1_ETRSEL_Pos)                /* 0x00000008 */


#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
