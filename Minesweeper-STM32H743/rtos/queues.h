#ifndef QUEUES_H
#define QUEUES_H

#include "FreeRTOS.h"
#include "queue.h"

#include "game_state.h"
#include <stdbool.h>

/*
 * Initialize RTOS queues.
 */
void Queues_Init(void);

/*
 * Send game event from one task
 * to another.
 */
bool Queue_SendGameEvent(GameEvent event);

/*
 * Receive game event.
 */
bool Queue_ReceiveGameEvent(
    GameEvent *event,
    TickType_t timeout
);

/*
 * Get queue handle.
 */
QueueHandle_t Queue_GetGameQueue(void);

#endif /* QUEUES_H */
