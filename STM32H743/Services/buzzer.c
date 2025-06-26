#include "buzzer.h"




buzzer_t out1_buzzer;
buzzer_t out2_PWM;

void buzzer_init(void)
{
   // tim1_init ();
    tim2_init ();
//    out1_buzzer.frequency =400.0f;
//    out1_buzzer.start_flag=false;
//    out1_buzzer.start_time =0x00;
//    out1_buzzer.time=0x00;
//    out1_buzzer.PWM=1.0f;
    
    out2_PWM.frequency =400.0f;
    out2_PWM.start_flag=false;
    out2_PWM.start_time =0x00;
    out2_PWM.time=0x00;
    out2_PWM.PWM=50.0f;
}


void buzzer_update()
{
    //    TIM1->ARR  = (uint32_t)((float)(SystemCoreClock/PWM_PRESCALER)/out1_buzzer.frequency   - 1.0f); // устанавливаем частоту
    //    TIM2->ARR  = (uint32_t)((float)(SystemCoreClock/PWM_PRESCALER)/out2_PWM.frequency   - 1.0f); // устанавливаем частоту
    uint32_t current_time = systick_get(); 
    
//    if (out1_buzzer.start_flag)
//    {
//        if (out1_buzzer.start_time == 0)
//        {
//            out1_buzzer.start_time = current_time;
//        }
//        
//        if ((current_time - out1_buzzer.start_time) < (out1_buzzer.time*(1000/1000)))  
//        {
//            tim_pwm_set(TIM1,0x03,out1_buzzer.PWM,out1_buzzer.frequency); 
//            out1_buzzer.active=true;
//        }
//        else
//        {
//            tim_pwm_set(TIM1,0x03,0.0f,out1_buzzer.frequency);
//            out1_buzzer.start_flag = false;
//            out1_buzzer.start_time = 0;
//            out1_buzzer.time = 0;
//            out1_buzzer.active=false;
//        }
//    }
    
    
    if (out2_PWM.start_flag)
    {
        if (out2_PWM.start_time == 0)
        {
            out2_PWM.start_time = current_time;
        }
        
        if ((current_time - out2_PWM.start_time) < (out2_PWM.time*(1000/1000)))  
        {
            tim_pwm_set(TIM2,0x01,out2_PWM.PWM,out2_PWM.frequency); 
            out2_PWM.active=true;
        }
        else
        {
            tim_pwm_set(TIM2,0x01,0.0f,out2_PWM.frequency);
            out2_PWM.start_flag = false;
            out2_PWM.start_time = 0;
            out2_PWM.time = 0;
            out2_PWM.active=false;
        }
    }
}
void  out1_buzzer_PWM_start(float frequency, uint32_t time_ms,float PWM)
{
    out1_buzzer.start_flag = true;
    out1_buzzer.frequency=frequency;
    out1_buzzer.time=time_ms;
    if( PWM != false)
    {
        out1_buzzer.PWM=PWM;
    }
}



void  out2_PWM_start(float frequency, uint32_t time_ms,float PWM)
{
    out2_PWM.start_flag = true;
    out2_PWM.frequency=frequency;
    out2_PWM.time=time_ms;
    if( PWM != false)
    {
        out2_PWM.PWM=PWM;
    }
}


void  out1_buzzer_PWM_emergency_stop(void)
{
    tim_pwm_set(TIM1,0x03,0.0f,out1_buzzer.frequency);
    out1_buzzer.active=false;
    out1_buzzer.start_flag = false;
    out1_buzzer.start_time = 0;
    out1_buzzer.time = 0;
}
void  out2_PWM_emergency_stop(void)
{
    tim_pwm_set(TIM2,0x01,0.0f,out2_PWM.frequency);
    out2_PWM.active=false;
    out2_PWM.start_flag = false;
    out2_PWM.start_time = 0;
    out2_PWM.time = 0;
}



bool  out1_buzzer_PWM_status_get(void)
{
    return out1_buzzer.active;
}
bool  out2_PWM_status_get(void)
{
    return out2_PWM.active;
}
