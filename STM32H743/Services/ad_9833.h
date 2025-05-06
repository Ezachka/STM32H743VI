
#ifndef AD_9833_H
#define AD_9833_H

#ifdef __cplusplus
extern C {
#endif
    
#include "stm32h7xx.h"
#include "gpio_config.h"
#include "stdlib.h"
#include "string.h"
#include <stdbool.h>
#include <stdio.h>  
    
    /****************************************************************************/
    /****************************** DEFINITIONS *********************************/
#define ad_9833_enable     SPI1_CS_PORT->BSRR = GPIO_BSRR_BR4
#define ad_9833_disable    SPI1_CS_PORT->BSRR = GPIO_BSRR_BS4
    
#define AD_MAX_FREQ                 12500000.0f
#define AD_CRYSTAl_FREQ             25000000.0f
#define AD_2_28                     268435456.0f
    
    
#define AD_9833_FREQ_REG_1          0x8000
#define AD_9833_FREQ_REG_0          0x4000
#define AD_9833_PHAZE_REG_0         0xC000
#define AD_9833_PHAZE_REG_1         0xE000
    
    
    /****************************************************************************/    
    /******************************* STRUCTS ************************************/
    
    typedef enum{
        sinus,
        triangle,
        square,
        square_half
    }signal_t;
    
    typedef union{
        uint16_t reg;
        struct
        {
            bool    db_0        :1;
            bool    mode        :1;
            bool    db_2        :1;
            bool    div_2       :1;
            bool    db_4        :1;
            bool    op_bit_en   :1;
            bool    sleep_12    :1;
            bool    sleep_1     :1;
            bool    reset       :1;
            bool    db_9        :1;
            bool    p_select    :1;
            bool    f_select    :1;
            bool    hlb         :1;
            bool    b_28        :1;
            bool    db_14       :1;
            bool    db_15       :1;
        };
    }control_reg_t;
    
    
    typedef struct{
        char            name[7]; 
        bool            working;
        struct{
            signal_t        signal;
            uint8_t         work_channel_freq;
            uint8_t         work_channel_phaze;
            bool            enable_28_bit;
            float           frequency[2];
            float           phaze[2];
        }settings;
        control_reg_t       control_reg;
        struct{
            bool            reset;
            bool            sleep;
            bool            wake_up;
        }operations;
        struct{
            control_reg_t       old_control_reg;
            signal_t            old_signal;
            uint8_t             old_work_channel_freq;
            uint8_t             old_work_channel_phaze;
            bool                old_enable_28_bit;
            float               old_frequency[2];
            float               old_phaze[2];
        }params;
    }ad_9833_t;
    /****************************************************************************/  

    ErrorStatus ad_9833_init(SPI_TypeDef *SPI_x);
    
    ErrorStatus ad_9833_update(SPI_TypeDef *SPI_x);
    
    void ad_9833_set_freq(float freq);
    
    
#ifdef __cplusplus
}
#endif

#endif
