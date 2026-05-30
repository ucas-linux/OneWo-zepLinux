/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_qspi_regs.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2024/08/20
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/08/20     1.0     build this module
 * Description:     This file contains all the register correlation defines for the QSPI firmware library.
 */

#ifndef AS32X601_QSPI_REG_H
#define AS32X601_QSPI_REG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601.h"
#include "as32x601_addrmap.h"

/*
 * @brief QSPI registers structure
 */
typedef struct
{
    __IO uint32_t CR;       /* QSPI_CR      QSPI control register,						Address offset: 0x00 */
    __IO uint32_t CCR;      /* QSPI_CCR     QSPI communication configuration register,  Address offset: 0x04 */
    __I  uint32_t SR;       /* QSPI_SR      QSPI status register,						Address offset: 0x08 */
    __O  uint32_t FCR;      /* QSPI_FCR     QSPI flag clear register,					Address offset: 0x0C */
    __IO uint32_t AR;       /* QSPI_AR      QSPI address register,						Address offset: 0x10 */
    __IO uint32_t ABR;      /* QSPI_ABR     QSPI alternate byte register,				Address offset: 0x14 */
    __IO uint32_t DR;       /* QSPI_DR      QSPI data register,							Address offset: 0x18 */
    __IO uint32_t DLR;      /* QSPI_DLR     QSPI data length register,					Address offset: 0x1C */
    __IO uint32_t TOR;      /* QSPI_TOR     QSPI timeout register,						Address offset: 0x20 */
    __IO uint32_t PIR;      /* QSPI_PIR     QSPI polling interval register,				Address offset: 0x24 */
    __IO uint32_t PSMAR;    /* QSPI_PSMAR   QSPI polling status matching register,    	Address offset: 0x28 */
    __IO uint32_t PSMKR;    /* QSPI_PSMKR   QSPI polling status mask register,			Address offset: 0x2C */
} QSPI_TypeDef;

/*
 * @addtogroup GPIO Peripheral declaration
 */
#define QSPI    ((QSPI_TypeDef *)QSPI_CTRL_BASE)

/******************  Bits definition for QSPI_CR register  *****************/
#define QSPI_CR_EN_DPos                 (0U)
#define QSPI_CR_EN_Pos                  QSPI_CR_EN_DPos
#define QSPI_CR_EN_Msk                  (0x1UL << QSPI_CR_EN_Pos)               /* 0x00000001 */
#define QSPI_CR_EN                      QSPI_CR_EN_Msk                          /* Enable QPSI */

#define QSPI_CR_ABORT_DPos              (1U)
#define QSPI_CR_ABORT_Pos               QSPI_CR_ABORT_DPos
#define QSPI_CR_ABORT_Msk               (0x1UL << QSPI_CR_ABORT_Pos)            /* 0x00000002 */
#define QSPI_CR_ABORT                   QSPI_CR_ABORT_Msk                       /* Abort request */

#define QSPI_CR_CKMODE_DPos             (2U)
#define QSPI_CR_CKMODE_Pos              QSPI_CR_CKMODE_DPos
#define QSPI_CR_CKMODE_Msk              (0x1UL << QSPI_CR_CKMODE_Pos)           /* 0x00000004 */
#define QSPI_CR_CKMODE                  QSPI_CR_CKMODE_Msk                      /* Mode 0/mode 3 */

#define QSPI_CR_CSHT_DPos               (3U)
#define QSPI_CR_CSHT_Pos                QSPI_CR_CSHT_DPos
#define QSPI_CR_CSHT_Msk                (0x7UL << QSPI_CR_CSHT_Pos)             /* 0x00000038 */
#define QSPI_CR_CSHT                    QSPI_CR_CSHT_Msk                        /* CSHT[2:0] bits (Chip select high time) */
#define QSPI_CR_CSHT_0                  (0x01UL << QSPI_CR_CSHT_Pos)            /* 0x00000008 */
#define QSPI_CR_CSHT_1                  (0x02UL << QSPI_CR_CSHT_Pos)            /* 0x00000010 */
#define QSPI_CR_CSHT_2                  (0x04UL << QSPI_CR_CSHT_Pos)            /* 0x00000020 */

#define QSPI_CR_PRESCALER_DPos          (8U)
#define QSPI_CR_PRESCALER_Pos           QSPI_CR_PRESCALER_DPos
#define QSPI_CR_PRESCALER_Msk           (0xFFUL << QSPI_CR_PRESCALER_Pos)       /* 0x0000FF00 */
#define QSPI_CR_PRESCALER               QSPI_CR_PRESCALER_Msk                   /* PRESCALER[7:0] bits (Clock prescaler) */
#define QSPI_CR_PRESCALER_0             (0x01UL << QSPI_CR_PRESCALER_Pos)       /* 0x00000100 */
#define QSPI_CR_PRESCALER_1             (0x02UL << QSPI_CR_PRESCALER_Pos)       /* 0x00000200 */
#define QSPI_CR_PRESCALER_2             (0x04UL << QSPI_CR_PRESCALER_Pos)       /* 0x00000400 */
#define QSPI_CR_PRESCALER_3             (0x08UL << QSPI_CR_PRESCALER_Pos)       /* 0x00000800 */
#define QSPI_CR_PRESCALER_4             (0x10UL << QSPI_CR_PRESCALER_Pos)       /* 0x00001000 */
#define QSPI_CR_PRESCALER_5             (0x20UL << QSPI_CR_PRESCALER_Pos)       /* 0x00002000 */
#define QSPI_CR_PRESCALER_6             (0x40UL << QSPI_CR_PRESCALER_Pos)       /* 0x00004000 */
#define QSPI_CR_PRESCALER_7             (0x80UL << QSPI_CR_PRESCALER_Pos)       /* 0x00008000 */

/******************  Bits definition for QSPI_CCR register  *****************/
#define QSPI_CCR_INST_DPos              (0U)
#define QSPI_CCR_INST_Pos               QSPI_CCR_INST_DPos
#define QSPI_CCR_INST_Msk               (0xFFUL << QSPI_CCR_INST_Pos)           /* 0x000000FF */
#define QSPI_CCR_INST                   QSPI_CCR_INST_Msk                       /* INST[7:0] bits (Instructions sent to the QSPI Flash) */
#define QSPI_CCR_INST_0                 (0x01UL << QSPI_CCR_INST_Pos)           /* 0x00000001 */
#define QSPI_CCR_INST_1                 (0x02UL << QSPI_CCR_INST_Pos)           /* 0x00000002 */
#define QSPI_CCR_INST_2                 (0x04UL << QSPI_CCR_INST_Pos)           /* 0x00000004 */
#define QSPI_CCR_INST_3                 (0x08UL << QSPI_CCR_INST_Pos)           /* 0x00000008 */
#define QSPI_CCR_INST_4                 (0x10UL << QSPI_CCR_INST_Pos)           /* 0x00000010 */
#define QSPI_CCR_INST_5                 (0x20UL << QSPI_CCR_INST_Pos)           /* 0x00000020 */
#define QSPI_CCR_INST_6                 (0x40UL << QSPI_CCR_INST_Pos)           /* 0x00000040 */
#define QSPI_CCR_INST_7                 (0x80UL << QSPI_CCR_INST_Pos)           /* 0x00000080 */

#define QSPI_CCR_IMODE_DPos             (8U)
#define QSPI_CCR_IMODE_Pos              QSPI_CCR_IMODE_DPos
#define QSPI_CCR_IMODE_Msk              (0x3UL << QSPI_CCR_IMODE_Pos)           /* 0x00000300 */
#define QSPI_CCR_IMODE                  QSPI_CCR_IMODE_Msk                      /* IMODE[1:0] bits (Instruction mode) */
#define QSPI_CCR_IMODE_0                (0x01UL << QSPI_CCR_IMODE_Pos)          /* 0x00000100 */
#define QSPI_CCR_IMODE_1                (0x02UL << QSPI_CCR_IMODE_Pos)          /* 0x00000200 */

#define QSPI_CCR_ADMODE_DPos            (10U)
#define QSPI_CCR_ADMODE_Pos             QSPI_CCR_ADMODE_DPos
#define QSPI_CCR_ADMODE_Msk             (0x3UL << QSPI_CCR_ADMODE_Pos)          /* 0x00000C00 */
#define QSPI_CCR_ADMODE                 QSPI_CCR_ADMODE_Msk                     /* ADMODE[1:0] bits (Address mode) */
#define QSPI_CCR_ADMODE_0               (0x01UL << QSPI_CCR_ADMODE_Pos)         /* 0x00000100 */
#define QSPI_CCR_ADMODE_1               (0x02UL << QSPI_CCR_ADMODE_Pos)         /* 0x00000200 */

#define QSPI_CCR_ADSIZE_DPos            (12U)
#define QSPI_CCR_ADSIZE_Pos             QSPI_CCR_ADSIZE_DPos
#define QSPI_CCR_ADSIZE_Msk             (0x3UL << QSPI_CCR_ADSIZE_Pos)          /* 0x00003000 */
#define QSPI_CCR_ADSIZE                 QSPI_CCR_ADSIZE_Msk                     /* ADSIZE [1:0] bits (Address size) */
#define QSPI_CCR_ADSIZE_0               (0x01UL << QSPI_CCR_ADSIZE_Pos)         /* 0x00001000 */
#define QSPI_CCR_ADSIZE_1               (0x02UL << QSPI_CCR_ADSIZE_Pos)         /* 0x00002000 */

#define QSPI_CCR_ABMODE_DPos            (14U)
#define QSPI_CCR_ABMODE_Pos             QSPI_CCR_ABMODE_DPos
#define QSPI_CCR_ABMODE_Msk             (0x3UL << QSPI_CCR_ABMODE_Pos)          /* 0x0000C000 */
#define QSPI_CCR_ABMODE                 QSPI_CCR_ABMODE_Msk                     /* ABMODE [1:0] bits (Alternate bytes mode) */
#define QSPI_CCR_ABMODE_0           	(0x01UL << QSPI_CCR_ABMODE_Pos)         /* 0x00004000 */
#define QSPI_CCR_ABMODE_1           	(0x02UL << QSPI_CCR_ABMODE_Pos)         /* 0x00008000 */

#define QSPI_CCR_ABSIZE_DPos            (16U)
#define QSPI_CCR_ABSIZE_Pos             QSPI_CCR_ABSIZE_DPos
#define QSPI_CCR_ABSIZE_Msk             (0x3UL << QSPI_CCR_ABSIZE_Pos)          /* 0x00030000 */
#define QSPI_CCR_ABSIZE                 QSPI_CCR_ABSIZE_Msk                     /* ABSIZE [1:0] bits (Alternate bytes size) */
#define QSPI_CCR_ABSIZE_0           	(0x01UL << QSPI_CCR_ABSIZE_Pos)         /* 0x00010000 */
#define QSPI_CCR_ABSIZE_1           	(0x02UL << QSPI_CCR_ABSIZE_Pos)         /* 0x00020000 */

#define QSPI_CCR_DUMMY_DPos             (18U)
#define QSPI_CCR_DUMMY_Pos              QSPI_CCR_DUMMY_DPos
#define QSPI_CCR_DUMMY_Msk              (0x1FUL << QSPI_CCR_DUMMY_Pos)          /* 0x007C0000 */
#define QSPI_CCR_DUMMY                  QSPI_CCR_DUMMY_Msk                      /* DUMMY [4:0] bits (Dummy cycles) */
#define QSPI_CCR_DUMMY_0                (0x01UL << QSPI_CCR_DUMMY_Pos)          /* 0x00040000 */
#define QSPI_CCR_DUMMY_1                (0x02UL << QSPI_CCR_DUMMY_Pos)          /* 0x00080000 */
#define QSPI_CCR_DUMMY_2                (0x04UL << QSPI_CCR_DUMMY_Pos)          /* 0x00100000 */
#define QSPI_CCR_DUMMY_3                (0x08UL << QSPI_CCR_DUMMY_Pos)          /* 0x00200000 */
#define QSPI_CCR_DUMMY_4                (0x10UL << QSPI_CCR_DUMMY_Pos)          /* 0x00400000 */

#define QSPI_CCR_DMODE_DPos             (23U)
#define QSPI_CCR_DMODE_Pos              QSPI_CCR_DMODE_DPos
#define QSPI_CCR_DMODE_Msk              (0x3UL << QSPI_CCR_DMODE_Pos)           /* 0x01800000 */
#define QSPI_CCR_DMODE                  QSPI_CCR_DMODE_Msk                      /* DMODE [1:0] bits (Data mode) */
#define QSPI_CCR_DMODE_0                (0x01UL << QSPI_CCR_DMODE_Pos)          /* 0x00800000 */
#define QSPI_CCR_DMODE_1                (0x02UL << QSPI_CCR_DMODE_Pos)          /* 0x01000000 */

#define QSPI_CCR_FMODE_DPos             (25U)
#define QSPI_CCR_FMODE_Pos              QSPI_CCR_FMODE_DPos
#define QSPI_CCR_FMODE_Msk              (0x3UL << QSPI_CCR_FMODE_Pos)           /* 0x06000000 */
#define QSPI_CCR_FMODE                  QSPI_CCR_FMODE_Msk                      /* FMODE [1:0] bits (Functional mode) */
#define QSPI_CCR_FMODE_0                (0x01UL << QSPI_CCR_FMODE_Pos)          /* 0x02000000 */
#define QSPI_CCR_FMODE_1                (0x02UL << QSPI_CCR_FMODE_Pos)          /* 0x04000000 */


/******************  Bits definition for QSPI_SR register  *****************/
#define QSPI_SR_BUSY_DPos               (0U)
#define QSPI_SR_BUSY_Pos                QSPI_SR_BUSY_DPos
#define QSPI_SR_BUSY_Msk                (0x1UL << QSPI_SR_BUSY_Pos)             /* 0x00000001 */
#define QSPI_SR_BUSY                    QSPI_SR_BUSY_Msk                        /* Busy */

#define QSPI_SR_FLEVEL_DPos             (1U)
#define QSPI_SR_FLEVEL_Pos              QSPI_SR_FLEVEL_DPos
#define QSPI_SR_FLEVEL_Msk              (0x3FUL << QSPI_SR_FLEVEL_Pos)          /* 0x0000007E */
#define QSPI_SR_FLEVEL                  QSPI_SR_FLEVEL_Msk                      /* FMODE [5:0] bits (FIFO level) */
#define QSPI_SR_FLEVEL_0                (0x01UL << QSPI_SR_FLEVEL_Pos)          /* 0x00000002 */
#define QSPI_SR_FLEVEL_1                (0x02UL << QSPI_SR_FLEVEL_Pos)          /* 0x00000004 */
#define QSPI_SR_FLEVEL_2                (0x04UL << QSPI_SR_FLEVEL_Pos)          /* 0x00000008 */
#define QSPI_SR_FLEVEL_3                (0x08UL << QSPI_SR_FLEVEL_Pos)          /* 0x00000010 */
#define QSPI_SR_FLEVEL_4                (0x10UL << QSPI_SR_FLEVEL_Pos)          /* 0x00000020 */
#define QSPI_SR_FLEVEL_5                (0x20UL << QSPI_SR_FLEVEL_Pos)          /* 0x00000040 */

#define QSPI_SR_TCF_DPos                (7U)
#define QSPI_SR_TCF_Pos                 QSPI_SR_TCF_DPos
#define QSPI_SR_TCF_Msk                 (0x1UL << QSPI_SR_TCF_Pos)              /* 0x00000080 */
#define QSPI_SR_TCF                     QSPI_SR_TCF_Msk                         /* Transfer complete flag */

#define QSPI_SR_TOF_DPos                (8U)
#define QSPI_SR_TOF_Pos                 QSPI_SR_TOF_DPos
#define QSPI_SR_TOF_Msk                 (0x1UL << QSPI_SR_TOF_Pos)              /* 0x00000100 */
#define QSPI_SR_TOF                     QSPI_SR_TOF_Msk                         /* Timeout flag */

#define QSPI_SR_PMF_DPos                (9U)
#define QSPI_SR_PMF_Pos                 QSPI_SR_PMF_DPos
#define QSPI_SR_PMF_Msk                 (0x1UL << QSPI_SR_PMF_Pos)              /* 0x00000200 */
#define QSPI_SR_PMF                     QSPI_SR_PMF_Msk                         /* Polling match flag */


/******************  Bits definition for QSPI_FCR register  *****************/
#define QSPI_FCR_CTCF_DPos              (0U)
#define QSPI_FCR_CTCF_Pos               QSPI_FCR_CTCF_DPos
#define QSPI_FCR_CTCF_Msk               (0x1UL << QSPI_FCR_CTCF_Pos)            /* 0x00000001 */
#define QSPI_FCR_CTCF                   QSPI_FCR_CTCF_Msk                       /* Clear transfer complete flag */

#define QSPI_FCR_CTOF_DPos              (1U)
#define QSPI_FCR_CTOF_Pos               QSPI_FCR_CTOF_DPos
#define QSPI_FCR_CTOF_Msk               (0x1UL << QSPI_FCR_CTOF_Pos)            /* 0x00000002 */
#define QSPI_FCR_CTOF                   QSPI_FCR_CTOF_Msk                       /* Clear timeout flag */

#define QSPI_FCR_CPMF_DPos              (2U)
#define QSPI_FCR_CPMF_Pos               QSPI_FCR_CPMF_DPos
#define QSPI_FCR_CPMF_Msk               (0x1UL << QSPI_FCR_CPMF_Pos)            /* 0x00000004 */
#define QSPI_FCR_CPMF                   QSPI_FCR_CPMF_Msk                       /* Clear polling match flag */


/******************  Bits definition for QSPI_AR register  *****************/
#define QSPI_AR_ADDRESS_DPos            (0U)
#define QSPI_AR_ADDRESS_Pos             QSPI_AR_ADDRESS_DPos
#define QSPI_AR_ADDRESS_Msk             (0xFFFFFFFFUL << QSPI_AR_ADDRESS_Pos)   /* 0xFFFFFFFF */
#define QSPI_AR_ADDRESS                 QSPI_AR_ADDRESS_Msk                     /* The address sent to the QSPI-Flash */

/******************  Bits definition for QSPI_ABR register  *****************/
#define QSPI_ABR_ALTERNATE_DPos         (0U)
#define QSPI_ABR_ALTERNATE_Pos          QSPI_ABR_ALTERNATE_DPos
#define QSPI_ABR_ALTERNATE_Msk          (0xFFFFFFFFUL << QSPI_ABR_ALTERNATE_Pos)    /* 0xFFFFFFFF */
#define QSPI_ABR_ALTERNATE              QSPI_ABR_ALTERNATE_Msk                      /* The alternate bytes for the QSPI-Flash */

/******************  Bits definition for QSPI_DR register  *****************/
#define QSPI_DR_DATA_DPos               (0U)
#define QSPI_DR_DATA_Pos                QSPI_DR_DATA_DPos
#define QSPI_DR_DATA_Msk                (0xFFFFFFFFUL << QSPI_DR_DATA_Pos)      /* 0xFFFFFFFF */
#define QSPI_DR_DATA                    QSPI_DR_DATA_Msk                        /* Alternate bytes */

/******************  Bits definition for QSPI_DLR register  *****************/
#define QSPI_DLR_DATALENGTH_DPos        (0U)
#define QSPI_DLR_DATALENGTH_Pos         QSPI_DLR_DATALENGTH_DPos
#define QSPI_DLR_DATALENGTH_Msk         (0xFFFFFFFFUL << QSPI_DLR_DATALENGTH_Pos)   /* 0xFFFFFFFF */
#define QSPI_DLR_DATALENGTH             QSPI_DLR_DATALENGTH_Msk                     /* Communication data length */

/******************  Bits definition for QSPI_TOR register  *****************/
#define QSPI_TOR_TIMEOUT_DPos           (0U)
#define QSPI_TOR_TIMEOUT_Pos            QSPI_TOR_TIMEOUT_DPos
#define QSPI_TOR_TIMEOUT_Msk            (0xFFFFUL << QSPI_TOR_TIMEOUT_Pos)      /* 0x0000FFFF */
#define QSPI_TOR_TIMEOUT                QSPI_TOR_TIMEOUT_Msk                    /* Timeout duration */

/******************  Bits definition for QSPI_PIR register  *****************/
#define QSPI_PIR_INTERVAL_DPos          (0U)
#define QSPI_PIR_INTERVAL_Pos           QSPI_PIR_INTERVAL_DPos
#define QSPI_PIR_INTERVAL_Msk           (0xFFFFUL << QSPI_PIR_INTERVAL_Pos)     /* 0x0000FFFF */
#define QSPI_PIR_INTERVAL               QSPI_PIR_INTERVAL_Msk                   /* Polling interval */

/******************  Bits definition for QSPI_PSMAR register  *****************/
#define QSPI_PSMAR_MATCH_DPos           (0U)
#define QSPI_PSMAR_MATCH_Pos            QSPI_PSMAR_MATCH_DPos
#define QSPI_PSMAR_MATCH_Msk            (0xFFFFFFFFUL << QSPI_PSMAR_MATCH_Pos)  /* 0xFFFFFFFF */
#define QSPI_PSMAR_MATCH                QSPI_PSMAR_MATCH_Msk                    /* Polling status matching */

/******************  Bits definition for QSPI_PSMKR register  *****************/
#define QSPI_PSMKR_MASK_DPos            (0U)
#define QSPI_PSMKR_MASK_Pos             QSPI_PSMKR_MASK_DPos
#define QSPI_PSMKR_MASK_Msk             (0xFFFFFFFFUL << QSPI_PSMKR_MASK_Pos)   /* 0xFFFFFFFF */
#define QSPI_PSMKR_MASK                 QSPI_PSMKR_MASK_Msk                     /* Polling status masking */

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
