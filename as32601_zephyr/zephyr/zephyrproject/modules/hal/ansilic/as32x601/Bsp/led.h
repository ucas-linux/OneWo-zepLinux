/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LED_H
#define LED_H

/* Includes ------------------------------------------------------------------*/
#include "as32x601.h"
#include "as32x601_gpio.h"
#include "as32x601_smu.h"

/* Exported define -----------------------------------------------------------*/

#define LED1_ON()       GPIO_WriteBits(GPIOG, GPIO_Pin_15, GPIO_OCtrl_Vol)
#define LED1_OFF()      GPIO_WriteBits(GPIOG, GPIO_Pin_15, GPIO_OCtrl_Voh)
#define LED1_TOGGLE()   GPIO_ToggleBits(GPIOG, GPIO_Pin_15)
#define LED2_ON()       GPIO_WriteBits(GPIOG, GPIO_Pin_14, GPIO_OCtrl_Vol)
#define LED2_OFF()      GPIO_WriteBits(GPIOG, GPIO_Pin_14, GPIO_OCtrl_Voh)
#define LED2_TOGGLE()   GPIO_ToggleBits(GPIOG, GPIO_Pin_14)
#define LED3_ON()       GPIO_WriteBits(GPIOG, GPIO_Pin_10, GPIO_OCtrl_Vol)
#define LED3_OFF()      GPIO_WriteBits(GPIOG, GPIO_Pin_10, GPIO_OCtrl_Voh)
#define LED3_TOGGLE()   GPIO_ToggleBits(GPIOG, GPIO_Pin_10)

/* Exported functions ------------------------------------------------------- */
extern void User_LED_Init();

#endif

/*****END OF FILE*****/