# Minesweeper — Software Design

## 1. Overview

The Minesweeper software is designed for the **STM32H743ZI2 MCU** on the **NUCLEO-H743ZI2** development board.

The software architecture separates:

* Hardware
* Drivers
* Middleware
* Game logic
* User interface
* Application control

The main goal is to keep the **Minesweeper game engine independent of the STM32 hardware** as much as possible.

---

# 2. Software Architecture

```text
+------------------------------------------------------+
|                  Application Layer                   |
|                                                      |
|              Minesweeper Application                 |
+------------------------------------------------------+
                         |
                         v
+------------------------------------------------------+
|                    Game Layer                        |
|                                                      |
|  Game Manager | Board | Mine Logic | Flood Fill     |
|  Win/Loss     | Flag  | Score       | Game State    |
+------------------------------------------------------+
                         |
                         v
+------------------------------------------------------+
|                  UI / Input Layer                    |
|                                                      |
| Input Manager | Display Manager | Buzzer Manager     |
+------------------------------------------------------+
                         |
                         v
+------------------------------------------------------+
|                   Driver Layer                       |
|                                                      |
| GPIO | Timer | UART | I2C | SPI | PWM | DMA         |
+------------------------------------------------------+
                         |
                         v
+------------------------------------------------------+
|                  STM32 HAL / LL                      |
+------------------------------------------------------+
                         |
                         v
+------------------------------------------------------+
|                 STM32H743ZI2 MCU                    |
+------------------------------------------------------+
```

---

# 3. Software Layer Responsibilities

```text
+-------------------+--------------------------------------+
| Layer             | Responsibility                        |
+-------------------+--------------------------------------+
| Application       | Overall game control                  |
| Game              | Minesweeper rules and algorithms      |
| UI                | Display and user interaction          |
| Drivers           | Hardware access                       |
| HAL/LL            | STM32 peripheral abstraction          |
| RTOS              | Task scheduling and synchronization   |
+-------------------+--------------------------------------+
```

---

# 4. Recommended Project Structure

```text
Minesweeper/
|
+-- Core/
|   |
|   +-- Inc/
|   |   +-- main.h
|   |   +-- app.h
|   |   +-- board.h
|   |   +-- game.h
|   |   +-- input.h
|   |   +-- display.h
|   |   +-- buzzer.h
|   |   +-- game_timer.h
|   |
|   +-- Src/
|       +-- main.c
|       +-- app.c
|       +-- board.c
|       +-- game.c
|       +-- input.c
|       +-- display.c
|       +-- buzzer.c
|       +-- game_timer.c
|
+-- Drivers/
|   |
|   +-- BSP/
|   |   +-- board.c
|   |   +-- board.h
|   |
|   +-- GPIO/
|   |   +-- gpio_driver.c
|   |   +-- gpio_driver.h
|   |
|   +-- UART/
|   |   +-- uart_driver.c
|   |   +-- uart_driver.h
|   |
|   +-- I2C/
|   |   +-- i2c_driver.c
|   |   +-- i2c_driver.h
|   |
|   +-- SPI/
|   |   +-- spi_driver.c
|   |   +-- spi_driver.h
|   |
|   +-- TIMER/
|   |   +-- timer_driver.c
|   |   +-- timer_driver.h
|   |
|   +-- BUZZER/
|       +-- buzzer_driver.c
|       +-- buzzer_driver.h
|
+-- Middleware/
|   |
|   +-- FreeRTOS/
|   |
|   +-- Display/
|   |
|   +-- Logging/
|
+-- Config/
|   +-- board_config.h
|   +-- game_config.h
|   +-- app_config.h
|
+-- Tests/
|
+-- docs/
|
+-- STM32CubeIDE/
|
+-- CMakeLists.txt
+-- Makefile
+-- README.md
```

---

# 5. Main Software Components

The main software components are:

```text
Application
    |
    +-- Game Manager
    |
    +-- Input Manager
    |
    +-- Display Manager
    |
    +-- Timer Manager
    |
    +-- Audio/Buzzer Manager
    |
    +-- Logger
```

---

# 6. Application Layer

The application layer controls the complete game.

```text
Application
     |
     +-- Initialize system
     |
     +-- Initialize game
     |
     +-- Initialize UI
     |
     +-- Start game
     |
     +-- Process input
     |
     +-- Update game
     |
     +-- Update display
     |
     +-- Handle win/loss
```

Example:

```c
void App_Init(void)
{
    Board_Init();
    Game_Init();
    Input_Init();
    Display_Init();
    Buzzer_Init();
    GameTimer_Init();
}
```

---

# 7. Game Layer

The game layer contains the actual Minesweeper rules.

Responsibilities:

```text
Mine placement
Number calculation
Cell reveal
Flood fill
Flag management
Win detection
Loss detection
Game state
```

The game layer should not directly access GPIO, UART, I2C or SPI.

For example:

```text
GOOD:

Game
 |
 v
Display Manager
 |
 v
Display Driver
 |
 v
I2C
```

Avoid:

```text
Game
 |
 +---- HAL_GPIO
 +---- HAL_I2C
 +---- HAL_UART
```

This separation makes the game easier to test.

---

# 8. Game State Machine

The game should use an explicit state machine.

```text
                    +---------+
                    |  IDLE   |
                    +----+----+
                         |
                    Start Game
                         |
                         v
                  +------+------+
                  |   PLAYING   |
                  +------+------+
                         |
              +----------+----------+
              |                     |
           Mine Hit              All Cells
              |                   Revealed
              v                     |
        +-----+------+              |
        |   GAMEOVER |              |
        +------------+              |
                                      v
                                +-----+-----+
                                |    WON    |
                                +-----------+
```

Recommended states:

```c
typedef enum
{
    GAME_IDLE,
    GAME_PLAYING,
    GAME_WON,
    GAME_LOST
} GameState;
```

---

# 9. Board Representation

The board should be separated from the display representation.

Example:

```c
typedef struct
{
    bool mine;
    bool revealed;
    bool flagged;
    uint8_t adjacent_mines;
} Cell;
```

Then:

```c
#define BOARD_ROWS 9
#define BOARD_COLS 9

Cell board[BOARD_ROWS][BOARD_COLS];
```

This is better than storing the complete game state inside characters.

---

# 10. Board Manager

The board manager provides:

```text
Board_Init()
Board_Reset()
Board_PlaceMines()
Board_CalculateNumbers()
Board_GetCell()
Board_RevealCell()
Board_ToggleFlag()
Board_IsMine()
```

Conceptually:

```text
Board Manager
     |
     +-- Mine placement
     |
     +-- Number calculation
     |
     +-- Cell state
     |
     +-- Boundary checking
```

---

# 11. Mine Placement Algorithm

The game starts by placing mines randomly.

```text
Game_Init()
     |
     v
Generate random position
     |
     v
Is cell already a mine?
     |
   +---+---+
   |       |
  Yes      No
   |       |
   |       v
   |    Place mine
   |       |
   +-------+
       |
       v
   Mine count reached?
```

For a beginner board:

```c
#define BOARD_ROWS 9
#define BOARD_COLS 9
#define MINE_COUNT 10
```

A modern C++ version can use `<random>` if the project is implemented in C++.

---

# 12. Adjacent Mine Calculation

Each cell has up to eight neighbors.

```text
NW   N   NE

 W   C    E

SW   S   SE
```

Direction arrays:

```c
static const int8_t dr[8] =
{
    -1, -1, -1,
     0,  0,
     1,  1,  1
};

static const int8_t dc[8] =
{
    -1,  0,  1,
    -1,  1,
    -1,  0,  1
};
```

The algorithm checks each valid neighbor.

---

# 13. Flood-Fill Algorithm

Flood fill reveals connected empty cells.

```text
Reveal Cell
     |
     v
Is valid?
     |
     v
Already revealed?
     |
     v
Mine?
     |
     v
Reveal
     |
     v
Adjacent mines == 0?
     |
   +---+---+
   |       |
  No      Yes
   |       |
   v       v
 Stop   Reveal neighbors
```

For the original terminal version, recursive flood fill is acceptable.

For a production embedded system, an **iterative queue/stack-based flood fill** is preferable when board size can grow, because it avoids uncontrolled call-stack growth.

---

# 14. Input Manager

The Input Manager converts hardware button events into game commands.

```text
Buttons
   |
   v
GPIO Driver
   |
   v
Input Manager
   |
   v
Game Command
```

Example commands:

```c
typedef enum
{
    INPUT_NONE,
    INPUT_UP,
    INPUT_DOWN,
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_SELECT,
    INPUT_FLAG,
    INPUT_NEW_GAME
} InputCommand;
```

---

# 15. Input Processing

```text
Button Press
     |
     v
GPIO Interrupt
     |
     v
Debounce
     |
     v
Input Manager
     |
     v
Command Queue
     |
     v
Game Task
```

The GPIO ISR should not execute the Minesweeper algorithm.

Instead, it should signal an event or queue a command.

---

# 16. Display Manager

The Display Manager converts game state into display output.

```text
Game State
    |
    v
Display Manager
    |
    v
Display Buffer
    |
    v
OLED/LCD Driver
    |
    v
I2C/SPI
    |
    v
Display
```

The display manager should know:

* Cursor position
* Revealed cells
* Flagged cells
* Mine count
* Timer
* Game status

---

# 17. Display Rendering

Example:

```text
+----------------------+
|    MINESWEEPER       |
|                      |
| # # # # # # # # #    |
| # 1 2 # # # # # #    |
| # 1 0 1 # # # # #    |
| # # # [#] # # # #    |
|                      |
| Mines: 10            |
| Time : 00:42         |
|                      |
| SELECT=OPEN FLAG=F   |
+----------------------+
```

The game engine should provide state; the display layer decides how that state is rendered.

---

# 18. Timer Manager

The Timer Manager maintains the game elapsed time.

```text
Hardware Timer
      |
      v
Timer Driver
      |
      v
Timer Manager
      |
      v
Game Time
```

Example:

```c
typedef struct
{
    uint32_t seconds;
    bool running;
} GameTimer;
```

The timer should not be implemented using blocking delays.

Avoid:

```c
HAL_Delay(1000);
```

inside the main game loop.

Prefer hardware timers or RTOS timing primitives.

---

# 19. Buzzer Manager

The Buzzer Manager provides audio feedback.

```text
Game Event
    |
    v
Buzzer Manager
    |
    v
PWM Driver
    |
    v
Buzzer
```

Events:

```text
BUTTON
FLAG
REVEAL
MINE
WIN
```

---

# 20. Logging System

UART logging should be centralized.

Example:

```c
LOG_INFO("Game started");
LOG_DEBUG("Cursor: %d,%d", row, col);
LOG_WARN("Invalid move");
LOG_ERROR("Display initialization failed");
```

Logging levels:

```text
ERROR
WARN
INFO
DEBUG
TRACE
```

Production builds can disable verbose debug logs.

---

# 21. FreeRTOS Architecture

For an advanced embedded version, FreeRTOS can separate responsibilities into tasks.

Recommended architecture:

```text
                    FreeRTOS
                       |
       +---------------+---------------+
       |               |               |
       v               v               v
  Input Task       Game Task       Display Task
       |               |               |
       |               |               |
       +-------+-------+-------+-------+
               |
               v
          Event Queue
               |
               v
        Minesweeper Engine
```

Additional tasks:

```text
Timer Task
Logger Task
Buzzer Task
```

Do not create tasks simply for every function. Tasks should represent meaningful concurrent activities.

---

# 22. Suggested FreeRTOS Tasks

### Input Task

```text
Priority: Medium
Purpose: Process button events
```

### Game Task

```text
Priority: High
Purpose: Process game commands and update game state
```

### Display Task

```text
Priority: Medium
Purpose: Render game state
```

### Logger Task

```text
Priority: Low
Purpose: Send diagnostic messages
```

### Timer

The timer can use a hardware timer interrupt, RTOS tick facilities, or a dedicated timing mechanism depending on the final design.

---

# 23. Inter-Task Communication

Use FreeRTOS queues/events rather than shared global variables wherever practical.

Example:

```text
Input Task
    |
    | InputCommand
    v
+-----------+
| Game Queue|
+-----------+
    |
    v
Game Task
```

Game-to-display:

```text
Game Task
    |
    | Display Update/Event
    v
Display Queue/Event
    |
    v
Display Task
```

---

# 24. Synchronization

Shared resources may require synchronization.

Possible mechanisms:

```text
Mutex
Semaphore
Queue
Event Group
Task Notification
```

Example:

```text
Game State
    |
    +---- Game Task
    |
    +---- Display Task
```

If both tasks access mutable shared state, use an appropriate synchronization strategy.

An alternative is to send a snapshot of the required state through a queue, reducing shared-state access.

---

# 25. Event-Driven Architecture

The preferred design is event-driven.

```text
                    +-------------+
                    |   Button    |
                    +------+------+
                           |
                           v
                     Input Event
                           |
                           v
                    +------+------+
                    | Game Manager|
                    +------+------+
                           |
             +-------------+-------------+
             |             |             |
             v             v             v
          Display        Timer         Buzzer
           Event         Update         Event
```

Example:

```text
SELECT pressed
      |
      v
REVEAL_CELL
      |
      v
Game Engine
      |
      +---- Cell revealed
      |
      +---- Display update
      |
      +---- Buzzer feedback
      |
      +---- Win/Loss check
```

---

# 26. Game Engine API

A clean API can be defined as:

```c
void Game_Init(void);

void Game_Start(void);

void Game_ProcessInput(InputCommand command);

GameState Game_GetState(void);

const Cell *Game_GetBoard(void);

uint8_t Game_GetRemainingMines(void);

uint32_t Game_GetElapsedTime(void);
```

The UI should use these interfaces instead of accessing internal game variables directly.

---

# 27. Driver API

Example GPIO API:

```c
void GPIO_Init(void);

bool GPIO_ReadButton(ButtonId button);

void GPIO_SetLed(LedId led, bool state);
```

Timer API:

```c
void Timer_Init(void);

void Timer_Start(void);

void Timer_Stop(void);

uint32_t Timer_GetTicks(void);
```

Display API:

```c
bool Display_Init(void);

void Display_Clear(void);

void Display_Render(const GameSnapshot *game);
```

Buzzer API:

```c
void Buzzer_Init(void);

void Buzzer_Beep(uint32_t frequency,
                 uint32_t duration);
```

---

# 28. Data Flow

The complete data flow is:

```text
Physical Button
       |
       v
GPIO
       |
       v
GPIO Driver
       |
       v
Input Manager
       |
       v
Input Command
       |
       v
Game Manager
       |
       v
Minesweeper Engine
       |
       +-------------------+
       |                   |
       v                   v
Game State             Game Event
       |                   |
       v                   +----> Buzzer
Display Manager            |
       |                   +----> Logger
       v
Display Driver
       |
       v
OLED/LCD
```

---

# 29. Error Handling

Hardware failures should be handled explicitly.

Example:

```text
Display_Init()
     |
     v
Success?
   /   \
 Yes    No
 |       |
 v       v
Continue Error Handler
```

Possible error states:

```c
typedef enum
{
    APP_OK,
    APP_ERROR_DISPLAY,
    APP_ERROR_TIMER,
    APP_ERROR_INPUT,
    APP_ERROR_COMMUNICATION
} AppStatus;
```

---

# 30. Assertions

Assertions can detect programming errors during development.

Example:

```c
assert(row < BOARD_ROWS);
assert(col < BOARD_COLS);
```

Assertions should generally be configured differently for debug and release builds.

---

# 31. Memory Management

For an embedded MCU application, prefer deterministic memory usage.

Recommended:

```text
Static allocation
Fixed-size arrays
Static task stacks
Static queues
```

Avoid unnecessary:

```text
malloc()
free()
new
delete
```

during normal gameplay.

For the Minesweeper board, static allocation is sufficient.

---

# 32. C++ Considerations

If the project is implemented in C++:

```text
Application
    |
    +-- Minesweeper class
    +-- Board class
    +-- Input class
    +-- Display class
    +-- Timer class
    +-- Buzzer class
```

Example:

```cpp
class Minesweeper
{
public:
    void init();
    void start();
    void processInput(InputCommand command);
    GameState state() const;

private:
    Board board;
    GameTimer timer;
    GameState gameState;
};
```

Hardware drivers can remain thin wrappers around STM32 HAL/LL functionality.

---

# 33. Unit Testing Strategy

The game engine should be testable without the physical board.

Test:

```text
Mine placement
Number calculation
Boundary checking
Flood fill
Reveal logic
Flag logic
Win detection
Loss detection
Game state transitions
```

Example:

```text
Test Game Engine
       |
       v
No STM32 Hardware Required
       |
       v
Host PC
       |
       v
Unit Tests
```

This is one of the main benefits of separating game logic from hardware drivers.

---

# 34. Debug Build

Development build:

```text
-Wall
-Wextra
-g
-DDEBUG
```

Example:

```bash
g++ -Wall -Wextra -g ...
```

For STM32, configure the equivalent compiler/debug options in the embedded build system.

Debug features:

```text
UART logging
Assertions
Debug LEDs
SWD
Breakpoints
Watchpoints
Memory inspection
```

---

# 35. Release Build

Production build should:

```text
Disable verbose logging
Reduce unnecessary debugging
Enable compiler optimization
Validate memory usage
Validate timing
Validate stack usage
```

Example concept:

```text
DEBUG
 |
 +-- Logs
 +-- Assertions
 +-- Debug instrumentation

RELEASE
 |
 +-- Optimized code
 +-- Minimal logging
 +-- Production configuration
```

---

# 36. Startup Software Flow

The complete startup sequence is:

```text
Reset
  |
  v
Startup Code
  |
  v
SystemInit()
  |
  v
Clock Configuration
  |
  v
HAL_Init()
  |
  v
Peripheral Initialization
  |
  +-- GPIO
  +-- UART
  +-- Timer
  +-- I2C/SPI
  +-- PWM
  |
  v
Application Init
  |
  v
Game Init
  |
  v
Display Init
  |
  v
Input Init
  |
  v
Timer Init
  |
  v
Buzzer Init
  |
  v
Start Scheduler / Main Loop
```

---

# 37. Main Loop Without RTOS

A simple bare-metal architecture can be:

```c
int main(void)
{
    HAL_Init();
    SystemClock_Config();

    Hardware_Init();
    App_Init();

    while (1)
    {
        App_Process();
    }
}
```

Application:

```text
while(1)
 |
 +-- Read input
 |
 +-- Process game
 |
 +-- Update timer
 |
 +-- Update display
 |
 +-- Process buzzer
 |
 +-- Handle events
```

---

# 38. Main Loop With FreeRTOS

```c
int main(void)
{
    HAL_Init();
    SystemClock_Config();

    Hardware_Init();
    App_Init();

    RTOS_Init();

    vTaskStartScheduler();

    while (1)
    {
    }
}
```

After the scheduler starts:

```text
FreeRTOS Scheduler
       |
       +-- Input Task
       |
       +-- Game Task
       |
       +-- Display Task
       |
       +-- Logger Task
       |
       +-- Other timing/event mechanisms
```

---

# 39. Software State Machine Example

```text
                 +------+
                 | IDLE |
                 +--+---+
                    |
               NEW_GAME
                    |
                    v
              +-----+------+
              | INITIALIZE |
              +-----+------+
                    |
                    v
              +-----+------+
              |  PLAYING   |
              +-----+------+
                |       |
          MINE  |       | WIN
                |       |
                v       v
           +----+---+ +--+----+
           |  LOST  | |  WON  |
           +--------+ +-------+
                \       /
                 \     /
                  v   v
                   IDLE
```

---

# 40. Software Safety Principles

The project should follow these embedded-software principles:

1. Validate all external inputs.
2. Avoid buffer overflows.
3. Check array boundaries.
4. Avoid uncontrolled recursion.
5. Keep ISRs short.
6. Avoid blocking operations inside interrupts.
7. Avoid unnecessary dynamic memory allocation.
8. Protect shared resources.
9. Use deterministic timing.
10. Keep hardware dependencies isolated.
11. Use compiler warnings.
12. Use static analysis where possible.
13. Test game logic independently.
14. Monitor stack usage when using RTOS.
15. Handle peripheral initialization failures.

---

# 41. Final Software Architecture

```text
                         APPLICATION
                              |
                              v
                    +-------------------+
                    |   Game Manager    |
                    +---------+---------+
                              |
             +----------------+----------------+
             |                |                |
             v                v                v
        Input Manager    Game Engine      Display Manager
             |                |                |
             |                |                v
             |                |          Display Driver
             |                |                |
             |                |                v
             |                |            I2C/SPI
             |                |
             |                +----> Timer Manager
             |                |
             |                +----> Buzzer Manager
             |                |
             |                +----> Logger
             |
             v
        GPIO Driver
             |
             v
       STM32 HAL / LL
             |
             v
       STM32H743ZI2
```

---

# 42. Design Goal

The final architecture should achieve this separation:

```text
+------------------------------------------------+
|                MINESWEEPER GAME               |
|                                                |
|       Completely independent game logic       |
+------------------------------------------------+
                       |
                       v
+------------------------------------------------+
|              HARDWARE ABSTRACTION              |
|                                                |
| GPIO | Timer | UART | I2C | SPI | PWM | DMA   |
+------------------------------------------------+
                       |
                       v
+------------------------------------------------+
|             STM32H743ZI2 MCU                   |
|                                                |
| ARM Cortex-M7 + Memory + Peripherals           |
+------------------------------------------------+
```

The key design principle is:

> **The Minesweeper engine should know about the game, not about STM32 pins. The driver layer should know about STM32 hardware, not about Minesweeper rules.**

This makes the project easier to debug, unit-test, maintain, and eventually port to another MCU or a PC-based implementation.

