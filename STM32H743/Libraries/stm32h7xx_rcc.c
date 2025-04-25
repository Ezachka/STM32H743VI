
#include "stm32h7xx_rcc.h"
#include "system_stm32h7xx.h"
#include "stm32h7xx_interrupts.h"


void HPRE_config(void);
void D2PPRE1_config(void);
void D2PPRE2_config(void);

rcc_clocks_t rcc_clocks = {
    
    .pll_source_select = hse, 
       
    .source_val.hse = HSE_VALUE,
    .source_val.hsi = HSI_VALUE,
    .source_val.lse = 32768,
    .source_val.lsi = 32000,
    
    .pll.DIVM = 14, 
    .pll.DIVN = 84,   
    .pll.DIVP = 2, 
    .pll.DIVQ = 8, 
    .pll.DIVR = 2,
    
    .prescaler.HPRE    = 1, 
    .prescaler.D2PPRE1 = 1,
    .prescaler.D2PPRE2 = 1, 
};

ErrorStatus rcc_clock_init ( void )
{
    
#ifdef CORE_CM7
    RCC->CR |=   RCC_CR_HSEON; 
    while ( !(RCC->CR & RCC_CR_HSERDY) ){}              //Wait until the HSE oscillator is stable
    
    /* PLL1 Configuration */
    
    switch(rcc_clocks.pll_source_select){
        case hse:
        RCC->PLLCKSELR |= RCC_PLLCKSELR_PLLSRC_HSE;         //PLL1 CLOCK SORCE =  HSE 
        break;
        case hsi:
        RCC->PLLCKSELR |= RCC_PLLCKSELR_PLLSRC_HSI;         //PLL1 CLOCK SORCE =  HSI 
        break;
//        case csi:
//        RCC->PLLCKSELR |= RCC_PLLCKSELR_PLLSRC_CSI;         //PLL1 CLOCK SORCE =  CSI 
//        break;
    }
    
    RCC->PLLCKSELR &= ~(RCC_PLLCKSELR_DIVM1_Msk);       //DIVM1 clear
    RCC->PLLCKSELR |=  (rcc_clocks.pll.DIVM << RCC_PLLCKSELR_DIVM1_Pos);   //DIVM1 set 
    
    /* PLL1 Configuration */
    RCC->CR &= ~(RCC_CR_PLL1ON);                         //PLL1 OFF
    while ( (RCC->CR & RCC_CR_PLL1RDY) ){}               //Wait until the PLL1 OFF 
    
    RCC->PLLCFGR |= (RCC_PLLCFGR_PLL1VCOSEL);              // Freq measurment as 150 to 420 MHz
    RCC->PLLCFGR |=  (0x01 << RCC_PLLCFGR_PLL1RGE_Pos);     // 25/7=3.3 MHZ in  
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLL1FRACEN_Msk);          //PLL1FRACEN CLEAR  Дробное значение для DIVN1, например DIVN1= 50, и если FRACEN 3, То будет DIVN1 = 50+3/50, можно менять на горячую!
    
    RCC->PLLCFGR |=  RCC_PLLCFGR_DIVP1EN;       // DIVP1 ENABLE
    RCC->PLLCFGR |=  RCC_PLLCFGR_DIVQ1EN;       // DIVQ1 ENABLE
    RCC->PLLCFGR |=  RCC_PLLCFGR_DIVR1EN;       // DIVR1 ENABLE
    
    RCC->PLL1DIVR &= ~(RCC_PLL1DIVR_N1_Msk);
    RCC->PLL1DIVR |= ((rcc_clocks.pll.DIVN-0x01) <<RCC_PLL1DIVR_N1_Pos);     //DIVN1 set 
    RCC->PLL1DIVR &= ~(RCC_PLL1DIVR_P1_Msk);
    RCC->PLL1DIVR |= ((rcc_clocks.pll.DIVP-0x01) <<RCC_PLL1DIVR_P1_Pos);     //DIVP1 set 
    RCC->PLL1DIVR &= ~(RCC_PLL1DIVR_Q1_Msk);
    RCC->PLL1DIVR |= ((rcc_clocks.pll.DIVQ-0x01) <<RCC_PLL1DIVR_Q1_Pos);     //DIVQ1 set 
    RCC->PLL1DIVR &= ~(RCC_PLL1DIVR_R1_Msk);
    RCC->PLL1DIVR |= ((rcc_clocks.pll.DIVR-0x01) <<RCC_PLL1DIVR_R1_Pos);     //DIVR1 set 
    
    RCC->CR |= (RCC_CR_PLL1ON);                           //PLL1 ON
    while ( !(RCC->CR & RCC_CR_PLL1RDY) ){}               //Wait until the PLL1 ON  

    
    /*BUS prescaler Configuration */

    HPRE_config();         //HPRE
    D2PPRE1_config();      //D2PPRE1
    D2PPRE2_config();      //D2PPRE2
    
    

    /*FLASH latency set */
    FLASH->ACR &= ~(FLASH_ACR_LATENCY);                  //flash latency clear
    FLASH->ACR |=  (FLASH_ACR_LATENCY_2WS);              //flash latency four wait states (для 140-185 мгц)
    FLASH->ACR |=  (0x01 <<FLASH_ACR_WRHIGHFREQ_Pos);    //WRHIGHFREQ is 1 (для 140-185 мгц)
    
    
    RCC->CFGR |= RCC_CFGR_SW_PLL1;                         // PLL1 select as system clock
  //  while ( !(RCC->CFGR & RCC_CFGR_SWS_PLL1) ){}          //Wait until the clocking switches to PLL1
    
#endif
    rcc_clocks_get ( &rcc_clocks );
    
    return SUCCESS;
}

void rcc_clocks_get (rcc_clocks_t* rcc)
{
    uint32_t  pll_vco = 0, pll_p_1 = 2, pll_n_1 = 0, pll_m_1 = 2,pll_q_1 = 2,pll_r_1 = 2;
    
    /* Get SYSCLK source -------------------------------------------------------*/
    switch (RCC->CFGR & RCC_CFGR_SWS)
    {
        case RCC_CFGR_SWS_HSI:  /* HSI used as system clock source */
        rcc->bus_freq.sysclk = HSI_VALUE;
        break;
        case RCC_CFGR_SWS_HSE:  /* HSE used as system clock  source */
        rcc->bus_freq.sysclk = HSE_VALUE;
        break;
        case RCC_CFGR_SWS_PLL1:  /* PLL1 P used as system clock  source */
        
        pll_m_1 = rcc->pll.DIVM;
        pll_n_1 = rcc->pll.DIVN;
        pll_p_1 = rcc->pll.DIVP;
        pll_q_1 = rcc->pll.DIVQ;
        pll_r_1 = rcc->pll.DIVR;
        
        if ( (RCC->PLLCKSELR & RCC_PLLCKSELR_PLLSRC) == RCC_PLLCKSELR_PLLSRC_HSE ){
            /* HSE used as PLL1 clock source */
            pll_vco = (HSE_VALUE / pll_m_1) * pll_n_1;
        }
        else if ( (RCC->PLLCKSELR & RCC_PLLCKSELR_PLLSRC) == RCC_PLLCKSELR_PLLSRC_HSI ){
            /* HSI used as PLL1 clock source */
            pll_vco = (HSI_VALUE / pll_m_1) * pll_n_1;      
        }
//        else if ( (RCC->PLLCKSELR & RCC_PLLCKSELR_PLLSRC) == RCC_PLLCKSELR_PLLSRC_CSI ){
//            /* HSI used as PLL1 clock source */
//            pll_vco = (CSI_VALUE / pll_m_1) * pll_n_1;  
//            
//        }
        rcc->bus_freq.sysclk = rcc->pll.pll_clk_r = pll_vco/pll_r_1;
        rcc->pll.pll_clk_q = pll_vco/pll_q_1;
        rcc->pll.pll_clk_p = pll_vco/pll_p_1;
        break;
        
        default:
        rcc->bus_freq.sysclk = HSI_VALUE;
        break;
    }
    /* Compute PLL_CLK_P/Q/R clocks frequencies ------------------------*/
    
    
    /* Compute HCLK, PCLK1 and PCLK2 clocks frequencies ------------------------*/
    rcc->bus_freq.cpu = rcc->bus_freq.sysclk;
    rcc->bus_freq.hclk = rcc->bus_freq.sysclk / rcc->prescaler.HPRE;
    rcc->bus_freq.apb_1 = rcc->bus_freq.hclk /  rcc->prescaler.D2PPRE1;  
    rcc->bus_freq.apb_2 = rcc->bus_freq.hclk /  rcc->prescaler.D2PPRE2;
}


void HPRE_config(void)
{
        RCC->D1CFGR &= ~( RCC_D1CFGR_HPRE_Msk);             //1 , default
    switch(rcc_clocks.prescaler.HPRE)
    {
        case 1:
        break;
        case 2:
        RCC->D1CFGR |= (0b1000<< RCC_D1CFGR_HPRE_Pos);        //2
        break;
        case 4:
        RCC->D1CFGR |= (0b1001<< RCC_D1CFGR_HPRE_Pos);        //4
        break;
        case 8:
        RCC->D1CFGR |= (0b1010<< RCC_D1CFGR_HPRE_Pos);         //8
        break;
        case 16:
        RCC->D1CFGR |= (0b1011<< RCC_D1CFGR_HPRE_Pos);        //16
        break;
        case 64:
        RCC->D1CFGR |= (0b1100<< RCC_D1CFGR_HPRE_Pos);        //64
        break;
        case 128:
        RCC->D1CFGR |= (0b1101<< RCC_D1CFGR_HPRE_Pos);        //128
        break;
        case 256:
        RCC->D1CFGR |= (0b1110<< RCC_D1CFGR_HPRE_Pos);        //256
        break;
        case 512:
        RCC->D1CFGR |= (0b1111<< RCC_D1CFGR_HPRE_Pos);        //512
        break;
        default:
        RCC->D1CFGR &= ~( RCC_D1CFGR_HPRE_Msk);             //1
        break;
    }
}


void D2PPRE1_config(void)
{
    RCC->D2CFGR &= ~( RCC_D2CFGR_D2PPRE1_Msk);              //1 , default
    switch(rcc_clocks.prescaler.D2PPRE1)
    {
        case 1:
        break;
        case 2:
        RCC->D2CFGR |= (0b100<< RCC_D2CFGR_D2PPRE1_Pos);        //2
        break;
        case 4:
        RCC->D2CFGR |= (0b101<< RCC_D2CFGR_D2PPRE1_Pos);        //4
        break;
        case 8:
        RCC->D2CFGR |= (0b110<< RCC_D2CFGR_D2PPRE1_Pos);        //8
        break;
        case 16:
        RCC->D2CFGR |= (0b111<< RCC_D2CFGR_D2PPRE1_Pos);       //16
        break;
        default:
        RCC->D2CFGR &= ~( RCC_D2CFGR_D2PPRE1_Msk);             //1
        break;
    }
}

void D2PPRE2_config(void)
{
    RCC->D2CFGR &= ~( RCC_D2CFGR_D2PPRE2_Msk);              //1 , default
    switch(rcc_clocks.prescaler.D2PPRE2)
    {
        case 1:
        break;
        case 2:
        RCC->D2CFGR |= (0b100<< RCC_D2CFGR_D2PPRE2_Pos);        //2
        break;
        case 4:
        RCC->D2CFGR |= (0b101<< RCC_D2CFGR_D2PPRE2_Pos);        //4
        break;
        case 8:
        RCC->D2CFGR |= (0b110<< RCC_D2CFGR_D2PPRE2_Pos);        //8
        break;
        case 16:
        RCC->D2CFGR |= (0b111<< RCC_D2CFGR_D2PPRE2_Pos);       //16
        break;
        default:
        RCC->D2CFGR &= ~( RCC_D2CFGR_D2PPRE2_Msk);             //1
        break;
    }
}
