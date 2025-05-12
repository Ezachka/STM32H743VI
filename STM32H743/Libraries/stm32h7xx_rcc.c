
#include "stm32h7xx_rcc.h"
#include "system_stm32h7xx.h"
#include "stm32h7xx_interrupts.h"


//#define core_m7_status (*(bool *)0x30000050) //sram1
//#define core_m4_status (*(bool *)0x30000051) //sram1


void D1CPRE_config(void);
void HPRE_config(void);
void D1PPRE_config(void);
void D2PPRE1_config(void);
void D2PPRE2_config(void);
void D3PPRE_config(void);

rcc_clocks_t rcc_clocks = {
    
    .pll_1_source_select = hse, 
    .per_source_select = hse,
    
    .source_val.hse = HSE_VALUE,
    .source_val.hsi = HSI_VALUE,
    .source_val.csi = CSI_VALUE,
    .source_val.lse = 32768,
    .source_val.lsi = 32000,
    
    .pll_1.DIVM = 20, 
    .pll_1.DIVN = 384,   
    .pll_1.DIVP = 2, 
    .pll_1.DIVQ = 2,
    .pll_1.DIVR = 2,
    
    .prescaler.D1CPRE  = 1, 
    .prescaler.HPRE    = 2, 
    .prescaler.D1PPRE  = 2, 
    .prescaler.D2PPRE1 = 2,
    .prescaler.D2PPRE2 = 2, 
    .prescaler.D3PPRE  = 2.
};

ErrorStatus rcc_clock_init ( void )
{
    
#ifdef CORE_CM7
//    
//    core_m7_status=false;
//    core_m4_status=false;   
    
    RCC->CR &=  ~RCC_CR_HSION;                            //HSI OFF
    RCC->CR |=   RCC_CR_HSEON;                            //HSE ON
    while ( !(RCC->CR & RCC_CR_HSIRDY) ){}              //Wait until the HSI oscillator is stable
    
    /* PLL1 Configuration */
    
    if(rcc_clocks.pll_1_source_select==hse)
    {
        RCC->PLLCKSELR |= RCC_PLLCKSELR_PLLSRC_HSE;         //PLL1 CLOCK SORCE =  HSE 
    }
    else if (rcc_clocks.pll_1_source_select==hsi)
    {
        RCC->PLLCKSELR |= RCC_PLLCKSELR_PLLSRC_HSI;         //PLL1 CLOCK SORCE =  HSI 
    }
    else if (rcc_clocks.pll_1_source_select==csi)
    {
        RCC->PLLCKSELR |= RCC_PLLCKSELR_PLLSRC_CSI;         //PLL1 CLOCK SORCE =  HSI 
    }
    
    RCC->PLLCKSELR &= ~(RCC_PLLCKSELR_DIVM1_Msk);       //DIVM1 clear
    RCC->PLLCKSELR |=  (rcc_clocks.pll_1.DIVM << RCC_PLLCKSELR_DIVM1_Pos);   //DIVM1 set 
    
    /* PLL1 Configuration */
    RCC->CR &= ~(RCC_CR_PLL1ON);                         //PLL1 OFF
    while ( (RCC->CR & RCC_CR_PLL1RDY) ){}               //Wait until the PLL1 OFF 
    
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLL1VCOSEL);              // Freq measurment as 192 till 960 Mgh
    RCC->PLLCFGR |=  (0x01 << RCC_PLLCFGR_PLL1RGE_Pos);     // 64/20=3.2 MHZ in 
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLL1FRACEN_Msk);          //PLL1FRACEN CLEAR  Дробное значение для DIVN1, например DIVN1= 50, и если FRACEN 3, То будет DIVN1 = 50+3/50, можно менять на горячую!
    
    RCC->PLLCFGR |=  RCC_PLLCFGR_DIVP1EN;       // DIVP1 ENABLE
    RCC->PLLCFGR |=  RCC_PLLCFGR_DIVQ1EN;       // DIVQ1 ENABLE
    RCC->PLLCFGR &= ~RCC_PLLCFGR_DIVR1EN;       // DIVR1 DISABLE
    
    RCC->PLL1DIVR &= ~(RCC_PLL1DIVR_N1_Msk);
    RCC->PLL1DIVR |= ((rcc_clocks.pll_1.DIVN-0x01) <<RCC_PLL1DIVR_N1_Pos);     //DIVN1 set 
    RCC->PLL1DIVR &= ~(RCC_PLL1DIVR_P1_Msk);
    RCC->PLL1DIVR |= ((rcc_clocks.pll_1.DIVP-0x01) <<RCC_PLL1DIVR_P1_Pos);     //DIVP1 set 
    RCC->PLL1DIVR &= ~(RCC_PLL1DIVR_Q1_Msk);
    RCC->PLL1DIVR |= ((rcc_clocks.pll_1.DIVQ-0x01) <<RCC_PLL1DIVR_Q1_Pos);     //DIVQ1 set 
    RCC->PLL1DIVR &= ~(RCC_PLL1DIVR_R1_Msk);
    RCC->PLL1DIVR |= ((rcc_clocks.pll_1.DIVR-0x01) <<RCC_PLL1DIVR_R1_Pos);     //DIVR1 set 
    
    RCC->CR |= (RCC_CR_PLL1ON);                           //PLL1 ON
    while ( !(RCC->CR & RCC_CR_PLL1RDY) ){}               //Wait until the PLL1 ON  
    
    /* PLL2 Configuration */
    
    /*BUS prescaler Configuration */
    D1CPRE_config();       //D1CPRE
    HPRE_config();         //HPRE
    D1PPRE_config();       //D1PPRE
    D2PPRE1_config();      //D2PPRE1
    D2PPRE2_config();      //D2PPRE2
    D3PPRE_config();       //D3PPRE
    
    
    /* PER source Configuration */
    if(rcc_clocks.per_source_select==hse)
    {
        RCC->D1CCIPR  |=(0x02 <<RCC_D1CCIPR_CKPERSEL_Pos);          //PER CLOCK SORCE =  HSE 
    }
    else if (rcc_clocks.per_source_select==hsi)
    {
        RCC->D1CCIPR  |=(0x00 <<RCC_D1CCIPR_CKPERSEL_Pos);          //PER CLOCK SORCE =  HSI 
    }
    else if (rcc_clocks.per_source_select==csi)
    {
        RCC->D1CCIPR  |=(0x01 <<RCC_D1CCIPR_CKPERSEL_Pos);          //PER CLOCK SORCE =  HSI 
    }
    
    /*FLASH latency set */
    FLASH->ACR &= ~(FLASH_ACR_LATENCY);                  //flash latency clear
    FLASH->ACR |=  (FLASH_ACR_LATENCY_4WS);              //flash latency four wait states (для 240 мгц)
    FLASH->ACR |=  (0x02 <<FLASH_ACR_WRHIGHFREQ_Pos);    //WRHIGHFREQ is 2 (для 240 мгц)
    
        /*USB source set */
    RCC->CR |= RCC_CR_HSI48ON;
    while ( !(RCC->CR & RCC_CR_HSI48ON) ){}               //Wait until the HSI48 ON  
    RCC->D2CCIP2R   |= 0x03 <<(RCC_D2CCIP2R_USBSEL_Pos);                             //HSI48 clock  tu USB selected
    RCC->AHB1ENR   |=  RCC_AHB1ENR_USB2OTGFSEN;                              //USB DS clock enable clock selected

    
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
        
        pll_m_1 = rcc->pll_1.DIVM;//(( RCC->PLLCKSELR & RCC_PLLCKSELR_DIVM1) >> 4) + 1;
        pll_n_1 = rcc->pll_1.DIVN;//(( RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 8);
        pll_p_1 = rcc->pll_1.DIVP;//(((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> 25) + 1 ) * 2;
        pll_q_1 = rcc->pll_1.DIVQ;
        pll_r_1 = rcc->pll_1.DIVR;
        
        if ( (RCC->PLLCKSELR & RCC_PLLCKSELR_PLLSRC) == RCC_PLLCKSELR_PLLSRC_HSE ){
            /* HSE used as PLL1 clock source */
            pll_vco = (HSE_VALUE / pll_m_1) * pll_n_1;
        }
        else if ( (RCC->PLLCKSELR & RCC_PLLCKSELR_PLLSRC) == RCC_PLLCKSELR_PLLSRC_HSI ){
            /* HSI used as PLL1 clock source */
            pll_vco = (HSI_VALUE / pll_m_1) * pll_n_1;      
        }
        else if ( (RCC->PLLCKSELR & RCC_PLLCKSELR_PLLSRC) == RCC_PLLCKSELR_PLLSRC_CSI ){
            /* HSI used as PLL1 clock source */
            pll_vco = (CSI_VALUE / pll_m_1) * pll_n_1;  
            
        }
        rcc->bus_freq.sysclk = rcc->pll_1.pll_clk_p = pll_vco/pll_p_1;
        rcc->pll_1.pll_clk_q = pll_vco/pll_q_1;
        rcc->pll_1.pll_clk_r = pll_vco/pll_r_1;
        break;
        
        default:
        rcc->bus_freq.sysclk = HSI_VALUE;
        break;
    }
    /* Compute PLL_CLK_P/Q/R clocks frequencies ------------------------*/
    
    
    /* Compute HCLK, PCLK1 and PCLK2 clocks frequencies ------------------------*/
    rcc->bus_freq.cpu_1 = rcc->bus_freq.sysclk / rcc->prescaler.D1CPRE ;
    rcc->bus_freq.cpu_2 = rcc->bus_freq.axi = rcc->bus_freq.ahb_1 =rcc->bus_freq.ahb_2=rcc->bus_freq.ahb_4 = rcc->bus_freq.cpu_1 / rcc->prescaler.HPRE;
    rcc->bus_freq.apb_3 = rcc->bus_freq.cpu_2 /  rcc->prescaler.D1PPRE;
    rcc->bus_freq.apb_1 = rcc->bus_freq.cpu_2 /  rcc->prescaler.D2PPRE1;  
    rcc->bus_freq.apb_2 = rcc->bus_freq.cpu_2 /  rcc->prescaler.D2PPRE2;
    rcc->bus_freq.apb_4 = rcc->bus_freq.cpu_2 /  rcc->prescaler.D3PPRE;
}

void D1CPRE_config(void)
{
    RCC->D1CFGR &= ~( RCC_D1CFGR_D1CPRE_Msk);             //1 , default
    switch(rcc_clocks.prescaler.D1CPRE)
    {
        case 1:
        break;
        case 2:
        RCC->D1CFGR |= (0b1000<< RCC_D1CFGR_D1CPRE_Pos);        //2
        break;
        case 4:
        RCC->D1CFGR |= (0b1001<< RCC_D1CFGR_D1CPRE_Pos);        //4
        break;
        case 8:
        RCC->D1CFGR |= (0b1010<< RCC_D1CFGR_D1CPRE_Pos);         //8
        break;
        case 16:
        RCC->D1CFGR |= (0b1011<< RCC_D1CFGR_D1CPRE_Pos);        //16
        break;
        case 64:
        RCC->D1CFGR |= (0b1100<< RCC_D1CFGR_D1CPRE_Pos);        //64
        break;
        case 128:
        RCC->D1CFGR |= (0b1101<< RCC_D1CFGR_D1CPRE_Pos);        //128
        break;
        case 256:
        RCC->D1CFGR |= (0b1110<< RCC_D1CFGR_D1CPRE_Pos);        //256
        break;
        case 512:
        RCC->D1CFGR |= (0b1111<< RCC_D1CFGR_D1CPRE_Pos);        //512
        break;
        default:
        RCC->D1CFGR &= ~( RCC_D1CFGR_D1CPRE_Msk);             //1
        break;
    }
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

void D1PPRE_config(void)
{
    RCC->D1CFGR &= ~( RCC_D1CFGR_D1PPRE_Msk);              //1 , default
    switch(rcc_clocks.prescaler.D1PPRE)
    {
        case 1:
        break;
        case 2:
        RCC->D1CFGR |= (0b100<< RCC_D1CFGR_D1PPRE_Pos);        //2
        break;
        case 4:
        RCC->D1CFGR |= (0b101<< RCC_D1CFGR_D1PPRE_Pos);        //4
        break;
        case 8:
        RCC->D1CFGR |= (0b110<< RCC_D1CFGR_D1PPRE_Pos);        //8
        break;
        case 16:
        RCC->D1CFGR |= (0b111<< RCC_D1CFGR_D1PPRE_Pos);       //16
        break;
        default:
        RCC->D1CFGR &= ~( RCC_D1CFGR_D1PPRE_Msk);             //1
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

void D3PPRE_config(void)
{
    RCC->D3CFGR &= ~( RCC_D3CFGR_D3PPRE_Msk);              //1 , default
    switch(rcc_clocks.prescaler.D3PPRE)
    {
        case 1:
        break;
        case 2:
        RCC->D3CFGR |= (0b100<< RCC_D3CFGR_D3PPRE_Pos);        //2
        break;
        case 4:
        RCC->D3CFGR |= (0b101<< RCC_D3CFGR_D3PPRE_Pos);        //4
        break;
        case 8:
        RCC->D3CFGR |= (0b110<< RCC_D3CFGR_D3PPRE_Pos);        //8
        break;
        case 16:
        RCC->D3CFGR |= (0b111<< RCC_D3CFGR_D3PPRE_Pos);       //16
        break;
        default:
        RCC->D3CFGR &= ~( RCC_D3CFGR_D3PPRE_Msk);             //1
        break;
    }
}
void core_m7_init_status_set(bool status)
{
    //core_m7_status =  status;
}

bool core_m7_init_status_get(void)
{
  //  return  core_m7_status ;
}

void core_m4_init_status_set(bool status)
{
  //  core_m4_status =  status;
}

bool core_m4_init_status_get(void)
{
   // return  core_m4_status ;
}