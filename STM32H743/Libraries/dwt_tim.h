#ifndef __DWT_TIM_H__
#define __DWT_TIM_H__

#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32h7xx.h"


//#define DWT_CONTROL *(volatile unsigned long *)0xE0001000
//#define SCB_DEMCR   *(volatile unsigned long *)0xE000EDFC

/******************************************************************************/
/* inline func */
__STATIC_INLINE void dwt_init(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // разрешаем использовать счётчик
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;   // запускаем счётчик
}

__STATIC_INLINE void delay_us(uint32_t us)
{
	uint32_t us_count_tic =  us * (SystemCoreClock / 1000000U);
	DWT->CYCCNT = 0U;
	while(DWT->CYCCNT < us_count_tic);
}

__STATIC_INLINE void delay_sysclk(uint32_t sysclk)
{
	DWT->CYCCNT = 0U;
	while(DWT->CYCCNT < sysclk);
}

#ifdef __cplusplus
}
#endif
#endif //__DWT_TIM_H__