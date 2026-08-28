#include <assert.h>
#include <stdio.h>

#include "../app/minesweeper/game_logic.h"

static void test_game_initialization(void)
{
    MinesweeperGame game;

    GameLogic_Init(&game);

    assert(game.initialized == true);

    printf("[PASS] Game initialization\n");
}

static void test_board_dimensions(void)
{
    assert(BOARD_ROWS == 9);
    assert(BOARD_COLS == 9);

    printf("[PASS] Board dimensions\n");
}

static void test_valid_coordinates(void)
{
    assert(GameLogic_IsValidCell(0, 0) == true);
    assert(GameLogic_IsValidCell(8, 8) == true);

    assert(GameLogic_IsValidCell(-1, 0) == false);
    assert(GameLogic_IsValidCell(0, -1) == false);
    assert(GameLogic_IsValidCell(9, 0) == false);
    assert(GameLogic_IsValidCell(0, 9) == false);

    printf("[PASS] Coordinate validation\n");
}

int main(void)
{
    printf("\n");
    printf("=== Game Logic Tests ===\n\n");

    test_game_initialization();
    test_board_dimensions();
    test_valid_coordinates();

    printf("\nAll game logic tests passed.\n");

    return 0;
}
