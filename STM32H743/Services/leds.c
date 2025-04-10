#include "leds.h"
#include "stm32h7xx_interrupts.h"
#include "gpio_config.h"

uint16_t blink_1_half_period;

uint16_t blink_2_third_period;

uint16_t blink_3_quarter_period;

void led_1_update( void );
void led_2_update( void );
void led_3_update( void );

ErrorStatus led_init( void )
{
    led_1_blink_set ( 1000 );   
    return SUCCESS;
}


void leds_update ( void )
{
    led_1_update();
}


void led_1_blink_set( uint16_t blink_period )
{
    blink_1_half_period = blink_period/2;
}
void led_2_blink_set( uint16_t blink_period )
{
    blink_2_third_period = blink_period/3;
}
void led_3_blink_set( uint16_t blink_period )
{
    blink_3_quarter_period = blink_period/4;
}


void led_1_update( void )
{
    static uint32_t led_1_timer;
    
    
    if( blink_1_half_period == 0x00 )
        return;
    
    if( systick_get() > led_1_timer )
    {
        led_1_timer = systick_get() + blink_1_half_period;
        
        LED_1_PORT->ODR ^= LED_1_PIN;
    } 
}


