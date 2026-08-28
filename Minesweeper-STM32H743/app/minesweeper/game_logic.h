#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "minesweeper.h"

/* Board management */
void GameLogic_ClearBoard(MinesweeperGame *game);

/* Mine generation */
void GameLogic_PlaceMines(MinesweeperGame *game);

/* Number calculation */
void GameLogic_CalculateNumbers(MinesweeperGame *game);

/* Cell validation */
bool GameLogic_IsValidCell(uint8_t row, uint8_t col);

/* Cell information */
bool GameLogic_IsMine(const MinesweeperGame *game,
                      uint8_t row,
                      uint8_t col);

bool GameLogic_IsFlagged(const MinesweeperGame *game,
                         uint8_t row,
                         uint8_t col);

/* Cell operations */
bool GameLogic_ToggleFlag(MinesweeperGame *game,
                          uint8_t row,
                          uint8_t col);

void GameLogic_FloodFill(MinesweeperGame *game,
                         uint8_t row,
                         uint8_t col);

/* Game result */
bool GameLogic_CheckWin(const MinesweeperGame *game);

/* Reveal mines after losing */
void GameLogic_RevealAllMines(MinesweeperGame *game);

#endif /* GAME_LOGIC_H */
