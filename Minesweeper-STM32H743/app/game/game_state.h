#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    GAME_STATE_INIT = 0,
    GAME_STATE_IDLE,
    GAME_STATE_NEW_GAME,
    GAME_STATE_PLAYING,
    GAME_STATE_WON,
    GAME_STATE_LOST
} GameState;

typedef enum
{
    GAME_EVENT_NONE = 0,

    GAME_EVENT_START,

    GAME_EVENT_UP,
    GAME_EVENT_DOWN,
    GAME_EVENT_LEFT,
    GAME_EVENT_RIGHT,

    GAME_EVENT_SELECT,
    GAME_EVENT_FLAG,

    GAME_EVENT_NEW_GAME,
    GAME_EVENT_EXIT

} GameEvent;


/*
 * Initialize game state machine.
 */
void GameState_Init(void);


/*
 * Process an event and perform
 * the required state transition.
 */
void GameState_ProcessEvent(GameEvent event);


/*
 * Get current state.
 */
GameState GameState_Get(void);


/*
 * Force transition to a new state.
 */
void GameState_Set(GameState state);


/*
 * Check current state.
 */
bool GameState_Is(GameState state);

#endif /* GAME_STATE_H */
