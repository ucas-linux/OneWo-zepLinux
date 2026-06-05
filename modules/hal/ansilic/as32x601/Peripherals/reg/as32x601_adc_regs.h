/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_adc_reg.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:			2024/8/29
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/8/29     1.0     build this module
 * Description:     This file contains all the register correlation defines for the ADC firmware library.
 */

#ifndef AS32X601_ADC_REGS_H
#define AS32X601_ADC_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601.h"
#include "as32x601_addrmap.h"

/*
 * @brief Max clock frequence of ADC function clock
 */
#define ADC_CLOCK_FREQ_MAX_RUNTIME (40000000U)

/*
 * @brief Number of ADC registers
 */
#define ADC_REGULAR_SEQ_NUM (16U)
#define ADC_INJECT_SEQ_NUM (4U)
#define ADC_SAMPLE_REG_NUM (3U)

/*
 * @brief General Purpose I/O registers structure
 */
typedef struct
{
	__IO uint32_t STATE;                      /* ADC Status Register,							  Address offset: 0x00 */
	__IO uint32_t CTRL0;                      /* ADC Control Register 0,						  Address offset: 0x04 */
	__IO uint32_t CTRL1;                      /* ADC Control Register 1,						  Address offset: 0x08 */
	__IO uint32_t AMOCR;                      /* ADC Analog monitor Configure Register,			  Address offset: 0x0C */
	__IO uint32_t AMOHR;                      /* ADC AMO High Threshold Register,				  Address offset: 0x10 */
	__IO uint32_t AMOLR;                      /* ADC AMO Low Threshold Register,				  Address offset: 0x14 */
	__IO uint32_t SPT[ADC_SAMPLE_REG_NUM];    /* ADC Sample Time Selection Register,			  Address offset: 0x18 - 0x20 */
	__IO uint32_t CONFIG;                     /* ADC Configure Register,						  Address offset: 0x24 */
	__IO uint32_t SQL;                        /* ADC Regular Sequence Length Register,			  Address offset: 0x28 */
	uint32_t RESERVED0;                       /* ADC Reserved,									  Address offset: 0x2C */
	__IO uint32_t IOFR[ADC_INJECT_SEQ_NUM];   /* ADC Injection Group Offset Register,			  Address offset: 0x30 - 0x3C */
	__IO uint32_t RSQR[ADC_REGULAR_SEQ_NUM];  /* ADC Regular Group Sequence Configure Register,	  Address offset: 0x40 - 0x7C */
	__IO uint32_t ISQR[ADC_INJECT_SEQ_NUM];   /* ADC Injection Group Sequence Configure Register, Address offset: 0x80 - 0x8C */
	__I  uint32_t RDR[ADC_REGULAR_SEQ_NUM];   /* ADC Regular Group Data Register,				  Address offset: 0x90 - 0xCC */
	__I  uint32_t IDR[ADC_INJECT_SEQ_NUM];    /* ADC Injection Group Data Register,				  Address offset: 0xD0 - 0xDC */
} ADC_TypeDef;

/*
 * @addtogroup ADC Peripheral declaration
 */
#define ADC0			((ADC_TypeDef*)ADC0_BASE)
#define ADC1			((ADC_TypeDef*)ADC1_BASE)

/******************  Bit definition for ADC_STATE register  *******************/
#define ADC_STATE_AMO_DPos        (0U)
#define ADC_STATE_AMO_Pos         ADC_STATE_AMO_DPos
#define ADC_STATE_AMO_Msk         (0x1UL << ADC_STATE_AMO_Pos)                  /* 0x00000001 */
#define ADC_STATE_AMO             ADC_STATE_AMO_Msk                             /* Analog monitor event occurs */

#define ADC_STATE_IDLE_DPos       (1UL)
#define ADC_STATE_IDLE_Pos        ADC_STATE_IDLE_DPos
#define ADC_STATE_IDLE_Msk        (0x1UL << ADC_STATE_IDLE_Pos)                 /* 0x00000002 */
#define ADC_STATE_IDLE            ADC_STATE_IDLE_Msk                            /* ADC idle state flag */

#define ADC_STATE_CONVCFT_DPos    (2UL)
#define ADC_STATE_CONVCFT_Pos     ADC_STATE_CONVCFT_DPos        
#define ADC_STATE_CONVCFT_Msk     (0x1UL << ADC_STATE_CONVCFT_Pos)              /* 0x00000004  */
#define ADC_STATE_CONVCFT         ADC_STATE_CONVCFT_Msk                         /* Convert trigger conflict status */

/******************  Bit definition for ADC_CTRL0 register  *******************/
#define ADC_CTRL0_IAUTO_DPos      (0U)
#define ADC_CTRL0_IAUTO_Pos       ADC_CTRL0_IAUTO_DPos
#define ADC_CTRL0_IAUTO_Msk       (0x1UL << ADC_CTRL0_IAUTO_Pos)                /* 0x00000001 */
#define ADC_CTRL0_IAUTO           ADC_CTRL0_IAUTO_Msk                           /* ADC IAUTO operation mode */

#define ADC_CTRL0_IDISCEN_DPos    (1UL)
#define ADC_CTRL0_IDISCEN_Pos     ADC_CTRL0_IDISCEN_DPos
#define ADC_CTRL0_IDISCEN_Msk     (0x1UL << ADC_CTRL0_IDISCEN_Pos)              /* 0x00000002 */
#define ADC_CTRL0_IDISCEN         ADC_CTRL0_IDISCEN_Msk                         /* ADC IDISCEN operation mode */

#define ADC_CTRL0_DISCEN_DPos     (2UL)  
#define ADC_CTRL0_DISCEN_Pos      ADC_CTRL0_DISCEN_DPos                                        
#define ADC_CTRL0_DISCEN_Msk      (0x1UL << ADC_CTRL0_DISCEN_Pos)               /* 0x00000004 */
#define ADC_CTRL0_DISCEN          ADC_CTRL0_DISCEN_Msk                          /* ADC DISCEN operation mode */

#define ADC_CTRL0_CONT_DPos       (3U)
#define ADC_CTRL0_CONT_Pos        ADC_CTRL0_CONT_DPos
#define ADC_CTRL0_CONT_Msk        (0x1UL << ADC_CTRL0_CONT_Pos)                 /* 0x00000008 */
#define ADC_CTRL0_CONT            ADC_CTRL0_CONT_Msk                            /* ADC CONT operation mode */

#define ADC_CTRL0_SCAN_DPos       (4UL)
#define ADC_CTRL0_SCAN_Pos        ADC_CTRL0_SCAN_DPos
#define ADC_CTRL0_SCAN_Msk        (0x1UL << ADC_CTRL0_SCAN_Pos)                 /* 0x00000010 */
#define ADC_CTRL0_SCAN            ADC_CTRL0_SCAN_Msk                            /* ADC SCAN operation mode */

#define ADC_CTRL0_INTERVAL_DPos   (5UL)   
#define ADC_CTRL0_INTERVAL_Pos    ADC_CTRL0_INTERVAL_DPos                                       
#define ADC_CTRL0_INTERVAL_Msk    (0x1UL << ADC_CTRL0_INTERVAL_Pos)             /* 0x00000020 */
#define ADC_CTRL0_INTERVAL        ADC_CTRL0_INTERVAL_Msk                        /* Interval mode */

#define ADC_CTRL0_DISCNUM_DPos    (6U)
#define ADC_CTRL0_DISCNUM_Pos     ADC_CTRL0_DISCNUM_DPos
#define ADC_CTRL0_DISCNUM_Msk     (0x7UL << ADC_CTRL0_DISCNUM_Pos)              /* 0x000001C0 */
#define ADC_CTRL0_DISCNUM         ADC_CTRL0_DISCNUM_Msk                         /* DISCNUM[2:0] bits (Discontinuous conversion length of channel)*/
#define ADC_CTRL0_DISCNUM_0       (0x1UL << ADC_CTRL0_DISCNUM_Pos)              /* 0x00000040 */
#define ADC_CTRL0_DISCNUM_1       (0x2UL << ADC_CTRL0_DISCNUM_Pos)              /* 0x00000080 */
#define ADC_CTRL0_DISCNUM_2       (0x4UL << ADC_CTRL0_DISCNUM_Pos)              /* 0x00000100 */

#define ADC_CTRL0_ALIGN_DPos      (9UL)
#define ADC_CTRL0_ALIGN_Pos       ADC_CTRL0_ALIGN_DPos
#define ADC_CTRL0_ALIGN_Msk       (0x1UL << ADC_CTRL0_ALIGN_Pos)                /* 0x00000200 */
#define ADC_CTRL0_ALIGN           ADC_CTRL0_ALIGN_Msk                           /* Data alignment */

#define ADC_CTRL0_SELDO_DPos      (10UL)    
#define ADC_CTRL0_SELDO_Pos       ADC_CTRL0_SELDO_DPos                                        
#define ADC_CTRL0_SELDO_Msk       (0x1UL << ADC_CTRL0_SELDO_Pos)                /* 0x00000400 */
#define ADC_CTRL0_SELDO           ADC_CTRL0_SELDO_Msk                           /* data type selection */

#define ADC_CTRL0_DMAEN_DPos      (11U)
#define ADC_CTRL0_DMAEN_Pos       ADC_CTRL0_DMAEN_DPos
#define ADC_CTRL0_DMAEN_Msk       (0x1UL << ADC_CTRL0_DMAEN_Pos)                /* 0x00000800 */
#define ADC_CTRL0_DMAEN           ADC_CTRL0_DMAEN_Msk                           /* DMA function enable */

#define ADC_CTRL0_TRIGEN_DPos     (16UL)
#define ADC_CTRL0_TRIGEN_Pos      ADC_CTRL0_TRIGEN_DPos
#define ADC_CTRL0_TRIGEN_Msk      (0x1UL << ADC_CTRL0_TRIGEN_Pos)               /* 0x00010000 */
#define ADC_CTRL0_TRIGEN          ADC_CTRL0_TRIGEN_Msk                          /* Regular group trigger enable */

#define ADC_CTRL0_SWSTART_DPos    (17UL)   
#define ADC_CTRL0_SWSTART_Pos     ADC_CTRL0_SWSTART_DPos                                       
#define ADC_CTRL0_SWSTART_Msk     (0x1UL << ADC_CTRL0_SWSTART_Pos)              /* 0x00020000 */
#define ADC_CTRL0_SWSTART         ADC_CTRL0_SWSTART_Msk                         /* Software trigger for regular channels */

#define ADC_CTRL0_TRIGSRC_DPos    (18UL)
#define ADC_CTRL0_TRIGSRC_Pos     ADC_CTRL0_TRIGSRC_DPos
#define ADC_CTRL0_TRIGSRC_Msk     (0x3FUL << ADC_CTRL0_TRIGSRC_Pos)             /* 0x00FC0000 */
#define ADC_CTRL0_TRIGSRC         ADC_CTRL0_TRIGSRC_Msk                         /* TRIGSRC[5:0] bits (Regular group trigger source select)*/
#define ADC_CTRL0_TRIGSRC_0       (0x1UL << ADC_CTRL0_DISCNUM_Pos)              /* 0x00040000 */
#define ADC_CTRL0_TRIGSRC_1       (0x2UL << ADC_CTRL0_DISCNUM_Pos)              /* 0x00080000 */
#define ADC_CTRL0_TRIGSRC_2       (0x4UL << ADC_CTRL0_DISCNUM_Pos)              /* 0x00100000 */
#define ADC_CTRL0_TRIGSRC_3       (0x8UL << ADC_CTRL0_DISCNUM_Pos)              /* 0x00200000 */
#define ADC_CTRL0_TRIGSRC_4       (0x10UL << ADC_CTRL0_DISCNUM_Pos)             /* 0x00400000 */
#define ADC_CTRL0_TRIGSRC_5       (0x20UL << ADC_CTRL0_DISCNUM_Pos)             /* 0x00800000 */

#define ADC_CTRL0_ITRIGEN_DPos    (24UL)   
#define ADC_CTRL0_ITRIGEN_Pos     ADC_CTRL0_ITRIGEN_DPos                                        
#define ADC_CTRL0_ITRIGEN_Msk     (0x1UL << ADC_CTRL0_ITRIGEN_Pos)              /* 0x01000000 */
#define ADC_CTRL0_ITRIGEN         ADC_CTRL0_ITRIGEN_Msk                         /* Injection group trigger enable */

#define ADC_CTRL0_ISWSTART_DPos   (25U)
#define ADC_CTRL0_ISWSTART_Pos    ADC_CTRL0_ISWSTART_DPos
#define ADC_CTRL0_ISWSTART_Msk    (0x1UL << ADC_CTRL0_ISWSTART_Pos)             /* 0x02000000 */
#define ADC_CTRL0_ISWSTART        ADC_CTRL0_ISWSTART_Msk                        /* Software trigger for injection channels */

#define ADC_CTRL0_ITRIGSRC_DPos   (26UL)
#define ADC_CTRL0_ITRIGSRC_Pos    ADC_CTRL0_ITRIGSRC_DPos
#define ADC_CTRL0_ITRIGSRC_Msk    (0x3FUL << ADC_CTRL0_ITRIGSRC_Pos)            /* 0xFC000000 */
#define ADC_CTRL0_ITRIGSRC        ADC_CTRL0_ITRIGSRC_Msk                        /* ITRIGSRC[5:0] bits (Injection group trigger source select)*/
#define ADC_CTRL0_ITRIGSRC_0      (0x1UL << ADC_CTRL0_ITRIGSRC_Pos)             /* 0x04000000 */
#define ADC_CTRL0_ITRIGSRC_1      (0x2UL << ADC_CTRL0_ITRIGSRC_Pos)             /* 0x08000000 */
#define ADC_CTRL0_ITRIGSRC_2      (0x4UL << ADC_CTRL0_ITRIGSRC_Pos)             /* 0x10000000 */
#define ADC_CTRL0_ITRIGSRC_3      (0x8UL << ADC_CTRL0_ITRIGSRC_Pos)             /* 0x20000000 */
#define ADC_CTRL0_ITRIGSRC_4      (0x10UL << ADC_CTRL0_ITRIGSRC_Pos)            /* 0x40000000 */
#define ADC_CTRL0_ITRIGSRC_5      (0x20UL << ADC_CTRL0_ITRIGSRC_Pos)            /* 0x80000000 */

/******************  Bit definition for ADC_CTRL1 register  *******************/
#define ADC_CTRL1_ADON_DPos       (0U)
#define ADC_CTRL1_ADON_Pos        ADC_CTRL1_ADON_DPos
#define ADC_CTRL1_ADON_Msk        (0x1UL << ADC_CTRL1_ADON_Pos)                 /* 0x00000001 */
#define ADC_CTRL1_ADON            ADC_CTRL1_ADON_Msk                            /* ADC Power on */

#define ADC_CTRL1_RESETN_DPos     (1U)
#define ADC_CTRL1_RESETN_Pos      ADC_CTRL1_RESETN_DPos
#define ADC_CTRL1_RESETN_Msk      (0x1UL << ADC_CTRL1_RESETN_Pos)               /* 0x00000002 */
#define ADC_CTRL1_RESETN          ADC_CTRL1_RESETN_Msk                          /* ADC reset */

#define ADC_CTRL1_PSC_DPos        (2U)
#define ADC_CTRL1_PSC_Pos         ADC_CTRL1_PSC_DPos
#define ADC_CTRL1_PSC_Msk         (0x1FUL << ADC_CTRL1_PSC_Pos)                 /* 0x0000007C */
#define ADC_CTRL1_PSC             ADC_CTRL1_PSC_Msk                             /* PSC[2:0] bits (ADC conversion clock prescaler used to generate ADCCLK)*/
#define ADC_CTRL1_PSC_0           (0x1UL << ADC_CTRL1_PSC_Pos)                  /* 0x00000004 */
#define ADC_CTRL1_PSC_1           (0x2UL << ADC_CTRL1_PSC_Pos)                  /* 0x00000008 */
#define ADC_CTRL1_PSC_2           (0x4UL << ADC_CTRL1_PSC_Pos)                  /* 0x00000010 */
#define ADC_CTRL1_PSC_3           (0x8UL << ADC_CTRL1_PSC_Pos)                  /* 0x00000020 */
#define ADC_CTRL1_PSC_4           (0x10UL << ADC_CTRL1_PSC_Pos)                 /* 0x00000040 */

#define ADC_CTRL1_RESL_DPos       (10U)
#define ADC_CTRL1_RESL_Pos        ADC_CTRL1_RESL_DPos
#define ADC_CTRL1_RESL_Msk        (0x3U << ADC_CTRL1_RESL_Pos)                  /* 0x00000C00 */
#define ADC_CTRL1_RESL            ADC_CTRL1_RESL_Msk                            /* RESL[1:0] bits (ADC conversion resolution selction)*/
#define ADC_CTRL1_RESL_0          (0x1U << ADC_CTRL1_RESL_Pos)                  /* 0x00000400 */
#define ADC_CTRL1_RESL_1          (0x2U << ADC_CTRL1_RESL_Pos)                  /* 0x00000800 */

/******************  Bit definition for ADC_AMOCR register  *******************/
#define ADC_AMOCR_AMOCH_DPos      (0U)
#define ADC_AMOCR_AMOCH_Pos       ADC_AMOCR_AMOCH_DPos
#define ADC_AMOCR_AMOCH_Msk       (0x1FUL << ADC_AMOCR_AMOCH_Pos)               /* 0x0000001F */
#define ADC_AMOCR_AMOCH           ADC_AMOCR_AMOCH_Msk                           /* AMOCH[4:0] bits (Analog monitor detecting channel)*/
#define ADC_AMOCR_AMOCH_0         (0x1UL << ADC_AMOCR_AMOCH_Pos)                /* 0x00000001 */
#define ADC_AMOCR_AMOCH_1         (0x2UL << ADC_AMOCR_AMOCH_Pos)                /* 0x00000002 */
#define ADC_AMOCR_AMOCH_2         (0x4UL << ADC_AMOCR_AMOCH_Pos)                /* 0x00000004 */
#define ADC_AMOCR_AMOCH_3         (0x8UL << ADC_AMOCR_AMOCH_Pos)                /* 0x00000008 */
#define ADC_AMOCR_AMOCH_4         (0x10UL << ADC_AMOCR_AMOCH_Pos)               /* 0x00000010 */

#define ADC_AMOCR_AMOSGL_DPos     (5U)
#define ADC_AMOCR_AMOSGL_Pos      ADC_AMOCR_AMOSGL_DPos
#define ADC_AMOCR_AMOSGL_Msk      (0x1UL << ADC_AMOCR_AMOSGL_Pos)               /* 0x00000020 */
#define ADC_AMOCR_AMOSGL          ADC_AMOCR_AMOSGL_Msk                          /* Analog monitor AMOSGL function configuration */

#define ADC_AMOCR_IAMOEN_DPos     (6U)
#define ADC_AMOCR_IAMOEN_Pos      ADC_AMOCR_IAMOEN_DPos
#define ADC_AMOCR_IAMOEN_Msk      (0x1UL << ADC_AMOCR_IAMOEN_Pos)               /* 0x00000040 */
#define ADC_AMOCR_IAMOEN          ADC_AMOCR_IAMOEN_Msk                          /* Analog monitor IAMOEN function configuration */

#define ADC_AMOCR_AMOEN_DPos      (7U)
#define ADC_AMOCR_AMOEN_Pos       ADC_AMOCR_AMOEN_DPos
#define ADC_AMOCR_AMOEN_Msk       (0x1UL << ADC_AMOCR_AMOEN_Pos)                /* 0x00000080 */
#define ADC_AMOCR_AMOEN           ADC_AMOCR_AMOEN_Msk                           /* Analog monitor AMOEN function configuration */

#define ADC_AMOCR_AMOIE_DPos      (8U)
#define ADC_AMOCR_AMOIE_Pos       ADC_AMOCR_AMOIE_DPos
#define ADC_AMOCR_AMOIE_Msk       (0x1UL << ADC_AMOCR_AMOIE_Pos)                /* 0x00000100 */
#define ADC_AMOCR_AMOIE           ADC_AMOCR_AMOIE_Msk                           /* AMO interrupt function enable */

/******************  Bit definition for ADC_AMOHR register  *******************/
#define ADC_AMOHR_AMOHT_DPos      (0U)
#define ADC_AMOHR_AMOHT_Pos       ADC_AMOHR_AMOHT_DPos
#define ADC_AMOHR_AMOHT_Msk       (0xFFFUL << ADC_AMOHR_AMOHT_Pos)              /* 0x00000FFF */
#define ADC_AMOHR_AMOHT           ADC_AMOHR_AMOHT_Msk                           /* AMOHT[11:0] bits (Analog monitor higher threshold)*/
#define ADC_AMOHR_AMOHT_0         (0x1UL << ADC_AMOHR_AMOHT_Pos)                /* 0x00000001 */
#define ADC_AMOHR_AMOHT_1         (0x2UL << ADC_AMOHR_AMOHT_Pos)                /* 0x00000002 */
#define ADC_AMOHR_AMOHT_2         (0x4UL << ADC_AMOHR_AMOHT_Pos)                /* 0x00000004 */
#define ADC_AMOHR_AMOHT_3         (0x8UL << ADC_AMOHR_AMOHT_Pos)                /* 0x00000008 */
#define ADC_AMOHR_AMOHT_4         (0x10UL << ADC_AMOHR_AMOHT_Pos)               /* 0x00000010 */
#define ADC_AMOHR_AMOHT_5         (0x20UL << ADC_AMOHR_AMOHT_Pos)               /* 0x00000020 */
#define ADC_AMOHR_AMOHT_6         (0x40UL << ADC_AMOHR_AMOHT_Pos)               /* 0x00000040 */
#define ADC_AMOHR_AMOHT_7         (0x80UL << ADC_AMOHR_AMOHT_Pos)               /* 0x00000080 */
#define ADC_AMOHR_AMOHT_8         (0x100UL << ADC_AMOHR_AMOHT_Pos)              /* 0x00000100 */
#define ADC_AMOHR_AMOHT_9         (0x200UL << ADC_AMOHR_AMOHT_Pos)              /* 0x00000200 */
#define ADC_AMOHR_AMOHT_10        (0x400UL << ADC_AMOHR_AMOHT_Pos)              /* 0x00000400 */
#define ADC_AMOHR_AMOHT_11        (0x800UL << ADC_AMOHR_AMOHT_Pos)              /* 0x00000800 */

/******************  Bit definition for ADC_AMOLR register  *******************/
#define ADC_AMOLR_AMOLT_DPos      (0U)
#define ADC_AMOLR_AMOLT_Pos       ADC_AMOLR_AMOLT_DPos
#define ADC_AMOLR_AMOLT_Msk       (0xFFFUL << ADC_AMOLR_AMOLT_Pos)              /* 0x00000FFF */
#define ADC_AMOLR_AMOLT           ADC_AMOLR_AMOLT_Msk                           /* AMOLT[11:0] bits (Analog monitor lower threshold)*/
#define ADC_AMOLR_AMOLT_0         (0x1UL << ADC_AMOLR_AMOLT_Pos)                /* 0x00000001 */
#define ADC_AMOLR_AMOLT_1         (0x2UL << ADC_AMOLR_AMOLT_Pos)                /* 0x00000002 */
#define ADC_AMOLR_AMOLT_2         (0x4UL << ADC_AMOLR_AMOLT_Pos)                /* 0x00000004 */
#define ADC_AMOLR_AMOLT_3         (0x8UL << ADC_AMOLR_AMOLT_Pos)                /* 0x00000008 */
#define ADC_AMOLR_AMOLT_4         (0x10UL << ADC_AMOLR_AMOLT_Pos)               /* 0x00000010 */
#define ADC_AMOLR_AMOLT_5         (0x20UL << ADC_AMOLR_AMOLT_Pos)               /* 0x00000020 */
#define ADC_AMOLR_AMOLT_6         (0x40UL << ADC_AMOLR_AMOLT_Pos)               /* 0x00000040 */
#define ADC_AMOLR_AMOLT_7         (0x80UL << ADC_AMOLR_AMOLT_Pos)               /* 0x00000080 */
#define ADC_AMOLR_AMOLT_8         (0x100UL << ADC_AMOLR_AMOLT_Pos)              /* 0x00000100 */
#define ADC_AMOLR_AMOLT_9         (0x200UL << ADC_AMOLR_AMOLT_Pos)              /* 0x00000200 */
#define ADC_AMOLR_AMOLT_10        (0x400UL << ADC_AMOLR_AMOLT_Pos)              /* 0x00000400 */
#define ADC_AMOLR_AMOLT_11        (0x800UL << ADC_AMOLR_AMOLT_Pos)              /* 0x00000800 */

/*******************  Bit definition for ADC_SPT register  ********************/
#define ADC_SPT_SPT_DPos          (0U)
#define ADC_SPT_SPT_Pos           ADC_SPT_SPT_DPos
#define ADC_SPT_SPT_Msk           (0xFUL << ADC_SPT_SPT_Pos)                    /* 0x0000000F */
#define ADC_SPT_SPT               ADC_SPT_SPT_Msk                               /* SPT[3:0] bits (Sample time selection for each channels)*/
#define ADC_SPT_SPT_0             (0x1UL << ADC_SPT_SPT_Pos)                    /* 0x00000001 */
#define ADC_SPT_SPT_1             (0x2UL << ADC_SPT_SPT_Pos)                    /* 0x00000002 */
#define ADC_SPT_SPT_2             (0x4UL << ADC_SPT_SPT_Pos)                    /* 0x00000004 */
#define ADC_SPT_SPT_3             (0x8UL << ADC_SPT_SPT_Pos)                    /* 0x00000008 */
#define ADC_SPT_Width             (4UL)                                         /* SPT width (4bit) */

/******************  Bit definition for ADC_CONFIG register  ******************/
#define ADC_CONFIG_EN_VREFBI_DPos (0U)
#define ADC_CONFIG_EN_VREFBI_Pos  ADC_CONFIG_EN_VREFBI_DPos
#define ADC_CONFIG_EN_VREFBI_Msk  (0x1UL << ADC_CONFIG_EN_VREFBI_Pos)           /* 0x00000001 */
#define ADC_CONFIG_EN_VREFBI      ADC_CONFIG_EN_VREFBI_Msk                      /* Built-in reference voltage enable signal */

#define ADC_CONFIG_EN_HIZ_DPos    (1U)
#define ADC_CONFIG_EN_HIZ_Pos     ADC_CONFIG_EN_HIZ_DPos
#define ADC_CONFIG_EN_HIZ_Msk     (0x1UL << ADC_CONFIG_EN_HIZ_Pos)              /* 0x00000002 */
#define ADC_CONFIG_EN_HIZ         ADC_CONFIG_EN_HIZ_Msk                         /* High-resistance state control signal */

#define ADC_CONFIG_SEL_VREFBI_DPos (2U)
#define ADC_CONFIG_SEL_VREFBI_Pos ADC_CONFIG_SEL_VREFBI_DPos
#define ADC_CONFIG_SEL_VREFBI_Msk (0x3UL << ADC_CONFIG_SEL_VREFBI_Pos)          /* 0x0000000C */
#define ADC_CONFIG_SEL_VREFBI     ADC_CONFIG_SEL_VREFBI_Msk                     /* SEL_VREFBI[1:0] bits (Built-in reference output voltage selected signal)*/
#define ADC_CONFIG_SEL_VREFBI_0   (0x1UL << ADC_CONFIG_SEL_VREFBI_Pos)          /* 0x00000004 */
#define ADC_CONFIG_SEL_VREFBI_1   (0x2UL << ADC_CONFIG_SEL_VREFBI_Pos)          /* 0x00000008 */

#define ADC_CONFIG_VTRIM_DPos     (4U)
#define ADC_CONFIG_VTRIM_Pos      ADC_CONFIG_VTRIM_DPos
#define ADC_CONFIG_VTRIM_Msk      (0x1FUL << ADC_CONFIG_VTRIM_Pos)              /* 0x000001F0 */
#define ADC_CONFIG_VTRIM          ADC_CONFIG_VTRIM_Msk                          /* VTRIM[4:0] bits (Built-In reference voltage output control)*/
#define ADC_CONFIG_VTRIM_0        (0x1UL << ADC_CONFIG_VTRIM_Pos)               /* 0x00000010 */
#define ADC_CONFIG_VTRIM_1        (0x2UL << ADC_CONFIG_VTRIM_Pos)               /* 0x00000020 */
#define ADC_CONFIG_VTRIM_2        (0x4UL << ADC_CONFIG_VTRIM_Pos)               /* 0x00000040 */
#define ADC_CONFIG_VTRIM_3        (0x8UL << ADC_CONFIG_VTRIM_Pos)               /* 0x00000080 */
#define ADC_CONFIG_VTRIM_4        (0x10UL << ADC_CONFIG_VTRIM_Pos)              /* 0x00000100 */

#define ADC_CONFIG_TTRIM_DPos     (9U)
#define ADC_CONFIG_TTRIM_Pos      ADC_CONFIG_TTRIM_DPos
#define ADC_CONFIG_TTRIM_Msk      (0xFUL << ADC_CONFIG_TTRIM_Pos)               /* 0x00001E00 */
#define ADC_CONFIG_TTRIM          ADC_CONFIG_TTRIM_Msk                          /* TTRIM[3:0] bits (Built-In reference voltage temperature ratio control)*/
#define ADC_CONFIG_TTRIM_0        (0x1UL << ADC_CONFIG_TTRIM_Pos)               /* 0x00000200 */
#define ADC_CONFIG_TTRIM_1        (0x2UL << ADC_CONFIG_TTRIM_Pos)               /* 0x00000400 */
#define ADC_CONFIG_TTRIM_2        (0x4UL << ADC_CONFIG_TTRIM_Pos)               /* 0x00000800 */
#define ADC_CONFIG_TTRIM_3        (0x8UL << ADC_CONFIG_TTRIM_Pos)               /* 0x00001000 */

#define ADC_CONFIG_SELVI_HD_LS_DPos (16U)
#define ADC_CONFIG_SELVI_HD_LS_Pos  ADC_CONFIG_SELVI_HD_LS_DPos
#define ADC_CONFIG_SELVI_HD_LS_Msk  (0x1UL << ADC_CONFIG_SELVI_HD_LS_Pos)       /* 0x00010000 */
#define ADC_CONFIG_SELVI_HD_LS      ADC_CONFIG_SELVI_HD_LS_Msk                  /* SAR input type selection */

#define ADC_CONFIG_EN_TS_DPos     (18U)
#define ADC_CONFIG_EN_TS_Pos      ADC_CONFIG_EN_TS_DPos
#define ADC_CONFIG_EN_TS_Msk      (0x1UL << ADC_CONFIG_EN_TS_Pos)               /* 0x00040000 */
#define ADC_CONFIG_EN_TS          ADC_CONFIG_EN_TS_Msk                          /* Temperature sensor enable signal */

#define ADC_CONFIG_EN_BUF_DPos    (19U)
#define ADC_CONFIG_EN_BUF_Pos     ADC_CONFIG_EN_BUF_DPos
#define ADC_CONFIG_EN_BUF_Msk     (0x1UL << ADC_CONFIG_EN_BUF_Pos)              /* 0x00080000 */
#define ADC_CONFIG_EN_BUF         ADC_CONFIG_EN_BUF_Msk                         /* Detected external reference input buffer enable signal */

#define ADC_CONFIG_ALG_MEAN_DPos  (20U)
#define ADC_CONFIG_ALG_MEAN_Pos   ADC_CONFIG_ALG_MEAN_DPos
#define ADC_CONFIG_ALG_MEAN_Msk   (0x3UL << ADC_CONFIG_ALG_MEAN_Pos)            /* 0x00300000 */
#define ADC_CONFIG_ALG_MEAN       ADC_CONFIG_ALG_MEAN_Msk                       /* ALG_MEAN[1:0] bits (Temperature sampling points selection for mean algorithm)*/
#define ADC_CONFIG_ALG_MEAN_0     (0x1UL << ADC_CONFIG_ALG_MEAN_Pos)            /* 0x00100000 */
#define ADC_CONFIG_ALG_MEAN_1     (0x2UL << ADC_CONFIG_ALG_MEAN_Pos)            /* 0x00200000 */

#define ADC_CONFIG_ADJ_TD_OS_DPos (24U)
#define ADC_CONFIG_ADJ_TD_OS_Pos  ADC_CONFIG_ADJ_TD_OS_DPos
#define ADC_CONFIG_ADJ_TD_OS_Msk  (0xFUL << ADC_CONFIG_ADJ_TD_OS_Pos)           /* 0x0F000000 */
#define ADC_CONFIG_ADJ_TD_OS      ADC_CONFIG_ADJ_TD_OS_Msk                      /* ADJ_TD_OS[3:0] bits (Temperature sensor offset adjustment signal)*/
#define ADC_CONFIG_ADJ_TD_OS_0    (0x1UL << ADC_CONFIG_ADJ_TD_OS_Pos)           /* 0x01000000 */
#define ADC_CONFIG_ADJ_TD_OS_1    (0x2UL << ADC_CONFIG_ADJ_TD_OS_Pos)           /* 0x02000000 */
#define ADC_CONFIG_ADJ_TD_OS_2    (0x4UL << ADC_CONFIG_ADJ_TD_OS_Pos)           /* 0x04000000 */
#define ADC_CONFIG_ADJ_TD_OS_3    (0x8UL << ADC_CONFIG_ADJ_TD_OS_Pos)           /* 0x08000000 */

#define ADC_CONFIG_ADJ_TD_GA_DPos (28U)
#define ADC_CONFIG_ADJ_TD_GA_Pos  ADC_CONFIG_ADJ_TD_GA_DPos
#define ADC_CONFIG_ADJ_TD_GA_Msk  (0xFUL << ADC_CONFIG_ADJ_TD_GA_Pos)           /* 0xF0000000 */
#define ADC_CONFIG_ADJ_TD_GA      ADC_CONFIG_ADJ_TD_GA_Msk                      /* ADJ_TD_GA[3:0] bits (Temperature sensor gain adjustment signal)*/
#define ADC_CONFIG_ADJ_TD_GA_0    (0x1UL << ADC_CONFIG_ADJ_TD_GA_Pos)           /* 0x10000000 */
#define ADC_CONFIG_ADJ_TD_GA_1    (0x2UL << ADC_CONFIG_ADJ_TD_GA_Pos)           /* 0x20000000 */
#define ADC_CONFIG_ADJ_TD_GA_2    (0x4UL << ADC_CONFIG_ADJ_TD_GA_Pos)           /* 0x40000000 */
#define ADC_CONFIG_ADJ_TD_GA_3    (0x8UL << ADC_CONFIG_ADJ_TD_GA_Pos)           /* 0x80000000 */

/*******************  Bit definition for ADC_SQL register  ********************/
#define ADC_SQL_RSQL_DPos         (0U)
#define ADC_SQL_RSQL_Pos          ADC_SQL_RSQL_DPos
#define ADC_SQL_RSQL_Msk          (0xFUL << ADC_SQL_RSQL_Pos)                   /* 0x0000000F */
#define ADC_SQL_RSQL              ADC_SQL_RSQL_Msk                              /* RSQL[3:0] bits (Length of the regular group)*/
#define ADC_SQL_RSQL_0            (0x1UL << ADC_SQL_RSQL_Pos)                   /* 0x00000001 */
#define ADC_SQL_RSQL_1            (0x2UL << ADC_SQL_RSQL_Pos)                   /* 0x00000002 */
#define ADC_SQL_RSQL_2            (0x4UL << ADC_SQL_RSQL_Pos)                   /* 0x00000004 */
#define ADC_SQL_RSQL_3            (0x8UL << ADC_SQL_RSQL_Pos)                   /* 0x00000008 */

#define ADC_SQL_ISQL_DPos         (8U)
#define ADC_SQL_ISQL_Pos          ADC_SQL_ISQL_DPos
#define ADC_SQL_ISQL_Msk          (0x3UL << ADC_SQL_ISQL_Pos)                   /* 0x00000300 */
#define ADC_SQL_ISQL              ADC_SQL_ISQL_Msk                              /* ISQL[1:0] bits (Length of the injection group)*/
#define ADC_SQL_ISQL_0            (0x1UL << ADC_SQL_ISQL_Pos)                   /* 0x00000100 */
#define ADC_SQL_ISQL_1            (0x2UL << ADC_SQL_ISQL_Pos)                   /* 0x00000200 */

/*******************  Bit definition for ADC_IOFR register  *******************/
#define ADC_IOFR_IOFR_DPos        (0U)
#define ADC_IOFR_IOFR_Pos         ADC_IOFR_IOFR_DPos
#define ADC_IOFR_IOFR_Msk         (0xFFFUL << ADC_IOFR_IOFR_Pos)                /* 0x00000FFF */
#define ADC_IOFR_IOFR             ADC_IOFR_IOFR_Msk                             /* IOFR[11:0] bits (Injection group offset value)*/
#define ADC_IOFR_IOFR_0           (0x1UL << ADC_IOFR_IOFR_Pos)                  /* 0x00000001 */
#define ADC_IOFR_IOFR_1           (0x2UL << ADC_IOFR_IOFR_Pos)                  /* 0x00000002 */
#define ADC_IOFR_IOFR_2           (0x4UL << ADC_IOFR_IOFR_Pos)                  /* 0x00000004 */
#define ADC_IOFR_IOFR_3           (0x8UL << ADC_IOFR_IOFR_Pos)                  /* 0x00000008 */
#define ADC_IOFR_IOFR_4           (0x10UL << ADC_IOFR_IOFR_Pos)                 /* 0x00000010 */
#define ADC_IOFR_IOFR_5           (0x20UL << ADC_IOFR_IOFR_Pos)                 /* 0x00000020 */
#define ADC_IOFR_IOFR_6           (0x40UL << ADC_IOFR_IOFR_Pos)                 /* 0x00000040 */
#define ADC_IOFR_IOFR_7           (0x80UL << ADC_IOFR_IOFR_Pos)                 /* 0x00000080 */
#define ADC_IOFR_IOFR_8           (0x100UL << ADC_IOFR_IOFR_Pos)                /* 0x00000100 */
#define ADC_IOFR_IOFR_9           (0x200UL << ADC_IOFR_IOFR_Pos)                /* 0x00000200 */
#define ADC_IOFR_IOFR_10          (0x400UL << ADC_IOFR_IOFR_Pos)                /* 0x00000400 */
#define ADC_IOFR_IOFR_11          (0x800UL << ADC_IOFR_IOFR_Pos)                /* 0x00000800 */

/*******************  Bit definition for ADC_RSQR register  *******************/
#define ADC_RSQR_SEQ_DPos         (0U)
#define ADC_RSQR_SEQ_Pos          ADC_RSQR_SEQ_DPos
#define ADC_RSQR_SEQ_Msk          (0x1FUL << ADC_RSQR_SEQ_Pos)                  /* 0x0000001F */
#define ADC_RSQR_SEQ              ADC_RSQR_SEQ_Msk                              /* SEQ[4:0] bits (Channel selection for regular group)*/
#define ADC_RSQR_SEQ_0            (0x1UL << ADC_RSQR_SEQ_Pos)                   /* 0x00000001 */
#define ADC_RSQR_SEQ_1            (0x2UL << ADC_RSQR_SEQ_Pos)                   /* 0x00000002 */
#define ADC_RSQR_SEQ_2            (0x4UL << ADC_RSQR_SEQ_Pos)                   /* 0x00000004 */
#define ADC_RSQR_SEQ_3            (0x8UL << ADC_RSQR_SEQ_Pos)                   /* 0x00000008 */
#define ADC_RSQR_SEQ_4            (0x10UL << ADC_RSQR_SEQ_Pos)                  /* 0x00000010 */

#define ADC_RSQR_IE_DPos          (6U)
#define ADC_RSQR_IE_Pos           ADC_RSQR_IE_DPos
#define ADC_RSQR_IE_Msk           (0x1UL << ADC_RSQR_IE_Pos)                    /* 0x00000040 */
#define ADC_RSQR_IE               ADC_RSQR_IE_Msk                               /* EOC interrupt enable */

#define ADC_RSQR_EOC_DPos         (7U)
#define ADC_RSQR_EOC_Pos          ADC_RSQR_EOC_DPos
#define ADC_RSQR_EOC_Msk          (0x1UL << ADC_RSQR_EOC_Pos)                   /* 0x00000080 */
#define ADC_RSQR_EOC              ADC_RSQR_EOC_Msk                              /* Indicate Regular Sequence conversion end */

/*******************  Bit definition for ADC_ISQR register  *******************/
#define ADC_ISQR_SEQ_DPos         (0U)
#define ADC_ISQR_SEQ_Pos          ADC_ISQR_SEQ_DPos
#define ADC_ISQR_SEQ_Msk          (0x1FUL << ADC_ISQR_SEQ_Pos)                  /* 0x0000001F */
#define ADC_ISQR_SEQ              ADC_ISQR_SEQ_Msk                              /* SEQ[4:0] bits (Channel selection for injection group)*/
#define ADC_ISQR_SEQ_0            (0x1UL << ADC_ISQR_SEQ_Pos)                   /* 0x00000001 */
#define ADC_ISQR_SEQ_1            (0x2UL << ADC_ISQR_SEQ_Pos)                   /* 0x00000002 */
#define ADC_ISQR_SEQ_2            (0x4UL << ADC_ISQR_SEQ_Pos)                   /* 0x00000004 */
#define ADC_ISQR_SEQ_3            (0x8UL << ADC_ISQR_SEQ_Pos)                   /* 0x00000008 */
#define ADC_ISQR_SEQ_4            (0x10UL << ADC_ISQR_SEQ_Pos)                  /* 0x00000010 */

#define ADC_ISQR_IE_DPos          (6U)
#define ADC_ISQR_IE_Pos           ADC_ISQR_IE_DPos
#define ADC_ISQR_IE_Msk           (0x1UL << ADC_ISQR_IE_Pos)                    /* 0x00000040 */
#define ADC_ISQR_IE               ADC_ISQR_IE_Msk                               /* EOC interrupt enable */

#define ADC_ISQR_EOC_DPos         (7U)
#define ADC_ISQR_EOC_Pos          ADC_ISQR_EOC_DPos
#define ADC_ISQR_EOC_Msk          (0x1UL << ADC_ISQR_EOC_Pos)                   /* 0x00000080 */
#define ADC_ISQR_EOC              ADC_ISQR_EOC_Msk                              /* Indicate Regular Sequence conversion end */

/*******************  Bit definition for ADC_DRD register  ********************/
#define ADC_DRD_RDR_DPos          (0U)
#define ADC_DRD_RDR_Pos           ADC_DRD_RDR_DPos
#define ADC_DRD_RDR_Msk           (0xFFFFUL << ADC_DRD_RDR_Pos)                 /* 0x0000FFFF */
#define ADC_DRD_RDR               ADC_DRD_RDR_Msk                               /* RDR[15:0] bits (Data register for regular group)*/
#define ADC_DRD_RDR_0             (0x1UL << ADC_DRD_RDR_Pos)                    /* 0x00000001 */
#define ADC_DRD_RDR_1             (0x2UL << ADC_DRD_RDR_Pos)                    /* 0x00000002 */
#define ADC_DRD_RDR_2             (0x4UL << ADC_DRD_RDR_Pos)                    /* 0x00000004 */
#define ADC_DRD_RDR_3             (0x8UL << ADC_DRD_RDR_Pos)                    /* 0x00000008 */
#define ADC_DRD_RDR_4             (0x10UL << ADC_DRD_RDR_Pos)                   /* 0x00000010 */
#define ADC_DRD_RDR_5             (0x20UL << ADC_DRD_RDR_Pos)                   /* 0x00000020 */
#define ADC_DRD_RDR_6             (0x40UL << ADC_DRD_RDR_Pos)                   /* 0x00000040 */
#define ADC_DRD_RDR_7             (0x80UL << ADC_DRD_RDR_Pos)                   /* 0x00000080 */
#define ADC_DRD_RDR_8             (0x100UL << ADC_DRD_RDR_Pos)                  /* 0x00000100 */
#define ADC_DRD_RDR_9             (0x200UL << ADC_DRD_RDR_Pos)                  /* 0x00000200 */
#define ADC_DRD_RDR_10            (0x400UL << ADC_DRD_RDR_Pos)                  /* 0x00000400 */
#define ADC_DRD_RDR_11            (0x800UL << ADC_DRD_RDR_Pos)                  /* 0x00000800 */
#define ADC_DRD_RDR_12            (0x1000UL << ADC_DRD_RDR_Pos)                 /* 0x00001000 */
#define ADC_DRD_RDR_13            (0x2000UL << ADC_DRD_RDR_Pos)                 /* 0x00002000 */
#define ADC_DRD_RDR_14            (0x4000UL << ADC_DRD_RDR_Pos)                 /* 0x00004000 */
#define ADC_DRD_RDR_15            (0x8000UL << ADC_DRD_RDR_Pos)                 /* 0x00008000 */

#define ADC_DRD_CH_DPos           (16U)
#define ADC_DRD_CH_Pos            ADC_DRD_CH_DPos
#define ADC_DRD_CH_Msk            (0x1FUL << ADC_DRD_CH_Pos)                    /* 0x001F0000 */
#define ADC_DRD_CH                ADC_DRD_CH_Msk                                /* CH[4:0] bits (Channel selection for regular group)*/
#define ADC_DRD_CH_0              (0x1UL << ADC_DRD_CH_Pos)                     /* 0x00000001 */
#define ADC_DRD_CH_1              (0x2UL << ADC_DRD_CH_Pos)                     /* 0x00000002 */
#define ADC_DRD_CH_2              (0x4UL << ADC_DRD_CH_Pos)                     /* 0x00000004 */
#define ADC_DRD_CH_3              (0x8UL << ADC_DRD_CH_Pos)                     /* 0x00000008 */
#define ADC_DRD_CH_4              (0x10UL << ADC_DRD_CH_Pos)                    /* 0x00000010 */

#define ADC_DRD_PARITY_DPos       (22U)
#define ADC_DRD_PARITY_Pos        ADC_DRD_PARITY_DPos
#define ADC_DRD_PARITY_Msk        (0x1UL << ADC_DRD_PARITY_Pos)                 /* 0x00400000 */
#define ADC_DRD_PARITY            ADC_DRD_PARITY_Msk                            /* Parity for regular conversion data */

/*******************  Bit definition for ADC_IRD register  ********************/
#define ADC_IRD_IDR_DPos          (0U)
#define ADC_IRD_IDR_Pos           ADC_IRD_IDR_DPos
#define ADC_IRD_IDR_Msk           (0xFFFFUL << ADC_IRD_IDR_Pos)                 /* 0x0000FFFF */
#define ADC_IRD_IDR               ADC_DRD_RDR_Msk                               /* IDR[15:0] bits (Data register for injection group)*/
#define ADC_IRD_IDR_0             (0x1UL << ADC_IRD_IDR_Pos)                    /* 0x00000001 */
#define ADC_IRD_IDR_1             (0x2UL << ADC_IRD_IDR_Pos)                    /* 0x00000002 */
#define ADC_IRD_IDR_2             (0x4UL << ADC_IRD_IDR_Pos)                    /* 0x00000004 */
#define ADC_IRD_IDR_3             (0x8UL << ADC_IRD_IDR_Pos)                    /* 0x00000008 */
#define ADC_IRD_IDR_4             (0x10UL << ADC_IRD_IDR_Pos)                   /* 0x00000010 */
#define ADC_IRD_IDR_5             (0x20UL << ADC_IRD_IDR_Pos)                   /* 0x00000020 */
#define ADC_IRD_IDR_6             (0x40UL << ADC_IRD_IDR_Pos)                   /* 0x00000040 */
#define ADC_IRD_IDR_7             (0x80UL << ADC_IRD_IDR_Pos)                   /* 0x00000080 */
#define ADC_IRD_IDR_8             (0x100UL << ADC_IRD_IDR_Pos)                  /* 0x00000100 */
#define ADC_IRD_IDR_9             (0x200UL << ADC_IRD_IDR_Pos)                  /* 0x00000200 */
#define ADC_IRD_IDR_10            (0x400UL << ADC_IRD_IDR_Pos)                  /* 0x00000400 */
#define ADC_IRD_IDR_11            (0x800UL << ADC_IRD_IDR_Pos)                  /* 0x00000800 */
#define ADC_IRD_IDR_12            (0x1000UL << ADC_IRD_IDR_Pos)                 /* 0x00001000 */
#define ADC_IRD_IDR_13            (0x2000UL << ADC_IRD_IDR_Pos)                 /* 0x00002000 */
#define ADC_IRD_IDR_14            (0x4000UL << ADC_IRD_IDR_Pos)                 /* 0x00004000 */
#define ADC_IRD_IDR_15            (0x8000UL << ADC_IRD_IDR_Pos)                 /* 0x00008000 */

#define ADC_IRD_CH_DPos           (16U)
#define ADC_IRD_CH_Pos            ADC_IRD_CH_DPos
#define ADC_IRD_CH_Msk            (0x1FUL << ADC_IRD_CH_Pos)                    /* 0x001F0000 */
#define ADC_IRD_CH                ADC_IRD_CH_Msk                                /* CH[4:0] bits (Channel selection for injection group)*/
#define ADC_IRD_CH_0              (0x1UL << ADC_IRD_CH_Pos)                     /* 0x00000001 */
#define ADC_IRD_CH_1              (0x2UL << ADC_IRD_CH_Pos)                     /* 0x00000002 */
#define ADC_IRD_CH_2              (0x4UL << ADC_IRD_CH_Pos)                     /* 0x00000004 */
#define ADC_IRD_CH_3              (0x8UL << ADC_IRD_CH_Pos)                     /* 0x00000008 */
#define ADC_IRD_CH_4              (0x10UL << ADC_IRD_CH_Pos)                    /* 0x00000010 */

#define ADC_IRD_PARITY_DPos       (22U)
#define ADC_IRD_PARITY_Pos        ADC_IRD_PARITY_DPos
#define ADC_IRD_PARITY_Msk        (0x1UL << ADC_IRD_PARITY_Pos)                 /* 0x00400000 */
#define ADC_IRD_PARITY            ADC_IRD_PARITY_Msk                            /* Parity for injection conversion data */

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
