#ifndef GAME_DISPLAY_H
#define GAME_DISPLAY_H

#include "minesweeper.h"

/*
 * Initialize game display.
 */
void GameDisplay_Init(void);

/*
 * Show the complete Minesweeper board.
 */
void GameDisplay_ShowBoard(const MinesweeperGame *game);

/*
 * Show the current cursor position.
 */
void GameDisplay_ShowCursor(uint8_t row, uint8_t col);

/*
 * Show game status.
 */
void GameDisplay_ShowStatus(GameState state);

/*
 * Show win screen.
 */
void GameDisplay_ShowWin(const MinesweeperGame *game);

/*
 * Show game-over screen.
 */
void GameDisplay_ShowGameOver(const MinesweeperGame *game);

/*
 * Clear display.
 */
void GameDisplay_Clear(void);

#endif /* GAME_DISPLAY_H */
