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
#include "ad_9833.h"

#include "gpio_config.h"
#include "leds.h"
#include "SH1106.h"
#include "bitmap.h"
#include "lis3dh.h"
#include "encoder.h"
#include "softuart.h"

#include "display_xxxx.h"

//#include "virtual_com_port.h"
/* CM7 Main */

init_status_t init_status;

char line_1[20]="    Hello";
char line_2[20]="    World!";
char line_11[20]="    You are";
char line_22[20]="     GAY!";
uint8_t buff_test_8[32];
uint16_t buff_test_16[16];
uint8_t buff_test_size=4;
float freq=1000.0f;
int main()
{ 
    init_status.rcc_init_status=rcc_clock_init(); 
    SystemCoreClockUpdate ();
    SysTick_Config(SystemCoreClock/1000);   //SysTick init = 1kHz
    dwt_init();
    
    gpio_init(); 
   // usb_init();
    led_init();  
    init_status.i2c_init_status.i2c_1 = i2c_init(I2C1);
    init_status.spi_init_status.spi_1 = spi_init(SPI1);
    init_status.usart_init_status.usart_1 = usart_init (USART1,9600,even_parity,none_swap);
    
    //  SoftUartInit(0, USART_SW_TX_PORT, 12, 0, 0); //
    lcd_xxxx_init(I2C1,DISPLAY_XXXX_I2C_ADDR,16, 2,true);
    //  SH1106_Init();
        ad_9833_init(SPI1);
    for(uint8_t i = 0; i<32;i++){
        buff_test_8[i]=i+2;
    }
    for(uint8_t i = 0; i<16;i++){
        buff_test_16[i]=i+2;
    }
	while(1)
	{
        leds_update();
        if( systick_f )
        {  
            systick_f = false;  
        }
        if( systick_div_500hz )
        {  
            systick_div_500hz = false;  
        }
        if( systick_div_200hz )
        {  
            systick_div_200hz = false;  
        }
        if( systick_div_100hz )
        {  
            systick_div_100hz = false;  
            lcd_xxxx_config_update(I2C1);
//                        freq+=1;
//            if (freq>4000){
//                freq=1000.0f;
//            }
//            ad_9833_set_freq(freq);
            ad_9833_update(SPI1);
        }
        
        if( systick_div_10hz )
        {    
            systick_div_10hz = false; 

        }
        if( systick_div_1hz )
        {  
            systick_div_1hz = false; 
            if((BUTTON_1_PORT->IDR & BUTTON_1_PIN)){
                lcd_xxxx_change_line(line_1,1);
                lcd_xxxx_change_line(line_2,2);
            }
            else{
                lcd_xxxx_change_line(line_11,1);
                lcd_xxxx_change_line(line_22,2);
            }
                    


        }
    }//. while(1)
    
}//. main

