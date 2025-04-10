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

#include "display_xxxx.h"
/* CM7 Main */

init_status_t init_status;


int main()
{ 
    init_status.rcc_init_status=rcc_clock_init(); 
    SystemCoreClockUpdate ();
    SysTick_Config(SystemCoreClock/1000);   //SysTick init = 1kHz
    dwt_init();
    
    gpio_init(); 
    led_init();  
    init_status.i2c_init_status.i2c_1 = i2c_init(I2C1);
    init_status.spi_init_status.spi_1 = spi_init(SPI1);
    init_status.usart_init_status.usart_1 = usart_init (USART1,9600,even_parity,none_swap);
    
    //  SoftUartInit(0, USART_SW_TX_PORT, 12, 0, 0); //
    lcd_xxxx_init(I2C1,DISPLAY_XXXX_I2C_ADDR,16, 2,true);
    lcd_xxxx_write_on_line(I2C1,"    Hello",1);
    lcd_xxxx_write_on_line(I2C1,"    World!",2);
    //  SH1106_Init();
    
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
        }
        
        if( systick_div_10hz )
        {    
            systick_div_10hz = false; 
            ///display test
            //            SH1106_GotoXY(10, 10);
            //            SH1106_Puts("STM32H743",&Font_7x10,SH1106_COLOR_WHITE);      
            //            SH1106_UpdateScreen();

            ///display test
        }
        if( systick_div_1hz )
        {  
            systick_div_1hz = false; 
             lcd_xxxx_config_update(I2C1);

    lcd_xxxx_write_on_line(I2C1,"    Hello",1);
    lcd_xxxx_write_on_line(I2C1,"    World!",2);
//            lcd_xxxx_cursor(I2C1,5, 1);
        }
    }//. while(1)
    
}//. main

