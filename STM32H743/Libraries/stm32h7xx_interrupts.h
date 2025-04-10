
#ifndef STM32H7XX_INTERRUPTS_H
#define STM32H7XX_INTERRUPTS_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "stm32h743xx.h"
    
#include "stm32h7xx.h"
#include <stdbool.h>
    extern volatile _Bool systick_f;
    extern volatile _Bool systick_div_1hz;
    extern volatile _Bool systick_div_5hz;
    extern volatile _Bool systick_div_10hz;
    extern volatile _Bool systick_div_100hz;
    extern volatile _Bool systick_div_200hz;
    extern volatile _Bool systick_div_500hz;
    uint32_t systick_get ( void );
    uint32_t systick_get_us ( void );
    void delay_ms (uint16_t ms);
    
    
#ifdef	__cplusplus
}
#endif

#endif //STM32H7XX_INTERRUPTS_H
