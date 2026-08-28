#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <stdint.h>
#include <stdbool.h>

#define BOARD_ROWS      9
#define BOARD_COLS      9
#define MINE_COUNT      10

typedef enum
{
    CELL_HIDDEN = 0,
    CELL_REVEALED,
    CELL_FLAGGED
} CellState;

typedef struct
{
    bool mine;
    uint8_t adjacent_mines;
    CellState state;
} Cell;

typedef enum
{
    GAME_IDLE = 0,
    GAME_PLAYING,
    GAME_WON,
    GAME_LOST
} GameState;

typedef struct
{
    Cell board[BOARD_ROWS][BOARD_COLS];

    uint8_t cursor_row;
    uint8_t cursor_col;

    uint8_t mines;
    uint8_t flags;
    uint16_t revealed_cells;

    GameState state;

} MinesweeperGame;


/* Game initialization */
void Minesweeper_Init(MinesweeperGame *game);

/* Start a new game */
void Minesweeper_NewGame(MinesweeperGame *game);

/* Process player selection */
void Minesweeper_Reveal(MinesweeperGame *game,
                        uint8_t row,
                        uint8_t col);

/* Toggle flag */
void Minesweeper_ToggleFlag(MinesweeperGame *game,
                            uint8_t row,
                            uint8_t col);

/* Move cursor */
void Minesweeper_MoveCursor(MinesweeperGame *game,
                             int8_t row_delta,
                             int8_t col_delta);

/* Get current game state */
GameState Minesweeper_GetState(const MinesweeperGame *game);

/* Check whether the game has been won */
bool Minesweeper_IsWon(const MinesweeperGame *game);

#endif /* MINESWEEPER_H */
