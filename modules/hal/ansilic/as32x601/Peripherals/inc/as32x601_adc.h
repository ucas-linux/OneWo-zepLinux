/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_adc.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:			2024/8/30
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/8/30     1.0     build this module
 * Description:     This file contains all the functions prototypes for the ADC firmware library.
 */
#ifndef AS32X601_ADC_H
#define AS32X601_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601_adc_regs.h"

/*
 * @brief  ADC regular group sequence length.
 */
#define ADC_REGULAR_SEQUENCE_LENGTH         (16UL)

/*
 * @brief  ADC injected group sequence length.
 */
#define ADC_INJECT_SEQUENCE_LENGTH          (4UL)

/*
 * @brief  ADC sampel timer setting number in each SPT register.
 */
#define ADC_EACH_REG_SPT_NUM                (8UL)

/*
 * @brief  ADC Regular discontinuous mode number.
 */
#define ADC_REGULAR_DISCNUM                 (7UL)

/*
 * @brief  Use by parameter examine.
 */
#define IS_ADC_ALL_PERIPH(PERIPH) (((PERIPH) == ADC0) || \
                                   ((PERIPH) == ADC1))

/*
 * @brief ADC Init Structure definition
 */
typedef struct
{
    uint32_t ADC_ClockDivide;                       /* ADC clock divide */
    uint32_t ADC_Resolution;                        /* ADC Conversion resolution selection setting */
    uint32_t ADC_Alignment;                         /* ADC result alignment setting */
    uint32_t ADC_Symbol;                            /* ADC data symbol enumeration setting */

    uint32_t ADC_RegularTrigger;                    /* regular group trigger source type */
    uint32_t ADC_InjectTrigger;                     /* injection group trigger source type */

    FunctionalState ADC_RegularEnable;              /* regular group trigger source enable */
    FunctionalState ADC_InjectEnable;               /* injection group trigger source enable */

    FunctionalState ADC_DmaEnable;                  /* Enable/disable ADC DMA support function */

    uint32_t ADC_TtrimRef;                          /* Set the Built-In reference voltage temperature ratio */
    uint32_t ADC_VtrimRef;                          /* Set the Built-In reference voltage output */
    uint32_t ADC_SelvRefbiRef;                      /* Set the Built-in reference output voltage */
    FunctionalState ADC_VrefbiEnable;               /* Enable/disable ADC Built-in reference voltage */
    uint32_t ADC_SelvihdlsRef;                      /* Set the SAR input type */

    FunctionalState ADC_ScanModeEn;                 /* Enable/disable scan mode for regular and injected group */
    FunctionalState ADC_ContinuousModeEn;           /* Enable/disable continuous mode */
    FunctionalState ADC_RegularDiscontinuousModeEn; /* Enable/disable discontinuous mode for regular group */
    FunctionalState ADC_InjectDiscontinuousModeEn;  /* Enable/disable discontinuous mode for injected group */
    FunctionalState ADC_InjectAutoModeEn;           /* Enable/disable antomatic mode for injected group */
    FunctionalState ADC_IntervalModeEn;             /* Enable/disable interval mode for mode3/5 */

    uint8_t ADC_RegularDiscontinuousNum;            /* Regular discontinuous mode number */

    uint8_t ADC_RegularSequenceLength;              /* Regular sequence length */
    uint8_t ADC_InjectSequenceLength;               /* Injected sequence length */

    FunctionalState ADC_PowerEn;                    /* Enable/disbale ADC analog module power supply */
    FunctionalState ADC_PowerRegain;                /* Enable/disbale ADC analog module power supply */
    FunctionalState ADC_BufEn;                      /* Enable/disbale ADC external reference input buffer */
} ADC_InitTypeDef;

/*
 * @brief Defines the control channel configuration
 */
typedef struct
{
    uint32_t ADC_Channel;                           /* Selection of input channel for measurement */
    uint32_t ADC_SampleTime;                        /* ADC sample time */
    FunctionalState ADC_InterruptEn;                /* ADC Enable interrupts for this channel */
} ADC_ChannelTypeDef;

/*
 * @brief ADC analog monitor(AMO) config structure
 */
typedef struct
{
    FunctionalState ADC_AmoInterruptEn;             /* Enable/disable AMO interrupt */
    FunctionalState ADC_AmoRegularEn;               /* Enable/disable analog monitor regular group mode */
    FunctionalState ADC_AmoInjectEn;                /* Enable/disable analog monitor injected group mode */
    FunctionalState ADC_AmoSingleModeEn;            /* Enable/disable analog monitor single channel mode */
    uint32_t ADC_AmoSingleChannel;                  /* Select analog monitor single channel */
    uint16_t ADC_AmoUpThreshold;                    /* Set analog monitor upper Threshold */
    uint16_t ADC_AmoLowThreshold;                   /* Set analog monitor lower Threshold */
} ADC_AmoTypeDef;

/*
 * @brief ADC  Temperature sensor config structure
 */
typedef struct
{
    uint32_t ADC_TempSamplePoint;                   /* Set the Temperature sampling points selection for mean algorithm */
    uint32_t ADC_TempSensorOffset;                  /* Set the Temperature sensor offset adjustment */
    uint32_t ADC_TempSensorGain;                    /* Set the Temperature sensor gain adjustment signal */
    FunctionalState TempSensorEn;                   /* Enable/disable Temperature sensor */
} ADC_TempTypeDef;

/*
 * @brief ADC Clock Divider selection
 */
#define ADC_CLK_DIVIDE_1                    ((uint32_t)0x00000000)  /* Input clock divided by 1. */
#define ADC_CLK_DIVIDE_2                    ((uint32_t)0x00000001)  /* Input clock divided by 2. */
#define ADC_CLK_DIVIDE_4                    ((uint32_t)0x00000002)  /* Input clock divided by 4. */
#define ADC_CLK_DIVIDE_6                    ((uint32_t)0x00000003)  /* Input clock divided by 6. */
#define ADC_CLK_DIVIDE_8                    ((uint32_t)0x00000004)  /* Input clock divided by 8. */
#define ADC_CLK_DIVIDE_10                   ((uint32_t)0x00000005)  /* Input clock divided by 10. */
#define ADC_CLK_DIVIDE_12                   ((uint32_t)0x00000006)  /* Input clock divided by 12. */
#define ADC_CLK_DIVIDE_14                   ((uint32_t)0x00000007)  /* Input clock divided by 14. */
#define ADC_CLK_DIVIDE_MAX                  ((uint32_t)0x00000008)  /* Invalid Input clock divided */
#define IS_ADC_CLK_DIVIDE(CLKDIVIDET) (((CLKDIVIDET) == ADC_CLK_DIVIDE_1) || ((CLKDIVIDET) == ADC_CLK_DIVIDE_2) || \
                                       ((CLKDIVIDET) == ADC_CLK_DIVIDE_4) || ((CLKDIVIDET) == ADC_CLK_DIVIDE_6) || \
                                       ((CLKDIVIDET) == ADC_CLK_DIVIDE_8) || ((CLKDIVIDET) == ADC_CLK_DIVIDE_10) || \
                                       ((CLKDIVIDET) == ADC_CLK_DIVIDE_12) || ((CLKDIVIDET) == ADC_CLK_DIVIDE_14))

/*
 * @brief ADC Conversion resolution selection
 */
#define ADC_RESOLUTION_12BIT                ((uint32_t)0x00000000)  /* 12-bit resolution mode */
#define ADC_RESOLUTION_10BIT                ((uint32_t)0x00000001)  /* 10-bit resolution mode */
#define ADC_RESOLUTION_8BIT                 ((uint32_t)0x00000002)  /* 8-bit resolution mode */
#define ADC_RESOLUTION_6BIT                 ((uint32_t)0x00000003)  /* 6-bit resolution mode */
#define ADC_RESOLUTION_MAX                  ((uint32_t)0x00000004)  /* Invalid resolution mode */
#define IS_ADC_RESOLUTION(RESOLUTIONT) (((RESOLUTIONT) == ADC_RESOLUTION_12BIT) || ((RESOLUTIONT) == ADC_RESOLUTION_10BIT) || \
                                        ((RESOLUTIONT) == ADC_RESOLUTION_8BIT) || ((RESOLUTIONT) == ADC_RESOLUTION_6BIT))

/*
 * @brief ADC data alignment enumeration.
 */
#define ADC_DATA_ALIGN_RIGHT                ((uint32_t)0x00000000)  /* Data right alignment */
#define ADC_DATA_ALIGN_LEFT                 ((uint32_t)0x00000001)  /* Data left alignment */
#define IS_ADC_ALIGN(ALIGNT) (((ALIGNT) == ADC_DATA_ALIGN_RIGHT) || ((ALIGNT) == ADC_DATA_ALIGN_LEFT))

/*
 * @brief ADC data symbol enumeration.
 */
#define ADC_DATA_SELDO_UNSIGNED             ((uint32_t)0x00000000)  /* Unsigned data */
#define ADC_DATA_SELDO_SIGNED               ((uint32_t)0x00000001)  /* Signed data */
#define IS_ADC_SELDO(SELDOT) (((SELDOT) == ADC_DATA_SELDO_UNSIGNED) || ((SELDOT) == ADC_DATA_SELDO_SIGNED))

/*
 * @brief ADC trigger source selec mode enumeration.
 */
#define ADC_TRIGGER_INTERNAL                ((uint32_t)0x00000000)  /* Internal software trigger */
#define ADC_TRIGGER_EXTERNAL_0              ((uint32_t)0x00000001)  /* External signal trigger 0 */
#define ADC_TRIGGER_EXTERNAL_1              ((uint32_t)0x00000002)  /* External signal trigger 1 */
#define ADC_TRIGGER_EXTERNAL_2              ((uint32_t)0x00000003)  /* External signal trigger 2 */
#define ADC_TRIGGER_EXTERNAL_MAX            ((uint32_t)0x00000004)  /* Invalid trigger */
#define IS_ADC_TRIGGER(TRIGGERT) (((TRIGGERT) == ADC_TRIGGER_INTERNAL) || ((TRIGGERT) == ADC_TRIGGER_EXTERNAL_0) || \
                                  ((TRIGGERT) == ADC_TRIGGER_EXTERNAL_1) || ((TRIGGERT) == ADC_TRIGGER_EXTERNAL_2))

/*
 * @brief ADC Built-In reference voltage temperature ratio control enumeration
 */
#define ADC_TTRIM_0                         ((uint32_t)0x00000000)   /* Built-In reference voltage temperature ratio control 0 */
#define ADC_TTRIM_1                         ((uint32_t)0x00000001)   /* Built-In reference voltage temperature ratio control 1 */
#define ADC_TTRIM_2                         ((uint32_t)0x00000002)   /* Built-In reference voltage temperature ratio control 2 */
#define ADC_TTRIM_3                         ((uint32_t)0x00000003)   /* Built-In reference voltage temperature ratio control 3 */
#define ADC_TTRIM_4                         ((uint32_t)0x00000004)   /* Built-In reference voltage temperature ratio control 4 */
#define ADC_TTRIM_5                         ((uint32_t)0x00000005)   /* Built-In reference voltage temperature ratio control 5 */
#define ADC_TTRIM_6                         ((uint32_t)0x00000006)   /* Built-In reference voltage temperature ratio control 6 */
#define ADC_TTRIM_7                         ((uint32_t)0x00000007)   /* Built-In reference voltage temperature ratio control 7 */
#define ADC_TTRIM_8                         ((uint32_t)0x00000008)   /* Built-In reference voltage temperature ratio control 8 */
#define ADC_TTRIM_9                         ((uint32_t)0x00000009)   /* Built-In reference voltage temperature ratio control 9 */
#define ADC_TTRIM_10                        ((uint32_t)0x0000000A)   /* Built-In reference voltage temperature ratio control 10 */
#define ADC_TTRIM_11                        ((uint32_t)0x0000000B)   /* Built-In reference voltage temperature ratio control 11 */
#define ADC_TTRIM_12                        ((uint32_t)0x0000000C)   /* Built-In reference voltage temperature ratio control 12 */
#define ADC_TTRIM_13                        ((uint32_t)0x0000000D)   /* Built-In reference voltage temperature ratio control 13 */
#define ADC_TTRIM_14                        ((uint32_t)0x0000000E)   /* Built-In reference voltage temperature ratio control 14 */
#define ADC_TTRIM_15                        ((uint32_t)0x0000000F)   /* Built-In reference voltage temperature ratio control 15 */
#define ADC_TTRIM_MAX                       ((uint32_t)0x00000010)   /* Invalid Built-In reference voltage temperature ratio control */
#define IS_ADC_TTRIM(TTRIMT)   (((TTRIMT) == ADC_TTRIM_0) || ((TTRIMT) == ADC_TTRIM_1) || \
                                ((TTRIMT) == ADC_TTRIM_2) || ((TTRIMT) == ADC_TTRIM_3) || \
                                ((TTRIMT) == ADC_TTRIM_4) || ((TTRIMT) == ADC_TTRIM_5) || \
                                ((TTRIMT) == ADC_TTRIM_6) || ((TTRIMT) == ADC_TTRIM_7) || \
                                ((TTRIMT) == ADC_TTRIM_8) || ((TTRIMT) == ADC_TTRIM_9) || \
                                ((TTRIMT) == ADC_TTRIM_10) || ((TTRIMT) == ADC_TTRIM_11) || \
                                ((TTRIMT) == ADC_TTRIM_12) || ((TTRIMT) == ADC_TTRIM_13) || \
                                ((TTRIMT) == ADC_TTRIM_14) || ((TTRIMT) == ADC_TTRIM_15))

/*
 * @brief ADC Built-In reference voltage output control enumeration
 */
#define ADC_VTRIM_0                         ((uint32_t)0x00000000)   /* ADC Built-In reference voltage output control 0 */
#define ADC_VTRIM_1                         ((uint32_t)0x00000001)   /* ADC Built-In reference voltage output control 1 */
#define ADC_VTRIM_2                         ((uint32_t)0x00000002)   /* ADC Built-In reference voltage output control 2 */
#define ADC_VTRIM_3                         ((uint32_t)0x00000003)   /* ADC Built-In reference voltage output control 3 */
#define ADC_VTRIM_4                         ((uint32_t)0x00000004)   /* ADC Built-In reference voltage output control 4 */
#define ADC_VTRIM_5                         ((uint32_t)0x00000005)   /* ADC Built-In reference voltage output control 5 */
#define ADC_VTRIM_6                         ((uint32_t)0x00000006)   /* ADC Built-In reference voltage output control 6 */
#define ADC_VTRIM_7                         ((uint32_t)0x00000007)   /* ADC Built-In reference voltage output control 7 */
#define ADC_VTRIM_8                         ((uint32_t)0x00000008)   /* ADC Built-In reference voltage output control 8 */
#define ADC_VTRIM_9                         ((uint32_t)0x00000009)   /* ADC Built-In reference voltage output control 9 */
#define ADC_VTRIM_10                        ((uint32_t)0x0000000A)   /* ADC Built-In reference voltage output control 10 */
#define ADC_VTRIM_11                        ((uint32_t)0x0000000B)   /* ADC Built-In reference voltage output control 11 */
#define ADC_VTRIM_12                        ((uint32_t)0x0000000C)   /* ADC Built-In reference voltage output control 12 */
#define ADC_VTRIM_13                        ((uint32_t)0x0000000D)   /* ADC Built-In reference voltage output control 13 */
#define ADC_VTRIM_14                        ((uint32_t)0x0000000E)   /* ADC Built-In reference voltage output control 14 */
#define ADC_VTRIM_15                        ((uint32_t)0x0000000F)   /* ADC Built-In reference voltage output control 15 */
#define ADC_VTRIM_16                        ((uint32_t)0x00000010)   /* ADC Built-In reference voltage output control 16 */
#define ADC_VTRIM_17                        ((uint32_t)0x00000011)   /* ADC Built-In reference voltage output control 17 */
#define ADC_VTRIM_18                        ((uint32_t)0x00000012)   /* ADC Built-In reference voltage output control 18 */
#define ADC_VTRIM_19                        ((uint32_t)0x00000013)   /* ADC Built-In reference voltage output control 19 */
#define ADC_VTRIM_20                        ((uint32_t)0x00000014)   /* ADC Built-In reference voltage output control 20 */
#define ADC_VTRIM_21                        ((uint32_t)0x00000015)   /* ADC Built-In reference voltage output control 21 */
#define ADC_VTRIM_22                        ((uint32_t)0x00000016)   /* ADC Built-In reference voltage output control 22 */
#define ADC_VTRIM_23                        ((uint32_t)0x00000017)   /* ADC Built-In reference voltage output control 23 */
#define ADC_VTRIM_24                        ((uint32_t)0x00000018)   /* ADC Built-In reference voltage output control 24 */
#define ADC_VTRIM_25                        ((uint32_t)0x00000019)   /* ADC Built-In reference voltage output control 25 */
#define ADC_VTRIM_26                        ((uint32_t)0x0000001A)   /* ADC Built-In reference voltage output control 26 */
#define ADC_VTRIM_27                        ((uint32_t)0x0000001B)   /* ADC Built-In reference voltage output control 27 */
#define ADC_VTRIM_28                        ((uint32_t)0x0000001C)   /* ADC Built-In reference voltage output control 28 */
#define ADC_VTRIM_29                        ((uint32_t)0x0000001D)   /* ADC Built-In reference voltage output control 29 */
#define ADC_VTRIM_30                        ((uint32_t)0x0000001E)   /* ADC Built-In reference voltage output control 30 */
#define ADC_VTRIM_31                        ((uint32_t)0x0000001F)   /* ADC Built-In reference voltage output control 31 */
#define ADC_VTRIM_MAX                       ((uint32_t)0x00000020)   /* Invalid Built-In reference voltage output control */
#define IS_ADC_VTRIM(VTRIMT)   (((VTRIMT) == ADC_VTRIM_0) || ((VTRIMT) == ADC_VTRIM_1) || \
                                ((VTRIMT) == ADC_VTRIM_2) || ((VTRIMT) == ADC_VTRIM_3) || \
                                ((VTRIMT) == ADC_VTRIM_4) || ((VTRIMT) == ADC_VTRIM_5) || \
                                ((VTRIMT) == ADC_VTRIM_6) || ((VTRIMT) == ADC_VTRIM_7) || \
                                ((VTRIMT) == ADC_VTRIM_8) || ((VTRIMT) == ADC_VTRIM_9) || \
                                ((VTRIMT) == ADC_VTRIM_10) || ((VTRIMT) == ADC_VTRIM_11) || \
                                ((VTRIMT) == ADC_VTRIM_12) || ((VTRIMT) == ADC_VTRIM_13) || \
                                ((VTRIMT) == ADC_VTRIM_14) || ((VTRIMT) == ADC_VTRIM_15) || \
                                ((VTRIMT) == ADC_VTRIM_16) || ((VTRIMT) == ADC_VTRIM_17) || \
                                ((VTRIMT) == ADC_VTRIM_18) || ((VTRIMT) == ADC_VTRIM_19) || \
                                ((VTRIMT) == ADC_VTRIM_20) || ((VTRIMT) == ADC_VTRIM_21) || \
                                ((VTRIMT) == ADC_VTRIM_22) || ((VTRIMT) == ADC_VTRIM_23) || \
                                ((VTRIMT) == ADC_VTRIM_24) || ((VTRIMT) == ADC_VTRIM_25) || \
                                ((VTRIMT) == ADC_VTRIM_26) || ((VTRIMT) == ADC_VTRIM_27) || \
                                ((VTRIMT) == ADC_VTRIM_28) || ((VTRIMT) == ADC_VTRIM_29) || \
                                ((VTRIMT) == ADC_VTRIM_30) || ((VTRIMT) == ADC_VTRIM_31))

/*
 * @brief ADC Built-in reference output voltage selected enumeration
 */
#define ADC_SEL_VREFBI_1V5                  ((uint32_t)0x00000000)  /* Built-in reference output voltage selected 1.5V */
#define ADC_SEL_VREFBI_2V0                  ((uint32_t)0x00000001)  /* Built-in reference output voltage selected 2.0V */
#define ADC_SEL_VREFBI_2V5                  ((uint32_t)0x00000002)  /* Built-in reference output voltage selected 2.5V */
#define ADC_SEL_VREFBI_MAX                  ((uint32_t)0x00000004)  /* Invalid Built-in reference output voltage selected */
#define IS_ADC_SELVREFBI(SELVREFBIT) (((SELVREFBIT) == ADC_SEL_VREFBI_1V5) || ((SELVREFBIT) == ADC_SEL_VREFBI_2V0) || \
                                      ((SELVREFBIT) == ADC_SEL_VREFBI_2V5))

/*
 * @brief ADC SAR input type selection selection
 */
#define ADC_SAR_INPUT_SINGLE_ENDED          ((uint32_t)0x00000000)  /* single-ended input */
#define ADC_SAR_INPUT_DIFFERENTIAL          ((uint32_t)0x00000001)  /* Differential input */
#define IS_ADC_SELVIHDLS(SELVIHDLST)   (((SELVIHDLST) == ADC_SAR_INPUT_SINGLE_ENDED) || \
                                        ((SELVIHDLST) == ADC_SAR_INPUT_DIFFERENTIAL))

/*
 * @brief ADC channel type
 */
#define ADC_CH_1                            ((uint32_t)0x00000000)  /* ADC channel 1 */
#define ADC_CH_2                            ((uint32_t)0x00000001)  /* ADC channel 2 */
#define ADC_CH_3                            ((uint32_t)0x00000002)  /* ADC channel 3 */
#define ADC_CH_4                            ((uint32_t)0x00000003)  /* ADC channel 4 */
#define ADC_CH_5                            ((uint32_t)0x00000004)  /* ADC channel 5 */
#define ADC_CH_6                            ((uint32_t)0x00000005)  /* ADC channel 6 */
#define ADC_CH_7                            ((uint32_t)0x00000006)  /* ADC channel 7 */
#define ADC_CH_8                            ((uint32_t)0x00000007)  /* ADC channel 8 */
#define ADC_CH_9                            ((uint32_t)0x00000008)  /* ADC channel 9 */
#define ADC_CH_10                           ((uint32_t)0x00000009)  /* ADC channel 10 */
#define ADC_CH_11                           ((uint32_t)0x0000000A)  /* ADC channel 11 */
#define ADC_CH_12                           ((uint32_t)0x0000000B)  /* ADC channel 12 */
#define ADC_CH_13                           ((uint32_t)0x0000000C)  /* ADC channel 13 */
#define ADC_CH_14                           ((uint32_t)0x0000000D)  /* ADC channel 14 */
#define ADC_CH_15                           ((uint32_t)0x0000000E)  /* ADC channel 15 */
#define ADC_CH_16                           ((uint32_t)0x0000000F)  /* ADC channel 16 */
#define ADC_CH_VTEMP                        ((uint32_t)0x00000010)  /* ADC Temperature sensor channel */
#define ADC_CH_VBAT                         ((uint32_t)0x00000011)  /* ADC detected external battery voltage input channel */
#define ADC_CH_VR_EX_1                      ((uint32_t)0x00000012)  /* ADC detected external reference input channel 1 */
#define ADC_CH_VR_EX_2                      ((uint32_t)0x00000013)  /* ADC detected external reference input channel 2 */
#define ADC_CH_VR_EX_3                      ((uint32_t)0x00000014)  /* ADC detected external reference input channel 3 */
#define ADC_CH_MAX                          ((uint32_t)0x00000015)  /* Invalid channel */
#define ADC_CH_DISABLE                      ((uint32_t)0x00000020)  /* Disabled channel */
#define IS_ADC_INPUT_CH(INPUTCHT) (((INPUTCHT) == ADC_CH_1) || ((INPUTCHT) == ADC_CH_2) ||  \
                                   ((INPUTCHT) == ADC_CH_3) || ((INPUTCHT) == ADC_CH_4) ||  \
                                   ((INPUTCHT) == ADC_CH_5) || ((INPUTCHT) == ADC_CH_6) ||  \
                                   ((INPUTCHT) == ADC_CH_7) || ((INPUTCHT) == ADC_CH_8) ||  \
                                   ((INPUTCHT) == ADC_CH_9) || ((INPUTCHT) == ADC_CH_10) || \
                                   ((INPUTCHT) == ADC_CH_11) || ((INPUTCHT) == ADC_CH_12) || \
                                   ((INPUTCHT) == ADC_CH_13) || ((INPUTCHT) == ADC_CH_14) || \
                                   ((INPUTCHT) == ADC_CH_15) || ((INPUTCHT) == ADC_CH_16) || \
                                   ((INPUTCHT) == ADC_CH_VTEMP) || ((INPUTCHT) == ADC_CH_VBAT) || \
                                   ((INPUTCHT) == ADC_CH_VR_EX_1) || ((INPUTCHT) == ADC_CH_VR_EX_2) || \
                                   ((INPUTCHT) == ADC_CH_VR_EX_3))

/*
 * @brief ADC sample time
 */
#define ADC_SPT_CLK_3                       ((uint32_t)0x00000000)  /* ADC Sample time 3 Clock Cycle */
#define ADC_SPT_CLK_5                       ((uint32_t)0x00000001)  /* ADC Sample time 5 Clock Cycle */
#define ADC_SPT_CLK_7                       ((uint32_t)0x00000002)  /* ADC Sample time 7 Clock Cycle */
#define ADC_SPT_CLK_10                      ((uint32_t)0x00000003)  /* ADC Sample time 10 Clock Cycle */
#define ADC_SPT_CLK_13                      ((uint32_t)0x00000004)  /* ADC Sample time 13 Clock Cycle */
#define ADC_SPT_CLK_16                      ((uint32_t)0x00000005)  /* ADC Sample time 16 Clock Cycle */
#define ADC_SPT_CLK_20                      ((uint32_t)0x00000006)  /* ADC Sample time 20 Clock Cycle */
#define ADC_SPT_CLK_30                      ((uint32_t)0x00000007)  /* ADC Sample time 30 Clock Cycle */
#define ADC_SPT_CLK_60                      ((uint32_t)0x00000008)  /* ADC Sample time 60 Clock Cycle */
#define ADC_SPT_CLK_80                      ((uint32_t)0x00000009)  /* ADC Sample time 80 Clock Cycle */
#define ADC_SPT_CLK_100                     ((uint32_t)0x0000000A)  /* ADC Sample time 100 Clock Cycle */
#define ADC_SPT_CLK_120                     ((uint32_t)0x0000000B)  /* ADC Sample time 120 Clock Cycle */
#define ADC_SPT_CLK_160                     ((uint32_t)0x0000000C)  /* ADC Sample time 160 Clock Cycle */
#define ADC_SPT_CLK_320                     ((uint32_t)0x0000000D)  /* ADC Sample time 320 Clock Cycle */
#define ADC_SPT_CLK_480                     ((uint32_t)0x0000000E)  /* ADC Sample time 480 Clock Cycle */
#define ADC_SPT_CLK_640                     ((uint32_t)0x0000000F)  /* ADC Sample time 640 Clock Cycle */
#define ADC_SPT_CLK_MAX                     ((uint32_t)0x00000010)  /* Invalid ADC Sample time */
#define IS_ADC_SAMPLE_TIME(SAMPLETIMET) (((SAMPLETIMET) == ADC_SPT_CLK_3) || ((SAMPLETIMET) == ADC_SPT_CLK_5) || \
                                         ((SAMPLETIMET) == ADC_SPT_CLK_7) || ((SAMPLETIMET) == ADC_SPT_CLK_10) || \
                                         ((SAMPLETIMET) == ADC_SPT_CLK_13) || ((SAMPLETIMET) == ADC_SPT_CLK_16) || \
                                         ((SAMPLETIMET) == ADC_SPT_CLK_20) || ((SAMPLETIMET) == ADC_SPT_CLK_30) || \
                                         ((SAMPLETIMET) == ADC_SPT_CLK_60) || ((SAMPLETIMET) == ADC_SPT_CLK_80) || \
                                         ((SAMPLETIMET) == ADC_SPT_CLK_100) || ((SAMPLETIMET) == ADC_SPT_CLK_120) || \
                                         ((SAMPLETIMET) == ADC_SPT_CLK_160) || ((SAMPLETIMET) == ADC_SPT_CLK_320) || \
                                         ((SAMPLETIMET) == ADC_SPT_CLK_480) || ((SAMPLETIMET) == ADC_SPT_CLK_640))

/*
 * @brief ADC Temperature sampling points
 */
#define ADC_ALG_MEAN_32                     ((uint32_t)0x00000000)  /* Temperature sampling points 32 */
#define ADC_ALG_MEAN_64                     ((uint32_t)0x00000001)  /* Temperature sampling points 64 */
#define ADC_ALG_MEAN_128                    ((uint32_t)0x00000002)  /* Temperature sampling points 128 */
#define ADC_ALG_MEAN_256                    ((uint32_t)0x00000003)  /* Temperature sampling points 256 */
#define ADC_ALG_MEAN_MAX                    ((uint32_t)0x00000004)  /* Invalid temperature sampling points */
#define IS_ADC_ALGMEAN(ALGMEANT) (((ALGMEANT) == ADC_ALG_MEAN_32) || ((ALGMEANT) == ADC_ALG_MEAN_64) || \
                                  ((ALGMEANT) == ADC_ALG_MEAN_128) || ((ALGMEANT) == ADC_ALG_MEAN_256))

/*
 * @brief ADC temperature sensor offset adjustment enumeration
 */
#define ADC_ADJ_TD_OS_0                     ((uint32_t)0x00000000)  /* Temperature sensor offset adjustment 0 */
#define ADC_ADJ_TD_OS_1                     ((uint32_t)0x00000001)  /* Temperature sensor offset adjustment 1 */
#define ADC_ADJ_TD_OS_2                     ((uint32_t)0x00000002)  /* Temperature sensor offset adjustment 2 */
#define ADC_ADJ_TD_OS_3                     ((uint32_t)0x00000003)  /* Temperature sensor offset adjustment 3 */
#define ADC_ADJ_TD_OS_4                     ((uint32_t)0x00000004)  /* Temperature sensor offset adjustment 4 */
#define ADC_ADJ_TD_OS_5                     ((uint32_t)0x00000005)  /* Temperature sensor offset adjustment 5 */
#define ADC_ADJ_TD_OS_6                     ((uint32_t)0x00000006)  /* Temperature sensor offset adjustment 6 */
#define ADC_ADJ_TD_OS_7                     ((uint32_t)0x00000007)  /* Temperature sensor offset adjustment 7 */
#define ADC_ADJ_TD_OS_8                     ((uint32_t)0x00000008)  /* Temperature sensor offset adjustment 8 */
#define ADC_ADJ_TD_OS_9                     ((uint32_t)0x00000009)  /* Temperature sensor offset adjustment 9 */
#define ADC_ADJ_TD_OS_10                    ((uint32_t)0x0000000A)  /* Temperature sensor offset adjustment 10 */
#define ADC_ADJ_TD_OS_11                    ((uint32_t)0x0000000B)  /* Temperature sensor offset adjustment 11 */
#define ADC_ADJ_TD_OS_12                    ((uint32_t)0x0000000C)  /* Temperature sensor offset adjustment 12 */
#define ADC_ADJ_TD_OS_13                    ((uint32_t)0x0000000D)  /* Temperature sensor offset adjustment 13 */
#define ADC_ADJ_TD_OS_14                    ((uint32_t)0x0000000E)  /* Temperature sensor offset adjustment 14 */
#define ADC_ADJ_TD_OS_15                    ((uint32_t)0x0000000F)  /* Temperature sensor offset adjustment 15 */
#define ADC_ADJ_TD_OS_MAX                   ((uint32_t)0x00000010)  /* Invalid temperature sensor offset adjustment */
#define IS_ADC_ADJTDOS(ADJTDOST) (((ADJTDOST) == ADC_ADJ_TD_OS_0) || ((ADJTDOST) == ADC_ADJ_TD_OS_1) || \
                                  ((ADJTDOST) == ADC_ADJ_TD_OS_2) || ((ADJTDOST) == ADC_ADJ_TD_OS_3) || \
                                  ((ADJTDOST) == ADC_ADJ_TD_OS_4) || ((ADJTDOST) == ADC_ADJ_TD_OS_5) || \
                                  ((ADJTDOST) == ADC_ADJ_TD_OS_6) || ((ADJTDOST) == ADC_ADJ_TD_OS_7) || \
                                  ((ADJTDOST) == ADC_ADJ_TD_OS_8) || ((ADJTDOST) == ADC_ADJ_TD_OS_9) || \
                                  ((ADJTDOST) == ADC_ADJ_TD_OS_10) || ((ADJTDOST) == ADC_ADJ_TD_OS_11) || \
                                  ((ADJTDOST) == ADC_ADJ_TD_OS_12) || ((ADJTDOST) == ADC_ADJ_TD_OS_13) || \
                                  ((ADJTDOST) == ADC_ADJ_TD_OS_14) || ((ADJTDOST) == ADC_ADJ_TD_OS_15))

/*
 * @brief ADC temperature sensor gain adjustment enumeration
 */
#define ADC_ADJ_TD_GA_0                     ((uint32_t)0x00000000)   /* Temperature sensor gain adjustment 0 */
#define ADC_ADJ_TD_GA_1                     ((uint32_t)0x00000001)   /* Temperature sensor gain adjustment 1 */
#define ADC_ADJ_TD_GA_2                     ((uint32_t)0x00000002)   /* Temperature sensor gain adjustment 2 */
#define ADC_ADJ_TD_GA_3                     ((uint32_t)0x00000003)   /* Temperature sensor gain adjustment 3 */
#define ADC_ADJ_TD_GA_4                     ((uint32_t)0x00000004)   /* Temperature sensor gain adjustment 4 */
#define ADC_ADJ_TD_GA_5                     ((uint32_t)0x00000005)   /* Temperature sensor gain adjustment 5 */
#define ADC_ADJ_TD_GA_6                     ((uint32_t)0x00000006)   /* Temperature sensor gain adjustment 6 */
#define ADC_ADJ_TD_GA_7                     ((uint32_t)0x00000007)   /* Temperature sensor gain adjustment 7 */
#define ADC_ADJ_TD_GA_8                     ((uint32_t)0x00000008)   /* Temperature sensor gain adjustment 8 */
#define ADC_ADJ_TD_GA_9                     ((uint32_t)0x00000009)   /* Temperature sensor gain adjustment 9 */
#define ADC_ADJ_TD_GA_10                    ((uint32_t)0x0000000A)   /* Temperature sensor gain adjustment 10 */
#define ADC_ADJ_TD_GA_11                    ((uint32_t)0x0000000B)   /* Temperature sensor gain adjustment 11 */
#define ADC_ADJ_TD_GA_12                    ((uint32_t)0x0000000C)   /* Temperature sensor gain adjustment 12 */
#define ADC_ADJ_TD_GA_13                    ((uint32_t)0x0000000D)   /* Temperature sensor gain adjustment 13 */
#define ADC_ADJ_TD_GA_14                    ((uint32_t)0x0000000E)   /* Temperature sensor gain adjustment 14 */
#define ADC_ADJ_TD_GA_15                    ((uint32_t)0x0000000F)   /* Temperature sensor gain adjustment 15 */
#define ADC_ADJ_TD_GA_MAX                   ((uint32_t)0x00000010)   /* Invalid temperature sensor gain adjustment */
#define IS_ADC_ADJTDGA(ADJTDGAT)   (((ADJTDGAT) == ADC_ADJ_TD_GA_0) || ((ADJTDGAT) == ADC_ADJ_TD_GA_1) || \
                                    ((ADJTDGAT) == ADC_ADJ_TD_GA_2) || ((ADJTDGAT) == ADC_ADJ_TD_GA_3) || \
                                    ((ADJTDGAT) == ADC_ADJ_TD_GA_4) || ((ADJTDGAT) == ADC_ADJ_TD_GA_5) || \
                                    ((ADJTDGAT) == ADC_ADJ_TD_GA_6) || ((ADJTDGAT) == ADC_ADJ_TD_GA_7) || \
                                    ((ADJTDGAT) == ADC_ADJ_TD_GA_8) || ((ADJTDGAT) == ADC_ADJ_TD_GA_9) || \
                                    ((ADJTDGAT) == ADC_ADJ_TD_GA_10) || ((ADJTDGAT) == ADC_ADJ_TD_GA_11) || \
                                    ((ADJTDGAT) == ADC_ADJ_TD_GA_12) || ((ADJTDGAT) == ADC_ADJ_TD_GA_13) || \
                                    ((ADJTDGAT) == ADC_ADJ_TD_GA_14) || ((ADJTDGAT) == ADC_ADJ_TD_GA_15))

/*
 * @brief ADC sequence type enumeration
 */
#define ADC_RSEQ_0                          ((uint32_t)0x00000000) /* ADC rseq 0 */
#define ADC_RSEQ_1                          ((uint32_t)0x00000001) /* ADC rseq 1 */
#define ADC_RSEQ_2                          ((uint32_t)0x00000002) /* ADC rseq 2 */
#define ADC_RSEQ_3                          ((uint32_t)0x00000003) /* ADC rseq 3 */
#define ADC_RSEQ_4                          ((uint32_t)0x00000004) /* ADC rseq 4 */
#define ADC_RSEQ_5                          ((uint32_t)0x00000005) /* ADC rseq 5 */
#define ADC_RSEQ_6                          ((uint32_t)0x00000006) /* ADC rseq 6 */
#define ADC_RSEQ_7                          ((uint32_t)0x00000007) /* ADC rseq 7 */
#define ADC_RSEQ_8                          ((uint32_t)0x00000008) /* ADC rseq 8 */
#define ADC_RSEQ_9                          ((uint32_t)0x00000009) /* ADC rseq 9 */
#define ADC_RSEQ_10                         ((uint32_t)0x0000000A) /* ADC rseq 10 */
#define ADC_RSEQ_11                         ((uint32_t)0x0000000B) /* ADC rseq 11 */
#define ADC_RSEQ_12                         ((uint32_t)0x0000000C) /* ADC rseq 12 */
#define ADC_RSEQ_13                         ((uint32_t)0x0000000D) /* ADC rseq 13 */
#define ADC_RSEQ_14                         ((uint32_t)0x0000000E) /* ADC rseq 14 */
#define ADC_RSEQ_15                         ((uint32_t)0x0000000F) /* ADC rseq 15 */
#define ADC_ISEQ_0                          ((uint32_t)0x00000010) /* ADC iseq 0 */
#define ADC_ISEQ_1                          ((uint32_t)0x00000011) /* ADC iseq 1 */
#define ADC_ISEQ_2                          ((uint32_t)0x00000012) /* ADC iseq 2 */
#define ADC_ISEQ_3                          ((uint32_t)0x00000013) /* ADC iseq 3 */
#define ADC_SEQ_MAX                         ((uint32_t)0x00000014) /* Invalid seq */
#define IS_ADC_SEQUENCE(SEQUENCET) (((SEQUENCET) == ADC_RSEQ_0) || ((SEQUENCET) == ADC_RSEQ_1) || \
                                    ((SEQUENCET) == ADC_RSEQ_2) || ((SEQUENCET) == ADC_RSEQ_3) || \
                                    ((SEQUENCET) == ADC_RSEQ_4) || ((SEQUENCET) == ADC_RSEQ_5) || \
                                    ((SEQUENCET) == ADC_RSEQ_6) || ((SEQUENCET) == ADC_RSEQ_7) || \
                                    ((SEQUENCET) == ADC_RSEQ_8) || ((SEQUENCET) == ADC_RSEQ_9) || \
                                    ((SEQUENCET) == ADC_RSEQ_10) || ((SEQUENCET) == ADC_RSEQ_11) || \
                                    ((SEQUENCET) == ADC_RSEQ_12) || ((SEQUENCET) == ADC_RSEQ_13) || \
                                    ((SEQUENCET) == ADC_RSEQ_14) || ((SEQUENCET) == ADC_RSEQ_15) || \
                                    ((SEQUENCET) == ADC_ISEQ_0) || ((SEQUENCET) == ADC_ISEQ_1) || \
                                    ((SEQUENCET) == ADC_ISEQ_2) || ((SEQUENCET) == ADC_ISEQ_3))

/*
 * @brief ADC Flags Definition
 */
#define ADC_FLAG_CONVCFT		ADC_STATE_CONVCFT   /* Convert trigger conflict status flag */
#define ADC_FLAG_IDLE			ADC_STATE_IDLE    /* ADC idle state flag */
#define ADC_FLAG_AMO			ADC_STATE_AMO    /* Analog monitor event occurs flag */
#define ADC_FLAG_EOC            ADC_RSQR_EOC
#define IS_ADC_GET_FLAG(FLAG) (((STATE) == ADC_FLAG_CONVCFT) || \
                               ((STATE) == ADC_FLAG_IDLE) || \
							   ((STATE) == ADC_FLAG_AMO) || \
                               ((STATE) == ADC_FLAG_EOC))
#define IS_ADC_CLEAR_FLAG(FLAG) (((STATE) == ADC_FLAG_CONVCFT) || \
								 ((STATE) == ADC_FLAG_AMO) || \
                                 ((STATE) == ADC_FLAG_EOC))

/*
 * @brief Function declaration
 */
float ADC_GetAverageValue(ADC_TypeDef *ADCx, const uint32_t Seq, const uint32_t Count);
void ADC_StructInit(ADC_InitTypeDef * const ADC_InitStruct);
void ADC_Init(ADC_TypeDef *ADCx, const ADC_InitTypeDef * const ADC_InitStruct);
void ADC_GetConfig(ADC_TypeDef *ADCx, ADC_InitTypeDef * const ADC_InitStruct);
void ADC_InitAMOStruct(ADC_AmoTypeDef * const ADC_AmoStruct);
void ADC_ConfigAMO(ADC_TypeDef *ADCx, const ADC_AmoTypeDef * const ADC_AmoStruct);
void ADC_GetAMOConfig(ADC_TypeDef *ADCx, ADC_AmoTypeDef * const ADC_AmoStruct);
void ADC_InitTempStruct(ADC_TempTypeDef * const ADC_TempStruct);
void ADC_TempConfig(ADC_TypeDef *ADCx, ADC_TempTypeDef * const ADC_TempStruct);
void ADC_GetTempConfig(ADC_TypeDef *ADCx, ADC_TempTypeDef * const ADC_TempStruct);
void ADC_InitChannelStruct(ADC_ChannelTypeDef * const ADC_ChannelStruct);
void ADC_ConfigChannel(ADC_TypeDef *ADCx, const uint32_t SeqIndex, const ADC_ChannelTypeDef * const ADC_ChannelStruct);
void ADC_GetChannelConfig(ADC_TypeDef *ADCx, const uint32_t SeqIndex, ADC_ChannelTypeDef * const ADC_ChannelStruct);
void ADC_SetInjectOffset(ADC_TypeDef *ADCx, const uint32_t SeqIndex, const uint16_t offset);
uint16_t ADC_GetInjectOffset(ADC_TypeDef *ADCx, const uint32_t SeqIndex);
FlagStatus ADC_GetBufferStatus(ADC_TypeDef *ADCx);
void ADC_ClearBufferStatus(ADC_TypeDef *ADCx);
FlagStatus ADC_GetTempSensorStatus(ADC_TypeDef *ADCx);
void ADC_ClearTempSensorStatus(ADC_TypeDef *ADCx);
void ADC_SoftRegularConvert(ADC_TypeDef *ADCx);
void ADC_SoftInjectConvert(ADC_TypeDef *ADCx);
FlagStatus ADC_GetFlagStatus(ADC_TypeDef *ADCx, uint8_t ADC_FLAG, const uint32_t SeqIndex);
void ADC_ClearFlag(ADC_TypeDef* ADCx, uint8_t ADC_FLAG, const uint32_t SeqIndex);
void ADC_WaitConvDone(ADC_TypeDef *ADCx);
uint16_t ADC_GetSeqResult(ADC_TypeDef *ADCx, const uint32_t SeqIndex, uint16_t * const Result);
void ADC_SetHighResStatus(ADC_TypeDef *ADCx);
FlagStatus ADC_GetHighResStatus(ADC_TypeDef *ADCx);
void ADC_ClearHighResStatus(ADC_TypeDef *ADCx);
uint8_t ADC_GetRegularParity(ADC_TypeDef *ADCx, const uint32_t SeqIndex);
uint8_t ADC_GetRegularChannel(ADC_TypeDef *ADCx, const uint32_t SeqIndex);
uint8_t ADC_GetInjectParity(ADC_TypeDef *ADCx, const uint32_t SeqIndex);
uint8_t ADC_GetInjectChannel(ADC_TypeDef *ADCx, const uint32_t SeqIndex);
float ADC_ConvertToTemp(const uint16_t Value, const float RefVoltage);
static void ADC_InitDelayus(uint32_t Delayus);

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
