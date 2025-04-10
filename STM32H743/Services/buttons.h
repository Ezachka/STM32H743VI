#ifndef BUTTONS_H
#define BUTTONS_H

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "stm32l4xx.h"
#include "stdlib.h"
#include <string.h>       
#include <stdbool.h>
    
   typedef struct
    {
        bool btn  ;
        bool start_flag;
        bool short_ban_flag;
        uint16_t buzzer_freq_short;
        uint16_t buzzer_freq_long;
        uint16_t buzzer_freq_short_time;
        uint16_t buzzer_freq_long_time;
        uint32_t start_time;
    }btn_params_t;
    typedef struct
    {
        bool short_push;
        bool long_push;
    } btn_t;
    
    
    typedef struct   
    { 
        btn_t up;   
        btn_t down;       
        btn_t left;
        btn_t right;
        btn_t select;
        struct {
            uint32_t short_push_time;
            uint32_t long_push_time;
        } config;          
        struct {      
            btn_params_t up;   
            btn_params_t down;       
            btn_params_t left;
            btn_params_t right;
            btn_params_t select; 
        } params;                  
    } buttons_t;
    
//    
//    buttons_t buttons;  
    
    void buttons_init(void);
    void buttons_update(void);
    
    
#ifdef __cplusplus
}
#endif

#endif /*BUTTONS_H */