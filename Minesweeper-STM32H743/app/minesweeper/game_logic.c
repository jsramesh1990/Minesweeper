#include "game_logic.h"
#include <stdlib.h>

static const int8_t direction_row[8] =
{
    -1, -1, -1,
     0,  0,
     1,  1,  1
};

static const int8_t direction_col[8] =
{
    -1,  0,  1,
    -1,  1,
    -1,  0,  1
};

static uint8_t CountAdjacentMines(
    const MinesweeperGame *game,
    uint8_t row,
    uint8_t col)
{
    uint8_t count = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        int16_t nr = row + direction_row[i];
        int16_t nc = col + direction_col[i];

        if (nr >= 0 &&
            nr < BOARD_ROWS &&
            nc >= 0 &&
            nc < BOARD_COLS)
        {
            if (game->board[nr][nc].mine)
            {
                count++;
            }
        }
    }

    return count;
}

void GameLogic_ClearBoard(MinesweeperGame *game)
{
    for (uint8_t row = 0; row < BOARD_ROWS; row++)
    {
        for (uint8_t col = 0; col < BOARD_COLS; col++)
        {
            game->board[row][col].mine = false;
            game->board[row][col].adjacent_mines = 0;
            game->board[row][col].state = CELL_HIDDEN;
        }
    }
}

void GameLogic_PlaceMines(MinesweeperGame *game)
{
    uint8_t placed = 0;

    while (placed < MINE_COUNT)
    {
        uint8_t row = rand() % BOARD_ROWS;
        uint8_t col = rand() % BOARD_COLS;

        if (!game->board[row][col].mine)
        {
            game->board[row][col].mine = true;
            placed++;
        }
    }
}

void GameLogic_CalculateNumbers(MinesweeperGame *game)
{
    for (uint8_t row = 0; row < BOARD_ROWS; row++)
    {
        for (uint8_t col = 0; col < BOARD_COLS; col++)
        {
            if (!game->board[row][col].mine)
            {
                game->board[row][col].adjacent_mines =
                    CountAdjacentMines(game, row, col);
            }
        }
    }
}

bool GameLogic_IsValidCell(uint8_t row, uint8_t col)
{
    return (row < BOARD_ROWS &&
            col < BOARD_COLS);
}

bool GameLogic_IsMine(const MinesweeperGame *game,
                      uint8_t row,
                      uint8_t col)
{
    return game->board[row][col].mine;
}

bool GameLogic_IsFlagged(const MinesweeperGame *game,
                         uint8_t row,
                         uint8_t col)
{
    return game->board[row][col].state == CELL_FLAGGED;
}

bool GameLogic_ToggleFlag(MinesweeperGame *game,
                          uint8_t row,
                          uint8_t col)
{
    Cell *cell = &game->board[row][col];

    if (cell->state == CELL_REVEALED)
    {
        return false;
    }

    if (cell->state == CELL_FLAGGED)
    {
        cell->state = CELL_HIDDEN;
        return false;
    }

    cell->state = CELL_FLAGGED;

    return true;
}

void GameLogic_FloodFill(MinesweeperGame *game,
                         uint8_t row,
                         uint8_t col)
{
    if (!GameLogic_IsValidCell(row, col))
    {
        return;
    }

    Cell *cell = &game->board[row][col];

    if (cell->state == CELL_REVEALED)
    {
        return;
    }

    if (cell->state == CELL_FLAGGED)
    {
        return;
    }

    if (cell->mine)
    {
        return;
    }

    cell->state = CELL_REVEALED;
    game->revealed_cells++;

    if (cell->adjacent_mines != 0)
    {
        return;
    }

    for (uint8_t i = 0; i < 8; i++)
    {
        int16_t nr = row + direction_row[i];
        int16_t nc = col + direction_col[i];

        if (nr >= 0 &&
            nr < BOARD_ROWS &&
            nc >= 0 &&
            nc < BOARD_COLS)
        {
            GameLogic_FloodFill(
                game,
                (uint8_t)nr,
                (uint8_t)nc);
        }
    }
}

bool GameLogic_CheckWin(const MinesweeperGame *game)
{
    const uint16_t total_safe_cells =
        (BOARD_ROWS * BOARD_COLS) - MINE_COUNT;

    return game->revealed_cells >= total_safe_cells;
}

void GameLogic_RevealAllMines(MinesweeperGame *game)
{
    for (uint8_t row = 0; row < BOARD_ROWS; row++)
    {
        for (uint8_t col = 0; col < BOARD_COLS; col++)
        {
            if (game->board[row][col].mine)
            {
                game->board[row][col].state =
                    CELL_REVEALED;
            }
        }
    }
}
