#include "print.h"

/*
 * Function:        User_Print_Init
 * Description:     Configure Print USART.
 * Param:           BaudRate: USART communication baud rate.                 
 * Return:          None.
 */
void User_Print_Init(uint32_t BaudRate)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIOD_CLK_ENABLE();
    USART0_CLK_ENABLE();
        
    /* Set GPIO multiplex mapping */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART0);       /* USART0_TX */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART0);       /* USART0_RX */
    /* GPIO Configure */
    GPIO_InitStructure.GPIO_Pin       = GPIO_Pin_8;             
    GPIO_InitStructure.GPIO_Mode      = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType     = GPIO_Out_PP;
    GPIO_InitStructure.GPIO_OStrength = GPIO_OStrength_4_5mA;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin       = GPIO_Pin_9;             
    GPIO_InitStructure.GPIO_Mode      = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_IType     = GPIO_IN_FLOATING;
    GPIO_InitStructure.GPIO_OStrength = GPIO_OStrength_4_5mA;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_DeInit(USART0);

    USART_StructInit(&USART_InitStructure);

    /* Initializes the USART0 */
    USART_InitStructure.USART_BaudRate     = BaudRate;
    USART_InitStructure.USART_WordLength   = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits     = USART_StopBits_1;
    USART_InitStructure.USART_Parity       = USART_Parity_No;
    USART_InitStructure.USART_Mode         = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_OverSampling = USART_OverSampling_16;
    USART_Init(USART0, &USART_InitStructure);

    USART_Cmd(USART0, ENABLE);
}


#undef putchar   /* picolibc/newlib defines putchar as a macro; undefine to allow function override */
int putchar(int ch)
{     
  /* Send a character. */     
  USART0->TDR = (uint8_t)ch;      

  /* Wait for UART to be ready to send. */
  while(USART_GetFlagStatus(USART0, USART_FLAG_TC) == RESET);  
  
  return ch; 
} 



