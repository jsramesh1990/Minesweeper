#ifndef TASKS_H
#define TASKS_H

#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

/*
 * RTOS task priorities.
 */
#define INPUT_TASK_PRIORITY      (tskIDLE_PRIORITY + 3)
#define GAME_TASK_PRIORITY       (tskIDLE_PRIORITY + 2)
#define DISPLAY_TASK_PRIORITY    (tskIDLE_PRIORITY + 1)
#define TIMER_TASK_PRIORITY      (tskIDLE_PRIORITY + 1)

/*
 * Task stack sizes.
 */
#define INPUT_TASK_STACK_SIZE    256
#define GAME_TASK_STACK_SIZE     512
#define DISPLAY_TASK_STACK_SIZE  512
#define TIMER_TASK_STACK_SIZE    256

/*
 * Initialize and create all application tasks.
 */
void Tasks_Init(void);

/*
 * Individual task functions.
 */
void InputTask(void *argument);
void GameTask(void *argument);
void DisplayTask(void *argument);
void TimerTask(void *argument);

#endif /* TASKS_H */
