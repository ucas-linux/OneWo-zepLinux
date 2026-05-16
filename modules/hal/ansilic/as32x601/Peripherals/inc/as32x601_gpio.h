/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_gpio.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2024/07/16
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/07/16     1.0     build this module
 * Description:     This file contains all the functions prototypes for the GPIO firmware library.
 */

#ifndef AS32X601_GPIO_H
#define AS32X601_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "as32x601_gpio_regs.h"

/*
 * @brief  Use by parameter examine
 */
#define IS_GPIO_ALL_PERIPH(PERIPH) (((PERIPH) == GPIOA) || \
                                    ((PERIPH) == GPIOB) || \
                                    ((PERIPH) == GPIOC) || \
                                    ((PERIPH) == GPIOD) || \
                                    ((PERIPH) == GPIOE) || \
                                    ((PERIPH) == GPIOF) || \
                                    ((PERIPH) == GPIOG))

/*
 * @brief  GPIO input/output mode enumeration
 */
typedef enum 
{
    GPIO_Mode_IN   = 0x00, /* Digital input mode */
    GPIO_Mode_OUT  = 0x01, /* Digital output mode */
    GPIO_Mode_AN   = 0x02, /* Simulation model */
    GPIO_Mode_IDDQ = 0x03  /* IDDQ model */
} GPIOMode_TypeDef;
#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_IN) || ((MODE) == GPIO_Mode_OUT) || \
                            ((MODE) == GPIO_Mode_AN) || ((MODE) == GPIO_Mode_IDDQ))

/*
 * @brief GPIO input type enumeration
 */
typedef enum 
{
    GPIO_IN_FLOATING = 0x00,    /* Floating input mode */
    GPIO_IPU         = 0x01,    /* PullUp input mode */
    GPIO_IPD         = 0x02,    /* PullDown input mode */
    GPIO_KEEP        = 0x03	    /* KEEPmode(Pull all the way up and down) */
} GPIOIType_TypeDef;
#define IS_GPIO_ITYPE(ITYPE) (((ITYPE) == GPIO_IN_FLOATING) || ((ITYPE) == GPIO_IPU) || \
                              ((ITYPE) == GPIO_IPD)         || ((ITYPE) == GPIO_KEEP))

/*
 * @brief GPIO output type enumeration
 */
typedef enum 
{
    GPIO_Out_OD = 0x00,	/* Output OD */
    GPIO_Out_PP = 0x01	/* Push–pull output */
} GPIOOType_TypeDef;
#define IS_GPIO_OTYPE(OTYPE) (((OTYPE) == GPIO_Out_OD) || ((OTYPE) == GPIO_Out_PP))

/*
 * @brief GPIO output strength enumeration
 */
typedef enum 
{
    GPIO_OStrength_4_5mA  = 0x00,	/* Output maximum drive current 4.5mA */
    GPIO_OStrength_9mA    = 0x01,	/* Output maximum drive current 9mA */
    GPIO_OStrength_13_5mA = 0x02,	/* Output maximum drive current 13.5mA */
    GPIO_OStrength_18mA   = 0x03	/* Output maximum drive current 18mA */
} GPIOOStrength_TypeDef;
#define IS_GPIO_OSTRENGTH(OSTRENGTH) (((OSTRENGTH) == GPIO_OStrength_4_5mA)  || ((OSTRENGTH) == GPIO_OStrength_9mA) || \
                                      ((OSTRENGTH) == GPIO_OStrength_13_5mA) || ((OSTRENGTH) == GPIO_OStrength_18mA))

/*
 * @brief GPIO output bit set enumeration
 */
typedef enum 
{
    GPIO_OBitSet_KEEP = 0x00,	/* Keep current state */
    GPIO_OBitSet_Voh  = 0x01	/* The GPIO output is set to high level */
} GPIOOBitSet_TypeDef;
#define IS_GPIO_BITSET(BITSET) (((BITSET) == GPIO_OBitSet_KEEP) || ((BITSET) == GPIO_OBitSet_Voh))

/*
 * @brief GPIO output bit clear enumeration
 */
typedef enum 
{
    GPIO_OBitClear_KEEP = 0x00,	/* Keep current state */
    GPIO_OBitClear_Vol  = 0x01	/* The GPIO output is set to low level */
} GPIOOBitClear_TypeDef;
#define IS_GPIO_BITCLEAR(BITCLEAR) (((BITCLEAR) == GPIO_OBitClear_KEEP) || ((BITCLEAR) == GPIO_OBitClear_Vol))

/*
 * @brief GPIO output bit toggle enumeration
 */
typedef enum 
{
    GPIO_OBitToggle_KEEP = 0x00,	/* Keep current state */
    GPIO_OBitToggle_Vot  = 0x01	/* The GPIO output is set to toggle level */
} GPIOOBitToggle_TypeDef;
#define IS_GPIO_OBITTOGGLE(OBITTOGGLE) (((OBITTOGGLE) == GPIO_OBitToggle_KEEP) || ((OBITTOGGLE) == GPIO_OBitToggle_Vot))

/*
 * @brief GPIO output cotrol enumeration
 */
typedef enum 
{
    GPIO_OCtrl_Vol = 0x00,	/* The GPIO output is set to low level */
    GPIO_OCtrl_Voh = 0x01	/* The GPIO output is set to high level */
} GPIOOCtrl_TypeDef;
#define IS_GPIO_OCTRL(OCTRL) (((OCTRL) == GPIO_OCtrl_Vol) || ((OCTRL) == GPIO_OCtrl_Voh))

/*
 * @brief GPIO input read enumeration
 */
typedef enum 
{
    GPIO_IRead_Vil = 0x00,	/* The GPIO input is set to low level */
    GPIO_IRead_Vih = 0x01	/* The GPIO input is set to high level */
} GPIOIRead_TypeDef;
#define IS_GPIO_IREAD(IREAD) (((IREAD) == GPIO_IRead_Vil) || ((IREAD) == GPIO_IRead_Vih))

/*
 * @brief GPIO Interrupt triggering mode enumeration
 */
typedef enum 
{
    GPIO_ITType_CLOSE    = 0x00,	/* The GPIO No-interrupt triggering */
    GPIO_ITType_EDGEUP   = 0x01,	/* The GPIO rising edge triggers interrupt */
    GPIO_ITType_EDGEDOWN = 0x02,	/* The GPIO falling edge triggers interrupt */
    GPIO_ITType_EDGE     = 0x03		/* The GPIO a rising or falling edge triggers an interrupt */
} GPIOITType_TypeDef;
#define IS_GPIO_ITTYPE(ITType) (((ITTYPE) == GPIO_ITType_CLOSE)    || ((ITTYPE) == GPIO_ITType_EDGEUP) || \
                                ((ITTYPE) == GPIO_ITType_EDGEDOWN) || ((ITTYPE) == GPIO_ITType_EDGE))

/*
 * @brief GPIO init structure definition
 */
typedef struct
{
    uint32_t GPIO_Pin;                      /* Specifies the GPIO pins to be configured */
    GPIOMode_TypeDef GPIO_Mode;             /* Specifies the operating mode for the selected pins */
    GPIOIType_TypeDef GPIO_IType;           /* Specifies the operating intput type for the selected pins */
    GPIOOType_TypeDef GPIO_OType;           /* Specifies the operating output type for the selected pins */
    GPIOOStrength_TypeDef GPIO_OStrength;   /* Specifies the operating output strength for the selected pins */
} GPIO_InitTypeDef;

/*
 * @defgroup GPIO pins definition
 */
#define GPIO_Pin_0           ((uint32_t)0x0001) /* GPIO pin 0 selected */
#define GPIO_Pin_1           ((uint32_t)0x0002) /* GPIO pin 1 selected */
#define GPIO_Pin_2           ((uint32_t)0x0004) /* GPIO pin 2 selected */
#define GPIO_Pin_3           ((uint32_t)0x0008) /* GPIO pin 3 selected */
#define GPIO_Pin_4           ((uint32_t)0x0010) /* GPIO pin 4 selected */
#define GPIO_Pin_5           ((uint32_t)0x0020) /* GPIO pin 5 selected */
#define GPIO_Pin_6           ((uint32_t)0x0040) /* GPIO pin 6 selected */
#define GPIO_Pin_7           ((uint32_t)0x0080) /* GPIO pin 7 selected */
#define GPIO_Pin_8           ((uint32_t)0x0100) /* GPIO pin 8 selected */
#define GPIO_Pin_9           ((uint32_t)0x0200) /* GPIO pin 9 selected */
#define GPIO_Pin_10          ((uint32_t)0x0400) /* GPIO pin 10 selected */
#define GPIO_Pin_11          ((uint32_t)0x0800) /* GPIO pin 11 selected */
#define GPIO_Pin_12          ((uint32_t)0x1000) /* GPIO pin 12 selected */
#define GPIO_Pin_13          ((uint32_t)0x2000) /* GPIO pin 13 selected */
#define GPIO_Pin_14          ((uint32_t)0x4000) /* GPIO pin 14 selected */
#define GPIO_Pin_15          ((uint32_t)0x8000) /* GPIO pin 14 selected */
#define GPIO_Pin_All         ((uint32_t)0xFFFF) /* GPIO all pins selected */

#define GPIO_PIN_MASK        ((uint32_t)0x0000FFFF) /* PIN mask for assert test */
#define IS_GPIO_PIN(PIN)     (((PIN) & GPIO_PIN_MASK) != (uint32_t)0x00)
#define IS_GET_GPIO_PIN(PIN) (((PIN) == GPIO_Pin_0)  || \
                              ((PIN) == GPIO_Pin_1)  || \
                              ((PIN) == GPIO_Pin_2)  || \
                              ((PIN) == GPIO_Pin_3)  || \
                              ((PIN) == GPIO_Pin_4)  || \
                              ((PIN) == GPIO_Pin_5)  || \
                              ((PIN) == GPIO_Pin_6)  || \
                              ((PIN) == GPIO_Pin_7)  || \
                              ((PIN) == GPIO_Pin_8)  || \
                              ((PIN) == GPIO_Pin_9)  || \
                              ((PIN) == GPIO_Pin_10) || \
                              ((PIN) == GPIO_Pin_11) || \
                              ((PIN) == GPIO_Pin_12) || \
                              ((PIN) == GPIO_Pin_13) || \
                              ((PIN) == GPIO_Pin_14) || \
                              ((PIN) == GPIO_Pin_15) || \
                              ((PIN) == GPIO_Pin_All))

/*
 * @defgroup GPIO pin sources definition
 */
#define GPIO_PinSource0               ((uint32_t)0x00)  /* GPIO pin source 0 selected */
#define GPIO_PinSource1               ((uint32_t)0x01)  /* GPIO pin source 1 selected */
#define GPIO_PinSource2               ((uint32_t)0x02)  /* GPIO pin source 2 selected */
#define GPIO_PinSource3               ((uint32_t)0x03)  /* GPIO pin source 3 selected */
#define GPIO_PinSource4               ((uint32_t)0x04)  /* GPIO pin source 4 selected */
#define GPIO_PinSource5               ((uint32_t)0x05)  /* GPIO pin source 5 selected */
#define GPIO_PinSource6               ((uint32_t)0x06)  /* GPIO pin source 6 selected */
#define GPIO_PinSource7               ((uint32_t)0x07)  /* GPIO pin source 7 selected */
#define GPIO_PinSource8               ((uint32_t)0x08)  /* GPIO pin source 8 selected */
#define GPIO_PinSource9               ((uint32_t)0x09)  /* GPIO pin source 9 selected */
#define GPIO_PinSource10              ((uint32_t)0x0A)  /* GPIO pin source 10 selected */
#define GPIO_PinSource11              ((uint32_t)0x0B)  /* GPIO pin source 11 selected */
#define GPIO_PinSource12              ((uint32_t)0x0C)  /* GPIO pin source 12 selected */
#define GPIO_PinSource13              ((uint32_t)0x0D)  /* GPIO pin source 13 selected */
#define GPIO_PinSource14              ((uint32_t)0x0E)  /* GPIO pin source 14 selected */
#define GPIO_PinSource15              ((uint32_t)0x0F)  /* GPIO pin source 14 selected */

#define IS_GPIO_PIN_SOURCE(PINSOURCE) (((PINSOURCE) == GPIO_PinSource0)  || \
                                       ((PINSOURCE) == GPIO_PinSource1)  || \
                                       ((PINSOURCE) == GPIO_PinSource2)  || \
                                       ((PINSOURCE) == GPIO_PinSource3)  || \
                                       ((PINSOURCE) == GPIO_PinSource4)  || \
                                       ((PINSOURCE) == GPIO_PinSource5)  || \
                                       ((PINSOURCE) == GPIO_PinSource6)  || \
                                       ((PINSOURCE) == GPIO_PinSource7)  || \
                                       ((PINSOURCE) == GPIO_PinSource8)  || \
                                       ((PINSOURCE) == GPIO_PinSource9)  || \
                                       ((PINSOURCE) == GPIO_PinSource10) || \
                                       ((PINSOURCE) == GPIO_PinSource11) || \
                                       ((PINSOURCE) == GPIO_PinSource12) || \
                                       ((PINSOURCE) == GPIO_PinSource13) || \
                                       ((PINSOURCE) == GPIO_PinSource14) || \
                                       ((PINSOURCE) == GPIO_PinSource15))

/*
 * @defgroup GPIO Alternat function selection define
 */
/*
 * @defgroup GPIO Alternate function pin x definition
 */
#define GPIO_AF_Pin_x       ((uint8_t)0x00)     /* GPIO pin x Alternate Function mapping */

/*
 * @defgroup GPIO Alternate function USART definition
 */
#define GPIO_AF_USART0      ((uint8_t)0x02)     /* USART0 Alternate Function mapping */
#define GPIO_AF_USART1      ((uint8_t)0x04)     /* USART1 Alternate Function mapping */
#define GPIO_AF_USART2      ((uint8_t)0x02)     /* USART2 Alternate Function mapping */
#define GPIO_AF_USART3      ((uint8_t)0x02)     /* USART3 Alternate Function mapping */
#define GPIO_AF_USART4      ((uint8_t)0x02)     /* USART4 Alternate Function mapping */
#define GPIO_AF_USART5      ((uint8_t)0x04)     /* USART5 Alternate Function mapping */
#define GPIO_AF_USART6      ((uint8_t)0x03)     /* USART6 Alternate Function mapping */
#define GPIO_AF_USART7      ((uint8_t)0x03)     /* USART7 Alternate Function mapping */

/*
 * @defgroup GPIO Alternate function CAN definition
 */
#define GPIO_AF_CAN0_M1     ((uint8_t)0x02)     /* CAN0 Alternate Function mapping */
#define GPIO_AF_CAN0_M3     ((uint8_t)0x04)     /* CAN0 Alternate Function mapping */
#define GPIO_AF_CAN1        ((uint8_t)0x02)     /* CAN1 Alternate Function mapping */
#define GPIO_AF_CAN2        ((uint8_t)0x02)     /* CAN2 Alternate Function mapping */
#define GPIO_AF_CAN3        ((uint8_t)0x02)     /* CAN3 Alternate Function mapping */

/*
 * @defgroup GPIO Alternate function IIC definition
 */
#define GPIO_AF_IIC0_M1     ((uint8_t)0x02)     /* IIC0 Alternate Function mapping */
#define GPIO_AF_IIC0_M3     ((uint8_t)0x04)     /* IIC0 Alternate Function mapping */
#define GPIO_AF_IIC1        ((uint8_t)0x02)     /* IIC1 Alternate Function mapping */
#define GPIO_AF_IIC2        ((uint8_t)0x04)     /* IIC2 Alternate Function mapping */
#define GPIO_AF_IIC3        ((uint8_t)0x02)     /* IIC3 Alternate Function mapping */

/*
 * @defgroup GPIO Alternate function SPI definition
 */
#define GPIO_AF_SPI0        ((uint8_t)0x02)     /* SPI0 Alternate Function mapping */
#define GPIO_AF_SPI1        ((uint8_t)0x03)     /* SPI1 Alternate Function mapping */
#define GPIO_AF_SPI2        ((uint8_t)0x03)     /* SPI2 Alternate Function mapping */
#define GPIO_AF_SPI3        ((uint8_t)0x02)     /* SPI3 Alternate Function mapping */
#define GPIO_AF_SPI4        ((uint8_t)0x03)     /* SPI4 Alternate Function mapping */
#define GPIO_AF_SPI5        ((uint8_t)0x04)     /* SPI5 Alternate Function mapping */

/*
 * @defgroup GPIO Alternate function HTIM definition
 */
#define GPIO_AF_HTIM0_M1    ((uint8_t)0x02)     /* HTIM0 Alternate Function mapping */
#define GPIO_AF_HTIM0_M2    ((uint8_t)0x03)     /* HTIM0 Alternate Function mapping */
#define GPIO_AF_HTIM1       ((uint8_t)0x02)     /* HTIM1 Alternate Function mapping */
#define GPIO_AF_HTIM2       ((uint8_t)0x03)     /* HTIM2 Alternate Function mapping */
#define GPIO_AF_HTIM5       ((uint8_t)0x03)     /* HTIM5 Alternate Function mapping */

/*
 * @defgroup GPIO Alternate function TIM definition
 */
#define GPIO_AF_TIM3        ((uint8_t)0x02)     /* TIM3 Alternate Function mapping */
#define GPIO_AF_TIM4        ((uint8_t)0x03)     /* TIM4 Alternate Function mapping */
#define GPIO_AF_TIM6_M1     ((uint8_t)0x02)     /* TIM6 Alternate Function mapping */
#define GPIO_AF_TIM6_M2     ((uint8_t)0x03)     /* TIM6 Alternate Function mapping */
#define GPIO_AF_TIM7        ((uint8_t)0x02)     /* TIM7 Alternate Function mapping */

/*
 * @defgroup GPIO multiplex interface MAC definition
 */
#define GPIO_AF_MAC         ((uint8_t)0x02)     /* MAC Alternate Function mapping */

/*
 * @defgroup GPIO multiplex interface FB definition
 */
#define GPIO_AF_FB          ((uint8_t)0x01)     /* FB Alternate Function mapping */

/*
 * @defgroup GPIO multiplex interface QSPI definition
 */
#define GPIO_AF_QSPI        ((uint8_t)0x01)     /* QSPI Alternate Function mapping */

/*
 * @defgroup GPIO multiplex interface FAULT definition
 */
#define GPIO_AF_FAULT       ((uint8_t)0x01)     /* FAULT Alternate Function mapping */

/*
 * @defgroup GPIO multiplex interface CLKOUT definition
 */
#define GPIO_AF_CLKOUT      ((uint8_t)0x02)     /* CLKOUT Alternate Function mapping */

/*
 * @defgroup GPIO multiplex interface JTAG definition
 */
#define GPIO_AF_JTAG        ((uint8_t)0x01)     /* JTAG Alternate Function mapping */

/*
 * @defgroup GPIO multiplex interface CMP definition
 */
#define GPIO_AF_CMP0_M1     ((uint8_t)0x02)     /* CMP0 Alternate Function mapping */
#define GPIO_AF_CMP0_M3     ((uint8_t)0x04)     /* CMP0 Alternate Function mapping */
#define GPIO_AF_CMP1_M1     ((uint8_t)0x02)     /* CMP1 Alternate Function mapping */
#define GPIO_AF_CMP1_M3     ((uint8_t)0x04)     /* CMP1 Alternate Function mapping */

#define IS_GPIO_AF(AF) (((AF) == GPIO_AF_Pin_x)    || ((AF) == GPIO_AF_USART0)   || \
                        ((AF) == GPIO_AF_USART1)   || ((AF) == GPIO_AF_USART2)   || \
                        ((AF) == GPIO_AF_USART3)   || ((AF) == GPIO_AF_USART4)   || \
                        ((AF) == GPIO_AF_USART5)   || ((AF) == GPIO_AF_USART6)   || \
                        ((AF) == GPIO_AF_USART7)   || ((AF) == GPIO_AF_CAN0_M1)  || \
                        ((AF) == GPIO_AF_CAN0_M3)  || ((AF) == GPIO_AF_CAN1)     || \
                        ((AF) == GPIO_AF_CAN2)     || ((AF) == GPIO_AF_CAN3)     || \
                        ((AF) == GPIO_AF_IIC0_M1)  || ((AF) == GPIO_AF_IIC0_M3)  || \
                        ((AF) == GPIO_AF_IIC1)     || ((AF) == GPIO_AF_IIC2)     || \
                        ((AF) == GPIO_AF_IIC3)     || ((AF) == GPIO_AF_SPI0)     || \
                        ((AF) == GPIO_AF_SPI1)     || ((AF) == GPIO_AF_SPI2)     || \
                        ((AF) == GPIO_AF_SPI3)     || ((AF) == GPIO_AF_SPI4)     || \
                        ((AF) == GPIO_AF_SPI5)     || ((AF) == GPIO_AF_HTIM0_M1) || \
                        ((AF) == GPIO_AF_HTIM0_M2) || ((AF) == GPIO_AF_HTIM1)    || \
                        ((AF) == GPIO_AF_HTIM2)    || ((AF) == GPIO_AF_HTIM5)    || \
                        ((AF) == GPIO_AF_TIM3)     || ((AF) == GPIO_AF_TIM4)     || \
                        ((AF) == GPIO_AF_TIM6_M1)  || ((AF) == GPIO_AF_TIM6_M2)  || \
                        ((AF) == GPIO_AF_TIM7)     || ((AF) == GPIO_AF_MAC)      || \
                        ((AF) == GPIO_AF_FB)       || ((AF) == GPIO_AF_QSPI)     || \
                        ((AF) == GPIO_AF_FAULT)    || ((AF) == GPIO_AF_CLKOUT)   || \
                        ((AF) == GPIO_AF_JTAG)     || ((AF) == GPIO_AF_CMP0_M1)  || \
                        ((AF) == GPIO_AF_CMP0_M3)  || ((AF) == GPIO_AF_CMP1_M1)  || \
                        ((AF) == GPIO_AF_CMP1_M3))

/*
 * @brief Function declaration
 */
void GPIO_StructInit(GPIO_InitTypeDef *GPIO_InitStruct);
void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct);
void GPIO_SetBits(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
void GPIO_ClearBits(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
void GPIO_ToggleBits(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
void GPIO_WriteBits(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, GPIOOCtrl_TypeDef OutState);
void GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t PortVal);
GPIOOCtrl_TypeDef GPIO_ReadOutputDataBit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
uint32_t GPIO_ReadOutputData(GPIO_TypeDef *GPIOx);
GPIOIRead_TypeDef GPIO_ReadInputDataBit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
uint32_t GPIO_ReadInputData(GPIO_TypeDef *GPIOx);
void GPIO_PinAFConfig(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource, uint8_t GPIO_AF);
void GPIO_ITConfig(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, GPIOITType_TypeDef ITType, FunctionalState NewState);
ITStatus GPIO_GetITStatus(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
void GPIO_ForceINT(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
void GPIO_ITMaskConfig(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, EnState NewState);
void GPIO_ClearITPendingBit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
