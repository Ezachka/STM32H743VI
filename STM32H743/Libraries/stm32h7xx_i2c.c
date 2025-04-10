#include "stm32h7xx_i2c.h"
#include "stm32h7xx_gpio.h"
#include "gpio_config.h"
#include "stm32h7xx_interrupts.h"
#include <stdbool.h>
#include <string.h>
/******************************* Variables ************************************/

uint8_t     i2c1_tx_buf[10];
uint8_t     i2c1_tx_buf_p =0;
uint8_t 	i2c1_rx_buf[10];
uint8_t     i2c1_rx_buf_p =0;

uint8_t     i2c_on_board[10];

void i2c_pin_config(I2C_TypeDef* I2C_x) ;
uint8_t i2c_scan( I2C_TypeDef* I2C_x);

/******************************* FUNCTIONS ************************************/

/**
* @brief i2c_init
*  Функция инициализации I2C, параметры инициализации :
*  скорость 100кГц, при 80МГц тактовом сигнале SYSCLK, без проерываний
* 
* @param[in] I2C_TypeDef* I2C_x    - используемый I2C
* @return 
*/
ErrorStatus i2c_init(I2C_TypeDef* I2C_x)
{    
    if( I2C_x == I2C1 )
    {
        RCC->APB1LENR |= RCC_APB1LENR_I2C1EN;
        RCC->D2CCIP2R   |=(0x02 <<RCC_D2CCIP2R_I2C123SEL_Pos); // Тактируем i2c от HSI
    }
    else if ( I2C_x == I2C2 )
    {
        RCC->APB1LENR |= RCC_APB1LENR_I2C2EN;
        RCC->D2CCIP2R   |=(0x02 <<RCC_D2CCIP2R_I2C123SEL_Pos); // Тактируем i2c от HSI
    }
    else if ( I2C_x == I2C3 )
    {
        RCC->APB1LENR |= RCC_APB1LENR_I2C3EN;
        RCC->D2CCIP2R   |=(0x02 <<RCC_D2CCIP2R_I2C123SEL_Pos); // Тактируем i2c от HSI
    } 
    else if ( I2C_x == I2C4 )
    {
        RCC->APB4ENR |= RCC_APB4ENR_I2C4EN;
        RCC->D3CCIPR   |=(0x02 <<RCC_D3CCIPR_I2C4SEL_Pos);   // Тактируем i2c от HSI
    } 
    
    i2c_pin_config(I2C_x);     
    
    I2C_x->CR1 &= ~(I2C_CR1_PE);
    
    I2C_x->TIMINGR =    0x10707DBC;// timing 100KHz if 64MHZ hsi in
 
    I2C_x->CR1  &= ~(I2C_CR1_NOSTRETCH); // растяжение клока  в мастер режиме 0 
    I2C_x->CR2  &= ~(I2C_CR2_ADD10);     // адресация 7 бит, а не 10
    I2C_x->OAR1 &= ~(I2C_OAR1_OA1EN);    // нет адресов подчиненного,  тк мы в мастер моде
    I2C_x->OAR2 &= ~(I2C_OAR2_OA2EN);     
    
    I2C_x->CR1 |= I2C_CR1_PE;             //Peripheral enable включили i2c
    
    if( I2C_x == I2C1 )
    {
        i2c_scan(I2C1) ;
    }
    else if ( I2C_x == I2C2 )
    {
     //   i2c_scan(I2C2) ;
    }
    else if ( I2C_x == I2C3 )
    {
     //   i2c_scan(I2C3) ;
    } 
        else if ( I2C_x == I2C4 )
    {
     //   i2c_scan(I2C4) ;
    } 
        return SUCCESS;
}


/**
* @brief i2c_scan
*  скан линии i2c
* 
* @param[in] I2C_TypeDef* I2C_x    - используемый I2C
* @param[in] 
* @param[in] 
* @param[in] 
* @param[out] i2c_status_t         - Возвращает последний адрес
* @return 
*/

uint8_t i2c_scan(I2C_TypeDef* I2C_x)  
{
    uint8_t adr=0;
    static uint8_t i  = 0;
    uint8_t data[1]={0x00};
    for(adr =0;adr<0b01111111;adr++)
    {
        if(i2c_transmit(I2C_x,adr, data,1,1) == transmitted)
        {
            i2c_on_board[i] = adr;
            i++;
        }
    }
    i2c_on_board[i] = 0xff;
    i++;
    return adr;
}

/**
* @brief i2c_transmit
*  Функция передачи определенного кол-ва байт по определенному адресу, не более 255 байт!!!!
* 
* @param[in] I2C_TypeDef* I2C_x    - используемый I2C
* @param[in] uint8_t addr          - адрес подчиненного устройства
* @param[in] uint8_t *buff         - буфер передаваемых данных
* @param[in] uint8_t data_len      - длина передаваемых данных
* @param[out] i2c_status_t         - Возвращает статус прогресса передачи данных
* @return 
*/

i2c_status_t i2c_transmit( I2C_TypeDef* I2C_x,uint8_t addr, uint8_t *buff, uint8_t data_len, uint32_t timeout_ms)  // отлажен
{
    uint32_t timeout_counter=0;
    
    I2C_x->ICR |=  (I2C_ICR_NACKCF);//очищаем nack, это важно, иначе не сработает защита от записи в TXDR  при неответе слейва
    I2C_x->CR2 &= ~(I2C_CR2_NBYTES); 
    I2C_x->CR2 |=  (data_len<<I2C_CR2_NBYTES_Pos); // записываем кол-во байт
    I2C_x->CR2 |=  (I2C_CR2_AUTOEND);
    I2C_x->CR2 &= ~(I2C_CR2_SADD);
    I2C_x->CR2 |=  ((addr<<1)<<I2C_CR2_SADD_Pos);  // адрес 
    I2C_x->CR2 &= ~(I2C_CR2_RD_WRN); // направление передачи на запись
    I2C_x->CR2 |=  (I2C_CR2_START);
    
    for(uint8_t i=0;i<data_len;i++)
    {
        timeout_counter = systick_get() + timeout_ms;
        while (!(I2C_x->ISR & I2C_ISR_TXIS)) 
        {
            if (timeout_counter<systick_get()) {
                break;
            }
        }
        
        if(I2C_x->ISR & I2C_ISR_NACKF)
        {
            return not_ask;
        }
        
        if(I2C_x->ISR & I2C_ISR_TXE && !(I2C_x->ISR & I2C_ISR_NACKF))  
        {
            I2C_x->TXDR = buff[i];
            if(i == (data_len-1))
            {
                return transmitted;
            }
        }
        
    }
    
    return transmitted;
    
}
/**
* @brief i2c_recive
*  Функция получения определенного кол-ва байт по определенному адрес , не более 255 байт!!!!
* 
* 
*
* @param[in] I2C_TypeDef* I2C_x    - используемый I2C
* @param[in] uint8_t addr          - адрес подчиненного устройства
* @param[in] uint8_t *buff         - буфер получаемых данных
* @param[in] uint8_t data_len      - длина получаемых данных
* @param[out] i2c_status_t         - Возвращает статус прогресса получения данных
* @return 
*/

i2c_status_t i2c_recive( I2C_TypeDef* I2C_x,uint8_t addr, uint8_t *buff, uint8_t data_len, uint32_t timeout_ms)  
{   
    uint32_t timeout_counter=0;
    
    while((I2C_x->ISR & I2C_ISR_TXE) ) 
    {
        i2c_status_clear(I2C_x);
        I2C_x->CR2 &= ~(I2C_CR2_NBYTES);
        I2C_x->CR2 |=  (data_len<<I2C_CR2_NBYTES_Pos);
        I2C_x->CR2 |=  (I2C_CR2_AUTOEND);
        I2C_x->CR2 &= ~(I2C_CR2_SADD);
        I2C_x->CR2 |=  ((addr<<1)<<I2C_CR2_SADD_Pos);   
        I2C_x->CR2 |=  (I2C_CR2_RD_WRN); // направление передачи на чтение
        I2C_x->CR2 |=  (I2C_CR2_START);
        
        for (uint8_t i =0;i<data_len;i++)
        {  
            if(I2C_x->ISR & I2C_ISR_NACKF)
            {
                return not_ask;
            }
            timeout_counter = systick_get() + timeout_ms;
            while (!(I2C_x->ISR & I2C_ISR_RXNE)) 
            {
                if (timeout_counter<systick_get()) {
                    return not_recived;
                }
            }
            if(I2C_x->ISR & I2C_ISR_RXNE)
            {
                buff[i] = I2C_x->RXDR;
                if(i == (data_len-1))
                {
                    if(!(I2C_x->ISR & I2C_ISR_TC))
                    {
                        return recived;
                    }
                }
            }
        }
    }   
    if(!(I2C_x->ISR & I2C_ISR_TC))
    {
        return recived;
    }
    return not_recived;
}

/**
* @brief i2c_transmit_it
*  Функция начала передачи определенного кол-ва байт по определенному адресу, используя прерывания 
* 
* @TODO не проверялась на работоспособность
*
* @param[in] I2C_TypeDef* I2C_x    - используемый I2C
* @param[in] uint8_t addr          - адрес подчиненного устройства
* @param[in] uint8_t *buff         - буфер передаваемых данных
* @param[in] uint8_t data_len      - длина передаваемых данных
* @return 
*/

void i2c_transmit_it( I2C_TypeDef* I2C_x,uint8_t addr, uint8_t *buff, uint8_t data_len) //не отлажен
{
    while(!(I2C_x->ISR & I2C_ISR_BUSY) && (I2C_x->ISR & I2C_ISR_TXE)) 
    {
        i2c1_tx_buf_p=0;
        memset(&i2c1_tx_buf,0x00,10);
        memcpy(&i2c1_tx_buf,buff,data_len);
        uint8_t temp =  I2C_x->TXDR;
        // i2c1_tx_buf
        I2C_x->CR2 &= ~(I2C_CR2_NBYTES);
        I2C_x->CR2 |=  (data_len<<I2C_CR2_NBYTES_Pos);
        // I2C_x->CR2 |=  (I2C_CR2_AUTOEND);
        I2C_x->CR2 &= ~(I2C_CR2_SADD);
        I2C_x->CR2 |=  ((addr<<1)<<I2C_CR2_SADD_Pos);   
        I2C_x->CR2 &= ~(I2C_CR2_RD_WRN); // master tx direction
        I2C_x->CR2 |=  (I2C_CR2_START);
        break;
    }
}
/**
* @brief i2c_recive_it
*  Функция начала приема определенного кол-ва байт по определенному адресу, используя прерывания 
* 
* @TODO не проверялась на работоспособность
*
* @param[in] I2C_TypeDef* I2C_x    - используемый I2C
* @param[in] uint8_t addr          - адрес подчиненного устройства
* @param[in] uint8_t *buff         - буфер принимаемых данных
* @param[in] uint8_t data_len      - длина принимаемых данных
* @return 
*/
void i2c_recive_it( I2C_TypeDef* I2C_x,uint8_t addr, uint8_t *buff, uint8_t data_len)  // не отлажен
{
    while(!(I2C_x->ISR & I2C_ISR_BUSY) && (I2C_x->ISR & I2C_ISR_TXE)) 
    {
        I2C_x->CR2 &= ~(I2C_CR2_NBYTES);
        I2C_x->CR2 |=  (data_len<<I2C_CR2_NBYTES_Pos);
        I2C_x->CR2 |=  (I2C_CR2_AUTOEND);
        I2C_x->CR2 &= ~(I2C_CR2_SADD);
        I2C_x->CR2 |=  ((addr<<1)<<I2C_CR2_SADD_Pos);
        
        I2C_x->CR2 |=  (I2C_CR2_RD_WRN); // master rx direction
        
        I2C_x->RXDR = 0x00;
        
        I2C_x->CR2 |=  (I2C_CR2_START);
    }
}

/**
* @brief i2c_1_event_handler
*  Функция обработчика прерываний I2C1
*   @TODO не проверялась на работоспособность
*   Не используется
* @param[in] 
* @return 
*/
void i2c_1_event_handler (void)
{
    
    if (I2C1->ISR & I2C_ISR_TXIS)       //ready to tx
    {
        I2C1->TXDR = i2c1_tx_buf[i2c1_tx_buf_p];      
        i2c1_tx_buf_p++;
    }
    if (I2C1->ISR & I2C_ISR_TC)         //tx complete
    {      
        I2C1->CR2 |=  (I2C_CR2_STOP);
    }
    if (I2C1->ISR & I2C_ISR_RXNE)
    {
        i2c1_rx_buf[i2c1_rx_buf_p] = I2C1->RXDR;
        i2c1_rx_buf_p = i2c1_rx_buf_p + 1;
        if(i2c1_rx_buf_p > 10)
        {
            i2c1_rx_buf_p = 0;
        }
    }
}

/**
* @brief i2c_reset
*  Функция аппаоатной перезагрузки выранного I2C
*  
* 
* @param[in] I2C_TypeDef* I2C_x    - используемый I2C
* @return 
*/
void i2c_reset( I2C_TypeDef* I2C_x)
{
    I2C_x->CR1 &= ~(I2C_CR1_PE);
    while (I2C_x->CR1 & I2C_CR1_PE) {}
    I2C_x->CR1 |=  I2C_CR1_PE;
}

/**
* @brief i2c_status_clear
*  Функция очистки всех флагов статусов
*  
* 
* @param[in] I2C_TypeDef* I2C_x    - используемый I2C
* @return 
*/
void i2c_status_clear( I2C_TypeDef* I2C_x)
{
    I2C_x->ICR |= I2C_ICR_ADDRCF;
    I2C_x->ICR |= I2C_ICR_NACKCF;
    I2C_x->ICR |= I2C_ICR_STOPCF;
    
    I2C_x->ICR |= I2C_ICR_BERRCF;
    I2C_x->ICR |= I2C_ICR_ARLOCF;
    I2C_x->ICR |= I2C_ICR_OVRCF;
    I2C_x->ICR |= I2C_ICR_PECCF;
    I2C_x->ICR |= I2C_ICR_TIMOUTCF;
    I2C_x->ICR |= I2C_ICR_ALERTCF;
}


/**
* @brief i2c_pin_config
*  Функция инициализации GPIO пинов для I2C
*  
* 
* @param[in] I2C_TypeDef* I2C_x    - используемый I2C
* @return 
*/
void i2c_pin_config(I2C_TypeDef* I2C_x) 
{
    /* GPIO configuration struct */
    gpio_configuration_t gpio_config;
    
    /**** I2C gpio setting ****/
    if( I2C_x == I2C1 ){
        //I2C1_SCL - PB8 
        gpio_config.port     = I2C1_SCL_PORT;
        gpio_config.pin      = I2C1_SCL_PIN;
        gpio_config.mode     = gpio_af;
        gpio_config.speed    = medium_speed;
        gpio_config.otype    = gpio_otype_od;
        gpio_config.pupd     = gpio_pupd_nopull;
        gpio_configuration ( &gpio_config );
        I2C1_SCL_PORT->AFR[1] |= (0x04 << GPIO_AFRH_AFSEL8_Pos); //AF = 4 =0b0100 (I2C1_SCL)
        
        //I2C1_SDA - PB9
        gpio_config.port     = I2C1_SDA_PORT;
        gpio_config.pin      = I2C1_SDA_PIN;
        gpio_config.mode     = gpio_af;
        gpio_config.speed    = medium_speed;
        gpio_config.otype    = gpio_otype_od;
        gpio_config.pupd     = gpio_pupd_nopull;
        gpio_configuration ( &gpio_config );
        I2C1_SDA_PORT->AFR[1] |= (0x04 << GPIO_AFRH_AFSEL9_Pos); //AF = 4 =0b0100 (I2C1_SDA)    
    }
    
    
    if( I2C_x == I2C2 ){

    }
    
}


/**
* @}
*/






