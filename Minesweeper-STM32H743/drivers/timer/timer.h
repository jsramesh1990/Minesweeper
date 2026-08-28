#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

/*
 * Initialize timer.
 */
void Timer_Init(void);

/*
 * Start timer.
 */
void Timer_Start(void);

/*
 * Stop timer.
 */
void Timer_Stop(void);

/*
 * Reset timer.
 */
void Timer_Reset(void);

/*
 * Get elapsed milliseconds.
 */
uint32_t Timer_GetMilliseconds(void);

/*
 * Get elapsed seconds.
 */
uint32_t Timer_GetSeconds(void);

/*
 * Check timer status.
 */
bool Timer_IsRunning(void);

#endif /* TIMER_H */
