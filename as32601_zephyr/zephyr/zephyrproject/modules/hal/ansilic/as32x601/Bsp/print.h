/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PRINT_H
#define PRINT_H

/* Includes ------------------------------------------------------------------*/
#include "as32x601.h"
#include "as32x601_gpio.h"
#include "as32x601_smu.h"
#include "as32x601_usart.h"

/* Exported define -----------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
extern void User_Print_Init(uint32_t BaudRate);

#endif

/*****END OF FILE*****/