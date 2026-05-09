/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_gpio.c
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2024/07/16
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/07/16     1.0     build this module
 * Description:     This file provides firmware functions to manage the functionalities of the GPIO peripheral.
 */

#include "as32x601_gpio.h"

/*
 * Function:        GPIO_StructInit
 * Description:     Fills each GPIO_InitStruct member with its default value.
 * Param:           GPIO_InitStruct : pointer to a GPIO_InitTypeDef structure which will be initialized.
 * Return:          None.
 */
void GPIO_StructInit(GPIO_InitTypeDef *GPIO_InitStruct)
{
    /* Reset GPIO init structure parameters values */
    GPIO_InitStruct->GPIO_Pin       = GPIO_Pin_All;         /* Specifies the GPIO pins to be configured */
    GPIO_InitStruct->GPIO_Mode      = GPIO_Mode_IN;         /* Specifies the operating mode for the selected pins */
    GPIO_InitStruct->GPIO_IType     = GPIO_IN_FLOATING;     /* Specifies the operating intput type for the selected pins */
    GPIO_InitStruct->GPIO_OType     = GPIO_Out_OD;          /* Specifies the operating output type for the selected pins */
    GPIO_InitStruct->GPIO_OStrength = GPIO_OStrength_4_5mA; /* Specifies the operating output strength for the selected pins */
}

/*
 * Function:        GPIO_Init
 * Description:     Initializes the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct.
 * Param:           GPIOx: where x can be (A..H) to select the GPIO peripheral.
 *                  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that contains the configuration information for the specified GPIO peripheral.
 * Return:          None.
 */
void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct)
{
    uint32_t pinpos = 0x00, pos = 0x00, currentpin = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));
    assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
    assert_param(IS_GPIO_ITYPE(GPIO_InitStruct->GPIO_IType));
    assert_param(IS_GPIO_OTYPE(GPIO_InitStruct->GPIO_OType));
    assert_param(IS_GPIO_OSTRENGTH(GPIO_InitStruct->GPIO_OStrength));

    /* GPIO mode and in/out type and out strength Configuration */
    for (pinpos = 0x00; pinpos < 16; pinpos++) 
    {
        pos = ((uint32_t)0x01) << pinpos;
        currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;

        if (currentpin == pos) 
        {
            /* GPIO mode Configuration */
            if(GPIOx != GPIOH)
            {
              GPIOx->RWM0CR &= ~((uint32_t)GPIO_RWM0CR_MODER0 << (pinpos * 2));
              GPIOx->RWM0CR |= (((uint32_t)GPIO_InitStruct->GPIO_Mode) << (pinpos * 2));
            }
            else
            {
              GPIOHMAP->RWM0CR &= ~((uint32_t)GPIO_RWM0CR_MODER0 << (pinpos * 2));
              GPIOHMAP->RWM0CR |= (((uint32_t)GPIO_InitStruct->GPIO_Mode) << (pinpos * 2));
            }
            /* GPIO input type configuration */
            if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IN) 
            {
                if(GPIOx != GPIOH)
                {
                    GPIOx->IM0CR &= ~((uint32_t)GPIO_IM0CR_PUPDR0 << (pinpos * 2));
                    GPIOx->IM0CR |= (((uint32_t)GPIO_InitStruct->GPIO_IType) << (pinpos * 2));
                }
                else
                {
                    GPIOHMAP->IM0CR &= ~((uint32_t)GPIO_IM0CR_PUPDR0 << (pinpos * 2));
                    GPIOHMAP->IM0CR |= (((uint32_t)GPIO_InitStruct->GPIO_IType) << (pinpos * 2));
                }
            }
            /* GPIO output type configuration */
            else if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OUT) 
            {
                if(GPIOx != GPIOH)
                {
                    GPIOx->OMCR &= ~((uint32_t)GPIO_OMCR_OT0 << pinpos);
                    GPIOx->OMCR |= (((uint32_t)GPIO_InitStruct->GPIO_OType) << pinpos);
                }
                else
                {
                    GPIOHMAP->OMCR &= ~((uint32_t)GPIO_OMCR_OT0 << pinpos);
                    GPIOHMAP->OMCR |= (((uint32_t)GPIO_InitStruct->GPIO_OType) << pinpos);
                }
            }
            else
            {
                /* Nothing to do */
            }
            /* GPIO output strength configuration */
            if(GPIOx != GPIOH)
            {
                GPIOx->OICR &= ~((uint32_t)GPIO_OICR_OSTRENGTH0 << (pinpos * 2));
                GPIOx->OICR |= (((uint32_t)GPIO_InitStruct->GPIO_OStrength) << (pinpos * 2));
            }
            else
            {
                GPIOHMAP->OICR &= ~((uint32_t)GPIO_OICR_OSTRENGTH0 << (pinpos * 2));
                GPIOHMAP->OICR |= (((uint32_t)GPIO_InitStruct->GPIO_OStrength) << (pinpos * 2));
            }
        }
        else
        {
            /* Nothing to do */
        }
    }
}

/*
 * Function:        GPIO_SetBits
 * Description:     Set the GPIO pin output to high level.
 * Param:           GPIOx: where x can be (A..H) to select the GPIO peripheral.
 *                  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_Pin_x where x can be (0..15).
 * Note:            The GPIOH has 15 channels,the GPIO_Pin_x where x can be (0..14).
 * Return:          None.
 */
void GPIO_SetBits(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    if(GPIOx != GPIOH)
    {
        GPIOx->OPBR |= GPIO_Pin;
    }
    else
    {
        GPIOHMAP->OPBR |= GPIO_Pin;
    }
}

/*
 * Function:        GPIO_ClearBits
 * Description:     Set the GPIO pin output to low level.
 * Param:           GPIOx: where x can be (A..H) to select the GPIO peripheral
 *                  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_Pin_x where x can be (0..15).
 * Note:            The GPIOH has 15 channels,the GPIO_Pin_x where x can be (0..14).
 * Return:          None.
 */
void GPIO_ClearBits(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    
    if(GPIOx != GPIOH)
    {
        GPIOx->OBCR |= GPIO_Pin;
    }
    else 
    {
        GPIOHMAP->OBCR |= GPIO_Pin;
    }
}

/*
 * Function:        GPIO_ToggleBits
 * Description:     Toggles the specified GPIO pins.
 * Param:           GPIOx: where x can be (A..H) to select the GPIO peripheral
 *                  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_Pin_x where x can be (0..15).
 * Note:            The GPIOH has 15 channels,the GPIO_Pin_x where x can be (0..14).
 * Return:          None.
 */
void GPIO_ToggleBits(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    if(GPIOx != GPIOH)
    {
        GPIOx->OBTR |= GPIO_Pin;
    }
    else 
    {
        GPIOHMAP->OBTR |= GPIO_Pin;
    }
}

/*
 * Function:        GPIO_WriteBits
 * Description:     Set the GPIO pin output level.
 * Param:           GPIOx: where x can be (A..H) to select the GPIO peripheral
 *                  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_Pin_x where x can be (0..15).
 *                  BitState: the value of GPIO pin output.
 *                   This parameter can be one of the following values: :
 *                      GPIO_OCtrl_Vol: The GPIO output is set to low level.
 *                      GPIO_OCtrl_Voh: The GPIO output is set to high level.
 * Note:            The GPIOH has 15 channels,the GPIO_Pin_x where x can be (0..14).
 * Return:          None.
 */
void GPIO_WriteBits(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, GPIOOCtrl_TypeDef OutState)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_OCTRL(OutState));

    if (OutState != (uint32_t)GPIO_OCtrl_Vol)
    {
        if(GPIOx != GPIOH)
        {
            GPIOx->OCR |= GPIO_Pin;
        }
        else 
        {
            GPIOHMAP->OCR |= GPIO_Pin;
        }  
    } 
    else 
    {
        if(GPIOx != GPIOH)
        {
            GPIOx->OCR &= ~GPIO_Pin;
        }
        else 
        {
            GPIOHMAP->OCR &= ~GPIO_Pin;
        } 
    }
}

/*
 * Function:        GPIO_Write
 * Description:     Set the GPIO pin output level.
 * Param:           GPIOx: where x can be (A..H) to select the GPIO peripheral.
 * Note:            The GPIOH has 15 channels,the GPIO_Pin_x where x can be (0..14).
 * Return:          None.
 */
void GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t PortVal)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    if(GPIOx != GPIOH)
    {
        GPIOx->OCR = PortVal;
    }
    else 
    {
        GPIOHMAP->OCR = PortVal;
    } 
}

/*
 * Function:        GPIO_ReadOutputDataBit
 * Description:     Gets the specified GPIO port output data value.
 * Param:           GPIOx: where x can be (A..H) to select the GPIO peripheral
 *                  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_Pin_x where x can be (0..15).
 * Note:            The GPIOH has 15 channels,the GPIO_Pin_x where x can be (0..14).
 * Return:          GPIO output data port value.
 */
GPIOOCtrl_TypeDef GPIO_ReadOutputDataBit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    GPIOOCtrl_TypeDef bitstatus = GPIO_OCtrl_Vol;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

    if(GPIOx != GPIOH)
    {
        if ((GPIOx->OCR & GPIO_Pin) != (uint32_t)GPIO_OCtrl_Vol) 
        {
            bitstatus = GPIO_OCtrl_Voh;
        } 
        else 
        {
            bitstatus = GPIO_OCtrl_Vol;
        }
    }
    else
    {
        if ((GPIOHMAP->OCR & GPIO_Pin) != (uint32_t)GPIO_OCtrl_Vol) 
        {
            bitstatus = GPIO_OCtrl_Voh;
        } 
        else 
        {
            bitstatus = GPIO_OCtrl_Vol;
        }
    }


    return bitstatus;
}

/*
 * Function:        GPIO_ReadOutputData
 * Description:     Gets the specified GPIO port output data value.
 * Param:           GPIOx: where x can be (A..H) to select the GPIO peripheral.
 * Note:            The GPIOH has 15 channels,the GPIO_Pin_x where x can be (0..14).
 * Return:          GPIO output data port value.
 */
uint32_t GPIO_ReadOutputData(GPIO_TypeDef *GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    if(GPIOx != GPIOH)
    {
        return ((uint32_t)GPIOx->OCR);
    }
    else
    {
        return ((uint32_t)GPIOHMAP->OCR);
    }

}

/*
 * Function:        GPIO_ReadInputDataBit
 * Description:     Gets the specified GPIO port input data value.
 * Param:           GPIOx: where x can be (A..H) to select the GPIO peripheral
 *                  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_Pin_x where x can be (0..15).
 * Note:            The GPIOH has 15 channels,the GPIO_Pin_x where x can be (0..14).
 * Return:          GPIO input data port value.
 */
GPIOIRead_TypeDef GPIO_ReadInputDataBit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    GPIOIRead_TypeDef bitstatus = GPIO_IRead_Vil;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
    if(GPIOx != GPIOH)
    {
        if ((GPIOx->IRR & GPIO_Pin) != (uint32_t)GPIO_IRead_Vil) 
        {
            bitstatus = GPIO_IRead_Vih;
        } 
        else 
        {
            bitstatus = GPIO_IRead_Vil;
        }
    }
    else
    {
        if ((GPIOHMAP->IRR & GPIO_Pin) != (uint32_t)GPIO_IRead_Vil) 
        {
            bitstatus = GPIO_IRead_Vih;
        } 
        else 
        {
            bitstatus = GPIO_IRead_Vil;
        }
    }

    return bitstatus;
}

/*
 * Function:        GPIO_ReadInputData
 * Description:     Gets the specified GPIO port input data value.
 * Param:           GPIOx: where x can be (A..H) to select the GPIO peripheral.
 * Note:            The GPIOH has 15 channels,the GPIO_Pin_x where x can be (0..14).
 * Return:          GPIO input data port value.
 */
uint32_t GPIO_ReadInputData(GPIO_TypeDef *GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    
    if(GPIOx != GPIOH)
    {
        return ((uint32_t)GPIOx->IRR);
    }
    else
    {
        return ((uint32_t)GPIOHMAP->IRR);
    }
}

/*
 * Function:        GPIO_PinAFConfig
 * Description:     Set the specified GPIO pin multiplex function.
 * Param:           GPIOx: where x can be (A..H) to select the GPIO peripheral
 *                  GPIO_PinSource: selects the GPIO pin source to used as multiplex interface.
 *                   This parameter can be one of GPIO_PinSourcex where x can be (0..15).
 *                  GPIO_Remap: select the function of the port to be multiplexed.
 *                   This parameter can be one of the following values:
 *                      GPIO_Mux_Pin_x: pin x serves as a universal GPIO function.
 *                      GPIO_Mux_RC16M: Connect MAC pins to multiplex function 0.
 *                      GPIO_Mux_RC32K: Connect TIM3 pins to multiplex function 0.
 *                      GPIO_Mux_HTIM0: Connect HTIM0 pins to multiplex function 1.
 *                      GPIO_Mux_HTIM1: Connect HTIM1 pins to multiplex function 1.
 *                      GPIO_Mux_CAN0: Connect CAN0 pins to multiplex function 1.
 *                      GPIO_Mux_CAN1: Connect CAN1 pins to multiplex function 1.
 *                      GPIO_Mux_CAN2: Connect CAN2 pins to multiplex function 1.
 *                      GPIO_Mux_CAN3: Connect CAN3 pins to multiplex function 1.
 *                      GPIO_Mux_IIC0: Connect IIC0 pins to multiplex function 1.
 *                      GPIO_Mux_IIC2: Connect IIC2 pins to multiplex function 1.
 *                      GPIO_Mux_SPI0: Connect SPI0 pins to multiplex function 1.
 *                      GPIO_Mux_SPI1: Connect SPI1 pins to multiplex function 1.
 *                      GPIO_Mux_SPI2: Connect SPI2 pins to multiplex function 1.
 *                      GPIO_Mux_USART0: Connect USART0 pins to multiplex function 1.
 *                      GPIO_Mux_USART1: Connect USART1 pins to multiplex function 1.
 *                      GPIO_Mux_USART2: Connect USART2 pins to multiplex function 1.
 *                      GPIO_Mux_USART3: Connect USART3 pins to multiplex function 1.
 *                      GPIO_Mux_CLKSEL: Connect CLKSEL pins to multiplex function 0.
 *                      GPIO_Mux_QSPI: Connect QSPI pins to multiplex function 0.
 *                      GPIO_Mux_JTAG: Connect JTAGT pins to multiplex function 0.
 * Note:            The GPIOH has 15 channels,the GPIO_PinSource_x where x can be (0..14).
 * Note:            By default, the GPIO pins as the multiplex function 0.
 * Return:          None.
 */
void GPIO_PinAFConfig(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource, uint8_t GPIO_AF)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
    assert_param(IS_GPIO_AF(GPIO_AF));

    /* Select different registers according to the size of the pin number value */
    if (GPIO_PinSource < 10) 
    {
        if(GPIOx != GPIOH)
        {
            GPIOx->MI0SR &= ~((uint32_t)GPIO_MI0SR_AFR0 << (GPIO_PinSource * 3));
            GPIOx->MI0SR |= (uint32_t)GPIO_AF << (GPIO_PinSource * 3);
        }
        else
        {
            GPIOHMAP->MI0SR &= ~((uint32_t)GPIO_MI0SR_AFR0 << (GPIO_PinSource * 3));
            GPIOHMAP->MI0SR |= (uint32_t)GPIO_AF << (GPIO_PinSource * 3);
        }
    } 
    else 
    {
        if(GPIOx != GPIOH)
        {
            GPIOx->MI1SR &= ~((uint32_t)GPIO_MI1SR_AFR10 << ((GPIO_PinSource - 10) * 3));
            GPIOx->MI1SR |= (uint32_t)GPIO_AF << ((GPIO_PinSource - 10) * 3);
        }
        else
        {
            /* Nothing to do */
        }
    }
}

/*
 * Function:        GPIO_ITConfig
 * Description:     Configer the GPIO pin interrupt trigger type.
 * Param:           GPIOx: where x can be (A..H) to select the GPIO peripheral
 *                  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_Pin_x where x can be (0..15).
 *                  ITType: GPIO pin interrupt trigger type.
 *                   This parameter can be one of the following values:
 *                      GPIO_ITType_CLOSE: the GPIO No-interrupt triggering.
 *                      GPIO_ITType_EDGEUP: the GPIO rising edge triggers interrupt.
 *                      GPIO_ITType_EDGEDOWN: the GPIO falling edge triggers interrupt.
 *                      GPIO_ITType_EDGE: the GPIO a rising or falling edge triggers an interrupt.
 * Note:            The GPIOH has 15 channels,the GPIO_Pin_x where x can be (0..14).
 *  *               NewState: new state of the GPIO software interrupt.
 *                   This parameter can be: ENABLE or DISABLE
 * Return:          None.
 */
void GPIO_ITConfig(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, GPIOITType_TypeDef ITType, FunctionalState NewState)
{
    uint32_t pinpos = 0x00, pos = 0x00, currentpin = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_ITTYPE(ITType));
    assert_param(IS_EnState_STATE(NewState));

    for (pinpos = 0x00; pinpos < 16; pinpos++) 
    {
        pos = ((uint32_t)0x01) << pinpos;
        currentpin = GPIO_Pin & pos;
        if (currentpin == pos) 
        {
            if (NewState != DISABLE)
            {
                if(GPIOx != GPIOH)
                {
                    GPIOx->ITYPE0R |= (uint32_t)ITType << (pinpos * 2);
                    GPIOx->IMR &= ~((uint32_t)0x01 << pinpos);
                }
                else
                {
                    GPIOHMAP->ITYPE0R |= (uint32_t)ITType << (pinpos * 2);
                    GPIOHMAP->IMR &= ~((uint32_t)0x01 << pinpos);
                }
            } 
            else 
            {
                if(GPIOx != GPIOH)
                {
                    GPIOx->ITYPE0R &= ~((uint32_t)ITType << (pinpos * 2));
                    GPIOx->IMR |= ((uint32_t)0x01 << pinpos);
                }
                else
                {
                    GPIOHMAP->ITYPE0R &= ~((uint32_t)ITType << (pinpos * 2));
                    GPIOHMAP->IMR |= ((uint32_t)0x01 << pinpos);
                }
            }           
        }
        else
        {
            /* Nothing to do */
        }
    }
}

/*
 * Function:        GPIO_GetITStatus
 * Description:     Gets the specified GPIO interrupt status.
 * Param:           GPIOx: where x can be (A..H) to select the GPIO peripheral
 *                  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_Pin_x where x can be (0..15).
 * Note:            The GPIOH has 15 channels,the GPIO_Pin_x where x can be (0..14).
 * Return:          GPIO interrupt status value.
 * Note:            The value of status register can reference this formula : status = raw && (!mask).
 */
ITStatus GPIO_GetITStatus(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    ITStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

    if(GPIOx != GPIOH)
    {
        if ((GPIOx->ISR & GPIO_Pin) != (uint32_t)GPIO_OCtrl_Vol) 
        {
            bitstatus = SET;
        } 
        else 
        {
            bitstatus = RESET;
        }
    }
    else
    {
        if ((GPIOHMAP->ISR & GPIO_Pin) != (uint32_t)GPIO_OCtrl_Vol) 
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
 * Function:        GPIO_ForceINT
 * Description:     Force the interrupt of the specified GPIO.
 * Param:           GPIOx: where x can be (A..H) to select the GPIO peripheral.
 *                  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_Pin_x where x can be (0..15).
 * Note:            The GPIOH has 15 channels,the GPIO_Pin_x where x can be (0..14).
 *                  NewState: new state of the GPIO software interrupt.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void GPIO_ForceINT(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    if(GPIOx != GPIOH)
    {
        GPIOx->IFR |= GPIO_Pin;
    }
    else
    {
        GPIOHMAP->IFR |= GPIO_Pin;
    }
}

/*
 * Function:        GPIO_ITMaskConfig
 * Description:     Configer the interrupt mask of the specified GPIO.
 * Param:           GPIOx: where x can be (A..H) to select the GPIO peripheral.
 *                  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_Pin_x where x can be (0..15).
 * Note:            The GPIOH has 15 channels,the GPIO_Pin_x where x can be (0..14).
 *                  NewState: new state of the GPIO software interrupt.
 *                   This parameter can be: ENABLE or DISABLE.
 * Return:          None.
 */
void GPIO_ITMaskConfig(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, EnState NewState)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_EnState_STATE(NewState));

    if (NewState == DISABLE) 
    {
        if(GPIOx != GPIOH)
        {
            GPIOx->IMR |= GPIO_Pin;
        }
        else
        {
            GPIOHMAP->IMR |= GPIO_Pin;
        }
    } 
    else 
    {
        if(GPIOx != GPIOH)
        {
            GPIOx->IMR &= ~(GPIO_Pin);
        }
        else
        {
            GPIOHMAP->IMR &= ~(GPIO_Pin);
        }
    }
}

/*
 * Function:        GPIO_ClearITPendingBit
 * Description:     Clear the interrupt pending bits of the specified GPIO.
 * Param:           GPIOx: where x can be (A..H) to select the GPIO peripheral.
 *                  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_Pin_x where x can be (0..15).
 * Note:            The GPIOH has 15 channels,the GPIO_Pin_x where x can be (0..14).
 * Return:          None.
 */
void GPIO_ClearITPendingBit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    if(GPIOx != GPIOH)
    {
        GPIOx->IRSR = GPIO_Pin;
    }
    else
    {
        GPIOHMAP->IRSR = GPIO_Pin;
    }
}

/*****END OF FILE*****/
