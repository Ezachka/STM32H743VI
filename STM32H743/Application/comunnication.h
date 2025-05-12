/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef COMUNNICATION_L
#define COMUNNICATION_L

#ifdef __cplusplus
extern "C" {
#endif
    
    /******************************* INCLUDES *************************************/
#include "stm32h7xx.h"

    
#include "stm32h7xx_usart.h"
    
    
    
    
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
    /******************************************************************************/
    
    
    
    /******************************* DEFINITIONS **********************************/        
    
    /******************************* Constants **********************************/    
    
    // send computer_tx_data(111,0.0f,0x00);  (burst_was is 111 and orthers is zero) if emegrency stop has been
    // emergency stop if got  message with freq = 0.0f or power=0x00 
    
    /******************************* STRUCTURES **********************************/
    /**
    * @struct computer_command_message
    * @brief  computer_command_message 
    *  freq or power == 0 it's emergenccy stop
    *
    *
    * 
    */ 
    typedef struct __attribute__((packed))
    {
        uint8_t                     Preambula[2];          // ?????????
        uint8_t                     size;         // ???? ???? ????, ?? ??????
        uint16_t                    burst;                // ????????
        uint8_t                     power;         // ???? ???? ????, ?? ??????
        float                       frequency;
        uint32_t                    CRC_32;
    }computer_command_message_t;
    
    
    
    /******************************* FUNCTIONS ************************************/
    void computer_tx_data(uint16_t burst_was, float with_freq, uint8_t power_precent);
    void computer_packet_parser_data(uint8_t* data, uint8_t size);
    void computer_parser_data(void);
    void computer_data_awalible(computer_command_message_t message);
    void send_welcome();
    void send_alive();
    
    
#ifdef __cplusplus
}
#endif

#endif /* comunnication_L */

/************************ *****END OF FILE****/ 