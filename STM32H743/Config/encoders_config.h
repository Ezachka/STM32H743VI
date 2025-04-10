#ifndef ENCODERS_CONFIG_H
#define ENCODERS_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

/** types of encoder */
typedef enum{
    kubler_14,
    china_16,  
    zettlex_16,
    sensna_18,
    none
} encoder_type_t;

/** types of encoder protocol */
typedef enum{
    ssi =0x00,
    usart=0x01
}protocol_type_t;

/** encoder location */
typedef enum{
    turret_ver,
    turret_hor,
    turret_1_ver,
    turret_2_hor,
    ppk_ver,
    ppk_hor,
    ppk_1_ver,
    ppk_2_hor
} encoder_location_t;



/****************************  CONFIG  **********************************/                                               
/** Gear ratio values */ // Передаточные числа                                       
static const float 	  	encoder_turret_hor_gear_ratio           =   1.0f; //-6.0f;
static const float 	  	encoder_turret_ver_gear_ratio           =   1.0f; // -2.38461538f;// 2.78947368 left; -2.38461538f; right номинал

                                                                
static const float 	  	encoder_ppk_hor_gear_ratio              =   0.0f;
static const float 	  	encoder_ppk_ver_gear_ratio              =   -3.0f;

/** encoder select */  // Тип энкодера выбранного расположения   
static encoder_type_t 	encoder_turret_hor                      =   sensna_18 ;
static protocol_type_t  encoder_turret_hor_protocol             =   ssi;
static encoder_type_t 	encoder_turret_ver                      =   sensna_18 ;
static protocol_type_t  encoder_turret_ver_protocol             =   usart;

//                                                                 
static encoder_type_t 	encoder_ppk_hor                         =   china_16 ;
static protocol_type_t  encoder_ppk_hor_protocol                =   ssi;
static encoder_type_t 	encoder_ppk_ver                         =   zettlex_16 ;
static protocol_type_t  encoder_ppk_ver_protocol                =   ssi;

/** encoder zero wire pin */  // зануление энкодера, пины и параметры
static const uint16_t	encoders_calibration_timeout	        =   1000; // ms время длительности зануления 
// испольхуемые пины требуется проинициализировать в gpio_init
static GPIO_TypeDef* 	encoder_turret_hor_zero_wire_port       =   GPIOE ;
static uint8_t          encoder_turret_hor_zero_wire_pin        =   11;
static GPIO_TypeDef* 	encoder_turret_ver_zero_wire_port       =   GPIOE ;
static uint8_t          encoder_turret_ver_zero_wire_pin        =   14;
//                                                           
static GPIO_TypeDef* 	encoder_ppk_hor_zero_wire_port          =   GPIOI ;
static uint8_t          encoder_ppk_hor_zero_wire_pin           =   0;
static GPIO_TypeDef* 	encoder_ppk_ver_zero_wire_port          =   GPIOI ;
static uint8_t          encoder_ppk_ver_zero_wire_pin           =   0;

// к какому usart подключена SENSNA, если она вообще по USART
//USART_TypeDef           *sensna_select_usart                            =   USART1 ;
/** encoder selec */    
/****************************  CONFIG  **********************************/                                                                             


/** For 14-bit single turn KUBLER values */ 

static const uint8_t	encoder_kubler_14_multiturn_bits		=   12;
static const uint32_t	encoder_kubler_14_multiturn_max		    =   0x00000FFF;
static const uint32_t	encoder_kubler_14_multiturn_max_half    =   0x000007FF;
static const uint32_t	encoder_kubler_14_ms_mask				=   0x3FFFFFF;//0x0FFFFFFF;
                                
static const uint8_t	encoder_kubler_14_singleturn_bits		=   14;//16;
static const uint32_t	encoder_kubler_14_singleturn_max		=   0x00003FFF;//0x0000FFFF;
static const uint32_t	encoder_kubler_14_wrong_value		    =   0x00FFFFFF;//0x03FFFFFF;
static const uint32_t 	encoder_kubler_14_ticks_per_turn		=   0x00004000;//0x00010000;
static const float 		encoder_kubler_14_angle_per_tick 		=   0.021973997f;//0.005493164; // (360 / (2^encoder_hor_singleturn_bits)) 
/** For 16-bit single turn CHINA values */  
    
static const uint8_t	encoder_china_16_multiturn_bits		    =   12;
static const uint32_t	encoder_china_16_multiturn_max		    =   0x00000FFF;
static const uint32_t	encoder_china_16_multiturn_max_half     =   0x000007FF;
static const uint32_t	encoder_china_16_ms_mask				=   0x0FFFFFFF;
                                
static const uint8_t	encoder_china_16_singleturn_bits		=   16;
static const uint32_t	encoder_china_16_singleturn_max		    =   0x0000FFFF;
static const uint32_t	encoder_china_16_wrong_value		    =   0x03FFFFFF;
static const uint32_t 	encoder_china_16_ticks_per_turn		    =   0x00010000;
static const float 		encoder_china_16_angle_per_tick 		=   0.005493164f; // (360 / (2^encoder_hor_singleturn_bits)) 
    
/** For 18-bit single turn SENSNA values */     
static const uint32_t	encoder_sensna_18_ms_mask				=   0x0FFFFFFF;
    
static const uint8_t    encoder_sensna_18_singleturn_bits       =   18;                          // Количество бит для значения
static const uint32_t   encoder_sensna_18_singleturn_max        =   0x0003FFFF;                  // Максимальное значение 18-битного энкодера (2^18 - 1)
static const uint32_t   encoder_sensna_18_wrong_value           =   0x3FFFFFFF;                  // Некорректное значение для 18-битного энкодера
static const uint32_t   encoder_sensna_18_ticks_per_turn        =   0x00040000;                  // Количество тиков за один полный оборот (2^18)
static const float      encoder_sensna_18_angle_per_tick        =   0.001373291015625f;          // Угол на один тик: 360 / (2^18)
    
/** For 16-bit single turn ZETTLEX values */    
static const uint32_t	encoder_zettlex_16_ms_mask				=   0x0FFFFFFF;
    
static const uint8_t    encoder_zettlex_16_singleturn_bits      =   16;                          // Количество бит для значения
static const uint32_t   encoder_zettlex_16_singleturn_max       =   0x0000FFFF;                  // Максимальное значение 16-битного энкодера (2^16 - 1)
static const uint32_t   encoder_zettlex_16_wrong_value          =   0x03FFFFFF;                  // Некорректное значение для 16-битного энкодера
static const uint32_t   encoder_zettlex_16_ticks_per_turn       =   0x00010000;                  // Количество тиков за один полный оборот (2^16)
static const float      encoder_zettlex_16_angle_per_tick       =   0.005493164f;                // Угол на один тик: 360 / (2^16)






#endif //ENCODERS_CONFIG_H
