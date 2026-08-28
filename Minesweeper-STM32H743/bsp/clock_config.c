#include "clock_config.h"

/*
 * System clock frequency.
 *
 * Set this to the actual configured
 * STM32H743ZI2 SYSCLK.
 */
static uint32_t system_clock_hz = 0;

static bool clock_ready = false;


void ClockConfig_Init(void)
{
    /*
     * STM32H743ZI2 clock configuration.
     *
     * Typical sequence:
     *
     * 1. Configure power supply / voltage scaling
     * 2. Configure HSE/HSI oscillator
     * 3. Configure PLL
     * 4. Configure AHB prescaler
     * 5. Configure APB prescalers
     * 6. Configure system clock source
     * 7. Configure Flash latency
     *
     * In a CubeMX project, the actual HAL
     * implementation can be generated in
     * SystemClock_Config().
     */

    /*
     * Example placeholder:
     *
     * SystemClock_Config();
     */

    system_clock_hz = 0;

    clock_ready = true;
}


uint32_t ClockConfig_GetSystemClock(void)
{
    return system_clock_hz;
}


bool ClockConfig_IsReady(void)
{
    return clock_ready;
}
