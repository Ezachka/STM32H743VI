#include "ad_9833.h"
#include "stm32h7xx_spi.h"
#include "stm32h7xx_interrupts.h"
#include "dwt_tim.h"

/**
******************************************************************************
* File Name          : ad_9833.c
* Description        : This lib made for AD 9833  freq gtntrator
*                      
******************************************************************************
* @attention
* 
* Done
* Just set settings in ad_9833_param_init, or make functon for change it in structure 
* ad_9833.settings , func ad_9833_set_freq for example
* 
******************************************************************************
*/

/****************************************************************************/
/****************************** DEFINITIONS *********************************/

/****************************************************************************/
/******************************* VARIABLES **********************************/
ad_9833_t ad_9833;
/****************************************************************************/
/******************************* FUNCTIONS **********************************/
ErrorStatus ad_9833_data_write(SPI_TypeDef *SPI_x,uint16_t data);
ErrorStatus ad_9833_param_init(void);
ErrorStatus ad_9833_set_signal_form(SPI_TypeDef *SPI_x,signal_t signal);
ErrorStatus ad_9833_set_frequency(SPI_TypeDef *SPI_x,float frequency, uint8_t reg);
ErrorStatus ad_9833_set_phaze(SPI_TypeDef *SPI_x,float phaze, uint8_t channel);
ErrorStatus ad_9833_set_freq_work_channel(SPI_TypeDef *SPI_x,uint8_t channel);
ErrorStatus ad_9833_set_phaze_work_channel(SPI_TypeDef *SPI_x,uint8_t channel);
ErrorStatus ad_9833_reset(SPI_TypeDef *SPI_x,bool en);
ErrorStatus ad_9833_sleep(SPI_TypeDef *SPI_x);
ErrorStatus ad_9833_wake_up(SPI_TypeDef *SPI_x);

/****************************************************************************/
/************************  HW DEPENDENT FUNCTIONS  **************************/

/**
* @brief ad_9833_data_write_to_reg
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus ad_9833_data_write(SPI_TypeDef *SPI_x,uint16_t data){
    ErrorStatus status = SUCCESS;   
    ad_9833_enable;
    spi_16_transmit(SPI_x,&data,true,true,5);///cpol=1,cpha=0,8bit_twice,
    delay_us(10); ///@TODO little delay
    ad_9833_disable;
    return status;
}



/****************************************************************************/
/**********************  HW UNDEPENDENT FUNCTIONS  **************************/
/**********************  PARAMETRS INITIALIZATION  **************************/
/**
* @brief ad_9833_param_init
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus ad_9833_param_init(void){
    ErrorStatus status = SUCCESS;   
    strncpy(ad_9833.name, "ad 9833", 7);
    ad_9833.working=true;
    
    ad_9833.settings.enable_28_bit=true; //28bit operations
    ad_9833.settings.signal=square;
    ad_9833.settings.frequency[0]=500.0f;
    ad_9833.settings.frequency[1]=0.0f;
    ad_9833.settings.phaze[0]=0.0f;
    ad_9833.settings.phaze[1]=0.0f;
    ad_9833.settings.work_channel_freq=0x00;
    ad_9833.settings.work_channel_phaze=0x00;
    return status;
}

/**
* @brief ad_9833_init
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus ad_9833_init(SPI_TypeDef *SPI_x){
    ErrorStatus status = SUCCESS;   
    ad_9833_param_init();
    
    ad_9833_reset(SPI_x,true);
    ad_9833.control_reg.reset=false;
    ad_9833_set_signal_form(SPI_x,ad_9833.settings.signal);
    ad_9833_set_frequency(SPI_x, ad_9833.settings.frequency[0],0);
    ad_9833_set_frequency(SPI_x, ad_9833.settings.frequency[1],1);
    ad_9833_set_phaze(SPI_x, ad_9833.settings.phaze[0],0);
    ad_9833_set_phaze(SPI_x, ad_9833.settings.phaze[1],1);
    ad_9833_reset(SPI_x,false);
    
    ad_9833_set_freq_work_channel(SPI_x,ad_9833.settings.work_channel_freq);  
    ad_9833_set_phaze_work_channel(SPI_x,ad_9833.settings.work_channel_phaze);        
    
    return status;
}
/**********************  MAIN WORK FUNC  **************************/
/**
* @brief ad_9833_update
*  
*  main function for all lib
* 
* @param[in] 
* @return    
*/

ErrorStatus ad_9833_update(SPI_TypeDef *SPI_x){
    ErrorStatus status = SUCCESS; 
    
    //settings
    if(ad_9833.params.old_control_reg.reg!=ad_9833.control_reg.reg){ //control reg
        ad_9833.params.old_control_reg.reg=ad_9833.control_reg.reg&0x3fff;
        ad_9833_data_write(SPI_x,ad_9833.control_reg.reg);
    }
    if(ad_9833.params.old_work_channel_phaze!=ad_9833.settings.work_channel_phaze){ //work_channel_phaze
        ad_9833_set_phaze_work_channel(SPI_x,ad_9833.settings.work_channel_phaze);        
    } 
    if(ad_9833.params.old_phaze[0]!=ad_9833.settings.phaze[0]){ //phaze
        ad_9833_set_phaze(SPI_x,ad_9833.settings.phaze[0],0);  
    }  
    if(ad_9833.params.old_phaze[1]!=ad_9833.settings.phaze[1]){ //phaze
        ad_9833_set_phaze(SPI_x,ad_9833.settings.phaze[1],1);  
    }  
    if(ad_9833.params.old_work_channel_freq!=ad_9833.settings.work_channel_freq){ //work_channel_freq
        ad_9833_set_freq_work_channel(SPI_x,ad_9833.settings.work_channel_freq);        
    } 
    if(ad_9833.params.old_frequency[0]!=ad_9833.settings.frequency[0]){ //freq
        ad_9833_set_frequency(SPI_x,ad_9833.settings.frequency[0],0);  
        
    }  
    if(ad_9833.params.old_frequency[1]!=ad_9833.settings.frequency[1]){ //freq
        ad_9833_set_frequency(SPI_x,ad_9833.settings.frequency[1],1);  
    }  
    if(ad_9833.params.old_signal!=ad_9833.settings.signal){ //signal_type
        ad_9833_set_signal_form(SPI_x,ad_9833.settings.signal);
    } 
    //operational
    if(ad_9833.operations.reset){
        ad_9833.operations.reset=false;
        ad_9833_reset(SPI_x,true);
        ad_9833_reset(SPI_x,false);
        
    }
    if(ad_9833.operations.sleep){
        ad_9833.operations.sleep=false;
        ad_9833.working=false;
        ad_9833_sleep(SPI_x);
    }
    if(ad_9833.operations.wake_up){
        ad_9833.operations.wake_up=false;
        ad_9833.working=true;
        ad_9833_wake_up(SPI_x);
    }
    return status;
}
/**********************  PRIVATE  **************************/
/**
* @brief ad_9833_set_freq_work_channel
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus ad_9833_set_freq_work_channel(SPI_TypeDef *SPI_x,uint8_t channel){
    ErrorStatus status = SUCCESS; 
    
    switch(channel){
        case 0:
        ad_9833.control_reg.f_select=false;
        break;
        case 1:
        ad_9833.control_reg.f_select=true;
        break;
        default:
        ad_9833.control_reg.f_select=false;
        break;
    }
    ad_9833_data_write(SPI_x,ad_9833.control_reg.reg);
    ad_9833.params.old_control_reg.reg=ad_9833.control_reg.reg;
    ad_9833.params.old_work_channel_freq=channel;
    return status;
}

/**
* @brief ad_9833_set_phaze_work_channel
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus ad_9833_set_phaze_work_channel(SPI_TypeDef *SPI_x,uint8_t channel){
    ErrorStatus status = SUCCESS; 
    
    switch(channel){
        case 0:
        ad_9833.control_reg.p_select=false;
        break;
        case 1:
        ad_9833.control_reg.p_select=true;
        break;
        default:
        ad_9833.control_reg.p_select=false;
        break;
    }
    ad_9833_data_write(SPI_x,ad_9833.control_reg.reg);
    ad_9833.params.old_control_reg.reg=ad_9833.control_reg.reg;
    ad_9833.params.old_work_channel_phaze=channel;
    
    return status;
}
/**
* @brief ad_9833_set_signal_form
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus ad_9833_set_signal_form(SPI_TypeDef *SPI_x,signal_t signal){
    ErrorStatus status = SUCCESS; 
    
    switch(signal){
        case sinus:
        ad_9833.control_reg.op_bit_en   =false;
        ad_9833.control_reg.mode        =false;
        ad_9833.control_reg.div_2       =false;
        break;
        case triangle:
        ad_9833.control_reg.op_bit_en   =false;
        ad_9833.control_reg.mode        =true;
        ad_9833.control_reg.div_2       =false;
        break;
        case square_half:
        ad_9833.control_reg.op_bit_en   =true;
        ad_9833.control_reg.mode        =false;
        ad_9833.control_reg.div_2       =false;
        break;
        case square:
        ad_9833.control_reg.op_bit_en   =true;
        ad_9833.control_reg.mode        =false;
        ad_9833.control_reg.div_2       =true;
        break;
        default:
        ad_9833.control_reg.sleep_1     =true; //off
        ad_9833.control_reg.sleep_12    =true;
        break;
    }
    ad_9833_data_write(SPI_x,ad_9833.control_reg.reg);
    ad_9833.params.old_control_reg.reg=ad_9833.control_reg.reg;
    ad_9833.params.old_signal=signal;
    
    return status;
}
/**
* @brief ad_9833_set_frequency
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus ad_9833_set_frequency(SPI_TypeDef *SPI_x,float frequency, uint8_t channel){
    ErrorStatus status = SUCCESS; 
    if(frequency<0x00){
        frequency=false;
    }
    else if(frequency>AD_MAX_FREQ){
        frequency=AD_MAX_FREQ;
    }
    uint32_t freq_raw=(uint32_t)(frequency*(AD_2_28/AD_CRYSTAl_FREQ)); 
    if(ad_9833.settings.enable_28_bit){    
        ad_9833.control_reg.b_28=true;
        ad_9833_data_write(SPI_x,ad_9833.control_reg.reg);
        
        uint16_t MSB =(uint16_t)((freq_raw>>14) & 0x3fff);
        uint16_t LSB =(uint16_t)(freq_raw & 0x3fff);
        if(channel==0){ 
            MSB|=AD_9833_FREQ_REG_0;
            LSB|=AD_9833_FREQ_REG_0;
        }
        else if(channel==1){
            MSB|=AD_9833_FREQ_REG_1;
            LSB|=AD_9833_FREQ_REG_1;   
        }
        ad_9833_data_write(SPI_x,LSB);
        ad_9833_data_write(SPI_x,MSB);
        ad_9833.control_reg.b_28=false;
    }else{
        ad_9833.control_reg.b_28=false;
        ad_9833_data_write(SPI_x,ad_9833.control_reg.reg);
        uint16_t freq_raw_16= (uint16_t)(freq_raw & 0x3fff);
        
        if(channel==0){ 
            ad_9833_data_write(SPI_x,(freq_raw_16 | AD_9833_FREQ_REG_0));
        }
        else if(channel==1){
            ad_9833_data_write(SPI_x,(freq_raw_16 | AD_9833_FREQ_REG_1));
        }
    }
    ad_9833.params.old_frequency[channel]=frequency;
    return status;
}
/**
* @brief ad_9833_set_phaze
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus ad_9833_set_phaze(SPI_TypeDef *SPI_x,float phaze, uint8_t channel){
    ErrorStatus status = SUCCESS; 
    if(phaze<0x00){
        phaze=phaze+360.0f;
    }
    else if(phaze>360.0f){
        phaze=phaze-360.0f;
    }
    uint16_t phaze_raw=(uint32_t)(phaze*(4095.0f/360.0f))&0xfff;
    if(channel==0){ 
        ad_9833_data_write(SPI_x,phaze_raw | AD_9833_PHAZE_REG_0);
    }
    else if(channel==1){
        ad_9833_data_write(SPI_x,phaze_raw | AD_9833_PHAZE_REG_1);
    }
        ad_9833.params.old_phaze[channel]=phaze;
    return status;
}
/**
* @brief ad_9833_reset
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus ad_9833_reset(SPI_TypeDef *SPI_x,bool en){
    ErrorStatus status = SUCCESS; 
    if(en){
        ad_9833.control_reg.reset=true;
    }else{
        ad_9833.control_reg.reset=false;
    }
    ad_9833_data_write(SPI_x,ad_9833.control_reg.reg);
    ad_9833.params.old_control_reg.reg=ad_9833.control_reg.reg;
    return status;
}
/**
* @brief ad_9833_sleep
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus ad_9833_sleep(SPI_TypeDef *SPI_x){
    ErrorStatus status = SUCCESS; 
    ad_9833.control_reg.sleep_1=true;
    ad_9833.control_reg.sleep_12=true;
    ad_9833.working=false;
    ad_9833_data_write(SPI_x,ad_9833.control_reg.reg);
    ad_9833.params.old_control_reg.reg=ad_9833.control_reg.reg;
    return status;
}
/**
* @brief ad_9833_wake_up
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus ad_9833_wake_up(SPI_TypeDef *SPI_x){
    ErrorStatus status = SUCCESS; 
    ad_9833.control_reg.sleep_1=false;
    ad_9833.control_reg.sleep_12=false;
    ad_9833.working=true;
    ad_9833_data_write(SPI_x,ad_9833.control_reg.reg);
    ad_9833.params.old_control_reg.reg=ad_9833.control_reg.reg;
    return status;
}
/**********************  PUBLIC  **************************/
/**
* @brief ad_9833_set_freq
*  Public func for set freq
*  
* 
* @param[in]  freq
* @return    
*/

void ad_9833_set_freq(float freq){
    ad_9833.settings.frequency[ad_9833.settings.work_channel_freq]=freq;
}