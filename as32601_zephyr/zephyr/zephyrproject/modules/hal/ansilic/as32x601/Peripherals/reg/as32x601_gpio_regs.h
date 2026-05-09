/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_gpio_regs.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2024/07/15
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/07/15     1.0     build this module
 * Description:     This file contains all the register correlation defines for the GPIO firmware library.
 */

#ifndef AS32X601_GPIO_REG_H
#define AS32X601_GPIO_REG_H

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
    __IO uint32_t RWM0CR;					/* GPIO_RW0               Read/write mode control register 0,      Address offset: 0x00 */
    __IO uint32_t IM0CR;					/* GPIO_IM0               Input mode control register 0,           Address offset: 0x04 */
    __IO uint32_t OMCR;						/* GPIO_OM                Output mode control register,            Address offset: 0x08 */
    __IO uint32_t OICR;						/* GPIO_OSTR0             Output intensity control register 0,     Address offset: 0x0C */
    __IO uint32_t MI0SR;					/* GPIO_PM0               Multiplex interface select register 0,   Address offset: 0x10 */
    __IO uint32_t MI1SR;					/* GPIO_PM1               Multiplex interface select register 1,   Address offset: 0x14 */
    __O  uint32_t OPBR;						/* GPIO_OBS               Output position bit register,            Address offset: 0x18 */
    __O  uint32_t OBCR;						/* GPIO_OBC               Output bit clear register,               Address offset: 0x1C */
    __O  uint32_t OBTR;						/* GPIO_OBT               Output bit flip register,                Address offset: 0x20 */
    __IO uint32_t OCR;						/* GPIO_OC                Output control register,                 Address offset: 0x24 */
    __I  uint32_t IRR;						/* GPIO_RD                Read interface register,                 Address offset: 0x28 */
    __IO uint32_t ITYPE0R;					/* GPIO_ITYPE0            Interrupt type register 0,               Address offset: 0x2C */
    __IO uint32_t IRSR;						/* GPIO_INTCFG_INT_RAW    Interrupt Raw status Register,           Address offset: 0x30 */
    __IO uint32_t IFR;						/* GPIO_intCfg_INT_FORCE  Interrupt Force Register,                Address offset: 0x34 */
    __IO uint32_t IMR;						/* GPIO_intCfg_INT_MASK   Interrupt Mask Register,                 Address offset: 0x38 */
    __IO uint32_t ISR;						/* GPIO_intCfg_INT_STATUS Interrupt status Register,               Address offset: 0x3C */
} GPIO_TypeDef;

/*
 * @addtogroup GPIO Peripheral declaration
 */
#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD   ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE   ((GPIO_TypeDef *)GPIOE_BASE)
#define GPIOF   ((GPIO_TypeDef *)GPIOF_BASE)
#define GPIOG   ((GPIO_TypeDef *)GPIOG_BASE)
#define GPIOH   ((GPIO_TypeDef *)GPIOH_BASE)  

/*
 * @brief GPIOH mapping registers structure
 */
typedef struct
{
    __IO uint32_t RWM0CR;					/* GPIO_RW0               Read/write mode control register 0,      Address offset: 0x00 */
    __IO uint32_t IM0CR;					/* GPIO_IM0               Input mode control register 0,           Address offset: 0x04 */
    __IO uint32_t OMCR;						/* GPIO_OM                Output mode control register,            Address offset: 0x08 */
    __IO uint32_t OICR;						/* GPIO_OSTR0             Output intensity control register 0,     Address offset: 0x0C */
    __IO uint32_t MI0SR;					/* GPIO_PM0               Multiplex interface select register 0,   Address offset: 0x10 */
    __O  uint32_t OPBR;						/* GPIO_OBS               Output position bit register,            Address offset: 0x18 */
    __O  uint32_t OBCR;						/* GPIO_OBC               Output bit clear register,               Address offset: 0x1C */
    __O  uint32_t OBTR;						/* GPIO_OBT               Output bit flip register,                Address offset: 0x20 */
    __IO uint32_t OCR;						/* GPIO_OC                Output control register,                 Address offset: 0x24 */
    __I  uint32_t IRR;						/* GPIO_RD                Read interface register,                 Address offset: 0x28 */
    __IO uint32_t ITYPE0R;					/* GPIO_ITYPE0            Interrupt type register 0,               Address offset: 0x2C */
    __IO uint32_t IRSR;						/* GPIO_INTCFG_INT_RAW    Interrupt Raw status Register,           Address offset: 0x30 */
    __IO uint32_t IFR;						/* GPIO_intCfg_INT_FORCE  Interrupt Force Register,                Address offset: 0x34 */
    __IO uint32_t IMR;						/* GPIO_intCfg_INT_MASK   Interrupt Mask Register,                 Address offset: 0x38 */
    __IO uint32_t ISR;						/* GPIO_intCfg_INT_STATUS Interrupt status Register,               Address offset: 0x3C */
} GPIOHMAPPING_TypeDef;

/*
 * @addtogroup GPIOH Peripheral declaration
 */ 
#define GPIOHMAP   ((GPIOHMAPPING_TypeDef *)GPIOH_BASE)

/******************  Bits definition for GPIO_RWM0CR register  *****************/
#define GPIO_RWM0CR_RW0_DPos        (0U)
#define GPIO_RWM0CR_MODER0_Pos      GPIO_RWM0CR_RW0_DPos
#define GPIO_RWM0CR_MODER0_Msk      (0x03UL << GPIO_RWM0CR_MODER0_Pos)          /* 0x00000003 */
#define GPIO_RWM0CR_MODER0          GPIO_RWM0CR_MODER0_Msk                      /* RW[1:0] bits (Control the read and write mode of Control 0) */
#define GPIO_RWM0CR_MODER0_0        (0x01UL << GPIO_RWM0CR_MODER0_Pos)          /* 0x00000001 */
#define GPIO_RWM0CR_MODER0_1        (0x02UL << GPIO_RWM0CR_MODER0_Pos)          /* 0x00000002 */

#define GPIO_RWM0CR_RW1_DPos        (2U)
#define GPIO_RWM0CR_MODER1_Pos      GPIO_RWM0CR_RW1_DPos
#define GPIO_RWM0CR_MODER1_Msk      (0x03UL << GPIO_RWM0CR_MODER1_Pos)          /* 0x0000000C */
#define GPIO_RWM0CR_MODER1          GPIO_RWM0CR_MODER1_Msk                      /* RW1[1:0] bits (Control the read and write mode of Channel 1) */
#define GPIO_RWM0CR_MODER1_0        (0x01UL << GPIO_RWM0CR_MODER1_Pos)          /* 0x00000004 */
#define GPIO_RWM0CR_MODER1_1        (0x02UL << GPIO_RWM0CR_MODER1_Pos)          /* 0x00000008 */

#define GPIO_RWM0CR_RW2_DPos        (4U)
#define GPIO_RWM0CR_MODER2_Pos      GPIO_RWM0CR_RW2_DPos
#define GPIO_RWM0CR_MODER2_Msk      (0x03UL << GPIO_RWM0CR_MODER2_Pos)          /* 0x00000030 */
#define GPIO_RWM0CR_MODER2          GPIO_RWM0CR_MODER2_Msk                      /* RW2[1:0] bits (Control the read and write mode of Channel 2) */
#define GPIO_RWM0CR_MODER2_0        (0x01UL << GPIO_RWM0CR_MODER2_Pos)          /* 0x00000010 */
#define GPIO_RWM0CR_MODER2_1        (0x02UL << GPIO_RWM0CR_MODER2_Pos)          /* 0x00000020 */

#define GPIO_RWM0CR_RW3_DPos        (6U)
#define GPIO_RWM0CR_MODER3_Pos      GPIO_RWM0CR_RW3_DPos
#define GPIO_RWM0CR_MODER3_Msk      (0x03UL << GPIO_RWM0CR_MODER3_Pos)          /* 0x000000C0 */
#define GPIO_RWM0CR_MODER3          GPIO_RWM0CR_MODER3_Msk                      /* RW3[1:0] bits (Control the read and write mode of Channel 3) */
#define GPIO_RWM0CR_MODER3_0        (0x01UL << GPIO_RWM0CR_MODER3_Pos)          /* 0x00000040 */
#define GPIO_RWM0CR_MODER3_1        (0x02UL << GPIO_RWM0CR_MODER3_Pos)          /* 0x00000080 */

#define GPIO_RWM0CR_RW4_DPos        (8U)
#define GPIO_RWM0CR_MODER4_Pos      GPIO_RWM0CR_RW4_DPos
#define GPIO_RWM0CR_MODER4_Msk      (0x03UL << GPIO_RWM0CR_MODER4_Pos)          /* 0x00000300 */
#define GPIO_RWM0CR_MODER4          GPIO_RWM0CR_MODER4_Msk                      /* RW4[1:0] bits (Control the read and write mode of Channel 4) */
#define GPIO_RWM0CR_MODER4_0        (0x01UL << GPIO_RWM0CR_MODER4_Pos)          /* 0x00000100 */
#define GPIO_RWM0CR_MODER4_1        (0x02UL << GPIO_RWM0CR_MODER4_Pos)          /* 0x00000200 */

#define GPIO_RWM0CR_RW5_DPos        (10U)
#define GPIO_RWM0CR_MODER5_Pos      GPIO_RWM0CR_RW5_DPos
#define GPIO_RWM0CR_MODER5_Msk      (0x03UL << GPIO_RWM0CR_MODER5_Pos)          /* 0x00000C00 */
#define GPIO_RWM0CR_MODER5          GPIO_RWM0CR_MODER5_Msk                      /* RW5[1:0] bits (Control the read and write mode of Channel 5) */
#define GPIO_RWM0CR_MODER5_0        (0x01UL << GPIO_RWM0CR_MODER5_Pos)          /* 0x00000400 */
#define GPIO_RWM0CR_MODER5_1        (0x02UL << GPIO_RWM0CR_MODER5_Pos)          /* 0x00000800 */

#define GPIO_RWM0CR_RW6_DPos        (12U)
#define GPIO_RWM0CR_MODER6_Pos      GPIO_RWM0CR_RW6_DPos
#define GPIO_RWM0CR_MODER6_Msk      (0x03UL << GPIO_RWM0CR_MODER6_Pos)          /* 0x00003000 */
#define GPIO_RWM0CR_MODER6          GPIO_RWM0CR_MODER6_Msk                      /* RW6[1:0] bits (Control the read and write mode of Channel 6) */
#define GPIO_RWM0CR_MODER6_0        (0x01UL << GPIO_RWM0CR_MODER6_Pos)          /* 0x00001000 */
#define GPIO_RWM0CR_MODER6_1        (0x02UL << GPIO_RWM0CR_MODER6_Pos)          /* 0x00002000 */

#define GPIO_RWM0CR_RW7_DPos        (14U)
#define GPIO_RWM0CR_MODER7_Pos      GPIO_RWM0CR_RW7_DPos
#define GPIO_RWM0CR_MODER7_Msk      (0x03UL << GPIO_RWM0CR_MODER7_Pos)          /* 0x0000C000 */
#define GPIO_RWM0CR_MODER7          GPIO_RWM0CR_MODER7_Msk                      /* RW7[1:0] bits (Control the read and write mode of Channel 7) */
#define GPIO_RWM0CR_MODER7_0        (0x01UL << GPIO_RWM0CR_MODER7_Pos)          /* 0x00004000 */
#define GPIO_RWM0CR_MODER7_1        (0x02UL << GPIO_RWM0CR_MODER7_Pos)          /* 0x00008000 */

#define GPIO_RWM0CR_RW8_DPos        (16U)
#define GPIO_RWM0CR_MODER8_Pos      GPIO_RWM0CR_RW8_DPos
#define GPIO_RWM0CR_MODER8_Msk      (0x03UL << GPIO_RWM0CR_MODER8_Pos)          /* 0x00030000 */
#define GPIO_RWM0CR_MODER8          GPIO_RWM0CR_MODER8_Msk                      /* RW8[1:0] bits (Control the read and write mode of Channel 8) */
#define GPIO_RWM0CR_MODER8_0        (0x01UL << GPIO_RWM0CR_MODER8_Pos)          /* 0x00010000 */
#define GPIO_RWM0CR_MODER8_1        (0x02UL << GPIO_RWM0CR_MODER8_Pos)          /* 0x00020000 */

#define GPIO_RWM0CR_RW9_DPos        (18U)
#define GPIO_RWM0CR_MODER9_Pos      GPIO_RWM0CR_RW9_DPos
#define GPIO_RWM0CR_MODER9_Msk      (0x03UL << GPIO_RWM0CR_MODER9_Pos)          /* 0x000C0000 */
#define GPIO_RWM0CR_MODER9          GPIO_RWM0CR_MODER9_Msk                      /* RW9[1:0] bits (Control the read and write mode of Channel 9) */
#define GPIO_RWM0CR_MODER9_0        (0x01UL << GPIO_RWM0CR_MODER9_Pos)          /* 0x00040000 */
#define GPIO_RWM0CR_MODER9_1        (0x02UL << GPIO_RWM0CR_MODER9_Pos)          /* 0x00080000 */

#define GPIO_RWM0CR_RW10_DPos       (20U)
#define GPIO_RWM0CR_MODER10_Pos     GPIO_RWM0CR_RW10_DPos
#define GPIO_RWM0CR_MODER10_Msk     (0x03UL << GPIO_RWM0CR_MODER10_Pos)         /* 0x00300000 */
#define GPIO_RWM0CR_MODER10         GPIO_RWM0CR_MODER10_Msk                     /* RW10[1:0] bits (Control the read and write mode of Channel 10) */
#define GPIO_RWM0CR_MODER10_0       (0x01UL << GPIO_RWM0CR_MODER10_Pos)         /* 0x00100000 */
#define GPIO_RWM0CR_MODER10_1       (0x02UL << GPIO_RWM0CR_MODER10_Pos)         /* 0x00200000 */

#define GPIO_RWM0CR_RW11_DPos       (22U)
#define GPIO_RWM0CR_MODER11_Pos     GPIO_RWM0CR_RW11_DPos
#define GPIO_RWM0CR_MODER11_Msk     (0x03UL << GPIO_RWM0CR_MODER11_Pos)         /* 0x00C00000 */
#define GPIO_RWM0CR_MODER11         GPIO_RWM0CR_MODER11_Msk                     /* RW11[1:0] bits (Control the read and write mode of Channel 11) */
#define GPIO_RWM0CR_MODER11_0       (0x01UL << GPIO_RWM0CR_MODER11_Pos)         /* 0x00400000 */
#define GPIO_RWM0CR_MODER11_1       (0x02UL << GPIO_RWM0CR_MODER11_Pos)         /* 0x00800000 */

#define GPIO_RWM0CR_RW12_DPos       (24U)
#define GPIO_RWM0CR_MODER12_Pos     GPIO_RWM0CR_RW12_DPos
#define GPIO_RWM0CR_MODER12_Msk     (0x03UL << GPIO_RWM0CR_MODER12_Pos)         /* 0x03000000 */
#define GPIO_RWM0CR_MODER12         GPIO_RWM0CR_MODER12_Msk                     /* RW12[1:0] bits (Control the read and write mode of Channel 12) */
#define GPIO_RWM0CR_MODER12_0       (0x01UL << GPIO_RWM0CR_MODER12_Pos)         /* 0x01000000 */
#define GPIO_RWM0CR_MODER12_1       (0x02UL << GPIO_RWM0CR_MODER12_Pos)         /* 0x02000000 */

#define GPIO_RWM0CR_RW13_DPos       (26U)
#define GPIO_RWM0CR_MODER13_Pos     GPIO_RWM0CR_RW13_DPos
#define GPIO_RWM0CR_MODER13_Msk     (0x03UL << GPIO_RWM0CR_MODER13_Pos)         /* 0x0C000000 */
#define GPIO_RWM0CR_MODER13         GPIO_RWM0CR_MODER13_Msk                     /* RW13[1:0] bits (Control the read and write mode of Channel 13) */
#define GPIO_RWM0CR_MODER13_0       (0x01UL << GPIO_RWM0CR_MODER13_Pos)         /* 0x04000000 */
#define GPIO_RWM0CR_MODER13_1       (0x02UL << GPIO_RWM0CR_MODER13_Pos)         /* 0x08000000 */

#define GPIO_RWM0CR_RW14_DPos       (28U)
#define GPIO_RWM0CR_MODER14_Pos     GPIO_RWM0CR_RW14_DPos
#define GPIO_RWM0CR_MODER14_Msk     (0x03UL << GPIO_RWM0CR_MODER14_Pos)         /* 0x30000000 */
#define GPIO_RWM0CR_MODER14         GPIO_RWM0CR_MODER14_Msk                     /* RW14[1:0] bits (Control the read and write mode of Channel 14) */
#define GPIO_RWM0CR_MODER14_0       (0x01UL << GPIO_RWM0CR_MODER14_Pos)         /* 0x10000000 */
#define GPIO_RWM0CR_MODER14_1       (0x02UL << GPIO_RWM0CR_MODER14_Pos)         /* 0x20000000 */

#define GPIO_RWM0CR_RW15_DPos       (30U)
#define GPIO_RWM0CR_MODER15_Pos     GPIO_RWM0CR_RW15_DPos
#define GPIO_RWM0CR_MODER15_Msk     (0x03UL << GPIO_RWM0CR_MODER15_Pos)         /* 0xC0000000 */
#define GPIO_RWM0CR_MODER15         GPIO_RWM0CR_MODER15_Msk                     /* RW15[1:0] bits (Control the read and write mode of Channel 15) */
#define GPIO_RWM0CR_MODER15_0       (0x01UL << GPIO_RWM0CR_MODER15_Pos)         /* 0x40000000 */
#define GPIO_RWM0CR_MODER15_1       (0x02UL << GPIO_RWM0CR_MODER15_Pos)         /* 0x80000000 */


/******************  Bits definition for GPIO_IM0CR register  *****************/
#define GPIO_IM0CR_iMode0_DPos      (0U)
#define GPIO_IM0CR_PUPDR0_Pos       GPIO_IM0CR_iMode0_DPos
#define GPIO_IM0CR_PUPDR0_Msk       (0x03UL << GPIO_IM0CR_PUPDR0_Pos)           /* 0x00000003 */
#define GPIO_IM0CR_PUPDR0           GPIO_IM0CR_PUPDR0_Msk                       /* iMode[1:0] bits (Channel 0 input mode control) */
#define GPIO_IM0CR_PUPDR0_0         (0x01UL << GPIO_IM0CR_PUPDR0_Pos)           /* 0x00000001 */
#define GPIO_IM0CR_PUPDR0_1         (0x02UL << GPIO_IM0CR_PUPDR0_Pos)           /* 0x00000002 */

#define GPIO_IM0CR_iMode1_DPos      (2U)
#define GPIO_IM0CR_PUPDR1_Pos       GPIO_IM0CR_iMode1_DPos                                           
#define GPIO_IM0CR_PUPDR1_Msk       (0x03UL << GPIO_IM0CR_PUPDR1_Pos)           /* 0x0000000C */
#define GPIO_IM0CR_PUPDR1           GPIO_IM0CR_PUPDR1_Msk                       /* iMode1[1:0] bits (Channel 1 input mode control) */
#define GPIO_IM0CR_PUPDR1_0         (0x01UL << GPIO_IM0CR_PUPDR1_Pos)           /* 0x00000004 */
#define GPIO_IM0CR_PUPDR1_1         (0x02UL << GPIO_IM0CR_PUPDR1_Pos)           /* 0x00000008 */

#define GPIO_IM0CR_iMode2_DPos      (4U)
#define GPIO_IM0CR_PUPDR2_Pos       GPIO_IM0CR_iMode2_DPos
#define GPIO_IM0CR_PUPDR2_Msk       (0x03UL << GPIO_IM0CR_PUPDR2_Pos)           /* 0x00000030 */
#define GPIO_IM0CR_PUPDR2           GPIO_IM0CR_PUPDR2_Msk                       /* iMode2[1:0] bits (Channel 2 input mode control) */
#define GPIO_IM0CR_PUPDR2_0         (0x01UL << GPIO_IM0CR_PUPDR2_Pos)           /* 0x00000010 */
#define GPIO_IM0CR_PUPDR2_1         (0x02UL << GPIO_IM0CR_PUPDR2_Pos)           /* 0x00000020 */

#define GPIO_IM0CR_iMode3_DPos      (6U)
#define GPIO_IM0CR_PUPDR3_Pos       GPIO_IM0CR_iMode3_DPos
#define GPIO_IM0CR_PUPDR3_Msk       (0x03UL << GPIO_IM0CR_PUPDR3_Pos)           /* 0x000000C0 */
#define GPIO_IM0CR_PUPDR3           GPIO_IM0CR_PUPDR3_Msk                       /* iMode3[1:0] bits (Channel 3 input mode control) */
#define GPIO_IM0CR_PUPDR3_0         (0x01UL << GPIO_IM0CR_PUPDR3_Pos)           /* 0x00000040 */
#define GPIO_IM0CR_PUPDR3_1         (0x02UL << GPIO_IM0CR_PUPDR3_Pos)           /* 0x00000080 */

#define GPIO_IM0CR_iMode4_DPos      (8U)
#define GPIO_IM0CR_PUPDR4_Pos       GPIO_IM0CR_iMode4_DPos
#define GPIO_IM0CR_PUPDR4_Msk       (0x03UL << GPIO_IM0CR_PUPDR4_Pos)           /* 0x00000300 */
#define GPIO_IM0CR_PUPDR4           GPIO_IM0CR_PUPDR4_Msk                       /* iMode4[1:0] bits (Channel 4 input mode control) */
#define GPIO_IM0CR_PUPDR4_0         (0x01UL << GPIO_IM0CR_PUPDR4_Pos)           /* 0x00000100 */
#define GPIO_IM0CR_PUPDR4_1         (0x02UL << GPIO_IM0CR_PUPDR4_Pos)           /* 0x00000200 */

#define GPIO_IM0CR_iMode5_DPos      (10U)
#define GPIO_IM0CR_PUPDR5_Pos       GPIO_IM0CR_iMode5_DPos
#define GPIO_IM0CR_PUPDR5_Msk       (0x03UL << GPIO_IM0CR_PUPDR5_Pos)           /* 0x00000C00 */
#define GPIO_IM0CR_PUPDR5           GPIO_IM0CR_PUPDR5_Msk                       /* iMode5[1:0] bits (Channel 5 input mode control) */
#define GPIO_IM0CR_PUPDR5_0         (0x01UL << GPIO_IM0CR_PUPDR5_Pos)           /* 0x00000400 */
#define GPIO_IM0CR_PUPDR5_1         (0x02UL << GPIO_IM0CR_PUPDR5_Pos)           /* 0x00000800 */

#define GPIO_IM0CR_iMode6_DPos      (12U)
#define GPIO_IM0CR_PUPDR6_Pos       GPIO_IM0CR_iMode6_DPos
#define GPIO_IM0CR_PUPDR6_Msk       (0x03UL << GPIO_IM0CR_PUPDR6_Pos)           /* 0x00003000 */
#define GPIO_IM0CR_PUPDR6           GPIO_IM0CR_PUPDR6_Msk                       /* iMode6[1:0] bits (Channel 6 input mode control) */
#define GPIO_IM0CR_PUPDR6_0         (0x01UL << GPIO_IM0CR_PUPDR6_Pos)           /* 0x00001000 */
#define GPIO_IM0CR_PUPDR6_1         (0x02UL << GPIO_IM0CR_PUPDR6_Pos)           /* 0x00002000 */

#define GPIO_IM0CR_iMode7_DPos      (14U)
#define GPIO_IM0CR_PUPDR7_Pos       GPIO_IM0CR_iMode7_DPos
#define GPIO_IM0CR_PUPDR7_Msk       (0x03UL << GPIO_IM0CR_PUPDR7_Pos)           /* 0x0000C000 */
#define GPIO_IM0CR_PUPDR7           GPIO_IM0CR_PUPDR7_Msk                       /* iMode7[1:0] bits (Channel 7 input mode control) */
#define GPIO_IM0CR_PUPDR7_0         (0x01UL << GPIO_IM0CR_PUPDR7_Pos)           /* 0x00004000 */
#define GPIO_IM0CR_PUPDR7_1         (0x02UL << GPIO_IM0CR_PUPDR7_Pos)           /* 0x00008000 */

#define GPIO_IM0CR_iMode8_DPos      (16U)
#define GPIO_IM0CR_PUPDR8_Pos       GPIO_IM0CR_iMode8_DPos
#define GPIO_IM0CR_PUPDR8_Msk       (0x03UL << GPIO_IM0CR_PUPDR8_Pos)           /* 0x00030000 */
#define GPIO_IM0CR_PUPDR8           GPIO_IM0CR_PUPDR8_Msk                       /* iMode8[1:0] bits (Channel 8 input mode control) */
#define GPIO_IM0CR_PUPDR8_0         (0x01UL << GPIO_IM0CR_PUPDR8_Pos)           /* 0x00010000 */
#define GPIO_IM0CR_PUPDR8_1         (0x02UL << GPIO_IM0CR_PUPDR8_Pos)           /* 0x00020000 */

#define GPIO_IM0CR_iMode9_DPos      (18U)
#define GPIO_IM0CR_PUPDR9_Pos       GPIO_IM0CR_iMode9_DPos
#define GPIO_IM0CR_PUPDR9_Msk       (0x03UL << GPIO_IM0CR_PUPDR9_Pos)           /* 0x000C0000 */
#define GPIO_IM0CR_PUPDR9           GPIO_IM0CR_PUPDR9_Msk                       /* iMode9[1:0] bits (Channel 9 input mode control) */
#define GPIO_IM0CR_PUPDR9_0         (0x01UL << GPIO_IM0CR_PUPDR9_Pos)           /* 0x00040000 */
#define GPIO_IM0CR_PUPDR9_1         (0x02UL << GPIO_IM0CR_PUPDR9_Pos)           /* 0x00080000 */

#define GPIO_IM0CR_iMode10_DPos     (20U)
#define GPIO_IM0CR_PUPDR10_Pos      GPIO_IM0CR_iMode10_DPos
#define GPIO_IM0CR_PUPDR10_Msk      (0x03UL << GPIO_IM0CR_PUPDR10_Pos)          /* 0x00300000 */
#define GPIO_IM0CR_PUPDR10          GPIO_IM0CR_PUPDR10_Msk                      /* iMode10[1:0] bits (Channel 10 input mode control) */
#define GPIO_IM0CR_PUPDR10_0        (0x01UL << GPIO_IM0CR_PUPDR10_Pos)          /* 0x00100000 */
#define GPIO_IM0CR_PUPDR10_1        (0x02UL << GPIO_IM0CR_PUPDR10_Pos)          /* 0x00200000 */

#define GPIO_IM0CR_iMode11_DPos     (22U)
#define GPIO_IM0CR_PUPDR11_Pos      GPIO_IM0CR_iMode11_DPos
#define GPIO_IM0CR_PUPDR11_Msk      (0x03UL << GPIO_IM0CR_PUPDR11_Pos)          /* 0x00C00000 */
#define GPIO_IM0CR_PUPDR11          GPIO_IM0CR_PUPDR11_Msk                      /* iMode11[1:0] bits (Channel 11 input mode control) */
#define GPIO_IM0CR_PUPDR11_0        (0x01UL << GPIO_IM0CR_PUPDR11_Pos)          /* 0x00400000 */
#define GPIO_IM0CR_PUPDR11_1        (0x02UL << GPIO_IM0CR_PUPDR11_Pos)          /* 0x00800000 */

#define GPIO_IM0CR_iMode12_DPos     (24U)
#define GPIO_IM0CR_PUPDR12_Pos      GPIO_IM0CR_iMode12_DPos
#define GPIO_IM0CR_PUPDR12_Msk      (0x03UL << GPIO_IM0CR_PUPDR12_Pos)          /* 0x03000000 */
#define GPIO_IM0CR_PUPDR12          GPIO_IM0CR_PUPDR12_Msk                      /* iMode12[1:0] bits (Channel 12 input mode control) */
#define GPIO_IM0CR_PUPDR12_0        (0x01UL << GPIO_IM0CR_PUPDR12_Pos)          /* 0x01000000 */
#define GPIO_IM0CR_PUPDR12_1        (0x02UL << GPIO_IM0CR_PUPDR12_Pos)          /* 0x02000000 */

#define GPIO_IM0CR_iMode13_DPos     (26U)
#define GPIO_IM0CR_PUPDR13_Pos      GPIO_IM0CR_iMode13_DPos
#define GPIO_IM0CR_PUPDR13_Msk      (0x03UL << GPIO_IM0CR_PUPDR13_Pos)          /* 0x0C000000 */
#define GPIO_IM0CR_PUPDR13          GPIO_IM0CR_PUPDR13_Msk                      /* iMode13[1:0] bits (Channel 13 input mode control) */
#define GPIO_IM0CR_PUPDR13_0        (0x01UL << GPIO_IM0CR_PUPDR13_Pos)          /* 0x04000000 */
#define GPIO_IM0CR_PUPDR13_1        (0x02UL << GPIO_IM0CR_PUPDR13_Pos)          /* 0x08000000 */

#define GPIO_IM0CR_iMode14_DPos     (28U)
#define GPIO_IM0CR_PUPDR14_Pos      GPIO_IM0CR_iMode14_DPos
#define GPIO_IM0CR_PUPDR14_Msk      (0x03UL << GPIO_IM0CR_PUPDR14_Pos)          /* 0x30000000 */
#define GPIO_IM0CR_PUPDR14          GPIO_IM0CR_PUPDR14_Msk                      /* iMode14[1:0] bits (Channel 14 input mode control) */
#define GPIO_IM0CR_PUPDR14_0        (0x01UL << GPIO_IM0CR_PUPDR14_Pos)          /* 0x10000000 */
#define GPIO_IM0CR_PUPDR14_1        (0x02UL << GPIO_IM0CR_PUPDR14_Pos)          /* 0x20000000 */

#define GPIO_IM0CR_iMode15_DPos     (30U)
#define GPIO_IM0CR_PUPDR15_Pos      GPIO_IM0CR_iMode15_DPos
#define GPIO_IM0CR_PUPDR15_Msk      (0x03UL << GPIO_IM0CR_PUPDR15_Pos)          /* 0xC0000000 */
#define GPIO_IM0CR_PUPDR15          GPIO_IM0CR_PUPDR15_Msk                      /* iMode15[1:0] bits (Channel 15 input mode control) */
#define GPIO_IM0CR_PUPDR15_0        (0x01UL << GPIO_IM0CR_PUPDR15_Pos)          /* 0x40000000 */
#define GPIO_IM0CR_PUPDR15_1        (0x02UL << GPIO_IM0CR_PUPDR15_Pos)          /* 0x80000000 */


/******************  Bits definition for GPIO_OMCR register  *****************/
#define GPIO_OMCR_oMode0_DPos       (0U)
#define GPIO_OMCR_OT0_Pos           GPIO_OMCR_oMode0_DPos
#define GPIO_OMCR_OT0_Msk           (0x01UL << GPIO_OMCR_OT0_Pos)               /* 0x00000001 */
#define GPIO_OMCR_OT0               GPIO_OMCR_OT0_Msk                           /* Channel 0 output mode control */

#define GPIO_OMCR_oMode1_DPos       (1U)
#define GPIO_OMCR_OT1_Pos           GPIO_OMCR_oMode1_DPos
#define GPIO_OMCR_OT1_Msk           (0x01UL << GPIO_OMCR_OT1_Pos)               /* 0x00000002 */
#define GPIO_OMCR_OT1               GPIO_OMCR_OT1_Msk                           /* Channel 1 output mode control */

#define GPIO_OMCR_oMode2_DPos       (2U)
#define GPIO_OMCR_OT2_Pos           GPIO_OMCR_oMode2_DPos
#define GPIO_OMCR_OT2_Msk           (0x01UL << GPIO_OMCR_OT2_Pos)               /* 0x00000004 */
#define GPIO_OMCR_OT2               GPIO_OMCR_OT2_Msk                           /* Channel 2 output mode control */

#define GPIO_OMCR_oMode3_DPos       (3U)
#define GPIO_OMCR_OT3_Pos           GPIO_OMCR_oMode3_DPos
#define GPIO_OMCR_OT3_Msk           (0x01UL << GPIO_OMCR_OT3_Pos)               /* 0x00000008 */
#define GPIO_OMCR_OT3               GPIO_OMCR_OT3_Msk                           /* Channel 3 output mode control */

#define GPIO_OMCR_oMode4_DPos       (4U)
#define GPIO_OMCR_OT4_Pos           GPIO_OMCR_oMode4_DPos
#define GPIO_OMCR_OT4_Msk           (0x01UL << GPIO_OMCR_OT4_Pos)               /* 0x00000010 */
#define GPIO_OMCR_OT4               GPIO_OMCR_OT4_Msk                           /* Channel 4 output mode control */

#define GPIO_OMCR_oMode5_DPos       (5U)
#define GPIO_OMCR_OT5_Pos           GPIO_OMCR_oMode5_DPos
#define GPIO_OMCR_OT5_Msk           (0x01UL << GPIO_OMCR_OT5_Pos)               /* 0x00000020 */
#define GPIO_OMCR_OT5               GPIO_OMCR_OT5_Msk                           /* Channel 5 output mode control */

#define GPIO_OMCR_oMode6_DPos       (6U)
#define GPIO_OMCR_OT6_Pos           GPIO_OMCR_oMode6_DPos
#define GPIO_OMCR_OT6_Msk           (0x01UL << GPIO_OMCR_OT6_Pos)               /* 0x00000040 */
#define GPIO_OMCR_OT6               GPIO_OMCR_OT6_Msk                           /* Channel 6 output mode control */

#define GPIO_OMCR_oMode7_DPos       (7U)
#define GPIO_OMCR_OT7_Pos           GPIO_OMCR_oMode7_DPos
#define GPIO_OMCR_OT7_Msk           (0x01UL << GPIO_OMCR_OT7_Pos)               /* 0x00000080 */
#define GPIO_OMCR_OT7               GPIO_OMCR_OT7_Msk                           /* Channel 7 output mode control */

#define GPIO_OMCR_oMode8_DPos       (8U)
#define GPIO_OMCR_OT8_Pos           GPIO_OMCR_oMode8_DPos
#define GPIO_OMCR_OT8_Msk           (0x01UL << GPIO_OMCR_OT8_Pos)               /* 0x00000100 */
#define GPIO_OMCR_OT8               GPIO_OMCR_OT8_Msk                           /* Channel 8 output mode control */

#define GPIO_OMCR_oMode9_DPos       (9U)
#define GPIO_OMCR_OT9_Pos           GPIO_OMCR_oMode9_DPos
#define GPIO_OMCR_OT9_Msk           (0x01UL << GPIO_OMCR_OT9_Pos)               /* 0x00000200 */
#define GPIO_OMCR_OT9               GPIO_OMCR_OT9_Msk                           /* Channel 9 output mode control */

#define GPIO_OMCR_oMode10_DPos      (10U)
#define GPIO_OMCR_OT10_Pos          GPIO_OMCR_oMode10_DPos
#define GPIO_OMCR_OT10_Msk          (0x01UL << GPIO_OMCR_OT10_Pos)              /* 0x00000400 */
#define GPIO_OMCR_OT10              GPIO_OMCR_OT10_Msk                          /* Channel 10 output mode control */

#define GPIO_OMCR_oMode11_DPos      (11U)
#define GPIO_OMCR_OT11_Pos          GPIO_OMCR_oMode11_DPos
#define GPIO_OMCR_OT11_Msk          (0x01UL << GPIO_OMCR_OT11_Pos)              /* 0x00000800 */
#define GPIO_OMCR_OT11              GPIO_OMCR_OT11_Msk                          /* Channel 11 output mode control */

#define GPIO_OMCR_oMode12_DPos      (12U)
#define GPIO_OMCR_OT12_Pos          GPIO_OMCR_oMode12_DPos
#define GPIO_OMCR_OT12_Msk          (0x01UL << GPIO_OMCR_OT12_Pos)              /* 0x00001000 */
#define GPIO_OMCR_OT12              GPIO_OMCR_OT12_Msk                          /* Channel 12 output mode control */

#define GPIO_OMCR_oMode13_DPos      (13U)
#define GPIO_OMCR_OT13_Pos          GPIO_OMCR_oMode13_DPos
#define GPIO_OMCR_OT13_Msk          (0x01UL << GPIO_OMCR_OT13_Pos)              /* 0x00002000 */
#define GPIO_OMCR_OT13              GPIO_OMCR_OT13_Msk                          /* Channel 13 output mode control */

#define GPIO_OMCR_oMode14_DPos      (14U)
#define GPIO_OMCR_OT14_Pos          GPIO_OMCR_oMode14_DPos
#define GPIO_OMCR_OT14_Msk          (0x01UL << GPIO_OMCR_OT14_Pos)              /* 0x00004000 */
#define GPIO_OMCR_OT14              GPIO_OMCR_OT14_Msk                          /* Channel 14 output mode control */

#define GPIO_OMCR_oMode15_DPos      (15U)
#define GPIO_OMCR_OT15_Pos          GPIO_OMCR_oMode15_DPos
#define GPIO_OMCR_OT15_Msk          (0x01UL << GPIO_OMCR_OT15_Pos)              /* 0x00008000 */
#define GPIO_OMCR_OT15              GPIO_OMCR_OT15_Msk                          /* Channel 15 output mode control */


/******************  Bits definition for GPIO_OICR register  *****************/
#define GPIO_OICR_oStrength0_DPos   (0U)
#define GPIO_OICR_OSTRENGTH0_Pos    GPIO_OICR_oStrength0_DPos
#define GPIO_OICR_OSTRENGTH0_Msk    (0x03UL << GPIO_OICR_OSTRENGTH0_Pos)        /* 0x00000003 */
#define GPIO_OICR_OSTRENGTH0        GPIO_OICR_OSTRENGTH0_Msk                    /* oStrength0[1:0] bits (Channel 0 output strength control) */
#define GPIO_OICR_OSTRENGTH0_0      (0x01UL << GPIO_OICR_OSTRENGTH0_Pos)        /* 0x00000001 */
#define GPIO_OICR_OSTRENGTH0_1      (0x02UL << GPIO_OICR_OSTRENGTH0_Pos)        /* 0x00000002 */

#define GPIO_OICR_oStrength1_DPos   (2U)
#define GPIO_OICR_OSTRENGTH1_Pos    GPIO_OICR_oStrength1_DPos
#define GPIO_OICR_OSTRENGTH1_Msk    (0x03UL << GPIO_OICR_OSTRENGTH1_Pos)        /* 0x0000000C */
#define GPIO_OICR_OSTRENGTH1        GPIO_OICR_OSTRENGTH1_Msk                    /* oStrength1[1:0] bits (Channel 1 output strength control) */
#define GPIO_OICR_OSTRENGTH1_0      (0x01UL << GPIO_OICR_OSTRENGTH1_Pos)        /* 0x00000004 */
#define GPIO_OICR_OSTRENGTH1_1      (0x02UL << GPIO_OICR_OSTRENGTH1_Pos)        /* 0x00000008 */

#define GPIO_OICR_oStrength2_DPos   (4U)
#define GPIO_OICR_OSTRENGTH2_Pos    GPIO_OICR_oStrength2_DPos
#define GPIO_OICR_OSTRENGTH2_Msk    (0x03UL << GPIO_OICR_OSTRENGTH2_Pos)        /* 0x00000030 */
#define GPIO_OICR_OSTRENGTH2        GPIO_OICR_OSTRENGTH2_Msk                    /* oStrength2[1:0] bits (Channel 2 output strength control) */
#define GPIO_OICR_OSTRENGTH2_0      (0x01UL << GPIO_OICR_OSTRENGTH2_Pos)        /* 0x00000010 */
#define GPIO_OICR_OSTRENGTH2_1      (0x02UL << GPIO_OICR_OSTRENGTH2_Pos)        /* 0x00000020 */

#define GPIO_OICR_oStrength3_DPos   (6U)
#define GPIO_OICR_OSTRENGTH3_Pos    GPIO_OICR_oStrength3_DPos
#define GPIO_OICR_OSTRENGTH3_Msk    (0x03UL << GPIO_OICR_OSTRENGTH3_Pos)        /* 0x000000C0 */
#define GPIO_OICR_OSTRENGTH3        GPIO_OICR_OSTRENGTH3_Msk                    /* oStrength3[1:0] bits (Channel 3 output strength control) */
#define GPIO_OICR_OSTRENGTH3_0      (0x01UL << GPIO_OICR_OSTRENGTH3_Pos)        /* 0x00000040 */
#define GPIO_OICR_OSTRENGTH3_1      (0x02UL << GPIO_OICR_OSTRENGTH3_Pos)        /* 0x00000080 */

#define GPIO_OICR_oStrength4_DPos   (8U)
#define GPIO_OICR_OSTRENGTH4_Pos    GPIO_OICR_oStrength4_DPos
#define GPIO_OICR_OSTRENGTH4_Msk    (0x03UL << GPIO_OICR_OSTRENGTH4_Pos)        /* 0x00000300 */
#define GPIO_OICR_OSTRENGTH4        GPIO_OICR_OSTRENGTH4_Msk                    /* oStrength4[1:0] bits (Channel 4 output strength control) */
#define GPIO_OICR_OSTRENGTH4_0      (0x01UL << GPIO_OICR_OSTRENGTH4_Pos)        /* 0x00000100 */
#define GPIO_OICR_OSTRENGTH4_1      (0x02UL << GPIO_OICR_OSTRENGTH4_Pos)        /* 0x00000200 */

#define GPIO_OICR_oStrength5_DPos   (10U)
#define GPIO_OICR_OSTRENGTH5_Pos    GPIO_OICR_oStrength5_DPos
#define GPIO_OICR_OSTRENGTH5_Msk    (0x03UL << GPIO_OICR_OSTRENGTH5_Pos)        /* 0x00000C00 */
#define GPIO_OICR_OSTRENGTH5        GPIO_OICR_OSTRENGTH5_Msk                    /* oStrength5[1:0] bits (Channel 5 output strength control) */
#define GPIO_OICR_OSTRENGTH5_0      (0x01UL << GPIO_OICR_OSTRENGTH5_Pos)        /* 0x00000400 */
#define GPIO_OICR_OSTRENGTH5_1      (0x02UL << GPIO_OICR_OSTRENGTH5_Pos)        /* 0x00000800 */

#define GPIO_OICR_oStrength6_DPos   (12U)
#define GPIO_OICR_OSTRENGTH6_Pos    GPIO_OICR_oStrength6_DPos
#define GPIO_OICR_OSTRENGTH6_Msk    (0x03UL << GPIO_OICR_OSTRENGTH6_Pos)        /* 0x00003000 */
#define GPIO_OICR_OSTRENGTH6        GPIO_OICR_OSTRENGTH6_Msk                    /* oStrength6[1:0] bits (Channel 6 output strength control) */
#define GPIO_OICR_OSTRENGTH6_0      (0x01UL << GPIO_OICR_OSTRENGTH6_Pos)        /* 0x00001000 */
#define GPIO_OICR_OSTRENGTH6_1      (0x02UL << GPIO_OICR_OSTRENGTH6_Pos)        /* 0x00002000 */

#define GPIO_OICR_oStrength7_DPos   (14U)
#define GPIO_OICR_OSTRENGTH7_Pos    GPIO_OICR_oStrength7_DPos
#define GPIO_OICR_OSTRENGTH7_Msk    (0x03UL << GPIO_OICR_OSTRENGTH7_Pos)        /* 0x0000C000 */
#define GPIO_OICR_OSTRENGTH7        GPIO_OICR_OSTRENGTH7_Msk                    /* oStrength7[1:0] bits (Channel 7 output strength control) */
#define GPIO_OICR_OSTRENGTH7_0      (0x01UL << GPIO_OICR_OSTRENGTH7_Pos)        /* 0x00004000 */
#define GPIO_OICR_OSTRENGTH7_1      (0x02UL << GPIO_OICR_OSTRENGTH7_Pos)        /* 0x00008000 */

#define GPIO_OICR_oStrength8_DPos   (16U)
#define GPIO_OICR_OSTRENGTH8_Pos    GPIO_OICR_oStrength8_DPos
#define GPIO_OICR_OSTRENGTH8_Msk    (0x03UL << GPIO_OICR_OSTRENGTH8_Pos)        /* 0x00030000 */
#define GPIO_OICR_OSTRENGTH8        GPIO_OICR_OSTRENGTH8_Msk                    /* oStrength8[1:0] bits (Channel 8 output strength control) */
#define GPIO_OICR_OSTRENGTH8_0      (0x01UL << GPIO_OICR_OSTRENGTH8_Pos)        /* 0x00010000 */
#define GPIO_OICR_OSTRENGTH8_1      (0x02UL << GPIO_OICR_OSTRENGTH8_Pos)        /* 0x00020000 */

#define GPIO_OICR_oStrength9_DPos   (18U)
#define GPIO_OICR_OSTRENGTH9_Pos    GPIO_OICR_oStrength9_DPos
#define GPIO_OICR_OSTRENGTH9_Msk    (0x03UL << GPIO_OICR_OSTRENGTH9_Pos)        /* 0x000C0000 */
#define GPIO_OICR_OSTRENGTH9        GPIO_OICR_OSTRENGTH9_Msk                    /* oStrength9[1:0] bits (Channel 9 output strength control) */
#define GPIO_OICR_OSTRENGTH9_0      (0x01UL << GPIO_OICR_OSTRENGTH9_Pos)        /* 0x00040000 */
#define GPIO_OICR_OSTRENGTH9_1      (0x02UL << GPIO_OICR_OSTRENGTH9_Pos)        /* 0x00080000 */

#define GPIO_OICR_oStrength10_DPos  (20U)
#define GPIO_OICR_OSTRENGTH10_Pos   GPIO_OICR_oStrength10_DPos
#define GPIO_OICR_OSTRENGTH10_Msk   (0x03UL << GPIO_OICR_OSTRENGTH10_Pos)       /* 0x00300000 */
#define GPIO_OICR_OSTRENGTH10       GPIO_OICR_OSTRENGTH10_Msk                   /* oStrength10[1:0] bits (Channel 10 output strength control) */
#define GPIO_OICR_OSTRENGTH10_0     (0x01UL << GPIO_OICR_OSTRENGTH10_Pos)       /* 0x00100000 */
#define GPIO_OICR_OSTRENGTH10_1     (0x02UL << GPIO_OICR_OSTRENGTH10_Pos)       /* 0x00200000 */

#define GPIO_OICR_oStrength11_DPos  (22U)
#define GPIO_OICR_OSTRENGTH11_Pos   GPIO_OICR_oStrength11_DPos
#define GPIO_OICR_OSTRENGTH11_Msk   (0x03UL << GPIO_OICR_OSTRENGTH11_Pos)       /* 0x00C00000 */
#define GPIO_OICR_OSTRENGTH11       GPIO_OICR_OSTRENGTH11_Msk                   /* oStrength11[1:0] bits (Channel 11 output strength control) */
#define GPIO_OICR_OSTRENGTH11_0     (0x01UL << GPIO_OICR_OSTRENGTH11_Pos)       /* 0x00400000 */
#define GPIO_OICR_OSTRENGTH11_1     (0x02UL << GPIO_OICR_OSTRENGTH11_Pos)       /* 0x00800000 */

#define GPIO_OICR_oStrength12_DPos  (24U)
#define GPIO_OICR_OSTRENGTH12_Pos   GPIO_OICR_oStrength12_DPos
#define GPIO_OICR_OSTRENGTH12_Msk   (0x03UL << GPIO_OICR_OSTRENGTH12_Pos)       /* 0x03000000 */
#define GPIO_OICR_OSTRENGTH12       GPIO_OICR_OSTRENGTH12_Msk                   /* oStrength12[1:0] bits (Channel 12 output strength control) */
#define GPIO_OICR_OSTRENGTH12_0     (0x01UL << GPIO_OICR_OSTRENGTH12_Pos)       /* 0x01000000 */
#define GPIO_OICR_OSTRENGTH12_1     (0x02UL << GPIO_OICR_OSTRENGTH12_Pos)       /* 0x02000000 */

#define GPIO_OICR_oStrength13_DPos  (26U)
#define GPIO_OICR_OSTRENGTH13_Pos   GPIO_OICR_oStrength13_DPos
#define GPIO_OICR_OSTRENGTH13_Msk   (0x03UL << GPIO_OICR_OSTRENGTH13_Pos)       /* 0x0C000000 */
#define GPIO_OICR_OSTRENGTH13       GPIO_OICR_OSTRENGTH13_Msk                   /* oStrength13[1:0] bits (Channel 12 output strength control) */
#define GPIO_OICR_OSTRENGTH13_0     (0x01UL << GPIO_OICR_OSTRENGTH13_Pos)       /* 0x04000000 */
#define GPIO_OICR_OSTRENGTH13_1     (0x02UL << GPIO_OICR_OSTRENGTH13_Pos)       /* 0x08000000 */

#define GPIO_OICR_oStrength14_DPos  (28U)
#define GPIO_OICR_OSTRENGTH14_Pos   GPIO_OICR_oStrength14_DPos
#define GPIO_OICR_OSTRENGTH14_Msk   (0x03UL << GPIO_OICR_OSTRENGTH14_Pos)       /* 0x30000000 */
#define GPIO_OICR_OSTRENGTH14       GPIO_OICR_OSTRENGTH14_Msk                   /* oStrength14[1:0] bits (Channel 14 output strength control) */
#define GPIO_OICR_OSTRENGTH14_0     (0x01UL << GPIO_OICR_OSTRENGTH14_Pos)       /* 0x10000000 */
#define GPIO_OICR_OSTRENGTH14_1     (0x02UL << GPIO_OICR_OSTRENGTH14_Pos)       /* 0x20000000 */

#define GPIO_OICR_oStrength15_DPos  (30U)
#define GPIO_OICR_OSTRENGTH15_Pos   GPIO_OICR_oStrength15_DPos
#define GPIO_OICR_OSTRENGTH15_Msk   (0x03UL << GPIO_OICR_OSTRENGTH15_Pos)       /* 0xC0000000 */
#define GPIO_OICR_OSTRENGTH15       GPIO_OICR_OSTRENGTH15_Msk                   /* oStrength15[1:0] bits (Channel 15 output strength control) */
#define GPIO_OICR_OSTRENGTH15_0     (0x01UL << GPIO_OICR_OSTRENGTH15_Pos)       /* 0x40000000 */
#define GPIO_OICR_OSTRENGTH15_1     (0x02UL << GPIO_OICR_OSTRENGTH15_Pos)       /* 0x80000000 */


/******************  Bits definition for GPIO_MI0SR register  *****************/
#define GPIO_MI0SR_perpMux0_DPos    (0U)
#define GPIO_MI0SR_AFR0_Pos         GPIO_MI0SR_perpMux0_DPos
#define GPIO_MI0SR_AFR0_Msk         (0x07UL << GPIO_MI0SR_AFR0_Pos)             /* 0x00000007 */
#define GPIO_MI0SR_AFR0             GPIO_MI0SR_AFR0_Msk                         /* perpMux0[2:0] bits (Channel 0 multiplexing interface selection) */
#define GPIO_MI0SR_AFR0_0           (0x01UL << GPIO_MI0SR_AFR0_Pos)             /* 0x00000001 */
#define GPIO_MI0SR_AFR0_1           (0x02UL << GPIO_MI0SR_AFR0_Pos)             /* 0x00000002 */
#define GPIO_MI0SR_AFR0_2           (0x03UL << GPIO_MI0SR_AFR0_Pos)             /* 0x00000004 */

#define GPIO_MI0SR_perpMux1_DPos    (3U)
#define GPIO_MI0SR_AFR1_Pos         GPIO_MI0SR_perpMux1_DPos
#define GPIO_MI0SR_AFR1_Msk         (0x07UL << GPIO_MI0SR_AFR1_Pos)             /* 0x00000038 */
#define GPIO_MI0SR_AFR1             GPIO_MI0SR_AFR1_Msk                         /* perpMux1[2:0] bits (Channel 1 multiplexing interface selection) */
#define GPIO_MI0SR_AFR1_0           (0x01UL << GPIO_MI0SR_AFR1_Pos)             /* 0x00000008 */
#define GPIO_MI0SR_AFR1_1           (0x02UL << GPIO_MI0SR_AFR1_Pos)             /* 0x00000010 */
#define GPIO_MI0SR_AFR1_2           (0x03UL << GPIO_MI0SR_AFR1_Pos)             /* 0x00000020 */

#define GPIO_MI0SR_perpMux2_DPos    (6U)
#define GPIO_MI0SR_AFR2_Pos         GPIO_MI0SR_perpMux2_DPos
#define GPIO_MI0SR_AFR2_Msk         (0x07UL << GPIO_MI0SR_AFR2_Pos)             /* 0x000001C0 */
#define GPIO_MI0SR_AFR2             GPIO_MI0SR_AFR2_Msk                         /* perpMux2[2:0] bits (Channel 2 multiplexing interface selection) */
#define GPIO_MI0SR_AFR2_0           (0x01UL << GPIO_MI0SR_AFR2_Pos)             /* 0x00000040 */
#define GPIO_MI0SR_AFR2_1           (0x02UL << GPIO_MI0SR_AFR2_Pos)             /* 0x00000080 */
#define GPIO_MI0SR_AFR2_2           (0x03UL << GPIO_MI0SR_AFR2_Pos)             /* 0x00000100 */

#define GPIO_MI0SR_perpMux3_DPos    (9U)
#define GPIO_MI0SR_AFR3_Pos         GPIO_MI0SR_perpMux3_DPos
#define GPIO_MI0SR_AFR3_Msk         (0x07UL << GPIO_MI0SR_AFR3_Pos)             /* 0x00000E00 */
#define GPIO_MI0SR_AFR3             GPIO_MI0SR_AFR3_Msk                         /* perpMux3[2:0] bits (Channel 3 multiplexing interface selection) */
#define GPIO_MI0SR_AFR3_0           (0x01UL << GPIO_MI0SR_AFR3_Pos)             /* 0x00000200 */
#define GPIO_MI0SR_AFR3_1           (0x02UL << GPIO_MI0SR_AFR3_Pos)             /* 0x00000400 */
#define GPIO_MI0SR_AFR3_2           (0x03UL << GPIO_MI0SR_AFR3_Pos)             /* 0x00000800 */

#define GPIO_MI0SR_perpMux4_DPos    (12U)
#define GPIO_MI0SR_AFR4_Pos         GPIO_MI0SR_perpMux4_DPos
#define GPIO_MI0SR_AFR4_Msk         (0x07UL << GPIO_MI0SR_AFR4_Pos)             /* 0x00007400 */
#define GPIO_MI0SR_AFR4             GPIO_MI0SR_AFR4_Msk                         /* perpMux4[2:0] bits (Channel 4 multiplexing interface selection) */
#define GPIO_MI0SR_AFR4_0           (0x01UL << GPIO_MI0SR_AFR4_Pos)             /* 0x00001000 */
#define GPIO_MI0SR_AFR4_1           (0x02UL << GPIO_MI0SR_AFR4_Pos)             /* 0x00002000 */
#define GPIO_MI0SR_AFR4_2           (0x03UL << GPIO_MI0SR_AFR4_Pos)             /* 0x00004000 */

#define GPIO_MI0SR_perpMux5_DPos    (15U)
#define GPIO_MI0SR_AFR5_Pos         GPIO_MI0SR_perpMux5_DPos
#define GPIO_MI0SR_AFR5_Msk         (0x07UL << GPIO_MI0SR_AFR5_Pos)             /* 0x000B0000 */
#define GPIO_MI0SR_AFR5             GPIO_MI0SR_AFR5_Msk                         /* perpMux5[2:0] bits (Channel 5 multiplexing interface selection) */
#define GPIO_MI0SR_AFR5_0           (0x01UL << GPIO_MI0SR_AFR5_Pos)             /* 0x00080000 */
#define GPIO_MI0SR_AFR5_1           (0x02UL << GPIO_MI0SR_AFR5_Pos)             /* 0x00010000 */
#define GPIO_MI0SR_AFR5_2           (0x03UL << GPIO_MI0SR_AFR5_Pos)             /* 0x00020000 */

#define GPIO_MI0SR_perpMux6_DPos    (18U)
#define GPIO_MI0SR_AFR6_Pos         GPIO_MI0SR_perpMux6_DPos
#define GPIO_MI0SR_AFR6_Msk         (0x07UL << GPIO_MI0SR_AFR6_Pos)             /* 0x001C0000 */
#define GPIO_MI0SR_AFR6             GPIO_MI0SR_AFR6_Msk                         /* perpMux6[2:0] bits (Channel 6 multiplexing interface selection) */
#define GPIO_MI0SR_AFR6_0           (0x01UL << GPIO_MI0SR_AFR6_Pos)             /* 0x00040000 */
#define GPIO_MI0SR_AFR6_1           (0x02UL << GPIO_MI0SR_AFR6_Pos)             /* 0x00080000 */
#define GPIO_MI0SR_AFR6_2           (0x03UL << GPIO_MI0SR_AFR6_Pos)             /* 0x00100000 */

#define GPIO_MI0SR_perpMux7_DPos    (21U)
#define GPIO_MI0SR_AFR7_Pos         GPIO_MI0SR_perpMux7_DPos
#define GPIO_MI0SR_AFR7_Msk         (0x07UL << GPIO_MI0SR_AFR7_Pos)             /* 0x00E00000 */
#define GPIO_MI0SR_AFR7             GPIO_MI0SR_AFR7_Msk                         /* perpMux7[2:0] bits (Channel 7 multiplexing interface selection) */
#define GPIO_MI0SR_AFR7_0           (0x01UL << GPIO_MI0SR_AFR7_Pos)             /* 0x00200000 */
#define GPIO_MI0SR_AFR7_1           (0x02UL << GPIO_MI0SR_AFR7_Pos)             /* 0x00400000 */
#define GPIO_MI0SR_AFR7_2           (0x03UL << GPIO_MI0SR_AFR7_Pos)             /* 0x00800000 */

#define GPIO_MI0SR_perpMux8_DPos    (24U)
#define GPIO_MI0SR_AFR8_Pos         GPIO_MI0SR_perpMux8_DPos
#define GPIO_MI0SR_AFR8_Msk         (0x07UL << GPIO_MI0SR_AFR8_Pos)             /* 0x07000000 */
#define GPIO_MI0SR_AFR8             GPIO_MI0SR_AFR8_Msk                         /* perpMux8[2:0] bits (Channel 8 multiplexing interface selection) */
#define GPIO_MI0SR_AFR8_0           (0x01UL << GPIO_MI0SR_AFR8_Pos)             /* 0x01000000 */
#define GPIO_MI0SR_AFR8_1           (0x02UL << GPIO_MI0SR_AFR8_Pos)             /* 0x02000000 */
#define GPIO_MI0SR_AFR8_2           (0x03UL << GPIO_MI0SR_AFR8_Pos)             /* 0x04000000 */

#define GPIO_MI0SR_perpMux9_DPos    (27U)
#define GPIO_MI0SR_AFR9_Pos         GPIO_MI0SR_perpMux9_DPos
#define GPIO_MI0SR_AFR9_Msk         (0x07UL << GPIO_MI0SR_AFR9_Pos)             /* 0x38000000 */
#define GPIO_MI0SR_AFR9             GPIO_MI0SR_AFR9_Msk                         /* perpMux9[2:0] bits (Channel 9 multiplexing interface selection) */
#define GPIO_MI0SR_AFR9_0           (0x01UL << GPIO_MI0SR_AFR9_Pos)             /* 0x08000000 */
#define GPIO_MI0SR_AFR9_1           (0x02UL << GPIO_MI0SR_AFR9_Pos)             /* 0x10000000 */
#define GPIO_MI0SR_AFR9_2           (0x03UL << GPIO_MI0SR_AFR9_Pos)             /* 0x20000000 */


/******************  Bits definition for GPIO_MI1SR register  *****************/
#define GPIO_MI1SR_perpMux10_DPos   (0U)
#define GPIO_MI1SR_AFR10_Pos        GPIO_MI1SR_perpMux10_DPos
#define GPIO_MI1SR_AFR10_Msk        (0x07UL << GPIO_MI1SR_AFR10_Pos)            /* 0x00000007 */
#define GPIO_MI1SR_AFR10            GPIO_MI1SR_AFR10_Msk                        /* perpMux10[2:0] bits (Channel 10 multiplexing interface selection) */
#define GPIO_MI1SR_AFR10_0          (0x01UL << GPIO_MI1SR_AFR10_Pos)            /* 0x00000001 */
#define GPIO_MI1SR_AFR10_1          (0x02UL << GPIO_MI1SR_AFR10_Pos)            /* 0x00000002 */
#define GPIO_MI1SR_AFR10_2          (0x03UL << GPIO_MI1SR_AFR10_Pos)            /* 0x00000004 */

#define GPIO_MI1SR_perpMux11_DPos   (3U)
#define GPIO_MI1SR_AFR11_Pos        GPIO_MI1SR_perpMux11_DPos
#define GPIO_MI1SR_AFR11_Msk        (0x07UL << GPIO_MI1SR_AFR11_Pos)            /* 0x00000038 */
#define GPIO_MI1SR_AFR11            GPIO_MI1SR_AFR11_Msk                        /* perpMux11[2:0] bits (Channel 11 multiplexing interface selection) */
#define GPIO_MI1SR_AFR11_0          (0x01UL << GPIO_MI1SR_AFR11_Pos)            /* 0x00000008 */
#define GPIO_MI1SR_AFR11_1          (0x02UL << GPIO_MI1SR_AFR11_Pos)            /* 0x00000010 */
#define GPIO_MI1SR_AFR11_2          (0x03UL << GPIO_MI1SR_AFR11_Pos)            /* 0x00000020 */

#define GPIO_MI1SR_perpMux12_DPos   (6U)
#define GPIO_MI1SR_AFR12_Pos        GPIO_MI1SR_perpMux12_DPos
#define GPIO_MI1SR_AFR12_Msk        (0x07UL << GPIO_MI1SR_AFR12_Pos)            /* 0x000001C0 */
#define GPIO_MI1SR_AFR12            GPIO_MI1SR_AFR12_Msk                        /* perpMux12[2:0] bits (Channel 12 multiplexing interface selection) */
#define GPIO_MI1SR_AFR12_0          (0x01UL << GPIO_MI1SR_AFR12_Pos)            /* 0x00000040 */
#define GPIO_MI1SR_AFR12_1          (0x02UL << GPIO_MI1SR_AFR12_Pos)            /* 0x00000080 */
#define GPIO_MI1SR_AFR12_2          (0x03UL << GPIO_MI1SR_AFR12_Pos)            /* 0x00000100 */

#define GPIO_MI1SR_perpMux13_DPos   (9U)
#define GPIO_MI1SR_AFR13_Pos        GPIO_MI1SR_perpMux13_DPos
#define GPIO_MI1SR_AFR13_Msk        (0x07UL << GPIO_MI1SR_AFR13_Pos)            /* 0x00000E00 */
#define GPIO_MI1SR_AFR13            GPIO_MI1SR_AFR13_Msk                        /* perpMux13[2:0] bits (Channel 13 multiplexing interface selection) */
#define GPIO_MI1SR_AFR13_0          (0x01UL << GPIO_MI1SR_AFR13_Pos)            /* 0x00000200 */
#define GPIO_MI1SR_AFR13_1          (0x02UL << GPIO_MI1SR_AFR13_Pos)            /* 0x00000400 */
#define GPIO_MI1SR_AFR13_2          (0x03UL << GPIO_MI1SR_AFR13_Pos)            /* 0x00000800 */

#define GPIO_MI1SR_perpMux14_DPos   (12U)
#define GPIO_MI1SR_AFR14_Pos        GPIO_MI1SR_perpMux14_DPos
#define GPIO_MI1SR_AFR14_Msk        (0x07UL << GPIO_MI1SR_AFR14_Pos)            /* 0x00007000 */
#define GPIO_MI1SR_AFR14            GPIO_MI1SR_AFR14_Msk                        /* perpMux14[2:0] bits (Channel 14 multiplexing interface selection) */
#define GPIO_MI1SR_AFR14_0          (0x01UL << GPIO_MI1SR_AFR14_Pos)            /* 0x00001000 */
#define GPIO_MI1SR_AFR14_1          (0x02UL << GPIO_MI1SR_AFR14_Pos)            /* 0x00002000 */
#define GPIO_MI1SR_AFR14_2          (0x03UL << GPIO_MI1SR_AFR14_Pos)            /* 0x00004000 */

#define GPIO_MI1SR_perpMux15_DPos   (15U)
#define GPIO_MI1SR_AFR15_Pos        GPIO_MI1SR_perpMux15_DPos
#define GPIO_MI1SR_AFR15_Msk        (0x07UL << GPIO_MI1SR_AFR15_Pos)            /* 0x00030000 */
#define GPIO_MI1SR_AFR15            GPIO_MI1SR_AFR15_Msk                        /* perpMux15[2:0] bits (Channel 15 multiplexing interface selection) */
#define GPIO_MI1SR_AFR15_0          (0x01UL << GPIO_MI1SR_AFR15_Pos)            /* 0x00008000 */
#define GPIO_MI1SR_AFR15_1          (0x02UL << GPIO_MI1SR_AFR15_Pos)            /* 0x00010000 */
#define GPIO_MI1SR_AFR15_2          (0x03UL << GPIO_MI1SR_AFR15_Pos)            /* 0x00020000 */


/******************  Bits definition for GPIO_OPBR register  *****************/
#define GPIO_OPBR_oBitSet0_DPos     (0U)
#define GPIO_OPBR_ODH0_Pos          GPIO_OPBR_oBitSet0_DPos
#define GPIO_OPBR_ODH0_Msk          (0x01UL << GPIO_OPBR_ODH0_Pos)              /* 0x00000001 */
#define GPIO_OPBR_ODH0              GPIO_OPBR_ODH0_Msk                          /* Channel 0 output set */

#define GPIO_OPBR_oBitSet1_DPos     (1U)
#define GPIO_OPBR_ODH1_Pos          GPIO_OPBR_oBitSet1_DPos
#define GPIO_OPBR_ODH1_Msk          (0x01UL << GPIO_OPBR_ODH1_Pos)              /* 0x00000002 */
#define GPIO_OPBR_ODH1              GPIO_OPBR_ODH1_Msk                          /* Channel 1 output set */

#define GPIO_OPBR_oBitSet2_DPos     (2U)
#define GPIO_OPBR_ODH2_Pos          GPIO_OPBR_oBitSet2_DPos
#define GPIO_OPBR_ODH2_Msk          (0x01UL << GPIO_OPBR_ODH2_Pos)              /* 0x00000004 */
#define GPIO_OPBR_ODH2              GPIO_OPBR_ODH2_Msk                          /* Channel 2 output set */

#define GPIO_OPBR_oBitSet3_DPos     (3U)
#define GPIO_OPBR_ODH3_Pos          GPIO_OPBR_oBitSet3_DPos
#define GPIO_OPBR_ODH3_Msk          (0x01UL << GPIO_OPBR_ODH3_Pos)              /* 0x00000008 */
#define GPIO_OPBR_ODH3              GPIO_OPBR_ODH3_Msk                          /* Channel 3 output set */

#define GPIO_OPBR_oBitSet4_DPos     (4U)
#define GPIO_OPBR_ODH4_Pos          GPIO_OPBR_oBitSet4_DPos
#define GPIO_OPBR_ODH4_Msk          (0x01UL << GPIO_OPBR_ODH4_Pos)              /* 0x00000010 */
#define GPIO_OPBR_ODH4              GPIO_OPBR_ODH4_Msk                          /* Channel 4 output set */

#define GPIO_OPBR_oBitSet5_DPos     (5U)
#define GPIO_OPBR_ODH5_Pos          GPIO_OPBR_oBitSet5_DPos
#define GPIO_OPBR_ODH5_Msk          (0x01UL << GPIO_OPBR_ODH5_Pos)              /* 0x00000020 */
#define GPIO_OPBR_ODH5              GPIO_OPBR_ODH5_Msk                          /* Channel 5 output set */

#define GPIO_OPBR_oBitSet6_DPos     (6U)
#define GPIO_OPBR_ODH6_Pos          GPIO_OPBR_oBitSet6_DPos
#define GPIO_OPBR_ODH6_Msk          (0x01UL << GPIO_OPBR_ODH6_Pos)              /* 0x00000040 */
#define GPIO_OPBR_ODH6              GPIO_OPBR_ODH6_Msk                          /* Channel 6 output set */

#define GPIO_OPBR_oBitSet7_DPos     (7U)
#define GPIO_OPBR_ODH7_Pos          GPIO_OPBR_oBitSet7_DPos
#define GPIO_OPBR_ODH7_Msk          (0x01UL << GPIO_OPBR_ODH7_Pos)              /* 0x00000080 */
#define GPIO_OPBR_ODH7              GPIO_OPBR_ODH7_Msk                          /* Channel 7 output set */

#define GPIO_OPBR_oBitSet8_DPos     (8U)
#define GPIO_OPBR_ODH8_Pos          GPIO_OPBR_oBitSet8_DPos
#define GPIO_OPBR_ODH8_Msk          (0x01UL << GPIO_OPBR_ODH8_Pos)              /* 0x00000100 */
#define GPIO_OPBR_ODH8              GPIO_OPBR_ODH8_Msk                          /* Channel 8 output set */

#define GPIO_OPBR_oBitSet9_DPos     (9U)
#define GPIO_OPBR_ODH9_Pos          GPIO_OPBR_oBitSet9_DPos
#define GPIO_OPBR_ODH9_Msk          (0x01UL << GPIO_OPBR_ODH9_Pos)              /* 0x00000200 */
#define GPIO_OPBR_ODH9              GPIO_OPBR_ODH9_Msk                          /* Channel 9 output set */

#define GPIO_OPBR_oBitSet10_DPos    (10U)
#define GPIO_OPBR_ODH10_Pos         GPIO_OPBR_oBitSet10_DPos
#define GPIO_OPBR_ODH10_Msk         (0x01UL << GPIO_OPBR_ODH10_Pos)             /* 0x00000400 */
#define GPIO_OPBR_ODH10             GPIO_OPBR_ODH10_Msk                         /* Channel 10 output set */

#define GPIO_OPBR_oBitSet11_DPos    (11U)
#define GPIO_OPBR_ODH11_Pos         GPIO_OPBR_oBitSet11_DPos
#define GPIO_OPBR_ODH11_Msk         (0x01UL << GPIO_OPBR_ODH11_Pos)             /* 0x00000800 */
#define GPIO_OPBR_ODH11             GPIO_OPBR_ODH11_Msk                         /* Channel 11 output set */

#define GPIO_OPBR_oBitSet12_DPos    (12U)
#define GPIO_OPBR_ODH12_Pos         GPIO_OPBR_oBitSet12_DPos
#define GPIO_OPBR_ODH12_Msk         (0x01UL << GPIO_OPBR_ODH12_Pos)             /* 0x00001000 */
#define GPIO_OPBR_ODH12             GPIO_OPBR_ODH12_Msk                         /* Channel 12 output set */

#define GPIO_OPBR_oBitSet13_DPos    (13U)
#define GPIO_OPBR_ODH13_Pos         GPIO_OPBR_oBitSet13_DPos
#define GPIO_OPBR_ODH13_Msk         (0x01UL << GPIO_OPBR_ODH13_Pos)             /* 0x00002000 */
#define GPIO_OPBR_ODH13             GPIO_OPBR_ODH13_Msk                         /* Channel 13 output set */

#define GPIO_OPBR_oBitSet14_DPos    (14U)
#define GPIO_OPBR_ODH14_Pos         GPIO_OPBR_oBitSet14_DPos
#define GPIO_OPBR_ODH14_Msk         (0x01UL << GPIO_OPBR_ODH14_Pos)             /* 0x00004000 */
#define GPIO_OPBR_ODH14             GPIO_OPBR_ODH14_Msk                         /* Channel 14 output set */

#define GPIO_OPBR_oBitSet15_DPos    (15U)
#define GPIO_OPBR_ODH15_Pos         GPIO_OPBR_oBitSet15_DPos
#define GPIO_OPBR_ODH15_Msk         (0x01UL << GPIO_OPBR_ODH15_Pos)             /* 0x00008000 */
#define GPIO_OPBR_ODH15             GPIO_OPBR_ODH15_Msk                         /* Channel 15 output set */


/******************  Bits definition for GPIO_OBCR register  *****************/
#define GPIO_OBCR_oBitClear0_DPos   (0U)
#define GPIO_OBCR_ODL0_Pos          GPIO_OBCR_oBitClear0_DPos
#define GPIO_OBCR_ODL0_Msk          (0x01UL << GPIO_OBCR_ODL0_Pos)              /* 0x00000001 */
#define GPIO_OBCR_ODL0              GPIO_OBCR_ODL0_Msk                          /* Channel 0 output clear */

#define GPIO_OBCR_oBitClear1_DPos   (1U)
#define GPIO_OBCR_ODL1_Pos          GPIO_OBCR_oBitClear1_DPos
#define GPIO_OBCR_ODL1_Msk          (0x01UL << GPIO_OBCR_ODL1_Pos)              /* 0x00000002 */
#define GPIO_OBCR_ODL1              GPIO_OBCR_ODL1_Msk                          /* Channel 1 output clear */

#define GPIO_OBCR_oBitClear2_DPos   (2U)
#define GPIO_OBCR_ODL2_Pos          GPIO_OBCR_oBitClear2_DPos
#define GPIO_OBCR_ODL2_Msk          (0x01UL << GPIO_OBCR_ODL2_Pos)              /* 0x00000004 */
#define GPIO_OBCR_ODL2              GPIO_OBCR_ODL2_Msk                          /* Channel 2 output clear */

#define GPIO_OBCR_oBitClear3_DPos   (3U)
#define GPIO_OBCR_ODL3_Pos          GPIO_OBCR_oBitClear3_DPos
#define GPIO_OBCR_ODL3_Msk          (0x01UL << GPIO_OBCR_ODL3_Pos)              /* 0x00000008 */
#define GPIO_OBCR_ODL3              GPIO_OBCR_ODL3_Msk                          /* Channel 3 output clear */

#define GPIO_OBCR_oBitClear4_DPos   (4U)
#define GPIO_OBCR_ODL4_Pos          GPIO_OBCR_oBitClear4_DPos
#define GPIO_OBCR_ODL4_Msk          (0x01UL << GPIO_OBCR_ODL4_Pos)              /* 0x00000010 */
#define GPIO_OBCR_ODL4              GPIO_OBCR_ODL4_Msk                          /* Channel 4 output clear */

#define GPIO_OBCR_oBitClear5_DPos   (5U)
#define GPIO_OBCR_ODL5_Pos          GPIO_OBCR_oBitClear5_DPos
#define GPIO_OBCR_ODL5_Msk          (0x01UL << GPIO_OBCR_ODL5_Pos)              /* 0x00000020 */
#define GPIO_OBCR_ODL5              GPIO_OBCR_ODL5_Msk                          /* Channel 5 output clear */

#define GPIO_OBCR_oBitClear6_DPos   (6U)
#define GPIO_OBCR_ODL6_Pos          GPIO_OBCR_oBitClear6_DPos
#define GPIO_OBCR_ODL6_Msk          (0x01UL << GPIO_OBCR_ODL6_Pos)              /* 0x00000040 */
#define GPIO_OBCR_ODL6              GPIO_OBCR_ODL6_Msk                          /* Channel 6 output clear */

#define GPIO_OBCR_oBitClear7_DPos   (7U)
#define GPIO_OBCR_ODL7_Pos          GPIO_OBCR_oBitClear7_DPos
#define GPIO_OBCR_ODL7_Msk          (0x01UL << GPIO_OBCR_ODL7_Pos)              /* 0x00000080 */
#define GPIO_OBCR_ODL7              GPIO_OBCR_ODL7_Msk                          /* Channel 7 output clear */

#define GPIO_OBCR_oBitClear8_DPos   (8U)
#define GPIO_OBCR_ODL8_Pos          GPIO_OBCR_oBitClear8_DPos
#define GPIO_OBCR_ODL8_Msk          (0x01UL << GPIO_OBCR_ODL8_Pos)              /* 0x00000100 */
#define GPIO_OBCR_ODL8              GPIO_OBCR_ODL8_Msk                          /* Channel 8 output clear */

#define GPIO_OBCR_oBitClear9_DPos   (9U)
#define GPIO_OBCR_ODL9_Pos          GPIO_OBCR_oBitClear9_DPos
#define GPIO_OBCR_ODL9_Msk          (0x01UL << GPIO_OBCR_ODL9_Pos)              /* 0x00000200 */
#define GPIO_OBCR_ODL9              GPIO_OBCR_ODL9_Msk                          /* Channel 9 output clear */

#define GPIO_OBCR_oBitClear10_DPos  (10U)
#define GPIO_OBCR_ODL10_Pos         GPIO_OBCR_oBitClear10_DPos
#define GPIO_OBCR_ODL10_Msk         (0x01UL << GPIO_OBCR_ODL10_Pos)             /* 0x00000400 */
#define GPIO_OBCR_ODL10             GPIO_OBCR_ODL10_Msk                         /* Channel 10 output clear */

#define GPIO_OBCR_oBitClear11_DPos  (11U)
#define GPIO_OBCR_ODL11_Pos         GPIO_OBCR_oBitClear11_DPos
#define GPIO_OBCR_ODL11_Msk         (0x01UL << GPIO_OBCR_ODL11_Pos)             /* 0x00000800 */
#define GPIO_OBCR_ODL11             GPIO_OBCR_ODL11_Msk                         /* Channel 11 output clear */

#define GPIO_OBCR_oBitClear12_DPos  (12U)
#define GPIO_OBCR_ODL12_Pos         GPIO_OBCR_oBitClear12_DPos
#define GPIO_OBCR_ODL12_Msk         (0x01UL << GPIO_OBCR_ODL12_Pos)             /* 0x00001000 */
#define GPIO_OBCR_ODL12             GPIO_OBCR_ODL12_Msk                         /* Channel 12 output clear */

#define GPIO_OBCR_oBitClear13_DPos  (13U)
#define GPIO_OBCR_ODL13_Pos         GPIO_OBCR_oBitClear13_DPos
#define GPIO_OBCR_ODL13_Msk         (0x01UL << GPIO_OBCR_ODL13_Pos)             /* 0x00002000 */
#define GPIO_OBCR_ODL13             GPIO_OBCR_ODL13_Msk                         /* Channel 13 output clear */

#define GPIO_OBCR_oBitClear14_DPos  (14U)
#define GPIO_OBCR_ODL14_Pos         GPIO_OBCR_oBitClear14_DPos
#define GPIO_OBCR_ODL14_Msk         (0x01UL << GPIO_OBCR_ODL14_Pos)             /* 0x00004000 */
#define GPIO_OBCR_ODL14             GPIO_OBCR_ODL14_Msk                         /* Channel 14 output clear */

#define GPIO_OBCR_oBitClear15_DPos  (15U)
#define GPIO_OBCR_ODL15_Pos         GPIO_OBCR_oBitClear15_DPos
#define GPIO_OBCR_ODL15_Msk         (0x01UL << GPIO_OBCR_ODL15_Pos)             /* 0x00008000 */
#define GPIO_OBCR_ODL15             GPIO_OBCR_ODL15_Msk                         /* Channel 15 output clear */


/******************  Bits definition for GPIO_OBTR register  *****************/
#define GPIO_OBTR_oBitTrigger0_DPos (0U)
#define GPIO_OBTR_ODT0_Pos          GPIO_OBTR_oBitTrigger0_DPos
#define GPIO_OBTR_ODT0_Msk          (0x01UL << GPIO_OBTR_ODT0_Pos)              /* 0x00000001 */
#define GPIO_OBTR_ODT0              GPIO_OBTR_ODT0_Msk                          /* Channel 0 output toggle */

#define GPIO_OBTR_oBitTrigger1_DPos  (1U)
#define GPIO_OBTR_ODT1_Pos          GPIO_OBTR_oBitTrigger1_DPos
#define GPIO_OBTR_ODT1_Msk          (0x01UL << GPIO_OBTR_ODT1_Pos)              /* 0x00000002 */
#define GPIO_OBTR_ODT1              GPIO_OBTR_ODT1_Msk                          /* Channel 1 output toggle */

#define GPIO_OBTR_oBitTrigger2_DPos  (2U)
#define GPIO_OBTR_ODT2_Pos          GPIO_OBTR_oBitTrigger2_DPos
#define GPIO_OBTR_ODT2_Msk          (0x01UL << GPIO_OBTR_ODT2_Pos)              /* 0x00000004 */
#define GPIO_OBTR_ODT2              GPIO_OBTR_ODT2_Msk                          /* Channel 2 output toggle */

#define GPIO_OBTR_oBitTrigger3_DPos  (3U)
#define GPIO_OBTR_ODT3_Pos          GPIO_OBTR_oBitTrigger3_DPos
#define GPIO_OBTR_ODT3_Msk          (0x01UL << GPIO_OBTR_ODT3_Pos)              /* 0x00000008 */
#define GPIO_OBTR_ODT3              GPIO_OBTR_ODT3_Msk                          /* Channel 3 output toggle */

#define GPIO_OBTR_oBitTrigger4_DPos  (4U)
#define GPIO_OBTR_ODT4_Pos          GPIO_OBTR_oBitTrigger4_DPos
#define GPIO_OBTR_ODT4_Msk          (0x01UL << GPIO_OBTR_ODT4_Pos)              /* 0x00000010 */
#define GPIO_OBTR_ODT4              GPIO_OBTR_ODT4_Msk                          /* Channel 4 output toggle */

#define GPIO_OBTR_oBitTrigger5_DPos  (5U)
#define GPIO_OBTR_ODT5_Pos          GPIO_OBTR_oBitTrigger5_DPos
#define GPIO_OBTR_ODT5_Msk          (0x01UL << GPIO_OBTR_ODT5_Pos)              /* 0x00000020 */
#define GPIO_OBTR_ODT5              GPIO_OBTR_ODT5_Msk                          /* Channel 5 output toggle */

#define GPIO_OBTR_oBitTrigger_DPos  (6U)
#define GPIO_OBTR_ODT6_Pos          GPIO_OBTR_oBitTrigger_DPos
#define GPIO_OBTR_ODT6_Msk          (0x01UL << GPIO_OBTR_ODT6_Pos)              /* 0x00000040 */
#define GPIO_OBTR_ODT6              GPIO_OBTR_ODT6_Msk                          /* Channel 6 output toggle */

#define GPIO_OBTR_oBitTrigger7_DPos (7U)
#define GPIO_OBTR_ODT7_Pos          GPIO_OBTR_oBitTrigger7_DPos
#define GPIO_OBTR_ODT7_Msk          (0x01UL << GPIO_OBTR_ODT7_Pos)              /* 0x00000080 */
#define GPIO_OBTR_ODT7              GPIO_OBTR_ODT7_Msk                          /* Channel 7 output toggle */

#define GPIO_OBTR_oBitTrigger8_DPos (8U)
#define GPIO_OBTR_ODT8_Pos          GPIO_OBTR_oBitTrigger8_DPos
#define GPIO_OBTR_ODT8_Msk          (0x01UL << GPIO_OBTR_ODT8_Pos)              /* 0x00000100 */
#define GPIO_OBTR_ODT8              GPIO_OBTR_ODT8_Msk                          /* Channel 8 output toggle */

#define GPIO_OBTR_oBitTrigger9_DPos (9U)
#define GPIO_OBTR_ODT9_Pos          GPIO_OBTR_oBitTrigger9_DPos
#define GPIO_OBTR_ODT9_Msk          (0x01UL << GPIO_OBTR_ODT9_Pos)              /* 0x00000200 */
#define GPIO_OBTR_ODT9              GPIO_OBTR_ODT9_Msk                          /* Channel 9 output toggle */

#define GPIO_OBTR_oBitTrigger10_DPos (10U)
#define GPIO_OBTR_ODT10_Pos         GPIO_OBTR_oBitTrigger10_DPos
#define GPIO_OBTR_ODT10_Msk         (0x01UL << GPIO_OBTR_ODT10_Pos)             /* 0x00000400 */
#define GPIO_OBTR_ODT10             GPIO_OBTR_ODT10_Msk                         /* Channel 10 output toggle */

#define GPIO_OBTR_oBitTrigger11_DPos (11U)
#define GPIO_OBTR_ODT11_Pos         GPIO_OBTR_oBitTrigger11_DPos
#define GPIO_OBTR_ODT11_Msk         (0x01UL << GPIO_OBTR_ODT11_Pos)             /* 0x00000800 */
#define GPIO_OBTR_ODT11             GPIO_OBTR_ODT11_Msk                         /* Channel 11 output toggle */

#define GPIO_OBTR_oBitTrigger12_DPos (12U)
#define GPIO_OBTR_ODT12_Pos         GPIO_OBTR_oBitTrigger12_DPos
#define GPIO_OBTR_ODT12_Msk         (0x01UL << GPIO_OBTR_ODT12_Pos)             /* 0x00001000 */
#define GPIO_OBTR_ODT12             GPIO_OBTR_ODT12_Msk                         /* Channel 12 output toggle */

#define GPIO_OBTR_oBitTrigger13_DPos (13U)
#define GPIO_OBTR_ODT13_Pos         GPIO_OBTR_oBitTrigger13_DPos
#define GPIO_OBTR_ODT13_Msk         (0x01UL << GPIO_OBTR_ODT13_Pos)             /* 0x00002000 */
#define GPIO_OBTR_ODT13             GPIO_OBTR_ODT13_Msk                         /* Channel 13 output toggle */

#define GPIO_OBTR_oBitTrigger14_DPos (14U)
#define GPIO_OBTR_ODT14_Pos         GPIO_OBTR_oBitTrigger14_DPos
#define GPIO_OBTR_ODT14_Msk         (0x01UL << GPIO_OBTR_ODT14_Pos)             /* 0x00004000 */
#define GPIO_OBTR_ODT14             GPIO_OBTR_ODT14_Msk                         /* Channel 14 output toggle */

#define GPIO_OBTR_oBitTrigger15_DPos (15U)
#define GPIO_OBTR_ODT15_Pos         GPIO_OBTR_oBitTrigger15_DPos
#define GPIO_OBTR_ODT15_Msk         (0x01UL << GPIO_OBTR_ODT15_Pos)             /* 0x00008000 */
#define GPIO_OBTR_ODT15             GPIO_OBTR_ODT15_Msk                         /* Channel 15 output toggle */


/******************  Bits definition for GPIO_OCR register  *****************/
#define GPIO_OCR_oCtrl0_DPos        (0U)
#define GPIO_OCR_ODR0_Pos           GPIO_OCR_oCtrl0_DPos
#define GPIO_OCR_ODR0_Msk           (0x01UL << GPIO_OCR_ODR0_Pos)               /* 0x00000001 */
#define GPIO_OCR_ODR0               GPIO_OCR_ODR0_Msk                           /* Channel 0 output control */


#define GPIO_OCR_oCtrl1_DPos        (1U)
#define GPIO_OCR_ODR1_Pos           GPIO_OCR_oCtrl1_DPos
#define GPIO_OCR_ODR1_Msk           (0x01UL << GPIO_OCR_ODR1_Pos)               /* 0x00000002 */
#define GPIO_OCR_ODR1               GPIO_OCR_ODR1_Msk                           /* Channel 1 output control */

#define GPIO_OCR_oCtrl2_DPos        (2U)
#define GPIO_OCR_ODR2_Pos           GPIO_OCR_oCtrl2_DPos
#define GPIO_OCR_ODR2_Msk           (0x01UL << GPIO_OCR_ODR2_Pos)               /* 0x00000004 */
#define GPIO_OCR_ODR2               GPIO_OCR_ODR2_Msk                           /* Channel 2 output control */

#define GPIO_OCR_oCtrl3_DPos        (3U)
#define GPIO_OCR_ODR3_Pos           GPIO_OCR_oCtrl3_DPos
#define GPIO_OCR_ODR3_Msk           (0x01UL << GPIO_OCR_ODR3_Pos)               /* 0x00000008 */
#define GPIO_OCR_ODR3               GPIO_OCR_ODR3_Msk                           /* Channel 3 output control */

#define GPIO_OCR_oCtrl4_DPos        (4U)
#define GPIO_OCR_ODR4_Pos           GPIO_OCR_oCtrl4_DPos
#define GPIO_OCR_ODR4_Msk           (0x01UL << GPIO_OCR_ODR4_Pos)               /* 0x00000010 */
#define GPIO_OCR_ODR4               GPIO_OCR_ODR4_Msk                           /* Channel 4 output control */

#define GPIO_OCR_oCtrl5_DPos        (5U)
#define GPIO_OCR_ODR5_Pos           GPIO_OCR_oCtrl5_DPos
#define GPIO_OCR_ODR5_Msk           (0x01UL << GPIO_OCR_ODR5_Pos)               /* 0x00000020 */
#define GPIO_OCR_ODR5               GPIO_OCR_ODR5_Msk                           /* Channel 5 output control */

#define GPIO_OCR_oCtrl6_DPos        (6U)
#define GPIO_OCR_ODR6_Pos           GPIO_OCR_oCtrl6_DPos
#define GPIO_OCR_ODR6_Msk           (0x01UL << GPIO_OCR_ODR6_Pos)               /* 0x00000000 */
#define GPIO_OCR_ODR6               GPIO_OCR_ODR6_Msk                           /* Channel 6 output control */

#define GPIO_OCR_oCtrl7_DPos        (7U)
#define GPIO_OCR_ODR7_Pos           GPIO_OCR_oCtrl7_DPos
#define GPIO_OCR_ODR7_Msk           (0x01UL << GPIO_OCR_ODR7_Pos)               /* 0x00000040 */
#define GPIO_OCR_ODR7               GPIO_OCR_ODR7_Msk                           /* Channel 7 output control */

#define GPIO_OCR_oCtrl8_DPos        (8U)
#define GPIO_OCR_ODR8_Pos           GPIO_OCR_oCtrl8_DPos
#define GPIO_OCR_ODR8_Msk           (0x01UL << GPIO_OCR_ODR8_Pos)               /* 0x00000080 */
#define GPIO_OCR_ODR8               GPIO_OCR_ODR8_Msk                           /* Channel 8 output control */

#define GPIO_OCR_oCtrl9_DPos        (9U)
#define GPIO_OCR_ODR9_Pos           GPIO_OCR_oCtrl9_DPos
#define GPIO_OCR_ODR9_Msk           (0x01UL << GPIO_OCR_ODR9_Pos)               /* 0x00000100 */
#define GPIO_OCR_ODR9               GPIO_OCR_ODR9_Msk                           /* Channel 9 output control */

#define GPIO_OCR_oCtrl10_DPos       (10U)
#define GPIO_OCR_ODR10_Pos          GPIO_OCR_oCtrl10_DPos
#define GPIO_OCR_ODR10_Msk          (0x01UL << GPIO_OCR_ODR10_Pos)              /* 0x00000200 */
#define GPIO_OCR_ODR10              GPIO_OCR_ODR10_Msk                          /* Channel 0 output control */

#define GPIO_OCR_oCtrl11_DPos       (11U)
#define GPIO_OCR_ODR11_Pos          GPIO_OCR_oCtrl11_DPos
#define GPIO_OCR_ODR11_Msk          (0x01UL << GPIO_OCR_ODR11_Pos)              /* 0x00000400 */
#define GPIO_OCR_ODR11              GPIO_OCR_ODR11_Msk                          /* Channel 11 output control */

#define GPIO_OCR_oCtrl12_DPos       (12U)
#define GPIO_OCR_ODR12_Pos          GPIO_OCR_oCtrl12_DPos
#define GPIO_OCR_ODR12_Msk          (0x01UL << GPIO_OCR_ODR12_Pos)              /* 0x00000800 */
#define GPIO_OCR_ODR12              GPIO_OCR_ODR12_Msk                          /* Channel 12 output control */

#define GPIO_OCR_oCtrl13_DPos       (13U)
#define GPIO_OCR_ODR13_Pos          GPIO_OCR_oCtrl13_DPos
#define GPIO_OCR_ODR13_Msk          (0x01UL << GPIO_OCR_ODR13_Pos)              /* 0x00001000 */
#define GPIO_OCR_ODR13              GPIO_OCR_ODR13_Msk                          /* Channel 13 output control */

#define GPIO_OCR_oCtrl14_DPos       (14U)
#define GPIO_OCR_ODR14_Pos          GPIO_OCR_oCtrl14_DPos
#define GPIO_OCR_ODR14_Msk          (0x01UL << GPIO_OCR_ODR14_Pos)              /* 0x00002000 */
#define GPIO_OCR_ODR14              GPIO_OCR_ODR14_Msk                          /* Channel 14 output control */

#define GPIO_OCR_oCtrl15_DPos       (15U)
#define GPIO_OCR_ODR15_Pos          GPIO_OCR_oCtrl15_DPos
#define GPIO_OCR_ODR15_Msk          (0x01UL << GPIO_OCR_ODR15_Pos)              /* 0x00004000 */
#define GPIO_OCR_ODR15              GPIO_OCR_ODR15_Msk                          /* Channel 15 output control */


/******************  Bits definition for GPIO_IRR register  *****************/
#define GPIO_IRR_rdCh0_DPos         (0U)
#define GPIO_IRR_IDR0_Pos           GPIO_IRR_rdCh0_DPos
#define GPIO_IRR_IDR0_Msk           (0x01UL << GPIO_IRR_IDR0_Pos)               /* 0x00000001 */
#define GPIO_IRR_IDR0               GPIO_IRR_IDR0_Msk                           /* Channel 0 input read */

#define GPIO_IRR_rdCh1_DPos         (1U)
#define GPIO_IRR_IDR1_Pos           GPIO_IRR_rdCh1_DPos     
#define GPIO_IRR_IDR1_Msk           (0x01UL << GPIO_IRR_IDR1_Pos)               /* 0x00000002 */
#define GPIO_IRR_IDR1               GPIO_IRR_IDR1_Msk                           /* Channel 1 input read */

#define GPIO_IRR_rdCh2_DPos         (2U)
#define GPIO_IRR_IDR2_Pos           GPIO_IRR_rdCh2_DPos
#define GPIO_IRR_IDR2_Msk           (0x01UL << GPIO_IRR_IDR2_Pos)               /* 0x00000004 */
#define GPIO_IRR_IDR2               GPIO_IRR_IDR2_Msk                           /* Channel 2 input read */

#define GPIO_IRR_rdCh3_DPos         (3U)
#define GPIO_IRR_IDR3_Pos           GPIO_IRR_rdCh3_DPos
#define GPIO_IRR_IDR3_Msk           (0x01UL << GPIO_IRR_IDR3_Pos)               /* 0x00000008 */
#define GPIO_IRR_IDR3               GPIO_IRR_IDR3_Msk                           /* Channel 3 input read */

#define GPIO_IRR_rdCh4_DPos         (4U)
#define GPIO_IRR_IDR4_Pos           GPIO_IRR_rdCh4_DPos
#define GPIO_IRR_IDR4_Msk           (0x01UL << GPIO_IRR_IDR4_Pos)               /* 0x00000010 */
#define GPIO_IRR_IDR4               GPIO_IRR_IDR4_Msk                           /* Channel 4 input read */

#define GPIO_IRR_rdCh5_DPos         (5U)
#define GPIO_IRR_IDR5_Pos           GPIO_IRR_rdCh5_DPos
#define GPIO_IRR_IDR5_Msk           (0x01UL << GPIO_IRR_IDR5_Pos)               /* 0x00000020 */
#define GPIO_IRR_IDR5               GPIO_IRR_IDR5_Msk                           /* Channel 5 input read */

#define GPIO_IRR_rdCh6_DPos         (6U)
#define GPIO_IRR_IDR6_Pos           GPIO_IRR_rdCh6_DPos
#define GPIO_IRR_IDR6_Msk           (0x01UL << GPIO_IRR_IDR6_Pos)               /* 0x00000040 */
#define GPIO_IRR_IDR6               GPIO_IRR_IDR6_Msk                           /* Channel 6 input read */

#define GPIO_IRR_rdCh7_DPos         (7U)
#define GPIO_IRR_IDR7_Pos           GPIO_IRR_rdCh7_DPos
#define GPIO_IRR_IDR7_Msk           (0x01UL << GPIO_IRR_IDR7_Pos)               /* 0x00000080 */
#define GPIO_IRR_IDR7               GPIO_IRR_IDR7_Msk                           /* Channel 7 input read */

#define GPIO_IRR_rdCh8_DPos         (8U)
#define GPIO_IRR_IDR8_Pos           GPIO_IRR_rdCh8_DPos
#define GPIO_IRR_IDR8_Msk           (0x01UL << GPIO_IRR_IDR8_Pos)               /* 0x00000100 */
#define GPIO_IRR_IDR8               GPIO_IRR_IDR8_Msk                           /* Channel 8 input read */

#define GPIO_IRR_rdCh9_DPos         (9U)
#define GPIO_IRR_IDR9_Pos           GPIO_IRR_rdCh9_DPos
#define GPIO_IRR_IDR9_Msk           (0x01UL << GPIO_IRR_IDR9_Pos)               /* 0x00000200 */
#define GPIO_IRR_IDR9               GPIO_IRR_IDR9_Msk                           /* Channel 9 input read */

#define GPIO_IRR_rdCh10_DPos        (10U)
#define GPIO_IRR_IDR10_Pos          GPIO_IRR_rdCh10_DPos
#define GPIO_IRR_IDR10_Msk          (0x01UL << GPIO_IRR_IDR10_Pos)              /* 0x00000400 */
#define GPIO_IRR_IDR10              GPIO_IRR_IDR10_Msk                          /* Channel 10 input read */

#define GPIO_IRR_rdCh11_DPos        (11U)
#define GPIO_IRR_IDR11_Pos          GPIO_IRR_rdCh11_DPos
#define GPIO_IRR_IDR11_Msk          (0x01UL << GPIO_IRR_IDR11_Pos)              /* 0x00000800 */
#define GPIO_IRR_IDR11              GPIO_IRR_IDR11_Msk                          /* Channel 11 input read */

#define GPIO_IRR_rdCh12_DPos        (12U)
#define GPIO_IRR_IDR12_Pos          GPIO_IRR_rdCh12_DPos
#define GPIO_IRR_IDR12_Msk          (0x01UL << GPIO_IRR_IDR12_Pos)              /* 0x00001000 */
#define GPIO_IRR_IDR12              GPIO_IRR_IDR12_Msk                          /* Channel 12 input read */

#define GPIO_IRR_rdCh13_DPos        (13U)
#define GPIO_IRR_IDR13_Pos          GPIO_IRR_rdCh13_DPos
#define GPIO_IRR_IDR13_Msk          (0x01UL << GPIO_IRR_IDR13_Pos)              /* 0x00002000 */
#define GPIO_IRR_IDR13              GPIO_IRR_IDR13_Msk                          /* Channel 13 input read */

#define GPIO_IRR_rdCh14_DPos        (14U)
#define GPIO_IRR_IDR14_Pos          GPIO_IRR_rdCh14_DPos
#define GPIO_IRR_IDR14_Msk          (0x01UL << GPIO_IRR_IDR14_Pos)              /* 0x00004000 */
#define GPIO_IRR_IDR14              GPIO_IRR_IDR14_Msk                          /* Channel 14 input read */

#define GPIO_IRR_rdCh15_DPos        (15U)
#define GPIO_IRR_IDR15_Pos          GPIO_IRR_rdCh15_DPos
#define GPIO_IRR_IDR15_Msk          (0x01UL << GPIO_IRR_IDR15_Pos)              /* 0x00008000 */
#define GPIO_IRR_IDR15              GPIO_IRR_IDR15_Msk                          /* Channel 15 input read */


/******************  Bits definition for GPIO_ITYPE0R register  *****************/
#define GPIO_ITYPE0R_intType0_DPos  (0U)
#define GPIO_ITYPE0R_ITEN0_Pos      GPIO_ITYPE0R_intType0_DPos
#define GPIO_ITYPE0R_ITEN0_Msk      (0x03UL << GPIO_ITYPE0R_ITEN0_Pos)          /* 0x00000003 */
#define GPIO_ITYPE0R_ITEN0          GPIO_ITYPE0R_ITEN0_Msk                      /* intType0[1:0] bits (Select the channel 0 interrupt type) */
#define GPIO_ITYPE0R_ITEN0_0        (0x01UL << GPIO_ITYPE0R_ITEN0_Pos)          /* 0x00000001 */
#define GPIO_ITYPE0R_ITEN0_1        (0x02UL << GPIO_ITYPE0R_ITEN0_Pos)          /* 0x00000002 */

#define GPIO_ITYPE0R_intType1_DPos  (2U)
#define GPIO_ITYPE0R_ITEN1_Pos      GPIO_ITYPE0R_intType1_DPos
#define GPIO_ITYPE0R_ITEN1_Msk      (0x03UL << GPIO_ITYPE0R_ITEN1_Pos)          /* 0x0000000C */
#define GPIO_ITYPE0R_ITEN1          GPIO_ITYPE0R_ITEN1_Msk                      /* intType1[1:0] bits (Select the channel 1 interrupt type) */
#define GPIO_ITYPE0R_ITEN1_0        (0x01UL << GPIO_ITYPE0R_ITEN1_Pos)          /* 0x00000004 */
#define GPIO_ITYPE0R_ITEN1_1        (0x02UL << GPIO_ITYPE0R_ITEN1_Pos)          /* 0x00000008 */

#define GPIO_ITYPE0R_intType2_DPos  (4U)
#define GPIO_ITYPE0R_ITEN2_Pos      GPIO_ITYPE0R_intType2_DPos
#define GPIO_ITYPE0R_ITEN2_Msk      (0x03UL << GPIO_ITYPE0R_ITEN2_Pos)          /* 0x00000030 */
#define GPIO_ITYPE0R_ITEN2          GPIO_ITYPE0R_ITEN2_Msk                      /* intType2[1:0] bits (Select the channel 2 interrupt type) */
#define GPIO_ITYPE0R_ITEN2_0        (0x01UL << GPIO_ITYPE0R_ITEN2_Pos)          /* 0x00000010 */
#define GPIO_ITYPE0R_ITEN2_1        (0x02UL << GPIO_ITYPE0R_ITEN2_Pos)          /* 0x00000020 */

#define GPIO_ITYPE0R_intType3_DPos  (6U)
#define GPIO_ITYPE0R_ITEN3_Pos      GPIO_ITYPE0R_intType3_DPos
#define GPIO_ITYPE0R_ITEN3_Msk      (0x03UL << GPIO_ITYPE0R_ITEN3_Pos)          /* 0x000000C0 */
#define GPIO_ITYPE0R_ITEN3          GPIO_ITYPE0R_ITEN3_Msk                      /* intType3[1:0] bits (Select the channel 3 interrupt type) */
#define GPIO_ITYPE0R_ITEN3_0        (0x01UL << GPIO_ITYPE0R_ITEN3_Pos)          /* 0x00000040 */
#define GPIO_ITYPE0R_ITEN3_1        (0x02UL << GPIO_ITYPE0R_ITEN3_Pos)          /* 0x00000080 */

#define GPIO_ITYPE0R_intType4_DPos  (8U)
#define GPIO_ITYPE0R_ITEN4_Pos      GPIO_ITYPE0R_intType4_DPos
#define GPIO_ITYPE0R_ITEN4_Msk      (0x03UL << GPIO_ITYPE0R_ITEN4_Pos)          /* 0x00000300 */
#define GPIO_ITYPE0R_ITEN4          GPIO_ITYPE0R_ITEN4_Msk                      /* intType4[1:0] bits (Select the channel 4 interrupt type) */
#define GPIO_ITYPE0R_ITEN4_0        (0x01UL << GPIO_ITYPE0R_ITEN4_Pos)          /* 0x00000100 */
#define GPIO_ITYPE0R_ITEN4_1        (0x02UL << GPIO_ITYPE0R_ITEN4_Pos)          /* 0x00000200 */

#define GPIO_ITYPE0R_intType5_DPos  (10U)
#define GPIO_ITYPE0R_ITEN5_Pos      GPIO_ITYPE0R_intType5_DPos
#define GPIO_ITYPE0R_ITEN5_Msk      (0x03UL << GPIO_ITYPE0R_ITEN5_Pos)          /* 0x00000C00 */
#define GPIO_ITYPE0R_ITEN5          GPIO_ITYPE0R_ITEN5_Msk                      /* intType5[1:0] bits (Select the channel 5 interrupt type) */
#define GPIO_ITYPE0R_ITEN5_0        (0x01UL << GPIO_ITYPE0R_ITEN5_Pos)          /* 0x00000400 */
#define GPIO_ITYPE0R_ITEN5_1        (0x02UL << GPIO_ITYPE0R_ITEN5_Pos)          /* 0x00000800 */

#define GPIO_ITYPE0R_intType6_DPos  (12U)
#define GPIO_ITYPE0R_ITEN6_Pos      GPIO_ITYPE0R_intType6_DPos
#define GPIO_ITYPE0R_ITEN6_Msk      (0x03UL << GPIO_ITYPE0R_ITEN6_Pos)          /* 0x00003000 */
#define GPIO_ITYPE0R_ITEN6          GPIO_ITYPE0R_ITEN6_Msk                      /* intType6[1:0] bits (Select the channel 6 interrupt type) */
#define GPIO_ITYPE0R_ITEN6_0        (0x01UL << GPIO_ITYPE0R_ITEN6_Pos)          /* 0x00001000 */
#define GPIO_ITYPE0R_ITEN6_1        (0x02UL << GPIO_ITYPE0R_ITEN6_Pos)          /* 0x00002000 */

#define GPIO_ITYPE0R_intType7_DPos  (14U)
#define GPIO_ITYPE0R_ITEN7_Pos      GPIO_ITYPE0R_intType7_DPos
#define GPIO_ITYPE0R_ITEN7_Msk      (0x03UL << GPIO_ITYPE0R_ITEN7_Pos)          /* 0x0000C000 */
#define GPIO_ITYPE0R_ITEN7          GPIO_ITYPE0R_ITEN7_Msk                      /* intType7[1:0] bits (Select the channel 7 interrupt type) */
#define GPIO_ITYPE0R_ITEN7_0        (0x01UL << GPIO_ITYPE0R_ITEN7_Pos)          /* 0x00004000 */
#define GPIO_ITYPE0R_ITEN7_1        (0x02UL << GPIO_ITYPE0R_ITEN7_Pos)          /* 0x00008000 */

#define GPIO_ITYPE0R_intType8_DPos  (16U)
#define GPIO_ITYPE0R_ITEN8_Pos      GPIO_ITYPE0R_intType8_DPos
#define GPIO_ITYPE0R_ITEN8_Msk      (0x03UL << GPIO_ITYPE0R_ITEN8_Pos)          /* 0x00030000 */
#define GPIO_ITYPE0R_ITEN8          GPIO_ITYPE0R_ITEN8_Msk                      /* intType8[1:0] bits (Select the channel 8 interrupt type) */
#define GPIO_ITYPE0R_ITEN8_0        (0x01UL << GPIO_ITYPE0R_ITEN8_Pos)          /* 0x00010000 */
#define GPIO_ITYPE0R_ITEN8_1        (0x02UL << GPIO_ITYPE0R_ITEN8_Pos)          /* 0x00020000 */

#define GPIO_ITYPE0R_intType9_DPos  (18U)
#define GPIO_ITYPE0R_ITEN9_Pos      GPIO_ITYPE0R_intType9_DPos
#define GPIO_ITYPE0R_ITEN9_Msk      (0x03UL << GPIO_ITYPE0R_ITEN9_Pos)          /* 0x000C0000 */
#define GPIO_ITYPE0R_ITEN9          GPIO_ITYPE0R_ITEN9_Msk                      /* intType9[1:0] bits (Select the channel 9 interrupt type) */
#define GPIO_ITYPE0R_ITEN9_0        (0x01UL << GPIO_ITYPE0R_ITEN9_Pos)          /* 0x00040000 */
#define GPIO_ITYPE0R_ITEN9_1        (0x02UL << GPIO_ITYPE0R_ITEN9_Pos)          /* 0x00080000 */

#define GPIO_ITYPE0R_intType10_DPos (20U)
#define GPIO_ITYPE0R_ITEN10_Pos     GPIO_ITYPE0R_intType10_DPos
#define GPIO_ITYPE0R_ITEN10_Msk     (0x03UL << GPIO_ITYPE0R_ITEN10_Pos)         /* 0x00300000 */
#define GPIO_ITYPE0R_ITEN10         GPIO_ITYPE0R_ITEN10_Msk                     /* intType10[1:0] bits (Select the channel 10 interrupt type) */
#define GPIO_ITYPE0R_ITEN10_0       (0x01UL << GPIO_ITYPE0R_ITEN10_Pos)         /* 0x00100000 */
#define GPIO_ITYPE0R_ITEN10_1       (0x02UL << GPIO_ITYPE0R_ITEN10_Pos)         /* 0x00200000 */

#define GPIO_ITYPE0R_intType11_DPos (22U)
#define GPIO_ITYPE0R_ITEN11_Pos     GPIO_ITYPE0R_intType11_DPos
#define GPIO_ITYPE0R_ITEN11_Msk     (0x03UL << GPIO_ITYPE0R_ITEN11_Pos)         /* 0x00C00000 */
#define GPIO_ITYPE0R_ITEN11         GPIO_ITYPE0R_ITEN11_Msk                     /* intType11[1:0] bits (Select the channel 11 interrupt type) */
#define GPIO_ITYPE0R_ITEN11_0       (0x01UL << GPIO_ITYPE0R_ITEN11_Pos)         /* 0x00400000 */
#define GPIO_ITYPE0R_ITEN11_1       (0x02UL << GPIO_ITYPE0R_ITEN11_Pos)         /* 0x00800000 */

#define GPIO_ITYPE0R_intType12_DPos (24U)
#define GPIO_ITYPE0R_ITEN12_Pos     GPIO_ITYPE0R_intType12_DPos
#define GPIO_ITYPE0R_ITEN12_Msk     (0x03UL << GPIO_ITYPE0R_ITEN12_Pos)         /* 0x03000000 */
#define GPIO_ITYPE0R_ITEN12         GPIO_ITYPE0R_ITEN12_Msk                     /* intType12[1:0] bits (Select the channel 12 interrupt type) */
#define GPIO_ITYPE0R_ITEN12_0       (0x01UL << GPIO_ITYPE0R_ITEN12_Pos)         /* 0x01000000 */
#define GPIO_ITYPE0R_ITEN12_1       (0x02UL << GPIO_ITYPE0R_ITEN12_Pos)         /* 0x02000000 */

#define GPIO_ITYPE0R_intType13_DPos (26U)
#define GPIO_ITYPE0R_ITEN13_Pos     GPIO_ITYPE0R_intType13_DPos
#define GPIO_ITYPE0R_ITEN13_Msk     (0x03UL << GPIO_ITYPE0R_ITEN13_Pos)         /* 0x0C000000 */
#define GPIO_ITYPE0R_ITEN13         GPIO_ITYPE0R_ITEN13_Msk                     /* intType13[1:0] bits (Select the channel 13 interrupt type) */
#define GPIO_ITYPE0R_ITEN13_0       (0x01UL << GPIO_ITYPE0R_ITEN13_Pos)         /* 0x04000000 */
#define GPIO_ITYPE0R_ITEN13_1       (0x02UL << GPIO_ITYPE0R_ITEN13_Pos)         /* 0x08000000 */

#define GPIO_ITYPE0R_intType14_DPos (28U)
#define GPIO_ITYPE0R_ITEN14_Pos     GPIO_ITYPE0R_intType14_DPos
#define GPIO_ITYPE0R_ITEN14_Msk     (0x03UL << GPIO_ITYPE0R_ITEN14_Pos)         /* 0x30000000 */
#define GPIO_ITYPE0R_ITEN14         GPIO_ITYPE0R_ITEN14_Msk                     /* intType14[1:0] bits (Select the channel 14 interrupt type) */
#define GPIO_ITYPE0R_ITEN14_0       (0x01UL << GPIO_ITYPE0R_ITEN14_Pos)         /* 0x10000000 */
#define GPIO_ITYPE0R_ITEN14_1       (0x02UL << GPIO_ITYPE0R_ITEN14_Pos)         /* 0x20000000 */

#define GPIO_ITYPE0R_intType15_DPos (30U)
#define GPIO_ITYPE0R_ITEN15_Pos     GPIO_ITYPE0R_intType15_DPos
#define GPIO_ITYPE0R_ITEN15_Msk     (0x03UL << GPIO_ITYPE0R_ITEN15_Pos)         /* 0xC0000000 */
#define GPIO_ITYPE0R_ITEN15         GPIO_ITYPE0R_ITEN15_Msk                     /* intType15[1:0] bits (Select the channel 15 interrupt type) */
#define GPIO_ITYPE0R_ITEN15_0       (0x01UL << GPIO_ITYPE0R_ITEN15_Pos)         /* 0x40000000 */
#define GPIO_ITYPE0R_ITEN15_1       (0x02UL << GPIO_ITYPE0R_ITEN15_Pos)         /* 0x80000000 */


/******************  Bits definition for GPIO_IRSR register  *****************/
#define GPIO_IRSR_Irq0_raw_DPos     (0U)
#define GPIO_IRSR_IRSR0_Pos         GPIO_IRSR_Irq0_raw_DPos
#define GPIO_IRSR_IRSR0_Msk         (0x01UL << GPIO_IRSR_IRSR0_Pos)             /* 0x00000001 */
#define GPIO_IRSR_IRSR0             GPIO_IRSR_IRSR0_Msk                         /* Interrupt Raw status of channel 0 */

#define GPIO_IRSR_Irq1_raw_DPos     (1U)
#define GPIO_IRSR_IRSR1_Pos         GPIO_IRSR_Irq1_raw_DPos
#define GPIO_IRSR_IRSR1_Msk         (0x01UL << GPIO_IRSR_IRSR1_Pos)             /* 0x00000002 */
#define GPIO_IRSR_IRSR1             GPIO_IRSR_IRSR1_Msk                         /* Interrupt Raw status of channel 1 */

#define GPIO_IRSR_Irq2_raw_DPos     (2U)
#define GPIO_IRSR_IRSR2_Pos         GPIO_IRSR_Irq2_raw_DPos
#define GPIO_IRSR_IRSR2_Msk         (0x01UL << GPIO_IRSR_IRSR2_Pos)             /* 0x00000004 */
#define GPIO_IRSR_IRSR2             GPIO_IRSR_IRSR2_Msk                         /* Interrupt Raw status of channel 2 */

#define GPIO_IRSR_Irq3_raw_DPos     (3U)
#define GPIO_IRSR_IRSR3_Pos         GPIO_IRSR_Irq3_raw_DPos
#define GPIO_IRSR_IRSR3_Msk         (0x01UL << GPIO_IRSR_IRSR3_Pos)             /* 0x00000008 */
#define GPIO_IRSR_IRSR3             GPIO_IRSR_IRSR3_Msk                         /* Interrupt Raw status of channel 3 */

#define GPIO_IRSR_Irq4_raw_DPos     (4U)
#define GPIO_IRSR_IRSR4_Pos         GPIO_IRSR_Irq4_raw_DPos
#define GPIO_IRSR_IRSR4_Msk         (0x01UL << GPIO_IRSR_IRSR4_Pos)             /* 0x00000010 */
#define GPIO_IRSR_IRSR4             GPIO_IRSR_IRSR4_Msk                         /* Interrupt Raw status of channel 4 */

#define GPIO_IRSR_Irq5_raw_DPos     (5U)
#define GPIO_IRSR_IRSR5_Pos         GPIO_IRSR_Irq5_raw_DPos
#define GPIO_IRSR_IRSR5_Msk         (0x01UL << GPIO_IRSR_IRSR5_Pos)             /* 0x00000020 */
#define GPIO_IRSR_IRSR5             GPIO_IRSR_IRSR5_Msk                         /* Interrupt Raw status of channel 5 */

#define GPIO_IRSR_Irq6_raw_DPos     (6U)
#define GPIO_IRSR_IRSR6_Pos         GPIO_IRSR_Irq6_raw_DPos
#define GPIO_IRSR_IRSR6_Msk         (0x01UL << GPIO_IRSR_IRSR6_Pos)             /* 0x00000040 */
#define GPIO_IRSR_IRSR6             GPIO_IRSR_IRSR6_Msk                         /* Interrupt Raw status of channel 6 */

#define GPIO_IRSR_Irq7_raw_DPos     (7U)
#define GPIO_IRSR_IRSR7_Pos         GPIO_IRSR_Irq7_raw_DPos
#define GPIO_IRSR_IRSR7_Msk         (0x01UL << GPIO_IRSR_IRSR7_Pos)             /* 0x00000080 */
#define GPIO_IRSR_IRSR7             GPIO_IRSR_IRSR7_Msk                         /* Interrupt Raw status of channel 7 */

#define GPIO_IRSR_Irq8_raw_DPos     (8U)
#define GPIO_IRSR_IRSR8_Pos         GPIO_IRSR_Irq8_raw_DPos
#define GPIO_IRSR_IRSR8_Msk         (0x01UL << GPIO_IRSR_IRSR8_Pos)             /* 0x00000100 */
#define GPIO_IRSR_IRSR8             GPIO_IRSR_IRSR8_Msk                         /* Interrupt Raw status of channel 8 */

#define GPIO_IRSR_Irq9_raw_DPos     (9U)
#define GPIO_IRSR_IRSR9_Pos         GPIO_IRSR_Irq9_raw_DPos
#define GPIO_IRSR_IRSR9_Msk         (0x01UL << GPIO_IRSR_IRSR9_Pos)             /* 0x00000200 */
#define GPIO_IRSR_IRSR9             GPIO_IRSR_IRSR9_Msk                         /* Interrupt Raw status of channel 9 */

#define GPIO_IRSR_Irq10_raw_DPos    (10U)
#define GPIO_IRSR_IRSR10_Pos        GPIO_IRSR_Irq10_raw_DPos
#define GPIO_IRSR_IRSR10_Msk        (0x01UL << GPIO_IRSR_IRSR10_Pos)            /* 0x00000400 */
#define GPIO_IRSR_IRSR10            GPIO_IRSR_IRSR10_Msk                        /* Interrupt Raw status of channel 10 */

#define GPIO_IRSR_Irq11_raw_DPos    (11U)
#define GPIO_IRSR_IRSR11_Pos        GPIO_IRSR_Irq11_raw_DPos
#define GPIO_IRSR_IRSR11_Msk        (0x01UL << GPIO_IRSR_IRSR11_Pos)            /* 0x00000800 */
#define GPIO_IRSR_IRSR11            GPIO_IRSR_IRSR11_Msk                        /* Interrupt Raw status of channel 11 */

#define GPIO_IRSR_Irq12_raw_DPos    (12U)
#define GPIO_IRSR_IRSR12_Pos        GPIO_IRSR_Irq12_raw_DPos
#define GPIO_IRSR_IRSR12_Msk        (0x01UL << GPIO_IRSR_IRSR12_Pos)            /* 0x00001000 */
#define GPIO_IRSR_IRSR12            GPIO_IRSR_IRSR12_Msk                        /* Interrupt Raw status of channel 12 */

#define GPIO_IRSR_Irq13_raw_DPos    (13U)
#define GPIO_IRSR_IRSR13_Pos        GPIO_IRSR_Irq13_raw_DPos
#define GPIO_IRSR_IRSR13_Msk        (0x01UL << GPIO_IRSR_IRSR13_Pos)            /* 0x00002000 */
#define GPIO_IRSR_IRSR13            GPIO_IRSR_IRSR13_Msk                        /* Interrupt Raw status of channel 13 */

#define GPIO_IRSR_Irq14_raw_DPos    (14U)
#define GPIO_IRSR_IRSR14_Pos        GPIO_IRSR_Irq14_raw_DPos
#define GPIO_IRSR_IRSR14_Msk        (0x01UL << GPIO_IRSR_IRSR14_Pos)            /* 0x00004000 */
#define GPIO_IRSR_IRSR14            GPIO_IRSR_IRSR14_Msk                        /* Interrupt Raw status of channel 14 */

#define GPIO_IRSR_Irq15_raw_DPos    (15U)
#define GPIO_IRSR_IRSR15_Pos        GPIO_IRSR_Irq15_raw_DPos
#define GPIO_IRSR_IRSR15_Msk        (0x01UL << GPIO_IRSR_IRSR15_Pos)            /* 0x00008000 */
#define GPIO_IRSR_IRSR15            GPIO_IRSR_IRSR15_Msk                        /* Interrupt Raw status of channel 15 */


/******************  Bits definition for GPIO_IFR register  *****************/
#define GPIO_IFR_Irq0_force_DPos    (0U)
#define GPIO_IFR_IFR0_Pos           GPIO_IFR_Irq0_force_DPos
#define GPIO_IFR_IFR0_Msk           (0x01UL << GPIO_IFR_IFR0_Pos)               /* 0x00000001 */
#define GPIO_IFR_IFR0               GPIO_IFR_IFR0_Msk                           /* Channel 0 interrupt force */

#define GPIO_IFR_Irq1_force_DPos    (1U)
#define GPIO_IFR_IFR1_Pos           GPIO_IFR_Irq1_force_DPos
#define GPIO_IFR_IFR1_Msk           (0x01UL << GPIO_IFR_IFR1_Pos)               /* 0x00000002 */
#define GPIO_IFR_IFR1               GPIO_IFR_IFR1_Msk                           /* Channel 1 interrupt force */

#define GPIO_IFR_Irq2_force_DPos    (2U)
#define GPIO_IFR_IFR2_Pos           GPIO_IFR_Irq2_force_DPos
#define GPIO_IFR_IFR2_Msk           (0x01UL << GPIO_IFR_IFR2_Pos)               /* 0x00000004 */
#define GPIO_IFR_IFR2               GPIO_IFR_IFR2_Msk                           /* Channel 2 interrupt force */

#define GPIO_IFR_Irq3_force_DPos    (3U)
#define GPIO_IFR_IFR3_Pos           GPIO_IFR_Irq3_force_DPos
#define GPIO_IFR_IFR3_Msk           (0x01UL << GPIO_IFR_IFR3_Pos)               /* 0x00000008 */
#define GPIO_IFR_IFR3               GPIO_IFR_IFR3_Msk                           /* Channel 3 interrupt force */

#define GPIO_IFR_Irq4_force_DPos    (4U)
#define GPIO_IFR_IFR4_Pos           GPIO_IFR_Irq4_force_DPos
#define GPIO_IFR_IFR4_Msk           (0x01UL << GPIO_IFR_IFR4_Pos)               /* 0x00000010 */
#define GPIO_IFR_IFR4               GPIO_IFR_IFR4_Msk                           /* Channel 4 interrupt force */

#define GPIO_IFR_Irq5_force_DPos    (5U)
#define GPIO_IFR_IFR5_Pos           GPIO_IFR_Irq5_force_DPos
#define GPIO_IFR_IFR5_Msk           (0x01UL << GPIO_IFR_IFR5_Pos)               /* 0x00000020 */
#define GPIO_IFR_IFR5               GPIO_IFR_IFR5_Msk                           /* Channel 5 interrupt force */

#define GPIO_IFR_Irq6_force_DPos    (6U)
#define GPIO_IFR_IFR6_Pos           GPIO_IFR_Irq6_force_DPos
#define GPIO_IFR_IFR6_Msk           (0x01UL << GPIO_IFR_IFR6_Pos)               /* 0x00000040 */
#define GPIO_IFR_IFR6               GPIO_IFR_IFR6_Msk                           /* Channel 6 interrupt force */

#define GPIO_IFR_Irq7_force_DPos    (7U)
#define GPIO_IFR_IFR7_Pos           GPIO_IFR_Irq7_force_DPos
#define GPIO_IFR_IFR7_Msk           (0x01UL << GPIO_IFR_IFR7_Pos)               /* 0x00000080 */
#define GPIO_IFR_IFR7               GPIO_IFR_IFR7_Msk                           /* Channel 7 interrupt force */

#define GPIO_IFR_Irq8_force_DPos    (8U)
#define GPIO_IFR_IFR8_Pos           GPIO_IFR_Irq8_force_DPos
#define GPIO_IFR_IFR8_Msk           (0x01UL << GPIO_IFR_IFR8_Pos)               /* 0x00000100 */
#define GPIO_IFR_IFR8               GPIO_IFR_IFR8_Msk                           /* Channel 8 interrupt force */

#define GPIO_IFR_Irq9_force_DPos    (9U)
#define GPIO_IFR_IFR9_Pos           GPIO_IFR_Irq9_force_DPos
#define GPIO_IFR_IFR9_Msk           (0x01UL << GPIO_IFR_IFR9_Pos)               /* 0x00000200 */
#define GPIO_IFR_IFR9               GPIO_IFR_IFR9_Msk                           /* Channel 9 interrupt force */

#define GPIO_IFR_Irq10_force_DPos   (10U)
#define GPIO_IFR_IFR10_Pos          GPIO_IFR_Irq10_force_DPos
#define GPIO_IFR_IFR10_Msk          (0x01UL << GPIO_IFR_IFR10_Pos)              /* 0x00000400 */
#define GPIO_IFR_IFR10              GPIO_IFR_IFR10_Msk                          /* Channel 10 interrupt force */

#define GPIO_IFR_Irq11_force_DPos   (11U)
#define GPIO_IFR_IFR11_Pos          GPIO_IFR_Irq11_force_DPos
#define GPIO_IFR_IFR11_Msk          (0x01UL << GPIO_IFR_IFR11_Pos)              /* 0x00000800 */
#define GPIO_IFR_IFR11              GPIO_IFR_IFR11_Msk                          /* Channel 11 interrupt force */

#define GPIO_IFR_Irq12_force_DPos   (12U)
#define GPIO_IFR_IFR12_Pos          GPIO_IFR_Irq12_force_DPos
#define GPIO_IFR_IFR12_Msk          (0x01UL << GPIO_IFR_IFR12_Pos)              /* 0x00001000 */
#define GPIO_IFR_IFR12              GPIO_IFR_IFR12_Msk                          /* Channel 12 interrupt force */

#define GPIO_IFR_Irq13_force_DPos   (13U)
#define GPIO_IFR_IFR13_Pos          GPIO_IFR_Irq13_force_DPos
#define GPIO_IFR_IFR13_Msk          (0x01UL << GPIO_IFR_IFR13_Pos)              /* 0x00002000 */
#define GPIO_IFR_IFR13              GPIO_IFR_IFR13_Msk                          /* Channel 13 interrupt force */

#define GPIO_IFR_Irq14_force_DPos   (14U)
#define GPIO_IFR_IFR14_Pos          GPIO_IFR_Irq14_force_DPos
#define GPIO_IFR_IFR14_Msk          (0x01UL << GPIO_IFR_IFR14_Pos)              /* 0x00004000 */
#define GPIO_IFR_IFR14              GPIO_IFR_IFR14_Msk                          /* Channel 14 interrupt force */

#define GPIO_IFR_Irq15_force_DPos   (15U)
#define GPIO_IFR_IFR15_Pos          GPIO_IFR_Irq15_force_DPos
#define GPIO_IFR_IFR15_Msk          (0x01UL << GPIO_IFR_IFR15_Pos)              /* 0x00008000 */
#define GPIO_IFR_IFR15              GPIO_IFR_IFR15_Msk                          /* Channel 15 interrupt force */


/******************  Bits definition for GPIO_IMR register  *****************/
#define GPIO_IMR_Irq0_mask_DPos     (0U)
#define GPIO_IMR_IMR0_Pos           GPIO_IMR_Irq0_mask_DPos
#define GPIO_IMR_IMR0_Msk           (0x01UL << GPIO_IMR_IMR0_Pos)               /* 0x00000001 */
#define GPIO_IMR_IMR0               GPIO_IMR_IMR0_Msk                           /* Channel 0 interrupt mask */

#define GPIO_IMR_Irq1_mask_DPos     (1U)
#define GPIO_IMR_IMR1_Pos           GPIO_IMR_Irq1_mask_DPos
#define GPIO_IMR_IMR1_Msk           (0x01UL << GPIO_IMR_IMR1_Pos)               /* 0x00000002 */
#define GPIO_IMR_IMR1               GPIO_IMR_IMR1_Msk                           /* Channel 1 interrupt mask */

#define GPIO_IMR_Irq2_mask_DPos     (2U)
#define GPIO_IMR_IMR2_Pos           GPIO_IMR_Irq2_mask_DPos
#define GPIO_IMR_IMR2_Msk           (0x01UL << GPIO_IMR_IMR2_Pos)               /* 0x00000004 */
#define GPIO_IMR_IMR2               GPIO_IMR_IMR2_Msk                           /* Channel 2 interrupt mask */

#define GPIO_IMR_Irq3_mask_DPos     (3U)
#define GPIO_IMR_IMR3_Pos           GPIO_IMR_Irq3_mask_DPos
#define GPIO_IMR_IMR3_Msk           (0x01UL << GPIO_IMR_IMR3_Pos)               /* 0x00000008 */
#define GPIO_IMR_IMR3               GPIO_IMR_IMR3_Msk                           /* Channel 3 interrupt mask */

#define GPIO_IMR_Irq4_mask_DPos     (4U)
#define GPIO_IMR_IMR4_Pos           GPIO_IMR_Irq4_mask_DPos
#define GPIO_IMR_IMR4_Msk           (0x01UL << GPIO_IMR_IMR4_Pos)               /* 0x00000010 */
#define GPIO_IMR_IMR4               GPIO_IMR_IMR4_Msk                           /* Channel 4 interrupt mask */

#define GPIO_IMR_Irq5_mask_DPos     (5U)
#define GPIO_IMR_IMR5_Pos           GPIO_IMR_Irq5_mask_DPos
#define GPIO_IMR_IMR5_Msk           (0x01UL << GPIO_IMR_IMR5_Pos)               /* 0x00000020 */
#define GPIO_IMR_IMR5               GPIO_IMR_IMR5_Msk                           /* Channel 5 interrupt mask */

#define GPIO_IMR_Irq6_mask_DPos     (6U)
#define GPIO_IMR_IMR6_Pos           GPIO_IMR_Irq6_mask_DPos
#define GPIO_IMR_IMR6_Msk           (0x01UL << GPIO_IMR_IMR6_Pos)               /* 0x00000040 */
#define GPIO_IMR_IMR6               GPIO_IMR_IMR6_Msk                           /* Channel 6 interrupt mask */

#define GPIO_IMR_Irq7_mask_DPos     (7U)
#define GPIO_IMR_IMR7_Pos           GPIO_IMR_Irq7_mask_DPos
#define GPIO_IMR_IMR7_Msk           (0x01UL << GPIO_IMR_IMR7_Pos)               /* 0x00000080 */
#define GPIO_IMR_IMR7               GPIO_IMR_IMR7_Msk                           /* Channel 7 interrupt mask */

#define GPIO_IMR_Irq8_mask_DPos     (8U)
#define GPIO_IMR_IMR8_Pos           GPIO_IMR_Irq8_mask_DPos
#define GPIO_IMR_IMR8_Msk           (0x01UL << GPIO_IMR_IMR8_Pos)               /* 0x00000100 */
#define GPIO_IMR_IMR8               GPIO_IMR_IMR8_Msk                           /* Channel 8 interrupt mask */

#define GPIO_IMR_Irq9_mask_DPos     (9U)
#define GPIO_IMR_IMR9_Pos           GPIO_IMR_Irq9_mask_DPos
#define GPIO_IMR_IMR9_Msk           (0x01UL << GPIO_IMR_IMR9_Pos)               /* 0x00000200 */
#define GPIO_IMR_IMR9               GPIO_IMR_IMR9_Msk                           /* Channel 9 interrupt mask */

#define GPIO_IMR_Irq10_mask_DPos    (10U)
#define GPIO_IMR_IMR10_Pos          GPIO_IMR_Irq10_mask_DPos
#define GPIO_IMR_IMR10_Msk          (0x01UL << GPIO_IMR_IMR10_Pos)              /* 0x00000400 */
#define GPIO_IMR_IMR10              GPIO_IMR_IMR10_Msk                          /* Channel 10 interrupt mask */

#define GPIO_IMR_Irq11_mask_DPos    (11U)
#define GPIO_IMR_IMR11_Pos          GPIO_IMR_Irq11_mask_DPos
#define GPIO_IMR_IMR11_Msk          (0x01UL << GPIO_IMR_IMR11_Pos)              /* 0x00000800 */
#define GPIO_IMR_IMR11              GPIO_IMR_IMR11_Msk                          /* Channel 11 interrupt mask */

#define GPIO_IMR_Irq12_mask_DPos    (12U)
#define GPIO_IMR_IMR12_Pos          GPIO_IMR_Irq12_mask_DPos
#define GPIO_IMR_IMR12_Msk          (0x01UL << GPIO_IMR_IMR12_Pos)              /* 0x00001000 */
#define GPIO_IMR_IMR12              GPIO_IMR_IMR12_Msk                          /* Channel 12 interrupt mask */

#define GPIO_IMR_Irq13_mask_DPos    (13U)
#define GPIO_IMR_IMR13_Pos          GPIO_IMR_Irq13_mask_DPos
#define GPIO_IMR_IMR13_Msk          (0x01UL << GPIO_IMR_IMR13_Pos)              /* 0x00002000 */
#define GPIO_IMR_IMR13              GPIO_IMR_IMR13_Msk                          /* Channel 13 interrupt mask */

#define GPIO_IMR_Irq14_mask_DPos    (14U)
#define GPIO_IMR_IMR14_Pos          GPIO_IMR_Irq14_mask_DPos
#define GPIO_IMR_IMR14_Msk          (0x01UL << GPIO_IMR_IMR14_Pos)              /* 0x00004000 */
#define GPIO_IMR_IMR14              GPIO_IMR_IMR14_Msk                          /* Channel 14 interrupt mask */

#define GPIO_IMR_Irq15_mask_DPos    (15U)
#define GPIO_IMR_IMR15_Pos          GPIO_IMR_Irq15_mask_DPos
#define GPIO_IMR_IMR15_Msk          (0x01UL << GPIO_IMR_IMR15_Pos)              /* 0x00008000 */
#define GPIO_IMR_IMR15              GPIO_IMR_IMR15_Msk                          /* Channel 15 interrupt mask */


/******************  Bits definition for GPIO_ISR register  *****************/
#define GPIO_ISR_Irq0_status_DPos   (0U)
#define GPIO_ISR_ISR0_Pos           GPIO_ISR_Irq0_status_DPos
#define GPIO_ISR_ISR0_Msk           (0x01UL << GPIO_ISR_ISR0_Pos)               /* 0x00000001 */
#define GPIO_ISR_ISR0               GPIO_ISR_ISR0_Msk                           /* Channel 0 interrupt status */

#define GPIO_ISR_Irq1_status_DPos   (1U)
#define GPIO_ISR_ISR1_Pos           GPIO_ISR_Irq1_status_DPos
#define GPIO_ISR_ISR1_Msk           (0x01UL << GPIO_ISR_ISR1_Pos)               /* 0x00000002 */
#define GPIO_ISR_ISR1               GPIO_ISR_ISR1_Msk                           /* Channel 1 interrupt status */

#define GPIO_ISR_Irq2_status_DPos   (2U)
#define GPIO_ISR_ISR2_Pos           GPIO_ISR_Irq2_status_DPos
#define GPIO_ISR_ISR2_Msk           (0x01UL << GPIO_ISR_ISR2_Pos)               /* 0x00000004 */
#define GPIO_ISR_ISR2               GPIO_ISR_ISR2_Msk                           /* Channel 2 interrupt status */

#define GPIO_ISR_Irq3_status_DPos   (3U)
#define GPIO_ISR_ISR3_Pos           GPIO_ISR_Irq3_status_DPos
#define GPIO_ISR_ISR3_Msk           (0x01UL << GPIO_ISR_ISR3_Pos)               /* 0x00000008 */
#define GPIO_ISR_ISR3               GPIO_ISR_ISR3_Msk                           /* Channel 3 interrupt status */

#define GPIO_ISR_Irq4_status_DPos   (4U)
#define GPIO_ISR_ISR4_Pos           GPIO_ISR_Irq4_status_DPos
#define GPIO_ISR_ISR4_Msk           (0x01UL << GPIO_ISR_ISR4_Pos)               /* 0x00000010 */
#define GPIO_ISR_ISR4               GPIO_ISR_ISR4_Msk                           /* Channel 4 interrupt status */

#define GPIO_ISR_Irq5_status_DPos   (5U)
#define GPIO_ISR_ISR5_Pos           GPIO_ISR_Irq5_status_DPos
#define GPIO_ISR_ISR5_Msk           (0x01UL << GPIO_ISR_ISR5_Pos)               /* 0x00000020 */
#define GPIO_ISR_ISR5               GPIO_ISR_ISR5_Msk                           /* Channel 5 interrupt status */

#define GPIO_ISR_Irq6_status_DPos   (6U)
#define GPIO_ISR_ISR6_Pos           GPIO_ISR_Irq6_status_DPos
#define GPIO_ISR_ISR6_Msk           (0x01UL << GPIO_ISR_ISR6_Pos)               /* 0x00000040 */
#define GPIO_ISR_ISR6               GPIO_ISR_ISR6_Msk                           /* Channel 6 interrupt status */

#define GPIO_ISR_Irq7_status_DPos   (7U)
#define GPIO_ISR_ISR7_Pos           GPIO_ISR_Irq7_status_DPos
#define GPIO_ISR_ISR7_Msk           (0x01UL << GPIO_ISR_ISR7_Pos)               /* 0x00000080 */
#define GPIO_ISR_ISR7               GPIO_ISR_ISR7_Msk                           /* Channel 7 interrupt status */

#define GPIO_ISR_Irq8_status_DPos   (8U)
#define GPIO_ISR_ISR8_Pos           GPIO_ISR_Irq8_status_DPos
#define GPIO_ISR_ISR8_Msk           (0x01UL << GPIO_ISR_ISR8_Pos)               /* 0x00000100 */
#define GPIO_ISR_ISR8               GPIO_ISR_ISR8_Msk                           /* Channel 8 interrupt status */

#define GPIO_ISR_Irq9_status_DPos   (9U)
#define GPIO_ISR_ISR9_Pos           GPIO_ISR_Irq9_status_DPos
#define GPIO_ISR_ISR9_Msk           (0x01UL << GPIO_ISR_ISR9_Pos)               /* 0x00000200 */
#define GPIO_ISR_ISR9               GPIO_ISR_ISR9_Msk                           /* Channel 9 interrupt status */

#define GPIO_ISR_Irq10_status_DPos  (10U)
#define GPIO_ISR_ISR10_Pos          GPIO_ISR_Irq10_status_DPos
#define GPIO_ISR_ISR10_Msk          (0x01UL << GPIO_ISR_ISR10_Pos)              /* 0x00000400 */
#define GPIO_ISR_ISR10              GPIO_ISR_ISR10_Msk                          /* Channel 10 interrupt status */

#define GPIO_ISR_Irq11_status_DPos  (11U)
#define GPIO_ISR_ISR11_Pos          GPIO_ISR_Irq11_status_DPos
#define GPIO_ISR_ISR11_Msk          (0x01UL << GPIO_ISR_ISR11_Pos)              /* 0x00000800 */
#define GPIO_ISR_ISR11              GPIO_ISR_ISR11_Msk                          /* Channel 11 interrupt status */

#define GPIO_ISR_Irq12_status_DPos  (12U)
#define GPIO_ISR_ISR12_Pos          GPIO_ISR_Irq12_status_DPos
#define GPIO_ISR_ISR12_Msk          (0x01UL << GPIO_ISR_ISR12_Pos)              /* 0x00001000 */
#define GPIO_ISR_ISR12              GPIO_ISR_ISR12_Msk                          /* Channel 12 interrupt status */

#define GPIO_ISR_Irq13_status_DPos  (13U)
#define GPIO_ISR_ISR13_Pos          GPIO_ISR_Irq13_status_DPos
#define GPIO_ISR_ISR13_Msk          (0x01UL << GPIO_ISR_ISR13_Pos)              /* 0x00002000 */
#define GPIO_ISR_ISR13              GPIO_ISR_ISR13_Msk                          /* Channel 13 interrupt status */

#define GPIO_ISR_Irq14_status_DPos  (14U)
#define GPIO_ISR_ISR14_Pos          GPIO_ISR_Irq14_status_DPos
#define GPIO_ISR_ISR14_Msk          (0x01UL << GPIO_ISR_ISR14_Pos)              /* 0x00004000 */
#define GPIO_ISR_ISR14              GPIO_ISR_ISR14_Msk                          /* Channel 14 interrupt status */

#define GPIO_ISR_Irq15_status_DPos  (15U)
#define GPIO_ISR_ISR15_Pos          GPIO_ISR_Irq15_status_DPos
#define GPIO_ISR_ISR15_Msk          (0x01UL << GPIO_ISR_ISR15_Pos)              /* 0x00008000 */
#define GPIO_ISR_ISR15              GPIO_ISR_ISR15_Msk                          /* Channel 15 interrupt status */


#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
