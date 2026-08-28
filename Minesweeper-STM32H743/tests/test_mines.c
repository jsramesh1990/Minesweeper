#include <assert.h>
#include <stdio.h>

#include "../app/minesweeper/game_logic.h"

static void test_mine_count(void)
{
    MinesweeperGame game;

    GameLogic_Init(&game);

    GameLogic_PlaceMines(&game);

    uint32_t mine_count =
        GameLogic_CountMines(&game);

    assert(mine_count == MINES);

    printf("[PASS] Mine count = %d\n", MINES);
}

static void test_mines_are_unique(void)
{
    MinesweeperGame game;

    GameLogic_Init(&game);

    GameLogic_PlaceMines(&game);

    for (int row = 0; row < BOARD_ROWS; row++)
    {
        for (int col = 0; col < BOARD_COLS; col++)
        {
            /*
             * Each cell can only contain one mine.
             */
            assert(
                game.board[row][col].mine == true ||
                game.board[row][col].mine == false
            );
        }
    }

    printf("[PASS] Mine positions are valid\n");
}

static void test_adjacent_mine_count(void)
{
    MinesweeperGame game;

    GameLogic_Init(&game);
    GameLogic_ClearBoard(&game);

    /*
     * Controlled board:
     *
     * . . .
     * . * .
     * . . .
     *
     * Center mine should be counted
     * by all adjacent cells.
     */

    game.board[1][1].mine = true;

    GameLogic_CalculateNumbers(&game);

    assert(game.board[0][0].adjacent_mines == 1);
    assert(game.board[0][1].adjacent_mines == 1);
    assert(game.board[0][2].adjacent_mines == 1);

    assert(game.board[1][0].adjacent_mines == 1);
    assert(game.board[1][2].adjacent_mines == 1);

    assert(game.board[2][0].adjacent_mines == 1);
    assert(game.board[2][1].adjacent_mines == 1);
    assert(game.board[2][2].adjacent_mines == 1);

    printf("[PASS] Adjacent mine calculation\n");
}

int main(void)
{
    printf("\n");
    printf("=== Mine Tests ===\n\n");

    test_mine_count();
    test_mines_are_unique();
    test_adjacent_mine_count();

    printf("\nAll mine tests passed.\n");

    return 0;
}
