/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):   Xiamen Guoke Anxin Technology Co., LTD
 * FileName:        as32x601_smu.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:            2023/11/21
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2023/11/21     1.0     build this module
 * Description:     This file contains all the functions prototypes for the SMU firmware library.
 */

#ifndef AS32X601_SMU_H
#define AS32X601_SMU_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601_smu_regs.h"
#include "as32x601_core.h"


/*
 * @brief System clock type enumeration
 */
typedef enum 
{
    SMU_OSCILLATORTYPE_FIRC = 0x00,     /* FIRC serves as the SYS clock */
    SMU_OSCILLATORTYPE_OSC  = 0x01,     /* OSC serves as the SYS clock */
} SMUOSCILLATOR_TypeDef;
#define IS_SMU_OSCILLATOR(OSCCLKSELECT) (((OSCCLKSELECT) == SMU_OSCILLATORTYPE_OSC) || ((OSCCLKSELECT) == SMU_OSCILLATORTYPE_FIRC))

/*
 * @brief System clock type enumeration
 */
typedef enum 
{
    SMU_PLLCLK_FIRC = 0x00,             /* FIRC serves as the PLL clock */
    SMU_PLLCLK_OSC  = 0x01              /* OSC serves as the PLL clock */
} SMUPLLCLKSelect_TypeDef;
#define IS_SMU_PLLCLKSELECT(PLLCLKSELECT) (((PLLCLKSELECT) == SMU_PLLCLK_OSC) || ((PLLCLKSELECT) == SMU_PLLCLK_FIRC))

/*
 * @brief System clock type enumeration
 */
typedef enum 
{
    SMU_SYSCLK_FIRC = 0x00,	            /* FIRC serves as the system clock */
    SMU_SYSCLK_PLL  = 0x01,	            /* PLL serves as the system clock */
    SMU_SYSCLK_OSC  = 0x02	            /* OSC serves as the system clock */
} SMUSYSCLKSelect_TypeDef;
#define IS_SMU_SYSCLKSELECT(SYSCLKSELECT) (((SYSCLKSELECT) == SMU_SYSCLK_OSC) || ((SYSCLKSELECT) == SMU_SYSCLK_PLL) || \
                                           ((SYSCLKSELECT) == SMU_SYSCLK_FIRC))

/*
 * @brief CANCLKX2 clock divider parameters enumeration
 */
typedef enum 
{
    CANX2CLKDiv1 = 0x01,                /* AXILiteBus2 clock frequency division 1 */
    CANX2CLKDiv2 = 0x02,                /* AXILiteBus2 clock frequency division 2 */
    CANX2CLKDiv4 = 0x04,                /* AXILiteBus2 clock frequency division 4 */
    CANX2CLKDiv8 = 0x08                 /* AXILiteBus2 clock frequency division 8 */
} SMUCANX2CLKDiv_TypeDef;
#define IS_SMU_CANX2CLKDIV(CANX2CLKDIV) (((CANX2CLKDIV) == CANX2CLKDiv1) || ((CANX2CLKDIV) == CANX2CLKDiv2) || \
                                         ((CANX2CLKDIV) == CANX2CLKDiv4) || ((CANX2CLKDIV) == CANX2CLKDiv8))

/*
 * @brief APBBus1 clock divider parameters enumeration
 */
typedef enum 
{
    APBBus1CLKDiv1 = 0x01,              /* APBBus1 clock frequency division 1 */
    APBBus1CLKDiv2 = 0x02,              /* APBBus1 clock frequency division 2 */
    APBBus1CLKDiv4 = 0x04,              /* APBBus1 clock frequency division 4 */
    APBBus1CLKDiv8 = 0x08               /* APBBus1 clock frequency division 8 */
} SMUAPBBus1CLKDiv_TypeDef;
#define IS_SMU_APBBUS1CLKDIV(APBBUS1CLKDIV) (((APBBUS1CLKDIV) == APBBus1CLKDiv1) || ((APBBUS1CLKDIV) == APBBus1CLKDiv2) || \
                                             ((APBBUS1CLKDIV) == APBBus1CLKDiv4) || ((APBBUS1CLKDIV) == APBBus1CLKDiv8))
/*
 * @brief APBBus0 clock divider parameters enumeration
 */
typedef enum 
{
    APBBus0CLKDiv1 = 0x01,              /* APBBus0 clock frequency division 1 */
    APBBus0CLKDiv2 = 0x02,              /* APBBus0 clock frequency division 2 */
    APBBus0CLKDiv4 = 0x04,              /* APBBus0 clock frequency division 4 */
    APBBus0CLKDiv8 = 0x08               /* APBBus0 clock frequency division 8 */
} SMUAPBBus0CLKDiv_TypeDef;
#define IS_SMU_APBBUS0CLKDIV(APBBUS0CLKDIV) (((APBBUS0CLKDIV) == APBBus0CLKDiv1) || ((APBBUS0CLKDIV) == APBBus0CLKDiv2) || \
                                             ((APBBUS0CLKDIV) == APBBus0CLKDiv4) || ((APBBUS0CLKDIV) == APBBus0CLKDiv8))

/*
 * @brief AXI4Bus3 clock divider parameters enumeration
 */
typedef enum 
{
    AXI4Bus3CLKDiv1 = 0x01,             /* AXI4Bus3 clock frequency division 1 */
    AXI4Bus3CLKDiv2 = 0x02              /* AXI4Bus3 clock frequency division 2 */
} SMUAXI4Bus3CLKDiv_TypeDef;
#define IS_SMU_AXI4BUS3CLKDIV(AXI4BUS3CLKDIV) (((AXI4BUS3CLKDIV) == AXI4Bus3CLKDiv1) || \
                                               ((AXI4BUS3CLKDIV) == AXI4Bus3CLKDiv2))

/*
 * @brief Timer brake enable signal enumeration
 */
typedef enum 
{
    Timer0Break = 0x01,                 /* Timer 0 break bit */
    Timer1Break = 0x02,                 /* Timer 1 break bit */
    Timer2Break = 0x04,                 /* Timer 2 break bit */
    Timer5Break = 0x08                  /* Timer 5 break bit */
} SMUTimerBreak_TypeDef;
#define IS_SMU_TIMEBREAK(TIMEBREAK) (((TIMEBREAK) == Timer0Break) || ((TIMEBREAK) == Timer1Break) || \
                                     ((TIMEBREAK) == Timer2Break) || ((TIMEBREAK) == Timer5Break))

/*
 * @brief SRAM low-power enable signal enumeration
 */
typedef enum 
{
    Sram0LowPower = 0x01,               /* SRAM0 low-power bit */
    Sram1LowPower = 0x02,               /* SRAM1 low-power bit */
    Sram2LowPower = 0x04,               /* SRAM2 low-power bit */
    Sram3LowPower = 0x08                /* SRAM3 low-power bit */
} SMUSramLowPower_TypeDef;
#define IS_SMU_SRAMLOWPOWER(SRAMLOWPOWER) (((SRAMLOWPOWER) == Sram0LowPower) || ((SRAMLOWPOWER) == Sram1LowPower) || \
                                           ((SRAMLOWPOWER) == Sram2LowPower) || ((SRAMLOWPOWER) == Sram3LowPower))

/*
 * @brief The system clock is divided by 8 and output enumeration
 */
typedef enum 
{
    CoreCLKDiv8Out    = 0x00,           /* CoreClk's 8-division clock output */
    AxiBus1ClkDiv8Out = 0x01,           /* AxiBus1Clk's 8-division clock output */
    AxiBus3ClkDiv8Out = 0x02,           /* AxiBus3Clk's 8-division clock output */
    ApbBus0ClkDiv8Out = 0x03,           /* ApbBus0Clk's 8-division clock output */
    ApbBus1ClkDiv8Out = 0x04,           /* ApbBus1Clk's 8-division clock output */
    FircClkDiv8Out    = 0x05,           /* FircClk's 8-division clock output */
    OscClkDiv8Out     = 0x06,           /* Osc's 8-division clock output */
    SircClkOut        = 0x07            /* SircClk's clock output */
} SMUCLKOutMux_TypeDef;
#define IS_SMU_CLKOUTMUX(CLKOUTMUX) (((CLKOUTMUX) == CoreCLKDiv8Out)    || ((CLKOUTMUX) == AxiBus1ClkDiv8Out) || \
                                     ((CLKOUTMUX) == AxiBus3ClkDiv8Out) || ((CLKOUTMUX) == ApbBus0ClkDiv8Out) || \
                                     ((CLKOUTMUX) == ApbBus1ClkDiv8Out) || ((CLKOUTMUX) == FircClkDiv8Out)    || \
                                     ((CLKOUTMUX) == OscClkDiv8Out)     || ((CLKOUTMUX) == SircClkOut))

/*
 * @brief PLL frequency division value structure definition
 */
typedef struct
{
    EnState PLLState;                   /* PLL state enable*/
    SMUPLLCLKSelect_TypeDef PLLSource;  /* PLL source select */
    uint32_t PLLDivR;                   /* CLK_R output frequency division control signal */
    uint32_t PLLDivQ;                   /* CLK_Q output frequency division control signal */
    uint32_t PLLDivN;                   /* CLK_N output frequency division control signal */
    uint32_t PLLDivF;                   /* Feedback clock control signal */
}SMU_PLLConfigTypeDef;
#define IS_SMU_PLLR_VALUE(VALUE) ((1U <= (VALUE)) && ((VALUE) <= 7U))
#define IS_SMU_PLLQ_VALUE(VALUE) ((1U <= (VALUE)) && ((VALUE) <= 15U))
#define IS_SMU_PLLN_VALUE(VALUE) ((1U <= (VALUE)) && ((VALUE) <= 63U))
#define IS_SMU_PLLF_VALUE(VALUE) ((50U <= (VALUE)) && ((VALUE) <= 511U))

/*
 * @brief PLL init structure definition
 */
typedef struct
{
    SMUOSCILLATOR_TypeDef OscillatorType;           /* oscillators select */
    EnState FIRCOscState;
    uint8_t FIRCCalibrationValue;                   /* 16M RC oscillator frequency adjustment */
    SMU_PLLConfigTypeDef PLLConfig;                 /* PLL frequency division value */
} SMU_PLLInitTypeDef;

/*
 * @brief Core clock init structure definition
 */
typedef struct 
{
    SMUSYSCLKSelect_TypeDef     SYSCLKSelect;       /* System clock select */
    SMUAXI4Bus3CLKDiv_TypeDef   AXI4Bus3CLKDiv;     /* AXI4Bus3 clock frequency division */
    SMUAPBBus0CLKDiv_TypeDef    APBBus0CLKDiv;      /* APBBus0 clock frequency division */
    SMUAPBBus1CLKDiv_TypeDef    APBBus1CLKDiv;      /* APBBus1 clock frequency division */
    SMUCANX2CLKDiv_TypeDef      CANX2CLKDiv;        /* CANX2 clock frequency division */
} SMU_ClockInitTypeDef;

/*
 * @brief Clock frequency structure definition
 */
typedef struct
{
    uint32_t AXIBus0_Frequency;                     /* AXIBus0 clock frequency expressed in Hz */
    uint32_t AXIBus1_Frequency;                     /* AXIBus1 clock frequency expressed in Hz */
    uint32_t AXIBus2_Frequency;                     /* AXIBus2 clock frequency expressed in Hz */
    uint32_t AXIBus3_Frequency;                     /* AXIBus3 clock frequency expressed in Hz */
    uint32_t AXILite4Bus0_Frequency;                /* AXILite4Bus0 clock frequency expressed in Hz */
    uint32_t AXILite4Bus1_Frequency;                /* AXILite4Bus1 clock frequency expressed in Hz */
    uint32_t AXILite4Bus2_Frequency;                /* AXILite4Bus2 clock frequency expressed in Hz */
    uint32_t APBBus0_Frequency;                     /* APBBus0 clock frequency expressed in Hz */
    uint32_t APBBus1_Frequency;                     /* APBBus0 clock frequency expressed in Hz */
    uint32_t CANCLKx2_Frequency;                    /* CANCLKx2 clock frequency expressed in Hz */
}SMU_ClocksTypeDef;

/*
 * @defgroup SMU FLAG definition
 */
#define SMU_FLAG_PLLLOCK            ((uint32_t)0x0001)      /* PLL clock lock flag */
#define SMU_FLAG_EFLASHICS          ((uint32_t)0x0002)      /* EFLASH initialization completion signal flag */
#define SMU_FLAG_PWNSTART           ((uint32_t)0x0000)      /* Power on start flag */
#define SMU_FLAG_DSSTART            ((uint32_t)0x0004)      /* Deep sleep activation flag */
#define SMU_FLAG_SLEEPSTART         ((uint32_t)0x0008)      /* Sleep start flag */
#define SMU_FLAG_LPWR               ((uint32_t)0x0010)      /* Power saving mode flag */
#define SMU_FLAG_AXI4Bus0           ((uint32_t)0x0020)      /* AXI4Bus0 bus status flag */
#define SMU_FLAG_AXI4Bus1           ((uint32_t)0x0040)      /* AXI4Bus1 bus status flag */
#define SMU_FLAG_AXI4Bus2           ((uint32_t)0x0080)      /* AXI4Bus2 bus status flag */
#define SMU_FLAG_SRAM0RDY           ((uint32_t)0x0100)      /* SRAM0 readiness flag */
#define SMU_FLAG_SRAM1RDY           ((uint32_t)0x0200)      /* SRAM1 readiness flag */
#define SMU_FLAG_SRAM2RDY           ((uint32_t)0x0400)      /* SRAM2 readiness flag */
#define SMU_FLAG_SRAM3RDY           ((uint32_t)0x0800)      /* SRAM3 readiness flag */
#define IS_SMU_FLAG(FLAG) (((FLAG) == SMU_FLAG_PLLLOCK)    || ((FLAG) == SMU_FLAG_EFLASHICS) || \
                           ((FLAG) == SMU_FLAG_PWNSTART)   || ((FLAG) == SMU_FLAG_DSSTART)   || \
                           ((FLAG) == SMU_FLAG_SLEEPSTART) || ((FLAG) == SMU_FLAG_LPWR)      || \
                           ((FLAG) == SMU_FLAG_AXI4Bus0)   || ((FLAG) == SMU_FLAG_AXI4Bus1)  || \
                           ((FLAG) == SMU_FLAG_AXI4Bus2)   || ((FLAG) == SMU_FLAG_SRAM0RDY)  || \
                           ((FLAG) == SMU_FLAG_SRAM1RDY)   || ((FLAG) == SMU_FLAG_SRAM2RDY)  || \
                           ((FLAG) == SMU_FLAG_SRAM3RDY))

/*
 * @defgroup RESET or SET the AxiBus0 peripherals definition.
 */
#define AXIBUS0_ALL_RESET() (CLEAR_BIT(SMU->AB0CFGR, 0x1F))
#define AXIBUS0_ALL_SET()   (SET_BIT(SMU->AB0CFGR, 0x1F))
#define QSPI_RESET()    (CLEAR_BIT(SMU->AB0CFGR, SMU_AB0CFGR_QSPIRST))
#define QSPI_SET()      (SET_BIT(SMU->AB0CFGR, SMU_AB0CFGR_QSPIRST))
#define EFLASH_RESET()  (CLEAR_BIT(SMU->AB0CFGR, SMU_AB0CFGR_EFLASHRST))
#define EFLASH_SET()    (SET_BIT(SMU->AB0CFGR, SMU_AB0CFGR_EFLASHRST))

/*
 * @defgroup RESET or SET the AxiBus1 peripherals definition.
 */
#define AXIBUS1_ALL_RESET() (CLEAR_BIT(SMU->AB1CFGR, 0xFFF))
#define AXIBUS1_ALL_SET()   (SET_BIT(SMU->AB1CFGR, 0xFFF))
#define DMA0_RESET()    (CLEAR_BIT(SMU->AB1CFGR, SMU_AB1CFGR_DMA0RST))
#define DMA0_SET()      (SET_BIT(SMU->AB1CFGR, SMU_AB1CFGR_DMA0RST))
#define DMA1_RESET()    (CLEAR_BIT(SMU->AB1CFGR, SMU_AB1CFGR_DMA1RST))
#define DMA1_SET()      (SET_BIT(SMU->AB1CFGR, SMU_AB1CFGR_DMA1RST))
#define SRAM0_RESET()   (CLEAR_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM0RST))
#define SRAM0_SET()     (SET_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM0RST))
#define SRAM1_RESET()   (CLEAR_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM1RST))
#define SRAM1_SET()     (SET_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM1RST))
#define SRAM2_RESET()   (CLEAR_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM2RST))
#define SRAM2_SET()     (SET_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM2RST))
#define SRAM3_RESET()   (CLEAR_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM3RST))
#define SRAM3_SET()     (SET_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM3RST))

/*
 * @defgroup RESET or SET the AxiBus3 peripherals definition.
 */
#define AXIBUS3_ALL_RESET() (CLEAR_BIT(SMU->AB3CFGR, 0xFC))
#define AXIBUS3_ALL_SET()   (SET_BIT(SMU->AB3CFGR, 0xFC))
#define AXI4TOAPB0_RESET()  (CLEAR_BIT(SMU->AB3CFGR, SMU_AB3CFGR_AXI4TOAPB0RST))
#define AXI4TOAPB0_SET()    (SET_BIT(SMU->AB3CFGR, SMU_AB3CFGR_AXI4TOAPB0RST))
#define AXI4TOAPB1_RESET()  (CLEAR_BIT(SMU->AB3CFGR, SMU_AB3CFGR_AXI4TOAPB1RST))
#define AXI4TOAPB1_SET()    (SET_BIT(SMU->AB3CFGR, SMU_AB3CFGR_AXI4TOAPB1RST))
#define AXIBUS3_RESET()     (CLEAR_BIT(SMU->AB3CFGR, SMU_AB3CFGR_AXIBUS3RST))
#define AXIBUS3_SET()       (SET_BIT(SMU->AB3CFGR, SMU_AB3CFGR_AXIBUS3RST))

/*
 * @defgroup RESET or SET the AxiLiteBus0 peripherals definition.
 */
#define AXILITEBUS0_ALL_RESET() (CLEAR_BIT(SMU->ALB0CFGR, 0x3FFF))
#define AXILITEBUS0_ALL_SET()   (SET_BIT(SMU->ALB0CFGR, 0x3FFF))
#define FCU_RESET()     (CLEAR_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_FCURST))
#define FCU_SET()       (SET_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_FCURST))
#define CMU0_RESET()    (CLEAR_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU0RST))
#define CMU0_SET()      (SET_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU0RST))
#define CMU1_RESET()    (CLEAR_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU1RST))
#define CMU1_SET()      (SET_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU1RST))
#define CMU2_RESET()    (CLEAR_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU2RST))
#define CMU2_SET()      (SET_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU2RST))
#define CMU3_RESET()    (CLEAR_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU3RST))
#define CMU3_SET()      (SET_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU3RST))
#define PLIC_RESET()    (CLEAR_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_PLICRST))
#define PLIC_SET()      (SET_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_PLICRST))
#define CLINT_RESET()   (CLEAR_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CLINTRST))
#define CLINT_SET()     (SET_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CLINTRST))

/*
 * @defgroup RESET or SET the AxiLiteBus1 peripherals definition.
 */
#define AXILITEBUS1_ALL_RESET() (CLEAR_BIT(SMU->ALB1CFGR, 0x3FFFFF))
#define AXILITEBUS1_ALL_SET()   (SET_BIT(SMU->ALB1CFGR, 0x3FFFFF))
#define HTIM0_RESET()       (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_HTIM0RST))
#define HTIM0_SET()         (SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_HTIM0RST))
#define CANFD0_RESET()      (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_CANFD0RST))
#define CANFD0_SET()        (SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_CANFD0RST))
#define CANFD1_RESET()      (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_CANFD1RST))
#define CANFD1_SET()        (SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_CANFD1RST))
#define ADC0_RESET()        (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_ADC0RST))
#define ADC0_SET()          (SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_ADC0RST))
#define ADC1_RESET()        (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_ADC1RST))
#define ADC1_SET()          (SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_ADC1RST))
#define DAC0_RESET()        (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_DAC0RST))
#define DAC0_SET()          (SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_DAC0RST))
#define GPIOA_RESET()       (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_GPIOARST))
#define GPIOA_SET()         (SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_GPIOARST))
#define GPIOB_RESET()       (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_GPIOBRST))
#define GPIOB_SET()         (SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_GPIOBRST))
#define GPIOC_RESET()       (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_GPIOCRST))
#define GPIOC_SET()         (SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_GPIOCRST))
#define DSE_RESET()         (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_DSERST))
#define DSE_SET()           (SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_DSERST))
#define AXILITEBUS1_RESET() (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_AXILITEBUS1RST))
#define AXILITEBUS1_SET()   (SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_AXILITEBUS1RST))

/*
 * @defgroup RESET or SET the AxiLiteBus2 peripherals definition.
 */
#define AXILITEBUS2_ALL_RESET() (CLEAR_BIT(SMU->ALB2CFGR, 0xFFFFF))
#define AXILITEBUS2_ALL_SET()   (SET_BIT(SMU->ALB2CFGR, 0xFFFFF))
#define HTIM1_RESET()       (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_HTIM1RST))
#define HTIM1_SET()         (SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_HTIM1RST))
#define CANFD2_RESET()      (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_CANFD2RST))
#define CANFD2_SET()        (SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_CANFD2RST))
#define CANFD3_RESET()      (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_CANFD3RST))
#define CANFD3_SET()        (SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_CANFD3RST))
#define ADC2_RESET()        (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_ADC2RST))
#define ADC2_SET()          (SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_ADC2RST))
#define DAC1_RESET()        (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_DAC1RST))
#define DAC1_SET()          (SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_DAC1RST))
#define GPIOD_RESET()       (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIODRST))
#define GPIOD_SET()         (SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIODRST))
#define GPIOE_RESET()       (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIOERST))
#define GPIOE_SET()         (SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIOERST))
#define GPIOF_RESET()       (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIOFRST))
#define GPIOF_SET()         (SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIOFRST))
#define GPIOG_RESET()       (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIOGRST))
#define GPIOG_SET()         (SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIOGRST))
#define AXILITEBUS2_RESET() (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_AXILITEBUS2RST))
#define AXILITEBUS2_SET()   (SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_AXILITEBUS2RST))

/*
 * @defgroup RESET or SET the ApbBus0 peripherals definition.
 */
#define APBBUS0_ALL_RESET() (CLEAR_BIT(SMU->APB0CFGR, 0xFFFFFFFF))
#define APBBUS0_ALL_SET()   (SET_BIT(SMU->APB0CFGR, 0xFFFFFFFF))
#define MAC_RESET()     (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_MACRST))
#define MAC_SET()       (SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_MACRST))
#define SPI0_RESET()    (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_SPI0RST))
#define SPI0_SET()      (SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_SPI0RST))
#define SPI1_RESET()    (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_SPI1RST))
#define SPI1_SET()      (SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_SPI1RST))
#define SPI2_RESET()    (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_SPI2RST))
#define SPI2_SET()      (SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_SPI2RST))
#define IIC0_RESET()    (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_IIC0RST))
#define IIC0_SET()      (SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_IIC0RST))
#define IIC1_RESET()    (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_IIC1RST))
#define IIC1_SET()      (SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_IIC1RST))
#define USART0_RESET()  (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART0RST))
#define USART0_SET()    (SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART0RST))
#define USART1_RESET()  (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART1RST))
#define USART1_SET()    (SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART1RST))
#define USART2_RESET()  (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART2RST))
#define USART2_SET()    (SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART2RST))
#define USART3_RESET()  (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART3RST))
#define USART3_SET()    (SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART3RST))
#define HTIM2_RESET()   (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_HTIM2RST))
#define HTIM2_SET()     (SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_HTIM2RST))
#define TIM3_RESET()    (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_TIM3RST))
#define TIM3_SET()      (SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_TIM3RST))
#define TIM4_RESET()    (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_TIM4RST))
#define TIM4_SET()      (SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_TIM4RST))
#define WWDG0_RESET()   (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_WWDG0RST))
#define WWDG0_SET()     (SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_WWDG0RST))
#define APBBUS0_RESET() (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_APBBUS0RST))
#define APBBUS0_SET()   (SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_APBBUS0RST))

/*
 * @defgroup RESET or SET the ApbBus1 peripherals definition.
 */
#define APBBUS1_ALL_RESET() (CLEAR_BIT(SMU->APB0CFGR, 0x3FFFFFFF))
#define APBBUS1_ALL_SET()   (SET_BIT(SMU->APB0CFGR, 0x3FFFFFFF))
#define SPI3_RESET()    (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_SPI3RST))
#define SPI3_SET()      (SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_SPI3RST))
#define SPI4_RESET()    (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_SPI4RST))
#define SPI4_SET()      (SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_SPI4RST))
#define SPI5_RESET()    (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_SPI5RST))
#define SPI5_SET()      (SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_SPI5RST))
#define IIC2_RESET()    (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_IIC2RST))
#define IIC2_SET()      (SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_IIC2RST))
#define IIC3_RESET()    (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_IIC3RST))
#define IIC3_SET()      (SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_IIC3RST))
#define USART4_RESET()  (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART4RST))
#define USART4_SET()    (SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART4RST))
#define USART5_RESET()  (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART5RST))
#define USART5_SET()    (SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART5RST))
#define USART6_RESET()  (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART6RST))
#define USART6_SET()    (SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART6RST))
#define USART7_RESET()  (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART7RST))
#define USART7_SET()    (SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART7RST))
#define HTIM5_RESET()   (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_HTIM5RST))
#define HTIM5_SET()     (SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_HTIM5RST))
#define TIM6_RESET()    (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_TIM6RST))
#define TIM6_SET()      (SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_TIM6RST))
#define TIM7_RESET()    (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_TIM7RST))
#define TIM7_SET()      (SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_TIM7RST))
#define WWDG1_RESET()   (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_WWDG1RST))
#define WWDG1_SET()     (SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_WWDG1RST))
#define CRC_RESET()     (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_CRCRST))
#define CRC_SET()       (SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_CRCRST))
#define APBBUS1_RESET() (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_APBBUS1RST))
#define APBBUS1_SET()   (SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_APBBUS1RST))

/*
 * @defgroup Enable Disable clock of the AxiBus0 peripherals definition.
 */
#define QSPI_CLK_DISABLE()          (CLEAR_BIT(SMU->AB0CFGR, SMU_AB0CFGR_QSPIEN))
#define EFLASH_CLK_DISABLE()        (CLEAR_BIT(SMU->AB0CFGR, SMU_AB0CFGR_EFLASHEN))
#define QSPI_CLK_UPDATE_DISABLE()   (CLEAR_BIT(SMU->AB0CFGR, SMU_AB0CFGR_QSPIUEN))
#define EFLASH_CLK_UPDATE_DISABLE() (CLEAR_BIT(SMU->AB0CFGR, SMU_AB0CFGR_EFLASHUEN))

#define QSPI_CLK_ENABLE() do {                                                      \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->AB0CFGR, SMU_AB0CFGR_QSPIEN);              \
                            tmpreg = READ_BIT(SMU->AB0CFGR, SMU_AB0CFGR_QSPIEN);    \
                            UNUSED(tmpreg);                                         \
                        } while (0U)
#define EFLASH_CLK_ENABLE() do {                                                    \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->AB0CFGR, SMU_AB0CFGR_EFLASHEN);            \
                            tmpreg = READ_BIT(SMU->AB0CFGR, SMU_AB0CFGR_EFLASHEN);  \
                            UNUSED(tmpreg);                                         \
                        } while (0U)
#define QSPI_CLK_UPDATE_ENABLE() do {                                               \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->AB0CFGR, SMU_AB0CFGR_QSPIUEN);             \
                            tmpreg = READ_BIT(SMU->AB0CFGR, SMU_AB0CFGR_QSPIUEN);   \
                            UNUSED(tmpreg);                                         \
                        } while (0U)
#define EFLASH_CLK_UPDATE_ENABLE() do {                                             \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->AB0CFGR, SMU_AB0CFGR_EFLASHUEN);           \
                            tmpreg = READ_BIT(SMU->AB0CFGR, SMU_AB0CFGR_EFLASHUEN); \
                            UNUSED(tmpreg);                                         \
                        } while (0U)

/*
 * @defgroup Enable Disable clock of the AxiBus1 peripherals definition.
 */
#define DMA0_CLK_DISABLE()      (CLEAR_BIT(SMU->AB1CFGR, SMU_AB1CFGR_DMA0EN))
#define DMA1_CLK_DISABLE()      (CLEAR_BIT(SMU->AB1CFGR, SMU_AB1CFGR_DMA1EN))
#define SRAM0_CLK_DISABLE()     (CLEAR_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM0EN))
#define SRAM1_CLK_DISABLE()     (CLEAR_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM1EN))
#define SRAM2_CLK_DISABLE()     (CLEAR_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM2EN))
#define SRAM3_CLK_DISABLE()     (CLEAR_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM3EN))

#define DMA0_CLK_ENABLE() do {                                                      \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->AB1CFGR, SMU_AB1CFGR_DMA0EN);              \
                            tmpreg = READ_BIT(SMU->AB1CFGR, SMU_AB1CFGR_DMA0EN);    \
                            UNUSED(tmpreg);                                         \
                        } while (0U)
#define DMA1_CLK_ENABLE() do {                                                      \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->AB1CFGR, SMU_AB1CFGR_DMA1EN);              \
                            tmpreg = READ_BIT(SMU->AB1CFGR, SMU_AB1CFGR_DMA1EN);    \
                            UNUSED(tmpreg);                                         \
                        } while (0U)
#define SRAM0_CLK_ENABLE() do {                                                     \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM0EN);             \
                            tmpreg = READ_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM0EN);   \
                            UNUSED(tmpreg);                                         \
                        } while (0U)
#define SRAM1_CLK_ENABLE() do {                                                     \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM1EN);             \
                            tmpreg = READ_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM1EN);   \
                            UNUSED(tmpreg);                                         \
                        } while (0U)
#define SRAM2_CLK_ENABLE() do {                                                     \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM2EN);             \
                            tmpreg = READ_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM2EN);   \
                            UNUSED(tmpreg);                                         \
                        } while (0U)
#define SRAM3_CLK_ENABLE() do {                                                     \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM3EN);             \
                            tmpreg = READ_BIT(SMU->AB1CFGR, SMU_AB1CFGR_SRAM3EN);   \
                            UNUSED(tmpreg);                                         \
                        } while (0U)

/*
 * @defgroup Enable Disable clock of the AxiBus3 peripherals definition.
 */
#define AXI4TOAPB0_CLK_DISABLE()    (CLEAR_BIT(SMU->AB3CFGR, SMU_AB3CFGR_AXI4TOAPB0EN))
#define AXI4TOAPB1_CLK_DISABLE()    (CLEAR_BIT(SMU->AB3CFGR, SMU_AB3CFGR_AXI4TOAPB1EN))
#define AXIBUS3_CLK_DISABLE()       (CLEAR_BIT(SMU->AB3CFGR, SMU_AB3CFGR_AXIBUS3EN))

#define AXI4TOAPB0_CLK_ENABLE() do {                                                    \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->AB3CFGR, SMU_AB3CFGR_AXI4TOAPB0EN);            \
                            tmpreg = READ_BIT(SMU->AB3CFGR, SMU_AB3CFGR_AXI4TOAPB0EN);  \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define AXI4TOAPB1_CLK_ENABLE() do {                                                    \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->AB3CFGR, SMU_AB3CFGR_AXI4TOAPB1EN);            \
                            tmpreg = READ_BIT(SMU->AB3CFGR, SMU_AB3CFGR_AXI4TOAPB1EN);  \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define AXIBUS3_CLK_ENABLE() do {                                                       \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->AB3CFGR, SMU_AB3CFGR_AXIBUS3EN);               \
                            tmpreg = READ_BIT(SMU->AB3CFGR, SMU_AB3CFGR_AXIBUS3EN);     \
                            UNUSED(tmpreg);                                             \
                        } while (0U)

/*
 * @defgroup Enable Disable clock of the AxiLiteBus0 peripherals definition.
 */
#define FCU_CLK_DISABLE()   (CLEAR_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_FCUEN))
#define CMU0_CLK_DISABLE()  (CLEAR_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU0EN))
#define CMU1_CLK_DISABLE()  (CLEAR_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU1EN))
#define CMU2_CLK_DISABLE()  (CLEAR_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU2EN))
#define CMU3_CLK_DISABLE()  (CLEAR_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU3EN))
#define PLIC_CLK_DISABLE()  (CLEAR_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_PLICEN))
#define CLINT_CLK_DISABLE() (CLEAR_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CLINTEN))

#define FCU_CLK_ENABLE() do {                                                       \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_FCUEN);             \
                            tmpreg = READ_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_FCUEN);   \
                            UNUSED(tmpreg);                                         \
                        } while (0U)
#define CMU0_CLK_ENABLE() do {                                                      \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU0EN);            \
                            tmpreg = READ_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU0EN);  \
                            UNUSED(tmpreg);                                         \
                        } while (0U)
#define CMU1_CLK_ENABLE() do {                                                      \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU1EN);            \
                            tmpreg = READ_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU1EN);  \
                            UNUSED(tmpreg);                                         \
                        } while (0U)
#define CMU2_CLK_ENABLE() do {                                                      \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU2EN);            \
                            tmpreg = READ_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU2EN);  \
                            UNUSED(tmpreg);                                         \
                        } while (0U)
#define CMU3_CLK_ENABLE() do {                                                      \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU3EN);            \
                            tmpreg = READ_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CMU3EN);  \
                            UNUSED(tmpreg);                                         \
                        } while (0U)
#define PLIC_CLK_ENABLE() do {                                                      \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_PLICEN);            \
                            tmpreg = READ_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_PLICEN);  \
                            UNUSED(tmpreg);                                         \
                        } while (0U)
#define CLINT_CLK_ENABLE() do {                                                     \
                            __IO uint32_t tmpreg = 0x00U;                           \
                            SET_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CLINTEN);           \
                            tmpreg = READ_BIT(SMU->ALB0CFGR, SMU_ALB0CFGR_CLINTEN); \
                            UNUSED(tmpreg);                                         \
                        } while (0U)

/*
 * @defgroup Enable Disable clock of the AxiLiteBus1 peripherals definition.
 */
#define HTIM0_CLK_DISABLE()         (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_HTIM0EN))
#define CANFD0_CLK_DISABLE()        (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_CANFD0EN))
#define CANFD1_CLK_DISABLE()        (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_CANFD1EN))
#define ADC0_CLK_DISABLE()          (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_ADC0EN))
#define ADC1_CLK_DISABLE()          (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_ADC1EN))
#define DAC0_CLK_DISABLE()          (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_DAC0EN))
#define GPIOA_CLK_DISABLE()         (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_GPIOAEN))
#define GPIOB_CLK_DISABLE()         (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_GPIOBEN))
#define GPIOC_CLK_DISABLE()         (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_GPIOCEN))
#define DSE_CLK_DISABLE()           (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_DSEEN))
#define AXILITEBUS1_CLK_DISABLE()   (CLEAR_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_AXILITEBUS1EN))

#define HTIM0_CLK_ENABLE() do {                                                         \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_HTIM0EN);               \
                            tmpreg = READ_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_HTIM0EN);     \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define CANFD0_CLK_ENABLE() do {                                                        \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_CANFD0EN);              \
                            tmpreg = READ_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_CANFD0EN);    \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define CANFD1_CLK_ENABLE() do {                                                        \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_CANFD1EN);              \
                            tmpreg = READ_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_CANFD1EN);    \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define ADC0_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_ADC0EN);                \
                            tmpreg = READ_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_ADC0EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define ADC1_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_ADC1EN);                \
                            tmpreg = READ_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_ADC1EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define DAC0_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_DAC0EN);                \
                            tmpreg = READ_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_DAC0EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define GPIOA_CLK_ENABLE() do {                                                         \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_GPIOAEN);               \
                            tmpreg = READ_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_GPIOAEN);     \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define GPIOB_CLK_ENABLE() do {                                                         \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_GPIOBEN);               \
                            tmpreg = READ_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_GPIOBEN);     \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define GPIOC_CLK_ENABLE() do {                                                         \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_GPIOCEN);               \
                            tmpreg = READ_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_GPIOCEN);     \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define DSE_CLK_ENABLE() do {                                                           \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_DSEEN);                 \
                            tmpreg = READ_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_DSEEN);       \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define AXILITEBUS1_CLK_ENABLE() do {                                                       \
                            __IO uint32_t tmpreg = 0x00U;                                   \
                            SET_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_AXILITEBUS1EN);             \
                            tmpreg = READ_BIT(SMU->ALB1CFGR, SMU_ALB1CFGR_AXILITEBUS1EN);   \
                            UNUSED(tmpreg);                                                 \
                        } while (0U)

/*
 * @defgroup Enable Disable clock of the AxiLiteBus2 peripherals definition.
 */
#define HTIM1_CLK_DISABLE()         (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_HTIM1EN))
#define CANFD2_CLK_DISABLE()        (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_CANFD2EN))
#define CANFD3_CLK_DISABLE()        (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_CANFD3EN))
#define ADC2_CLK_DISABLE()          (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_ADC2EN))
#define DAC1_CLK_DISABLE()          (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_DAC1EN))
#define GPIOD_CLK_DISABLE()         (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIODEN))
#define GPIOE_CLK_DISABLE()         (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIOEEN))
#define GPIOF_CLK_DISABLE()         (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIOFEN))
#define GPIOG_CLK_DISABLE()         (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIOGEN))
#define AXILITEBUS2_CLK_DISABLE()   (CLEAR_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_AXILITEBUS2EN))

#define HTIM1_CLK_ENABLE() do {                                                         \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_HTIM1EN);               \
                            tmpreg = READ_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_HTIM1EN);     \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define CANFD2_CLK_ENABLE() do {                                                        \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_CANFD2EN);              \
                            tmpreg = READ_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_CANFD2EN);    \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define CANFD3_CLK_ENABLE() do {                                                        \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_CANFD3EN);              \
                            tmpreg = READ_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_CANFD3EN);    \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define ADC2_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_ADC2EN);                \
                            tmpreg = READ_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_ADC2EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define DAC1_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_DAC1EN);                \
                            tmpreg = READ_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_DAC1EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define GPIOD_CLK_ENABLE() do {                                                         \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIODEN);               \
                            tmpreg = READ_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIODEN);     \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define GPIOE_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIOEEN);               \
                            tmpreg = READ_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIOEEN);     \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define GPIOF_CLK_ENABLE() do {                                                         \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIOFEN);               \
                            tmpreg = READ_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIOFEN);     \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define GPIOG_CLK_ENABLE() do {                                                         \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIOGEN);               \
                            tmpreg = READ_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_GPIOGEN);     \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define AXILITEBUS2_CLK_ENABLE() do {                                                       \
                            __IO uint32_t tmpreg = 0x00U;                                   \
                            SET_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_AXILITEBUS2EN);             \
                            tmpreg = READ_BIT(SMU->ALB2CFGR, SMU_ALB2CFGR_AXILITEBUS2EN);   \
                            UNUSED(tmpreg);                                                 \
                        } while (0U)

/*
 * @defgroup Enable Disable clock of the ApbBus0 peripherals definition.
 */
#define MAC_CLK_DISABLE()       (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_MACEN))
#define SPI0_CLK_DISABLE()      (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_SPI0EN))
#define SPI1_CLK_DISABLE()      (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_SPI1EN))
#define SPI2_CLK_DISABLE()      (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_SPI2EN))
#define IIC0_CLK_DISABLE()      (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_IIC0EN))
#define IIC1_CLK_DISABLE()      (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_IIC1EN))
#define USART0_CLK_DISABLE()    (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART0EN))
#define USART1_CLK_DISABLE()    (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART1EN))
#define USART2_CLK_DISABLE()    (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART2EN))
#define USART3_CLK_DISABLE()    (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART3EN))
#define HTIM2_CLK_DISABLE()     (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_HTIM2EN))
#define TIM3_CLK_DISABLE()      (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_TIM3EN))
#define TIM4_CLK_DISABLE()      (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_TIM4EN))
#define WWDG0_CLK_DISABLE()     (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_WWDG0EN))
#define APBBUS0_CLK_DISABLE()   (CLEAR_BIT(SMU->APB0CFGR, SMU_APB0CFGR_APBBUS0EN))

#define MAC_CLK_ENABLE() do {                                                           \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_MACEN);                 \
                            tmpreg = READ_BIT(SMU->APB0CFGR, SMU_APB0CFGR_MACEN);       \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define SPI0_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_SPI0EN);                \
                            tmpreg = READ_BIT(SMU->APB0CFGR, SMU_APB0CFGR_SPI0EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define SPI1_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_SPI1EN);                \
                            tmpreg = READ_BIT(SMU->APB0CFGR, SMU_APB0CFGR_SPI1EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define SPI2_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_SPI2EN);                \
                            tmpreg = READ_BIT(SMU->APB0CFGR, SMU_APB0CFGR_SPI2EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define IIC0_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_IIC0EN);                \
                            tmpreg = READ_BIT(SMU->APB0CFGR, SMU_APB0CFGR_IIC0EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define IIC1_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_IIC1EN);                \
                            tmpreg = READ_BIT(SMU->APB0CFGR, SMU_APB0CFGR_IIC1EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define USART0_CLK_ENABLE() do {                                                        \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART0EN);              \
                            tmpreg = READ_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART0EN);    \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define USART1_CLK_ENABLE() do {                                                        \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART1EN);              \
                            tmpreg = READ_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART1EN);    \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define USART2_CLK_ENABLE() do {                                                        \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART2EN);              \
                            tmpreg = READ_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART2EN);    \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define USART3_CLK_ENABLE() do {                                                        \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART3EN);              \
                            tmpreg = READ_BIT(SMU->APB0CFGR, SMU_APB0CFGR_USART3EN);    \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define HTIM2_CLK_ENABLE() do {                                                         \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_HTIM2EN);               \
                            tmpreg = READ_BIT(SMU->APB0CFGR, SMU_APB0CFGR_HTIM2EN);     \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define TIM3_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_TIM3EN);                \
                            tmpreg = READ_BIT(SMU->APB0CFGR, SMU_APB0CFGR_TIM3EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define TIM4_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_TIM4EN);                \
                            tmpreg = READ_BIT(SMU->APB0CFGR, SMU_APB0CFGR_TIM4EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define WWDG0_CLK_ENABLE() do {                                                         \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_WWDG0EN);               \
                            tmpreg = READ_BIT(SMU->APB0CFGR, SMU_APB0CFGR_WWDG0EN);     \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define APBBUS0_CLK_ENABLE() do {                                                       \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB0CFGR, SMU_APB0CFGR_APBBUS0EN);             \
                            tmpreg = READ_BIT(SMU->APB0CFGR, SMU_APB0CFGR_APBBUS0EN);   \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
							 
/*
 * @defgroup Enable Disable clock of the ApbBus1 peripherals definition.
 */
#define SPI3_CLK_DISABLE()      (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_SPI3EN))
#define SPI4_CLK_DISABLE()      (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_SPI4EN))
#define SPI5_CLK_DISABLE()      (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_SPI5EN))
#define IIC2_CLK_DISABLE()      (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_IIC2EN))
#define IIC3_CLK_DISABLE()      (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_IIC3EN))
#define USART4_CLK_DISABLE()    (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART4EN))
#define USART5_CLK_DISABLE()    (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART5EN))
#define USART6_CLK_DISABLE()    (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART6EN))
#define USART7_CLK_DISABLE()    (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART7EN))
#define HTIM5_CLK_DISABLE()     (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_HTIM5EN))
#define TIM6_CLK_DISABLE()      (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_TIM6EN))
#define TIM7_CLK_DISABLE()      (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_TIM7EN))
#define WWDG1_CLK_DISABLE()     (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_WWDG1EN))
#define CRC_CLK_DISABLE()       (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_CRCEN))
#define APBBUS1_CLK_DISABLE()   (CLEAR_BIT(SMU->APB1CFGR, SMU_APB1CFGR_APBBUS1EN))

#define SPI3_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_SPI3EN);                \
                            tmpreg = READ_BIT(SMU->APB1CFGR, SMU_APB1CFGR_SPI3EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define SPI4_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_SPI4EN);                \
                            tmpreg = READ_BIT(SMU->APB1CFGR, SMU_APB1CFGR_SPI4EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define SPI5_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_SPI5EN);                \
                            tmpreg = READ_BIT(SMU->APB1CFGR, SMU_APB1CFGR_SPI5EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define IIC2_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_IIC2EN);                \
                            tmpreg = READ_BIT(SMU->APB1CFGR, SMU_APB1CFGR_IIC2EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define IIC3_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_IIC3EN);                \
                            tmpreg = READ_BIT(SMU->APB1CFGR, SMU_APB1CFGR_IIC3EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define USART4_CLK_ENABLE() do {                                                        \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART4EN);              \
                            tmpreg = READ_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART4EN);    \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define USART5_CLK_ENABLE() do {                                                        \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART5EN);              \
                            tmpreg = READ_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART5EN);    \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define USART6_CLK_ENABLE() do {                                                        \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART6EN);              \
                            tmpreg = READ_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART6EN);    \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define USART7_CLK_ENABLE() do {                                                        \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART7EN);              \
                            tmpreg = READ_BIT(SMU->APB1CFGR, SMU_APB1CFGR_USART7EN);    \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define HTIM5_CLK_ENABLE() do {                                                         \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_HTIM5EN);               \
                            tmpreg = READ_BIT(SMU->APB1CFGR, SMU_APB1CFGR_HTIM5EN);     \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define TIM6_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_TIM6EN);                \
                            tmpreg = READ_BIT(SMU->APB1CFGR, SMU_APB1CFGR_TIM6EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define TIM7_CLK_ENABLE() do {                                                          \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_TIM7EN);                \
                            tmpreg = READ_BIT(SMU->APB1CFGR, SMU_APB1CFGR_TIM7EN);      \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define WWDG1_CLK_ENABLE() do {                                                         \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_WWDG1EN);               \
                            tmpreg = READ_BIT(SMU->APB1CFGR, SMU_APB1CFGR_WWDG1EN);     \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define CRC_CLK_ENABLE() do {                                                           \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_CRCEN);                 \
                            tmpreg = READ_BIT(SMU->APB1CFGR, SMU_APB1CFGR_CRCEN);       \
                            UNUSED(tmpreg);                                             \
                        } while (0U)
#define APBBUS1_CLK_ENABLE() do {                                                       \
                            __IO uint32_t tmpreg = 0x00U;                               \
                            SET_BIT(SMU->APB1CFGR, SMU_APB1CFGR_APBBUS1EN);             \
                            tmpreg = READ_BIT(SMU->APB1CFGR, SMU_APB1CFGR_APBBUS1EN);   \
                            UNUSED(tmpreg);                                             \
                        } while (0U)

                          
/*
 * @brief Global Clock Struct Types
 */
extern SMU_ClocksTypeDef   SMU_ClocksStruct;

/*
 * @brief Function declaration
 */
void SMU_PLLStructInit(SMU_PLLInitTypeDef *SMU_PLLInitStruct);
ErrorStatus SMU_PLLInit(SMU_PLLInitTypeDef *SMU_PLLInitStruct);
void SMU_ClockStructInit(SMU_ClockInitTypeDef *SMU_ClockInitStruct);
ErrorStatus SMU_ClockInit(SMU_ClockInitTypeDef *SMU_ClockInitStruct);
ErrorStatus SMU_GetClocksFreq(SMU_ClocksTypeDef* SMU_Clocks);
void SMU_FIRCRc16mPdCmd(EnState NewState);
void SMU_FIRCRc16mFreConfig(uint8_t FIRCRc16mPdFrequency);
void SMU_PLLCmd(EnState NewState);
void SMU_PLLRstnCmd(EnState NewState);
void SMU_PLLUpdateCmd(EnState NewState);
ErrorStatus SMU_PLLConfig(SMU_PLLConfigTypeDef *SMU_PLLConfigStruct);
FlagStatus SMU_GetFIRCReadyStatus(void);
FlagStatus SMU_GetFlagStatus(uint32_t SMU_FLAG);
void SMU_DCLSCmd(EnState NewState);
void SMU_ECCCmd(EnState NewState);
void SMU_TimerBreak(SMUTimerBreak_TypeDef TimerSelect);
void SMU_ClearTimeOutFlag(void);
void SMU_SetSRAMLowPower(SMUSramLowPower_TypeDef SRAMSelect, EnState NewState);
void SMU_SetSysClockOut(SMUCLKOutMux_TypeDef CLKSelect);
void SMU_BUSTimeOutCmd(EnState NewState);
void SMU_SetBUSTimeOutValue(uint32_t TimeOutValue);
extern void Systemclock_Init();

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
