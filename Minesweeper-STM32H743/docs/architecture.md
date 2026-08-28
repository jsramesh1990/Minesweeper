# Minesweeper STM32H743ZI2 — Software Architecture

## 1. Overview

This project is an embedded version of the classic Minesweeper game running on the **NUCLEO-H743ZI2 development board**, based on the **STM32H743ZI2 microcontroller** with an ARM Cortex-M7 processor core.

The project is designed to demonstrate practical embedded-software concepts including:

* Embedded C/C++
* ARM Cortex-M7
* GPIO
* Interrupts
* Hardware timers
* UART debugging
* I2C/SPI communication
* Display drivers
* Buzzer/PWM
* Finite State Machine (FSM)
* Memory management
* FreeRTOS
* Hardware abstraction
* Unit testing

---

# 2. Hardware Architecture

```text
                         NUCLEO-H743ZI2
                                |
                                v
                       STM32H743ZI2 MCU
                                |
                         ARM Cortex-M7
                                |
             +------------------+------------------+
             |                  |                  |
            GPIO              TIMER              UART
             |                  |                  |
             v                  v                  v
          Buttons          Game Timer          Debug PC
             |
             v
      Input Controller
             |
             v
       Minesweeper Engine
             |
       +-----+------+
       |            |
       v            v
     OLED          Buzzer
   I2C / SPI       PWM/GPIO
```

---

# 3. Software Architecture

The software is divided into multiple layers.

```text
+--------------------------------------------------+
|                 APPLICATION                      |
|                                                  |
|  Minesweeper Engine | Game State | Input Logic  |
+--------------------------------------------------+
                       |
                       v
+--------------------------------------------------+
|                  SERVICES                        |
|                                                  |
|  Game Timer | Display Manager | Input Manager   |
|  Sound Manager | Debug Manager                  |
+--------------------------------------------------+
                       |
                       v
+--------------------------------------------------+
|                  DRIVERS                         |
|                                                  |
| GPIO | Button | UART | Timer | OLED | Buzzer    |
+--------------------------------------------------+
                       |
                       v
+--------------------------------------------------+
|                    BSP                           |
|                                                  |
| Board configuration | Pin configuration          |
| Clock configuration | Peripheral configuration  |
+--------------------------------------------------+
                       |
                       v
+--------------------------------------------------+
|              STM32 HAL / CMSIS                   |
+--------------------------------------------------+
                       |
                       v
+--------------------------------------------------+
|              STM32H743ZI2 Hardware                |
+--------------------------------------------------+
```

---

# 4. Application Layer

The application layer contains the actual Minesweeper functionality.

```text
app/
└── minesweeper/
    ├── minesweeper.c
    ├── minesweeper.h
    ├── game_logic.c
    └── game_logic.h
```

Responsibilities:

* Initialize a new game
* Generate mines
* Calculate adjacent mine counts
* Reveal cells
* Flag cells
* Execute flood-fill
* Detect win
* Detect loss
* Maintain game state

The game engine should not directly access STM32 hardware.

For example, game logic should not directly call:

```c
HAL_GPIO_WritePin();
HAL_UART_Transmit();
HAL_I2C_Master_Transmit();
```

Instead, it should communicate through application/service interfaces.

---

# 5. Minesweeper Game Engine

The game engine is the core of the project.

```text
                 Game Engine
                     |
       +-------------+-------------+
       |             |             |
       v             v             v
   Mine Logic    Reveal Logic   Win/Loss
       |             |
       v             v
 Calculate        Flood Fill
 Numbers
```

Main operations:

```c
Minesweeper_Init();

Minesweeper_NewGame();

Minesweeper_RevealCell(row, col);

Minesweeper_ToggleFlag(row, col);

Minesweeper_FloodFill(row, col);

Minesweeper_CheckWin();

Minesweeper_CheckLoss();
```

---

# 6. Game Board

The embedded version uses fixed-size memory instead of dynamically allocated containers.

Example:

```c
#define BOARD_ROWS 9
#define BOARD_COLS 9
#define TOTAL_MINES 10

typedef struct
{
    uint8_t mines[BOARD_ROWS][BOARD_COLS];
    uint8_t numbers[BOARD_ROWS][BOARD_COLS];
    uint8_t revealed[BOARD_ROWS][BOARD_COLS];
    uint8_t flagged[BOARD_ROWS][BOARD_COLS];

} MineBoard;
```

The board is stored in MCU RAM.

```text
STM32 SRAM
|
+-- mines[][]
+-- numbers[][]
+-- revealed[][]
+-- flagged[][]
+-- game state
+-- stack
+-- RTOS objects
```

---

# 7. Game State Machine

The game uses a finite state machine.

```text
                    +---------+
                    |  INIT   |
                    +----+----+
                         |
                         v
                  +------+------+
                  |   RUNNING   |
                  +------+------+
                         |
              +----------+----------+
              |                     |
              v                     v
         +---------+           +---------+
         |   WON   |           |  LOST   |
         +---------+           +---------+
              |                     |
              +----------+----------+
                         |
                         v
                    NEW GAME
```

Example states:

```c
typedef enum
{
    GAME_INIT,
    GAME_RUNNING,
    GAME_PAUSED,
    GAME_WON,
    GAME_LOST
} GameState;
```

---

# 8. Input Architecture

The user interacts with Minesweeper using physical buttons.

```text
Physical Button
      |
      v
     GPIO
      |
      v
Button Driver
      |
      v
Input Manager
      |
      v
Game Engine
```

Supported inputs:

```text
UP
DOWN
LEFT
RIGHT
SELECT
FLAG
```

Example:

```c
typedef enum
{
    INPUT_UP,
    INPUT_DOWN,
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_SELECT,
    INPUT_FLAG
} InputEvent;
```

The game engine does not need to know which GPIO pin generated the event.

---

# 9. Display Architecture

The game display is separated from the game engine.

```text
Game Engine
     |
     v
Game Display Manager
     |
     v
OLED Driver
     |
     v
I2C / SPI
     |
     v
OLED / LCD
```

The display manager can provide:

```c
GameDisplay_DrawBoard();

GameDisplay_ShowCursor();

GameDisplay_ShowTimer();

GameDisplay_ShowMines();

GameDisplay_ShowGameOver();

GameDisplay_ShowWin();
```

This makes it possible to replace the display without modifying the Minesweeper algorithm.

---

# 10. Timer Architecture

The game timer uses the STM32 hardware timer.

```text
STM32 Hardware Timer
          |
          v
      Timer ISR
          |
          v
     Game Timer
          |
          v
       Display
```

Example:

```text
Game starts
     |
     v
Timer = 0
     |
     v
1 second interrupt
     |
     v
Timer++
     |
     v
Display
```

The timer can be used to measure the player's completion time.

---

# 11. UART Debug Architecture

UART provides a debugging interface between the STM32 and the development PC.

```text
STM32H743ZI2
      |
     UART
      |
      v
ST-LINK / USB
      |
      v
Ubuntu PC
      |
      v
Serial Terminal
```

Example debug output:

```text
[INFO] Minesweeper initialized
[INFO] Mines = 10
[INFO] Player position = 4,4
[INFO] Cell value = 0
[INFO] Flood fill started
[INFO] Flood fill completed
[INFO] Timer = 32 seconds
```

UART should primarily be used for debugging and diagnostics.

---

# 12. Buzzer Architecture

The buzzer is controlled through GPIO or PWM.

```text
Game Engine
     |
     v
Sound Manager
     |
     v
Buzzer Driver
     |
     v
PWM / GPIO
     |
     v
Buzzer
```

Example events:

```text
Normal move → Short beep

Flag mine   → Double beep

Mine hit    → Long beep

Game won    → Success melody
```

---

# 13. Driver Layer

The driver layer contains hardware-specific functionality.

```text
drivers/
|
+-- gpio/
+-- button/
+-- uart/
+-- timer/
+-- display/
+-- buzzer/
```

Responsibilities:

### GPIO Driver

Controls digital input/output.

```text
GPIO
 |
+-- Button input
+-- Buzzer output
+-- LEDs
```

### UART Driver

Provides serial communication.

```text
Application
     |
UART Driver
     |
STM32 USART
```

### Display Driver

Controls OLED/LCD communication.

```text
Display Driver
      |
   I2C/SPI
      |
    OLED
```

### Timer Driver

Provides hardware timing services.

```text
Timer Driver
      |
STM32 Timer Peripheral
```

---

# 14. BSP — Board Support Package

The BSP isolates the actual NUCLEO-H743ZI2 board configuration.

```text
bsp/
|
+-- board.c
+-- board.h
+-- clock_config.c
+-- clock_config.h
```

The BSP is responsible for:

* Board initialization
* Clock configuration
* GPIO pin mapping
* Peripheral initialization
* Board-specific settings

The application should not contain board-specific pin numbers.

---

# 15. Hardware Abstraction

The project follows this principle:

```text
Application
     |
     v
Hardware-independent interface
     |
     v
Driver
     |
     v
STM32 HAL
     |
     v
Hardware
```

This allows the Minesweeper game engine to be reused on another STM32 board with minimal changes.

---

# 16. FreeRTOS Architecture

FreeRTOS can be introduced after the basic bare-metal version is working.

Recommended tasks:

```text
                  FreeRTOS
                     |
       +-------------+-------------+
       |             |             |
       v             v             v
 InputTask       GameTask      DisplayTask
       |             |             |
       v             v             v
    Buttons      Game Engine       OLED
                     |
                     v
                 TimerTask
```

Communication:

```text
Button
  |
  v
GPIO Interrupt
  |
  v
Queue
  |
  v
InputTask
  |
  v
GameTask
  |
  v
DisplayTask
```

Possible FreeRTOS objects:

```text
Tasks
Queues
Mutexes
Semaphores
Software Timers
```

---

# 17. Interrupt Architecture

Interrupts can be used for buttons and timers.

```text
                 STM32
                   |
          +--------+--------+
          |                 |
       GPIO IRQ          Timer IRQ
          |                 |
          v                 v
     Button ISR         Timer ISR
          |                 |
          v                 v
     Input Event        Game Timer
```

Interrupt service routines should remain short.

Heavy processing such as flood-fill should execute in the appropriate application task rather than inside the ISR.

---

# 18. Memory Architecture

The STM32H743ZI2 contains internal memory used by the application.

Conceptually:

```text
STM32 Memory
|
+-- Flash
|    |
|    +-- Program code
|    +-- Constants
|
+-- SRAM
     |
     +-- Game board
     +-- Variables
     +-- Stack
     +-- RTOS objects
     +-- Buffers
```

The project should avoid unnecessary dynamic memory allocation.

Fixed-size board storage provides predictable memory usage.

---

# 19. Testing Architecture

Game logic should be testable independently from hardware.

```text
             Game Engine
                 |
        +--------+--------+
        |                 |
        v                 v
    Host Test          STM32 Test
        |                 |
      Ubuntu           NUCLEO
```

Possible tests:

```text
test_mine_generation()
test_number_calculation()
test_flood_fill()
test_reveal_cell()
test_flag_cell()
test_win_condition()
test_loss_condition()
```

This separation allows the core algorithm to be tested on a PC before running it on the MCU.

---

# 20. Debugging Flow

Recommended debugging flow:

```text
Source Code
    |
    v
Compile
    |
    v
Flash STM32
    |
    v
Run Application
    |
    +---------> UART Debug
    |
    +---------> SWD Debugger
    |
    +---------> GPIO Observation
    |
    +---------> Logic Analyzer
```

For difficult problems:

```text
Software Issue
      |
      v
UART Logs
      |
      v
Debugger
      |
      v
Breakpoints
      |
      v
Register Inspection
      |
      v
Memory Inspection
```

---

# 21. Complete System Architecture

The final system can be represented as:

```text
                         NUCLEO-H743ZI2
                                |
                                v
                        STM32H743ZI2 MCU
                                |
                         ARM Cortex-M7
                                |
                    +-----------+-----------+
                    |                       |
              APPLICATION                 RTOS
                    |                       |
                    v                       v
             Minesweeper Engine       Tasks / Queues
                    |
          +---------+---------+
          |         |         |
          v         v         v
       Input     Game FSM   Display
       Manager             Manager
          |         |         |
          v         v         v
       Button     Game     OLED/LCD
       Driver    Logic     Driver
          |         |         |
          +---------+---------+
                    |
                    v
                DRIVERS
                    |
       +------------+-------------+
       |            |             |
      GPIO         UART         Timer
       |            |             |
    Buttons         PC          Game Time
       
                    +
                  PWM
                    |
                  Buzzer
```

---

# 22. Design Principles

The project follows these principles:

1. Keep game logic independent from hardware.
2. Keep hardware access inside drivers.
3. Keep board-specific configuration inside the BSP.
4. Keep interrupt handlers short.
5. Prefer static memory for predictable behavior.
6. Use queues for asynchronous input/events.
7. Separate display logic from game logic.
8. Make the game engine testable on a host PC.
9. Use UART for diagnostics.
10. Introduce FreeRTOS only after the bare-metal version is stable.

---

# 23. Development Roadmap

```text
Phase 1
  |
  +-- Port Minesweeper game engine to STM32
  |
Phase 2
  |
  +-- UART debugging
  |
Phase 3
  |
  +-- GPIO buttons
  |
Phase 4
  |
  +-- OLED/LCD display
  |
Phase 5
  |
  +-- Hardware timer
  |
Phase 6
  |
  +-- Interrupts
  |
Phase 7
  |
  +-- Buzzer + PWM
  |
Phase 8
  |
  +-- FreeRTOS
  |
Phase 9
  |
  +-- Unit testing
  |
Phase 10
  |
  +-- Optimization + documentation
```

---

# 24. Final Objective

The final project should demonstrate the complete embedded-software flow:

```text
C/C++ Application
       |
       v
Minesweeper Game Engine
       |
       v
Game State Machine
       |
       v
FreeRTOS / Bare Metal
       |
       v
Drivers
       |
       v
STM32 HAL
       |
       v
STM32H743ZI2
       |
       v
Hardware
```

The goal is not simply to make Minesweeper run on an STM32.

The goal is to demonstrate a **professional embedded-software architecture** using a real ARM Cortex-M7 microcontroller, hardware peripherals, interrupts, drivers, RTOS concepts, debugging, testing, and hardware abstraction.

