#ifndef LEDS_H
#define LEDS_H

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "stm32h7xx.h"
#include "stm32h743xx.h"


    ErrorStatus led_init( void );
    void leds_update ( void );
    void led_1_blink_set( uint16_t blink_period );
    void led_2_blink_set( uint16_t blink_period );
    void led_3_blink_set( uint16_t blink_period );
    
#ifdef __cplusplus
}
#endif

#endif /*LEDS_H */