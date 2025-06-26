#ifndef BUZZER_H
#define BUZZER_H

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "stm32h7xx.h"
#include "gpio_config.h"
#include "stm32h7xx_interrupts.h"
#include "stm32h7xx_tim.h"
    
// Частоты нот в первой октаве (в герцах)
#define NOTE_C1  32.70f  // До
#define NOTE_CS1 34.65f  // До-диез
#define NOTE_D1  36.71f  // Ре
#define NOTE_DS1 38.89f  // Ре-диез
#define NOTE_E1  41.20f  // Ми
#define NOTE_F1  43.65f  // Фа
#define NOTE_FS1 46.25f  // Фа-диез
#define NOTE_G1  49.00f  // Соль
#define NOTE_GS1 51.91f  // Соль-диез
#define NOTE_A1  55.00f  // Ля
#define NOTE_AS1 58.27f  // Ля-диез
#define NOTE_B1  61.74f  // Си

// Макрос для вычисления частоты ноты в другой октаве
    // Пример использования: NOTE_OCTAVE(NOTE_C1, 4) для До 4-й октавы
#define NOTE_OCTAVE(note, octave) ((note) * (1.0f << ((octave) - 1.0f)))
    
    // Примеры для 4-й октавы
#define NOTE_C4  NOTE_OCTAVE(NOTE_C1, 4.0f)  // До 4-й октавы
#define NOTE_D4  NOTE_OCTAVE(NOTE_D1, 4.0f)  // Ре 4-й октавы
#define NOTE_E4  NOTE_OCTAVE(NOTE_E1, 4.0f)  // Ми 4-й октавы
#define NOTE_G4  NOTE_OCTAVE(NOTE_G1, 4.0f)  //соль 4-й октавы
#define NOTE_A4  NOTE_OCTAVE(NOTE_A1, 4.0f)  // Ля 4-й октавы (440 Гц)
    
    
    typedef struct
    {
        bool start_flag;
        uint32_t time;
        uint32_t start_time;
        float frequency;
        float PWM;
        bool active;
    }buzzer_t;
    
    
    
    void buzzer_init(void);
    void buzzer_update();
    void  out1_buzzer_PWM_start(float frequency, uint32_t time_ms,float PWM);    
    void  out2_PWM_start(float frequency, uint32_t time_ms,float PWM);
    void  out1_buzzer_PWM_emergency_stop(void);
    void  out2_PWM_emergency_stop(void);
    bool  out1_buzzer_PWM_status_get(void);
    bool  out2_PWM_status_get(void);
    
#ifdef __cplusplus
}
#endif

#endif /*BUZZER_H */