#include "buttons.h"
#include "buzzer.h"
#include "gpio_config.h"
#include "stm32l4xx_interrupts.h"
#include "stm32l4xx_tim.h"  

static const uint32_t short_freq_time    = 100;
static const uint32_t long_freq_time     = 400;
static const uint16_t up_button_freq      = 261;  // C4 - До
static const uint16_t down_button_freq    = 293;  // D4 - Ре
static const uint16_t left_button_freq    = 329;  // E4 - Ми
static const uint16_t right_button_freq   = 392;  // G4 - Соль
static const uint16_t select_button_freq  = 440;  // A4 - Ля


static const uint32_t short_push_time_config=30 ; // время от которого считается коротконе нажатие, антиджребезг
static const uint32_t long_push_time_config =300 ; // время от которого считается длинное нажатие, 


void button_update( bool btn_pressed, uint32_t *start_time, bool *start_flag, bool *short_ban_flag, bool *short_push,
                    bool *long_push, uint32_t current_time, uint32_t short_push_time, uint32_t long_push_time); 

void buttons_clear(void);/// защита в случае зануления структуры


 buttons_t buttons;



void buttons_init(void)
{
    buttons.config.short_push_time  =   short_push_time_config;
    buttons.config.long_push_time   =   long_push_time_config;
    
    
    buttons.params.up.start_time=0;
    buttons.params.up.start_flag=true;
    buttons.params.up.short_ban_flag=true;
    buttons.params.up.buzzer_freq_short         =   up_button_freq;
    buttons.params.up.buzzer_freq_long          =   up_button_freq;   
    buttons.params.up.buzzer_freq_short_time    =   short_freq_time; 
    buttons.params.up.buzzer_freq_long_time     =   long_freq_time; 
    
    buttons.params.down.start_time=0;
    buttons.params.down.start_flag=true;
    buttons.params.down.short_ban_flag=true;
    buttons.params.down.buzzer_freq_short         =   down_button_freq;
    buttons.params.down.buzzer_freq_long          =   down_button_freq;   
    buttons.params.down.buzzer_freq_short_time    =   short_freq_time; 
    buttons.params.down.buzzer_freq_long_time     =   long_freq_time;
        
    buttons.params.left.start_time=0;
    buttons.params.left.start_flag=true;
    buttons.params.left.short_ban_flag=true;
    buttons.params.left.buzzer_freq_short         =   left_button_freq;
    buttons.params.left.buzzer_freq_long          =   left_button_freq;   
    buttons.params.left.buzzer_freq_short_time    =   short_freq_time; 
    buttons.params.left.buzzer_freq_long_time     =   long_freq_time;
        
    buttons.params.right.start_time=0;
    buttons.params.right.start_flag=true;
    buttons.params.right.short_ban_flag=true;
    buttons.params.right.buzzer_freq_short         =   right_button_freq;
    buttons.params.right.buzzer_freq_long          =   right_button_freq;   
    buttons.params.right.buzzer_freq_short_time    =   short_freq_time; 
    buttons.params.right.buzzer_freq_long_time     =   long_freq_time;
        
    buttons.params.select.start_time=0;
    buttons.params.select.start_flag=true;
    buttons.params.select.short_ban_flag=true;
    buttons.params.select.buzzer_freq_short         =   select_button_freq;
    buttons.params.select.buzzer_freq_long          =   select_button_freq;   
    buttons.params.select.buzzer_freq_short_time    =   short_freq_time; 
    buttons.params.select.buzzer_freq_long_time     =   long_freq_time;
    

    
}

void buttons_clear(void)
{
        buttons.config.short_push_time  =   short_push_time_config;
    buttons.config.long_push_time   =   long_push_time_config;
    
    
//    buttons.params.up.start_time=0;
//    buttons.params.up.start_flag=true;
//    buttons.params.up.short_ban_flag=true;
    buttons.params.up.buzzer_freq_short         =   up_button_freq;
    buttons.params.up.buzzer_freq_long          =   up_button_freq;   
    buttons.params.up.buzzer_freq_short_time    =   short_freq_time; 
    buttons.params.up.buzzer_freq_long_time     =   long_freq_time; 
    
//    buttons.params.down.start_time=0;
//    buttons.params.down.start_flag=true;
//    buttons.params.down.short_ban_flag=true;
    buttons.params.down.buzzer_freq_short         =   down_button_freq;
    buttons.params.down.buzzer_freq_long          =   down_button_freq;   
    buttons.params.down.buzzer_freq_short_time    =   short_freq_time; 
    buttons.params.down.buzzer_freq_long_time     =   long_freq_time;
//        
//    buttons.params.left.start_time=0;
//    buttons.params.left.start_flag=true;
//    buttons.params.left.short_ban_flag=true;
    buttons.params.left.buzzer_freq_short         =   left_button_freq;
    buttons.params.left.buzzer_freq_long          =   left_button_freq;   
    buttons.params.left.buzzer_freq_short_time    =   short_freq_time; 
    buttons.params.left.buzzer_freq_long_time     =   long_freq_time;
        
//    buttons.params.right.start_time=0;
//    buttons.params.right.start_flag=true;
//    buttons.params.right.short_ban_flag=true;
    buttons.params.right.buzzer_freq_short         =   right_button_freq;
    buttons.params.right.buzzer_freq_long          =   right_button_freq;   
    buttons.params.right.buzzer_freq_short_time    =   short_freq_time; 
    buttons.params.right.buzzer_freq_long_time     =   long_freq_time;
        
//    buttons.params.select.start_time=0;
//    buttons.params.select.start_flag=true;
//    buttons.params.select.short_ban_flag=true;
    buttons.params.select.buzzer_freq_short         =   select_button_freq;
    buttons.params.select.buzzer_freq_long          =   select_button_freq;   
    buttons.params.select.buzzer_freq_short_time    =   short_freq_time; 
    buttons.params.select.buzzer_freq_long_time     =   long_freq_time;
    
}


void buttons_update(void)
{   
    
    ///
    buttons.params.up.btn &= ~(1 << 0);
    buttons.params.up.btn |= (BUTTON_UP_PORT->IDR & BUTTON_UP_PIN) ? 0 : (1 << 0);
    
    buttons.params.down.btn &= ~(1 << 0);
    buttons.params.down.btn |= (BUTTON_DOWN_PORT->IDR & BUTTON_DOWN_PIN) ? 0 : (1 << 0);
    
    buttons.params.left.btn &= ~(1 << 0);
    buttons.params.left.btn |= (BUTTON_LEFT_PORT->IDR & BUTTON_LEFT_PIN) ? 0 : (1 << 0);
    
    buttons.params.right.btn &= ~(1 << 0);
    buttons.params.right.btn |= (BUTTON_RIGHT_PORT->IDR & BUTTON_RIGHT_PIN) ? 0 : (1 << 0);
    
    buttons.params.select.btn &= ~(1 << 0);
    buttons.params.select.btn |= (BUTTON_SELECT_PORT->IDR & BUTTON_SELECT_PIN) ? 0 : (1 << 0);    
    ///
    ///  сделаны долгие и короткие нажатия
    if(  buttons.config.short_push_time <10 || buttons.config.long_push_time <100) ///  защита, в случае если егде-то нулится, такое было, нашел, исправил
    {
     buttons_clear();
    }
    button_update( buttons.params.up.btn, &buttons.params.up.start_time, &buttons.params.up.start_flag,
                  &buttons.params.up.short_ban_flag, &buttons.up.short_push, &buttons.up.long_push,
                  systick_get(), buttons.config.short_push_time, buttons.config.long_push_time );
    
    button_update( buttons.params.down.btn, &buttons.params.down.start_time, &buttons.params.down.start_flag,
                  &buttons.params.down.short_ban_flag, &buttons.down.short_push, &buttons.down.long_push,
                  systick_get(), buttons.config.short_push_time, buttons.config.long_push_time );
    
    button_update( buttons.params.left.btn, &buttons.params.left.start_time, &buttons.params.left.start_flag,
                  &buttons.params.left.short_ban_flag, &buttons.left.short_push, &buttons.left.long_push,
                  systick_get(), buttons.config.short_push_time, buttons.config.long_push_time );
    
    button_update( buttons.params.right.btn, &buttons.params.right.start_time, &buttons.params.right.start_flag,
                    &buttons.params.right.short_ban_flag, &buttons.right.short_push, &buttons.right.long_push,
                    systick_get(), buttons.config.short_push_time, buttons.config.long_push_time );
    
    button_update( buttons.params.select.btn, &buttons.params.select.start_time, &buttons.params.select.start_flag,
                    &buttons.params.select.short_ban_flag, &buttons.select.short_push, &buttons.select.long_push,
                    systick_get(), buttons.config.short_push_time, buttons.config.long_push_time );   
    
}

void button_update( bool btn_pressed, uint32_t *start_time, bool *start_flag, bool *short_ban_flag, bool *short_push,
                    bool *long_push, uint32_t current_time, uint32_t short_push_time, uint32_t long_push_time) 
{
    if (*start_time > current_time) {
        *start_time = 0;
    }

    // Если кнопка нажата
    if (btn_pressed) 
    {
        if (*start_flag) 
        {
            *start_time = current_time;
            *start_flag = false;
        }
        if ((current_time - *start_time) > long_push_time) 
        {
            *long_push = true;
            *short_push = false;
            *short_ban_flag = true;
            *start_time = current_time;
        }
    } 
    else 
    {
        if ((current_time - *start_time) < long_push_time && (current_time - *start_time) > short_push_time && !*start_flag && !*long_push && !*short_ban_flag) 
        {
            *short_push = true;
        }
        *start_flag = true;
        *short_ban_flag = false;
    }
    
    
        
//
//  if (buttons.params.left.start_time>systick_get())
//   {buttons.params.left.start_time=0;}
//  //обработка нажатия левой кнопки
//  if (buttons.params.left.LEFT_btn)
//  {
//    if(buttons.params.left.start_flag)
//    {
//      buttons.params.left.start_time=systick_get();
//      buttons.params.left.start_flag=0;
//    }
//    if((systick_get()-buttons.params.left.start_time)>buttons.config.long_push_time)
//    {
//      buttons.left.long_push=1;
//      buttons.left.short_push=0;
//      buttons.params.left.short_ban_flag=1;
//      buttons.params.left.start_time=systick_get();
//    }
//  }
//  else
//  {
//    if((systick_get()-buttons.params.left.start_time)<buttons.config.long_push_time && (systick_get()-buttons.params.left.start_time)>buttons.config.short_push_time  && !buttons.params.left.start_flag && !buttons.Left.btn_long && !buttons.params.left.short_ban_flag)
//    {
//      buttons.left.short_push=1;
//    }
//    buttons.params.left.start_flag=1;
//    buttons.params.left.short_ban_flag=0;
//   }
  
}