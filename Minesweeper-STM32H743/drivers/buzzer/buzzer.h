#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
#include <stdbool.h>

/*
 * Initialize buzzer.
 */
void Buzzer_Init(void);

/*
 * Turn buzzer on.
 */
void Buzzer_On(uint32_t frequency_hz);

/*
 * Turn buzzer off.
 */
void Buzzer_Off(void);

/*
 * Generate short beep.
 */
void Buzzer_Beep(uint32_t frequency_hz,
                 uint32_t duration_ms);

/*
 * Check buzzer status.
 */
bool Buzzer_IsActive(void);

#endif /* BUZZER_H */
