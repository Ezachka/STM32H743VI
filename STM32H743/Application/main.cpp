#include "stm32h743xx.h"
#include "stm32h7xx.h"
#include "stm32h7xx_rcc.h"
#include "stm32h7xx_i2c.h"
#include "stm32h7xx_tim.h"
#include "stm32h7xx_spi.h"
#include "stm32h7xx_usart.h"

#include "stm32h7xx_gpio.h"   
#include "gpio_config.h"

#include "peripherals_init.h"
#include "stm32h7xx_interrupts.h"
#include "dwt_tim.h"

#include "gpio_config.h"
#include "leds.h"
#include "SH1106.h"
#include "bitmap.h"
#include "lis3dh.h"
#include "encoder.h"
#include "softuart.h"

/* CM7 Main */

uint8_t  test_data_tx [128] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11};
uint8_t  test_data_tx_2 [128] = {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};
uint16_t test_size_tx =3;
uint8_t  test_data_rx [128] ;
uint16_t test_size_rx =10;
uint32_t data_32_test_rx [10];
uint8_t  test;
uint32_t bod = 460800;
uint32_t tick = 1;
bool  encoder_to_usart_flag=false;
bool  encoder_to_ssi_flag=false;
bool  encoder_calibrate_flag=false;
bool encoder_9600_flag=false;
bool encoder_460800_flag=false;
bool encoder_soft_usart_zero_flag=false;

extern encoders_t encoders;
init_status_t init_status;


int main()
{ 
    init_status.rcc_init_status=rcc_clock_init(); 
    SystemCoreClockUpdate ();
    SysTick_Config(SystemCoreClock/1000);   //SysTick init = 1kHz
//    dwt_init();
//    gpio_init(); 
//    led_init();  
//    init_status.i2c_init_status.i2c_1 = i2c_init(I2C1);
//    init_status.spi_init_status.spi_1 = spi_init(SPI1);
//   // init_status.usart_init_status.usart_1 = usart_init (USART1,9600,even_parity,none_swap);
//    init_status.usart_init_status.usart_1 = usart_init (USART1,460800,even_parity,none_swap);
//    encoders_init();
    

    SoftUartInit(0, USART_SW_TX_PORT, 12, 0, 0); // ???????? ???????? ?????????? ?????????? ?????????? GPIO
    
    SH1106_Init();
    
	while(1)
	{
        
        if( systick_f )
        {  
            systick_f = false;  
        }
        if( systick_div_400hz )
        {  
            systick_div_400hz = false;  
        }
        if( systick_div_500hz )
        {  
            systick_div_500hz = false;  
        }
        if( systick_div_100hz )
        {  
            systick_div_100hz = false;  
        }
        
        if( systick_div_10hz )
        {    
            ///display test
            SH1106_GotoXY(10, 10);
            SH1106_Puts("STM32H743",&Font_7x10,SH1106_COLOR_WHITE);      
            SH1106_UpdateScreen();
            ///display test
        }
        if( systick_div_1hz )
        {  
            systick_div_1hz = false; 

        }
    }//. while(1)
    
}//. main

