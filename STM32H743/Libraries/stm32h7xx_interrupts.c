
#include "stm32h7xx_interrupts.h"
#include "stm32h7xx_usart.h"
#include "stm32h7xx_spi.h"
#include "softuart.h"
#include "gpio_config.h"
/**
* @brief  This function handles USART1 Handler.
* @param  None
* @retval None
*/
void USART1_IRQHandler ( void )
{
    usart_1_irq_handler();
}

/**
* @brief  This function handles USART2 Handler.
* @param  None
* @retval None
*/
void USART2_IRQHandler ( void )
{
    
}

/**
* @brief  This function handles USART3 Handler.
* @param  None
* @retval None
*/
void USART3_IRQHandler ( void )
{
    
}

/**
* @brief  This function handles UART4 Handler.
* @param  None
* @retval None
*/
void UART4_IRQHandler ( void )
{
    
}

/**
* @brief  This function handles UART5 Handler.
* @param  None
* @retval None
*/
void UART5_IRQHandler ( void )
{
    
}

/**
* @brief  This function handles LPUART1 Handler.
* @param  None
* @retval None
*/
void LPUART1_IRQHandler ( void )
{
    
}


/**
* @brief  This function handles External interrupt ch5-ch9 Handler.
* @param  None
* @retval None
*/
void EXTI9_5_IRQHandler ( void )
{
    
}


/**
* @brief  This function handles External interrupt ch15-ch10 Handler.
* @param  None
* @retval None
*/
void EXTI15_10_IRQHandler ( void )
{
    
}

/**
* @brief  
* @param  None
* @retval None
*/
volatile uint32_t tim2_counter=0;
void TIM2_IRQHandler ( void )
{
    TIM2->SR &= ~TIM_SR_UIF;    
    tim2_counter++;
    USART_SW_TX_PORT->ODR ^= USART_SW_TX_PIN;
    while( TIM2->SR & TIM_SR_UIF ){
        TIM2->SR &= ~TIM_SR_UIF;    
    }
    return;
}

/**
* @brief  
* @param  None
* @retval None
*/
void TIM8_TRG_COM_TIM14_IRQHandler ( void )
{

    TIM14->SR &= ~TIM_SR_UIF;    
    //SoftUartHandler();
    while( TIM14->SR & TIM_SR_UIF ){
        TIM14->SR &= ~TIM_SR_UIF;    
    }
 
    return;
}

/**
* @brief  This function returns the value of the Systick interrupt counter.
* @param  None
* @retval uint_32t microseconds
*/
uint32_t systick_get_us ( void )
{
    return tim2_counter;
}


/**
* @brief  
* @param  None
* @retval None
*/
void TIM3_IRQHandler ( void )
{
    
}

/**
* @brief  This function handles ADC 1_2 Handler.
* @param  None
* @retval None
*/
void ADC1_2_IRQHandler ( void )
{  
    
}


/**
* @brief  This function handles SPI1_IRQHandler.
* @param  None
* @retval None
*/
void SPI1_IRQHandler ( void )
{  
    spi_1_irq();
    return;
}

/**
* @brief  This function returns the value of the Systick interrupt counter.
* @param  None
* @retval uint_32t milliseconds
*/
volatile uint32_t systick_counter=0;
volatile uint32_t systick_counter_2=0;
uint32_t systick_get ( void )
{
    return systick_counter;
}

/**
* @brief  
* @param  
* @retval 
*/
void delay_ms (uint16_t ms)
{
    uint32_t waiting_time = systick_get() + ms; 
    while(waiting_time-systick_get())
    {
    }
}

/**
* @brief  This function handles SysTick Handler.
* @param  None
* @retval None
*/

volatile _Bool systick_f;
volatile _Bool systick_div_1hz;
volatile _Bool systick_div_5hz;
volatile _Bool systick_div_10hz;
volatile _Bool systick_div_100hz;
volatile _Bool systick_div_200hz;
volatile _Bool systick_div_500hz;

void SysTick_Handler ( void )
{
	systick_counter++;
    systick_counter_2++;
    systick_f = 1;
    
    if( ((systick_counter-1) % 1000) == 0 )      systick_div_1hz   = 1; 
    if( ((systick_counter-2) % 200)  == 0 )      systick_div_5hz   = 1; 
    if( ((systick_counter-3) % 100)  == 0 )      systick_div_10hz  = 1;
    if( ((systick_counter-4) % 10)   == 0 )      systick_div_100hz = 1;    
    if( ((systick_counter-5) % 5)   == 0 )       systick_div_200hz = 1;   
    if( ((systick_counter-6) % 2)   == 0 )       systick_div_500hz = 1;
}


/**
* @brief  This function handles Hard Fault exception.
* @param  None
* @retval None
*/
void HardFault_Handler ( void )
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
	}
}

/**
* @brief  This function handles Memory Manage exception.
* @param  None
* @retval None
*/
void MemManage_Handler ( void )
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
	{
	}
}

/**
* @brief  This function handles Bus Fault exception.
* @param  None
* @retval None
*/
void BusFault_Handler ( void )
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{
	}
}

/**
* @brief  This function handles Usage Fault exception.
* @param  None
* @retval None
*/
void UsageFault_Handler ( void )
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
	{
	}
}

/**
* @brief  This function handles SVCall exception.
* @param  None
* @retval None
*/
void SVC_Handler(void)
{
}

/**
* @brief  This function handles Debug Monitor exception.
* @param  None
* @retval None
*/
void DebugMon_Handler(void)
{
}

/**
* @brief  This function handles PendSVC exception.
* @param  None
* @retval None
*/
void PendSV_Handler(void)
{
}
