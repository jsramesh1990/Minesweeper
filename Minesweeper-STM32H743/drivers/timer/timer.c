#include "timer.h"
#include "stm32h7xx_hal.h"

static uint32_t start_time = 0;
static bool timer_running = false;

void Timer_Init(void)
{
    start_time = 0;
    timer_running = false;
}

void Timer_Start(void)
{
    start_time = HAL_GetTick();

    timer_running = true;
}

void Timer_Stop(void)
{
    timer_running = false;
}

void Timer_Reset(void)
{
    start_time = HAL_GetTick();
}

uint32_t Timer_GetMilliseconds(void)
{
    if (!timer_running)
    {
        return 0;
    }

    return HAL_GetTick() - start_time;
}

uint32_t Timer_GetSeconds(void)
{
    return Timer_GetMilliseconds() / 1000U;
}

bool Timer_IsRunning(void)
{
    return timer_running;
}
