/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_adc.c
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:			2024/8/30
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/8/30     1.0     build this module
 * Description:     This file provides firmware functions to manage the functionalities of the ADC peripheral.
 */

#include "as32x601_adc.h"
#include <stddef.h>

/* Private function prototypes -----------------------------------------------*/
static void ADC_InitDelayus(uint32_t Delayus);

/*
 * Function:        ADC_GetAverageValue
 * Description:     Get average value of ADC convert result from assigned sequence.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  Seq: ADC group seqeunce index.
 *                   This parameter can be one of the ADC_RSEQ_x where x can be (0..15).
 *                   This parameter can be one of the ADC_ISEQ_x where x can be (0..3).
 *                  Count: average count.
 * Return:          Averaged ADC result.
 */
float ADC_GetAverageValue(ADC_TypeDef *ADCx, const uint32_t Seq, const uint32_t Count)
{
    float sum = 0;
    float result = 0;
    uint32_t i = 0x00;
    uint16_t value = 0x00;

    /* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(Count > 0U);
    assert_param(IS_ADC_SEQUENCE(Seq));

    for (i = 0; i < Count; i++)
    {
		ADCx->CTRL0 |= (uint32_t)((ADC_CTRL0_SWSTART));

        /* Wait for convertion complete */
        while (!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC, Seq))
        {
            ;
        }

        ADC_GetSeqResult(ADCx, Seq, &value);
        sum += value;
    }

    result = (sum / Count);
    
    return result;
}

/*
 * Function:        ADC_StructInit
 * Description:     Initialization convert structure.
 * Param:           ADC_InitStruct: convert config structure pointer.
 * Return:          None.
 */
void ADC_StructInit(ADC_InitTypeDef * const ADC_InitStruct)
{
    /* Reset ADC init structure parameters values */
    ADC_InitStruct->ADC_ClockDivide                = ADC_CLK_DIVIDE_1;
    ADC_InitStruct->ADC_Resolution                 = ADC_RESOLUTION_12BIT;
    ADC_InitStruct->ADC_Alignment                  = ADC_DATA_ALIGN_RIGHT;
    ADC_InitStruct->ADC_Symbol                     = ADC_DATA_SELDO_UNSIGNED;

    ADC_InitStruct->ADC_RegularTrigger             = ADC_TRIGGER_INTERNAL;
    ADC_InitStruct->ADC_InjectTrigger              = ADC_TRIGGER_INTERNAL;

    ADC_InitStruct->ADC_RegularEnable              = DISABLE;
    ADC_InitStruct->ADC_InjectEnable               = DISABLE;

    ADC_InitStruct->ADC_DmaEnable                  = DISABLE;

    ADC_InitStruct->ADC_TtrimRef                   = ADC_TTRIM_7;
    ADC_InitStruct->ADC_VtrimRef                   = ADC_VTRIM_14;
    ADC_InitStruct->ADC_SelvRefbiRef               = ADC_SEL_VREFBI_2V5;
    ADC_InitStruct->ADC_VrefbiEnable               = DISABLE;
    ADC_InitStruct->ADC_SelvihdlsRef               = ADC_SAR_INPUT_SINGLE_ENDED;

    /* Conversion mode */
    ADC_InitStruct->ADC_ScanModeEn                 = DISABLE;
    ADC_InitStruct->ADC_ContinuousModeEn           = DISABLE;
    ADC_InitStruct->ADC_RegularDiscontinuousModeEn = DISABLE;
    ADC_InitStruct->ADC_InjectDiscontinuousModeEn  = DISABLE;
    ADC_InitStruct->ADC_InjectAutoModeEn           = DISABLE;
    ADC_InitStruct->ADC_IntervalModeEn             = DISABLE;
    ADC_InitStruct->ADC_RegularDiscontinuousNum    = 0x00;

    ADC_InitStruct->ADC_RegularSequenceLength      = 0x00;
    ADC_InitStruct->ADC_InjectSequenceLength       = 0x00;

    ADC_InitStruct->ADC_PowerEn                    = DISABLE;
    ADC_InitStruct->ADC_PowerRegain                = DISABLE;
    ADC_InitStruct->ADC_BufEn                      = DISABLE;
}

/*
 * Function:        ADC_Init
 * Description:     Config ADC convert setting.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  ADC_InitStruct: convert config that need to config.
 * Return:          None.
 */
void ADC_Init(ADC_TypeDef *ADCx, const ADC_InitTypeDef * const ADC_InitStruct)
{
    uint32_t frequence = ADC_CLOCK_FREQ_MAX_RUNTIME;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(ADC_InitStruct != NULL);
    assert_param(IS_ADC_CLK_DIVIDE(ADC_InitStruct->ADC_ClockDivide));
    assert_param(IS_ADC_BitAction(ADC_InitStruct->ADC_DmaEnable));
    assert_param(IS_ADC_RESOLUTION(ADC_InitStruct->ADC_Resolution));
    assert_param(IS_ADC_SELDO(ADC_InitStruct->ADC_Symbol));
    assert_param(IS_ADC_ALIGN(ADC_InitStruct->ADC_Alignment));
    assert_param(IS_ADC_SELVIHDLS(ADC_InitStruct->ADC_SelvihdlsRef));
    assert_param(IS_ADC_BitAction(ADC_InitStruct->ADC_VrefbiEnable));
    assert_param(IS_ADC_TTRIM(ADC_InitStruct->ADC_TtrimRef));
    assert_param(IS_ADC_VTRIM(ADC_InitStruct->ADC_VtrimRef));
    assert_param(IS_ADC_SELVREFBI(ADC_InitStruct->ADC_SelvRefbiRef));
    assert_param(IS_ADC_BitAction(ADC_InitStruct->ADC_ScanModeEn));
    assert_param(IS_ADC_BitAction(ADC_InitStruct->ADC_ContinuousModeEn));
    assert_param(IS_ADC_BitAction(ADC_InitStruct->ADC_RegularDiscontinuousModeEn));
    assert_param(IS_ADC_BitAction(ADC_InitStruct->ADC_InjectDiscontinuousModeEn));
    assert_param(IS_ADC_BitAction(ADC_InitStruct->ADC_InjectAutoModeEn));
    assert_param(IS_ADC_BitAction(ADC_InitStruct->ADC_IntervalModeEn));
    assert_param(ADC_REGULAR_DISCNUM >= ADC_InitStruct->ADC_RegularDiscontinuousNum);
    assert_param(IS_ADC_TRIGGER(ADC_InitStruct->ADC_RegularTrigger));
    assert_param(IS_ADC_TRIGGER(ADC_InitStruct->ADC_InjectTrigger));
    assert_param(IS_ADC_BitAction(ADC_InitStruct->ADC_RegularEnable));
    assert_param(IS_ADC_BitAction(ADC_InitStruct->ADC_InjectEnable));
    assert_param(ADC_REGULAR_SEQUENCE_LENGTH >= ADC_InitStruct->ADC_RegularSequenceLength);
    assert_param(ADC_INJECT_SEQUENCE_LENGTH >= ADC_InitStruct->ADC_InjectSequenceLength);
    assert_param(IS_ADC_BitAction(ADC_InitStruct->ADC_PowerEn));
    assert_param(IS_ADC_BitAction(ADC_InitStruct->ADC_BufEn));
    assert_param(IS_ADC_BitAction(ADC_InitStruct->ADC_PowerRegain));

    if(ADC_InitStruct->ADC_ClockDivide == ADC_CLK_DIVIDE_1)
    {
        frequence = frequence;
    }
    else
    {
        frequence = frequence / ((uint32_t)ADC_InitStruct->ADC_ClockDivide * 2);
    }
    assert_param(frequence <= ADC_CLOCK_FREQ_MAX_RUNTIME);
    UNUSED(frequence);

    /* Set clock divide */
    ADCx->CTRL1 &= ~((uint32_t)ADC_CTRL1_PSC);
    ADCx->CTRL1 |= (((uint32_t)ADC_InitStruct->ADC_ClockDivide) << ADC_CTRL1_PSC_Pos);

    /* Set DMA enable flag */
    ADCx->CTRL0 &= ~((uint32_t)ADC_CTRL0_DMAEN);
    ADCx->CTRL0 |= (((uint32_t)ADC_InitStruct->ADC_DmaEnable) << ADC_CTRL0_DMAEN_Pos);

    /* Set resolution */
    ADCx->CTRL1 &= ~((uint32_t)ADC_CTRL1_RESL);
    ADCx->CTRL1 |= (((uint32_t)ADC_InitStruct->ADC_Resolution) << ADC_CTRL1_RESL_Pos);

    /* Set symbols */
    ADCx->CTRL0 &= ~((uint32_t)ADC_CTRL0_SELDO);
    ADCx->CTRL0 |= (((uint32_t)ADC_InitStruct->ADC_Symbol) << ADC_CTRL0_SELDO_Pos);

    /* Set alignment */
    ADCx->CTRL0 &= ~((uint32_t)ADC_CTRL0_ALIGN);
    ADCx->CTRL0 |= (((uint32_t)ADC_InitStruct->ADC_Alignment) << ADC_CTRL0_ALIGN_Pos);

    /* Set SAR input type */
    ADCx->CONFIG &= ~((uint32_t)ADC_CONFIG_SELVI_HD_LS);
    ADCx->CONFIG |= (((uint32_t)ADC_InitStruct->ADC_SelvihdlsRef) << ADC_CONFIG_SELVI_HD_LS_Pos);

    /* Set Built-in reference voltage */
    ADCx->CONFIG &= ~((uint32_t)ADC_CONFIG_EN_VREFBI);
    if(ADC_InitStruct->ADC_VrefbiEnable == ENABLE)
    {
        ADCx->CONFIG &= ~((uint32_t)ADC_CONFIG_TTRIM);
        ADCx->CONFIG |= (((uint32_t)ADC_InitStruct->ADC_TtrimRef) << ADC_CONFIG_TTRIM_Pos);
        ADCx->CONFIG &= ~((uint32_t)ADC_CONFIG_VTRIM);
        ADCx->CONFIG |= (((uint32_t)ADC_InitStruct->ADC_VtrimRef) << ADC_CONFIG_VTRIM_Pos);
        ADCx->CONFIG &= ~((uint32_t)ADC_CONFIG_SEL_VREFBI);
        ADCx->CONFIG |= (((uint32_t)ADC_InitStruct->ADC_SelvRefbiRef) << ADC_CONFIG_SEL_VREFBI_Pos);
        ADCx->CONFIG |= (((uint32_t)ADC_InitStruct->ADC_VrefbiEnable) << ADC_CONFIG_EN_VREFBI_Pos);
    }
    else
    {
        /* Nothing to do */
    }

    /* Set adc mode */
	ADCx->CTRL0 &= ~((uint32_t)ADC_CTRL0_SCAN);
    ADCx->CTRL0 |= (((uint32_t)ADC_InitStruct->ADC_ScanModeEn) << ADC_CTRL0_SCAN_Pos);
	ADCx->CTRL0 &= ~((uint32_t)ADC_CTRL0_CONT);
    ADCx->CTRL0 |= (((uint32_t)ADC_InitStruct->ADC_ContinuousModeEn) << ADC_CTRL0_CONT_Pos);
	ADCx->CTRL0 &= ~((uint32_t)ADC_CTRL0_DISCEN);
    ADCx->CTRL0 |= (((uint32_t)ADC_InitStruct->ADC_RegularDiscontinuousModeEn) << ADC_CTRL0_DISCEN_Pos);
	ADCx->CTRL0 &= ~((uint32_t)ADC_CTRL0_IDISCEN);
    ADCx->CTRL0 |= (((uint32_t)ADC_InitStruct->ADC_InjectDiscontinuousModeEn) << ADC_CTRL0_IDISCEN_Pos);
	ADCx->CTRL0 &= ~((uint32_t)ADC_CTRL0_IAUTO);
    ADCx->CTRL0 |= (((uint32_t)ADC_InitStruct->ADC_InjectAutoModeEn) << ADC_CTRL0_IAUTO_Pos);
	ADCx->CTRL0 &= ~((uint32_t)ADC_CTRL0_INTERVAL);
    ADCx->CTRL0 |= (((uint32_t)ADC_InitStruct->ADC_IntervalModeEn) << ADC_CTRL0_INTERVAL_Pos);
	ADCx->CTRL0 &= ~((uint32_t)ADC_CTRL0_DISCNUM);
    ADCx->CTRL0 |= (((uint32_t)ADC_InitStruct->ADC_RegularDiscontinuousNum) << ADC_CTRL0_DISCNUM_Pos);

    /* Set Regular/Inject group trigger source */
	ADCx->CTRL0 &= ~((uint32_t)ADC_CTRL0_TRIGSRC);
    ADCx->CTRL0 |= (((uint32_t)ADC_InitStruct->ADC_RegularTrigger) << ADC_CTRL0_TRIGSRC_Pos);
	ADCx->CTRL0 &= ~((uint32_t)ADC_CTRL0_ITRIGSRC);
    ADCx->CTRL0 |= (((uint32_t)ADC_InitStruct->ADC_InjectTrigger) << ADC_CTRL0_ITRIGSRC_Pos);

    /* Set Regular/Inject group trigger source enable */
    ADCx->CTRL0 &= ~((uint32_t)ADC_CTRL0_TRIGEN);
    ADCx->CTRL0 |= (((uint32_t)ADC_InitStruct->ADC_RegularEnable) << ADC_CTRL0_TRIGEN_Pos);
	ADCx->CTRL0 &= ~((uint32_t)ADC_CTRL0_ITRIGEN);
    ADCx->CTRL0 |= (((uint32_t)ADC_InitStruct->ADC_InjectEnable) << ADC_CTRL0_ITRIGEN_Pos);

    /* Set adc sequence length */
	ADCx->SQL &= ~((uint32_t)ADC_SQL_RSQL);
	ADCx->SQL |= (((uint32_t)ADC_InitStruct->ADC_RegularSequenceLength - 1) << ADC_SQL_RSQL_Pos);
	ADCx->SQL &= ~((uint32_t)ADC_SQL_ISQL);
	ADCx->SQL |= (((uint32_t)ADC_InitStruct->ADC_InjectSequenceLength - 1) << ADC_SQL_ISQL_Pos);

    /* Set power */
	ADCx->CTRL1 &= ~((uint32_t)ADC_CTRL1_ADON);
    ADCx->CTRL1 |= (((uint32_t)ADC_InitStruct->ADC_PowerEn) << ADC_CTRL1_ADON_Pos);

    /* Set external reference input buffer */
    ADCx->CONFIG |= (((uint32_t)ADC_InitStruct->ADC_BufEn) << ADC_CONFIG_EN_BUF_Pos);

    /* When ADC_EN=1, if EN_BUF is set from 0 to 1, the start time of buffer is 3us@Typ, 8us@Max */
	if(ADC_InitStruct->ADC_BufEn == (uint32_t)RESET)
	{
		ADC_InitDelayus(2);
	}
	else
	{
		ADC_InitDelayus(8);
	}

    ADCx->CTRL1 |= (((uint32_t)ADC_InitStruct->ADC_PowerRegain) << ADC_CTRL1_RESETN_Pos);
}

/*
 * Function:        ADC_GetConfig
 * Description:     Get ADC converter config from register.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  ADC_InitStruct: config pointer that need to fill with current register config.
 * Return:          None.
 */
void ADC_GetConfig(ADC_TypeDef *ADCx, ADC_InitTypeDef * const ADC_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(ADC_InitStruct != NULL);

    ADC_InitStruct->ADC_ClockDivide                = (uint32_t)((ADCx->CTRL1) & (((uint32_t)(ADC_CTRL1_PSC)) >> ADC_CTRL1_PSC_Pos));
    ADC_InitStruct->ADC_Resolution                 = (uint32_t)((ADCx->CTRL1) & (((uint32_t)(ADC_CTRL1_RESL)) >> ADC_CTRL1_RESL_Pos));
    ADC_InitStruct->ADC_Alignment                  = (uint32_t)((ADCx->CTRL0) & (((uint32_t)(ADC_CTRL0_ALIGN)) >> ADC_CTRL0_ALIGN_Pos));
    ADC_InitStruct->ADC_Symbol                     = (uint32_t)((ADCx->CTRL0) & (((uint32_t)(ADC_CTRL0_SELDO)) >> ADC_CTRL0_SELDO_Pos));

    ADC_InitStruct->ADC_RegularTrigger             = (uint32_t)((ADCx->CTRL0) & (((uint32_t)(ADC_CTRL0_TRIGSRC)) >> ADC_CTRL0_TRIGSRC_Pos));
    ADC_InitStruct->ADC_InjectTrigger              = (uint32_t)((ADCx->CTRL0) & (((uint32_t)(ADC_CTRL0_ITRIGSRC)) >> ADC_CTRL0_ITRIGSRC_Pos));

    ADC_InitStruct->ADC_RegularEnable              = (FunctionalState)((ADCx->CTRL0) & (((uint32_t)(ADC_CTRL0_TRIGEN)) >> ADC_CTRL0_TRIGEN_Pos));
    ADC_InitStruct->ADC_InjectEnable               = (FunctionalState)((ADCx->CTRL0) & (((uint32_t)(ADC_CTRL0_ITRIGEN)) >> ADC_CTRL0_ITRIGEN_Pos));

    ADC_InitStruct->ADC_DmaEnable                  = (FunctionalState)((ADCx->CTRL0) & (((uint32_t)(ADC_CTRL0_DMAEN)) >> ADC_CTRL0_DMAEN_Pos));

    ADC_InitStruct->ADC_TtrimRef                   = (uint32_t)((ADCx->CONFIG) & (((uint32_t)(ADC_CONFIG_TTRIM)) >> ADC_CONFIG_TTRIM_Pos));
    ADC_InitStruct->ADC_VtrimRef                   = (uint32_t)((ADCx->CONFIG) & (((uint32_t)(ADC_CONFIG_VTRIM)) >> ADC_CONFIG_VTRIM_Pos));
    ADC_InitStruct->ADC_SelvRefbiRef               = (uint32_t)((ADCx->CONFIG) & (((uint32_t)(ADC_CONFIG_SEL_VREFBI)) >> ADC_CONFIG_SEL_VREFBI_Pos));
    ADC_InitStruct->ADC_VrefbiEnable               = (FunctionalState)((ADCx->CONFIG) & (((uint32_t)(ADC_CONFIG_EN_VREFBI)) >> ADC_CONFIG_EN_VREFBI_Pos));
    ADC_InitStruct->ADC_SelvihdlsRef               = (uint32_t)((ADCx->CONFIG) & (((uint32_t)(ADC_CONFIG_SELVI_HD_LS)) >> ADC_CONFIG_SELVI_HD_LS_Pos));

    ADC_InitStruct->ADC_ScanModeEn                 = (FunctionalState)((ADCx->CTRL0) & (((uint32_t)(ADC_CTRL0_SCAN)) >> ADC_CTRL0_SCAN_Pos));
    ADC_InitStruct->ADC_ContinuousModeEn           = (FunctionalState)((ADCx->CTRL0) & (((uint32_t)(ADC_CTRL0_CONT)) >> ADC_CTRL0_CONT_Pos));
    ADC_InitStruct->ADC_RegularDiscontinuousModeEn = (FunctionalState)((ADCx->CTRL0) & (((uint32_t)(ADC_CTRL0_DISCEN)) >> ADC_CTRL0_DISCEN_Pos));
    ADC_InitStruct->ADC_InjectDiscontinuousModeEn  = (FunctionalState)((ADCx->CTRL0) & (((uint32_t)(ADC_CTRL0_IDISCEN)) >> ADC_CTRL0_IDISCEN_Pos));
    ADC_InitStruct->ADC_InjectAutoModeEn           = (FunctionalState)((ADCx->CTRL0) & (((uint32_t)(ADC_CTRL0_IAUTO)) >> ADC_CTRL0_IAUTO_Pos));
    ADC_InitStruct->ADC_IntervalModeEn             = (FunctionalState)((ADCx->CTRL0) & (((uint32_t)(ADC_CTRL0_INTERVAL)) >> ADC_CTRL0_INTERVAL_Pos));
    ADC_InitStruct->ADC_RegularDiscontinuousNum    = (uint8_t)((ADCx->CTRL0) & (((uint32_t)(ADC_CTRL0_DISCNUM)) >> ADC_CTRL0_DISCNUM_Pos));

    ADC_InitStruct->ADC_RegularSequenceLength      = (uint8_t)(((ADCx->SQL) & (((uint32_t)(ADC_SQL_RSQL)) >> ADC_SQL_RSQL_Pos)) + 1);
    ADC_InitStruct->ADC_InjectSequenceLength       = (uint8_t)(((ADCx->SQL) & (((uint32_t)(ADC_SQL_ISQL)) >> ADC_SQL_ISQL_Pos)) + 1);

    ADC_InitStruct->ADC_PowerEn                    = (FunctionalState)((ADCx->CTRL1) & (((uint32_t)(ADC_CTRL1_ADON)) >> ADC_CTRL1_ADON_Pos));
    ADC_InitStruct->ADC_PowerRegain                = (FunctionalState)((ADCx->CTRL1) & (((uint32_t)(ADC_CTRL1_RESETN)) >> ADC_CTRL1_RESETN_Pos));
    ADC_InitStruct->ADC_BufEn                      = (FunctionalState)((ADCx->CONFIG) & (((uint32_t)(ADC_CONFIG_EN_BUF)) >> ADC_CONFIG_EN_BUF_Pos));
}

/*
 * Function:        ADC_InitAMOStruct
 * Description:     Initialize Analog Monitor config structure.
 * Param:           ADC_AmoStruct: pointer to the config that need to be initialized.
 * Return:          None.
 */
void ADC_InitAMOStruct(ADC_AmoTypeDef * const ADC_AmoStruct)
{
    ADC_AmoStruct->ADC_AmoInterruptEn = DISABLE;
    ADC_AmoStruct->ADC_AmoRegularEn = DISABLE;
    ADC_AmoStruct->ADC_AmoInjectEn = DISABLE;
    ADC_AmoStruct->ADC_AmoSingleModeEn = DISABLE;
    ADC_AmoStruct->ADC_AmoSingleChannel = ADC_CH_1;

    ADC_AmoStruct->ADC_AmoUpThreshold = 0x00;
    ADC_AmoStruct->ADC_AmoLowThreshold = 0x00;
}

/*
 * Function:        ADC_ConfigAMO
 * Description:     Config Analog Monitor function.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  ADC_AmoStruct: pointer to configuration that need to apply to the register.
 * Return:          None.
 */
void ADC_ConfigAMO(ADC_TypeDef *ADCx, const ADC_AmoTypeDef * const ADC_AmoStruct)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(ADC_AmoStruct != NULL);
    assert_param(IS_ADC_BitAction(ADC_AmoStruct->ADC_AmoRegularEn));
    assert_param(IS_ADC_BitAction(ADC_AmoStruct->ADC_AmoInjectEn));
    assert_param(IS_ADC_BitAction(ADC_AmoStruct->ADC_AmoSingleModeEn));
    assert_param(IS_ADC_INPUT_CH(ADC_AmoStruct->ADC_AmoSingleChannel));
    assert_param(IS_ADC_BitAction(ADC_AmoStruct->ADC_AmoInterruptEn));

    /* Set adc analog monitor */
    ADCx->AMOCR &= ~((uint32_t)ADC_AMOCR_AMOEN);
    ADCx->AMOCR |= (((uint32_t)ADC_AmoStruct->ADC_AmoRegularEn) << ADC_AMOCR_AMOEN_Pos);
    ADCx->AMOCR &= ~((uint32_t)ADC_AMOCR_IAMOEN);
    ADCx->AMOCR |= (((uint32_t)ADC_AmoStruct->ADC_AmoInjectEn) << ADC_AMOCR_IAMOEN_Pos);
    ADCx->AMOCR &= ~((uint32_t)ADC_AMOCR_AMOSGL);
    ADCx->AMOCR |= (((uint32_t)ADC_AmoStruct->ADC_AmoSingleModeEn) << ADC_AMOCR_AMOSGL_Pos);
    ADCx->AMOCR &= ~((uint32_t)ADC_AMOCR_AMOCH);
    ADCx->AMOCR |= (((uint32_t)ADC_AmoStruct->ADC_AmoSingleChannel) << ADC_AMOCR_AMOCH_Pos);

    ADCx->AMOHR &= ~((uint32_t)ADC_AMOHR_AMOHT);
    ADCx->AMOHR |= (((uint32_t)ADC_AmoStruct->ADC_AmoUpThreshold) << ADC_AMOHR_AMOHT_Pos);

    ADCx->AMOLR &= ~((uint32_t)ADC_AMOLR_AMOLT);
    ADCx->AMOLR |= (((uint32_t)ADC_AmoStruct->ADC_AmoLowThreshold) << ADC_AMOLR_AMOLT_Pos);

    ADCx->AMOCR &= ~((uint32_t)ADC_AMOCR_AMOIE);
    ADCx->AMOCR |= (((uint32_t)ADC_AmoStruct->ADC_AmoInterruptEn) << ADC_AMOCR_AMOIE_Pos);
}

/*
 * Function:        ADC_GetAMOConfig
 * Description:     Get the configuration of AMO function.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  ADC_AmoStruct: pointer that need to be filled with AMO register configuration.
 * Return:          None.
 */
void ADC_GetAMOConfig(ADC_TypeDef *ADCx, ADC_AmoTypeDef * const ADC_AmoStruct)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(ADC_AmoStruct != NULL);

    ADC_AmoStruct->ADC_AmoRegularEn = (FunctionalState)((ADCx->AMOCR) & (((uint32_t)(ADC_AMOCR_AMOEN)) >> ADC_AMOCR_AMOEN_Pos));
    ADC_AmoStruct->ADC_AmoInjectEn = (FunctionalState)((ADCx->AMOCR) & (((uint32_t)(ADC_AMOCR_IAMOEN)) >> ADC_AMOCR_IAMOEN_Pos));
    ADC_AmoStruct->ADC_AmoSingleModeEn = (FunctionalState)((ADCx->AMOCR) & (((uint32_t)(ADC_AMOCR_AMOSGL)) >> ADC_AMOCR_AMOSGL_Pos));
    ADC_AmoStruct->ADC_AmoSingleChannel = (uint32_t)((ADCx->AMOCR) & (((uint32_t)(ADC_AMOCR_AMOCH)) >> ADC_AMOCR_AMOCH_Pos));

    ADC_AmoStruct->ADC_AmoUpThreshold = (uint16_t)((ADCx->AMOHR) & (((uint32_t)(ADC_AMOHR_AMOHT)) >> ADC_AMOHR_AMOHT_Pos));
    ADC_AmoStruct->ADC_AmoLowThreshold = (uint16_t)((ADCx->AMOLR) & (((uint32_t)(ADC_AMOLR_AMOLT)) >> ADC_AMOLR_AMOLT_Pos));

    ADC_AmoStruct->ADC_AmoInterruptEn = (FunctionalState)((ADCx->AMOCR) & (((uint32_t)(ADC_AMOCR_AMOIE)) >> ADC_AMOCR_AMOIE_Pos));
}

/*
 * Function:        ADC_InitTempStruct
 * Description:     Initialize temperature sensor config structure.
 * Param:           ADC_TempStruct: pointer to the config that need to be initialized.
 * Return:          None.
 */
void ADC_InitTempStruct(ADC_TempTypeDef * const ADC_TempStruct)
{
    ADC_TempStruct->ADC_TempSensorGain = ADC_ADJ_TD_GA_8;
    ADC_TempStruct->ADC_TempSensorOffset = ADC_ADJ_TD_OS_8;
    ADC_TempStruct->ADC_TempSamplePoint = ADC_ALG_MEAN_128;
    ADC_TempStruct->TempSensorEn = DISABLE;
}

/*
 * Function:        ADC_TempConfig
 * Description:     Config temperature sensor.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  ADC_TempStruct: pointer to configuration that need to apply to the register.
 * Return:          None.
 */
void ADC_TempConfig(ADC_TypeDef *ADCx, ADC_TempTypeDef * const ADC_TempStruct)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(ADC_TempStruct != NULL);

    /* Set Temperature sensor adjustment */
    ADCx->CONFIG &= ~((uint32_t)ADC_CONFIG_ADJ_TD_GA);
    ADCx->CONFIG |= (((uint32_t)ADC_TempStruct->ADC_TempSensorGain) << ADC_CONFIG_ADJ_TD_GA_Pos);
    ADCx->CONFIG &= ~((uint32_t)ADC_CONFIG_ADJ_TD_OS);
    ADCx->CONFIG |= (((uint32_t)ADC_TempStruct->ADC_TempSensorOffset) << ADC_CONFIG_ADJ_TD_OS_Pos);
    ADCx->CONFIG &= ~((uint32_t)ADC_CONFIG_ALG_MEAN);
    ADCx->CONFIG |= (((uint32_t)ADC_TempStruct->ADC_TempSamplePoint) << ADC_CONFIG_ALG_MEAN_Pos);

    /* When ADC_EN=1, If EN_TS is set from 0 to 1, the start time of temperature sensor is 6us@Typ, 10us@Max */
    ADCx->CTRL1 &= ~((uint32_t)ADC_CTRL1_RESETN);
    ADCx->CONFIG |= (((uint32_t)ADC_TempStruct->TempSensorEn) << ADC_CONFIG_EN_TS_Pos);
    ADC_InitDelayus(10);
    ADCx->CTRL1 |= ((uint32_t)ADC_CTRL1_RESETN);
}

/*
 * Function:        ADC_GetTempConfig
 * Description:     Get the configuration of temperature sensor.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  ADC_TempStruct: pointer that need to be filled with temperature sensor configuration.
 * Return:          None.
 */
void ADC_GetTempConfig(ADC_TypeDef *ADCx, ADC_TempTypeDef * const ADC_TempStruct)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(ADC_TempStruct != NULL);

    ADC_TempStruct->ADC_TempSensorGain = (uint32_t)((ADCx->CONFIG) & (((uint32_t)(ADC_CONFIG_ADJ_TD_GA)) >> ADC_CONFIG_ADJ_TD_GA_Pos));
    ADC_TempStruct->ADC_TempSensorOffset = (uint32_t)((ADCx->CONFIG) & (((uint32_t)(ADC_CONFIG_ADJ_TD_OS)) >> ADC_CONFIG_ADJ_TD_OS_Pos));
    ADC_TempStruct->ADC_TempSamplePoint = (uint32_t)((ADCx->CONFIG) & (((uint32_t)(ADC_CONFIG_ALG_MEAN)) >> ADC_CONFIG_ALG_MEAN_Pos));
    ADC_TempStruct->TempSensorEn = (FunctionalState)((ADCx->CONFIG) & (((uint32_t)(ADC_CONFIG_EN_TS)) >> ADC_CONFIG_EN_TS_Pos));
}

/*
 * Function:        ADC_InitChannelStruct
 * Description:     Initialization channel config structure.
 * Param:           ADC_ChannelStruct: channel configuration structure that need to be initialized.
 * Return:          None.
 */
void ADC_InitChannelStruct(ADC_ChannelTypeDef * const ADC_ChannelStruct)
{
    ADC_ChannelStruct->ADC_SampleTime = ADC_SPT_CLK_3;
    ADC_ChannelStruct->ADC_Channel = ADC_CH_DISABLE;
    ADC_ChannelStruct->ADC_InterruptEn = DISABLE;
}

/*
 * Function:        ADC_ConfigChannel
 * Description:     Apply the channel configuration to the hardware.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral
 *                  SeqIndex: ADC group seqeunce index.
 *                   This parameter can be one of the ADC_RSEQ_x where x can be (0..15).
 *                   This parameter can be one of the ADC_ISEQ_x where x can be (0..3).
 *                  ADC_ChannelStruct: configuration that need to apply to the group sequence function.
 * Return:          None.
 */
void ADC_ConfigChannel(ADC_TypeDef *ADCx, const uint32_t SeqIndex, const ADC_ChannelTypeDef * const ADC_ChannelStruct)
{
    uint8_t index = 0x00, pos = 0x00;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_SEQUENCE(SeqIndex));
    assert_param(ADC_ChannelStruct != NULL);
    assert_param(IS_ADC_SAMPLE_TIME(ADC_ChannelStruct->ADC_SampleTime));
    assert_param(IS_ADC_INPUT_CH(ADC_ChannelStruct->ADC_Channel));
    assert_param(IS_ADC_BitAction(ADC_ChannelStruct->ADC_InterruptEn));

    /* Check SAR input type selection */
    if(ADC_ChannelStruct->ADC_Channel > ADC_CH_16)
    {
        assert_param((ADCx->ADC_ChannelStruct & ADC_CONFIG_SELVI_HD_LS) == RESET);
    }
    else
    {
        /* Nothing to do */
    }

    /* Set sample time */
    index = (uint8_t)(ADC_ChannelStruct->ADC_Channel / ADC_EACH_REG_SPT_NUM);
    pos = (uint8_t)((ADC_ChannelStruct->ADC_Channel % ADC_EACH_REG_SPT_NUM) * ADC_SPT_Width);
    ADCx->SPT[index] &= ~((uint32_t)ADC_SPT_SPT << pos);
    ADCx->SPT[index] |= (((uint32_t)ADC_ChannelStruct->ADC_SampleTime) << pos);

    if ((uint32_t)SeqIndex < ADC_REGULAR_SEQ_NUM)
    {
        /* Regular group */
        ADCx->RSQR[SeqIndex] &= ~((uint32_t)ADC_RSQR_SEQ);
        ADCx->RSQR[SeqIndex] |= (((uint32_t)ADC_ChannelStruct->ADC_Channel) << ADC_RSQR_SEQ_Pos);
        ADCx->RSQR[SeqIndex] &= ~((uint32_t)ADC_RSQR_IE);
        ADCx->RSQR[SeqIndex] |= (((uint32_t)ADC_ChannelStruct->ADC_InterruptEn) << ADC_RSQR_IE_Pos);
    }
    else if (SeqIndex >= ADC_ISEQ_0 && SeqIndex < ADC_SEQ_MAX)
    {
        /* Injection group */
        ADCx->ISQR[SeqIndex - ADC_ISEQ_0] &= ~((uint32_t)ADC_ISQR_SEQ);
        ADCx->ISQR[SeqIndex - ADC_ISEQ_0] |= (((uint32_t)ADC_ChannelStruct->ADC_Channel) << ADC_ISQR_SEQ_Pos);
        ADCx->ISQR[SeqIndex - ADC_ISEQ_0] &= ~((uint32_t)ADC_ISQR_IE);
        ADCx->ISQR[SeqIndex - ADC_ISEQ_0] |= (((uint32_t)ADC_ChannelStruct->ADC_InterruptEn) << ADC_ISQR_IE_Pos);
    }
    else
    {
        /* Nothing to do */
    }
}

/*
 * Function:        ADC_GetChannelConfig
 * Description:     Get group sequence channel configuration form register.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  SeqIndex: ADC group seqeunce index.
 *                   This parameter can be one of the ADC_RSEQ_x where x can be (0..15).
 *                   This parameter can be one of the ADC_ISEQ_x where x can be (0..3).
 *                  ADC_ChannelStruct: pointer that need to be filled with group sequence channel register.
 * Return:          None.
 */
void ADC_GetChannelConfig(ADC_TypeDef *ADCx, const uint32_t SeqIndex, ADC_ChannelTypeDef * const ADC_ChannelStruct)
{
    uint8_t index = 0x00, pos = 0x00;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_SEQUENCE(SeqIndex));
    assert_param(ADC_ChannelStruct != NULL);

    if ((uint32_t)SeqIndex < ADC_REGULAR_SEQ_NUM)
    {
        ADC_ChannelStruct->ADC_InterruptEn = (FunctionalState)((ADCx->RSQR[SeqIndex]) & (((uint32_t)(ADC_RSQR_IE)) >> ADC_RSQR_IE_Pos));
        ADC_ChannelStruct->ADC_Channel = (uint32_t)((ADCx->RSQR[SeqIndex]) & (((uint32_t)(ADC_RSQR_SEQ)) >> ADC_RSQR_SEQ_Pos));
    }
    else if (SeqIndex >= ADC_ISEQ_0 && SeqIndex < ADC_SEQ_MAX)
    {
        ADC_ChannelStruct->ADC_InterruptEn = (FunctionalState)((ADCx->ISQR[SeqIndex - ADC_ISEQ_0]) & (((uint32_t)(ADC_ISQR_IE)) >> ADC_ISQR_IE_Pos));
        ADC_ChannelStruct->ADC_Channel = (uint32_t)((ADCx->ISQR[SeqIndex - ADC_ISEQ_0]) & (((uint32_t)(ADC_ISQR_SEQ)) >> ADC_ISQR_SEQ_Pos));
    }
    else
    {
        /* Nothing to do */
    }

    index = (uint8_t)(ADC_ChannelStruct->ADC_Channel / ADC_EACH_REG_SPT_NUM);
    pos = (uint8_t)((ADC_ChannelStruct->ADC_Channel % ADC_EACH_REG_SPT_NUM) * ADC_SPT_Width);
    ADC_ChannelStruct->ADC_SampleTime = (uint32_t)((ADCx->SPT[index]) & (((uint32_t)(ADC_SPT_SPT)) >> pos));
}

/*
 * Function:        ADC_SetInjectOffset
 * Description:     Set injection group offset value.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  SeqIndex: ADC group seqeunce index.
 *                   This parameter can be one of the ADC_ISEQ_x where x can be (0..3).
 *                  offset:  offset value, which will be subtracted after injection group index convertion complete.
 *                   This value should be less than 0x1000.
 * Return:          None.
 */
void ADC_SetInjectOffset(ADC_TypeDef *ADCx, const uint32_t SeqIndex, const uint16_t offset)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param((SeqIndex > ADC_RSEQ_15) && (SeqIndex < ADC_SEQ_MAX));
    assert_param(offset < 0x1000U);

    ADCx->IOFR[SeqIndex - ADC_ISEQ_0] &= ~((uint32_t)ADC_IOFR_IOFR);
    ADCx->IOFR[SeqIndex - ADC_ISEQ_0] |= (((uint32_t)offset) << ADC_IOFR_IOFR_Pos);
}

/*
 * Function:        ADC_GetInjectOffset
 * Description:     Set injection group offset value.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  SeqIndex: ADC group seqeunce index.
 *                   This parameter can be one of the ADC_ISEQ_x where x can be (0..3).
 * Return:          Offset value.
 */
uint16_t ADC_GetInjectOffset(ADC_TypeDef *ADCx, const uint32_t SeqIndex)
{
    uint16_t value = 0x00;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param((SeqIndex > ADC_RSEQ_15) && (SeqIndex < ADC_SEQ_MAX));

    value = (uint16_t)((ADCx->IOFR[SeqIndex - ADC_ISEQ_0] & ADC_IOFR_IOFR_Msk) >> ADC_IOFR_IOFR_Pos);
    
    return value;
}

/*
 * Function:        ADC_GetBufferStatus
 * Description:     Get the external reference input buffer enable signal flag.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 * Return:          External reference input buffer enable signal flag(SET or RESET).
 */
FlagStatus ADC_GetBufferStatus(ADC_TypeDef *ADCx)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    if ((ADCx->CONFIG & ADC_CONFIG_EN_BUF) != (uint32_t)RESET) 
    {
        bitstatus = SET;
    } 
    else 
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/*
 * Function:        ADC_ClearBufferStatus
 * Description:     Clear the external reference input buffer enable signal flag.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 * Return:          None.
 */
void ADC_ClearBufferStatus(ADC_TypeDef *ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    ADCx->CONFIG &= ~((uint32_t)ADC_CONFIG_EN_BUF);
}

/*
 * Function:        ADC_GetTempSensorStatus
 * Description:     Get the Temperature sensor enable signal flag.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 * Return:          Temperature sensor enable signal flag(SET or RESET).
 */
FlagStatus ADC_GetTempSensorStatus(ADC_TypeDef *ADCx)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    if ((ADCx->CONFIG & ADC_CONFIG_EN_TS) != (uint32_t)RESET) 
    {
        bitstatus = SET;
    } 
    else 
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/*
 * Function:        ADC_ClearTempSensorStatus
 * Description:     Clear the Temperature sensor enable signal flag.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 * Return:          None.
 */
void ADC_ClearTempSensorStatus(ADC_TypeDef *ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    ADCx->CONFIG &= ~((uint32_t)ADC_CONFIG_EN_TS);
}

/*
 * Function:        ADC_SoftRegularConvert
 * Description:     Start software trigger regular group convertion.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 * Return:          None.
 */
void ADC_SoftRegularConvert(ADC_TypeDef *ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    ADCx->CTRL0 |= ((uint32_t)ADC_CTRL0_SWSTART);
}

/*
 * Function:        ADC_SoftInjectConvert
 * Description:     Start software trigger inject group convertion.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 * Return:          None.
 */
void ADC_SoftInjectConvert(ADC_TypeDef *ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    ADCx->CTRL0 |= ((uint32_t)ADC_CTRL0_ISWSTART);
}

/*
 * Function:        ADC_GetFlagStatus
 * Description:     Checks whether the specified ADC flag is set or not.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  ADC_FLAG: specifies the flag to check. 
 *                   This parameter can be one of the following values:
 *                    ADC_FLAG_CONVCFT: Convert trigger conflict status flag.
 *                    ADC_FLAG_IDLE: ADC idle state flag.
 *                    ADC_FLAG_AMO: Analog monitor event occurs flag.
 *                  SeqIndex: ADC group seqeunce index.
 *                   This parameter can be one of the ADC_RSEQ_x where x can be (0..15).
 *                   This parameter can be one of the ADC_ISEQ_x where x can be (0..3).
 * Return:          The new state of ADC_FLAG (SET or RESET).
 */

FlagStatus ADC_GetFlagStatus(ADC_TypeDef *ADCx, uint8_t ADC_FLAG, const uint32_t SeqIndex)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_GET_FLAG(ADC_FLAG));
    assert_param(IS_ADC_SEQUENCE(SeqIndex));

    if(ADC_FLAG == ADC_FLAG_EOC)
    {
        if ((uint32_t)SeqIndex < ADC_REGULAR_SEQ_NUM)
        {
            bitstatus = (FlagStatus)((ADCx->RSQR[SeqIndex] & ((uint32_t)(ADC_RSQR_EOC))) >> ADC_RSQR_EOC_Pos);
        }
        else if (SeqIndex >= ADC_ISEQ_0 && SeqIndex < ADC_SEQ_MAX)
        {
            bitstatus = (FlagStatus)((ADCx->ISQR[(uint32_t)SeqIndex - (uint32_t)ADC_ISEQ_0]  & ((uint32_t)(ADC_ISQR_EOC))) >> ADC_ISQR_EOC_Pos);
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        if ((ADCx->STATE & ADC_FLAG) != (uint32_t)RESET) 
        {
            bitstatus = SET;
        } 
        else 
        {
            bitstatus = RESET;
        }
    }

    return bitstatus;
}

/*
 * Function:        ADC_ClearFlag
 * Description:     Clears the ADCx's pending flags.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  ADC_FLAG: specifies the flag to clear.
 *                   This parameter can be any combination of the following values:
 *                    ADC_FLAG_CONVCFT: Convert trigger conflict status flag.
 *                    ADC_FLAG_AMO: Analog monitor event occurs flag.
 *                  SeqIndex: ADC group seqeunce index.
 *                   This parameter can be one of the ADC_RSEQ_x where x can be (0..15).
 *                   This parameter can be one of the ADC_ISEQ_x where x can be (0..3).
 * Return:          None.
 */
void ADC_ClearFlag(ADC_TypeDef* ADCx, uint8_t ADC_FLAG, const uint32_t SeqIndex)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CLEAR_FLAG(ADC_FLAG));
  assert_param(IS_ADC_SEQUENCE(SeqIndex));

  /* Clear the selected ADC flags */
  if(ADC_FLAG == ADC_FLAG_EOC)
    {
        if ((uint32_t)SeqIndex < ADC_REGULAR_SEQ_NUM)
        {
            ADCx->RSQR[SeqIndex] |= ((uint32_t)ADC_RSQR_EOC);
        }
        else if (SeqIndex >= ADC_ISEQ_0 && SeqIndex < ADC_SEQ_MAX)
        {
            ADCx->ISQR[SeqIndex - ADC_ISEQ_0] |= ((uint32_t)ADC_ISQR_EOC);
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        ADCx->STATE = ~(uint32_t)ADC_FLAG;
    }
  
}

/*
 * Function:        ADC_WaitConvDone
 * Description:     Wait for the ADC conversion to finish.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 * Return:          None.
 */
void ADC_WaitConvDone(ADC_TypeDef *ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    while (!((ADCx->STATE & ADC_STATE_IDLE) >> ADC_STATE_IDLE_Pos))
    {
        /* Wait */
    }
}

/*
 * Function:        ADC_GetSeqResult
 * Description:     Get the ADC convertion result for the group sequence.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  SeqIndex: ADC group seqeunce index.
 *                   This parameter can be one of the ADC_RSEQ_x where x can be (0..15).
 *                   This parameter can be one of the ADC_ISEQ_x where x can be (0..3).
 *                  Result: ADC convertion result.
 * Return:          The ADC Sequence Channel.
 */
uint16_t ADC_GetSeqResult(ADC_TypeDef *ADCx, const uint32_t SeqIndex, uint16_t * const Result)
{
  uint32_t tmpreg = 0;
  uint16_t returnValue = 0;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_SEQUENCE(SeqIndex));

    if ((uint32_t)SeqIndex < ADC_REGULAR_SEQ_NUM)
    {
        tmpreg = ADCx->RDR[SeqIndex];
        *Result = (uint16_t)((tmpreg & ((uint32_t)ADC_DRD_RDR)) >> ADC_DRD_RDR_Pos);
        returnValue =  (uint16_t)((tmpreg & (uint32_t)ADC_DRD_CH)>>ADC_DRD_CH_Pos);
    }
    else if (SeqIndex >= ADC_ISEQ_0 && SeqIndex < ADC_SEQ_MAX)
    {
       tmpreg = ADCx->IDR[(uint32_t)SeqIndex - (uint32_t)ADC_ISEQ_0];
       *Result = (uint16_t)((tmpreg  & ((uint32_t)(ADC_IRD_IDR))) >> ADC_IRD_IDR_Pos);
       returnValue = (uint16_t)((tmpreg  & ((uint32_t)(ADC_IRD_CH))) >> ADC_IRD_CH_Pos);
    }
    else
    {
        /* Nothing to do */
    }
    
    return returnValue;
}

/*
 * Function:        ADC_SetHighResStatus
 * Description:     Set the High-resistance state flag.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 * Return:          None.
 */
void ADC_SetHighResStatus(ADC_TypeDef *ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    ADCx->CONFIG |= ((uint32_t)ADC_CONFIG_EN_HIZ);
}

/*
 * Function:        ADC_GetHighResStatus
 * Description:     Get the High-resistance state flag.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 * Return:          High-resistance state flag(SET or RESET).
 */
FlagStatus ADC_GetHighResStatus(ADC_TypeDef *ADCx)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    if ((ADCx->CONFIG &ADC_CONFIG_EN_HIZ) != (uint32_t)RESET) 
    {
        bitstatus = SET;
    } 
    else 
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/*
 * Function:        ADC_ClearHighResStatus
 * Description:     Clear the High-resistance state flag.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 * Return:          None.
 */
void ADC_ClearHighResStatus(ADC_TypeDef *ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    ADCx->CONFIG &= ~((uint32_t)ADC_CONFIG_EN_HIZ);
}

/*
 * Function:        ADC_GetRegularParity
 * Description:     Get ADC regular group data even parity.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  SeqIndex: ADC group seqeunce index.
 *                   This parameter can be one of the ADC_RSEQ_x where x can be (0..15).
 * Return:          ADC data even parity value of particular sequence.
 */
uint8_t ADC_GetRegularParity(ADC_TypeDef *ADCx, const uint32_t SeqIndex)
{
    uint8_t value = 0x00;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(SeqIndex <= ADC_RSEQ_15);

    value = (uint8_t)((ADCx->RDR[SeqIndex] & ADC_DRD_PARITY) >> ADC_DRD_PARITY_Pos);

    return value;
}

/*
 * Function:        ADC_GetRegularChannel
 * Description:     Get ADC regular group sequence channel number.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  SeqIndex: ADC group seqeunce index.
 *                   This parameter can be one of the ADC_RSEQ_x where x can be (0..15).
 * Return:          ADC channel of particular sequence.
 */
uint8_t ADC_GetRegularChannel(ADC_TypeDef *ADCx, const uint32_t SeqIndex)
{
    uint8_t value = 0x00;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(SeqIndex <= ADC_RSEQ_15);

    value = (uint8_t)((ADCx->RDR[SeqIndex] & ADC_DRD_CH) >> ADC_DRD_CH_Pos);

    return value;
}

/*
 * Function:        ADC_GetInjectParity
 * Description:     Get ADC injection group data even parity.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  SeqIndex: ADC group seqeunce index.
 *                   This parameter can be one of the ADC_RSEQ_x where x can be (0..15).
 * Return:          ADC data even parity value of particular sequence.
 */
uint8_t ADC_GetInjectParity(ADC_TypeDef *ADCx, const uint32_t SeqIndex)
{
    uint8_t value = 0x00;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param((SeqIndex > ADC_RSEQ_15) && (SeqIndex < ADC_SEQ_MAX));

    value = (uint8_t)((ADCx->IDR[(uint32_t)SeqIndex - (uint32_t)ADC_ISEQ_0] & ADC_IRD_PARITY) >> ADC_IRD_PARITY_Pos);

    return value;
}

/*
 * Function:        ADC_GetInjectChannel
 * Description:     Get ADC injection group sequence channel number.
 * Param:           ADCx: where x can be (0) to select the ADC peripheral.
 *                  SeqIndex: ADC group seqeunce index.
 *                   This parameter can be one of the ADC_RSEQ_x where x can be (0..15).
 * Return:          ADC channel of particular sequence.
 */
uint8_t ADC_GetInjectChannel(ADC_TypeDef *ADCx, const uint32_t SeqIndex)
{
    uint8_t value = 0x00;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param((SeqIndex > ADC_RSEQ_15) && (SeqIndex < ADC_SEQ_MAX));

    value = (uint8_t)((ADCx->IDR[(uint32_t)SeqIndex - (uint32_t)ADC_ISEQ_0] & ADC_IRD_CH) >> ADC_IRD_CH_Pos);

    return value;
}

/*
 * Function:        ADC_ConvertToTemp
 * Description:     Convert ADC value of T-Sensor to Temperature.
 * Param:           Value: ADC conversion result of internal T-Sensor.
 *                  refVoltage: reference voltage of ADC, in mV.
 * Return:          temperature value in celsius scale.
 */
float ADC_ConvertToTemp(const uint16_t Value, const float RefVoltage)
{
    float temperature = 0x00;
    /* voltage of ADC, in mV. The output slope KT is about 6mV/�?. The output VVTEMP is about 1.1V at 0�? */
    const float slope = 6, volTempBase = 1100;

    temperature = ((float)Value / 4096 * RefVoltage - volTempBase) / slope;

    return temperature;
}

/*
 * Function:        ADC_InitDelayus
 * Description:     ADC initialization delay.
 * Param:           Delayus : delay duration.
 * Return:          None.
 */
static void ADC_InitDelayus(uint32_t Delayus)
{
    int i = 0x00;
    /* Set this value based on the CPU clock （MHz�?*/
    const int count = 40U;

    /* Check the parameters */
    assert_param(Delayus > 0);

    for (i = 0; i < count * Delayus; i++)
    {
        __asm volatile("nop");
    }
}

/*****END OF FILE*****/
