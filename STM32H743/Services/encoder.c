#include "encoder.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "stm32h7xx_interrupts.h"
#include "base.h"
#include "stm32h7xx_spi.h"
#include "stm32h7xx_usart.h"

uint8_t test_smesh_kubler=0;
uint32_t test_kubler_mask=0x3FFF;
uint8_t test_smesh=0;
/******************************* VARIABLES **********************************/
encoders_t encoders;
/******************************* FUNCTIONS ************************************/
float encoder_calculate_angle_zetlex_16( encoder_t *encoder );
float encoder_calculate_angle_sensna_18( encoder_t *encoder );
float encoder_calculate_angle_kubler_14( encoder_t *encoder );
ErrorStatus encoder_init(encoder_t *encoder);
ErrorStatus encoder_update( encoder_t *encoder );
ErrorStatus encoder_get_ssi_data(SPI_TypeDef* SPI_x,encoder_t *encoder);
ErrorStatus encoder_get_usart_data(USART_TypeDef* USART_x,encoder_t *encoder);


uint32_t sensna_18_usart_parser(USART_TypeDef* USART_x );
uint32_t sensna_18_usart_packet_parser(uint8_t byte);


/****************************  dependent func  **********************************/
/**
* @brief encoder_get_ssi_data
*  получение данных для выбранного энкодера
*  s
* 
* @param[in] SPI_TypeDef* SPI_x к какому SPI подвязан SSI
* @param[in] encoder_t *encoder выбранный экнодер
* @return ErrorStatus статус  ()
*/
ErrorStatus encoder_get_ssi_data(SPI_TypeDef* SPI_x,encoder_t *encoder  )
{
    encoder->param.data_new=false;
    switch(encoder->type)
    {
        case kubler_14: 
        encoder->param.position_raw = (int32_t)ssi_single_receive(SPI_x);//,encoder->param.singleturn_bits);  //По хорошему надо сделать через DMA
        encoder->param.position_raw =  ((encoder->param.position_raw>>0x04) & encoder->param.singleturn_max);
        break;
        case china_16: 
        encoder->param.position_raw = (int32_t)ssi_single_receive(SPI_x);//,encoder->param.singleturn_bits); 
        break;
        case zettlex_16: 
        encoder->param.position_raw = (int32_t)ssi_single_receive(SPI_x);//,encoder->param.singleturn_bits); 
        break;
        case sensna_18: 
        encoder->param.position_raw = (int32_t)ssi_single_receive(SPI_x);//,encoder->param.singleturn_bits); 
        encoder->param.position_raw =  encoder->param.position_raw>>0x0d;
        break;
        default:
        return ERROR;
        break;
    }
    
    if (encoder->param.position_raw != ERROR) // Проверка на отсутствие подклбчения
    {
        encoder->param.data_new=true;
    }
    else
    {
        encoder->connection=false;   
    }
    
    return SUCCESS;
} //.encoder_get_ssi_data()


/**
* @brief encoder_get_usart_data
*  получение данных для выбранного энкодера по USART
*  
* 
* @param[in] USART_TypeDef* USART_x,к USART к которому подключен энкодер 
* @param[in] encoder_t *encoder выбранный экнодер
* @return ErrorStatus статус  ()
*/
ErrorStatus encoder_get_usart_data(USART_TypeDef* USART_x,encoder_t *encoder  )
{
    uint32_t data;
    encoder->param.data_new=false;
    switch(encoder->type)
    {
        case sensna_18: 
        data=sensna_18_usart_parser(USART_x); 
        if (data != ERROR)
        {
            encoder->param.position_raw = data; 
        }
        break;
        default:
        return ERROR;
        break;
    }
    if (encoder->param.position_raw != ERROR) // Проверка на отсутствие подклбчения
    {
        encoder->param.data_new=true;
    }
    else
    {
        encoder->connection=false;   
    }
    
    return SUCCESS;
} //.encoder_get_usart_data()
/****************************  dependent func  **********************************/

/**
* @brief encoders_init
*  инициализация энкодеров
*  
* 
* @param[in] 
* @param[in] 
* @return 
*/
void encoders_init( void )
{
    encoder_init(&encoders.turret_ver);
    encoder_init(&encoders.turret_hor);
    encoder_init(&encoders.ppk_ver);
    encoder_init(&encoders.ppk_hor);
}
/**
* @brief encoder_init
*  Инициплизпция энкодера
*  
* 
* @param[in] encoder_type_t encoder_type
* @param[in] encoder_location_t encoder_location
* @param[in] float gear_ratio
* @param[in] encoder_t *encoder
* @return ErrorStatus статус инициализации ()
*/
ErrorStatus encoder_init(encoder_t *encoder)
{
    
    
    if(encoder == &encoders.turret_ver)
    {
        encoder->location=turret_ver;
        encoder->type=encoder_turret_ver;
        encoder->param.gear_ratio = encoder_turret_ver_gear_ratio;
        encoder->protocol = encoder_turret_ver_protocol; 
        encoder->param.zero_wire_port = encoder_turret_ver_zero_wire_port; 
        encoder->param.zero_wire_pin  = encoder_turret_ver_zero_wire_pin; 
    }
    else if(encoder == &encoders.turret_hor)
    {
        encoder->location=turret_hor;
        encoder->type=encoder_turret_hor;
        encoder->param.gear_ratio = encoder_turret_hor_gear_ratio;
        encoder->protocol = encoder_turret_hor_protocol; 
        encoder->param.zero_wire_port = encoder_turret_hor_zero_wire_port; 
        encoder->param.zero_wire_pin  = encoder_turret_hor_zero_wire_pin; 
    }
    else if(encoder == &encoders.ppk_ver)
    {
        encoder->location=ppk_ver;
        encoder->type=encoder_ppk_ver;
        encoder->param.gear_ratio = encoder_ppk_ver_gear_ratio;
        encoder->protocol = encoder_ppk_ver_protocol; 
        encoder->param.zero_wire_port = encoder_ppk_ver_zero_wire_port; 
        encoder->param.zero_wire_pin  = encoder_ppk_ver_zero_wire_pin; 
    }
    else if(encoder == &encoders.ppk_hor)
    {
        encoder->location=ppk_hor;
        encoder->type=encoder_ppk_hor;
        encoder->param.gear_ratio = encoder_ppk_hor_gear_ratio;
        encoder->protocol = encoder_ppk_hor_protocol; 
        encoder->param.zero_wire_port = encoder_ppk_hor_zero_wire_port; 
        encoder->param.zero_wire_pin  = encoder_ppk_hor_zero_wire_pin; 
    }
    else
    {
        return ERROR;
    }
    
    switch(encoder->type) 
    {
        case kubler_14: 
        
        //        encoder->param.address	= encoder_hor_address;//@TODO
        //        encoder->param.id	        = encoder_hor_id;//@TODO
        
        encoder->position 	                    = 0.0f;
        encoder->param.timestamp 	            = 0;
        
        encoder->param.zero_angle_correction    = 0.0f;	
        
        encoder->param.ms_mask 	                = encoder_kubler_14_ms_mask;
        encoder->param.multiturn_bits 	        = encoder_kubler_14_multiturn_bits;
        encoder->param.multiturn_max 	        = encoder_kubler_14_multiturn_max;
        encoder->param.multiturn_max_half       = encoder_kubler_14_multiturn_max_half;
        encoder->param.singleturn_bits          = encoder_kubler_14_singleturn_bits;
        encoder->param.singleturn_max 	        = encoder_kubler_14_singleturn_max;
        encoder->param.ticks_per_turn           = encoder_kubler_14_ticks_per_turn;
        encoder->param.wrong_val                = encoder_kubler_14_wrong_value;
        encoder->param.angle_per_tick           = encoder_kubler_14_angle_per_tick;
        break;
        
        case china_16: 
        
        
        //        encoder->param.address	= encoder_hor_address;//@TODO
        //        encoder->param.id	        = encoder_hor_id;//@TODO
        
        encoder->position 	                    = 0.0f;
        encoder->param.timestamp 	            = 0;
        
        encoder->param.zero_angle_correction    = 0.0f;	
        
        encoder->param.ms_mask 	                = encoder_china_16_ms_mask;
        encoder->param.multiturn_bits 	        = encoder_china_16_multiturn_bits;
        encoder->param.multiturn_max 	        = encoder_china_16_multiturn_max;
        encoder->param.multiturn_max_half       = encoder_china_16_multiturn_max_half;
        encoder->param.singleturn_bits          = encoder_china_16_singleturn_bits;
        encoder->param.singleturn_max 	        = encoder_china_16_singleturn_max;
        encoder->param.ticks_per_turn           = encoder_china_16_ticks_per_turn;
        encoder->param.wrong_val                = encoder_china_16_wrong_value;
        encoder->param.angle_per_tick           = encoder_china_16_angle_per_tick;
        break;
        case zettlex_16: 
        
        //        encoder->param.address	= encoder_ver_address;//@TODO
        //        encoder->param.id	        = encoder_ver_id;// //@TODO
        
        encoder->position 	                    = 0.0f;
        encoder->param.timestamp 	            = 0;
        encoder->param.zero_angle_correction    = 0.0f;	
        
        encoder->param.ms_mask 	                = encoder_zettlex_16_ms_mask;
        
        encoder->param.singleturn_bits          = encoder_zettlex_16_singleturn_bits;
        encoder->param.singleturn_max           = encoder_zettlex_16_singleturn_max;
        encoder->param.ticks_per_turn           = encoder_zettlex_16_ticks_per_turn;
        encoder->param.wrong_val                = encoder_zettlex_16_wrong_value;
        encoder->param.angle_per_tick	        = encoder_zettlex_16_angle_per_tick;
        
        break;
        case sensna_18: 
        
        //        encoder->param.address	= encoder_ver_address;//@TODO
        //        encoder->param.id	        = encoder_ver_id;// //@TODO
        
        encoder->position 	                    = 0.0f;
        encoder->param.timestamp 	            = 0;
        encoder->param.zero_angle_correction    = 0.0f;	
        
        encoder->param.ms_mask 	                = encoder_sensna_18_ms_mask;
        
        encoder->param.singleturn_bits          = encoder_sensna_18_singleturn_bits;
        encoder->param.singleturn_max           = encoder_sensna_18_singleturn_max;
        encoder->param.ticks_per_turn           = encoder_sensna_18_ticks_per_turn;
        encoder->param.wrong_val                = encoder_sensna_18_wrong_value;
        encoder->param.angle_per_tick	        = encoder_sensna_18_angle_per_tick;
        
        break; 
        default:
        return ERROR;
        break; 
    }
    return SUCCESS;
}

/**
* @brief encoders_update
*  обновление энкодеров
*  
* 
* @param[in] 
* @param[in] 
* @return 
*/
void encoders_update( void )
{
    
    encoder_get_usart_data(USART1,&encoders.turret_ver);
    encoder_get_ssi_data(SPI1,&encoders.turret_hor);
    
    encoder_update(&encoders.turret_hor);
    encoder_update(&encoders.turret_ver);
    
} //.encoders_update()


/**
* @brief encoder_update
*  обновление энкодера
*  
* 
* @param[in] encoder_t *encoder выбранный энкодер
* @param[in] 
* @return  статус
*/
ErrorStatus encoder_update( encoder_t *encoder )
{
    float naughty =(float) 0xFFFFFFFF;
    float  angle;
    switch(encoder->type)
    {
        case kubler_14: 
        angle = (float)angle_180(encoder_calculate_angle_kubler_14(encoder));///@TODO TEST переделать под актуальную работку кублера @TODO
        if( angle != naughty && encoder->param.data_new && encoder->param.position_raw != encoder->param.wrong_val)
        {
            encoder->position = angle;
            encoder->connection = true;			
            encoder->param.timestamp = systick_get();
        }
        else
        {
            encoder->connection = false;	
        }
        break;
        case china_16: 
        
        break;
        case zettlex_16: 
        
        angle = encoder_calculate_angle_zetlex_16(encoder);
        if( angle != naughty && encoder->param.data_new && encoder->param.position_raw != encoder->param.wrong_val)
        {
            encoder->position = angle;
            encoder->connection = true;			
            encoder->param.timestamp = systick_get();
        }
        else
        {
            encoder->connection = false;	
        }
        break;
        case sensna_18: 
        
        angle = encoder_calculate_angle_sensna_18(encoder);
        if( angle != naughty  && encoder->param.data_new && encoder->param.position_raw != encoder->param.wrong_val)
        {
            encoder->position = angle;
            encoder->connection = true;			
            encoder->param.timestamp = systick_get();
        }
        else
        {
            encoder->connection = false;	
        }
        break;
        default:
        return ERROR;
        break;
    }  
    
    return SUCCESS;
    
} //.encoder_update()

/**
* @brief encoder_calibrate
*  калибровка энкодера
*  
* 
* @param[in] encoder_t *encoder выбранный энкодер
* @param[in] 
* @return  статус
*/

ErrorStatus encoder_calibrate( encoder_t *encoder )
{     
      static const uint32_t time_delay = 10;
        uint8_t data_1[5]={0x51,0xCF,0x02,0x11,0x11};
        uint8_t data_2[5]={0x51,0xCF,0x02,0xF1,0xF1};
        uint8_t data_3[5]={0x51,0xCF,0x02,0xAB,0xAB};
        uint8_t data_4[5]={0x51,0xCF,0x02,0xF0,0xF0};
        uint8_t data_5[5]={0x51,0xCF,0x02,0xAB,0xAB};    
    switch(encoder->type)
    {
        case sensna_18: 

        //Sensna zero data

        //Sensna zero data
        if(encoder->protocol==ssi)
        {
                    usart_transmit(USART1,data_1, 5 );
                    delay_ms(time_delay);
                    usart_transmit(USART1,data_2, 5 );
                    delay_ms(time_delay);
                    usart_transmit(USART1,data_3, 5 );
                    delay_ms(time_delay+500);
                    usart_transmit(USART1,data_4, 5 );
                    delay_ms(time_delay);
                    usart_transmit(USART1,data_5, 5 );
                    delay_ms(time_delay+500);
            
            ///Доработать локально, много чести под это создавать глобальные вещи @TODO
            /* GPIO configuration struct */
            //            gpio_configuration_t gpio_config;    
            //            gpio_config.port     = SPI1_CLK_PORT;
            //            gpio_config.pin      = SPI1_CLK_PIN;
            //            gpio_config.mode     = gpio_output;
            //            gpio_config.speed    = very_high_speed;
            //            gpio_config.otype    = gpio_otype_pp;
            //            gpio_config.pupd     = gpio_pupd_up;
            //            gpio_config.state    = gpio_set;
            //            gpio_configuration ( &gpio_config );
            //            
            //            SoftUartInit(0, SPI1_CLK_PORT, 5, 0, 0); // Скорость передачи ограничена скокростью отдельного GPIO
            //            
            //            
            //            test = SoftUartPuts(0,data_1,  sizeof(data_1));
            //            soft_usart_with_dwt_delay_boud(0,9600);
            //            delay_ms(10);
            //            test = SoftUartPuts(0,data_2,  sizeof(data_2));
            //            soft_usart_with_dwt_delay_boud(0,9600);
            //            delay_ms(10);
            //            test = SoftUartPuts(0,data_3,  sizeof(data_3));
            //            soft_usart_with_dwt_delay_boud(0,9600);
            //            delay_ms(10+500);
            //            test = SoftUartPuts(0,data_4,  sizeof(data_4));
            //            soft_usart_with_dwt_delay_boud(0,9600);
            //            delay_ms(10);
            //            test = SoftUartPuts(0,data_5,  sizeof(data_5));
            //            soft_usart_with_dwt_delay_boud(0,9600);
            //            delay_ms(10+500);
            //            encoder_soft_usart_zero_flag=false;
            //            
            //            //SPI1_CLK - PA5
            //            gpio_config.port     = SPI1_CLK_PORT;
            //            gpio_config.pin      = SPI1_CLK_PIN;
            //            gpio_config.mode     = gpio_af;
            //            gpio_config.speed    = high_speed;
            //            gpio_config.otype    = gpio_otype_pp;
            //            gpio_config.pupd     = gpio_pupd_up;
            //            gpio_configuration ( &gpio_config );
            //            SPI1_CLK_PORT->AFR[0] |= (0x05 << GPIO_AFRL_AFSEL5_Pos); //AF = 5 
            
//            encoder->param.zero_wire_port->ODR |=  (0x1UL << encoder->param.zero_wire_pin);   
//            delay_ms (encoders_calibration_timeout);
//            encoder->param.zero_wire_port->ODR &=  ~(0x1UL << encoder->param.zero_wire_pin);   
//            delay_ms (encoders_calibration_timeout);
            ///
        }
        if(encoder->protocol==usart)
        {
            usart_transmit(USART1,data_1, 5 );
            delay_ms(time_delay);
            usart_transmit(USART1,data_2, 5 );
            delay_ms(time_delay);
            usart_transmit(USART1,data_3, 5 );
            delay_ms(time_delay+500);
            usart_transmit(USART1,data_4, 5 );
            delay_ms(time_delay);
            usart_transmit(USART1,data_5, 5 );
            delay_ms(time_delay+500);
            sesnsa_reselect_protocol(encoder,usart);
        }
        break;    
        
        case kubler_14:
        encoder->param.zero_wire_port->ODR |=  (0x1UL << encoder->param.zero_wire_pin);   
        delay_ms (encoders_calibration_timeout);
        encoder->param.zero_wire_port->ODR &=  ~(0x1UL << encoder->param.zero_wire_pin);   
        delay_ms (encoders_calibration_timeout);
        break;    
    }
    
    return SUCCESS;
}

/**
* @brief encoder_calculate_angle_kubler_14
*  функция просчета угла для энкодера типа Кублер 14 бит
*  
* 
* @param[in] encoder_t *encoder выбранный энкодер
* @param[in] 
* @return  float -180 +180 угол 
*/

float encoder_calculate_angle_kubler_14( encoder_t *encoder )
{
    float calculated_angle;   
    uint32_t encoder_value = encoder->param.position_raw & encoder->param.ms_mask;   
    uint32_t encoder_value_mt = encoder_value >> encoder->param.singleturn_bits;  
    calculated_angle = 1;
    if( encoder_value_mt > encoder->param.multiturn_max_half )
    {
        calculated_angle *= -1; 
        encoder_value_mt = encoder->param.multiturn_max - encoder_value_mt;			
        uint32_t encoder_value_st = encoder_value >> encoder->param.singleturn_bits;
        encoder_value_st = encoder->param.singleturn_max - encoder_value & encoder->param.singleturn_max;      
        encoder_value = (encoder_value_mt << encoder->param.singleturn_bits) | encoder_value_st;
    }  
    calculated_angle *= encoder_value * encoder->param.angle_per_tick / encoder->param.gear_ratio;  
    calculated_angle += encoder->param.zero_angle_correction;	
    return calculated_angle;
}

/**
* @brief encoder_calculate_angle_zetlex_16
*  функция просчета угла для энкодера типа Зетликс 16 бит
*  
* 
* @param[in] encoder_t *encoder выбранный энкодер
* @param[in] 
* @return  float -180 +180 угол 
*/

float encoder_calculate_angle_zetlex_16( encoder_t *encoder )
{           
    uint32_t encoder_value = encoder->param.position_raw & encoder->param.ms_mask;
    uint32_t payload = encoder_value & 0x003FFFFF; 
    _Bool position_valid = (encoder_value & (1 << 23)) != 0;  // 23-й бит для position_valid
    // _Bool zero_point = (encoder_value & (1 << 22)) != 0;  // 22-й бит для zero_point 
    if (!position_valid) {
        return 0;
    }
    float angle = (float)payload * encoder->param.angle_per_tick;
    float result = ((float)angle_180(angle) / (-1))/encoder->param.gear_ratio;
    return result;
}

/**
* @brief encoder_calculate_angle_sensna_18
*  функция просчета угла для энкодера типа Sensna 18 бит
*  
* 
* @param[in] encoder_t *encoder выбранный энкодер
* @param[in] 
* @return  float -180 +180 угол 
*/

float encoder_calculate_angle_sensna_18( encoder_t *encoder )
{                                      
    uint32_t encoder_value = encoder->param.position_raw  &  encoder->param.singleturn_max;//encoder->param.ms_mask;
    float angle = (float)encoder_value * encoder->param.angle_per_tick;
    float result = (float)angle_180(angle) ;
   // float result = ((float)angle_180(angle) / (-1))/encoder->param.gear_ratio;
    return result;
}
/*
* @brief sesnsa_reselect_protocol
*  смена протокола для связи
*  
* 
* @param[in] encoder_t *encoder выбранный энкодер
* @param[in]  protocol_type_t protocol выбранный протокол
* @return  статус
*/

ErrorStatus sesnsa_reselect_protocol(encoder_t *encoder, protocol_type_t protocol)
{     
    
    uint8_t data_to_usart[5]={0x51,0xCF,0x02,0x11,0x11};
    uint8_t data_to_ssi[5]={0x51,0xCF,0x02,0xF1,0xF1};
    
    switch(encoder->type)
    {
        case sensna_18: 
        if(protocol == usart)
        {
            usart_transmit(USART1,data_to_usart, 5 );
            encoder->protocol=usart;
        }
        else if(protocol == ssi)
        {
            usart_transmit(USART1,data_to_ssi, 5 );
            encoder->protocol=ssi;
        }
        break;
        
    }
    return SUCCESS;
}

/*
* @brief sesnsa_reselect_boudrate
*  смена баудрейта USART
*  
* 
* @param[in] encoder_t *encoder выбранный энкодер
* @param[in]  uint16_t boudrate  скорость 9600 57600 115200 или 460800
* @return  статус
*/

ErrorStatus sesnsa_reselect_boudrate(encoder_t *encoder,uint16_t boudrate )
{     
    uint8_t data_to_9600_usart[6]  ={0x51,0xCF,0x03,0x89,0x01,0x8A};
    uint8_t data_to_57600_usart[6] ={0x51,0xCF,0x03,0x89,0x02,0x8B};
    uint8_t data_to_115200_usart[6]={0x51,0xCF,0x03,0x89,0x03,0x8C};
    uint8_t data_to_460800_usart[6]={0x51,0xCF,0x03,0x89,0x04,0x8D};
    
    if((encoder->protocol)==usart  &&(encoder->type)==sensna_18)
    {
        switch(boudrate)
        {
            
            case 9600: 
            usart_transmit(USART1,data_to_9600_usart, 6 );
            return SUCCESS;
            break;
            case 57600: 
            usart_transmit(USART1,data_to_57600_usart, 6 );
            return SUCCESS;
            break;
            case 115200: 
            usart_transmit(USART1,data_to_115200_usart, 6 );
            return SUCCESS;
            break;
            case 460800: 
            usart_transmit(USART1,data_to_460800_usart, 6 );
            return SUCCESS;
            break;
            default:
            usart_transmit(USART1,data_to_460800_usart, 6 );
            return SUCCESS;
            break;
        }
        
    }
    return ERROR;
}

static bool end_sensna_parser = false;
/**
* @brief sensna_18_usart_parser
*  Фунеция парсера usart  даных 
* 
* 
* @param[in] 
* @param[in] 
* @return 
*/
uint32_t sensna_18_usart_parser(USART_TypeDef* USART_x )
{
	uint8_t data_len = uart_read_available(USART_x);
    uint8_t temp_byte;
    uint32_t data;
	end_sensna_parser=false;
	for( uint8_t i = 0; i < data_len; i++ )
	{
		temp_byte = usart_read_byte(USART_x);
		data=sensna_18_usart_packet_parser(temp_byte);
        if (end_sensna_parser)
        {
            data_len=0x00;
            usart_clear_rx_buf(USART_x);
            if (data == ERROR)
            {
                return ERROR;
            }
            else 
            {
                return data;
            }
        }
        
        
	}
    return ERROR;
    
}

#define sensna_header_1 0x5A
#define sensna_header_2 0x7E
#define sensna_protocol_flag_bit 0xC1 ///  спецефично для страны покупателя

//static bool end_sensna_parser = false;
/**
* @brief sensna_18_usart_packet_parser
*  Фунеция парсера usart  даных  побайтово, из пакета 
* 
* 
* @param[in] 
* @param[in] 
* @return 
*/
uint32_t sensna_18_usart_packet_parser(uint8_t byte)
{
    static uint8_t  parser_progress =0x01;
    static uint32_t position_data=0x00;
    static uint16_t frame_count=0x00;
    static uint16_t crc=0x00;
    static uint8_t  bytes[10];
    static uint16_t crc_soft=0x00;
    
    if(parser_progress<0x04)
    {
        switch (byte)
        {
            case sensna_header_1:
            if(parser_progress != 0x01)
            {
                parser_progress=0x01;
                return ERROR;
            }
            parser_progress++;//0x02
            position_data=0x00;
            bytes[0]=byte;
            break;
            case sensna_header_2:
            if(parser_progress != 0x02)
            {
                parser_progress=0x01;
                return ERROR;
            }
            parser_progress++;//0x03
            bytes[1]=byte;
            break;
            case sensna_protocol_flag_bit:
            if(parser_progress != 0x03)
            {
                parser_progress=0x01;
                return ERROR;
            }
            parser_progress++;//0x04
            bytes[2]=byte;
            break;
            default:
            parser_progress =0x01;
            return ERROR;
            break;
        }
    }
    else if(parser_progress<0x0B && parser_progress>0x03)
    {
        switch (parser_progress)
        {
            case 0x04:
            parser_progress++;//0x05
            position_data |= byte<<16;
            bytes[3]=byte;
            break;
            case 0x05:
            parser_progress++;//0x06
            position_data |= byte<<8;
            bytes[4]=byte;
            break;
            case 0x06:
            parser_progress++;//0x07
            position_data |= byte;
            bytes[5]=byte;
            break;
            case 0x07:
            frame_count |= byte<<8; 
            parser_progress++;//0x08
            bytes[6]=byte;
            break;
            case 0x08:
            frame_count |= byte; 
            parser_progress++;//0x09
            bytes[7]=byte;
            break;
            case 0x09:
            crc |= byte<<8; 
            parser_progress++;//0x0A
            bytes[8]=byte;
            break;
            case 0x0A:
            crc |= byte; 
            parser_progress++;//0x0B
            bytes[9]=byte;
            break;
            default:
            parser_progress =0x01;
            return ERROR;
            break;
        }
        
    }
    if(parser_progress==0x0B)
    {
        //  crc = (uint16_t)((bytes[8]<<8)+bytes[9]);
        crc_soft = ((uint16_t)((bytes[2]<<8)+bytes[3])+(uint16_t)((bytes[4]<<8)+bytes[5])+ (uint16_t)((bytes[6]<<8)+bytes[7])) ;
        
        
        if (crc == crc_soft)
        {  
            parser_progress =0x01;
            frame_count=0x00;
            crc=0x00;
            crc_soft =0x00;
            memset(bytes,0x00,10);           
            end_sensna_parser=true;
            uint32_t ret = position_data;
            return ret;
        }
        else 
        {
            parser_progress =0x01;
            frame_count=0x00;
            crc=0x00;
            crc_soft =0x00;
            memset(bytes,0x00,10);
            end_sensna_parser=true;
            return ERROR;
        }
        
        
    }
    
    return SUCCESS;
}