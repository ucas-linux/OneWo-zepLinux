/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_timer.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:			2024/08/08
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/08/08     1.0     build this module
 * Description:     This file contains all the functions prototypes for the TIMER firmware library.
 */

#ifndef AS32X601_TIMER_ST_H
#define AS32X601_TIMER_ST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601_timer_regs.h"



/*
 * @defgroup  TIM Time base Configuration Structure definition
 */
typedef struct
{
    uint32_t TIM_Prescaler;            /* Specifies the prescaler value used to divide the TIM clock */
    uint32_t TIM_CounterMode;          /* Specifies the counter mode */
    uint32_t TIM_Period;               /* Specifies the period value to be loaded into the active
                                           Auto-Reload Register at the next update event */
    uint32_t TIM_ClockDivision;        /* Specifies the clock division */
    uint32_t TIM_RepetitionCounter;    /* Specifies the repetition counter value */
} TIM_TimeBaseInitTypeDef;

/* 
 * @defgroup  TIM Output Compare Init structure definition  
 */
typedef struct
{
    uint32_t TIM_OCMode;                /* Specifies the TIM mode.*/
    uint32_t TIM_OutputState;           /* Specifies the TIM Output Compare state. */
    uint32_t TIM_OutputNState;          /* Specifies the TIM complementary Output Compare state.*/
    uint32_t TIM_Pulse;                 /* Specifies the pulse value to be loaded into the Capture Compare Register. */
    uint32_t TIM_OCPolarity;            /* Specifies the output polarity. */
    uint32_t TIM_OCNPolarity;           /* Specifies the complementary output polarity.*/
    uint32_t TIM_OCIdleState;           /* Specifies the TIM Output Compare pin state during Idle state. */
    uint32_t TIM_OCNIdleState;          /* Specifies the TIM Output Compare pin state during Idle state. */
} TIM_OCInitTypeDef;

/* 
 *@brief  TIM Input Capture Init structure definition  
 */
typedef struct
{

  uint32_t TIM_Channel;      /* Specifies the TIM channel.*/
  uint32_t TIM_ICPolarity;   /* Specifies the active edge of the input signal.*/
  uint32_t TIM_ICSelection;  /* Specifies the input.*/
  uint32_t TIM_ICPrescaler;  /* Specifies the Input Capture Prescaler.*/
  uint32_t TIM_ICFilter;     /* Specifies the input capture filter.*/
} TIM_ICInitTypeDef;

/*
 * @brief  BDTR structure definition 
 */
typedef struct
{

  uint32_t TIM_OSSRState;        /*!< Specifies the Off-State selection used in Run mode. */
  uint32_t TIM_OSSIState;        /*!< Specifies the Off-State used in Idle state. */
  uint32_t TIM_LOCKLevel;        /*!< Specifies the LOCK level parameters. */ 
  uint32_t TIM_DeadTime;         /*!< Specifies the delay time between the switching-off and the
                                      switching-on of the outputs. */
  uint32_t TIM_Break;            /*!< Specifies whether the TIM Break input is enabled or not.*/
  uint32_t TIM_BreakPolarity;    /*!< Specifies the TIM Break Input pin polarity. */
  uint32_t TIM_AutomaticOutput;  /*!< Specifies whether the TIM Automatic Output feature is enabled or not.*/
} TIM_BDTRInitTypeDef;

/*
 * @defgroup  Use by parameter examine.
 */
#define IS_TIM_ALL_PERIPH(PERIPH) (((PERIPH) == TIM0) || \
                                   ((PERIPH) == TIM1) || \
                                   ((PERIPH) == TIM2) || \
                                   ((PERIPH) == TIM3) || \
                                   ((PERIPH) == TIM4) || \
                                   ((PERIPH) == TIM5) || \
                                   ((PERIPH) == TIM6) || \
                                   ((PERIPH) == TIM7))

#define IS_TIM_ADV_PERIPH(PERIPH) (((PERIPH) == TIM0) || \
                                   ((PERIPH) == TIM1)  || \
                                   ((PERIPH) == TIM2)  || \
                                   ((PERIPH) == TIM5))

#define IS_TIM_GEN_PERIPH(PERIPH) (((PERIPH) == TIM3) || \
                                   ((PERIPH) == TIM4)  || \
                                   ((PERIPH) == TIM6)  || \
                                   ((PERIPH) == TIM7))

#define IS_TIM_IC_FILTER(ICFILTER) ((ICFILTER) <= 0xF) 

#define IS_TIM_EXT_FILTER(EXTFILTER) ((EXTFILTER) <= 0xF)

/*
 * @defgroup  TIM Channel
 */
#define TIM_Channel_1                      ((uint32_t)0x00000000)
#define TIM_Channel_2                      ((uint32_t)0x00000004)
#define TIM_Channel_3                      ((uint32_t)0x00000008)
#define TIM_Channel_4                      ((uint32_t)0x0000000C)
#define TIM_Channel_5                      ((uint32_t)0x00000010)
#define TIM_Channel_6                      ((uint32_t)0x00000014)
#define IS_TIM_CHANNEL(CHANNEL) (((CHANNEL) == TIM_Channel_1) || \
                                 ((CHANNEL) == TIM_Channel_2) || \
                                 ((CHANNEL) == TIM_Channel_3) || \
                                 ((CHANNEL) == TIM_Channel_4) || \
                                 ((CHANNEL) == TIM_Channel_5) || \
                                 ((CHANNEL) == TIM_Channel_6))
                                 

/*
 * @defgroup TIM Counter Mode
 */
#define TIM_CounterMode_Up                 ((uint32_t)0x00000000)
#define TIM_CounterMode_Down               ((uint32_t)0x00000010)
#define TIM_CounterMode_CenterAligned1     ((uint32_t)0x00000020)
#define TIM_CounterMode_CenterAligned2     ((uint32_t)0x00000040)
#define TIM_CounterMode_CenterAligned3     ((uint32_t)0x00000060)
#define IS_TIM_COUNTER_MODE(MODE) (((MODE) == TIM_CounterMode_Up) ||  \
                                   ((MODE) == TIM_CounterMode_Down) || \
                                   ((MODE) == TIM_CounterMode_CenterAligned1) || \
                                   ((MODE) == TIM_CounterMode_CenterAligned2) || \
                                   ((MODE) == TIM_CounterMode_CenterAligned3))

/*
 * @defgroup TIM Clock Division
 */
#define TIM_CKD_DIV1                       ((uint32_t)0x00000000)
#define TIM_CKD_DIV2                       ((uint32_t)0x00000100)
#define TIM_CKD_DIV4                       ((uint32_t)0x00000200)
#define IS_TIM_CKD_DIV(DIV) (((DIV) == TIM_CKD_DIV1) || \
                             ((DIV) == TIM_CKD_DIV2) || \
                             ((DIV) == TIM_CKD_DIV4))

/*
 * @defgroup TIM Prescaler Reload Mode
 */
#define TIM_PSCReloadMode_Update           ((uint32_t)0x00000000)
#define TIM_PSCReloadMode_Immediate        ((uint32_t)0x00000001)
#define IS_TIM_PRESCALER_RELOAD(RELOAD) (((RELOAD) == TIM_PSCReloadMode_Update) || \
                                         ((RELOAD) == TIM_PSCReloadMode_Immediate))

/*
 * @defgroup TIM Prescaler Reload Mode
 */
#define TIM_OPMode_Single                  ((uint32_t)0x00000008)
#define TIM_OPMode_Repetitive              ((uint32_t)0x00000000)
#define IS_TIM_OPM_MODE(MODE) (((MODE) == TIM_OPMode_Single) || \
                               ((MODE) == TIM_OPMode_Repetitive))



#define TIM_ForcedAction_Active            ((uint32_t)0x00000050)
#define TIM_ForcedAction_InActive          ((uint32_t)0x00000040)
#define IS_TIM_FORCED_ACTION(ACTION) (((ACTION) == TIM_ForcedAction_Active) || \
                                      ((ACTION) == TIM_ForcedAction_InActive))

/*
 * @defgroup TIM Output Compare and PWM modes 
 */
#define TIM_OCMode_Timing                  ((uint32_t)0x00000000)
#define TIM_OCMode_Active                  ((uint32_t)0x00000010)
#define TIM_OCMode_Inactive                ((uint32_t)0x00000020)
#define TIM_OCMode_Toggle                  ((uint32_t)0x00000030)
#define TIM_OCMode_PWM1                    ((uint32_t)0x00000060)
#define TIM_OCMode_PWM2                    ((uint32_t)0x00000070)
#define TIM_OCMode_OPM1                    ((uint32_t)0x00010000)
#define TIM_OCMode_OPM2                    ((uint32_t)0x00010010)
#define TIM_OCMode_1110                    ((uint32_t)0x00010060)
#define TIM_OCMode_1111                    ((uint32_t)0x00010070)
#define IS_TIM_OC_MODE(MODE) (((MODE) == TIM_OCMode_Timing) || \
                              ((MODE) == TIM_OCMode_Active) || \
                              ((MODE) == TIM_OCMode_Inactive) || \
                              ((MODE) == TIM_OCMode_Toggle)|| \
                              ((MODE) == TIM_OCMode_PWM1) || \
                              ((MODE) == TIM_OCMode_PWM2) ||\
                              ((MODE) == TIM_OCMode_OPM1) ||\
                              ((MODE) == TIM_OCMode_OPM2) ||\
                              ((MODE) == TIM_OCMode_1110) ||\
                              ((MODE) == TIM_OCMode_1111))
#define IS_TIM_OCM(MODE) (((MODE) == TIM_OCMode_Timing) || \
                          ((MODE) == TIM_OCMode_Active) || \
                          ((MODE) == TIM_OCMode_Inactive) || \
                          ((MODE) == TIM_OCMode_Toggle)|| \
                          ((MODE) == TIM_OCMode_PWM1) || \
                          ((MODE) == TIM_OCMode_PWM2) ||	\
                          ((MODE) == TIM_OCMode_OPM1) ||\
                          ((MODE) == TIM_OCMode_OPM2) ||\
                          ((MODE) == TIM_OCMode_1110) ||\
                          ((MODE) == TIM_OCMode_1111) ||\
                          ((MODE) == TIM_ForcedAction_Active) || \
                          ((MODE) == TIM_ForcedAction_InActive))

/*
 * @defgroup TIM Output Compare State
 */
#define TIM_OutputState_Disable            ((uint32_t)0x00000000)
#define TIM_OutputState_Enable             ((uint32_t)0x00000001)
#define IS_TIM_OUTPUT_STATE(STATE) (((STATE) == TIM_OutputState_Disable) || \
                                    ((STATE) == TIM_OutputState_Enable))

/*
 * @defgroup TIM Output Compare Polarity
 */
#define TIM_OCPolarity_High                ((uint32_t)0x00000000)
#define TIM_OCPolarity_Low                 ((uint32_t)0x00000002)
#define IS_TIM_OC_POLARITY(POLARITY) (((POLARITY) == TIM_OCPolarity_High) || \
                                      ((POLARITY) == TIM_OCPolarity_Low))

/*
 * @defgroup TIM Output Compare N State
 */
#define TIM_OutputNState_Disable           ((uint32_t)0x00000000)
#define TIM_OutputNState_Enable            ((uint32_t)0x00000004)
#define IS_TIM_OUTPUTN_STATE(STATE) (((STATE) == TIM_OutputNState_Disable) || \
                                     ((STATE) == TIM_OutputNState_Enable))

/*
 * @defgroup TIM Output Compare N Polarity
 */
#define TIM_OCNPolarity_High               ((uint32_t)0x00000000)
#define TIM_OCNPolarity_Low                ((uint32_t)0x00000008)
#define IS_TIM_OCN_POLARITY(POLARITY) (((POLARITY) == TIM_OCNPolarity_High) || \
                                       ((POLARITY) == TIM_OCNPolarity_Low))

/*
 * @defgroup TIM Output Compare N Idle State
 */
#define TIM_OCNIdleState_Set               ((uint32_t)0x00000200)
#define TIM_OCNIdleState_Reset             ((uint32_t)0x00000000)
#define IS_TIM_OCNIDLE_STATE(STATE) (((STATE) == TIM_OCNIdleState_Set) || \
                                     ((STATE) == TIM_OCNIdleState_Reset))

/*
 * @defgroup TIM Output Compare N Idle State
 */
#define TIM_OCIdleState_Set                ((uint32_t)0x00000100)
#define TIM_OCIdleState_Reset              ((uint32_t)0x00000000)
#define IS_TIM_OCIDLE_STATE(STATE) (((STATE) == TIM_OCIdleState_Set) || \
                                    ((STATE) == TIM_OCIdleState_Reset))

/*
 * @defgroup TIM Output Compare Idle State
 */
#define TIM_CCx_Enable                      ((uint32_t)0x00000001)
#define TIM_CCx_Disable                     ((uint32_t)0x00000000)
#define IS_TIM_CCX(CCX) (((CCX) == TIM_CCx_Enable) || \
                         ((CCX) == TIM_CCx_Disable))

/*
 * @defgroup TIM Output Compare Fast State
 */
#define TIM_OCFast_Enable                  ((uint32_t)0x00000004)
#define TIM_OCFast_Disable                 ((uint32_t)0x00000000)
#define IS_TIM_OCFAST_STATE(STATE) (((STATE) == TIM_OCFast_Enable) || \
                                    ((STATE) == TIM_OCFast_Disable))

/*
 * @defgroup TIM Output Compare Clear State
 */
#define TIM_OCClear_Enable                 ((uint32_t)0x00000080)
#define TIM_OCClear_Disable                ((uint32_t)0x00000000)
#define IS_TIM_OCCLEAR_STATE(STATE) (((STATE) == TIM_OCClear_Enable) || \
                                     ((STATE) == TIM_OCClear_Disable))

/*
 * @defgroup TIM Capture Compare N State
 */
#define TIM_CCxN_Enable                     ((uint32_t)0x00000004)
#define TIM_CCxN_Disable                    ((uint32_t)0x00000000)
#define IS_TIM_CCXN(CCXN) (((CCXN) == TIM_CCxN_Enable) || \
                           ((CCXN) == TIM_CCxN_Disable))

#define  TIM_ICPolarity_Rising               ((uint32_t)0x00000000)
#define  TIM_ICPolarity_Falling              ((uint32_t)0x00000002)
#define  TIM_ICPolarity_BothEdge             ((uint32_t)0x0000000A)
#define IS_TIM_IC_POLARITY(POLARITY) (((POLARITY) == TIM_ICPolarity_Rising) || \
                                      ((POLARITY) == TIM_ICPolarity_Falling)|| \
                                      ((POLARITY) == TIM_ICPolarity_BothEdge))

/*
 * @defgroup  TIM Input Capture Selection 
 */
#define TIM_ICSelection_DirectTI            ((uint32_t)0x00000001) /* TIM Input 1, 2, 3, 4,5 or 6 is selected to be 
                                                                   connected to IC1, IC2, IC3,IC4,IC5 or IC6, respectively */
#define TIM_ICSelection_IndirectTI          ((uint32_t)0x00000002) /* TIM Input 1, 2, 3, 4,5 or 6 is selected to be 
                                                                   connected to IC1, IC2, IC3,IC4,IC5 or IC6, respectively */
#define TIM_ICSelection_TRC                 ((uint32_t)0x00000003) /* TIM Input 1, 2, 3 ,4 ,5 or 6 is selected to be connected to TRC. */
#define IS_TIM_IC_SELECTION(SELECTION) (((SELECTION) == TIM_ICSelection_DirectTI) || \
                                        ((SELECTION) == TIM_ICSelection_IndirectTI) || \
                                        ((SELECTION) == TIM_ICSelection_TRC))

/*
 * @defgroup  TIM Input Capture Prescaler
 */
#define TIM_ICPSC_DIV1                     ((uint32_t)0x00000000) /*!< Capture performed each time an edge is detected on the capture input. */
#define TIM_ICPSC_DIV2                     ((uint32_t)0x00000004) /*!< Capture performed once every 2 events. */
#define TIM_ICPSC_DIV4                     ((uint32_t)0x00000008) /*!< Capture performed once every 4 events. */
#define TIM_ICPSC_DIV8                     ((uint32_t)0x0000000C) /*!< Capture performed once every 8 events. */
#define IS_TIM_IC_PRESCALER(PRESCALER) (((PRESCALER) == TIM_ICPSC_DIV1) || \
                                        ((PRESCALER) == TIM_ICPSC_DIV2) || \
                                        ((PRESCALER) == TIM_ICPSC_DIV4) || \
                                        ((PRESCALER) == TIM_ICPSC_DIV8))

/*
 * @defgroup  TIM OSSR Off State Selection for Run mode state
 */
#define TIM_OSSRState_Enable               ((uint32_t)0x00000800)
#define TIM_OSSRState_Disable              ((uint32_t)0x00000000)
#define IS_TIM_OSSR_STATE(STATE) (((STATE) == TIM_OSSRState_Enable) || \
                                  ((STATE) == TIM_OSSRState_Disable))

/*
 * @defgroup  TIM OSSI Off State Selection for Idle mode state
 */
#define TIM_OSSIState_Enable               ((uint32_t)0x00000400)
#define TIM_OSSIState_Disable              ((uint32_t)0x00000000)
#define IS_TIM_OSSI_STATE(STATE) (((STATE) == TIM_OSSIState_Enable) || \
                                  ((STATE) == TIM_OSSIState_Disable))

/*
 * @defgroup  TIM Lock level
 */
#define TIM_LOCKLevel_OFF                  ((uint32_t)0x00000000)
#define TIM_LOCKLevel_1                    ((uint32_t)0x00000100)
#define TIM_LOCKLevel_2                    ((uint32_t)0x00000200)
#define TIM_LOCKLevel_3                    ((uint32_t)0x00000300)
#define IS_TIM_LOCK_LEVEL(LEVEL) (((LEVEL) == TIM_LOCKLevel_OFF) || \
                                  ((LEVEL) == TIM_LOCKLevel_1) || \
                                  ((LEVEL) == TIM_LOCKLevel_2) || \
                                  ((LEVEL) == TIM_LOCKLevel_3))
/*
 * @defgroup  TIM Break Input enable disable
 */
#define TIM_Break_Enable                   ((uint32_t)0x00001000)
#define TIM_Break_Disable                  ((uint32_t)0x00000000)
#define IS_TIM_BREAK_STATE(STATE) (((STATE) == TIM_Break_Enable) || \
                                   ((STATE) == TIM_Break_Disable))

/*
 * @defgroup  TIM Break Polarity
 */
#define TIM_BreakPolarity_Low              ((uint32_t)0x00000000)
#define TIM_BreakPolarity_High             ((uint32_t)0x00002000)
#define IS_TIM_BREAK_POLARITY(POLARITY) (((POLARITY) == TIM_BreakPolarity_Low) || \
                                         ((POLARITY) == TIM_BreakPolarity_High))

/*
 * @defgroup  TIM AOE Bit Set Reset
 */
#define TIM_AutomaticOutput_Enable         ((uint32_t)0x00004000)
#define TIM_AutomaticOutput_Disable        ((uint32_t)0x00000000)
#define IS_TIM_AUTOMATIC_OUTPUT_STATE(STATE) (((STATE) == TIM_AutomaticOutput_Enable) || \
                                              ((STATE) == TIM_AutomaticOutput_Disable))

/*
 * @defgroup  TIM interrupt sources 
 */
#define TIM_IT_Update                      ((uint32_t)0x00000001)
#define TIM_IT_CC1                         ((uint32_t)0x00010000)
#define TIM_IT_CC2                         ((uint32_t)0x00020000)
#define TIM_IT_CC3                         ((uint32_t)0x00040000)
#define TIM_IT_CC4                         ((uint32_t)0x00080000)
#define TIM_IT_CC5                         ((uint32_t)0x00100000)
#define TIM_IT_CC6                         ((uint32_t)0x00200000)
#define TIM_IT_COM                         ((uint32_t)0x00000020)
#define TIM_IT_Trigger                     ((uint32_t)0x00000040)
#define TIM_IT_Break                       ((uint32_t)0x00000080)
#define IS_TIM_IT(IT) ((((IT) & (uint32_t)0xC0C0FEF2) == 0x00000000) && ((IT) != 0x00000000))

#define IS_TIM_GET_IT(IT) (((IT) == TIM_IT_Update) || \
                           ((IT) == TIM_IT_CC1) || \
                           ((IT) == TIM_IT_CC2) || \
                           ((IT) == TIM_IT_CC3) || \
                           ((IT) == TIM_IT_CC4) || \
                           ((IT) == TIM_IT_CC5) || \
                           ((IT) == TIM_IT_CC6) || \
                           ((IT) == TIM_IT_COM) || \
                           ((IT) == TIM_IT_Trigger) || \
                           ((IT) == TIM_IT_Break))

/*
 * @defgroup  TIM Event sources 
 */
#define TIM_EventSource_Update             ((uint32_t)0x00000001)
#define TIM_EventSource_CC1                ((uint32_t)0x00010000)
#define TIM_EventSource_CC2                ((uint32_t)0x00020000)
#define TIM_EventSource_CC3                ((uint32_t)0x00040000)
#define TIM_EventSource_CC4                ((uint32_t)0x00080000)
#define TIM_EventSource_CC5                ((uint32_t)0x00100000)
#define TIM_EventSource_CC6                ((uint32_t)0x00200000)
#define TIM_EventSource_COM                ((uint32_t)0x00000020)
#define TIM_EventSource_Trigger            ((uint32_t)0x00000040)
#define TIM_EventSource_Break              ((uint32_t)0x00000080)
#define IS_TIM_EVENT_SOURCE(SOURCE) ((((SOURCE) & (uint32_t)0xC0C0FEF2) == 0x00000000) && ((SOURCE) != 0x00000000)) 

/*
 * @defgroup  TIM Flags
 */
#define TIM_FLAG_Update                    ((uint32_t)0x00000001)
#define TIM_FLAG_CC1                       ((uint32_t)0x00010000)
#define TIM_FLAG_CC2                       ((uint32_t)0x00020000)
#define TIM_FLAG_CC3                       ((uint32_t)0x00040000)
#define TIM_FLAG_CC4                       ((uint32_t)0x00080000)
#define TIM_FLAG_CC5                       ((uint32_t)0x00100000)
#define TIM_FLAG_CC6                       ((uint32_t)0x00200000)
#define TIM_FLAG_COM                       ((uint32_t)0x00000020)
#define TIM_FLAG_Trigger                   ((uint32_t)0x00000040)
#define TIM_FLAG_Break                     ((uint32_t)0x00000080)
#define TIM_FLAG_CC1OF                     ((uint32_t)0x01000000)
#define TIM_FLAG_CC2OF                     ((uint32_t)0x02000000)
#define TIM_FLAG_CC3OF                     ((uint32_t)0x04000000)
#define TIM_FLAG_CC4OF                     ((uint32_t)0x08000000)
#define TIM_FLAG_CC5OF                     ((uint32_t)0x10000000)
#define TIM_FLAG_CC6OF                     ((uint32_t)0x20000000)
#define IS_TIM_GET_FLAG(FLAG) (((FLAG) == TIM_FLAG_Update) || \
                               ((FLAG) == TIM_FLAG_CC1) || \
                               ((FLAG) == TIM_FLAG_CC2) || \
                               ((FLAG) == TIM_FLAG_CC3) || \
                               ((FLAG) == TIM_FLAG_CC4) || \
                               ((FLAG) == TIM_FLAG_CC5) || \
                               ((FLAG) == TIM_FLAG_CC6) || \
                               ((FLAG) == TIM_FLAG_COM) || \
                               ((FLAG) == TIM_FLAG_Trigger) || \
                               ((FLAG) == TIM_FLAG_Break) || \
                               ((FLAG) == TIM_FLAG_CC1OF) || \
                               ((FLAG) == TIM_FLAG_CC2OF) || \
                               ((FLAG) == TIM_FLAG_CC3OF) || \
                               ((FLAG) == TIM_FLAG_CC4OF) || \
                               ((FLAG) == TIM_FLAG_CC5OF) || \
                               ((FLAG) == TIM_FLAG_CC6OF))

/*
 * @defgroup  TIM DMA Base address
 */
#define TIM_DMABase_CR1                    ((uint32_t)0x00000000)
#define TIM_DMABase_CR2                    ((uint32_t)0x00000001)
#define TIM_DMABase_SMCR                   ((uint32_t)0x00000002)
#define TIM_DMABase_DIER                   ((uint32_t)0x00000003)
#define TIM_DMABase_SR                     ((uint32_t)0x00000004)
#define TIM_DMABase_EGR                    ((uint32_t)0x00000005)
#define TIM_DMABase_CCMR1                  ((uint32_t)0x00000006)
#define TIM_DMABase_CCMR2                  ((uint32_t)0x00000007)
#define TIM_DMABase_CCMR3                  ((uint32_t)0x00000008)
#define TIM_DMABase_CCER                   ((uint32_t)0x00000009)
#define TIM_DMABase_CNT                    ((uint32_t)0x0000000A)
#define TIM_DMABase_PSC                    ((uint32_t)0x0000000B)
#define TIM_DMABase_ARR                    ((uint32_t)0x0000000B)
#define TIM_DMABase_RCR                    ((uint32_t)0x0000000D)
#define TIM_DMABase_CCR1                   ((uint32_t)0x0000000E)
#define TIM_DMABase_CCR2                   ((uint32_t)0x0000000F)
#define TIM_DMABase_CCR3                   ((uint32_t)0x00000010)
#define TIM_DMABase_CCR4                   ((uint32_t)0x00000011)
#define TIM_DMABase_CCR5                   ((uint32_t)0x00000012)
#define TIM_DMABase_CCR6                   ((uint32_t)0x00000013)
#define TIM_DMABase_BDTR                   ((uint32_t)0x00000014)
#define TIM_DMABase_DCR                    ((uint32_t)0x00000015)
#define TIM_DMABase_DMAR                   ((uint32_t)0x00000016)
#define TIM_DMABase_AF1                    ((uint32_t)0x00000017)
#define IS_TIM_DMA_BASE(BASE) (((BASE) == TIM_DMABase_CR1) || \
                               ((BASE) == TIM_DMABase_CR2) || \
                               ((BASE) == TIM_DMABase_SMCR) || \
                               ((BASE) == TIM_DMABase_DIER) || \
                               ((BASE) == TIM_DMABase_SR) || \
                               ((BASE) == TIM_DMABase_EGR) || \
                               ((BASE) == TIM_DMABase_CCMR1) || \
                               ((BASE) == TIM_DMABase_CCMR2) || \
                               ((BASE) == TIM_DMABase_CCMR3) || \
                               ((BASE) == TIM_DMABase_CCER) || \
                               ((BASE) == TIM_DMABase_CNT) || \
                               ((BASE) == TIM_DMABase_PSC) || \
                               ((BASE) == TIM_DMABase_ARR) || \
                               ((BASE) == TIM_DMABase_RCR) || \
                               ((BASE) == TIM_DMABase_CCR1) || \
                               ((BASE) == TIM_DMABase_CCR2) || \
                               ((BASE) == TIM_DMABase_CCR3) || \
                               ((BASE) == TIM_DMABase_CCR4) || \
                               ((BASE) == TIM_DMABase_CCR5) || \
                               ((BASE) == TIM_DMABase_CCR6) || \
                               ((BASE) == TIM_DMABase_BDTR) || \
                               ((BASE) == TIM_DMABase_DCR) || \
                               ((BASE) == TIM_DMABase_DMAR)||\
                               ((BASE) == TIM_DMABase_AF1))     

/*
 * @defgroup  TIM DMA Burst Length
 */
#define TIM_DMABurstLength_1Transfer           ((uint32_t)0x00000000)
#define TIM_DMABurstLength_2Transfers          ((uint32_t)0x00000100)
#define TIM_DMABurstLength_3Transfers          ((uint32_t)0x00000200)
#define TIM_DMABurstLength_4Transfers          ((uint32_t)0x00000300)
#define TIM_DMABurstLength_5Transfers          ((uint32_t)0x00000400)
#define TIM_DMABurstLength_6Transfers          ((uint32_t)0x00000500)
#define TIM_DMABurstLength_7Transfers          ((uint32_t)0x00000600)
#define TIM_DMABurstLength_8Transfers          ((uint32_t)0x00000700)
#define TIM_DMABurstLength_9Transfers          ((uint32_t)0x00000800)
#define TIM_DMABurstLength_10Transfers         ((uint32_t)0x00000900)
#define TIM_DMABurstLength_11Transfers         ((uint32_t)0x00000A00)
#define TIM_DMABurstLength_12Transfers         ((uint32_t)0x00000B00)
#define TIM_DMABurstLength_13Transfers         ((uint32_t)0x00000C00)
#define TIM_DMABurstLength_14Transfers         ((uint32_t)0x00000D00)
#define TIM_DMABurstLength_15Transfers         ((uint32_t)0x00000E00)
#define TIM_DMABurstLength_16Transfers         ((uint32_t)0x00000F00)
#define TIM_DMABurstLength_17Transfers         ((uint32_t)0x00001000)
#define TIM_DMABurstLength_18Transfers         ((uint32_t)0x00001100)
#define IS_TIM_DMA_LENGTH(LENGTH) (((LENGTH) == TIM_DMABurstLength_1Transfer) || \
                                   ((LENGTH) == TIM_DMABurstLength_2Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_3Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_4Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_5Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_6Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_7Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_8Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_9Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_10Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_11Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_12Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_13Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_14Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_15Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_16Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_17Transfers) || \
                                   ((LENGTH) == TIM_DMABurstLength_18Transfers))

/*
 * @defgroup  TIM DMA sources
 */
#define TIM_DMA_Update                     ((uint32_t)0x00000100)
#define TIM_DMA_CC1                        ((uint32_t)0x01000000)
#define TIM_DMA_CC2                        ((uint32_t)0x02000000)
#define TIM_DMA_CC3                        ((uint32_t)0x04000000)
#define TIM_DMA_CC4                        ((uint32_t)0x08000000)
#define TIM_DMA_CC5                        ((uint32_t)0x10000000)
#define TIM_DMA_CC6                        ((uint32_t)0x20000000)
#define IS_TIM_DMA_SOURCE(SOURCE) ((((SOURCE) & (uint32_t)0xC0FFEFF) == 0x00000000) && ((SOURCE) != 0x00000000))

/*
 * @defgroup  TIM Internal Trigger Selection
 */
#define TIM_TS_ITR0                        ((uint32_t)0x00000000)
#define TIM_TS_ITR1                        ((uint32_t)0x00000010)
#define TIM_TS_ITR2                        ((uint32_t)0x00000020)
#define TIM_TS_ITR3                        ((uint32_t)0x00000030)
#define TIM_TS_TI1F_ED                     ((uint32_t)0x00000040)
#define TIM_TS_TI1FP1                      ((uint32_t)0x00000050)
#define TIM_TS_TI2FP2                      ((uint32_t)0x00000060)
#define TIM_TS_ETRF                        ((uint32_t)0x00000070)
#define IS_TIM_TRIGGER_SELECTION(SELECTION) (((SELECTION) == TIM_TS_ITR0) || \
                                             ((SELECTION) == TIM_TS_ITR1) || \
                                             ((SELECTION) == TIM_TS_ITR2) || \
                                             ((SELECTION) == TIM_TS_ITR3) || \
                                             ((SELECTION) == TIM_TS_TI1F_ED) || \
                                             ((SELECTION) == TIM_TS_TI1FP1) || \
                                             ((SELECTION) == TIM_TS_TI2FP2) || \
                                             ((SELECTION) == TIM_TS_ETRF))
#define IS_TIM_INTERNAL_TRIGGER_SELECTION(SELECTION) (((SELECTION) == TIM_TS_ITR0) || \
                                                      ((SELECTION) == TIM_TS_ITR1) || \
                                                      ((SELECTION) == TIM_TS_ITR2) || \
                                                      ((SELECTION) == TIM_TS_ITR3))

/*
 * @defgroup  TIM Slave Mode
 */
#define TIM_SlaveMode_Reset                ((uint32_t)0x00000004)
#define TIM_SlaveMode_Gated                ((uint32_t)0x00000005)
#define TIM_SlaveMode_Trigger              ((uint32_t)0x00000006)
#define TIM_SlaveMode_External1            ((uint32_t)0x00000007)
#define TIM_SlaveMode_Reset_Trigger        ((uint32_t)0x00000008)
#define IS_TIM_SLAVE_MODE(MODE) (((MODE) == TIM_SlaveMode_Reset) || \
                                 ((MODE) == TIM_SlaveMode_Gated) || \
                                 ((MODE) == TIM_SlaveMode_Trigger) || \
                                 ((MODE) == TIM_SlaveMode_External1)||\
                                 ((MODE) == TIM_SlaveMode_Reset_Trigger))

/*
 * @defgroup  TIM TIx External Clock Source
 */
#define TIM_TIxExternalCLK1Source_TI1      ((uint32_t)0x00000050)
#define TIM_TIxExternalCLK1Source_TI2      ((uint32_t)0x00000060)
#define TIM_TIxExternalCLK1Source_TI1ED    ((uint32_t)0x00000040)

/*
 * @defgroup  TIM External Trigger Prescaler
 */
#define TIM_ExtTRGPSC_OFF                  ((uint32_t)0x00000000)
#define TIM_ExtTRGPSC_DIV2                 ((uint32_t)0x00001000)
#define TIM_ExtTRGPSC_DIV4                 ((uint32_t)0x00002000)
#define TIM_ExtTRGPSC_DIV8                 ((uint32_t)0x00003000)
#define IS_TIM_EXT_PRESCALER(PRESCALER) (((PRESCALER) == TIM_ExtTRGPSC_OFF) || \
                                         ((PRESCALER) == TIM_ExtTRGPSC_DIV2) || \
                                         ((PRESCALER) == TIM_ExtTRGPSC_DIV4) || \
                                         ((PRESCALER) == TIM_ExtTRGPSC_DIV8))

/*
 * @defgroup  TIM External Trigger Polarity
 */
#define TIM_ExtTRGPolarity_Inverted        ((uint32_t)0x00008000)
#define TIM_ExtTRGPolarity_NonInverted     ((uint32_t)0x00000000)
#define IS_TIM_EXT_POLARITY(POLARITY) (((POLARITY) == TIM_ExtTRGPolarity_Inverted) || \
                                       ((POLARITY) == TIM_ExtTRGPolarity_NonInverted))

/*
 * @defgroup  TIM Trigger Output Source
 */
#define TIM_TRGOSource_Reset                       ((uint32_t)0x00000000)
#define TIM_TRGOSource_Enable                      ((uint32_t)0x00000010)
#define TIM_TRGOSource_Update                      ((uint32_t)0x00000020)
#define TIM_TRGOSource_OC1                         ((uint32_t)0x00000030)
#define TIM_TRGOSource_OC1Ref                      ((uint32_t)0x00000040)
#define TIM_TRGOSource_OC2Ref                      ((uint32_t)0x00000050)
#define TIM_TRGOSource_OC3Ref                      ((uint32_t)0x00000060)
#define TIM_TRGOSource_OC4Ref                      ((uint32_t)0x00000070)
#define TIM_TRGO2Source_Reset                      ((uint32_t)0x00000000)
#define TIM_TRGO2Source_Enable                     ((uint32_t)0x00100000)
#define TIM_TRGO2Source_Update                     ((uint32_t)0x00200000)
#define TIM_TRGO2Source_OC1                        ((uint32_t)0x00300000)
#define TIM_TRGO2Source_OC1Ref                     ((uint32_t)0x00400000)
#define TIM_TRGO2Source_OC2Ref                     ((uint32_t)0x00500000)
#define TIM_TRGO2Source_OC3Ref                     ((uint32_t)0x00600000)
#define TIM_TRGO2Source_OC4Ref                     ((uint32_t)0x00700000)
#define TIM_TRGO2Source_OC5Ref                     ((uint32_t)0x00800000)
#define TIM_TRGO2Source_OC6Ref                     ((uint32_t)0x00900000)
#define TIM_TRGO2Source_OC4Ref_BothEdge            ((uint32_t)0x00A00000)
#define TIM_TRGO2Source_OC6Ref_BothEdge            ((uint32_t)0x00B00000)
#define TIM_TRGO2Source_OC46Ref_Rising             ((uint32_t)0x00C00000)
#define TIM_TRGO2Source_OC4_Rising_OR_OC6_Falling  ((uint32_t)0x00D00000)
#define TIM_TRGO2Source_OC5_Rising_OR_OC6_Rising   ((uint32_t)0x00E00000)
#define TIM_TRGO2Source_OC5_Rising_OR_OC6_Falling  ((uint32_t)0x00F00000)

#define IS_TIM_TRGO_SOURCE(SOURCE) (((SOURCE) == TIM_TRGOSource_Reset) || \
                                    ((SOURCE) == TIM_TRGOSource_Enable) || \
                                    ((SOURCE) == TIM_TRGOSource_Update) || \
                                    ((SOURCE) == TIM_TRGOSource_OC1) || \
                                    ((SOURCE) == TIM_TRGOSource_OC1Ref) || \
                                    ((SOURCE) == TIM_TRGOSource_OC2Ref) || \
                                    ((SOURCE) == TIM_TRGOSource_OC3Ref) || \
                                    ((SOURCE) == TIM_TRGOSource_OC4Ref) || \
                                     ((SOURCE) == TIM_TRGO2Source_Reset) || \
                                     ((SOURCE) == TIM_TRGO2Source_Enable) || \
                                     ((SOURCE) == TIM_TRGO2Source_Update) || \
                                     ((SOURCE) == TIM_TRGO2Source_OC1) || \
                                     ((SOURCE) == TIM_TRGO2Source_OC1Ref) || \
                                     ((SOURCE) == TIM_TRGO2Source_OC2Ref) || \
                                     ((SOURCE) == TIM_TRGO2Source_OC3Ref) || \
                                     ((SOURCE) == TIM_TRGO2Source_OC4Ref) || \
                                     ((SOURCE) == TIM_TRGO2Source_OC5Ref) || \
                                     ((SOURCE) == TIM_TRGO2Source_OC6Ref) || \
                                     ((SOURCE) == TIM_TRGO2Source_OC4Ref_BothEdge) || \
                                     ((SOURCE) == TIM_TRGO2Source_OC6Ref_BothEdge) || \
                                     ((SOURCE) == TIM_TRGO2Source_OC46Ref_Rising) || \
                                     ((SOURCE) == TIM_TRGO2Source_OC4_Rising_OR_OC6_Falling) || \
                                     ((SOURCE) == TIM_TRGO2Source_OC5_Rising_OR_OC6_Rising) || \
                                     ((SOURCE) == TIM_TRGO2Source_OC5_Rising_OR_OC6_Falling))

/*
 * @defgroup  TIM Encoder Mode
 */
#define TIM_EncoderMode_TI1                ((uint32_t)0x00000001)
#define TIM_EncoderMode_TI2                ((uint32_t)0x00000002)
#define TIM_EncoderMode_TI12               ((uint32_t)0x00000003)
#define IS_TIM_ENCODER_MODE(MODE) (((MODE) == TIM_EncoderMode_TI1) || \
                                   ((MODE) == TIM_EncoderMode_TI2) || \
                                   ((MODE) == TIM_EncoderMode_TI12))

/*
 * @defgroup  TIM External Trigger Selection
 */
#define TIM_TS_ETR0                        ((uint32_t)0x00000000)
#define TIM_TS_ETR1                        ((uint32_t)0x00000001)
#define IS_TIM_EXTERNAL_TRIGGER_SELECTION(SELECTION) (((SELECTION) == TIM_TS_ETR0) || \
                                                        ((SELECTION) == TIM_TS_ETR1))

/*
 * @defgroup  TIM Update Source
 */
#define TIM_UpdateSource_Global            ((uint32_t)0x00000000) /*!< Source of update is the counter overflow/underflow
                                                                   or the setting of UG bit, or an update generation
                                                                   through the slave mode controller. */
#define TIM_UpdateSource_Regular           ((uint32_t)0x00000001) /*!< Source of update is counter overflow/underflow. */
#define IS_TIM_UPDATE_SOURCE(SOURCE) (((SOURCE) == TIM_UpdateSource_Global) || \
                                      ((SOURCE) == TIM_UpdateSource_Regular))

/*
 * @defgroup  TIM Output Compare Preload State
 */
#define TIM_OCPreload_Enable               ((uint32_t)0x00000008)
#define TIM_OCPreload_Disable              ((uint32_t)0x00000000)
#define IS_TIM_OCPRELOAD_STATE(STATE) (((STATE) == TIM_OCPreload_Enable) || \
                                       ((STATE) == TIM_OCPreload_Disable))

/* TimeBase management ********************************************************/
void TIM_DeInit(TIM_TypeDef* TIMx);
void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);
void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint32_t Prescaler, uint32_t TIM_PSCReloadMode);
void TIM_CounterModeConfig(TIM_TypeDef* TIMx, uint32_t TIM_CounterMode);
void TIM_SetCounter(TIM_TypeDef* TIMx, uint32_t Counter);
void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint32_t Autoreload);
uint32_t TIM_GetCounter(TIM_TypeDef* TIMx);
uint32_t TIM_GetPrescaler(TIM_TypeDef* TIMx);
void TIM_UpdateDisableConfig(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_UpdateRequestConfig(TIM_TypeDef* TIMx, uint32_t TIM_UpdateSource);
void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_SelectOnePulseMode(TIM_TypeDef* TIMx, uint32_t TIM_OPMode);
void TIM_SetClockDivision(TIM_TypeDef* TIMx, uint32_t TIM_CKD);
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState);

/* Output Compare management **************************************************/
void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC2Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC3Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC4Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC5Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC6Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OCStructInit(TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_SelectOCxM(TIM_TypeDef* TIMx, uint32_t TIM_Channel, uint32_t TIM_OCMode);
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint32_t Compare1);
void TIM_SetCompare2(TIM_TypeDef* TIMx, uint32_t Compare2);
void TIM_SetCompare3(TIM_TypeDef* TIMx, uint32_t Compare3);
void TIM_SetCompare4(TIM_TypeDef* TIMx, uint32_t Compare4);
void TIM_SetCompare5(TIM_TypeDef* TIMx, uint32_t Compare3);
void TIM_SetCompare6(TIM_TypeDef* TIMx, uint32_t Compare4);
void TIM_ForcedOC1Config(TIM_TypeDef* TIMx, uint32_t TIM_ForcedAction);
void TIM_ForcedOC2Config(TIM_TypeDef* TIMx, uint32_t TIM_ForcedAction);
void TIM_ForcedOC3Config(TIM_TypeDef* TIMx, uint32_t TIM_ForcedAction);
void TIM_ForcedOC4Config(TIM_TypeDef* TIMx, uint32_t TIM_ForcedAction);
void TIM_ForcedOC5Config(TIM_TypeDef* TIMx, uint32_t TIM_ForcedAction);
void TIM_ForcedOC6Config(TIM_TypeDef* TIMx, uint32_t TIM_ForcedAction);
void TIM_OC1PreloadConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPreload);
void TIM_OC2PreloadConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPreload);
void TIM_OC3PreloadConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPreload);
void TIM_OC4PreloadConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPreload);
void TIM_OC5PreloadConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPreload);
void TIM_OC6PreloadConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPreload);
void TIM_OC1FastConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCFast);
void TIM_OC2FastConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCFast);
void TIM_OC3FastConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCFast);
void TIM_OC4FastConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCFast);
void TIM_ClearOC1Ref(TIM_TypeDef* TIMx, uint32_t TIM_OCClear);
void TIM_ClearOC2Ref(TIM_TypeDef* TIMx, uint32_t TIM_OCClear);
void TIM_ClearOC3Ref(TIM_TypeDef* TIMx, uint32_t TIM_OCClear);
void TIM_ClearOC4Ref(TIM_TypeDef* TIMx, uint32_t TIM_OCClear);
void TIM_OC1PolarityConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPolarity);
void TIM_OC1NPolarityConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCNPolarity);
void TIM_OC2PolarityConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPolarity);
void TIM_OC2NPolarityConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCNPolarity);
void TIM_OC3PolarityConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPolarity);
void TIM_OC3NPolarityConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCNPolarity);
void TIM_OC4PolarityConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPolarity);
void TIM_CCxCmd(TIM_TypeDef* TIMx, uint32_t TIM_Channel, uint32_t TIM_CCx);
void TIM_CCxNCmd(TIM_TypeDef* TIMx, uint32_t TIM_Channel, uint32_t TIM_CCxN);

/* Input Capture management ***************************************************/
void TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct);
void TIM_ICStructInit(TIM_ICInitTypeDef* TIM_ICInitStruct);
void TIM_PWMIConfig(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct);
uint32_t TIM_GetCapture1(TIM_TypeDef* TIMx);
uint32_t TIM_GetCapture2(TIM_TypeDef* TIMx);
uint32_t TIM_GetCapture3(TIM_TypeDef* TIMx);
uint32_t TIM_GetCapture4(TIM_TypeDef* TIMx);
uint32_t TIM_GetCapture5(TIM_TypeDef* TIMx);
uint32_t TIM_GetCapture6(TIM_TypeDef* TIMx);
void TIM_SetIC1Prescaler(TIM_TypeDef* TIMx, uint32_t TIM_ICPSC);
void TIM_SetIC2Prescaler(TIM_TypeDef* TIMx, uint32_t TIM_ICPSC);
void TIM_SetIC3Prescaler(TIM_TypeDef* TIMx, uint32_t TIM_ICPSC);
void TIM_SetIC4Prescaler(TIM_TypeDef* TIMx, uint32_t TIM_ICPSC);
void TIM_SetIC5Prescaler(TIM_TypeDef* TIMx, uint32_t TIM_ICPSC);
void TIM_SetIC6Prescaler(TIM_TypeDef* TIMx, uint32_t TIM_ICPSC);

/* Advanced-control timers (TIM0,TIM1,TIM2,TIM5) specific features ******************/
void TIM_BDTRConfig(TIM_TypeDef* TIMx, TIM_BDTRInitTypeDef *TIM_BDTRInitStruct);
void TIM_BDTRStructInit(TIM_BDTRInitTypeDef* TIM_BDTRInitStruct);
void TIM_CtrlPWMOutputs(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_SelectCOM(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_CCPreloadControl(TIM_TypeDef* TIMx, FunctionalState NewState);

/* Interrupts, DMA and flags management ***************************************/
void TIM_ITConfig(TIM_TypeDef* TIMx, uint32_t TIM_IT, FunctionalState NewState);
void TIM_GenerateEvent(TIM_TypeDef* TIMx, uint32_t TIM_EventSource);
FlagStatus TIM_GetFlagStatus(TIM_TypeDef* TIMx, uint32_t TIM_FLAG);
void TIM_ClearFlag(TIM_TypeDef* TIMx, uint32_t TIM_FLAG);
ITStatus TIM_GetITStatus(TIM_TypeDef* TIMx, uint32_t TIM_IT);
void TIM_ClearITPendingBit(TIM_TypeDef* TIMx, uint32_t TIM_IT);
void TIM_DMAConfig(TIM_TypeDef* TIMx, uint32_t TIM_DMABase, uint32_t TIM_DMABurstLength);
void TIM_DMACmd(TIM_TypeDef* TIMx, uint32_t TIM_DMASource, FunctionalState NewState);
void TIM_SelectCCDMA(TIM_TypeDef* TIMx, FunctionalState NewState);

/* Clocks management **********************************************************/
void TIM_InternalClockConfig(TIM_TypeDef* TIMx);
void TIM_ITRxExternalClockConfig(TIM_TypeDef* TIMx, uint32_t TIM_InputTriggerSource);
void TIM_TIxExternalClockConfig(TIM_TypeDef* TIMx, uint32_t TIM_TIxExternalCLKSource,
                                uint32_t TIM_ICPolarity, uint32_t ICFilter);
void TIM_ETRClockMode1Config(TIM_TypeDef* TIMx, uint32_t TIM_ExtTRGPrescaler, uint32_t TIM_ExtTRGPolarity,
                             uint32_t ExtTRGFilter);
void TIM_ETRClockMode2Config(TIM_TypeDef* TIMx, uint32_t TIM_ExtTRGPrescaler, 
                             uint32_t TIM_ExtTRGPolarity, uint32_t ExtTRGFilter);

/* Synchronization management *************************************************/
void TIM_SelectInputTrigger(TIM_TypeDef* TIMx, uint32_t TIM_InputTriggerSource);
void TIM_SelectOutputTrigger(TIM_TypeDef* TIMx, uint32_t TIM_TRGOSource);
void TIM_SelectSlaveMode(TIM_TypeDef* TIMx, uint32_t TIM_SlaveMode);
void TIM_ETRConfig(TIM_TypeDef* TIMx, uint32_t TIM_ExtTRGPrescaler, uint32_t TIM_ExtTRGPolarity,
                   uint32_t ExtTRGFilter);
void TIM_SelectInputETR(TIM_TypeDef* TIMx,uint32_t TIM_InputETRSource);

/* Specific interface management **********************************************/   
void TIM_EncoderInterfaceConfig(TIM_TypeDef* TIMx, uint32_t TIM_EncoderMode,
                                uint32_t TIM_IC1Polarity, uint32_t TIM_IC2Polarity);
void TIM_SelectHallSensor(TIM_TypeDef* TIMx, FunctionalState NewState);


#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
