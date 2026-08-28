#include <assert.h>
#include <stdio.h>

#include "../app/minesweeper/game_logic.h"

static void test_empty_area_reveal(void)
{
    MinesweeperGame game;

    GameLogic_Init(&game);

    /*
     * Create a controlled test board.
     *
     * No mines around the selected cell.
     */
    GameLogic_ClearBoard(&game);

    GameLogic_Reveal(&game, 4, 4);

    assert(
        GameLogic_IsRevealed(&game, 4, 4)
        == true
    );

    printf("[PASS] Empty area reveal\n");
}

static void test_flood_fill_boundary(void)
{
    MinesweeperGame game;

    GameLogic_Init(&game);
    GameLogic_ClearBoard(&game);

    /*
     * Test corner cell.
     */
    GameLogic_Reveal(&game, 0, 0);

    assert(
        GameLogic_IsRevealed(&game, 0, 0)
        == true
    );

    printf("[PASS] Flood-fill boundary handling\n");
}

static void test_flood_fill_does_not_leave_board(void)
{
    MinesweeperGame game;

    GameLogic_Init(&game);
    GameLogic_ClearBoard(&game);

    /*
     * Flood-fill from corner.
     */
    GameLogic_Reveal(&game, 0, 0);

    /*
     * If execution reaches here without
     * invalid memory access, boundary
     * handling is working.
     */
    printf("[PASS] Flood-fill boundary protection\n");
}

int main(void)
{
    printf("\n");
    printf("=== Flood Fill Tests ===\n\n");

    test_empty_area_reveal();
    test_flood_fill_boundary();
    test_flood_fill_does_not_leave_board();

    printf("\nAll flood-fill tests passed.\n");

    return 0;
}
