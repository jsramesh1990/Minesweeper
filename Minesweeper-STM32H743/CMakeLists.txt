cmake_minimum_required(VERSION 3.16)

project(MinesweeperSTM32H743
        VERSION 1.0
        LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

#
# Compiler warnings
#
add_compile_options(
    -Wall
    -Wextra
    -Werror
)

#
# Application game logic
#
set(GAME_SOURCES
    app/minesweeper/game_logic.c
    app/minesweeper/minesweeper.c
)

#
# Game logic library
#
add_library(game_logic STATIC
    ${GAME_SOURCES}
)

target_include_directories(game_logic
    PUBLIC
        app/minesweeper
)

#
# Test: Game Logic
#
add_executable(test_game_logic
    tests/test_game_logic.c
)

target_link_libraries(test_game_logic
    PRIVATE
        game_logic
)

#
# Test: Flood Fill
#
add_executable(test_flood_fill
    tests/test_flood_fill.c
)

target_link_libraries(test_flood_fill
    PRIVATE
        game_logic
)

#
# Test: Mines
#
add_executable(test_mines
    tests/test_mines.c
)

target_link_libraries(test_mines
    PRIVATE
        game_logic
)

#
# Enable CTest
#
enable_testing()

add_test(
    NAME GameLogicTest
    COMMAND test_game_logic
)

add_test(
    NAME FloodFillTest
    COMMAND test_flood_fill
)

add_test(
    NAME MinesTest
    COMMAND test_mines
)
