#ifndef STM32H7XX_SPI_H
#define STM32H7XX_SPI_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "stm32h743xx.h"
    
#include "stm32h7xx.h"
#include "stm32h7xx_rcc.h"
    
    typedef enum
    {
        pll_1_q,    
        pll_2_p,  
        pll_2_q,    
        pll_3_q,
        pll_3_p,
        per_ck,
        pclk_2,  
        pclk_4, 
        hsi_spi,
        csi_spi,
        hse_spi
    }spi_sources_t;
    
    typedef enum
    {
        disable = 0x00,
        enable = 0x01
    }spi_enable_t;  
    typedef enum
    {
        slave = 0x00,
        master = 0x01
    }spi_mode_t;   
    
    typedef enum
    {
        full_duplex = 0x00,
        simplex_transmitter = 0x01,
        simplex_receiver = 0x02,
        half_duplex = 0x03  
    }spi_communication_mode_t;
    
    typedef enum
    {
        cpol_0 = 0x00,
        cpol_1 = 0x01
    }spi_clock_polarity_t;   
    
    typedef enum
    {
        first = 0x00,
        second = 0x01
    }spi_clock_phase_t;  
    
    typedef struct
    {
        struct
        { 
            spi_enable_t spi_1;
            spi_enable_t spi_2;
            spi_enable_t spi_3;       
            spi_enable_t spi_4;
            spi_enable_t spi_5;
            spi_enable_t spi_6;
        }spi_enable;
        struct
        { 
            spi_mode_t spi_1;
            spi_mode_t spi_2;
            spi_mode_t spi_3;       
            spi_mode_t spi_4;
            spi_mode_t spi_5;
            spi_mode_t spi_6;
        }spi_mode;
        struct
        {        
            spi_communication_mode_t spi_1;
            spi_communication_mode_t spi_2;
            spi_communication_mode_t spi_3;       
            spi_communication_mode_t spi_4;
            spi_communication_mode_t spi_5;
            spi_communication_mode_t spi_6;
        }spi_communication_mode;
        struct
        {  
            spi_clock_polarity_t spi_1;
            spi_clock_polarity_t spi_2;
            spi_clock_polarity_t spi_3;     
            spi_clock_polarity_t spi_4;
            spi_clock_polarity_t spi_5;
            spi_clock_polarity_t spi_6;
        }spi_cpol;
        
        struct
        {
            spi_clock_phase_t spi_1;
            spi_clock_phase_t spi_2;
            spi_clock_phase_t spi_3;     
            spi_clock_phase_t spi_4;
            spi_clock_phase_t spi_5;
            spi_clock_phase_t spi_6;
        }spi_cpha;
        
        
        struct
        {
            uint16_t spi_1;
            uint16_t spi_2;
            uint16_t spi_3;
            uint16_t spi_4;
            uint16_t spi_5;
            uint16_t spi_6;
        }spi_mbr_presc;       
        
        struct
        {
            uint8_t spi_1;
            uint8_t spi_2;
            uint8_t spi_3;
            uint8_t spi_4;
            uint8_t spi_5;
            uint8_t spi_6;        
        }spi_data_size;   
        
        struct
        {
            uint8_t spi_1;
            uint8_t spi_2;
            uint8_t spi_3;
            uint8_t spi_4;
            uint8_t spi_5;
            uint8_t spi_6;
        }spi_datas_in_one_fifo;   
        
        
        spi_sources_t spi_1_2_3_source;      
        spi_sources_t spi_4_5_source;      
        spi_sources_t spi_6_source;  
        
    }spi_config_t;
    ///@TODO Move it in own func
    typedef struct{
        struct{
            uint16_t OVR;
            uint16_t UDR;
            uint16_t TXP;
            uint16_t RXP;
            uint16_t EOT;
        }it_counts;
    }spi_info_t;
    ////
    ErrorStatus spi_init(SPI_TypeDef* SPI_x);
    void spi_receive(SPI_TypeDef *SPI_x,uint8_t *buff,uint16_t data_sz,uint32_t timeout_ms);
    void spi_transmit(SPI_TypeDef *SPI_x,uint8_t *buff,uint16_t data_sz,uint32_t timeout_ms);
    void spi_16_transmit(SPI_TypeDef *SPI_x,uint16_t *buff,uint16_t data_sz,bool MSB_first,uint32_t timeout_ms);
    void spi_1_irq (void);  
    uint32_t ssi_single_receive(SPI_TypeDef *SPI_x);//,uint8_t resolution_bit);
    
#ifdef	__cplusplus
}
#endif

#endif //STM32H7XX_SPI_H