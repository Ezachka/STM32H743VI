#ifndef STM32H7XX_RCC_H
#define STM32H7XX_RCC_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "stm32h743xx.h"
    
#include "stm32h7xx.h"
#include <stdbool.h>   
    
    
    typedef enum
    {
        hse,    
        hsi,    
        lse,
        lsi
    }sources_t;
    
    
    typedef struct {
        uint8_t  DIVM;
        uint16_t DIVN;
        uint8_t  DIVP;
        uint8_t  DIVQ;
        uint8_t  DIVR;
        uint32_t pll_clk_p;
        uint32_t pll_clk_q;
        uint32_t pll_clk_r;
    }pll_t;   
    
    typedef struct
    {
        struct{
            uint32_t hse;       /*!<  HSE clock frequency expressed in Hz  */
            uint32_t hsi;       /*!<  HSI clock frequency expressed in Hz  */
            uint32_t csi;       /*!<  CSI clock frequency expressed in Hz  */ 
            uint32_t lse;       /*!<  LSE clock frequency expressed in Hz  */ 
            uint32_t lsi;       /*!<  LSI clock frequency expressed in Hz  */ 
        }source_val;
        sources_t pll_source_select;
        
        struct{
            uint32_t sysclk;    /*!<  SYSCLK clock frequency expressed in Hz */
            uint32_t cpu;      /*!<  clock frequency expressed in Hz   */
            uint32_t hclk;      /*!<  clock frequency expressed in Hz   */           
            uint32_t apb_1;      /*!<  clock frequency expressed in Hz  */
            uint32_t apb_2;      /*!<  clock frequency expressed in Hz  */ 
        } bus_freq;
        
        pll_t pll;
 
        struct {
            uint16_t  HPRE;
            uint8_t  D2PPRE1;
            uint8_t  D2PPRE2;
        }prescaler;
        
    }rcc_clocks_t;
    
    
    extern rcc_clocks_t rcc_clocks;
    
    
    ErrorStatus rcc_clock_init ( void );
    void rcc_clocks_get (rcc_clocks_t* _rcc_clocks);
    
    
    void core_m7_init_status_set(bool status);
    bool core_m7_init_status_get(void);
    void core_m4_init_status_set(bool status);
    bool core_m4_init_status_get(void);
    
#ifdef	__cplusplus
}
#endif

#endif //STM32H7XX_RCC_H
