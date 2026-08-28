#include "game_state.h"

static GameState current_state = GAME_STATE_INIT;


/*
 * Initialize state machine.
 */
void GameState_Init(void)
{
    current_state = GAME_STATE_INIT;
}


/*
 * Process state-machine events.
 */
void GameState_ProcessEvent(GameEvent event)
{
    switch (current_state)
    {
        case GAME_STATE_INIT:

            if (event == GAME_EVENT_NONE)
            {
                current_state = GAME_STATE_IDLE;
            }

            break;


        case GAME_STATE_IDLE:

            if (event == GAME_EVENT_START)
            {
                current_state = GAME_STATE_NEW_GAME;
            }

            break;


        case GAME_STATE_NEW_GAME:

            /*
             * New board initialization is handled
             * by the Minesweeper game controller.
             */

            if (event == GAME_EVENT_NONE)
            {
                current_state = GAME_STATE_PLAYING;
            }

            break;


        case GAME_STATE_PLAYING:

            /*
             * Normal gameplay events.
             *
             * UP/DOWN/LEFT/RIGHT
             * SELECT
             * FLAG
             *
             * are processed by the game controller.
             */

            if (event == GAME_EVENT_NEW_GAME)
            {
                current_state = GAME_STATE_NEW_GAME;
            }

            break;


        case GAME_STATE_WON:

            if (event == GAME_EVENT_NEW_GAME)
            {
                current_state = GAME_STATE_NEW_GAME;
            }
            else if (event == GAME_EVENT_EXIT)
            {
                current_state = GAME_STATE_IDLE;
            }

            break;


        case GAME_STATE_LOST:

            if (event == GAME_EVENT_NEW_GAME)
            {
                current_state = GAME_STATE_NEW_GAME;
            }
            else if (event == GAME_EVENT_EXIT)
            {
                current_state = GAME_STATE_IDLE;
            }

            break;


        default:

            current_state = GAME_STATE_INIT;

            break;
    }
}


/*
 * Return current game state.
 */
GameState GameState_Get(void)
{
    return current_state;
}


/*
 * Force state transition.
 */
void GameState_Set(GameState state)
{
    current_state = state;
}


/*
 * Check whether the current state matches
 * the requested state.
 */
bool GameState_Is(GameState state)
{
    return current_state == state;
}
