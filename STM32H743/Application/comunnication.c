#include "comunnication.h"
//#include "usbd_cdc_if.h"
//#include "stm32h7xx_crc.h"
#include "stm32h7xx_usart.h"
#include <string.h>

/**
******************************************************************************
* File Name          : .c
* Description        : 
*                      
******************************************************************************
* @attention
* 
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
uint8_t communication_rx_preambule[2] = {0x74,0x47};
uint8_t communication_tx_preambule[2] = {0x47,0x74};
static  uint8_t Error_string[]    = "ERROR";
static  uint8_t Error_crc_string[]    = "ERROR CRC";
uint16_t error_data_counter;
uint32_t CRC_buffer[32];

computer_command_message_t computer_command_message;
/****************************************************************************/
/******************************* FUNCTIONS **********************************/
void computer_data_awalible(computer_command_message_t message);

/****************************************************************************/
/************************  HW DEPENDENT FUNCTIONS  **************************/


/****************************************************************************/
/**********************  HW UNDEPENDENT FUNCTIONS  **************************/

/**
* @brief computer_tx_data
* 
*  send computer_tx_data(111,0.0f,0x00); if emegrency stop has been
* @param[in] 
* @param[in] 
* @return 
*/
void computer_tx_data(uint16_t burst_was, float with_freq, uint8_t power_precent){  
//    uint8_t tx_data[64]={0x00,};
//    uint32_t CRC_32=0;
//    size_t total_size = sizeof(communication_tx_preambule) ;
//    total_size += sizeof(uint8_t) ;
//    total_size += sizeof(burst_was) ;
//    total_size += sizeof(with_freq);
//    total_size += sizeof(power_precent);
//    
//    size_t offset = 0;
//    memcpy(tx_data,communication_tx_preambule,sizeof(communication_tx_preambule));
//    offset  +=  sizeof(communication_tx_preambule);
//    memcpy(tx_data+offset,&total_size,sizeof(uint8_t));
//    offset  +=  sizeof(uint8_t);
//    memcpy(tx_data+offset,&burst_was,sizeof(burst_was));
//    offset  +=  sizeof(burst_was);
//    memcpy(tx_data+offset,&with_freq,sizeof(with_freq));
//    offset  +=  sizeof(with_freq);
//    memcpy(tx_data+offset,&power_precent,sizeof(power_precent));
//    offset  +=  sizeof(power_precent);
//    
//    //crc
//    memset(CRC_buffer,0x00,sizeof(CRC_buffer));
//    uint8_t packet_data_size = total_size;
//    memcpy(&CRC_buffer,tx_data+2,packet_data_size-2); //crcбез преамбулы
//    uint16_t test =(uint16_t)ceilf((float)(packet_data_size-2)/4.0f);
//    CRC_32=crc_hw_buf_calculate(CRC_buffer,test);
//    memcpy(tx_data+offset,&CRC_32,sizeof(CRC_32));
//    packet_data_size+=sizeof(CRC_32);
//    
//    usart_send_data(USART1,tx_data,packet_data_size);
//    CDC_Transmit_FS(tx_data,packet_data_size);
}


void computer_parser_data(void)
{
//    uint8_t data_len = uart_read_available(USART1);
//    uint8_t rx_buf[64]={0x00,};
//    if(data_len>7)
//    {
//        // data_len=sizeof(slave_device_message_t)+16;
//        memset(rx_buf,0x00,sizeof(rx_buf));
//        for( uint8_t i = 0; i < data_len; i++ )
//        {
//            rx_buf[i] = usart_read_byte(USART1);
//        }
//        
//        computer_packet_parser_data(rx_buf,data_len);
//
//        usart_clear_rx_buf(USART1);
//    }
//    
}

void computer_packet_parser_data(uint8_t* data, uint8_t size){
//    uint8_t tx_ready_buf[64]={0x00,};
//    uint8_t parcer_progress=0x00;
//    uint8_t packet_data_size=0x00;
//    uint8_t data_in_packet_real_offset=0x00;
//    uint32_t crc_32_in_data=0x00;
//    uint32_t crc_32_from_data=0x00;
//    uint32_t crc_32_from_data_sw=0x00;
//    
//    memset(tx_ready_buf,0x00,sizeof(tx_ready_buf));
//    memcpy(tx_ready_buf,data,size);
//    
//    for(uint8_t i=0; i<size+7;i++)
//    {
//        switch (parcer_progress)
//        {
//            
//            case 0:
//            if(tx_ready_buf[i]==communication_rx_preambule[0])
//            {
//                parcer_progress++; 
//            }
//            break;
//            case 1:
//            if(tx_ready_buf[i]==communication_rx_preambule[1])
//            {
//                parcer_progress++; 
//            }
//            break;
//            case 2:
//            packet_data_size=tx_ready_buf[i];
//            data_in_packet_real_offset=i-2;
//            parcer_progress++; 
//            if(packet_data_size != (sizeof(computer_command_message_t)-sizeof(uint32_t)))
//            {
//                i=size+10;
//                error_data_counter++;
//                usart_send_data(USART1,Error_string,sizeof(Error_string));
//                CDC_Transmit_FS(Error_string,sizeof(Error_string));
//                
//            }
//            break;
//            case 3://crc
//            
//            memset(&computer_command_message,0x00,sizeof(computer_command_message_t));
//            memset(CRC_buffer,0x00,sizeof(CRC_buffer));
//            memcpy(CRC_buffer,tx_ready_buf+data_in_packet_real_offset,packet_data_size);
//            uint16_t test =(uint16_t)ceilf((float)packet_data_size/4.0f);
//            crc_32_from_data=crc_hw_buf_calculate(CRC_buffer,test);
//            crc_32_from_data_sw=crc_sw_buf_calculate(CRC_buffer,test);
//            crc_32_in_data|=(tx_ready_buf[data_in_packet_real_offset+packet_data_size+3]<<24) | (tx_ready_buf[data_in_packet_real_offset+packet_data_size+2]<<16) | (tx_ready_buf[data_in_packet_real_offset+packet_data_size+1] <<8) | (tx_ready_buf[data_in_packet_real_offset+packet_data_size]);
//            if(crc_32_in_data==crc_32_from_data)
//            {
//                parcer_progress++; 
//            }
//            else
//            {
//                //  parcer_progress++; 
//                i=size+10;
//                error_data_counter++;
//                usart_send_data(USART1,Error_crc_string,sizeof(Error_crc_string));
//                CDC_Transmit_FS(Error_crc_string,sizeof(Error_crc_string));
//            }
//            break;
//            case 4:
//            memset(tx_ready_buf,0x00,sizeof(tx_ready_buf));
//            memcpy(tx_ready_buf,data,packet_data_size);
//            memcpy(&computer_command_message,tx_ready_buf,sizeof(computer_command_message_t));
//            computer_data_awalible(computer_command_message);
//            //   burst_outside_start(test_burst,test_freq);
//            
//            i=size+10;
//            break;
//            default:
//            i=size+10;
//            error_data_counter++;
//            break;
//            
//            
//        }
//    }
//    
//    
}
void send_welcome(){
    static  uint8_t welcome_string[]    = "WELCOME";
    usart_send_data(USART1,welcome_string,sizeof(welcome_string));
    CDC_Transmit_FS(welcome_string,sizeof(welcome_string));  
}
void send_alive(){
    static  uint8_t alive_string[]    = "alive";
    usart_send_data(USART1,alive_string,sizeof(alive_string));
    CDC_Transmit_FS(alive_string,sizeof(alive_string));  
}
void computer_data_awalible(computer_command_message_t message){
//    if((message.frequency==0.0f) || (message.power==0x00)) 
//    {
//        emergency_stop();
//    }
//    
//    if((message.frequency==33.3f) || (message.power==0x0A)) 
//    {
//        NVIC_SystemReset();
//    }
//    
//    burst_outside_start(message.burst,message.frequency,message.power);
//    memset(&message,0x00,sizeof(computer_command_message_t));
}



/**
* @}
*/






