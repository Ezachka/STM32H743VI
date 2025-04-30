#include "stm32h7xx_spi.h"
#include "stm32h7xx_gpio.h"
#include <string.h>
#include "gpio_config.h"
#include "stm32h7xx_interrupts.h"

/******************************* FUNCTIONS DECLARATION ************************************/
void reset_spi_errors(SPI_TypeDef* SPI_x);
   
void spi_pin_config(SPI_TypeDef* SPI_x) ;

void spi_source_config(SPI_TypeDef* SPI_x);

ErrorStatus spi_mbr_config(SPI_TypeDef* SPI_x);
ErrorStatus spi_mbr(SPI_TypeDef* SPI_x,uint16_t presc);

ErrorStatus spi_data_size_config(SPI_TypeDef* SPI_x);
ErrorStatus spi_data_size(SPI_TypeDef* SPI_x,uint16_t size);

ErrorStatus spi_datas_in_fifo_config(SPI_TypeDef* SPI_x);
ErrorStatus spi_datas_in_fifo(SPI_TypeDef* SPI_x,uint16_t in_fifo_size);

void spi_master_slave_config(SPI_TypeDef* SPI_x);

void spi_communication_mode_config(SPI_TypeDef* SPI_x);

void spi_polarity_config(SPI_TypeDef* SPI_x);
void spi_phase_config(SPI_TypeDef* SPI_x);

void spi_enable_config(SPI_TypeDef* SPI_x);


spi_info_t spi_1_info;
/******************************* SPI CONFIG ************************************/

spi_config_t spi_config = {
    
    /* SPI1 */
//    .spi_mode.spi_1=master,                             //режим мастер или слейв
//    
//    .spi_communication_mode.spi_1 = simplex_receiver,     //режим работы
//    
//    .spi_cpol.spi_1 = cpol_1,                           //стартовый уровень CLK высокий или низкий
//    
//    .spi_cpha.spi_1 = second,                           //фаза 
//    
//    .spi_data_size.spi_1=32,                            //используемых бит в буфере `
//    
//    .spi_datas_in_one_fifo.spi_1=1,                     //количество 
//
//    .spi_1_2_3_source = per_ck,                         //источник тактирования 
    //    
    //    .spi_mbr_presc.spi_1=16                             //предделитель 
    .spi_mode.spi_1=master,                             //режим мастер или слейв
    
    .spi_communication_mode.spi_1 = simplex_transmitter,     //режим работы
    
    .spi_cpol.spi_1 = cpol_1,                           //стартовый уровень CLK высокий или низкий
    
    .spi_cpha.spi_1 = second,                           //фаза 
    
    .spi_data_size.spi_1=8,                            //используемых бит в буфере `
    
    .spi_datas_in_one_fifo.spi_1=8,                     //количество 
    
    .spi_1_2_3_source = pll_1_q,                         //источник тактирования 
    
    .spi_mbr_presc.spi_1=256                             //предделитель 
        /* SPI2 */
        
        /* SPI3 */
        
        /* SPI4 */
        
        /* SPI5 */
        
        /* SPI6 */
        
};

/******************************* FUNCTIONS ************************************/

/**
* @brief spi_init
*  Функция инициализации SPI
*  
* 
* @param[in] SPI_TypeDef* SPI_x    - используемый SPI
* @return 
*/
ErrorStatus spi_init(SPI_TypeDef* SPI_x)
{    
    /* SPI  config */
    reset_spi_errors(SPI_x);
    
    spi_source_config(SPI_x);
    
    spi_pin_config(SPI_x);
    
    
    SPI_x->CR1 &= ~SPI_CR1_TCRCINI;      //CRC zero pattern
    SPI_x->CR1 &= ~SPI_CR1_CRC33_17;     //polynom not used
    SPI_x->CR1 &= ~I2C_CR1_TXDMAEN;     // tx DMA disable
    SPI_x->CR1 |=  SPI_CR1_SSI;		    // Internal slave select high level (always)  
    
    if(spi_mbr_config(SPI_x) == ERROR)
    {
        return ERROR;
    }
    if(spi_data_size_config(SPI_x) == ERROR)
    {
        return ERROR;
    }
    if(spi_datas_in_fifo_config(SPI_x) == ERROR)
    {
        return ERROR;
    }
    
    
    SPI_x->CFG1 &= ~SPI_CFG1_CRCEN;      // CRC disable
    SPI_x->CFG1 &= ~SPI_CFG1_CRCSIZE;    // CRCSIZE = 0    
    SPI_x->CFG1 &= ~SPI_CFG1_RXDMAEN;    // rx DMA disable
    SPI_x->CFG1 &= ~SPI_CFG1_TXDMAEN;    // tx DMA disable
    SPI_x->CFG1 &= ~SPI_CFG1_FTHLV;      // fifo threshold level = 1-data
    
    spi_phase_config(SPI_x);
    spi_polarity_config(SPI_x);
    
    SPI_x->CFG2 |=  SPI_CFG2_SSOE;		 // Not multimaster mode
    SPI_x->CFG2 |=  (0x00 <<SPI_CFG2_SP_Pos);   // Motorola mode
    SPI_x->CFG2 &= ~SPI_CFG2_LSBFRST;    // MSB first      
    SPI_x->CFG2 |=  SPI_CFG2_SSM;        // Enable software SS control
    SPI_x->CFG2 |=  (0x00 <<SPI_CFG2_MIDI_Pos); // inter data idleness
    SPI_x->CFG2 |=  (0x00 <<SPI_CFG2_MSSI_Pos); // ss idleness 
    
    spi_communication_mode_config(SPI_x);
    spi_master_slave_config(SPI_x);
    
    ///@TODO Move it in own func
    if(SPI_x==SPI1){
    SPI1->IER |=    SPI_IER_OVRIE | SPI_IER_UDRIE ;//|
             //       SPI_IER_RXPIE | SPI_IER_TXPIE |
             //       SPI_IER_EOTIE;
    
    NVIC_EnableIRQ( SPI1_IRQn );   
    }
    ///
    spi_enable_config(SPI_x);
    
    return SUCCESS;
}


/**
* @brief spi_1_irq
*  
* 
* @param[in] 
* @return 
*/

void spi_1_irq(void){
    
    if(SPI1 -> SR & SPI_SR_OVR){
        SPI1 -> IFCR |= SPI_IFCR_OVRC;
        spi_1_info.it_counts.OVR++;
        return;
    }
    if(SPI1 -> SR & SPI_SR_UDR){
        SPI1 -> IFCR |= SPI_IFCR_UDRC;
        spi_1_info.it_counts.UDR++;
        return;
    }
    if(SPI1 -> SR & SPI_SR_TXP){
        spi_1_info.it_counts.TXP++;
        return;
    }
    if(SPI1 -> SR & SPI_SR_RXP){
        spi_1_info.it_counts.RXP++;
        return;
    }
    if(SPI1 -> SR & SPI_SR_EOT){
        spi_1_info.it_counts.EOT++;
        return;
    }
    return;
}

/**
* @brief spi_transmit
*  Функция передачи данных по SPI   8 бит
*  
* 
* @param[in] SPI_TypeDef* SPI_x    - используемый SPI
* @return 
*/

void spi_transmit(SPI_TypeDef *SPI_x,uint8_t *buff,uint16_t data_sz,uint32_t timeout_ms)
{
    reset_spi_errors(SPI_x);
    
    uint32_t timeout_counter=0;
    if(data_sz<=0 || data_sz >0x1fff)
    {
        return;
    } 
    uint8_t i=0; 
    
    SPI_x->CR2 &=   ~(SPI_CR2_TSIZE_Msk); // Clear TSIZE (it not clear after rewraiting)

    SPI_x->CR2 |=   (data_sz << SPI_CR2_TSIZE_Pos); // set dsize

    SPI_x->CR1 |=   SPI_CR1_CSTART; //start
    
    while ( i < data_sz) {    
        timeout_counter = systick_get() + timeout_ms;
        while (!(SPI_x->SR & SPI_SR_TXP)){
            if (timeout_counter<systick_get()) {
                break;
            }
        } //wait free fifo 
        *((__IO uint8_t *)&(SPI_x->TXDR)) = buff[i];
        i++;
    }
    // (void)(SPI_x->TXDR);

    timeout_counter = systick_get() + timeout_ms;
    while (!(SPI_x->SR & SPI_SR_EOT)){
        if (timeout_counter<systick_get()) {
            break;
        }
    } //wait end of transaction
 
    SPI_x->IFCR |= SPI_IFCR_EOTC; // DISABLE End of Transaction Flag
}

/**
* @brief spi_16_transmit
*  Функция передачи данных по SPI   16 бит 
*  Work!
* 
* @param[in] SPI_TypeDef* SPI_x    - используемый SPI
* @return 
*/

void spi_16_transmit(SPI_TypeDef *SPI_x,uint16_t *buff,uint16_t data_sz,uint32_t timeout_ms){
    uint32_t timeout_counter=0;
    uint16_t data_element_id=0;
    uint8_t data_buf_8[0x1fff];
    if(data_sz==0 || data_sz>0x1fff)
    {
        return;
    } 
    SPI_x->CR2 &=   ~(SPI_CR2_TSIZE_Msk); // Clear TSIZE (it not clear after rewraiting)
    
    //
    data_sz=data_sz*2;
    memset(data_buf_8,0x00,0x1fff);
    memcpy(data_buf_8,buff,data_sz); 
    
    for(uint16_t i = 0; i < data_sz / 2; i++) {
        data_buf_8[2 * i]     = (buff[i] >> 8) & 0xFF;  // старший байт
        data_buf_8[2 * i + 1] = buff[i] & 0xFF;         // младший байт
    }
    if(data_buf_8[data_sz-1]&0x01){
        data_sz=data_sz+1;
    }
    SPI_x->CR1 |=   SPI_CR1_CSTART; //start
    SPI_x->CR2 |=   (data_sz << SPI_CR2_TSIZE_Pos); // set dsize
    while ( data_element_id < data_sz) {    
        timeout_counter = systick_get() + timeout_ms;
        while (!(SPI_x->SR & SPI_SR_TXP)){
            if (timeout_counter<systick_get()) {
                asm("NOP");
                break;
            }
        } //wait free fifo 
        *((__IO uint8_t *)&(SPI_x->TXDR)) = data_buf_8[data_element_id];
        data_element_id++;
    }
    timeout_counter = systick_get() + timeout_ms;
    while (!(SPI_x->SR & SPI_SR_EOT)){
        if (timeout_counter<systick_get()) {
            asm("NOP");
            break;
        }
    } //wait end of transaction    
    if(SPI_x->SR & SPI_SR_EOT) {  SPI_x->IFCR |= SPI_IFCR_EOTC;} // DISABLE End of Transaction Flag
    if(SPI_x->SR & SPI_SR_SUSP){  SPI_x->IFCR |= SPI_IFCR_SUSPC;} 
    if(SPI_x->SR & SPI_SR_TXTF){  SPI_x->IFCR |= SPI_IFCR_TXTFC;} 
    
}

/**
* @brief spi_receive_from_reg
*  Функция получения данных по SPI :
*  
* 
* @param[in] SPI_TypeDef* SPI_x    - используемый SPI
* @return 
*/
void spi_receive(SPI_TypeDef *SPI_x,uint8_t *buff,uint16_t data_sz,uint32_t timeout_ms) 
{
            uint32_t timeout_counter=0;
	while (data_sz){ 
       //while (!(SPI_x->SR & SPI_SR_DXP)) {};
        while (!(SPI_x->SR & SPI_SR_RXP)){
            if (timeout_counter<systick_get()) {
                break;
            } //если приняли больше 16 байт , то оверрун сразу, надо через прерывание сделать
          // while ((SPI_x->SR & SPI_SR_RXWNE)) {};
        *buff++ = *((__IO uint8_t *)&(SPI_x->RXDR));
        data_sz--;
    }
    //	while (SPI_x->SR & SPI_SR_RXP) {};
}
}

/**
* @brief ssi_single_receive
*  Функция получения данных по SSI :
*  
* 
* @param[in] SPI_TypeDef* SPI_x    - используемый SPI
* @return 
*/
uint32_t ssi_single_receive(SPI_TypeDef *SPI_x)//,uint8_t resolution_bit) 
{
    
    SPI_x->CR1 |=  SPI_CR1_SPE;               //SPI_EN
    SPI_x->IFCR |= SPI_IFCR_OVRC;
    SPI_x->CR1 |= SPI_CR1_CSTART;
    
    while (!(SPI_x->SR & SPI_SR_RXP)) {};
    uint32_t ret= *((__IO uint32_t *)&(SPI_x->RXDR));
    SPI_x->CR1 &= ~SPI_CR1_CSTART;
    SPI_x->CR1 &=  ~SPI_CR1_SPE;               //SPI_EN 
    if (ret==0xFFFFFFFF)
    {
        return ERROR;
    }
 //   ret = ret>>(0x1F-resolution_bit);
    return ret;  // чтоб не резать фрейм фифо в инициализации
}


/**
* @brief spi_receive
*  Функция обнуления ошибок SPI:
*  
* 
* @param[in] SPI_TypeDef* SPI_x    - используемый SPI
* @return 
*/


void reset_spi_errors(SPI_TypeDef* SPI_x) 
{	
    (void)(SPI_x->SR);
    SPI_x->IFCR |= (0b111111111<<SPI_IFCR_EOTC_Pos);
}


/**
* @brief spi_pin_config
*  Функция инициализации GPIO пинов для spi
*  
* 
* @param[in] spi_TypeDef* spi_x    - используемый spi
* @return 
*/
void spi_pin_config(SPI_TypeDef* SPI_x) 
{
    /* GPIO configuration struct */
    gpio_configuration_t gpio_config;
    
    /**** I2C gpio setting ****/
    if( SPI_x == SPI1 ){
        //SPI1_MOSI - PA7
        gpio_config.port     = SPI1_MOSI_PORT;
        gpio_config.pin      = SPI1_MOSI_PIN;
        gpio_config.mode     = gpio_af;
        gpio_config.speed    = high_speed;
        gpio_config.otype    = gpio_otype_pp;
        gpio_config.pupd     = gpio_pupd_up;
        gpio_configuration ( &gpio_config );
        SPI1_MOSI_PORT->AFR[0] |= (0x05 << GPIO_AFRL_AFSEL7_Pos); //AF = 5 
        
        //SPI1_MISO - PA6
        gpio_config.port     = SPI1_MISO_PORT;
        gpio_config.pin      = SPI1_MISO_PIN;
        gpio_config.mode     = gpio_af;
        gpio_config.speed    = high_speed;
        gpio_config.otype    = gpio_otype_pp;
        gpio_config.pupd     = gpio_pupd_up;
        gpio_configuration ( &gpio_config );
        SPI1_MISO_PORT->AFR[0] |= (0x05 << GPIO_AFRL_AFSEL6_Pos); //AF = 5 
        
        //SPI1_CLK - PA5
        gpio_config.port     = SPI1_CLK_PORT;
        gpio_config.pin      = SPI1_CLK_PIN;
        gpio_config.mode     = gpio_af;
        gpio_config.speed    = high_speed;
        gpio_config.otype    = gpio_otype_pp;
        gpio_config.pupd     = gpio_pupd_up;
        gpio_configuration ( &gpio_config );
        SPI1_CLK_PORT->AFR[0] |= (0x05 << GPIO_AFRL_AFSEL5_Pos); //AF = 5 
        
        //SPI1_CS - PA4
        gpio_config.port     = SPI1_CS_PORT;
        gpio_config.pin      = SPI1_CS_PIN;
        gpio_config.mode     = gpio_output;
        gpio_config.otype    = gpio_otype_pp;
        gpio_config.pupd     = gpio_pupd_up;
        gpio_configuration ( &gpio_config ); 
        SET_BIT( SPI1_CS_PORT->ODR, SPI1_CS_PIN);
        
    }
}

void spi_source_config(SPI_TypeDef* SPI_x)
{
        if( SPI_x == SPI1 || SPI_x == SPI2 || SPI_x == SPI3)
    {
        
        if (spi_config.spi_1_2_3_source == pll_1_q)
        {
            RCC->D2CCIP1R   |=(0x00 <<RCC_D2CCIP1R_SPI123SEL_Pos); // Тактируем SPI от PLL1_Q
        }
        else if (spi_config.spi_1_2_3_source == pll_2_p)
        {
            RCC->D2CCIP1R   |=(0x01 <<RCC_D2CCIP1R_SPI123SEL_Pos); // Тактируем SPI от PLL2_P
        }
        else if (spi_config.spi_1_2_3_source == pll_3_p)
        {
            RCC->D2CCIP1R   |=(0x02 <<RCC_D2CCIP1R_SPI123SEL_Pos); // Тактируем SPI от PLL3_P
        }
        else if (spi_config.spi_1_2_3_source == per_ck)
        {
            RCC->D2CCIP1R   |=(0x04 <<RCC_D2CCIP1R_SPI123SEL_Pos); // Тактируем SPI от PER_CK
        }
        
        
        if(SPI_x==SPI1)
        {
            RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;     //SPI1 clock enable
        }
        else if(SPI_x==SPI2)
        {
             RCC->APB1LENR |= RCC_APB1LENR_SPI2EN;     //SPI1 clock enable 
        }
        else if(SPI_x==SPI3)
        {
            RCC->APB1LENR |= RCC_APB1LENR_SPI3EN;     //SPI1 clock enable
        }    
    }
    
    
        else if( SPI_x == SPI4 || SPI_x == SPI5 )
        {
            if (spi_config.spi_4_5_source == pclk_2)
            {
                RCC->D2CCIP1R   |=(0x00 <<RCC_D2CCIP1R_SPI45SEL_Pos); // Тактируем SPI
            }
            else if (spi_config.spi_4_5_source == pll_2_q)
            {
                RCC->D2CCIP1R   |=(0x01 <<RCC_D2CCIP1R_SPI45SEL_Pos); // Тактируем SPI от 
            }
            else if (spi_config.spi_4_5_source == pll_3_q)
            {
                RCC->D2CCIP1R   |=(0x02 <<RCC_D2CCIP1R_SPI45SEL_Pos); // Тактируем SPI от
            }
            else if (spi_config.spi_4_5_source == hsi_spi)
            {
                RCC->D2CCIP1R   |=(0x03 <<RCC_D2CCIP1R_SPI45SEL_Pos); // Тактируем SPI от
            }
            else if (spi_config.spi_4_5_source == csi_spi)
            {
                RCC->D2CCIP1R   |=(0x04 <<RCC_D2CCIP1R_SPI45SEL_Pos); // Тактируем SPI от HSI
            }  
            else if (spi_config.spi_4_5_source == hse_spi)
            {
                RCC->D2CCIP1R   |=(0x05 <<RCC_D2CCIP1R_SPI45SEL_Pos); // Тактируем SPI от HSE
                
            } 
            
            if(SPI_x==SPI4)
            {
                RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;     //SPI4 clock enable
            }
            else if (SPI_x==SPI5)
            {
                RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;     //SPI5 clock enable  
            } 
        }
        
        else if( SPI_x == SPI6 )
        {
            if (spi_config.spi_6_source == pclk_4)
            {
                RCC->D3CCIPR   |=(0x00 <<RCC_D3CCIPR_SPI6SEL_Pos); // Тактируем SPI
            }
            else if (spi_config.spi_6_source == pll_2_q)
            {
                RCC->D3CCIPR   |=(0x01 <<RCC_D3CCIPR_SPI6SEL_Pos); // Тактируем SPI от 
            }
            else if (spi_config.spi_6_source == pll_3_q)
            {
                RCC->D3CCIPR   |=(0x02 <<RCC_D3CCIPR_SPI6SEL_Pos); // Тактируем SPI от
            }
            else if (spi_config.spi_6_source == hsi_spi)
            {
                RCC->D3CCIPR   |=(0x03 <<RCC_D3CCIPR_SPI6SEL_Pos); // Тактируем SPI от
            }
            else if (spi_config.spi_6_source == csi_spi)
            {
                RCC->D3CCIPR   |=(0x04 <<RCC_D3CCIPR_SPI6SEL_Pos); // Тактируем SPI от HSI
                
            }  
            else if (spi_config.spi_6_source == hse_spi)
            {
                RCC->D3CCIPR   |=(0x05 <<RCC_D3CCIPR_SPI6SEL_Pos); // Тактируем SPI от HSE
                
            } 
            
            RCC->APB4ENR |= RCC_APB4ENR_SPI6EN;     //SPI5 clock enable  
            
        }
}








ErrorStatus spi_mbr(SPI_TypeDef* SPI_x,uint16_t presc)
{
    SPI_x->CFG1 &= ~( SPI_CFG1_MBR_Msk);             //2 , default
    switch(presc)
    {
        case 2:
        break;
        case 4:
        SPI_x->CFG1 |=  (0x01<<SPI_CFG1_MBR_Pos);           // f/4
        break;
        case 8:
        SPI_x->CFG1 |=  (0x02<<SPI_CFG1_MBR_Pos);           // f/8
        break;
        case 16:
        SPI_x->CFG1 |=  (0x03<<SPI_CFG1_MBR_Pos);           // f/16
        break;
        case 32:
        SPI_x->CFG1 |=  (0x04<<SPI_CFG1_MBR_Pos);           // f/32
        break;
        case 64:
        SPI_x->CFG1 |=  (0x05<<SPI_CFG1_MBR_Pos);           // f/64
        break;
        case 128:
        SPI_x->CFG1 |=  (0x06<<SPI_CFG1_MBR_Pos);           // f/128
        break;
        case 256:
        SPI_x->CFG1 |=  (0x07<<SPI_CFG1_MBR_Pos);           // f/256
        break;
        default:  
        SPI_x->CFG1 |=  (0x05<<SPI_CFG1_MBR_Pos);           // f/64
        return ERROR;
        break;
    }
      return SUCCESS;
}

ErrorStatus spi_mbr_config(SPI_TypeDef* SPI_x)
{
            ErrorStatus status;
    if(SPI_x==SPI1)
    {
        status=spi_mbr(SPI_x,spi_config.spi_mbr_presc.spi_1);
    }
    else if(SPI_x==SPI2)
    {
        status=spi_mbr(SPI_x,spi_config.spi_mbr_presc.spi_2);
    }
    else if(SPI_x==SPI3)
    {
        status=spi_mbr(SPI_x,spi_config.spi_mbr_presc.spi_3);
    }
    else if(SPI_x==SPI4)
    {
        status=spi_mbr(SPI_x,spi_config.spi_mbr_presc.spi_4);
    }
    else if(SPI_x==SPI5)
    {
        status=spi_mbr(SPI_x,spi_config.spi_mbr_presc.spi_5);
    }
    else if(SPI_x==SPI6)
    {
        status=spi_mbr(SPI_x,spi_config.spi_mbr_presc.spi_6);
    }
       return status;
}   



ErrorStatus spi_data_size(SPI_TypeDef* SPI_x,uint16_t size)
{
    if((size>3 && size <=32) && ( SPI_x==SPI1 || SPI_x==SPI2 ||SPI_x==SPI3 ) )
    {
        SPI_x->CFG1 &= ~( SPI_CFG1_DSIZE_Msk);            
        SPI_x->CFG1 |=  ((size-0x01)<<SPI_CFG1_DSIZE_Pos);     // Set buff size to 4->32 bits
        return SUCCESS;
    }
    else if ((size>3 && size <=16) && ( SPI_x==SPI4 || SPI_x==SPI5 || SPI_x==SPI6 ) )
    { 
        SPI_x->CFG1 &= ~( SPI_CFG1_DSIZE_Msk);            
        SPI_x->CFG1 |=  ((size-0x01)<<SPI_CFG1_DSIZE_Pos);     // Set buff size to 4->16 bits
        return SUCCESS;
    }
    else
    {
        return ERROR;   
    }
}
    
ErrorStatus spi_data_size_config(SPI_TypeDef* SPI_x)
{
        ErrorStatus status;
    if(SPI_x==SPI1)
    {
        status=spi_data_size(SPI_x,spi_config.spi_data_size.spi_1);
    }
    else if(SPI_x==SPI2)
    {
        status=spi_data_size(SPI_x,spi_config.spi_data_size.spi_2);
    }
    else if(SPI_x==SPI3)
    {
        status=spi_data_size(SPI_x,spi_config.spi_data_size.spi_3);
    }
    else if(SPI_x==SPI4)
    {
        status=spi_data_size(SPI_x,spi_config.spi_data_size.spi_4);
    }
    else if(SPI_x==SPI5)
    {
        status=spi_data_size(SPI_x,spi_config.spi_data_size.spi_5);
    }
    else if(SPI_x==SPI6)
    {
        status=spi_data_size(SPI_x,spi_config.spi_data_size.spi_6);
    }
       return status;
}   


ErrorStatus spi_datas_in_fifo(SPI_TypeDef* SPI_x,uint16_t datas_in_fifo)
{
    if(datas_in_fifo>=1 && datas_in_fifo <=16)
    {
        SPI_x->CFG1 &= ~( SPI_CFG1_FTHLV_Msk);            
        SPI_x->CFG1 |=  ((datas_in_fifo-0x01)<<SPI_CFG1_FTHLV_Pos);     // Set datas in one fifo
        return SUCCESS;
    }
    else
    {
        return ERROR;  
    }
    
}

ErrorStatus spi_datas_in_fifo_config(SPI_TypeDef* SPI_x)
{
    ErrorStatus status;
    if(SPI_x==SPI1)
    {
        status=spi_datas_in_fifo(SPI_x,spi_config.spi_datas_in_one_fifo.spi_1);
    }
    else if(SPI_x==SPI2)
    {
        status=spi_datas_in_fifo(SPI_x,spi_config.spi_datas_in_one_fifo.spi_2);
    }
    else if(SPI_x==SPI3)
    {
        status=spi_datas_in_fifo(SPI_x,spi_config.spi_datas_in_one_fifo.spi_3);
    }
    else if(SPI_x==SPI4)
    {
        status=spi_datas_in_fifo(SPI_x,spi_config.spi_datas_in_one_fifo.spi_4);
    }
    else if(SPI_x==SPI5)
    {
        status=spi_datas_in_fifo(SPI_x,spi_config.spi_datas_in_one_fifo.spi_5);
    }
    else if(SPI_x==SPI6)
    {
        status=spi_datas_in_fifo(SPI_x,spi_config.spi_datas_in_one_fifo.spi_6);
    }
         return status;
}   

void spi_master_slave_config(SPI_TypeDef* SPI_x)
{
        SPI_x->CFG2 &=  ~(SPI_CFG2_MASTER_Msk);
  if(SPI_x==SPI1)
    {
           SPI_x->CFG2 |= (   spi_config.spi_mode.spi_1 << SPI_CFG2_MASTER_Pos);     
    }
    else if(SPI_x==SPI2)
    {
           SPI_x->CFG2 |= (   spi_config.spi_mode.spi_2 << SPI_CFG2_MASTER_Pos);     
    }
    else if(SPI_x==SPI3)
    {
           SPI_x->CFG2 |= (   spi_config.spi_mode.spi_3 << SPI_CFG2_MASTER_Pos);     
    }
    else if(SPI_x==SPI4)
    {
           SPI_x->CFG2 |= (   spi_config.spi_mode.spi_4 << SPI_CFG2_MASTER_Pos);     
    }
    else if(SPI_x==SPI5)
    {
           SPI_x->CFG2 |= (   spi_config.spi_mode.spi_5 << SPI_CFG2_MASTER_Pos);      
    }
    else if(SPI_x==SPI6)
    {
           SPI_x->CFG2 |= (   spi_config.spi_mode.spi_6 << SPI_CFG2_MASTER_Pos);     
    }   
  
}

void spi_communication_mode_config(SPI_TypeDef* SPI_x)
{
    SPI_x->CFG2 &=  ~(SPI_CFG2_COMM_Msk);
    
    if(SPI_x==SPI1)
    {
        SPI_x->CFG2 |= (    spi_config.spi_communication_mode.spi_1  << (SPI_CFG2_COMM_Pos));  
    }
    else if(SPI_x==SPI2)
    {
        SPI_x->CFG2 |= (    spi_config.spi_communication_mode.spi_2  << SPI_CFG2_COMM_Pos);     
    }
    else if(SPI_x==SPI3)
    {
        SPI_x->CFG2 |= (    spi_config.spi_communication_mode.spi_3  << SPI_CFG2_COMM_Pos);     
    }
    else if(SPI_x==SPI4)
    {
        SPI_x->CFG2 |= (    spi_config.spi_communication_mode.spi_4  << SPI_CFG2_COMM_Pos);     
    }
    else if(SPI_x==SPI5)
    {
        SPI_x->CFG2 |= (    spi_config.spi_communication_mode.spi_5  << SPI_CFG2_COMM_Pos);      
    }
    else if(SPI_x==SPI6)
    {
        SPI_x->CFG2 |= (    spi_config.spi_communication_mode.spi_6  << SPI_CFG2_COMM_Pos);     
    }   
    
}

void spi_polarity_config(SPI_TypeDef* SPI_x)
{
    
        SPI_x->CFG2 &=  ~(SPI_CFG2_CPOL_Msk);
  if(SPI_x==SPI1)
    {
           SPI_x->CFG2 |= (   spi_config.spi_cpol.spi_1 << SPI_CFG2_CPOL_Pos);     
    }
    else if(SPI_x==SPI2)
    {
           SPI_x->CFG2 |= (   spi_config.spi_cpol.spi_2 << SPI_CFG2_CPOL_Pos);     
    }
    else if(SPI_x==SPI3)
    {
           SPI_x->CFG2 |= (   spi_config.spi_cpol.spi_3 << SPI_CFG2_CPOL_Pos);     
    }
    else if(SPI_x==SPI4)
    {
           SPI_x->CFG2 |= (   spi_config.spi_cpol.spi_4 << SPI_CFG2_CPOL_Pos);     
    }
    else if(SPI_x==SPI5)
    {
           SPI_x->CFG2 |= (   spi_config.spi_cpol.spi_5 << SPI_CFG2_CPOL_Pos);      
    }
    else if(SPI_x==SPI6)
    {
           SPI_x->CFG2 |= (   spi_config.spi_cpol.spi_6 << SPI_CFG2_CPOL_Pos);     
    }   
    
}

void spi_phase_config(SPI_TypeDef* SPI_x)
{
            SPI_x->CFG2 &=  ~(SPI_CFG2_CPHA_Msk);
  if(SPI_x==SPI1)
    {
           SPI_x->CFG2 |= (   spi_config.spi_cpha.spi_1 << SPI_CFG2_CPHA_Pos);     
    }
    else if(SPI_x==SPI2)
    {
           SPI_x->CFG2 |= (   spi_config.spi_cpha.spi_2 << SPI_CFG2_CPHA_Pos);     
    }
    else if(SPI_x==SPI3)
    {
           SPI_x->CFG2 |= (   spi_config.spi_cpha.spi_3 << SPI_CFG2_CPHA_Pos);     
    }
    else if(SPI_x==SPI4)
    {
           SPI_x->CFG2 |= (   spi_config.spi_cpha.spi_4 << SPI_CFG2_CPHA_Pos);     
    }
    else if(SPI_x==SPI5)
    {
           SPI_x->CFG2 |= (   spi_config.spi_cpha.spi_5 << SPI_CFG2_CPHA_Pos);      
    }
    else if(SPI_x==SPI6)
    {
           SPI_x->CFG2 |= (   spi_config.spi_cpha.spi_6 << SPI_CFG2_CPHA_Pos);     
    }   
    
}

void spi_enable_config(SPI_TypeDef* SPI_x)
{
    SPI_x->CR1 |=  SPI_CR1_SPE;               //SPI_EN
    
    if(SPI_x==SPI1)
    {
        spi_config.spi_enable.spi_1=enable;
    }
    else if(SPI_x==SPI2)
    {
        spi_config.spi_enable.spi_2=enable;   
    }
    else if(SPI_x==SPI3)
    {
        spi_config.spi_enable.spi_3=enable;
    }
    else if(SPI_x==SPI4)
    {
        spi_config.spi_enable.spi_4=enable; 
    }
    else if(SPI_x==SPI5)
    {
        spi_config.spi_enable.spi_5=enable;   
    }
    else if(SPI_x==SPI6)
    {
        spi_config.spi_enable.spi_6=enable; 
    }   
    
}
