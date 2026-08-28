CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c11 -g

GAME_DIR = app/minesweeper
TEST_DIR = tests

GAME_SOURCES = \
	$(GAME_DIR)/game_logic.c \
	$(GAME_DIR)/minesweeper.c

GAME_OBJECTS = $(GAME_SOURCES:.c=.o)

TEST_GAME = test_game_logic
TEST_FLOOD = test_flood_fill
TEST_MINES = test_mines

.PHONY: all clean test game_logic

all: $(TEST_GAME) $(TEST_FLOOD) $(TEST_MINES)

$(TEST_GAME): $(TEST_DIR)/test_game_logic.c $(GAME_SOURCES)
	$(CC) $(CFLAGS) \
		-I$(GAME_DIR) \
		-o $@ \
		$^

$(TEST_FLOOD): $(TEST_DIR)/test_flood_fill.c $(GAME_SOURCES)
	$(CC) $(CFLAGS) \
		-I$(GAME_DIR) \
		-o $@ \
		$^

$(TEST_MINES): $(TEST_DIR)/test_mines.c $(GAME_SOURCES)
	$(CC) $(CFLAGS) \
		-I$(GAME_DIR) \
		-o $@ \
		$^

test: all
	./$(TEST_GAME)
	./$(TEST_FLOOD)
	./$(TEST_MINES)

clean:
	rm -f $(TEST_GAME)
	rm -f $(TEST_FLOOD)
	rm -f $(TEST_MINES)
	rm -f $(GAME_OBJECTS)
