#include "peripherals_init.h"
#include "stm32h7xx_gpio.h"   
#include "gpio_config.h"




void gpio_init ( void )
{
    /* GPIO configuration struct */
    gpio_configuration_t gpio_config;    
    
    /* LEDs GPIOs */
    gpio_config.port     = LED_1_PORT;
    gpio_config.pin      = LED_1_PIN;
    gpio_config.mode     = gpio_output;
    gpio_config.speed    = low_speed;
    gpio_config.otype    = gpio_otype_pp;
    gpio_config.pupd     = gpio_pupd_down;
    gpio_config.state    = gpio_set;
    gpio_configuration ( &gpio_config );
    

    
    
    /* Button  */
    gpio_config.port     = BUTTON_1_PORT;
    gpio_config.pin      = BUTTON_1_PIN;
    gpio_config.mode     = gpio_input;
    gpio_config.speed    = low_speed;
    gpio_config.otype    = gpio_otype_pp;
    gpio_config.pupd     = gpio_pupd_nopull;
    gpio_config.state    = gpio_set;
    gpio_configuration ( &gpio_config );   
    
    gpio_config.port     = BUTTON_2_PORT;
    gpio_config.pin      = BUTTON_2_PIN;
    gpio_config.mode     = gpio_input;
    gpio_config.speed    = low_speed;
    gpio_config.otype    = gpio_otype_pp;
    gpio_config.pupd     = gpio_pupd_nopull;
    gpio_config.state    = gpio_set;
    gpio_configuration ( &gpio_config );   


    /* SW USART GPIOs */
    gpio_config.port     = USART_SW_TX_PORT;
    gpio_config.pin      = USART_SW_TX_PIN;
    gpio_config.mode     = gpio_output;
    gpio_config.speed    = very_high_speed;
    gpio_config.otype    = gpio_otype_pp;
    gpio_config.pupd     = gpio_pupd_up;
    gpio_config.state    = gpio_set;
    gpio_configuration ( &gpio_config );
    
    
    gpio_config.port     = USART_SW_TX2_PORT;
    gpio_config.pin      = USART_SW_TX2_PIN;
    gpio_config.mode     = gpio_output;
    gpio_config.speed    = very_high_speed;
    gpio_config.otype    = gpio_otype_pp;
    gpio_config.pupd     = gpio_pupd_up;
    gpio_config.state    = gpio_set;
    gpio_configuration ( &gpio_config );
    
    
    /* USB GPIOs */
    gpio_config.port     = USB_DP_PORT;
    gpio_config.pin      = USB_DP_PIN;
    gpio_config.mode     = gpio_af;
    gpio_config.speed    = low_speed;
    gpio_config.otype    = gpio_otype_od;
    gpio_config.pupd     = gpio_pupd_nopull;
    gpio_configuration ( &gpio_config );
    USB_DP_PORT->AFR[1] |= (0x0A << GPIO_AFRH_AFSEL12_Pos); //AF = 10 (I2C1_SCL)
    
    
    gpio_config.port     = USB_DN_PORT;
    gpio_config.pin      = USB_DN_PIN;
    gpio_config.mode     = gpio_af;
    gpio_config.speed    = low_speed;
    gpio_config.otype    = gpio_otype_od;
    gpio_config.pupd     = gpio_pupd_nopull;
    gpio_configuration ( &gpio_config );
    USB_DN_PORT->AFR[1] |= (0x0A << GPIO_AFRH_AFSEL11_Pos); //AF = 10
    
    
    
}

    void init_status_update(void)
    {
        
    }