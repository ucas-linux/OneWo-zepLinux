/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_spi.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2023/12/24
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2023/12/24     1.0     build this module
 * Description:     This file contains all the functions prototypes for the SPI firmware library.
 */

#ifndef AS32X601_SPI_H
#define AS32X601_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601_spi_regs.h"

/*
 * @defgroup SPI Exported Constants
 */
#define IS_SPI_ALL_PERIPH(PERIPH) (((PERIPH) == SPI0) || \
                                   ((PERIPH) == SPI1) || \
                                   ((PERIPH) == SPI2) || \
                                   ((PERIPH) == SPI3) || \
                                   ((PERIPH) == SPI4) || \
                                   ((PERIPH) == SPI5))

/*
 * @brief Motorola trans mode enumeration
 */
typedef enum 
{
    SPI_TransNor  = 0x00,           /* Normal transmission mode */
    SPI_TransBToB = 0x10            /* Back-to-back transmission mode */
} SPITransMode_TypeDef;
#define IS_SPI_TRANSMODE(TRANSMODE) (((TRANSMODE) == SPI_TransNor) || ((TRANSMODE) == SPI_TransBToB))

/*
 * @brief Motorola trans mode enumeration
 */
typedef enum
{
    SPI_ResetFIFO_Rx      = 0x01,   /* Reset the SPI receive FIFO */
    SPI_ResetFIFO_Tx      = 0x02,   /* Reset the SPI send FIFO */
    SPI_ResetFIFO_TxandRx = 0x03    /* Reset the SPI send and receive FIFO */
} SPIResetFIFO_TypeDef;
#define IS_SPI_RESETFIFO(RESETFIFO) (((RESETFIFO) == SPI_ResetFIFO_Rx) || ((RESETFIFO) == SPI_ResetFIFO_Tx) || \
                                     ((RESETFIFO) == SPI_ResetFIFO_TxandRx))

/*
 * @brief SPI slave selection enumeration
 */
typedef enum 
{
    SPI_Slave0 = 0x01,              /* Selection slave 0 */
    SPI_Slave1 = 0x02,              /* Selection slave 1 */
    SPI_Slave2 = 0x04,              /* Selection slave 2 */
    SPI_Slave3 = 0x08
} SPISlaveSel_TypeDef;
#define IS_SPI_SLAVESEL(SLAVESEL)   (((SLAVESEL) == SPI_Slave0) || ((SLAVESEL) == SPI_Slave1) || \
                                     ((SLAVESEL) == SPI_Slave2) || ((SLAVESEL) == SPI_Slave3))

/*
 * @brief SPI init structure definition
 */
typedef struct
{
    uint32_t SPI_CLKDiv;            /* Specifies the clock frequency division */
    uint32_t SPI_Mode;              /* Specifies the SPI operating mode */
    uint32_t SPI_FrameSize;         /* Specifies the SPI data size */
    uint32_t SPI_MotorolaMode;      /* Select the specifies SPI motorola mode */
    uint32_t SPI_TransMode;         /* Specifies the SPI transmission mode */
} SPI_InitTypeDef;

/*
 * @defgroup SPI mode definition
 */
#define SPI_Mode_Master         ((uint32_t)0x00000002)      /* Master mode */
#define SPI_Mode_Slave          ((uint32_t)0x00000000)      /* Slave mode */
#define IS_SPI_Mode(MODE) (((MODE) == SPI_Mode_Master) || ((MODE) == SPI_Mode_Slave))

/*
 * @defgroup Baud rate prescaler definition
 */
#define SPI_CLKDiv2             ((uint32_t)0x00000000)      /* SPI clock frequency division 2 */
#define SPI_CLKDiv4             ((uint32_t)0x00000001)      /* SPI clock frequency division 4 */
#define SPI_CLKDiv8             ((uint32_t)0x00000003)      /* SPI clock frequency division 8 */
#define SPI_CLKDiv16            ((uint32_t)0x00000007)      /* SPI clock frequency division 16 */
#define SPI_CLKDiv32            ((uint32_t)0x0000000F)      /* SPI clock frequency division 32 */
#define SPI_CLKDiv64            ((uint32_t)0x0000001F)      /* SPI clock frequency division 64 */
#define SPI_CLKDiv128           ((uint32_t)0x0000003F)      /* SPI clock frequency division 128 */
#define SPI_CLKDiv256           ((uint32_t)0x0000007F)      /* SPI clock frequency division 256 */
#define SPI_CLKDiv512           ((uint32_t)0x000000FF)      /* SPI clock frequency division 512 */
#define IS_SPI_CLKDIV(CLKDIV) (((CLKDIV) == SPI_CLKDiv2)   || ((CLKDIV) == SPI_CLKDiv4)   || \
                               ((CLKDIV) == SPI_CLKDiv8)   || ((CLKDIV) == SPI_CLKDiv16)  || \
                               ((CLKDIV) == SPI_CLKDiv32)  || ((CLKDIV) == SPI_CLKDiv64)  || \
                               ((CLKDIV) == SPI_CLKDiv128) || ((CLKDIV) == SPI_CLKDiv256) || \
                               ((CLKDIV) == SPI_CLKDiv512))

/*
 * @defgroup SPI frame size definition
 */
#define SPI_FrameSize_4b        ((uint32_t)0x00001000)      /* Frame size 4bit */
#define SPI_FrameSize_5b        ((uint32_t)0x00001400)      /* Frame size 5bit */
#define SPI_FrameSize_6b        ((uint32_t)0x00001800)      /* Frame size 6bit */
#define SPI_FrameSize_7b        ((uint32_t)0x00001C00)      /* Frame size 7bit */
#define SPI_FrameSize_8b        ((uint32_t)0x00002000)      /* Frame size 8bit */
#define SPI_FrameSize_9b        ((uint32_t)0x00002400)      /* Frame size 9bit */
#define SPI_FrameSize_10b       ((uint32_t)0x00002800)      /* Frame size 10bit */
#define SPI_FrameSize_11b       ((uint32_t)0x00002C00)      /* Frame size 11bit */
#define SPI_FrameSize_12b       ((uint32_t)0x00003000)      /* Frame size 12bit */
#define SPI_FrameSize_13b       ((uint32_t)0x00003400)      /* Frame size 13bit */
#define SPI_FrameSize_14b       ((uint32_t)0x00003800)      /* Frame size 14bit */
#define SPI_FrameSize_15b       ((uint32_t)0x00003C00)      /* Frame size 15bit */
#define SPI_FrameSize_16b       ((uint32_t)0x00004000)      /* Frame size 16bit */
#define SPI_FrameSize_17b       ((uint32_t)0x00004400)      /* Frame size 17bit */
#define SPI_FrameSize_18b       ((uint32_t)0x00004800)      /* Frame size 18bit */
#define SPI_FrameSize_19b       ((uint32_t)0x00004C00)      /* Frame size 19bit */
#define SPI_FrameSize_20b       ((uint32_t)0x00005000)      /* Frame size 20bit */
#define SPI_FrameSize_21b       ((uint32_t)0x00005400)      /* Frame size 21bit */
#define SPI_FrameSize_22b       ((uint32_t)0x00005800)      /* Frame size 22bit */
#define SPI_FrameSize_23b       ((uint32_t)0x00005C00)      /* Frame size 23bit */
#define SPI_FrameSize_24b       ((uint32_t)0x00006000)      /* Frame size 24bit */
#define SPI_FrameSize_25b       ((uint32_t)0x00006400)      /* Frame size 25bit */
#define SPI_FrameSize_26b       ((uint32_t)0x00006800)      /* Frame size 26bit */
#define SPI_FrameSize_27b       ((uint32_t)0x00006C00)      /* Frame size 27bit */
#define SPI_FrameSize_28b       ((uint32_t)0x00007000)      /* Frame size 28bit */
#define SPI_FrameSize_29b       ((uint32_t)0x00007400)      /* Frame size 29bit */
#define SPI_FrameSize_30b       ((uint32_t)0x00007800)      /* Frame size 30bit */
#define SPI_FrameSize_31b       ((uint32_t)0x00007C00)      /* Frame size 31bit */
#define SPI_FrameSize_32b       ((uint32_t)0x00008000)      /* Frame size 32bit */
#define IS_SPI_FRAMESIZE(FRAMESIZE) (((FRAMESIZE) == SPI_FrameSize_4b)  || ((FRAMESIZE) == SPI_FrameSize_5b)  || \
                                     ((FRAMESIZE) == SPI_FrameSize_6b)  || ((FRAMESIZE) == SPI_FrameSize_7b)  || \
                                     ((FRAMESIZE) == SPI_FrameSize_8b)  || ((FRAMESIZE) == SPI_FrameSize_9b)  || \
                                     ((FRAMESIZE) == SPI_FrameSize_10b) || ((FRAMESIZE) == SPI_FrameSize_11b) || \
                                     ((FRAMESIZE) == SPI_FrameSize_12b) || ((FRAMESIZE) == SPI_FrameSize_13b) || \
                                     ((FRAMESIZE) == SPI_FrameSize_14b) || ((FRAMESIZE) == SPI_FrameSize_15b) || \
                                     ((FRAMESIZE) == SPI_FrameSize_16b) || ((FRAMESIZE) == SPI_FrameSize_17b) || \
                                     ((FRAMESIZE) == SPI_FrameSize_18b) || ((FRAMESIZE) == SPI_FrameSize_19b) || \
                                     ((FRAMESIZE) == SPI_FrameSize_20b) || ((FRAMESIZE) == SPI_FrameSize_21b) || \
                                     ((FRAMESIZE) == SPI_FrameSize_22b) || ((FRAMESIZE) == SPI_FrameSize_23b) || \
                                     ((FRAMESIZE) == SPI_FrameSize_24b) || ((FRAMESIZE) == SPI_FrameSize_25b) || \
                                     ((FRAMESIZE) == SPI_FrameSize_26b) || ((FRAMESIZE) == SPI_FrameSize_27b) || \
                                     ((FRAMESIZE) == SPI_FrameSize_28b) || ((FRAMESIZE) == SPI_FrameSize_29b) || \
                                     ((FRAMESIZE) == SPI_FrameSize_30b) || ((FRAMESIZE) == SPI_FrameSize_31b) || \
                                     ((FRAMESIZE) == SPI_FrameSize_32b))

/*
 * @defgroup Motorola mode definition
 */
#define SPI_Motorola_Mode0      ((uint32_t)0x00000000)      /* Motorola mode0 */
#define SPI_Motorola_Mode1      ((uint32_t)0x00000004)      /* Motorola mode1 */
#define SPI_Motorola_Mode2      ((uint32_t)0x00000008)      /* Motorola mode2 */
#define SPI_Motorola_Mode3      ((uint32_t)0x0000000c)      /* Motorola mode3 */
#define IS_SPI_MOTOROLAMODE(MOTOROLAMODE) (((MOTOROLAMODE) == SPI_Motorola_Mode0) || ((MOTOROLAMODE) == SPI_Motorola_Mode1) || \
                                           ((MOTOROLAMODE) == SPI_Motorola_Mode2) || ((MOTOROLAMODE) == SPI_Motorola_Mode3))

/*
 * @defgroup SPI Interruption Flags Mask
 */
#define SPI_IT_MASK             ((uint32_t)0x0000001F)      /* SPI interruptions flags mask */
#define SPI_CR_MASK             ((uint32_t)0x000000E0)      /* SPI control register mask */
#define SPI_CR_POS              ((uint32_t)0x00000005)      /* SPI control register position */

/*
 * @defgroup SPI Interrupts Definition
 *        Elements values convention: 000000000XXYYYYYb
 *           - YYYYY  : Interrupt source position in the XX register (5bits)
 *           - XX  : Interrupt source register (2bits)
 *                 - 01: CR0 register
 *                 - 10: IER register
 */
#define SPI_IT_TD               ((uint32_t)0x00000023)      /* Tx done interrupt */
#define SPI_IT_RXO              ((uint32_t)0x00000024)      /* Rx buffer overflow interrupt */
#define SPI_IT_TXE              ((uint32_t)0x00000025)      /* Tx buffer empty interrupt */
#define SPI_IT_NSSV             ((uint32_t)0x00000045)      /* NSS Voh interrupt */
#define SPI_IT_RXNE             ((uint32_t)0x00000046)      /* Rx buffer non-empty interrupt */
#define SPI_IT_TXNE             ((uint32_t)0x00000047)      /* Tx buffer non-full interrupt */
#define IS_SPI_CONFIG_IT(IT) (((IT) == SPI_IT_TD)   || ((IT) == SPI_IT_RXO)  || \
                              ((IT) == SPI_IT_TXE)  || ((IT) == SPI_IT_NSSV) || \
                              ((IT) == SPI_IT_RXNE) || ((IT) == SPI_IT_TXNE))

/*
 * @defgroup SPI interrupts status definition
 */
#define SPI_ITStatus_TD         ((uint32_t)0x00000001)      /* Tx done interrupt status */
#define SPI_ITStatus_RXO        ((uint32_t)0x00000004)      /* Rx buffer overflow interrupt status */
#define SPI_ITStatus_TXE        ((uint32_t)0x00000008)      /* Tx buffer empty interrupt status */
#define SPI_ITStatus_NSSV       ((uint32_t)0x00000020)      /* NSS Voh interrupt status */
#define SPI_ITStatus_RXNE       ((uint32_t)0x00000040)      /* Rx buffer non-empty interrupt status */
#define SPI_ITStatus_TXNF       ((uint32_t)0x00000080)      /* Tx buffer non-full interrupt status */
#define IS_SPI_GETSTA_IT(ITSTA) (((ITSTA) == SPI_ITStatus_TD)   || ((ITSTA) == SPI_ITStatus_RXO)  || \
                                 ((ITSTA) == SPI_ITStatus_TXE)  || ((ITSTA) == SPI_ITStatus_NSSV) || \
                                 ((ITSTA) == SPI_ITStatus_RXNE) || ((ITSTA) == SPI_ITStatus_TXNF))

/*
 * @defgroup SPI interrupts status definition
 */
#define SPI_CLEAR_TD            ((uint32_t)0x00000001)      /* Tx done clear interrupt status */
#define SPI_CLEAR_RXO           ((uint32_t)0x00000004)      /* Rx buffer overflow clear interrupt status */
#define SPI_CLEAR_TXE           ((uint32_t)0x00000008)      /* Tx buffer empty clear interrupt status */
#define SPI_CLEAR_NSSV          ((uint32_t)0x00000020)      /* NSS Voh clear interrupt status */
#define SPI_CLEAR_RXNE          ((uint32_t)0x00000040)      /* Rx buffer non-empty clear interrupt status */
#define SPI_CLEAR_TXNF          ((uint32_t)0x00000080)      /* Tx buffer non-full clear interrupt status */
#define IS_SPI_CLEAR_IT(IT) (((IT) == SPI_CLEAR_TD)   || ((IT) == SPI_CLEAR_RXO)  || \
                             ((IT) == SPI_CLEAR_TXE)  || ((IT) == SPI_CLEAR_NSSV) || \
                             ((IT) == SPI_CLEAR_RXNE) || ((IT) == SPI_CLEAR_TXNF))

/*
 * @defgroup SPI interrupts raw status definition
 */
#define SPI_ITRaw_TD            ((uint32_t)0x00000001)      /* Tx done interrupt raw status */
#define SPI_ITRaw_RXO           ((uint32_t)0x00000004)      /* Rx buffer overflow interrupt raw status */
#define SPI_ITRaw_TXE           ((uint32_t)0x00000008)      /* Tx buffer empty interrupt raw status */
#define SPI_ITRaw_NSSV          ((uint32_t)0x00000020)      /* NSS Voh interrupt raw status */
#define SPI_ITRaw_RXNE          ((uint32_t)0x00000040)      /* Rx buffer non-empty interrupt raw status */
#define SPI_ITRaw_TXNF          ((uint32_t)0x00000080)      /* Tx buffer non-full interrupt raw status */
#define IS_SPI_GETRAW_IT(ITRAW) (((ITRAW) == SPI_ITRaw_TD)   || ((ITRAW) == SPI_ITRaw_RXO)  || \
                                 ((ITRAW) == SPI_ITRaw_TXE)  || ((ITRAW) == SPI_ITRaw_NSSV) || \
                                 ((ITRAW) == SPI_ITRaw_RXNE) || ((ITRAW) == SPI_ITRaw_TXNF))

/*
 * @defgroup SPI status definition
 */
#define SPI_Flag_NFRX           ((uint32_t)0x00000001)      /* NSS is effective, the next frame in the receiving buffer is received for the first time */
#define SPI_Flag_DONE           ((uint32_t)0x00000002)      /* No request for receiving and sending */
#define SPI_Flag_RXE            ((uint32_t)0x00000004)      /* Rx buffer is empty */
#define SPI_Flag_TXF            ((uint32_t)0x00000008)      /* Tx buffer is full */
#define SPI_Flag_RXO            ((uint32_t)0x00000010)      /* Rx buffer is overflow */
#define SPI_Flag_TXE            ((uint32_t)0x00000020)      /* Tx buffer is empty */
#define SPI_Flag_NSSS           ((uint32_t)0x00000040)      /* The state of select slave */
#define SPI_Flag_BUSS           ((uint32_t)0x00000080)      /* SPI is still transmitting data */
#define IS_SPI_FLAG(FLAG) (((FLAG) == SPI_Flag_NFRX) || ((FLAG) == SPI_Flag_DONE) || \
                           ((FLAG) == SPI_Flag_RXE)  || ((FLAG) == SPI_Flag_TXF)  || \
                           ((FLAG) == SPI_Flag_RXO)  || ((FLAG) == SPI_Flag_TXE)  || \
                           ((FLAG) == SPI_Flag_NSSS) || ((FLAG) == SPI_Flag_BUSS))

/*
 * @brief Function declaration
 */
void SPI_StructInit(SPI_InitTypeDef *SPI_InitStruct);
void SPI_Init(SPI_TypeDef *SPIx, SPI_InitTypeDef *SPI_InitStruct);
void SPI_Cmd(SPI_TypeDef *SPIx, FunctionalState NewState);
void SPI_ResetFIFO(SPI_TypeDef *SPIx, SPIResetFIFO_TypeDef SPI_FIFOType);
void SPI_NSSConfig(SPI_TypeDef *SPIx, SPISlaveSel_TypeDef Slavex, FunctionalState NewState);
void SPI_SendData(SPI_TypeDef *SPIx, uint32_t Data);
void SPI_SendDataLast(SPI_TypeDef *SPIx, uint32_t DataLast);
uint32_t SPI_ReceiveData(SPI_TypeDef *SPIx);
void SPI_ITConfig(SPI_TypeDef *SPIx, uint32_t SPI_IT, FunctionalState NewState);
ITStatus SPI_GetITStatus(SPI_TypeDef *SPIx, uint32_t SPI_ITSTA);
ITStatus SPI_GetITRawStatus(SPI_TypeDef *SPIx, uint32_t SPI_ITRaw);
FlagStatus SPI_GetFlagStatus(SPI_TypeDef *SPIx, uint32_t SPI_FLAG);
void SPI_ClearITPendingBit(SPI_TypeDef *SPIx, uint32_t SPI_ITSTA);

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
