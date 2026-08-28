# Minesweeper STM32H743ZI2 — Debugging Guide

## 1. Overview

This document describes the debugging strategy for the Minesweeper embedded project running on the **NUCLEO-H743ZI2** development board with the **STM32H743ZI2 MCU**.

The debugging approach covers:

* UART debugging
* SWD debugging
* Breakpoints
* Watchpoints
* Register inspection
* Memory inspection
* GPIO debugging
* Timer debugging
* Interrupt debugging
* FreeRTOS debugging
* HardFault debugging
* Stack and memory analysis
* Game-logic debugging

---

# 2. Debugging Architecture

```text
                         Ubuntu PC
                            |
             +--------------+--------------+
             |                             |
             v                             v
        Serial Terminal              Debugger / IDE
             |                             |
            UART                           SWD
             |                             |
             +--------------+--------------+
                            |
                            v
                     NUCLEO-H743ZI2
                            |
                            v
                     STM32H743ZI2
                            |
                    ARM Cortex-M7
                            |
              +-------------+-------------+
              |             |             |
              v             v             v
            Game          Timer         GPIO
            Logic        Interrupts     Buttons
```

---

# 3. Debugging Layers

Debugging should be performed from the highest software layer down to the hardware.

```text
Level 1  Application
           |
           v
Level 2  Game Logic
           |
           v
Level 3  RTOS / Tasks
           |
           v
Level 4  Drivers
           |
           v
Level 5  HAL
           |
           v
Level 6  MCU Registers
           |
           v
Level 7  Hardware
```

Start with the highest-level problem and move downward only when necessary.

---

# 4. UART Debugging

UART is the simplest method for observing program execution.

```text
STM32H743ZI2
      |
      v
    UART
      |
      v
  USB / ST-LINK
      |
      v
 Ubuntu PC
      |
      v
Serial Terminal
```

Example log:

```text
[INFO] System initialization started
[INFO] GPIO initialized
[INFO] UART initialized
[INFO] Timer initialized
[INFO] Minesweeper initialized
[INFO] Mines = 10
[INFO] Game started
```

---

# 5. Debug Log Levels

Use different debug levels instead of printing everything.

```c
typedef enum
{
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG
} LogLevel;
```

Example:

```text
[ERROR] Invalid cell
[WARN ] Mine count is incorrect
[INFO ] New game started
[DEBUG] Reveal cell row=4 col=5
```

Recommended usage:

```text
ERROR → Important failures
WARN  → Unexpected conditions
INFO  → Normal system events
DEBUG → Detailed development information
```

---

# 6. UART Debug API

Create a simple debug interface.

```c
void Debug_Init(void);

void Debug_Error(const char *message);

void Debug_Warn(const char *message);

void Debug_Info(const char *message);

void Debug_Printf(const char *format, ...);
```

Example:

```c
Debug_Info("Minesweeper initialized");

Debug_Printf(
    "Reveal row=%d col=%d\n",
    row,
    col
);
```

---

# 7. Game Logic Debugging

The Minesweeper engine should generate useful diagnostic information.

Example:

```text
[DEBUG] New game
[DEBUG] Mine count = 10
[DEBUG] Reveal cell (4,4)
[DEBUG] Cell value = 0
[DEBUG] Flood fill started
[DEBUG] Flood fill completed
```

For a mine:

```text
[DEBUG] Reveal cell (2,5)
[DEBUG] Cell contains mine
[INFO ] GAME OVER
```

---

# 8. Debugging the Mine Generator

The game uses randomly generated mine positions.

During development, print the mine map.

```text
[DEBUG] Mine map:

0 0 1 0 0 0 0 0 0
0 0 0 0 1 0 0 0 0
1 0 0 0 0 0 1 0 0
...
```

This allows verification that:

* Exactly 10 mines are generated.
* No duplicate mine positions exist.
* Mines remain inside the board.
* Mine generation does not corrupt memory.

Do not expose the mine map in the final user interface.

It is only a debugging feature.

---

# 9. Flood-Fill Debugging

Flood-fill is one of the most important algorithms in the project.

Expected flow:

```text
Reveal Cell
     |
     v
Is cell valid?
     |
     +---- NO ---> Return
     |
    YES
     |
     v
Already revealed?
     |
     +---- YES --> Return
     |
    NO
     |
     v
Reveal cell
     |
     v
Is number != 0?
     |
     +---- YES --> Stop
     |
    NO
     |
     v
Process 8 neighbors
```

Debug example:

```text
[DEBUG] FloodFill(4,4)
[DEBUG] FloodFill(3,3)
[DEBUG] FloodFill(3,4)
[DEBUG] FloodFill(3,5)
[DEBUG] FloodFill(4,3)
```

If the program continuously recurses, check:

* Boundary validation
* Already-revealed condition
* Board coordinates
* Recursion termination

---

# 10. Breakpoints

Breakpoints pause program execution at a specific source line.

Useful breakpoint locations:

```text
Minesweeper_Init()
Minesweeper_NewGame()
Minesweeper_RevealCell()
Minesweeper_FloodFill()
Minesweeper_CheckWin()
Button_IRQHandler()
Timer_IRQHandler()
```

Example:

```c
void Minesweeper_RevealCell(uint8_t row, uint8_t col)
{
    /* BREAKPOINT HERE */

    ...
}
```

When execution stops, inspect:

```text
row
col
board[row][col]
revealed[row][col]
flagged[row][col]
gameState
```

---

# 11. Conditional Breakpoints

Conditional breakpoints are useful when a function is called many times.

For example:

```text
Break when:

row == 4 && col == 4
```

This avoids stopping execution for every cell.

Another useful condition:

```text
gameState == GAME_LOST
```

---

# 12. Watchpoints

Watchpoints stop execution when a variable changes.

Useful variables:

```text
gameState
mineCount
revealedCount
playerRow
playerCol
gameTimer
```

For example:

```text
revealedCount
```

can be monitored to detect unexpected board updates.

---

# 13. Register Debugging

When a peripheral is not working, inspect the STM32 registers.

For GPIO:

```text
GPIOx_MODER
GPIOx_OTYPER
GPIOx_OSPEEDR
GPIOx_PUPDR
GPIOx_IDR
GPIOx_ODR
```

For UART:

```text
USARTx_CR1
USARTx_CR2
USARTx_BRR
USARTx_ISR
USARTx_RDR
USARTx_TDR
```

For timers:

```text
TIMx_CR1
TIMx_DIER
TIMx_SR
TIMx_CNT
TIMx_PSC
TIMx_ARR
```

Register-level debugging helps determine whether the problem is in:

```text
Application
      |
      v
Driver
      |
      v
HAL
      |
      v
Peripheral Configuration
```

---

# 14. GPIO Debugging

For button problems, verify the signal path.

```text
Button
  |
  v
GPIO Pin
  |
  v
GPIO Driver
  |
  v
Input Manager
  |
  v
Game Engine
```

Debug sequence:

```text
1. Check physical button.
2. Check GPIO configuration.
3. Check GPIO input state.
4. Check pull-up/pull-down configuration.
5. Check interrupt configuration.
6. Check button driver.
7. Check input event.
8. Check game response.
```

A GPIO pin can also be toggled as a debug signal.

```c
HAL_GPIO_TogglePin(DEBUG_GPIO_Port, DEBUG_Pin);
```

A logic analyzer or oscilloscope can then measure the signal.

---

# 15. Button Debouncing

Mechanical buttons can generate multiple transitions for one press.

Example:

```text
Actual press:

____|‾‾‾‾‾‾‾‾‾

Electrical signal:

____|‾|_|‾|_|‾‾‾
```

Without debouncing:

```text
One press
   |
   +--> UP
   +--> UP
   +--> UP
```

With debouncing:

```text
One press
   |
   v
One INPUT event
```

Possible solutions:

```text
Software debounce
Hardware debounce
Timer-based debounce
RTOS debounce task
```

---

# 16. Timer Debugging

The game timer should be verified independently.

Expected:

```text
Game Start
    |
    v
Timer = 0
    |
    v
1 second
    |
    v
Timer = 1
    |
    v
2 seconds
    |
    v
Timer = 2
```

Debug output:

```text
[DEBUG] Timer started
[DEBUG] Timer = 1
[DEBUG] Timer = 2
[DEBUG] Timer = 3
```

Check:

```text
Clock frequency
Prescaler
Auto-reload value
Timer interrupt
Interrupt enable
Timer callback
Counter value
```

---

# 17. Interrupt Debugging

Interrupt handlers should be short.

Recommended:

```c
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    /* Capture event */
    input_event = INPUT_SELECT;

    /* Notify application */
}
```

Avoid doing this inside an ISR:

```text
Flood fill
Display rendering
Long loops
UART blocking operations
Large memory operations
```

Preferred architecture:

```text
GPIO Interrupt
      |
      v
Capture Event
      |
      v
Queue / Flag
      |
      v
Game Task
      |
      v
Process Event
```

---

# 18. Interrupt Debugging Checklist

If a button interrupt does not work:

```text
1. Is GPIO configured correctly?
2. Is EXTI configured?
3. Is the interrupt enabled?
4. Is the NVIC enabled?
5. Is the correct IRQ handler used?
6. Is the callback executed?
7. Is the GPIO pin correct?
8. Is debounce handled?
9. Is the event sent to the application?
```

---

# 19. HardFault Debugging

A HardFault can occur because of:

* Invalid memory access
* Stack overflow
* Invalid function pointer
* Corrupted stack
* Incorrect peripheral access
* Buffer overflow
* RTOS misuse
* Invalid interrupt handling

Typical flow:

```text
Application
    |
    v
Unexpected fault
    |
    v
HardFault_Handler()
    |
    v
Cortex-M7 Fault Registers
    |
    v
Find faulting instruction
    |
    v
Identify root cause
```

---

# 20. HardFault Handler

During development, collect fault information.

```c
void HardFault_Handler(void)
{
    Debug_Error("HardFault occurred");

    while (1)
    {
    }
}
```

A more advanced implementation should capture:

```text
R0
R1
R2
R3
R12
LR
PC
xPSR
```

The most important value is usually:

```text
PC
```

because it identifies the program counter at the time of the exception.

---

# 21. Stack Debugging

The Cortex-M7 uses a stack for:

```text
Function calls
Local variables
Return addresses
Interrupt context
RTOS task context
```

A stack overflow can cause unpredictable behavior.

Symptoms:

```text
Random crash
HardFault
Corrupted variables
Incorrect game state
Unexpected return address
```

Avoid large local arrays.

Bad:

```c
void GameTask(void)
{
    uint8_t huge_buffer[10000];
}
```

Prefer static/global storage or appropriately managed memory.

---

# 22. Memory Debugging

Monitor:

```text
Flash usage
SRAM usage
Stack usage
Heap usage
Global variables
RTOS task stacks
```

Important areas:

```text
Flash
 |
 +-- .text
 +-- .rodata

RAM
 |
 +-- .data
 +-- .bss
 +-- heap
 +-- stack
```

For the Minesweeper board, prefer predictable static allocation.

---

# 23. Buffer Overflow Debugging

Check all array accesses.

For:

```c
uint8_t board[9][9];
```

valid indices are:

```text
row = 0..8
col = 0..8
```

Never access:

```c
board[9][0];
board[0][9];
board[-1][0];
```

Always validate:

```c
bool isValid(uint8_t row, uint8_t col)
{
    return (row < BOARD_ROWS &&
            col < BOARD_COLS);
}
```

---

# 24. FreeRTOS Debugging

When FreeRTOS is introduced, debugging becomes more complex.

Architecture:

```text
                  FreeRTOS
                     |
       +-------------+-------------+
       |             |             |
       v             v             v
 InputTask       GameTask      DisplayTask
       |             |             |
       +---- Queue --+             |
                     |             |
                     +---- Queue --+
```

Monitor:

```text
Task state
Task priority
Stack usage
Queue usage
Semaphore state
CPU utilization
Task starvation
Deadlocks
```

---

# 25. FreeRTOS Task States

A task can be:

```text
RUNNING
READY
BLOCKED
SUSPENDED
```

Example:

```text
InputTask
   |
   v
BLOCKED
   |
Button event
   |
   v
READY
   |
Scheduler
   |
   v
RUNNING
```

If a task never runs, investigate:

```text
Priority
Blocking call
Queue state
Semaphore state
Interrupt configuration
Scheduler state
```

---

# 26. Queue Debugging

Input events can be passed through a queue.

```text
Button ISR
    |
    v
Input Queue
    |
    v
InputTask
    |
    v
GameTask
```

Example events:

```text
INPUT_UP
INPUT_DOWN
INPUT_LEFT
INPUT_RIGHT
INPUT_SELECT
INPUT_FLAG
```

Debug:

```text
[DEBUG] Queue event: INPUT_UP
[DEBUG] Queue event: INPUT_SELECT
[DEBUG] Queue event: INPUT_FLAG
```

If events disappear, inspect:

```text
Queue size
Queue full condition
Producer
Consumer
Interrupt-to-task synchronization
```

---

# 27. OLED/LCD Debugging

If the display is blank:

```text
Application
     |
     v
Display Manager
     |
     v
OLED Driver
     |
     v
I2C/SPI
     |
     v
Display
```

Debug in this order:

```text
1. Power
2. Ground
3. Wiring
4. I2C/SPI configuration
5. Device address
6. Peripheral clock
7. Driver initialization
8. Communication
9. Display initialization sequence
10. Rendering function
```

For I2C, verify:

```text
SCL
SDA
Device address
Pull-up resistors
Bus speed
ACK
```

---

# 28. UART Debugging Problems

If no UART output appears:

```text
Check:

1. Baud rate
2. TX pin
3. RX pin
4. UART instance
5. GPIO alternate function
6. UART clock
7. Terminal configuration
8. USB/ST-LINK connection
```

Example terminal configuration:

```text
Baud Rate : 115200
Data Bits : 8
Parity    : None
Stop Bits : 1
Flow Ctrl : None
```

---

# 29. Buzzer Debugging

If the buzzer does not work:

```text
Game Event
    |
    v
Sound Manager
    |
    v
Buzzer Driver
    |
    v
PWM
    |
    v
GPIO
    |
    v
Buzzer
```

Check:

```text
PWM frequency
Duty cycle
Timer configuration
GPIO alternate function
Buzzer polarity
Power
Ground
```

---

# 30. SWD Debugging

The STM32H743ZI2 can be debugged using the SWD interface.

```text
Ubuntu PC
    |
    v
ST-LINK
    |
    v
SWD
    |
    v
STM32H743ZI2
```

SWD provides access to:

```text
Breakpoints
Watchpoints
CPU registers
Memory
Peripheral registers
Program execution
```

---

# 31. Recommended Debugging Workflow

When a feature fails, follow this sequence:

```text
             Problem
                |
                v
        Reproduce the issue
                |
                v
          Add UART logs
                |
                v
       Identify failing layer
                |
                v
        Set breakpoint
                |
                v
       Inspect variables
                |
                v
       Inspect registers
                |
                v
       Check hardware signals
                |
                v
          Find root cause
                |
                v
             Fix
                |
                v
             Test
                |
                v
          Regression Test
```

---

# 32. Example: Button Does Not Move Cursor

Problem:

```text
Press RIGHT
    |
    v
Cursor doesn't move
```

Debug:

```text
Step 1
Check physical button
        |
        v
Step 2
Check GPIO input
        |
        v
Step 3
Check interrupt
        |
        v
Step 4
Check InputEvent
        |
        v
Step 5
Check InputTask
        |
        v
Step 6
Check GameTask
        |
        v
Step 7
Check cursor position
        |
        v
Step 8
Check DisplayTask
```

Possible UART output:

```text
[DEBUG] Button RIGHT detected
[DEBUG] Input event = RIGHT
[DEBUG] Game event received
[DEBUG] Cursor = (4,5)
[DEBUG] Display update
```

The last successful message identifies the failing layer.

---

# 33. Example: Game Crashes During Flood Fill

Debug sequence:

```text
[DEBUG] Reveal (4,4)
[DEBUG] FloodFill started
[DEBUG] FloodFill (3,3)
[DEBUG] FloodFill (3,4)
[DEBUG] FloodFill (3,5)
...
[ERROR] HardFault
```

Check:

```text
1. Board boundary
2. Already-revealed condition
3. Recursive termination
4. Stack usage
5. Array indices
6. Board memory corruption
```

For larger boards, consider an iterative flood-fill implementation instead of deep recursion.

---

# 34. Debug Build

Maintain a dedicated debug configuration.

Recommended compiler options:

```text
-g
-O0
-Wall
-Wextra
```

Example:

```bash
arm-none-eabi-gcc -g -O0 -Wall -Wextra
```

Optimization should normally be reduced during source-level debugging.

For release:

```text
-O2
```

can be enabled after functionality is verified.

---

# 35. Assertions

Use assertions to detect invalid conditions early.

Example:

```c
assert(row < BOARD_ROWS);
assert(col < BOARD_COLS);
```

Useful conditions:

```text
Board dimensions
Mine count
Game state
Array indices
Pointer validity
RTOS object validity
```

Assertions should provide enough information to identify the failure.

---

# 36. Debugging Best Practices

1. Reproduce the problem consistently.
2. Identify which software layer is failing.
3. Use UART logs for execution flow.
4. Use breakpoints for detailed inspection.
5. Use watchpoints for unexpected memory changes.
6. Inspect peripheral registers when drivers fail.
7. Keep ISRs short.
8. Avoid blocking operations inside interrupts.
9. Validate all array boundaries.
10. Monitor stack usage.
11. Check RTOS queues and task states.
12. Use assertions during development.
13. Keep debug code separate from game logic.
14. Test game logic independently on the host.
15. Fix the root cause rather than hiding the symptom.

---

# 37. Debugging Checklist

### System

```text
[ ] MCU starts correctly
[ ] Clock initialized
[ ] GPIO initialized
[ ] UART initialized
[ ] Timer initialized
[ ] Display initialized
```

### Game

```text
[ ] Board initialized
[ ] Mines generated
[ ] Mine count correct
[ ] Numbers calculated correctly
[ ] Cell reveal works
[ ] Flood-fill works
[ ] Flags work
[ ] Win detection works
[ ] Loss detection works
```

### Input

```text
[ ] Button detected
[ ] Debouncing works
[ ] Input event generated
[ ] Input event received
[ ] Cursor moves correctly
```

### Display

```text
[ ] Display powers up
[ ] I2C/SPI communication works
[ ] Board renders
[ ] Numbers render
[ ] Flags render
[ ] Game-over screen works
[ ] Win screen works
```

### Timer

```text
[ ] Timer starts
[ ] Timer interrupt occurs
[ ] Counter increments
[ ] Timer stops on game end
[ ] Display shows correct time
```

### RTOS

```text
[ ] Tasks start
[ ] Queues initialized
[ ] Task priorities correct
[ ] No deadlocks
[ ] No starvation
[ ] Stack usage acceptable
```

---

# 38. Final Debugging Strategy

The recommended debugging strategy for this project is:

```text
                    MINESWEEPER
                         |
                         v
                  Game Logic Debug
                         |
                         v
                    UART Logs
                         |
                         v
                   SWD Debugger
                         |
             +-----------+-----------+
             |                       |
             v                       v
        CPU/Memory               Peripherals
             |                       |
             v                       v
       Registers/Stack       GPIO/UART/I2C/SPI
             |                       |
             +-----------+-----------+
                         |
                         v
                  Hardware Signals
                         |
                         v
                  Logic Analyzer
                         |
                         v
                    Root Cause
```

The objective is to develop the ability to debug from **application level → driver level → MCU register level → physical hardware level**.

This debugging methodology is directly applicable beyond Minesweeper to real embedded systems such as automotive ECUs, industrial controllers, IoT devices, robotics, and RTOS-based products.

