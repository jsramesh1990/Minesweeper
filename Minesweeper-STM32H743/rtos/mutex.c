#include "mutex.h"

static SemaphoreHandle_t display_mutex = NULL;


void Mutex_Init(void)
{
    display_mutex = xSemaphoreCreateMutex();
}


bool Mutex_DisplayLock(TickType_t timeout)
{
    if (display_mutex == NULL)
    {
        return false;
    }

    return (
        xSemaphoreTake(
            display_mutex,
            timeout
        ) == pdTRUE
    );
}


void Mutex_DisplayUnlock(void)
{
    if (display_mutex != NULL)
    {
        xSemaphoreGive(display_mutex);
    }
}


SemaphoreHandle_t Mutex_GetDisplay(void)
{
    return display_mutex;
}
