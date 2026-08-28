#ifndef MUTEX_H
#define MUTEX_H

#include "FreeRTOS.h"
#include "semphr.h"
#include <stdbool.h>

/*
 * Initialize RTOS mutexes.
 */
void Mutex_Init(void);

/*
 * Lock display resource.
 */
bool Mutex_DisplayLock(TickType_t timeout);

/*
 * Unlock display resource.
 */
void Mutex_DisplayUnlock(void);

/*
 * Get display mutex.
 */
SemaphoreHandle_t Mutex_GetDisplay(void);

#endif /* MUTEX_H */
