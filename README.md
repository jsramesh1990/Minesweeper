# Minesweeper - Terminal Edition

A classic Minesweeper puzzle game implemented in C++ that runs in the Ubuntu 24 terminal. Uncover cells without detonating hidden mines using logic and deduction.

## Features

- 💣 **Random mine placement** (10 mines on a 9×9 grid)
- 🔢 **Number hints** showing adjacent mine counts
- 🌊 **Recursive flood-fill** algorithm for empty cells
- 🎮 **Interactive gameplay** with coordinate input
- 🏆 **Win detection** (all non-mine cells revealed)
- 💀 **Loss detection** (mine detonation reveals all mines)
- 🧩 **Pure terminal UI** with simple graphics

## Prerequisites

- **Ubuntu 24** (or any Linux distribution)
- **G++ compiler** (part of build-essential package)

## Installation

### 1. Install G++ (if not already installed)

```bash
sudo apt update
sudo apt install g++
```

Verify installation:
```bash
g++ --version
```

### 2. Create the source file

```bash
nano minesweeper.cpp
```

Copy the complete C++ code from this repository into the file.

### 3. Compile the game

Basic compilation:
```bash
g++ minesweeper.cpp -o minesweeper
```

With warnings (recommended):
```bash
g++ -Wall -Wextra minesweeper.cpp -o minesweeper
```

### 4. Run the game

```bash
./minesweeper
```

## How to Play

### Objective
Reveal all cells that do not contain mines. Use number hints to deduce mine locations.

### Controls
- Enter coordinates in `row column` format
- Grid size: 9×9 (rows and columns numbered 0-8)
- Hidden cells show `#`
- Empty cells (0 adjacent mines) show `.`
- Number cells show `1-8`
- Mines show `*` (only revealed when game ends)

### Game Display Legend

| Symbol | Meaning |
|--------|---------|
| `#` | Hidden cell (not yet revealed) |
| `.` | Empty cell (0 adjacent mines) |
| `1-8` | Number of adjacent mines |
| `*` | Mine (game over if revealed) |

### Game Flow Example

```
   0 1 2 3 4 5 6 7 8
0  # # # # # # # # #
1  # # # # # # # # #
2  # # # # # # # # #
3  # # # # # # # # #
4  # # # # # # # # #
5  # # # # # # # # #
6  # # # # # # # # #
7  # # # # # # # # #
8  # # # # # # # # #

Enter row and column: 4 4

   0 1 2 3 4 5 6 7 8
0  . . . 1 # # # # #
1  . . . 1 # # # # #
2  . . . . 1 1 1 # #
3  . . . . . . 1 # #
4  . . . . . . 1 # #
5  . . . . . . 1 # #
6  . . . 1 1 1 1 # #
7  . . . 1 # # # # #
8  . . . 1 # # # # #
```

## Code Structure

### Class: `Minesweeper`

| Method | Purpose |
|--------|---------|
| `Minesweeper()` | Constructor - initializes boards, places mines, calculates numbers |
| `placeMines()` | Randomly places mines on the grid |
| `calculateNumbers()` | Counts adjacent mines for each cell |
| `printBoard()` | Displays current game state |
| `floodFill()` | **Recursive algorithm** to reveal empty areas |
| `playMove()` | Processes player's move and reveals cells |
| `checkWin()` | Determines if player has won |
| `isValid()` | Boundary checking utility |

### Key C++ Concepts Demonstrated

- **2D Vectors** - Dynamic board storage (`vector<vector<char>>`)
- **Recursion** - Flood-fill algorithm calls itself
- **Object-Oriented Programming** - Encapsulation in Minesweeper class
- **Random Number Generation** - `rand() % ROWS` for mine placement
- **Loops** - Nested loops for board initialization and display
- **Conditional Logic** - Win/loss detection and boundary checking

## Algorithm Deep Dive

### Place Mines

Randomly mines `MINES = 10` unique positions:
```cpp
while (placed < MINES) {
    int r = rand() % ROWS;
    int c = rand() % COLS;
    if (!mineBoard[r][c]) {
        mineBoard[r][c] = true;
        placed++;
    }
}
```

### Calculate Numbers

Uses eight directional offsets to count adjacent mines:
```cpp
int dr[] = {-1,-1,-1,0,0,1,1,1};
int dc[] = {-1,0,1,-1,1,-1,0,1};

for (int i = 0; i < 8; i++) {
    int nr = r + dr[i];
    int nc = c + dc[i];
    if (mineBoard[nr][nc]) count++;
}
```

### Flood-Fill Algorithm (Recursive)

When a cell with `0` adjacent mines is revealed:
1. Reveal the current cell
2. Stop if cell has a number (not zero)
3. Recursively reveal all eight neighbors

```cpp
void floodFill(int r, int c) {
    if (!isValid(r, c)) return;
    if (revealed[r][c]) return;
    if (board[r][c] == '*') return;
    
    revealed[r][c] = true;
    
    if (board[r][c] != '0') return;  // Stop at numbers
    
    for (int i = 0; i < 8; i++) {
        floodFill(r + dr[i], c + dc[i]);
    }
}
```

**Visual Example of Flood Fill:**
```
Before:          After click at (4,4):
0 0 0 1 *        0 0 0 1 *
0 0 1 2 2        0 0 1 2 2
0 0 1 * 1        0 0 1 * 1
1 1 1 1 1        1 1 1 1 1
```

## Customization Guide

### Change Board Size

Edit constants in the code:
```cpp
const int ROWS = 12;   // Change to desired rows
const int COLS = 12;   // Change to desired columns
const int MINES = 20;  // Adjust mines based on size
```

### Difficulty Presets

| Difficulty | Rows | Cols | Mines |
|------------|------|------|-------|
| Beginner | 9 | 9 | 10 |
| Intermediate | 16 | 16 | 40 |
| Expert | 16 | 30 | 99 |

### Change Display Symbols

Modify the `printBoard()` method:
```cpp
if (board[r][c] == '0')
    cout << "□ ";  // Different empty cell symbol
else
    cout << "■ ";  // Different hidden cell symbol
```

## Sample Game Session

```
   0 1 2 3 4 5 6 7 8
0  # # # # # # # # #
1  # # # # # # # # #
2  # # # # # # # # #
3  # # # # # # # # #
4  # # # # # # # # #
5  # # # # # # # # #
6  # # # # # # # # #
7  # # # # # # # # #
8  # # # # # # # # #

Enter row and column: 4 4

   0 1 2 3 4 5 6 7 8
0  . . . 1 # # # # #
1  . . . 1 # # # # #
2  . . . . 1 1 1 # #
3  . . . . . . 1 # #
4  . . . . . . 1 # #
5  . . . . . . 1 # #
6  . . . 1 1 1 1 # #
7  . . . 1 # # # # #
8  . . . 1 # # # # #

Enter row and column: 0 4

BOOM! You hit a mine!

   0 1 2 3 4 5 6 7 8
0  . . . 1 * # # # #
1  . . . 1 1 # # # #
2  . . . . 1 1 1 # #
3  . . . . . . 1 # #
4  . . . . . . 1 # #
5  . . . . . . 1 # #
6  . . . 1 1 1 1 # #
7  . . . 1 1 # # # #
8  . . . 1 # # # # #

Game Over!
```

## Troubleshooting

| Issue | Solution |
|-------|----------|
| `g++: command not found` | Install build-essential: `sudo apt install g++` |
| Program crashes on invalid input | Input validation exists; enter numbers only |
| Flood fill not working | Check recursion depth; default is sufficient for 9×9 |
| Same mine positions every game | Ensure `srand(time(0))` is in `main()` |
| Compilation errors with vectors | Include `<vector>` header (already in code) |

## Possible Enhancements

After mastering the basic version, try adding:

- 🚩 **Flag system** - Mark suspected mine locations with 'F'
- 🎨 **Colored output** - Different colors for numbers 1-8
- ⏱️ **Timer** - Track how quickly you solve the puzzle
- 🏆 **High score leaderboard** - Save best times to file
- 🔄 **Replay option** - Play multiple games without restarting
- 🎯 **Difficulty levels** - Beginner, Intermediate, Expert presets
- ⌨️ **Arrow key controls** - Better UI using ncurses
- 💾 **Save/Load game** - Persist game state
- 🛡️ **First-click safe** - Ensure first move never hits mine
- 🤖 **Auto-solver** - AI that solves the puzzle for you

## Advanced Enhancement: Flag System

Add to the class:
```cpp
vector<vector<bool>> flagged;

// In constructor
flagged.resize(ROWS, vector<bool>(COLS, false));

// Modified printBoard
if (flagged[r][c] && !revealed[r][c])
    cout << "F ";
else if (!revealed[r][c])
    cout << "# ";
    
// New method
void toggleFlag(int r, int c) {
    if (!revealed[r][c])
        flagged[r][c] = !flagged[r][c];
}
```

## Building with Makefile

Create a `Makefile`:

```makefile
CXX = g++
CXXFLAGS = -Wall -Wextra -O2
TARGET = minesweeper
SOURCES = minesweeper.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

debug: $(SOURCES)
	$(CXX) -g -o $(TARGET)-debug $(SOURCES)
	gdb $(TARGET)-debug
```

Usage:
```bash
make        # Compile
make run    # Compile and run
make clean  # Remove executable
```

## Learning Path Progression

After Minesweeper, challenge yourself with:

1. **Tic-Tac-Toe** - 2D arrays, win conditions
2. **Hangman** - Strings, file I/O
3. **Snake Game** - Real-time input, ncurses
4. **Minesweeper** (this project) - Recursion, flood fill
5. **Sudoku Solver** - Backtracking algorithm
6. **Chess Game** - Complex piece movement, AI
7. **Raycasting Engine** - 2.5D graphics, trigonometry

## Resources

- [Flood Fill Algorithm](https://en.wikipedia.org/wiki/Flood_fill) - Wikipedia explanation
- [Minesweeper Strategy](https://minesweepergame.com/strategy.php) - Advanced techniques
- [C++ Reference](https://en.cppreference.com/) - Standard library documentation
- [Recursion Tutorial](https://www.geeksforgeeks.org/recursion/) - In-depth guide

## License

Open source for educational purposes.

---

## Author

Created as a C++ programming project demonstrating recursion and flood-fill algorithms on Ubuntu 24.

**Sweep safely! 💣🔍**
```

