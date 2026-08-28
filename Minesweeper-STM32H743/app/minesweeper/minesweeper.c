#include "minesweeper.h"
#include "game_logic.h"

void Minesweeper_Init(MinesweeperGame *game)
{
    if (game == NULL)
    {
        return;
    }

    game->cursor_row = 0;
    game->cursor_col = 0;
    game->mines = MINE_COUNT;
    game->flags = 0;
    game->revealed_cells = 0;
    game->state = GAME_IDLE;

    GameLogic_ClearBoard(game);
}

void Minesweeper_NewGame(MinesweeperGame *game)
{
    if (game == NULL)
    {
        return;
    }

    GameLogic_ClearBoard(game);

    game->cursor_row = 0;
    game->cursor_col = 0;
    game->mines = MINE_COUNT;
    game->flags = 0;
    game->revealed_cells = 0;
    game->state = GAME_PLAYING;

    GameLogic_PlaceMines(game);
    GameLogic_CalculateNumbers(game);
}

void Minesweeper_Reveal(MinesweeperGame *game,
                        uint8_t row,
                        uint8_t col)
{
    if (game == NULL)
    {
        return;
    }

    if (game->state != GAME_PLAYING)
    {
        return;
    }

    if (!GameLogic_IsValidCell(row, col))
    {
        return;
    }

    if (GameLogic_IsFlagged(game, row, col))
    {
        return;
    }

    if (GameLogic_IsMine(game, row, col))
    {
        GameLogic_RevealAllMines(game);
        game->state = GAME_LOST;
        return;
    }

    GameLogic_FloodFill(game, row, col);

    if (GameLogic_CheckWin(game))
    {
        game->state = GAME_WON;
    }
}

void Minesweeper_ToggleFlag(MinesweeperGame *game,
                            uint8_t row,
                            uint8_t col)
{
    if (game == NULL)
    {
        return;
    }

    if (game->state != GAME_PLAYING)
    {
        return;
    }

    if (!GameLogic_IsValidCell(row, col))
    {
        return;
    }

    if (GameLogic_ToggleFlag(game, row, col))
    {
        game->flags++;
    }
}

void Minesweeper_MoveCursor(MinesweeperGame *game,
                            int8_t row_delta,
                            int8_t col_delta)
{
    if (game == NULL)
    {
        return;
    }

    int16_t new_row = game->cursor_row + row_delta;
    int16_t new_col = game->cursor_col + col_delta;

    if (new_row >= 0 && new_row < BOARD_ROWS)
    {
        game->cursor_row = (uint8_t)new_row;
    }

    if (new_col >= 0 && new_col < BOARD_COLS)
    {
        game->cursor_col = (uint8_t)new_col;
    }
}

GameState Minesweeper_GetState(const MinesweeperGame *game)
{
    if (game == NULL)
    {
        return GAME_IDLE;
    }

    return game->state;
}

bool Minesweeper_IsWon(const MinesweeperGame *game)
{
    if (game == NULL)
    {
        return false;
    }

    return game->state == GAME_WON;
}
