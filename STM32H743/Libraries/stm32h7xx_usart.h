#ifndef STM32H7XX_UART_H
#define STM32H7XX_UART_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "stm32h7xx.h"
#include "stm32h743xx.h"
 
    
#define SCB_RX_BUFF_LEN     (uint8_t) 128
#define SCB_TX_BUFF_LEN     (uint8_t) 64
    
    typedef enum
    {
        even_parity = 0x00,
        odd_parity = 0x01,
        none_parity
    }usart_parity_mode_t;   
    typedef enum
    {
        none_swap = 0x00,
        swap = 0x01
    }usart_swap_pin_t;   
    
    ErrorStatus usart_init ( USART_TypeDef* USART_x, uint32_t baund_rate,usart_parity_mode_t parity,usart_swap_pin_t swap);
    
    uint8_t usart_receive ( USART_TypeDef* USART_x );
	void usart_transmit_byte ( USART_TypeDef* USART_x, void *data );
	void usart_transmit ( USART_TypeDef* USART_x, void *data_buff, uint16_t data_len );
    
    uint8_t uart_read_available( USART_TypeDef* USART_x );
    uint8_t usart_read_byte( USART_TypeDef* USART_x );
    void usart_send_byte( USART_TypeDef* USART_x, uint8_t byte );
    void usart_send_data( USART_TypeDef* USART_x, uint8_t* data, uint8_t size );
    void usart_clear_tx_buf(  USART_TypeDef* USART_x );
    void usart_clear_rx_buf( USART_TypeDef* USART_x );
    
    void usart_1_irq_handler    ( void );
    void usart_2_irq_handler    ( void );
    void usart_3_irq_handler    ( void );
    void usart_6_irq_handler    ( void );
    void uart_4_irq_handler     ( void );
    void uart_5_irq_handler     ( void );
    void uart_7_irq_handler     ( void );
    void uart_8_irq_handler     ( void );
    void lpuart_1_irq_handler   ( void );
    
#ifdef	__cplusplus
}
#endif

#endif //STM32H7XX_UART_H
