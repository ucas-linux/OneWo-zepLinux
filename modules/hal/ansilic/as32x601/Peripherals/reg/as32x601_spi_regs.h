/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_spi_regs.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2023/12/24
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2023/11/21     1.0     build this module
 * Description:     This file contains all the register correlation defines for the SPI firmware library.
 */

#ifndef AS32X601_SPI_REG_H
#define AS32X601_SPI_REG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601_addrmap.h"
#include "as32x601.h"

/*
 * @brief SPI registers structure
 */
typedef struct
{
    __IO uint32_t CR0;          /* SPI_Ctrl0        Control register 0,                     Address offset: 0x00 */
    __O  uint32_t IFCR;         /* SPI_IntClear     Interrupt clear register,               Address offset: 0x04 */
    __I  uint32_t RXDR;         /* SPI_Rxdata       Receive data register,                  Address offset: 0x08 */
    __O  uint32_t TXDR;         /* SPI_Txdata       Transmit data register,                 Address offset: 0x0C */
    __I  uint32_t ISR;          /* SPI_IntSr        Interrupt status register,              Address offset: 0x10 */
    __I  uint32_t IRSR;         /* SPI_IntRaw       Interrupt the raw status register,      Address offset: 0x14 */
    __IO uint32_t IER;          /* SPI_Ctrl1        Control register 1,                     Address offset: 0x18 */
    __O  uint32_t CR1;          /* SPI_Command      Buffer reset control register,          Address offset: 0x1C */
    __I  uint32_t SR;           /* SPI_Status       Status register,                        Address offset: 0x20 */
    __O  uint32_t SSR;          /* SPI_SSel         Slave select register,                  Address offset: 0x24 */
    __O  uint32_t TXEDR;        /* SPI_TxdataLast   End of send data register,         	    Address offset: 0x28 */
    __IO uint32_t CLKCR;        /* SPI_CfgClk       Clock division configuration register,  Address offset: 0x2C */
    __IO uint32_t CFG;          /* SPI_Mode         Mode register,                		    Address offset: 0x30 */
} SPI_TypeDef;

/*
 * @addtogroup SPI peripheral declaration
 */
#define SPI0    ((SPI_TypeDef *)SPI0_BASE)
#define SPI1    ((SPI_TypeDef *)SPI1_BASE)
#define SPI2    ((SPI_TypeDef *)SPI2_BASE)
#define SPI3    ((SPI_TypeDef *)SPI3_BASE)
#define SPI4    ((SPI_TypeDef *)SPI4_BASE)
#define SPI5    ((SPI_TypeDef *)SPI5_BASE)

/******************  Bits definition for SPI_CR0 register  *****************/
#define SPI_CR0_ENABLE_DPos         (0U)
#define SPI_CR0_SPE_Pos             SPI_CR0_ENABLE_DPos
#define SPI_CR0_SPE_Msk             (0x01UL << SPI_CR0_SPE_Pos)             /* 0x00000001 */
#define SPI_CR0_SPE                 SPI_CR0_SPE_Msk                         /* Enable SPI */

#define SPI_CR0_MASTER_DPos         (1U)
#define SPI_CR0_MSTR_Pos            SPI_CR0_MASTER_DPos
#define SPI_CR0_MSTR_Msk            (0x01UL << SPI_CR0_MSTR_Pos)            /* 0x00000002 */
#define SPI_CR0_MSTR                SPI_CR0_MSTR_Msk                        /* Select the master/slave mode */

#define SPI_CR0_INTTXDONE_DPos      (3U)
#define SPI_CR0_TDIE_Pos            SPI_CR0_INTTXDONE_DPos
#define SPI_CR0_TDIE_Msk            (0x01UL << SPI_CR0_TDIE_Pos)            /* 0x00000004 */
#define SPI_CR0_TDIE                SPI_CR0_TDIE_Msk                        /* Tx done interrup enable */

#define SPI_CR0_INTRXOVFLOW_DPos    (4U)
#define SPI_CR0_RXOIE_Pos           SPI_CR0_INTRXOVFLOW_DPos
#define SPI_CR0_RXOIE_Msk           (0x01UL << SPI_CR0_RXOIE_Pos)           /* 0x00000008 */
#define SPI_CR0_RXOIE               SPI_CR0_RXOIE_Msk                       /* Rx buffer overflow interrupt enable */

#define SPI_CR0_INTTXURUN_DPos      (5U)
#define SPI_CR0_TXEIE_Pos           SPI_CR0_INTTXURUN_DPos
#define SPI_CR0_TXEIE_Msk           (0x01UL << SPI_CR0_TXEIE_Pos)           /* 0x00000010 */
#define SPI_CR0_TXEIE               SPI_CR0_TXEIE_Msk                       /* Tx buffer empty interrupt enable */


/******************  Bits definition for SPI_IFCR register  *****************/          
#define SPI_IFCR_TXDONE_DPos        (0U)
#define SPI_IFCR_TDC_Pos            SPI_IFCR_TXDONE_DPos
#define SPI_IFCR_TDC_Msk            (0x01UL << SPI_IFCR_TDC_Pos)            /* 0x00000001 */
#define SPI_IFCR_TDC                SPI_IFCR_TDC_Msk                        /* Tx done interrup clear */

#define SPI_IFCR_RXOVERFLOW_DPos    (2U)
#define SPI_IFCR_RXOC_Pos           SPI_IFCR_RXOVERFLOW_DPos
#define SPI_IFCR_RXOC_Msk           (0x01UL << SPI_IFCR_RXOC_Pos)           /* 0x00000004 */
#define SPI_IFCR_RXOC               SPI_IFCR_RXOC_Msk                       /* Rx buffer overflow interrupt clear */

#define SPI_IFCR_TXUNDERRUN_DPos    (3U)
#define SPI_IFCR_TXEC_Pos           SPI_IFCR_TXUNDERRUN_DPos
#define SPI_IFCR_TXEC_Msk           (0x01UL << SPI_IFCR_TXEC_Pos)           /* 0x00000008 */
#define SPI_IFCR_TXEC               SPI_IFCR_TXEC_Msk                       /* Tx buffer empty interrupt clear */

#define SPI_IFCR_SSLOW_DPos         (5U)
#define SPI_IFCR_NSSVC_Pos          SPI_IFCR_SSLOW_DPos
#define SPI_IFCR_NSSVC_Msk          (0x01UL << SPI_IFCR_NSSVC_Pos)          /* 0x00000020 */
#define SPI_IFCR_NSSVC              SPI_IFCR_NSSVC_Msk                      /* NSS Voh interrupt clear */

#define SPI_IFCR_RXFFNF_DPos        (6U)
#define SPI_IFCR_RXNEC_Pos          SPI_IFCR_RXFFNF_DPos
#define SPI_IFCR_RXNEC_Msk          (0x01UL << SPI_IFCR_RXNEC_Pos)          /* 0x00000040 */
#define SPI_IFCR_RXNEC              SPI_IFCR_RXNEC_Msk                      /* Rx buffer non-empty interrupt clear */

#define SPI_IFCR_TXFFNF_DPos        (7U)
#define SPI_IFCR_TXNFC_Pos          SPI_IFCR_TXFFNF_DPos
#define SPI_IFCR_TXNFC_Msk          (0x01UL << SPI_IFCR_TXNFC_Pos)          /* 0x00000080 */
#define SPI_IFCR_TXNFC              SPI_IFCR_TXNFC_Msk                      /* Tx buffer non-full interrupt clear */


/******************  Bits definition for SPI_RXDR register  *****************/            
#define SPI_RXDR_RXDATA_DPos        (0U)
#define SPI_RXDR_RXDR_Pos           SPI_RXDR_RXDATA_DPos
#define SPI_RXDR_RXDR_Msk           (0xFFFFFFFFUL << SPI_RXDR_RXDR_Pos)     /* 0xFFFFFFFF */
#define SPI_RXDR_RXDR               SPI_RXDR_RXDR_Msk                       /* RXDONE[31:0] bits (Receive data) */


/******************  Bits definition for SPI_TXDR register  *****************/            
#define SPI_TXDR_TXDATA_DPos        (0U)
#define SPI_TXDR_TXDR_Pos           SPI_TXDR_TXDATA_DPos
#define SPI_TXDR_TXDR_Msk           (0xFFFFFFFFUL << SPI_TXDR_TXDR_Pos)     /* 0xFFFFFFFF */
#define SPI_TXDR_TXDR               SPI_TXDR_TXDR_Msk                       /* TXDATA[31:0] bits (Transmit data) */


/******************  Bits definition for SPI_ISR register  *****************/            
#define SPI_ISR_TXDONE_DPos         (0U)
#define SPI_ISR_TDIS_Pos            SPI_ISR_TXDONE_DPos
#define SPI_ISR_TDIS_Msk            (0x01UL << SPI_ISR_TDIS_Pos)            /* 0x00000001 */
#define SPI_ISR_TDIS                SPI_ISR_TDIS_Msk                        /* The last frame has been sent */

#define SPI_ISR_RXOVERFLOW_DPos     (2U)
#define SPI_ISR_RXOIS_Pos           SPI_ISR_RXOVERFLOW_DPos
#define SPI_ISR_RXOIS_Msk           (0x01UL << SPI_ISR_RXOIS_Pos)           /* 0x00000004 */
#define SPI_ISR_RXOIS               SPI_ISR_RXOIS_Msk                       /* Receive buffer data overflow */

#define SPI_ISR_TXUNDERRUN_DPos     (3U)
#define SPI_ISR_TXEIS_Pos           SPI_ISR_TXUNDERRUN_DPos
#define SPI_ISR_TXEIS_Msk           (0x01UL << SPI_ISR_TXEIS_Pos)           /* 0x00000008 */
#define SPI_ISR_TXEIS               SPI_ISR_TXEIS_Msk                       /* Data in the send buffer is not available */

#define SPI_ISR_SSLOW_DPos          (5U)
#define SPI_ISR_NSSVIS_Pos          SPI_ISR_SSLOW_DPos
#define SPI_ISR_NSSVIS_Msk          (0x01UL << SPI_ISR_NSSVIS_Pos)          /* 0x00000020 */
#define SPI_ISR_NSSVIS              SPI_ISR_NSSVIS_Msk                      /* The chip selection signal is low interrupt */

#define SPI_ISR_RXFFNE_DPos         (6U)
#define SPI_ISR_RXNEIS_Pos          SPI_ISR_RXFFNE_DPos
#define SPI_ISR_RXNEIS_Msk          (0x01UL << SPI_ISR_RXNEIS_Pos)          /* 0x00000040 */
#define SPI_ISR_RXNEIS              SPI_ISR_RXNEIS_Msk                      /* Rx buffer non-empty interrupt */

#define SPI_ISR_TXFFNF_DPos         (7U)
#define SPI_ISR_TXNFIS_Pos          SPI_ISR_TXFFNF_DPos
#define SPI_ISR_TXNFIS_Msk          (0x01UL << SPI_ISR_TXNFIS_Pos)          /* 0x00000080 */
#define SPI_ISR_TXNFIS              SPI_ISR_TXNFIS_Msk                      /* Tx buffer non-full interrupt */


/******************  Bits definition for SPI_IRSR register  *****************/
#define SPI_IRSR_TXDONE_DPos        (0U)
#define SPI_IRSR_TDIRS_Pos          SPI_IRSR_TXDONE_DPos
#define SPI_IRSR_TDIRS_Msk          (0x01UL << SPI_IRSR_TDIRS_Pos)          /* 0x00000001 */
#define SPI_IRSR_TDIRS              SPI_IRSR_TDIRS_Msk                      /* Tx done interrupt raw status */

#define SPI_IRSR_RXOVERFLOW_DPos    (2U)
#define SPI_IRSR_RXOIRS_Pos         SPI_IRSR_RXOVERFLOW_DPos
#define SPI_IRSR_RXOIRS_Msk         (0x01UL << SPI_IRSR_RXOIRS_Pos)         /* 0x00000004 */
#define SPI_IRSR_RXOIRS             SPI_IRSR_RXOIRS_Msk                     /* Rx buffer overflow interrupt raw status */

#define SPI_IRSR_TXUNDERRUN_DPos    (3U)
#define SPI_IRSR_TXEIRS_Pos         SPI_IRSR_TXUNDERRUN_DPos
#define SPI_IRSR_TXEIRS_Msk         (0x01UL << SPI_IRSR_TXEIRS_Pos)         /* 0x00000008 */
#define SPI_IRSR_TXEIRS             SPI_IRSR_TXEIRS_Msk                     /* Tx buffer empty interrupt raw status */

#define SPI_IRSR_SSLOW_DPos         (5U)
#define SPI_IRSR_NSSVIRS_Pos        SPI_IRSR_SSLOW_DPos
#define SPI_IRSR_NSSVIRS_Msk        (0x01UL << SPI_IRSR_NSSVIRS_Pos)        /* 0x00000020 */
#define SPI_IRSR_NSSVIRS            SPI_IRSR_NSSVIRS_Msk                    /* NSS Voh interrupt raw status */

#define SPI_IRSR_RXFFNE_DPos       (6U)
#define SPI_IRSR_RXNEIRS_Pos        SPI_IRSR_RXFFNE_DPos
#define SPI_IRSR_RXNEIRS_Msk        (0x01UL << SPI_IRSR_RXNEIRS_Pos)        /* 0x00000040 */
#define SPI_IRSR_RXNEIRS            SPI_IRSR_RXNEIRS_Msk                    /* Rx buffer non-empty interrupt raw status */

#define SPI_IRSR_TXFFNF_DPos        (7U)
#define SPI_IRSR_TXNFIRS_Pos        SPI_IRSR_TXFFNF_DPos
#define SPI_IRSR_TXNFIRS_Msk        (0x01UL << SPI_IRSR_TXNFIRS_Pos)        /* 0x00000080 */
#define SPI_IRSR_TXNFIRS            SPI_IRSR_TXNFIRS_Msk                    /* Tx buffer non-full interrupt raw status */


/******************  Bits definition for SPI_IER register  *****************/         
#define SPI_IER_SSLOW_DPos          (5U)
#define SPI_IER_NSSVIE_Pos          SPI_IER_SSLOW_DPos
#define SPI_IER_NSSVIE_Msk          (0x01UL << SPI_IER_NSSVIE_Pos)          /* 0x00000020 */
#define SPI_IER_NSSVIE              SPI_IER_NSSVIE_Msk                      /* NSS Voh interrupt enable */

#define SPI_IER_RXFFNE_DPos         (6U)
#define SPI_IER_RXNEIE_Pos          SPI_IER_RXFFNE_DPos
#define SPI_IER_RXNEIE_Msk          (0x01UL << SPI_IER_RXNEIE_Pos)          /* 0x00000040 */
#define SPI_IER_RXNEIE              SPI_IER_RXNEIE_Msk                      /* Rx buffer non-empty interrupt enable */

#define SPI_IER_TXFFNF_DPos         (7U)
#define SPI_IER_TXNEIE_Pos          SPI_IER_TXFFNF_DPos
#define SPI_IER_TXNEIE_Msk          (0x01UL << SPI_IER_TXNEIE_Pos)          /* 0x00000080 */
#define SPI_IER_TXNEIE              SPI_IER_TXNEIE_Msk                      /* Tx buffer non-full interrupt enable */


/******************  Bits definition for SPI_CR1 register  *****************/           
#define SPI_CR1_RXFIFORST_DPos      (0U)
#define SPI_CR1_RXRE_Pos            SPI_CR1_RXFIFORST_DPos
#define SPI_CR1_RXRE_Msk            (0x01UL << SPI_CR1_RXRE_Pos)            /* 0x00000001 */
#define SPI_CR1_RXRE                SPI_CR1_RXRE_Msk                        /* Reset receive buffer */

#define SPI_CR1_TXFIFORST_DPos      (1U)
#define SPI_CR1_TXRE_Pos            SPI_CR1_TXFIFORST_DPos
#define SPI_CR1_TXRE_Msk            (0x01UL << SPI_CR1_TXRE_Pos)            /* 0x00000002 */
#define SPI_CR1_TXRE                SPI_CR1_TXRE_Msk                        /* Reset transmit buffer */


/******************  Bits definition for SPI_SR register  *****************/          
#define SPI_SR_FIRSTFRAME_DPos      (0U)
#define SPI_SR_NFRX_Pos             SPI_SR_FIRSTFRAME_DPos
#define SPI_SR_NFRX_Msk             (0x01UL << SPI_SR_NFRX_Pos)             /* 0x00000001 */
#define SPI_SR_NFRX                 SPI_SR_NFRX_Msk                         /* NSS is effective, the next frame in the receiving buffer is received for the first time */

#define SPI_SR_DONE_DPos            (1U)
#define SPI_SR_DONE_Pos             SPI_SR_DONE_DPos
#define SPI_SR_DONE_Msk             (0x01UL << SPI_SR_DONE_Pos)             /* 0x00000002 */
#define SPI_SR_DONE                 SPI_SR_DONE_Msk                         /* No request for receiving and sending */

#define SPI_SR_RXEMPTY_DPos         (2U)
#define SPI_SR_RXE_Pos              SPI_SR_RXEMPTY_DPos
#define SPI_SR_RXE_Msk              (0x01UL << SPI_SR_RXE_Pos)              /* 0x00000004 */
#define SPI_SR_RXE                  SPI_SR_RXE_Msk                          /* Rx buffer is empty */

#define SPI_SR_TXFULL_DPos          (3U)
#define SPI_SR_TXF_Pos              SPI_SR_TXFULL_DPos
#define SPI_SR_TXF_Msk              (0x01UL << SPI_STAT_TXFULL_Pos)         /* 0x00000008 */
#define SPI_SR_TXF                  SPI_STAT_TXFULL_Msk                     /* Tx buffer is full */

#define SPI_SR_RXOVFLOW_DPos        (4U)
#define SPI_SR_RXO_Pos              SPI_SR_RXOVFLOW_DPos
#define SPI_SR_RXO_Msk              (0x01UL << SPI_SR_RXO_Pos)              /* 0x00000010 */
#define SPI_SR_RXO                  SPI_SR_RXO_Msk                          /* Rx buffer is overflow */

#define SPI_SR_TXUNDERRUN_DPos      (5U)
#define SPI_SR_TXE_Pos              SPI_SR_TXUNDERRUN_DPos
#define SPI_SR_TXE_Msk              (0x01UL << SPI_SR_TXE_Pos)              /* 0x00000020 */
#define SPI_SR_TXE                  SPI_SR_TXE_Msk                          /* Tx buffer is empty */

#define SPI_SR_SSEL_DPos            (6U)
#define SPI_SR_NSSS_Pos             SPI_SR_SSEL_DPos
#define SPI_SR_NSSS_Msk             (0x01UL << SPI_SR_NSSS_Pos)             /* 0x00000040 */
#define SPI_SR_NSSS                 SPI_SR_NSSS_Msk                         /* The state of select slave */

#define SPI_SR_ACTIVE_DPos          (7U)
#define SPI_SR_BUSS_Pos             SPI_SR_ACTIVE_DPos
#define SPI_SR_BUSS_Msk             (0x01UL << SPI_SR_BUSS_Pos)             /* 0x00000080 */
#define SPI_SR_BUSS                 SPI_SR_BUSS_Msk                         /* SPI is still transmitting data */


/******************  Bits definition for SPI_SSR register  *****************/          
#define SPI_SSR_SSEL_DPos           (0U)
#define SPI_SSR_SSR_Pos             SPI_SSR_SSEL_DPos
#define SPI_SSR_SSR_Msk             (0xFFUL << SPI_SSR_SSR_Pos)             /* 0x000000FF */
#define SPI_SSR_SSR                 SPI_SSR_SSR_Msk                         /* SSEL[7:0] bits (Slave selection) */


/******************  Bits definition for SPI_TXEDR register  *****************/
#define SPI_TXEDR_TXDATA_LAST_DPos  (0U)
#define SPI_TXEDR_TXEDR_Pos         SPI_TXEDR_TXDATA_LAST_DPos
#define SPI_TXEDR_TXEDR_Msk         (0xFFFFFFFFUL << SPI_TXEDR_TXEDR_Pos)   /* 0xFFFFFFFF */
#define SPI_TXEDR_TXEDR             SPI_TXEDR_TXEDR_Msk                     /* TXDATA_LAST[31:0] bits (Transmitter last frame) */


/******************  Bits definition for SPI_CLKCR register  *****************/
#define SPI_CLKCR_CFG_CLK_DPos      (0U)
#define SPI_CLKCR_CLKCR_Pos         SPI_CLKCR_CFG_CLK_DPos
#define SPI_CLKCR_CLKCR_Msk         (0xFFUL << SPI_CLKCR_CLKCR_Pos)         /* 0x000000FF */
#define SPI_CLKCR_CLKCR             SPI_CLKCR_CLKCR_Msk                     /* CFG_CLK[7:0] bits (Frequency divider) */


/******************  Bits definition for SPI_CFG register  *****************/
#define SPI_CFG_MOT_MODE_DPos       (2U)
#define SPI_CFG_MOTMODESEL_Pos      SPI_CFG_MOT_MODE_DPos
#define SPI_CFG_MOTMODESEL_Msk      (0x03UL << SPI_CFG_MOTMODESEL_Pos)      /* 0x00000006 */
#define SPI_CFG_MOTMODESEL          SPI_CFG_MOTMODESEL_Msk                  /* MOT_MODE[1:0] bits (Motorola mode selection) */

#define SPI_CFG_SSEL_DPos           (4U)
#define SPI_CFG_TRANSMODE_Pos       SPI_CFG_SSEL_DPos
#define SPI_CFG_TRANSMODE_Msk       (0x01UL << SPI_CFG_TRANSMODE_Pos)       /* 0x00000010 */
#define SPI_CFG_TRANSMODE           SPI_CFG_TRANSMODE_Msk                   /* Transmission mode */

#define SPI_CFG_FRAME_SIZE_DPos     (10U)
#define SPI_CFG_FS_Pos              SPI_CFG_FRAME_SIZE_DPos
#define SPI_CFG_FS_Msk              (0x3FUL << SPI_CFG_FS_Pos)              /* 0x0000FC00 */
#define SPI_CFG_FS                  SPI_CFG_FS_Msk                          /* FRAME_SIZE[6:0] bits (SPI frame size) */


#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
