#ifndef STM32H7XX_TIM_H
#define STM32H7XX_TIM_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "stm32h743xx.h"

    #include "stm32h7xx.h"
    
    #define TIM2_PRESCALER       10//60
    #define TIM14_PRESCALER      24
    
    
    void tim2_init ( void );
    void tim2_disable( void );
    void tim2_enable( uint16_t value );


    void tim14_init ( void );
    void tim14_disable( void );
    void tim14_enable( uint16_t value );
    
#ifdef	__cplusplus
}
#endif

#endif //STM32H7XX_TIM_H
