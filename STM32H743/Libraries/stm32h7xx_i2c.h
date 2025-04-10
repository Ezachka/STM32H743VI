#ifndef STM32H7XX_I2C_H
#define STM32H7XX_I2C_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "stm32h7xx.h"    
#include "stm32h743xx.h"


    typedef enum 
    {   
        not_transmitted = 0x00,
        not_recived     = 0x01,
        not_ask         = 0x02,
        transmitted     = 0x03,
        recived         = 0x04,
    } i2c_status_t;
    
    ErrorStatus i2c_init(I2C_TypeDef* I2C_x);
    void i2c_reset(I2C_TypeDef* I2C_x);
    void i2c_status_clear( I2C_TypeDef* I2C_x);
    void i2c_1_event_handler (void);
    void i2c_transmit_it(I2C_TypeDef* I2C_x,uint8_t addr, uint8_t *buff, uint8_t data_len);
    void i2c_recive_it(I2C_TypeDef* I2C_x,uint8_t addr, uint8_t *buff, uint8_t data_len);  
    
    
    i2c_status_t i2c_recive( I2C_TypeDef* I2C_x,uint8_t addr, uint8_t *buff, uint8_t data_len, uint32_t timeout_ms);
    i2c_status_t i2c_transmit( I2C_TypeDef* I2C_x,uint8_t addr, uint8_t *buff, uint8_t data_len, uint32_t timeout_ms);
    
    
    
    
    
#ifdef	__cplusplus
}
#endif

#endif 