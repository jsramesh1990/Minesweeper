#include "game_display.h"

/*
 * Hardware display driver will be connected here later.
 *
 * Example:
 *
 * #include "lcd.h"
 * #include "oled.h"
 */

void GameDisplay_Init(void)
{
    /*
     * Initialize LCD/OLED.
     *
     * Example:
     * LCD_Init();
     */
}

void GameDisplay_ShowBoard(const MinesweeperGame *game)
{
    if (game == 0)
    {
        return;
    }

    /*
     * Render Minesweeper board.
     *
     * CELL_HIDDEN    -> "#"
     * CELL_REVEALED  -> number / empty / mine
     * CELL_FLAGGED   -> "F"
     */

    for (uint8_t row = 0; row < BOARD_ROWS; row++)
    {
        for (uint8_t col = 0; col < BOARD_COLS; col++)
        {
            const Cell *cell = &game->board[row][col];

            if (cell->state == CELL_FLAGGED)
            {
                /* Display flag */
            }
            else if (cell->state == CELL_HIDDEN)
            {
                /* Display hidden cell */
            }
            else
            {
                if (cell->mine)
                {
                    /* Display mine */
                }
                else
                {
                    /*
                     * Display adjacent mine count.
                     */
                }
            }
        }
    }
}

void GameDisplay_ShowCursor(uint8_t row, uint8_t col)
{
    /*
     * Highlight selected cell.
     */
    (void)row;
    (void)col;
}

void GameDisplay_ShowStatus(GameState state)
{
    switch (state)
    {
        case GAME_IDLE:
            /* Display "READY" */
            break;

        case GAME_PLAYING:
            /* Display "PLAYING" */
            break;

        case GAME_WON:
            /* Display "YOU WIN" */
            break;

        case GAME_LOST:
            /* Display "GAME OVER" */
            break;

        default:
            break;
    }
}

void GameDisplay_ShowWin(const MinesweeperGame *game)
{
    (void)game;

    /*
     * Display:
     *
     * +----------------+
     * |    YOU WIN!    |
     * |                |
     * |   Time: 01:25  |
     * +----------------+
     */
}

void GameDisplay_ShowGameOver(const MinesweeperGame *game)
{
    (void)game;

    /*
     * Display:
     *
     * +----------------+
     * |   GAME OVER    |
     * |                |
     * |     BOOM!      |
     * +----------------+
     */
}

void GameDisplay_Clear(void)
{
    /*
     * Clear LCD/OLED.
     */
}
