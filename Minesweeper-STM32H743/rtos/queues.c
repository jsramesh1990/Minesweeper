#include "queues.h"

#define GAME_EVENT_QUEUE_LENGTH    16

static QueueHandle_t game_event_queue = NULL;

void Queues_Init(void)
{
    game_event_queue =
        xQueueCreate(
            GAME_EVENT_QUEUE_LENGTH,
            sizeof(GameEvent)
        );
}


bool Queue_SendGameEvent(GameEvent event)
{
    if (game_event_queue == NULL)
    {
        return false;
    }

    return (
        xQueueSend(
            game_event_queue,
            &event,
            0
        ) == pdPASS
    );
}


bool Queue_ReceiveGameEvent(
    GameEvent *event,
    TickType_t timeout)
{
    if ((game_event_queue == NULL) ||
        (event == NULL))
    {
        return false;
    }

    return (
        xQueueReceive(
            game_event_queue,
            event,
            timeout
        ) == pdPASS
    );
}


QueueHandle_t Queue_GetGameQueue(void)
{
    return game_event_queue;
}
