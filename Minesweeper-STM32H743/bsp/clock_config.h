#ifndef CLOCK_CONFIG_H
#define CLOCK_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

/*
 * Initialize STM32H743ZI2 system clock.
 */
void ClockConfig_Init(void);

/*
 * Return configured system clock frequency.
 */
uint32_t ClockConfig_GetSystemClock(void);

/*
 * Check clock initialization.
 */
bool ClockConfig_IsReady(void);

#endif /* CLOCK_CONFIG_H */
