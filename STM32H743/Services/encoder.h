#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32h7xx.h"
#include "encoders_config.h"
#include "stm32h743xx.h"



typedef struct
{	
    encoder_type_t      type;
    encoder_location_t  location;
    protocol_type_t     protocol;
    
    bool 		connection;
    
    float 		position;				//degree
    float 		position_180;	
    
    struct
    {
//        uint8_t		address;
//        uint8_t		id;
        GPIO_TypeDef*        zero_wire_port;
        uint8_t              zero_wire_pin;
        
        bool 		data_new;
        int32_t     position_raw;
        uint32_t 	timestamp;	
        
        float		zero_angle_correction;		
        float		gear_ratio;
        
        uint32_t    ms_mask;
        uint8_t		multiturn_bits;		
        uint32_t	multiturn_max;
        uint32_t	multiturn_max_half;
        uint8_t		singleturn_bits;
        uint32_t	singleturn_max;				
        uint32_t	ticks_per_turn;
        uint32_t	wrong_val;
        float 		angle_per_tick;
    }param;
    
} encoder_t;


typedef struct{
    encoder_t turret_ver;
    encoder_t turret_hor;
    encoder_t ppk_ver;
    encoder_t ppk_hor;
}encoders_t;



void encoders_init( void );
void encoders_update( void );

ErrorStatus sesnsa_reselect_boudrate(encoder_t *encoder,uint16_t boudrate );
ErrorStatus sesnsa_reselect_protocol(encoder_t *encoder, protocol_type_t protocol);
ErrorStatus encoder_calibrate(encoder_t *encoder);



#endif //ENCODER_H
