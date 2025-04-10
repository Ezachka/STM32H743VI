#include "stm32h7xx_gpio.h"


void gpio_configuration ( gpio_configuration_t* gpio)
{
    if ( gpio->port == GPIOA ){
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN;    //GPIO port A clock enable
    }
    else if ( gpio->port == GPIOB ){
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN;    //GPIO port B clock enable
    }
    else if ( gpio->port == GPIOC ){
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIOCEN;    //GPIO port C clock enable
    }
    else if ( gpio->port == GPIOD ){
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIODEN;    //GPIO port D clock enable
    }
    else if ( gpio->port == GPIOE ){
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIOEEN;    //GPIO port E clock enable
    }
    else if ( gpio->port == GPIOF ){
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIOFEN;    //GPIO port F clock enable
    }
    else if ( gpio->port == GPIOH ){
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIOHEN;    //GPIO port H clock enable
    }
    else if ( gpio->port == GPIOG ){
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIOGEN;    //GPIO port G clock enable
    }
    else if ( gpio->port == GPIOJ ){
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIOJEN;    //GPIO port J clock enable
    }
    else if ( gpio->port == GPIOK ){
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIOKEN;    //GPIO port K clock enable
    }
    
    
    for ( uint8_t pin_pos=0; pin_pos<0x10; pin_pos++ ){
        if ( (gpio->pin >> pin_pos) & 0x01 )
        {
            /* GPIO State set */
            (gpio->state == gpio_set) ? (gpio->port->ODR |=  (0x01<<pin_pos)):
                                        (gpio->port->ODR &= ~(0x01<<pin_pos));
            
            /* GPIO Mode set */
            gpio->port->MODER   &= ~( 0x03 << (pin_pos*2) );            //Clear bit
            gpio->port->MODER   |=  ( gpio->mode << (pin_pos*2) );      //Set bit
            
            /* GPIO Speed set */
            gpio->port->OSPEEDR &= ~( 0x03 << (pin_pos*2) );            //Clear bit
            gpio->port->OSPEEDR |=  ( gpio->speed << (pin_pos*2) );     //Set bit
            
            /* GPIO Output type set */
            gpio->port->OTYPER  &= ~( 0x03 << pin_pos );                //Clear bit
            gpio->port->OTYPER  |=  ( gpio->otype << pin_pos );         //Set bit
            
            /* GPIO Pull UP/Pull Down set */
            gpio->port->PUPDR   &= ~( 0x03 << (pin_pos*2) );            //Clear bit
            gpio->port->PUPDR   |=  ( gpio->pupd << (pin_pos*2) );      //Set bit
            
            if( gpio->mode == gpio_af )
            {
                if( pin_pos > 0x07 )
                {  
                }
                else
                {
                }
            } 
        }//.if
    }//.for 
}
