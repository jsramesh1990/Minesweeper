#include "tasks.h"

#include "input_manager.h"
#include "game_state.h"
#include "minesweeper.h"
#include "game_display.h"
#include "queues.h"
#include "timer.h"

static TaskHandle_t input_task_handle = NULL;
static TaskHandle_t game_task_handle = NULL;
static TaskHandle_t display_task_handle = NULL;
static TaskHandle_t timer_task_handle = NULL;

void Tasks_Init(void)
{
    xTaskCreate(
        InputTask,
        "Input",
        INPUT_TASK_STACK_SIZE,
        NULL,
        INPUT_TASK_PRIORITY,
        &input_task_handle
    );

    xTaskCreate(
        GameTask,
        "Game",
        GAME_TASK_STACK_SIZE,
        NULL,
        GAME_TASK_PRIORITY,
        &game_task_handle
    );

    xTaskCreate(
        DisplayTask,
        "Display",
        DISPLAY_TASK_STACK_SIZE,
        NULL,
        DISPLAY_TASK_PRIORITY,
        &display_task_handle
    );

    xTaskCreate(
        TimerTask,
        "Timer",
        TIMER_TASK_STACK_SIZE,
        NULL,
        TIMER_TASK_PRIORITY,
        &timer_task_handle
    );
}


/*
 * Reads physical buttons and sends
 * events to the Game task.
 */
void InputTask(void *argument)
{
    (void)argument;

    TickType_t last_wake_time =
        xTaskGetTickCount();

    while (1)
    {
        GameEvent event;

        event = InputManager_GetEvent();

        if (event != GAME_EVENT_NONE)
        {
            Queue_SendGameEvent(event);
        }

        vTaskDelayUntil(
            &last_wake_time,
            pdMS_TO_TICKS(20)
        );
    }
}


/*
 * Main Minesweeper application task.
 */
void GameTask(void *argument)
{
    (void)argument;

    GameEvent event;

    while (1)
    {
        if (Queue_ReceiveGameEvent(
                &event,
                portMAX_DELAY))
        {
            /*
             * Process state-machine event.
             */
            GameState_ProcessEvent(event);

            /*
             * Actual Minesweeper processing
             * belongs here / application layer.
             */
        }
    }
}


/*
 * Display update task.
 */
void DisplayTask(void *argument)
{
    (void)argument;

    while (1)
    {
        /*
         * Display current game state.
         */

        vTaskDelay(
            pdMS_TO_TICKS(50)
        );
    }
}


/*
 * Game timer task.
 */
void TimerTask(void *argument)
{
    (void)argument;

    while (1)
    {
        /*
         * Update game timer.
         */

        vTaskDelay(
            pdMS_TO_TICKS(1000)
        );
    }
}
