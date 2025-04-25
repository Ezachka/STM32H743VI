#include "ad_9833.h"
#include "stm32h7xx_spi.h"
#include "stm32h7xx_interrupts.h"

/**
******************************************************************************
* File Name          : ad_9833.c
* Description        : This lib made for AD 9833  freq gtntrator
*                      
******************************************************************************
* @attention
* 
* 
* 
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
ErrorStatus ad_9833_reset(SPI_TypeDef *SPI_x);
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
  //  delay_ms(1);
    uint8_t data_2 [4];
    data_2[0]=(uint8_t)(data>>8);
    data_2[1]=(uint8_t)(data);
    data_2[2]=0x00;
    data_2[3]=0x00;
    spi_transmit(SPI_x,data_2,2,50);///@TODO dsize 2 sometimes makes dont lower tx pin after transmit
    //spi_16_transmit(SPI_x,&data,1,50);///@TODO 

    delay_ms(1);
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
    memset(&ad_9833.control_reg.reg,0x00,sizeof(uint16_t));
    memset(&ad_9833.settings,0x00,sizeof(ad_9833.settings));
    strncpy(ad_9833.name, "ad 9833", 7);
    ad_9833.working=true;
    
    ad_9833.control_reg.b_28=0x00; //14bit operations
    
    ad_9833.settings.signal=square_half;
    ad_9833.settings.frequency[0]=500.0f;
    ad_9833.settings.frequency[1]=1000.0f;
    ad_9833.settings.phaze[0]=0.0f;
    ad_9833.settings.phaze[1]=90.0f;
    ad_9833.settings.work_channel=0x00;
    
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
    
    ad_9833_set_signal_form(SPI_x,ad_9833.settings.signal);
 
    ad_9833_set_frequency(SPI_x,ad_9833.settings.frequency[0],0);

    ad_9833_set_frequency(SPI_x,ad_9833.settings.frequency[1],1);   
    
    ad_9833_set_phaze(SPI_x,ad_9833.settings.phaze[0],0);

    ad_9833_set_phaze(SPI_x,ad_9833.settings.phaze[1],1);     
    
    ad_9833_set_freq_work_channel(SPI_x,0);

    ad_9833_set_phaze_work_channel(SPI_x,0);    
    
    ad_9833_update(SPI_x);
    return status;
}

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
    
    static uint16_t     old_control_reg;
    static uint8_t      old_work_channel;
    static signal_t     old_signal_type;
    static float        old_freq[2];
    static float        old_phaze[2];
    //settings
    if(old_control_reg!=ad_9833.control_reg.reg){ //control reg
        ad_9833.control_reg.reg=ad_9833.control_reg.reg&0x3fff;
        ad_9833_data_write(SPI_x,ad_9833.control_reg.reg);
        old_control_reg=ad_9833.control_reg.reg;
    }
    if(old_work_channel!=ad_9833.settings.work_channel){ //work_channel
        ad_9833_set_freq_work_channel(SPI_x,ad_9833.settings.work_channel);
        ad_9833_set_phaze_work_channel(SPI_x,ad_9833.settings.work_channel);    
        old_work_channel=ad_9833.settings.work_channel;
    } 
    if(old_signal_type!=ad_9833.settings.signal){ //signal_type
        ad_9833_set_signal_form(SPI_x,ad_9833.settings.signal);
        old_signal_type=ad_9833.settings.signal;
    } 
    if(old_freq[0]!=ad_9833.settings.frequency[0]){ //freq
        ad_9833_set_frequency(SPI_x,ad_9833.settings.frequency[0],0);  
        old_freq[0]=ad_9833.settings.frequency[0];
    }  
    if(old_freq[1]!=ad_9833.settings.frequency[1]){ //freq
        ad_9833_set_frequency(SPI_x,ad_9833.settings.frequency[1],1);  
        old_freq[1]=ad_9833.settings.frequency[1];
    }  
    if(old_phaze[0]!=ad_9833.settings.phaze[0]){ //phaze
        ad_9833_set_phaze(SPI_x,ad_9833.settings.phaze[0],0);  
        old_phaze[0]=ad_9833.settings.phaze[0];
    }  
    if(old_phaze[1]!=ad_9833.settings.phaze[1]){ //phaze
        ad_9833_set_phaze(SPI_x,ad_9833.settings.phaze[1],1);  
        old_phaze[1]=ad_9833.settings.phaze[1];
    }  
    //operational
    if(ad_9833.operations.reset){
    ad_9833.operations.reset=false;
    ad_9833_reset(SPI_x);
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
        ad_9833.control_reg.f_select=0x00;
        break;
        case 1:
        ad_9833.control_reg.f_select=0x01;
        break;
        default:
        ad_9833.control_reg.f_select=0x00;
        break;
    }
  //  ad_9833_data_write(SPI_x,ad_9833.control_reg.reg);
    
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
        ad_9833.control_reg.p_select=0x00;
        break;
        case 1:
        ad_9833.control_reg.p_select=0x01;
        break;
        default:
        ad_9833.control_reg.p_select=0x00;
        break;
    }
  //  ad_9833_data_write(SPI_x,ad_9833.control_reg.reg);
    
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
        ad_9833.control_reg.op_bit_en   =0x00;
        ad_9833.control_reg.mode        =0x00;
        ad_9833.control_reg.div_2       =0x00;
        break;
        case triangle:
        ad_9833.control_reg.op_bit_en   =0x00;
        ad_9833.control_reg.mode        =0x01;
        ad_9833.control_reg.div_2       =0x00;
        break;
        case square:
        ad_9833.control_reg.op_bit_en   =0x01;
        ad_9833.control_reg.mode        =0x00;
        ad_9833.control_reg.div_2       =0x00;
        break;
        case square_half:
        ad_9833.control_reg.op_bit_en   =0x01;
        ad_9833.control_reg.mode        =0x00;
        ad_9833.control_reg.div_2       =0x01;
        break;
        default:
        ad_9833.control_reg.sleep_1     =0x01; //off
        ad_9833.control_reg.sleep_12    =0x01;
        break;
    }
 //   ad_9833_data_write(SPI_x,ad_9833.control_reg.reg);
    
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
        frequency=0x00;
    }
    else if(frequency>AD_MAX_FREQ){
        frequency=AD_MAX_FREQ;
    }
    
    uint32_t freq=(uint32_t)(frequency*(AD_2_28/AD_CRYSTAl_FREQ));
    
    if(channel==0){ 
    ad_9833_data_write(SPI_x,freq | AD_9833_FREQ_REG_0);
    }
    else if(channel==1){
    ad_9833_data_write(SPI_x,freq | AD_9833_FREQ_REG_1);
    }
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
    
    uint32_t freq=(uint32_t)(phaze*(4095.0f/360.0f));
    
    if(channel==0){ 
    ad_9833_data_write(SPI_x,freq | AD_9833_PHAZE_REG_0);
    }
    else if(channel==1){
    ad_9833_data_write(SPI_x,freq | AD_9833_PHAZE_REG_1);
    }
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

ErrorStatus ad_9833_reset(SPI_TypeDef *SPI_x){
    ErrorStatus status = SUCCESS; 
    ad_9833.control_reg.reset=0x01;
    ad_9833_data_write(SPI_x,ad_9833.control_reg.reg);
    ad_9833.control_reg.reset=0x00;
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
    ad_9833.control_reg.sleep_1=0x01;
    ad_9833.control_reg.sleep_12=0x01;
   // ad_9833_data_write(SPI_x,ad_9833.control_reg.reg);
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
    ad_9833.control_reg.sleep_1=0x00;
    ad_9833.control_reg.sleep_12=0x00;
  //  ad_9833_data_write(SPI_x,ad_9833.control_reg.reg);
    return status;
}
