#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "stm32h7xx.h"
    
    
    /* GPIO PIN SOURCE MACROS*/
#define GPIO_PIN_0      (uint32_t)(0x01UL<<0x00U)
#define GPIO_PIN_1      (uint32_t)(0x01UL<<0x01U)
#define GPIO_PIN_2      (uint32_t)(0x01UL<<0x02U)
#define GPIO_PIN_3      (uint32_t)(0x01UL<<0x03U)
#define GPIO_PIN_4      (uint32_t)(0x01UL<<0x04U)
#define GPIO_PIN_5      (uint32_t)(0x01UL<<0x05U)
#define GPIO_PIN_6      (uint32_t)(0x01UL<<0x06U)
#define GPIO_PIN_7      (uint32_t)(0x01UL<<0x07U)
#define GPIO_PIN_8      (uint32_t)(0x01UL<<0x08U)
#define GPIO_PIN_9      (uint32_t)(0x01UL<<0x09U)
#define GPIO_PIN_10     (uint32_t)(0x01UL<<0x0AU)
#define GPIO_PIN_11     (uint32_t)(0x01UL<<0x0BU)
#define GPIO_PIN_12     (uint32_t)(0x01UL<<0x0CU)   
#define GPIO_PIN_13     (uint32_t)(0x01UL<<0x0DU)
#define GPIO_PIN_14     (uint32_t)(0x01UL<<0x0EU)
#define GPIO_PIN_15     (uint32_t)(0x01UL<<0x0FU)
    
    /* LEDs GPIOs */ 
#define LED_1_PORT          GPIOA      
#define LED_1_PIN           GPIO_PIN_1   
    

    /* BUTTONs GPIOs */ 
#define BUTTON_1_PORT         GPIOE
#define BUTTON_1_PIN          GPIO_PIN_3
    
#define BUTTON_2_PORT         GPIOC
#define BUTTON_2_PIN          GPIO_PIN_5
    
    /* I2C GPIOs */ 
#define I2C1_SCL_PORT       GPIOB
#define I2C1_SCL_PIN        GPIO_PIN_8
#define I2C1_SDA_PORT       GPIOB
#define I2C1_SDA_PIN        GPIO_PIN_9
    /* SPI GPIOs */  
#define SPI1_MOSI_PORT      GPIOB
#define SPI1_MOSI_PIN       GPIO_PIN_5
#define SPI1_MISO_PORT      GPIOA
#define SPI1_MISO_PIN       GPIO_PIN_6
#define SPI1_CLK_PORT       GPIOA
#define SPI1_CLK_PIN        GPIO_PIN_5
#define SPI1_CS_PORT        GPIOD
#define SPI1_CS_PIN         GPIO_PIN_14
    
    
    /* SW TX USART GPIOs */  
#define USART_SW_TX_PORT    GPIOG  
#define USART_SW_TX_PIN     GPIO_PIN_12  
    
#define USART_SW_TX2_PORT   GPIOE  
#define USART_SW_TX2_PIN    GPIO_PIN_11  
    /* USART GPIOs */ 
#define USART_1_TX_PORT     GPIOB
#define USART_1_TX_PIN      GPIO_PIN_6
#define USART_1_RX_PORT     GPIOB
#define USART_1_RX_PIN      GPIO_PIN_7
    
#ifdef __cplusplus
}
#endif 
#endif /*GPIO_CONFIG_H */