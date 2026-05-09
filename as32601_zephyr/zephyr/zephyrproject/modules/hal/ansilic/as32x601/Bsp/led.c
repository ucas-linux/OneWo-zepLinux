#include "led.h"


/*
 * Function:        User_LED_Init
 * Description:     Configure LED GPIO.
 * Param:           None.                 
 * Return:          None.
 */
void User_LED_Init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    GPIOG_CLK_ENABLE();
   
    /* GPIOB Configure */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15; 
    GPIO_InitStructure.GPIO_Mode      = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType     = GPIO_Out_PP;
    GPIO_InitStructure.GPIO_OStrength = GPIO_OStrength_9mA;
    
    GPIO_Init(GPIOG, &GPIO_InitStructure);  
}



