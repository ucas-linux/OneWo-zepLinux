/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_usart.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:			2024/08/01
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/08/01     1.0     build this module
 * Description:     This file contains all the functions prototypes for the USART firmware library.
 */

#ifndef AS32X601_USART_H
#define AS32X601_USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "as32x601_usart_regs.h"

/*
 * @brief  Use by parameter examine.
 */
#define IS_USART_ALL_PERIPH(PERIPH) (((PERIPH) == USART0) || ((PERIPH) == USART1) || \
                                     ((PERIPH) == USART2) || ((PERIPH) == USART3) || \
                                     ((PERIPH) == USART4) || ((PERIPH) == USART5) || \
                                     ((PERIPH) == USART6) || ((PERIPH) == USART7))

/*
 * @brief  USART Advanced Features initalization structure definition
 */
typedef struct
{
    uint32_t USART_AdvFeatureInit;      /* Specifies which advanced USART features is initialized. Several
                                        Advanced Features may be initialized at the same time. */
    uint32_t USART_TxPinLevelInvert;    /* Specifies whether the TX pin active level is inverted. */
    uint32_t USART_RxPinLevelInvert;    /* Specifies whether the RX pin active level is inverted. */
    uint32_t USART_DataInvert;          /* Specifies whether data are inverted (positive/direct logic
                                        vs negative/inverted logic). */
    uint32_t USART_OverrunDisable;      /* Specifies whether the reception overrun detection is disabled. */
    uint32_t USART_AutoBaudRateEnable;  /* Specifies whether auto Baud rate detection is enabled. */
    uint32_t USART_AutoBaudRateMode;    /* If auto Baud rate detection is enabled, specifies how the rate
                                        detection is carried out. */
    uint32_t USART_MSBFirst;            /* Specifies whether MSB is sent first on USART line. */
} USART_AdvFeatureInitTypeDef;

/*
 * @brief USART Init Structure definition
 */
typedef struct
{
    uint32_t USART_BaudRate;                        /* This member configures the USART communication baud rate. */
    uint16_t USART_WordLength;                      /* Specifies the number of data bits transmitted or received in a frame.  */
    uint16_t USART_StopBits;                        /* Specifies the number of stop bits transmitted. */
    uint16_t USART_Parity;                          /* Specifies the parity mode. */
    uint16_t USART_Mode;                            /* Specifies wether the Receive or Transmit mode is enabled or disabled. */
    uint16_t USART_HardwareFlowControl;             /* Specifies wether the hardware flow control mode is enabled or disabled. */
    uint32_t USART_OverSampling;                    /* Specifies whether the Over sampling 8 is enabled or disabled */
    USART_AdvFeatureInitTypeDef USART_AdvancedInit; /* USART Advanced Features initialization parameters */
} USART_InitTypeDef;


/* 
 * @brief  USART Clock Init Structure definition  
 */ 
typedef struct
{
    uint16_t USART_Clock;               /* Specifies whether the USART clock is enabled or disabled. */
    uint16_t USART_CPOL;                /* Specifies the steady state of the serial clock. */
    uint16_t USART_CPHA;                /* Specifies the clock transition on which the bit capture is made. */
    uint16_t USART_LastBit;             /* Specifies whether the clock pulse corresponding to the last transmitted
                                           data bit (MSB) has to be output on the SCLK pin in synchronous mode. */
} USART_ClockInitTypeDef;

/*
 * @defgroup USART function selection define
 */
/*
 * @defgroup USART Mode
 */
#define USART_Mode_Rx                       ((uint32_t)0x00000004)              /* RX mode */
#define USART_Mode_Tx                       ((uint32_t)0x00000008)              /* TX mode */
#define IS_USART_MODE(MODE) ((((MODE) & (uint32_t)0xFFFFFFF3) == 0x00) && ((MODE) != (uint32_t)0x00))

/*
 * @defgroup USART Parity
 */
#define USART_Parity_No                     ((uint32_t)0x00000000)              /* No parity */
#define USART_Parity_Even                   ((uint32_t)0x00000400)              /* Even parity */
#define USART_Parity_Odd                    ((uint32_t)0x00000600)              /* Odd parity */
#define IS_USART_PARITY(PARITY) (((PARITY) == USART_Parity_No)   || \
                                 ((PARITY) == USART_Parity_Even) || \
                                 ((PARITY) == USART_Parity_Odd))

/*
 * @defgroup USART Word Length
 */
#define USART_WordLength_6b                 ((uint32_t)0x10001000)              /* 6-bit long USART frame */
#define USART_WordLength_7b                 ((uint32_t)0x10000000)              /* 7-bit long USART frame */
#define USART_WordLength_8b                 ((uint32_t)0x00000000)              /* 8-bit long USART frame */
#define USART_WordLength_9b                 ((uint32_t)0x00001000)              /* 9-bit long USART frame */
#define IS_USART_WORD_LENGTH(LENGTH) (((LENGTH) == USART_WordLength_6b) || \
                                      ((LENGTH) == USART_WordLength_7b) || \
                                      ((LENGTH) == USART_WordLength_8b) || \
                                      ((LENGTH) == USART_WordLength_9b))

/*
 * @defgroup USART Over Sampling
 */
#define USART_OverSampling_8                ((uint32_t)0x00008000)              /* Oversampling by 8 */
#define USART_OverSampling_16               ((uint32_t)0x00000000)              /* Oversampling by 16 */
#define IS_USART_OVERSAMPLING(SAMPLING) (((SAMPLING) == USART_OverSampling_16) || \
                                         ((SAMPLING) == USART_OverSampling_8))

/*
 * @defgroup USART FIFO mode
 */
#define USART_FIFOMode_Disable              ((uint32_t)0x00000000)              /* FIFO mode disable */
#define USART_FIFOMode_Enable               ((uint32_t)0x20000000)              /* FIFO mode enable */
#define IS_USART_FIFOMODE_STATE(STATE) (((STATE) == USART_FIFOMode_Disable) || \
                                        ((STATE) == USART_FIFOMode_Enable))

/*
 * @defgroup USART Synchronous Slave mode enable
 */
#define USART_SlaveMode_Disable             ((uint32_t)0x00000000)              /* USART SPI Slave Mode Disable */
#define USART_SlaveMode_Enable              ((uint32_t)0x00000001)              /* USART SPI Slave Mode Enable */
#define IS_USART_SLAVEMODE_STATE(STATE) (((STATE) == USART_SlaveMode_Disable) || \
                                         ((STATE) == USART_SlaveMode_Enable))

/*
 * @defgroup USART LIN Break Detection
 */
#define USART_LINBreakDetectLength_11b      ((uint32_t)0x00000006)              /* LIN 11-bit break detection length */
#define USART_LINBreakDetectLength_12b      ((uint32_t)0x00000004)              /* LIN 12-bit break detection length */
#define USART_LINBreakDetectLength_13b      ((uint32_t)0x00000000)              /* LIN 13-bit break detection length */
#define USART_LINBreakDetectLength_14b      ((uint32_t)0x00000002)              /* LIN 14-bit break detection length */
#define IS_USART_LIN_BREAK_DETECT_LENGTH(LENGTH) (((LENGTH) == USART_LINBreakDetectLength_11b) || \
                                                  ((LENGTH) == USART_LINBreakDetectLength_12b) || \
                                                  ((LENGTH) == USART_LINBreakDetectLength_13b) || \
                                                  ((LENGTH) == USART_LINBreakDetectLength_14b))

/*
 * @defgroup USART Slave Select Management
 */
#define USART_NSS_Hw                        ((uint32_t)0x00000000)              /* USART Hardware NSS management */
#define USART_NSS_Sw                        ((uint32_t)0x00000008)              /* USART Software NSS management */
#define IS_USART_NSS(NSS) (((NSS) == USART_NSS_Hw) || \
                           ((NSS) == USART_NSS_Sw))

/*
 * @defgroup USART Last Bit
 */
#define USART_LastBit_Disable               ((uint32_t)0x0000)                  /* USART frame last data bit clock pulse not output to SCLK pin */
#define USART_LastBit_Enable                ((uint32_t)0x0100)                  /* USART frame last data bit clock pulse output to SCLK pin     */
#define IS_USART_LASTBIT(LASTBIT) (((LASTBIT) == USART_LastBit_Disable) || \
                                   ((LASTBIT) == USART_LastBit_Enable))

/*
 * @defgroup USART Clock Phase
 */
#define USART_CPHA_1Edge                    ((uint32_t)0x0000)                  /* USART frame phase on first clock transition  */
#define USART_CPHA_2Edge                    ((uint32_t)0x0200)                  /* USART frame phase on second clock transition */
#define IS_USART_CPHA(CPHA) (((CPHA) == USART_CPHA_1Edge) || ((CPHA) == USART_CPHA_2Edge))

/*
 * @defgroup USART Clock Polarity
 */
#define USART_CPOL_Low                      ((uint32_t)0x0000)                  /* USART Clock signal is steady Low  */
#define USART_CPOL_High                     ((uint32_t)0x0400)                  /* USART Clock signal is steady High */
#define IS_USART_CPOL(CPOL) (((CPOL) == USART_CPOL_Low) || ((CPOL) == USART_CPOL_High))

/*
 * @defgroup USART Clock
 */ 
#define USART_Clock_Disable                 ((uint32_t)0x0000)                  /* USART clock disable */
#define USART_Clock_Enable                  ((uint32_t)0x0800)                  /* USART clock enable  */
#define IS_USART_CLOCK(CLOCK) (((CLOCK) == USART_Clock_Disable) || \
                               ((CLOCK) == USART_Clock_Enable))
/*
 * @defgroup USART Number of Stop Bits
 */
#define USART_StopBits_1                    ((uint32_t)0x00000000)              /* USART frame with 1 stop bit */
#define USART_StopBits_2                    ((uint32_t)0x00002000)              /* USART frame with 2 stop bits */
#define IS_USART_STOPBITS(STOPBITS) (((STOPBITS) == USART_StopBits_1) || \
                                     ((STOPBITS) == USART_StopBits_2))

/*
 * @defgroup USART Local Interconnection Network mode
 */
#define USART_LIN_Disable                   ((uint32_t)0x00000000)              /* Local Interconnect Network disable */
#define USART_LIN_Enable                    ((uint32_t)0x00004000)              /* Local Interconnect Network enable */
#define IS_USART_LIN(LIN) (((LIN) == USART_LIN_Disable) || \
                           ((LIN) == USART_LIN_Enable))

/*
 * @defgroup USART Advanced Feature RX Pin Active Level Inversion
 */
#define USART_ADVFeature_RXINV_Disable      ((uint32_t)0x00000000)              /* RX pin active level inversion disable */
#define USART_ADVFeature_RXINV_Enable       ((uint32_t)0x00010000)              /* RX pin active level inversion enable  */
#define IS_USART_ADVFEATURE_RXINV(RXINV) (((RXINV) == USART_ADVFeature_RXINV_Disable) || \
                                         ((RXINV) == USART_ADVFeature_RXINV_Enable))

/*
 * @defgroup USART Advanced Feature TX Pin Active Level Inversion
 */
#define USART_ADVFeature_TXINV_Disable      ((uint32_t)0x00000000)              /* TX pin active level inversion disable */
#define USART_ADVFeature_TXINV_Enable       ((uint32_t)0x00020000)              /* TX pin active level inversion enable  */
#define IS_USART_ADVFEATURE_TXINV(TXINV) (((TXINV) == USART_ADVFeature_TXINV_Disable) || \
                                         ((TXINV) == USART_ADVFeature_TXINV_Enable))

/*
 * @defgroup USART Advanced Feature Binary Data Inversion
 */
#define USART_ADVFeature_DATAINV_Disable    ((uint32_t)0x00000000)              /* Binary data inversion disable */
#define USART_ADVFeature_DATAINV_Enable     ((uint32_t)0x00020000)              /* Binary data inversion enable  */
#define IS_USART_ADVFEATURE_DATAINV(DATAINV) (((DATAINV) == USART_ADVFeature_DATAINV_Disable) || \
                                             ((DATAINV) == USART_ADVFeature_DATAINV_Enable))

/*
 * @defgroup USART Advanced Feature MSB First
 */
#define USART_ADVFeature_MSBFIRST_Disable   ((uint32_t)0x00000000)              /* Most significant bit sent/received first disable */
#define USART_ADVFeature_MSBFIRST_Enable    ((uint32_t)0x00080000)              /* Most significant bit sent/received first enable  */
#define IS_USART_ADVFEATURE_MSBFIRST(MSBFIRST) (((MSBFIRST) == USART_ADVFEATURE_MSBFIRST_Disable) || \
                                                ((MSBFIRST) == USART_ADVFEATURE_MSBFIRST_Enable))

/*
 * @defgroup USART Advanced Feature Auto BaudRate Enable
 */
#define USART_ADVFeature_AutoBaudRate_Disable   ((uint32_t)0x00000000)          /* RX Auto Baud rate detection disable */
#define USART_ADVFeature_AutoBaudRate_Enable    ((uint32_t)0x00100000)          /* RX Auto Baud rate detection enable */
#define IS_USART_ADVFEATURE_AUTOBAUDRATE(AUTOBAUDRATE) (((AUTOBAUDRATE) == USART_ADVFeature_AutoBaudRate_Disable) || \
                                                        ((AUTOBAUDRATE) == USART_ADVFeature_AutoBaudRate_Enable))

/*
 * @defgroup USART Advanced Feature AutoBaud Rate Mode
 */
#define USART_ADVFEATURE_AutoBaudRate_ON0X55Frame   ((uint32_t)0x00600000)      /* Auto Baud rate detection on 0x55 frame detection */
#define IS_USART_ADVFEATURE_AUTOBAUDRATEMODE(MODE)  (((MODE) == USART_ADVFEATURE_AutoBaudRate_ON0X55Frame))

/*
 * @defgroup USART Half Duplex Selection
 */
#define USART_Half_Duplex_Disable           ((uint32_t)0x00000000)              /* USART half-duplex disabled */
#define USART_Half_Duplex_Enable            ((uint32_t)0x00000002)              /* USART half-duplex enabled  */
#define IS_USART_HALF_DUPLEX(HDSEL)     (((HDSEL) == USART_Half_Duplex_Disable) || \
                                         ((HDSEL) == USART_Half_Duplex_Enable))

/*
 * @defgroup USART DMA Requests
 */
#define USART_DMAReq_Tx                     ((uint32_t)0x0020)                  /* USART DMA TX enabled  */
#define USART_DMAReq_Rx                     ((uint32_t)0x0010)                  /* USART DMA RX enabled  */
#define IS_USART_DMAREQ(DMAREQ) ((((DMAREQ) & (uint32_t)0xFFFFFFCF) == 0x00) && ((DMAREQ) != (uint32_t)0x00))

/*
 * @defgroup USART Hardware Flow Control
 */
#define USART_HWControl_No                  ((uint32_t)0x00000000)              /* No hardware control */
#define USART_HWControl_RTS                 ((uint32_t)0x00000040)              /* Request To Send */
#define USART_HWControl_CTS                 ((uint32_t)0x00000080)              /* Clear To Send */
#define USART_HWControl_RTS_CTS             ((uint32_t)0x000000C0)	            /* Request and Clear To Send */
#define IS_USART_HARDWARE_FLOW_CONTROL(CONTROL) (((CONTROL) == USART_HWControl_No) || \
                                                 ((CONTROL) == USART_HWControl_RTS) ||  \
                                                 ((CONTROL) == USART_HWControl_CTS) ||  \
                                                 ((CONTROL) == USART_HWControl_RTS_CTS))

/*
 * @defgroup USART One Bit Sampling Method
 */
#define USART_One_BIT_SAMPLE_Enable         ((uint32_t)0x00000200)              /* One-bit sampling disable */
#define USART_Three_BIT_SAMPLE_Enable       ((uint32_t)0x00000000)              /* Three-bit sampling enable  */
#define IS_USART_ONE_BIT_SAMPLE(ONEBIT) (((ONEBIT) == USART_One_BIT_SAMPLE_Enable) || \
                                         ((ONEBIT) == USART_Three_BIT_SAMPLE_Enable))

/*
 * @defgroup USART Advanced Feature Overrun Disable
 */
#define USART_ADVFeature_Overrun_Enable     ((uint32_t)0x00000000)              /* RX overrun enable  */
#define USART_ADVFeature_Overrun_Disable    ((uint32_t)0x00000400)              /* RX overrun disable */
#define IS_USART_OVERRUN(OVERRUN)     (((OVERRUN) == USART_ADVFeature_Overrun_Enable) || \
                                      ((OVERRUN) == USART_ADVFeature_Overrun_Disable))

/*
 * @defgroup USART RXFIFO threshold level
 */
#define USART_RXFIFO_Threshold_1_8          ((uint32_t)0x00000000)              /* RXFIFO FIFO reaches 1/8 of its depth */
#define USART_RXFIFO_Threshold_1_4          ((uint32_t)0x00400000)              /* RXFIFO FIFO reaches 1/4 of its depth */
#define USART_RXFIFO_Threshold_1_2          ((uint32_t)0x00800000)              /* RXFIFO FIFO reaches 1/2 of its depth */
#define USART_RXFIFO_Threshold_3_4          ((uint32_t)0x00C00000)              /* RXFIFO FIFO reaches 3/4 of its depth */
#define USART_RXFIFO_Threshold_7_8          ((uint32_t)0x01000000)              /* RXFIFO FIFO reaches 7/8 of its depth */
#define USART_RXFIFO_Threshold_8_8          ((uint32_t)0x01400000)              /* RXFIFO FIFO becomes full */
#define IS_USART_RXFIFO_THRESHOLD(THRESHOLD) (((THRESHOLD) == USART_RXFIFO_Threshold_1_8) || \
                                              ((THRESHOLD) == USART_RXFIFO_Threshold_1_4) || \
                                              ((THRESHOLD) == USART_RXFIFO_Threshold_1_2) || \
                                              ((THRESHOLD) == USART_RXFIFO_Threshold_3_4) || \
                                              ((THRESHOLD) == USART_RXFIFO_Threshold_7_8) || \
                                              ((THRESHOLD) == USART_RXFIFO_Threshold_8_8))

/*
 * @defgroup USART TXFIFO threshold level
 */
#define USART_TXFIFO_Threshold_1_8          ((uint32_t)0x00000000)              /* TXFIFO reaches 1/8 of its depth */
#define USART_TXFIFO_Threshold_1_4          ((uint32_t)0x04000000)              /* TXFIFO reaches 1/4 of its depth */
#define USART_TXFIFO_Threshold_1_2          ((uint32_t)0x08000000)              /* TXFIFO reaches 1/2 of its depth */
#define USART_TXFIFO_Threshold_3_4          ((uint32_t)0x0C000000)              /* TXFIFO reaches 3/4 of its depth */
#define USART_TXFIFO_Threshold_7_8          ((uint32_t)0x10000000)              /* TXFIFO reaches 7/8 of its depth */
#define USART_TXFIFO_Threshold_8_8          ((uint32_t)0x14000000)              /* TXFIFO becomes empty */
#define IS_USART_TXFIFO_THRESHOLD(THRESHOLD) (((THRESHOLD) == USART_TXFIFO_Threshold_1_8) || \
                                              ((THRESHOLD) == USART_TXFIFO_Threshold_1_4) || \
                                              ((THRESHOLD) == USART_TXFIFO_Threshold_1_2) || \
                                              ((THRESHOLD) == USART_TXFIFO_Threshold_3_4) || \
                                              ((THRESHOLD) == USART_TXFIFO_Threshold_7_8) || \
                                              ((THRESHOLD) == USART_TXFIFO_Threshold_8_8))

/*
 * @defgroup USART Request Parameters
 */
#define USART_AutoBaud_Request              ((uint32_t)0x00000001)              /* Auto-Baud Rate Request      */
#define USART_SendBreak_Request             ((uint32_t)0x00000002)              /* Send Break Request          */
#define USART_RXData_Flush_Request          ((uint32_t)0x00000008)              /* Receive Data flush Request */
#define USART_TXData_Flush_Request          ((uint32_t)0x00000010)              /* Transmit data flush Request */
#define IS_USART_REQUEST_PARAMETER(PARAM) (((PARAM) == USART_AutoBaud_Request)     || \
                                           ((PARAM) == USART_SendBreak_Request)    || \
                                           ((PARAM) == USART_RXData_Flush_Request) || \
                                           ((PARAM) == USART_TXData_Flush_Request))

/*
 * @defgroup USART Advanced Feature Initialization Type
 */
#define USART_ADVFeature_No_Init                ((uint32_t)0x00000000)          /* No advanced feature initialization       */
#define USART_ADVFeature_TXInvert_Init          ((uint32_t)0x00000001)          /* TX pin active level inversion            */
#define USART_ADVFeature_RXInvert_Init          ((uint32_t)0x00000002)          /* RX pin active level inversion            */
#define USART_ADVFeature_DATAInvert_Init        ((uint32_t)0x00000004)          /* Binary data inversion                    */
#define USART_ADVFeature_RXOverrunDisable_Init  ((uint32_t)0x00000008)          /* RX overrun disable                       */
#define USART_ADVFeature_AutoBaudrate_Init      ((uint32_t)0x00000010)          /* Auto Baud rate detection initialization  */
#define USART_ADVFeature_MSBFirst_Init          ((uint32_t)0x00000020)          /* Most significant bit sent/received first */
#define IS_USART_ADVFEATURE_INIT(INIT) ((INIT) <= (USART_ADVFeature_No_Init                | \
                                                   USART_ADVFeature_TXInvert_Init          | \
                                                   USART_ADVFeature_RXInvert_Init          | \
                                                   USART_ADVFeature_DATAInvert_Init        | \
                                                   USART_ADVFeature_RXOverrunDisable_Init  | \
                                                   USART_ADVFeature_AutoBaudrate_Init      | \
                                                   USART_ADVFeature_MSBFirst_Init))

/*
 * @defgroup USART Flags
 */
#define USART_FLAG_TXFT                     ((uint32_t)0x08000000)              /* USART TXFIFO threshold flag */
#define USART_FLAG_RXFT                     ((uint32_t)0x04000000)              /* USART RXFIFO threshold flag */
#define USART_FLAG_RXFF                     ((uint32_t)0x01000000)              /* USART RXFIFO Full flag */
#define USART_FLAG_TXFE                     ((uint32_t)0x00800000)              /* USART TXFIFO Empty flag */
#define USART_FLAG_SBKF                     ((uint32_t)0x00040000)              /* USART send break flag */
#define USART_FLAG_BUSY                     ((uint32_t)0x00010000)              /* USART busy flag */
#define USART_FLAG_ABRF                     ((uint32_t)0x00008000)              /* USART auto Baud rate flag */
#define USART_FLAG_ABRE                     ((uint32_t)0x00004000)              /* USART uto Baud rate error */
#define USART_FLAG_CTS                      ((uint32_t)0x00000400)              /* USART CTS flag */
#define USART_FLAG_CTSIF                    ((uint32_t)0x00000200)              /* USART CTS interrupt flag */
#define USART_FLAG_LBDF                     ((uint32_t)0x00000100)              /* USART LIN break detection flag */
#define USART_FLAG_TXE                      ((uint32_t)0x00000080)              /* USART transmit data register empty */
#define USART_FLAG_TXFNF                    ((uint32_t)0x00000080)              /* USART TXFIFO not full */
#define USART_FLAG_TC                       ((uint32_t)0x00000040)              /* USART transmission complete */
#define USART_FLAG_RXNE                     ((uint32_t)0x00000020)              /* USART read data register not empty */
#define USART_FLAG_RXFNE                    ((uint32_t)0x00000020)              /* USART RXFIFO not empty  */
#define USART_FLAG_IDLE                     ((uint32_t)0x00000010)              /* USART idle flag */
#define USART_FLAG_ORE                      ((uint32_t)0x00000008)              /* USART overrun error */
#define USART_FLAG_NE                       ((uint32_t)0x00000004)              /* USART noise error */
#define USART_FLAG_FE                       ((uint32_t)0x00000002)              /* USART frame error */
#define USART_FLAG_PE                       ((uint32_t)0x00000001)              /* USART parity error */

#define IS_USART_FLAG(FLAG) (((FLAG) == USART_FLAG_TXFT)  || ((FLAG) == USART_FLAG_RXFT)  || \
                             ((FLAG) == USART_FLAG_RXFF)  || ((FLAG) == USART_FLAG_TXFE)  || \
                             ((FLAG) == USART_FLAG_SBKF)  || ((FLAG) == USART_FLAG_BUSY)  || \
                             ((FLAG) == USART_FLAG_ABRF)  || ((FLAG) == USART_FLAG_ABRE)  || \
                             ((FLAG) == USART_FLAG_CTS)   || ((FLAG) == USART_FLAG_CTSIF) || \
                             ((FLAG) == USART_FLAG_LBDF)  || ((FLAG) == USART_FLAG_TXE)   || \
                             ((FLAG) == USART_FLAG_TXFNF) || ((FLAG) == USART_FLAG_TC)    || \
                             ((FLAG) == USART_FLAG_RXNE)  || ((FLAG) == USART_FLAG_RXFNE) || \
                             ((FLAG) == USART_FLAG_IDLE)  || ((FLAG) == USART_FLAG_ORE)   || \
                             ((FLAG) == USART_FLAG_NE)    || ((FLAG) == USART_FLAG_FE)    || \
                             ((FLAG) == USART_FLAG_PE))


/*
 * @defgroup USART Interruption Flags Mask
 */
#define USART_IT_MASK                       ((uint32_t)0x0000001F)              /* USART interruptions flags mask */
#define USART_CR_MASK                       ((uint32_t)0x000000E0)              /* USART control register mask */
#define USART_CR_POS                        ((uint32_t)0x00000005)              /* USART control register position */

/*
 * @defgroup USART Interrupts Definition
 *        Elements values convention: 000000000XXYYYYYb
 *           - YYYYY  : Interrupt source position in the XX register (5bits)
 *           - XX  : Interrupt source register (2bits)
 *                 - 01: CR1 register
 *                 - 10: CR2 register
 *                 - 11: CR3 register
 */
#define USART_IT_IDLE                       ((uint32_t)0x00000024)              /* USART idle interruption*/
#define USART_IT_RXNE                       ((uint32_t)0x00000025)              /* USART read data register not empty interruption */
#define USART_IT_RXFNE                      ((uint32_t)0x00000025)              /* USART RXFIFO not empty interruption */
#define USART_IT_TC                         ((uint32_t)0x00000026)              /* USART transmission complete interruption */
#define USART_IT_TXE                        ((uint32_t)0x00000027)              /* USART transmit data register empty interruption */
#define USART_IT_TXFNF                      ((uint32_t)0x00000027)              /* USART TX FIFO not full interruption */
#define USART_IT_PE                         ((uint32_t)0x00000028)              /* USART parity error interruption */
#define USART_IT_TXFE                       ((uint32_t)0x0000003E)              /* USART TXFIFO empty interruption */
#define USART_IT_RXFF                       ((uint32_t)0x0000003F)              /* USART RXFIFO full interruption */
#define USART_IT_LBD                        ((uint32_t)0x00000046)              /* USART LIN break detection interruption */
#define USART_IT_ERR                        ((uint32_t)0x00000060)              /* USART error interruption */
#define USART_IT_CTS                        ((uint32_t)0x00000068)              /* USART CTS interruption*/
#define USART_IT_TXFT                       ((uint32_t)0x00000074)              /* USART TXFIFO threshold reached interruption */
#define USART_IT_RXFT                       ((uint32_t)0x00000079)              /* USART RXFIFO threshold reached interruption */
#define IS_USART_IT(IT) (((IT) == USART_IT_IDLE)  || ((IT) == USART_IT_RXNE)  || \
                         ((IT) == USART_IT_RXFNE) || ((IT) == USART_IT_TC)    || \
                         ((IT) == USART_IT_TXE)   || ((IT) == USART_IT_TXFNF) || \
                         ((IT) == USART_IT_PE)    || ((IT) == USART_IT_TXFE)  || \
                         ((IT) == USART_IT_RXFF)  || ((IT) == USART_IT_LBD)   || \
                         ((IT) == USART_IT_ERR)   || ((IT) == USART_IT_CTS)   || \
                         ((IT) == USART_IT_TXFT)  || ((IT) == USART_IT_RXFT))

/*
 * @defgroup USART Interruption Clear Flags
 */
#define USART_CLEAR_PEF                     ((uint32_t)0x00000001)              /* Parity error clear flag */
#define USART_CLEAR_FEF                     ((uint32_t)0x00000002)              /* Framing error clear flag */
#define USART_CLEAR_NEF                     ((uint32_t)0x00000004)              /* Noise error detected clear flag */
#define USART_CLEAR_OREF                    ((uint32_t)0x00000008)              /* Overrun error clear flag */
#define USART_CLEAR_IDLEF                   ((uint32_t)0x00000010)              /* Idle line detected clear flag */
#define USART_CLEAR_TXFEF                   ((uint32_t)0x00000020)              /* Txfifo empty clear flag */
#define USART_CLEAR_TCF                     ((uint32_t)0x00000040)              /* Transmission complete clear flag */
#define USART_CLEAR_LBDF                    ((uint32_t)0x00000100)              /* Lin break detection clear flag */
#define USART_CLEAR_CTSF                    ((uint32_t)0x00000200)              /* Cts clear flag */
#define USART_CLEAR_RXFTF                   ((uint32_t)0x04000000)              /* Clear rxfifo threshold interrupt flag */
#define USART_CLEAR_TXFTF                   ((uint32_t)0x08000000)              /* Clear txfifo threshold interrupt flag */
#define IS_USART_CLEAR(CLEAR) (((CLEAR) == USART_CLEAR_PEF) || ((CLEAR) == USART_CLEAR_FEF) ||     \
                               ((CLEAR) == USART_CLEAR_NEF) || ((CLEAR) == USART_CLEAR_OREF) ||    \
                               ((CLEAR) == USART_CLEAR_IDLEF) || ((CLEAR) == USART_CLEAR_TXFEF) || \
                               ((CLEAR) == USART_CLEAR_TCF) || ((CLEAR) == USART_CLEAR_LBDF) ||    \
                               ((CLEAR) == USART_CLEAR_CTSF) || ((CLEAR) == USART_CLEAR_RXFTF) ||    \
                               ((CLEAR) == USART_CLEAR_TXFTF))

#define IS_USART_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) <= 0xFFFFU))
#define IS_USART_DATA(DATA) ((DATA) <= 0x1FF)

/*
 * @brief Function declaration
 */
void USART_Init(USART_TypeDef *USARTx, USART_InitTypeDef *USART_InitStruct);
void USART_StructInit(USART_InitTypeDef* USART_InitStruct);
void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitTypeDef* USART_ClockInitStruct);
void USART_ClockStructInit(USART_ClockInitTypeDef* USART_ClockInitStruct);
void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_OneBitMethodCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SendData(USART_TypeDef* USARTx, uint8_t Data);
uint8_t USART_ReceiveData(USART_TypeDef* USARTx);
void USART_LINBreakDetectLengthConfig(USART_TypeDef* USARTx, uint16_t USART_LINBreakDetectLength);
void USART_LINCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SendBreak(USART_TypeDef *USARTx);
void USART_AutoBaudRate(USART_TypeDef *USARTx);
void USART_RxDataFlush(USART_TypeDef *USARTx);
void USART_TxDataFlush(USART_TypeDef *USARTx);
void USART_HalfDuplexCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_DMACmd(USART_TypeDef* USARTx, uint16_t USART_DMAReq, FunctionalState NewState);
void USART_EnableFifoMode(USART_TypeDef *USARTx);
void USART_DisableFifoMode(USART_TypeDef *USARTx);
void USART_SetTxFifoThreshold(USART_TypeDef *USARTx, uint32_t Threshold);
void USART_SetRxFifoThreshold(USART_TypeDef *USARTx, uint32_t Threshold);
void USART_EnableSlaveMode(USART_TypeDef *USARTx);
void USART_DisableSlaveMode(USART_TypeDef *USARTx);
void USART_ConfigNSS(USART_TypeDef *USARTx, uint32_t NSSConfig);
void USART_DeInit(USART_TypeDef *USARTx);
void USART_ITConfig(USART_TypeDef *USARTx, uint32_t ITType, EnState NewState);
FlagStatus USART_GetFlagStatus(USART_TypeDef *USARTx, uint32_t USART_FLAG);
FlagStatus USART_GetITStatus(USART_TypeDef *USARTx, uint32_t USART_IT);
void USART_ClearITPendingBit(USART_TypeDef *USARTx, uint32_t ClearIT);
void USART_AdvFeatureConfig(USART_TypeDef *USARTx, USART_AdvFeatureInitTypeDef *USART_AdvFeatureInitStruct);

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
