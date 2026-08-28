#include "main.h"

#include "board.h"
#include "input_manager.h"
#include "game_state.h"
#include "minesweeper.h"
#include "game_display.h"

int main(void)
{
    /*
     * HAL initialization.
     */
    HAL_Init();

    /*
     * Board Support Package initialization.
     */
    Board_Init();

    /*
     * Application initialization.
     */
    InputManager_Init();
    GameDisplay_Init();
    GameState_Init();

    /*
     * Minesweeper game initialization.
     */
    MinesweeperGame game;

    Minesweeper_Init(&game);

    /*
     * Main application loop.
     */
    while (1)
    {
        GameEvent event;

        event = InputManager_GetEvent();

        if (event != GAME_EVENT_NONE)
        {
            /*
             * Process application event.
             */
            GameState_ProcessEvent(event);

            /*
             * Game state handling.
             */
            switch (GameState_Get())
            {
                case GAME_STATE_IDLE:

                    /*
                     * Waiting for START.
                     */
                    break;

                case GAME_STATE_NEW_GAME:

                    Minesweeper_NewGame(&game);

                    GameState_Set(GAME_STATE_PLAYING);

                    break;

                case GAME_STATE_PLAYING:

                    if (event == GAME_EVENT_UP)
                    {
                        Minesweeper_MoveCursor(
                            &game, -1, 0);
                    }
                    else if (event == GAME_EVENT_DOWN)
                    {
                        Minesweeper_MoveCursor(
                            &game, 1, 0);
                    }
                    else if (event == GAME_EVENT_LEFT)
                    {
                        Minesweeper_MoveCursor(
                            &game, 0, -1);
                    }
                    else if (event == GAME_EVENT_RIGHT)
                    {
                        Minesweeper_MoveCursor(
                            &game, 0, 1);
                    }
                    else if (event == GAME_EVENT_SELECT)
                    {
                        Minesweeper_Reveal(
                            &game,
                            game.cursor_row,
                            game.cursor_col);
                    }
                    else if (event == GAME_EVENT_FLAG)
                    {
                        Minesweeper_ToggleFlag(
                            &game,
                            game.cursor_row,
                            game.cursor_col);
                    }

                    if (Minesweeper_GetState(&game)
                        == GAME_WON)
                    {
                        GameState_Set(GAME_STATE_WON);
                    }
                    else if (Minesweeper_GetState(&game)
                             == GAME_LOST)
                    {
                        GameState_Set(GAME_STATE_LOST);
                    }

                    break;

                case GAME_STATE_WON:

                    GameDisplay_ShowWin(&game);

                    break;

                case GAME_STATE_LOST:

                    GameDisplay_ShowGameOver(&game);

                    break;

                default:
                    break;
            }
        }

        /*
         * Update display.
         */
        GameDisplay_ShowBoard(&game);

        /*
         * Small scheduling delay.
         *
         * In the final RTOS version this can be
         * replaced by task scheduling.
         */
        HAL_Delay(10);
    }
}
