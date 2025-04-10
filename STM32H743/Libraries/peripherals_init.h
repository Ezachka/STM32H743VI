#ifndef PERIPHERALS_INIT_H
#define PERIPHERALS_INIT_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "stm32h7xx.h"
#include "stm32h743xx.h"
 
    typedef struct 
    {
        ErrorStatus rcc_init_status;

        struct{
            ErrorStatus gpio_a;
            ErrorStatus gpio_b;
            ErrorStatus gpio_c;
            ErrorStatus gpio_d;
            ErrorStatus gpio_e;
            ErrorStatus gpio_f;
            ErrorStatus gpio_g;
        }gpio_init_status;
        struct{
            ErrorStatus i2c_1;
            ErrorStatus i2c_2;
            ErrorStatus i2c_3;
            ErrorStatus i2c_4;
            ErrorStatus i2c_5;
            ErrorStatus i2c_6;
        }i2c_init_status;
        struct{
            ErrorStatus spi_1;
            ErrorStatus spi_2;
            ErrorStatus spi_3;
            ErrorStatus spi_4;
            ErrorStatus spi_5;
            ErrorStatus spi_6;
        }spi_init_status;   
        struct{
            ErrorStatus usart_1;
            ErrorStatus usart_2;
            ErrorStatus usart_3;
            ErrorStatus usart_4;
            ErrorStatus usart_5;
            ErrorStatus usart_6;
        }usart_init_status;    
        
    }init_status_t;
    
    void gpio_init ( void );
    
    void init_status_update(void);
    
#ifdef	__cplusplus
}
#endif

#endif //PERIPHERALS_INIT_H