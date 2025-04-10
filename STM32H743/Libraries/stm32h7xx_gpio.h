
#ifndef STM32H7XX_GPIO_H
#define STM32H7XX_GPIO_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include "stm32h7xx.h"
#include "stm32h743xx.h"

/*   ________________________________________________________________
    |  MODE(i) |OTYPE(i)| OSPEED(i) |PUPD(i)|   I/O CONFIGURATION    |     
    |   [1:0]  |        |   [1:0]   | [1:0] |                        |
    |__________|________|___________|_______|________________________|
    |          |    0   |           | 0 | 0 | GP OUTPUT |   PP       |    
    |          |    0   |           | 0 | 1 | GP OUTPUT |   PP+PU    |  
    |          |    0   |           | 1 | 0 | GP OUTPUT |   PP+PD    |
    |          |    0   |           | 1 | 1 | Reserved  |            |   
    |    01    |    1   |SPEED[1:0] | 0 | 0 | GP OUTPUT |   OD       |
    |          |    1   |           | 0 | 1 | GP OUTPUT |   OD+PU    |
    |          |    1   |           | 1 | 0 | GP OUTPUT |   OD+PD    |
    |          |    1   |           | 1 | 1 | Reserved  |            |
    |__________|________|___________|_______|___________|____________|
    |          |    0   |           | 0 | 0 | AF        |   PP       |
    |          |    0   |           | 0 | 1 | AF        |   PP+PU    |
    |          |    0   |           | 1 | 0 | AF        |   PP+PD    |
    |          |    0   |           | 1 | 1 | Reserved  |            |
    |    10    |    1   |SPEED[1:0] | 0 | 0 | AF        |   OD       |
    |          |    1   |           | 0 | 1 | AF        |   OD+PU    |
    |          |    1   |           | 1 | 0 | AF        |   OD+PD    |
    |          |    1   |           | 1 | 1 | Reserved  |            |
    |__________|________|___________|_______|___________|____________|
    |          |    x   |     x     | 0 | 0 | INPUT     | Floating   |
    |          |    x   |     x     | 0 | 1 | INPUT     | PU         |
    |    00    |    x   |     x     | 1 | 0 | INPUT     | PD         |
    |          |    x   |     x     | 1 | 1 | Reserved (floating)    |
    |__________|________|___________|_______|________________________|
    |          |    x   |     x     | 0 | 0 | Analog (input/output)  |
    |          |    x   |     x     | 0 | 1 | Reserved               |
    |    11    |    x   |     x     | 1 | 0 | Analog,PD(input/output)|
    |          |    x   |     x     | 1 | 1 | Reserved               |
    |__________|________|___________|_______|________________________|

     _____________________________
    | OSPEED(i) |                 |
    |   [1:0]   | CONFIGURATION   |
    |___________|_________________|
    |    00     | Low speed       |
    |    01     | Medium speed    |
    |    10     | High speed      |
    |    11     | Very high speed |
    |___________|_________________|

*/  

    
    typedef enum {
        low_speed            = 0x00,   
        medium_speed         = 0x01,
        high_speed           = 0x02,
        very_high_speed      = 0x03
    } gpio_speed_t;
    
    typedef enum {
        gpio_input           = 0x00,
        gpio_output          = 0x01,
        gpio_af              = 0x02,
        gpio_analog          = 0x03
    } gpio_mode_t;
    
    typedef enum {
        gpio_otype_pp        = 0x00,
        gpio_otype_od        = 0x01
    } gpio_otype_t;
    
    typedef enum {
        gpio_pupd_nopull     = 0x00,
        gpio_pupd_up         = 0x01,
        gpio_pupd_down       = 0x02,
    } gpio_pupd_t;
    
    typedef enum {
        gpio_reset           = 0x00,
        gpio_set             = 0x01
    } gpio_state_t;
    
    typedef struct{
        GPIO_TypeDef*        port;
        uint32_t             pin;
        gpio_mode_t          mode;
        gpio_speed_t         speed;
        gpio_otype_t         otype;
        gpio_pupd_t          pupd;
        gpio_state_t         state;
    }gpio_configuration_t;
    
    void gpio_configuration ( gpio_configuration_t* gpio);

#ifdef	__cplusplus
extern "C" {
#endif

#endif //STM32H7XX_GPIO_H
