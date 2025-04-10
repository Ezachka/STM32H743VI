
#ifndef DISPLAY_XXXX_H
#define DISPLAY_XXXX_H

#ifdef __cplusplus
extern C {
#endif

#include "stm32h7xx_i2c.h"
#include "stm32h7xx_interrupts.h"
#include "stdlib.h"
#include "string.h"
#include <stdbool.h>
#include <stdio.h>  
    
/****************************************************************************/
/****************************** DEFINITIONS *********************************/
    /* I2C address */
#define DISPLAY_XXXX_I2C_ADDR               0x27
        
#define DISPLAY_XXXX_SEND_SETTINGS          0x00
#define DISPLAY_XXXX_SEND_DATA              0x01

        /* service bits */
#define DISPLAY_XXXX_RS                     0x01
#define DISPLAY_XXXX_RW                     0x02
#define DISPLAY_XXXX_EN                     0x04
    
        /* commands */
#define DISPLAY_XXXX_CMD_CLEAR              0x01
#define DISPLAY_XXXX_CMD_RETURN_HOME        0x02
#define DISPLAY_XXXX_CMD_ENTRY_MODE_SET     0x04
#define DISPLAY_XXXX_CMD_DISPLAY_CONTROL    0x08
#define DISPLAY_XXXX_CMD_CURSOR_SHIFT       0x10  
#define DISPLAY_XXXX_CMD_FUNCTION_SET       0x20  
#define DISPLAY_XXXX_CMD_SET_CGRAMA_DDR     0x40   
#define DISPLAY_XXXX_CMD_SET_DDRAMA_DDR     0x80   
    /* flags entry mode */
#define DISPLAY_XXXX_ENTRY_RIGHT            0x00
#define DISPLAY_XXXX_ENTRY_LEFT             0x02
#define DISPLAY_XXXX_ENTRY_SHIFT_INCREMENT  0x01
#define DISPLAY_XXXX_ENTRY_SHIFT_DECREMENT  0x00

    /* flags display control*/
#define DISPLAY_XXXX_DISPLAY_ON             0x04
#define DISPLAY_XXXX_DISPLAY_OFF            0x00
#define DISPLAY_XXXX_CURSOR_ON              0x02
#define DISPLAY_XXXX_CURSOR_OFF             0x00
#define DISPLAY_XXXX_BLINK_ON               0x01
#define DISPLAY_XXXX_BLINK_OFF              0x00

    /* flags cursor shift */
#define DISPLAY_XXXX_DISPLAY_MOVE           0x08
#define DISPLAY_XXXX_CURSOR_MOVE            0x00
#define DISPLAY_XXXX_MOVE_RIGHT             0x04
#define DISPLAY_XXXX_MOVE_LEFT              0x00

    /* flags and functions set */
#define DISPLAY_XXXX_8_BIT_MODE             0x10
#define DISPLAY_XXXX_4_BIT_MODE             0x00
#define DISPLAY_XXXX_2_LINE                 0x08
#define DISPLAY_XXXX_1_LINE                 0x00
#define DISPLAY_XXXX_5x10_DOTS              0x04
#define DISPLAY_XXXX_5x8_DOTS               0x00

    /* flags of backlight */
#define DISPLAY_XXXX_BACKLIGHT              0x08
#define DISPLAY_XXXX_NO_BACKLIGHT           0x00
    
    /****************************************************************************/    
    /******************************* STRUCTS ************************************/
    typedef struct{
        char            name[4]; 
        uint8_t         adr;
        uint8_t         columns;
        uint8_t         lines;
        bool            connect;
        struct{
            uint8_t     entry_mode;
            uint8_t     functions;
            uint8_t     display_control;
            uint8_t     cursor_shift;
        }regs;
        struct{
            bool        clear_display;
            bool        return_home;
        }operation;
        struct{ 
            bool        display_on;
            bool        cursor_on;
            bool        cursor_blink_on;
            bool        black_light_on;
            bool        interface_8_or_4;
            bool        line_one_or_two;
            bool        dot_size_h;
        }settings;
        
        
    }display_xxxx_t;
    /****************************************************************************/  
    ErrorStatus lcd_xxxx_init(I2C_TypeDef* I2C_x,uint8_t adr,uint8_t columns, uint8_t lines,bool back_light);
    ErrorStatus lcd_xxxx_config_update(I2C_TypeDef* I2C_x);
ErrorStatus lcd_xxxx_write_on_line(I2C_TypeDef* I2C_x,char *str, uint8_t line);
ErrorStatus lcd_xxxx_clear(I2C_TypeDef* I2C_x);
ErrorStatus lcd_xxxx_home(I2C_TypeDef* I2C_x);
ErrorStatus lcd_xxxx_cursor(I2C_TypeDef* I2C_x,uint8_t column, uint8_t line);
ErrorStatus lcd_xxxx_off(I2C_TypeDef* I2C_x);
ErrorStatus lcd_xxxx_on(I2C_TypeDef* I2C_x);
ErrorStatus lcd_xxxx_cursor_off(I2C_TypeDef* I2C_x);
ErrorStatus lcd_xxxx_cursor_on(I2C_TypeDef* I2C_x);
ErrorStatus lcd_xxxx_cursor_blink_off(I2C_TypeDef* I2C_x);
ErrorStatus lcd_xxxx_cursor_blink_on(I2C_TypeDef* I2C_x);
ErrorStatus lcd_xxxx_scroll_left(I2C_TypeDef* I2C_x);
ErrorStatus lcd_xxxx_scroll_right(I2C_TypeDef* I2C_x);
ErrorStatus lcd_xxxx_text_left_to_right(I2C_TypeDef* I2C_x);
ErrorStatus lcd_xxxx_text_right_to_left(I2C_TypeDef* I2C_x);
ErrorStatus lcd_xxxx_autoscroll_on(I2C_TypeDef* I2C_x);
ErrorStatus lcd_xxxx_autoscroll_off(I2C_TypeDef* I2C_x);
#ifdef __cplusplus
}
#endif

#endif
