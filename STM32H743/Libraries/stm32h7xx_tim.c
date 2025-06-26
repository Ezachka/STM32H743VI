#include "stm32h7xx_tim.h"


void tim1_init ( void )
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    
    
    /* TIM1  CH3 Init */
    //PWM1 - PB1 (TIM1_CH3),
	TIM1->PSC  = PWM_PRESCALER - 1;  // 10MHz(80MHz/PSC)
	TIM1->ARR  = PWM_TIMER_PERIOD - 1;
	TIM1->CCR1 = 0;
	TIM1->CCR2 = 0;
	TIM1->CCR3 = 0;
    TIM1->CCR4 = 0;
    
	TIM1->CR1  &= ~(TIM_CR1_DIR);      //Up counter
	TIM1->CR1  &= ~(TIM_CR1_CMS);      //Center-aligned mode selection
	TIM1->BDTR |=  (TIM_BDTR_MOE);     //Main output enable
	TIM1->CCER |=  (TIM_CCER_CC3NE);     // Capture/compare output enable ch3_N
    TIM1->CCER &= ~(TIM_CCER_CC3NP);     //Active polarity CH3_N is low
    
	TIM1->CCMR2 |= (TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2);   //PWM  mode 1
	TIM1->CCMR2 |= (TIM_CCMR2_OC3PE);                       //Output3 Compare 1 Preload enable
    
    TIM1->CR1  |= TIM_CR1_CEN;  //Timer enable	 
}

void tim_pwm_set (TIM_TypeDef* TIMx,uint8_t channel, float pwm_duty_cycle, float pwm_frequency)
{
      TIMx->ARR  = (uint32_t)((float)(SystemCoreClock/PWM_PRESCALER)/pwm_frequency   - 1.0f); 
      
      float pwm_period   =  (BUS_FREQUANCY/PWM_PRESCALER)/pwm_frequency  ;
      
      if( pwm_duty_cycle > 100.0f ) {pwm_duty_cycle = 100.0f;}
    
    uint16_t pwm = (uint16_t)( (float)pwm_duty_cycle * (float)(pwm_period) / 100.0f ); 
    
    if( pwm > pwm_period ){ pwm = (uint16_t)pwm_period;}
    
    switch (channel)
    {
        case 1: TIMx->CCR1 = pwm; break; 
        case 2: TIMx->CCR2 = pwm; break; 
        case 3: TIMx->CCR3 = pwm; break; 
        case 4: TIMx->CCR4 = pwm; break;   
    }  
}



//2
void tim2_init ( void )
{
   // RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
        RCC->APB1LENR |= RCC_APB1LENR_TIM2EN;
    
    /* TIM2  CH1 Init */
    //PWM2 - PA15 (TIM2_CH1),
	TIM2->PSC  = PWM_PRESCALER - 1;  // 10MHz(80MHz/PSC)
	TIM2->ARR  = PWM_TIMER_PERIOD - 1;
	TIM2->CCR1 = 0;
	TIM2->CCR2 = 0;
	TIM2->CCR3 = 0;
    TIM2->CCR4 = 0;
    
	TIM2->CR1  &= ~(TIM_CR1_DIR);      //Up counter
	TIM2->CR1  &= ~(TIM_CR1_CMS);      //Center-aligned mode selection
	TIM2->BDTR |=  (TIM_BDTR_MOE);     //Main output enable
	TIM2->CCER |=  (TIM_CCER_CC1E);     // Capture/compare output enable ch3_N
    TIM2->CCER &= ~(TIM_CCER_CC1P);     //Active polarity CH3_N is low
    
	TIM2->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);   //PWM  mode 1
	TIM2->CCMR1 |= (TIM_CCMR1_OC1PE);                       //Output3 Compare 1 Preload enable
    
    TIM2->CR1  |= TIM_CR1_CEN;  //Timer enable	 
}
//3
void tim7_init ( void )
{
  //  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM7EN;
        RCC->APB1LENR |= RCC_APB1LENR_TIM7EN;
    TIM7->PSC = TIM7_PRESCALER - 1;
    TIM7->ARR = 0;
    
    TIM7->CR1 &= ~TIM_CR1_CEN;
    TIM7->CR1 &= ~TIM_CR1_CKD;     
    
    TIM7->DIER &= ~TIM_DIER_UIE; 
    
    NVIC_SetPriority( TIM7_IRQn, 0 );
    NVIC_EnableIRQ( TIM7_IRQn );   
    
    tim7_enable(1);
}

void tim7_disable( void )
{
    TIM7->CR1 &= ~TIM_CR1_CEN;
    TIM7->DIER &= ~TIM_DIER_UIE;
    TIM7->CNT = 0;
    TIM7->ARR = 0;
}


void tim7_enable( uint16_t value )
{
    TIM7->ARR = value;
    TIM7->CNT = 0;
    TIM7->DIER |=  TIM_DIER_UIE;   
    TIM7->CR1 |= TIM_CR1_CEN;   
}



