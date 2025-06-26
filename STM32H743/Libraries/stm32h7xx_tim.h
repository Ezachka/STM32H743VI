#ifndef STM32H7XX_TIM_H
#define STM32H7XX_TIM_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "stm32h743xx.h"
    
#include "stm32h7xx.h"
#include <string.h>
#include <stdbool.h>  
    
#define PWM_FREQUANCY       1000   //20kHz
#define PWM_PRESCALER       8
#define BUS_FREQUANCY       80000000UL
#define PWM_TIMER_PERIOD    (BUS_FREQUANCY/PWM_PRESCALER)/PWM_FREQUANCY  
    
    
#define TIM7_PRESCALER      40
    
    
    void tim_pwm_set (TIM_TypeDef* TIMx,uint8_t channel, float pwm_duty_cycle, float pwm_frequency);  
    void tim1_init ( void );
    void tim2_init ( void );
    void tim7_init ( void );
    void tim7_disable( void );
    void tim7_enable( uint16_t value );
    
    
    
#ifdef	__cplusplus
}
#endif

#endif //STM32H7XX_TIM_H
