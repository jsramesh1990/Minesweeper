#include "buzzer.h"

/*
 * Buzzer state.
 */
static bool buzzer_active = false;

void Buzzer_Init(void)
{
    /*
     * Initialize timer PWM output
     * connected to the buzzer.
     */
}

void Buzzer_On(uint32_t frequency_hz)
{
    if (frequency_hz == 0)
    {
        return;
    }

    /*
     * Configure timer PWM frequency.
     *
     * Start PWM output.
     */

    buzzer_active = true;
}

void Buzzer_Off(void)
{
    /*
     * Stop PWM output.
     */

    buzzer_active = false;
}

void Buzzer_Beep(uint32_t frequency_hz,
                 uint32_t duration_ms)
{
    Buzzer_On(frequency_hz);

    /*
     * In a production embedded system,
     * avoid blocking delays here.
     *
     * A timer/task can be used to stop
     * the buzzer asynchronously.
     */

    (void)duration_ms;
}

bool Buzzer_IsActive(void)
{
    return buzzer_active;
}
