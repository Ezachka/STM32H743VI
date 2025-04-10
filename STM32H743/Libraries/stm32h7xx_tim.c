#include "stm32h7xx_tim.h"
//2
void tim2_init ( void )
{
    RCC->APB1LENR |= RCC_APB1LENR_TIM2EN;
        
    TIM2->PSC = TIM2_PRESCALER - 1;
    TIM2->ARR = 0;
    
    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM2->CR1 &= ~TIM_CR1_CKD;     

    TIM2->DIER &= ~TIM_DIER_UIE; 
    
    NVIC_SetPriority( TIM2_IRQn, 10 );
    NVIC_EnableIRQ( TIM2_IRQn );   
    
    tim2_enable(1);
}


void tim2_disable( void )
{
    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM2->DIER &= ~TIM_DIER_UIE;
    TIM2->CNT = 0;
    TIM2->ARR = 0;
}


void tim2_enable( uint16_t value )
{
    TIM2->ARR = value;
    TIM2->CNT = 0;
    TIM2->DIER |=  TIM_DIER_UIE;   
    TIM2->CR1 |= TIM_CR1_CEN;   
}
//14
void tim14_init ( void )
{
    RCC->APB1LENR |= RCC_APB1LENR_TIM14EN;
        
    TIM14->PSC = TIM14_PRESCALER - 1;
    TIM14->ARR = 0;
    
    TIM14->CR1 &= ~TIM_CR1_CEN;
    TIM14->CR1 &= ~TIM_CR1_CKD;     

    TIM14->DIER &= ~TIM_DIER_UIE; 
    
    NVIC_SetPriority( TIM8_TRG_COM_TIM14_IRQn, 0 );
    NVIC_EnableIRQ( TIM8_TRG_COM_TIM14_IRQn );   
    
    tim14_enable(10);
}


void tim14_disable( void )
{
    TIM14->CR1 &= ~TIM_CR1_CEN;
    TIM14->DIER &= ~TIM_DIER_UIE;
    TIM14->CNT = 0;
    TIM14->ARR = 0;
}


void tim14_enable( uint16_t value )
{
    TIM14->ARR = value;
    TIM14->CNT = 0;
    TIM14->DIER |=  TIM_DIER_UIE;   
    TIM14->CR1 |= TIM_CR1_CEN;   
}

