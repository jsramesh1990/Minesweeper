#  Minesweeper — STM32H743ZI2 

![Language](https://img.shields.io/badge/Language-C-blue)
![MCU](https://img.shields.io/badge/MCU-STM32H743ZI2-blue)
![Core](https://img.shields.io/badge/Core-ARM%20Cortex--M7-orange)
![Board](https://img.shields.io/badge/Board-NUCLEO--H743ZI2-green)
![RTOS](https://img.shields.io/badge/RTOS-FreeRTOS-red)
![HAL](https://img.shields.io/badge/STM32-HAL-lightgrey)
![Build](https://img.shields.io/badge/Build-CMake%20%7C%20Make-informational)
![Testing](https://img.shields.io/badge/Testing-Unit%20Tests-success)
![Platform](https://img.shields.io/badge/Platform-Embedded%20C-lightblue)
![License](https://img.shields.io/badge/License-MIT-yellow)

A hardware-based **Minesweeper game implemented in Embedded C for the STM32H743ZI2 MCU**, running on the **NUCLEO-H743ZI2 development board**.

The project demonstrates practical embedded-software concepts including **ARM Cortex-M7 programming, STM32 HAL, FreeRTOS multitasking, GPIO, buttons, OLED display, UART debugging, timers, buzzer control, state machines, flood-fill recursion, unit testing, CMake, and Makefile-based builds**.

---

##  Project Overview

This project converts the traditional Minesweeper game into an embedded system.

The player interacts with the game using physical buttons. The STM32H743ZI2 processes the game logic, maintains the game state, updates the display, manages the game timer, and generates audio feedback through a buzzer.

```text
                    NUCLEO-H743ZI2
                           │
                           ▼
                    STM32H743ZI2
                    ARM Cortex-M7
                           │
          ┌────────────────┼────────────────┐
          │                │                │
          ▼                ▼                ▼
       Buttons           OLED             Buzzer
          │                │                │
          ▼                │                ▼
     Input Driver          │          Audio Feedback
          │                │
          ▼                │
    Input Manager          │
          │                │
          └───────┬────────┘
                  ▼
             Game State
                  │
                  ▼
            Minesweeper
             Game Logic
                  │
       ┌──────────┼──────────┐
       ▼          ▼          ▼
    Mines      Flood Fill   Win/Loss
       │          │          │
       └──────────┼──────────┘
                  ▼
             Game Display
```

---

#  Features

*  9 × 9 Minesweeper board
*  Configurable mine count
*  Adjacent mine calculation
*  Recursive flood-fill algorithm
*  Physical button-based navigation
*  Mine flag support
*  OLED/LCD game display
*  Buzzer feedback
*  Game timer
*  Finite State Machine
*  FreeRTOS task architecture
*  FreeRTOS queues
*  FreeRTOS mutex synchronization
*  UART debugging
*  Host-based unit testing
*  CMake build
*  Makefile build
*  STM32 HAL abstraction
*  CMSIS support
*  Hardware abstraction through BSP
*  Modular software architecture

---

#  Embedded Concepts Demonstrated

This project is designed as more than a game. It demonstrates real embedded software development practices.

| Area             | Concepts                        |
| ---------------- | ------------------------------- |
| MCU              | STM32H743ZI2                    |
| CPU              | ARM Cortex-M7                   |
| Programming      | Embedded C                      |
| RTOS             | FreeRTOS                        |
| Drivers          | GPIO, UART, Timer, OLED, Buzzer |
| Communication    | I2C / SPI / UART                |
| Algorithms       | Flood Fill, Mine Detection      |
| Architecture     | Layered Architecture            |
| State Management | Finite State Machine            |
| Concurrency      | Tasks                           |
| IPC              | Queues                          |
| Synchronization  | Mutex                           |
| Debugging        | UART / Debugger                 |
| Testing          | Host-based Unit Tests           |
| Build            | CMake / Make                    |
| Toolchain        | GCC / ARM GCC                   |
| Version Control  | Git                             |

---

#  Hardware

## NUCLEO-H743ZI2

The NUCLEO-H743ZI2 is the development board used for this project.

It contains the:

**STM32H743ZI2 MCU**

The MCU is based on an:

**ARM Cortex-M7 processor core**

### MCU Architecture

```text
                    STM32H743ZI2
                         │
                 ARM Cortex-M7
                         │
          ┌──────────────┼──────────────┐
          │              │              │
         CPU           Memory        Peripherals
          │              │              │
          │        ┌─────┴─────┐       │
          │        │           │       │
          │      Flash        SRAM     │
          │                         ┌───┼────┐
          │                         │   │    │
          │                        GPIO UART Timer
          │                         │
          │                    I2C / SPI
          │                         │
          ▼                         ▼
       Game Logic               External HW
```

---

#  Hardware Block Diagram

```text
                    ┌──────────────────────┐
                    │   NUCLEO-H743ZI2     │
                    │                      │
                    │   STM32H743ZI2       │
                    │   Cortex-M7          │
                    └──────────┬───────────┘
                               │
             ┌─────────────────┼──────────────────┐
             │                 │                  │
             ▼                 ▼                  ▼
         GPIO/Button        I2C/SPI             Timer
             │                 │                  │
             ▼                 ▼                  ▼
        User Input          OLED Display       Buzzer
             │                 │
             └────────┬────────┘
                      ▼
                 Minesweeper
                   Engine
                      │
              ┌───────┼────────┐
              ▼       ▼        ▼
             Mine   Flood    Game
            Logic    Fill    State
```

---

#  Hardware Components

| Component    | Interface      | Purpose                |
| ------------ | -------------- | ---------------------- |
| STM32H743ZI2 | MCU            | Main controller        |
| Push Buttons | GPIO           | User input             |
| OLED         | I2C/SPI        | Game display           |
| Buzzer       | Timer/PWM/GPIO | Sound feedback         |
| UART         | USART          | Debug logging          |
| Timer        | TIM / SysTick  | Game timing            |
| ST-LINK      | SWD            | Flashing and debugging |

> Exact GPIO pins, timer instance, UART instance, and display controller should be defined according to the final hardware wiring and CubeMX configuration.

---

#  Software Architecture

The project uses a layered architecture.

```text
┌─────────────────────────────────────────────┐
│                 APPLICATION                 │
│                                             │
│ Minesweeper │ Game State │ Input │ Display │
└──────────────────────┬──────────────────────┘
                       │
┌──────────────────────▼──────────────────────┐
│                    RTOS                     │
│                                             │
│ Tasks │ Queues │ Mutexes                    │
└──────────────────────┬──────────────────────┘
                       │
┌──────────────────────▼──────────────────────┐
│              PROJECT DRIVERS                │
│                                             │
│ GPIO │ Button │ OLED │ Buzzer │ UART │ Timer│
└──────────────────────┬──────────────────────┘
                       │
┌──────────────────────▼──────────────────────┐
│                STM32 HAL                    │
│                                             │
│ GPIO │ UART │ TIM │ I2C │ SPI │ RCC        │
└──────────────────────┬──────────────────────┘
                       │
┌──────────────────────▼──────────────────────┐
│                  CMSIS                     │
│                                             │
│ ARM Cortex-M7 / STM32 Device Definitions   │
└──────────────────────┬──────────────────────┘
                       │
┌──────────────────────▼──────────────────────┐
│              STM32H743ZI2 MCU               │
└─────────────────────────────────────────────┘
```

---


#  System Working Flow

The overall game execution follows:

```text
Power ON
   │
   ▼
STM32 Reset
   │
   ▼
Startup Code
   │
   ▼
System Clock Configuration
   │
   ▼
HAL Initialization
   │
   ▼
GPIO / UART / Timer / I2C / SPI
   │
   ▼
FreeRTOS Initialization
   │
   ▼
Create Tasks
   │
   ├───────────────┐
   │               │
   ▼               ▼
Input Task       Game Task
   │               │
   │               ▼
   │          Game State Machine
   │               │
   │               ▼
   │          Minesweeper Logic
   │               │
   │               ▼
   └──────────► Queue
                   │
                   ▼
              Display Task
                   │
                   ▼
                  OLED
```

---

#  Boot Flow

```text
Power ON
   │
   ▼
Reset
   │
   ▼
Boot / Startup
   │
   ▼
Vector Table
   │
   ▼
Reset_Handler
   │
   ▼
SystemInit()
   │
   ▼
C Runtime Initialization
   │
   ▼
main()
   │
   ▼
HAL_Init()
   │
   ▼
Clock Configuration
   │
   ▼
Peripheral Initialization
   │
   ▼
FreeRTOS Kernel
   │
   ▼
Scheduler
   │
   ▼
Application Tasks
```

---

#  FreeRTOS Architecture

The application uses multiple RTOS tasks.

```text
                    FreeRTOS
                       │
       ┌───────────────┼────────────────┐
       │               │                │
       ▼               ▼                ▼
 Input Task        Game Task       Display Task
       │               │                │
       ▼               ▼                ▼
 Button Driver     Game Logic       OLED Driver
       │               │                │
       └───────┐       │       ┌────────┘
               ▼       ▼       ▼
             ┌────────────────────┐
             │    Event Queue     │
             └────────────────────┘

                       │
                       ▼
                  Timer Task
                       │
                       ▼
                  Game Timer
```

---

#  Queue Flow

The Input Task does not directly modify the game state.

Instead:

```text
Physical Button
      │
      ▼
Button Driver
      │
      ▼
Input Manager
      │
      ▼
Game Event
      │
      ▼
FreeRTOS Queue
      │
      ▼
Game Task
      │
      ▼
State Machine
      │
      ▼
Game Logic
```

Example events:

```text
GAME_EVENT_UP
GAME_EVENT_DOWN
GAME_EVENT_LEFT
GAME_EVENT_RIGHT
GAME_EVENT_SELECT
GAME_EVENT_FLAG
GAME_EVENT_START
```

This provides clean task-to-task communication.

---

#  Mutex Flow

Shared hardware resources can be protected using FreeRTOS mutexes.

```text
Task A
  │
  ▼
Take Mutex
  │
  ▼
Access Shared Resource
  │
  ▼
Release Mutex
  │
  ▼
Task B
  │
  ▼
Take Mutex
  │
  ▼
Access Shared Resource
  │
  ▼
Release Mutex
```

This prevents simultaneous access to shared resources such as communication interfaces.

---

#  Minesweeper Game Flow

```text
                    GAME START
                        │
                        ▼
                 Initialize Board
                        │
                        ▼
                 Place Mines
                        │
                        ▼
             Calculate Mine Numbers
                        │
                        ▼
                  WAIT FOR INPUT
                        │
                        ▼
                 Read Button Event
                        │
                        ▼
              ┌─────────┴─────────┐
              │                   │
           Movement             Action
              │                   │
              ▼                   ▼
       Update Cursor        Reveal / Flag
                                  │
                         ┌────────┴────────┐
                         │                 │
                      Mine Hit          Safe Cell
                         │                 │
                         ▼                 ▼
                    GAME OVER         Check Number
                                           │
                                           ▼
                                      Flood Fill
                                           │
                                           ▼
                                      Check Win
                                           │
                              ┌────────────┴──────────┐
                              │                       │
                           WIN                     CONTINUE
                              │                       │
                              ▼                       └──────► WAIT
                         GAME COMPLETE
```

---

#  Minesweeper Algorithm

## Mine Placement

The board contains a configurable number of mines.

```text
Generate random position
        │
        ▼
Is position already occupied?
        │
    ┌───┴───┐
   YES      NO
    │        │
    │        ▼
    │     Place Mine
    │        │
    └────────┘
         │
         ▼
   Required count reached?
         │
         ├── NO ──► Generate next position
         │
         ▼
        DONE
```

---

#  Adjacent Mine Calculation

Each cell checks its eight neighboring positions.

```text
        ┌─────┬─────┬─────┐
        │ -1  │  0  │ +1  │
        ├─────┼─────┼─────┤
        │ -1  │  X  │ +1  │
        ├─────┼─────┼─────┤
        │ -1  │  0  │ +1  │
        └─────┴─────┴─────┘

             8 neighbors
```

Direction offsets:

```c
dr = {-1,-1,-1,0,0,1,1,1};
dc = {-1,0,1,-1,1,-1,0,1};
```

For every valid neighbor:

```text
Is it a mine?
   │
   ├── YES → count++
   │
   └── NO  → continue
```

---

#  Flood-Fill Algorithm

When the player selects a cell containing zero adjacent mines:

```text
Reveal Cell
    │
    ▼
Is cell valid?
    │
    ├── NO → Return
    │
    ▼
Already revealed?
    │
    ├── YES → Return
    │
    ▼
Reveal Cell
    │
    ▼
Is cell numbered?
    │
    ├── YES → Return
    │
    ▼
Recursively process
8 neighboring cells
```

This is implemented using recursion.

---

#  State Machine

The game is modeled using states such as:

```text
              ┌──────────────┐
              │    INIT      │
              └──────┬───────┘
                     ▼
              ┌──────────────┐
              │    READY     │
              └──────┬───────┘
                     ▼
              ┌──────────────┐
              │    PLAYING   │◄────────┐
              └──────┬───────┘         │
                     │                 │
            ┌────────┼────────┐        │
            ▼        ▼        ▼        │
          SAFE      MINE     WIN       │
            │        │        │        │
            │        ▼        ▼        │
            │      GAME      GAME      │
            │      OVER     COMPLETE   │
            │                           │
            └───────────────────────────┘
```

---

#  Display Flow

```text
Game State
    │
    ▼
Game Display Manager
    │
    ▼
Generate Display Data
    │
    ▼
OLED Driver
    │
    ▼
I2C / SPI
    │
    ▼
OLED Controller
    │
    ▼
Player
```

---

#  Buzzer Flow

```text
Game Event
    │
    ├── Mine Hit ───────► Long / Error Tone
    │
    ├── Safe Reveal ────► Short Tone
    │
    ├── Flag ───────────► Confirmation Tone
    │
    └── Win ────────────► Success Melody
```

The buzzer can be driven using a timer/PWM peripheral.

---

#  UART Debugging

UART provides runtime diagnostics.

Example:

```text
[INFO] System initialization
[INFO] FreeRTOS started
[INFO] Game initialized
[INFO] Mines placed: 10
[INFO] Cursor: row=4 col=5
[INFO] Cell revealed
[INFO] Adjacent mines: 2
[INFO] Timer: 00:32
[INFO] Game state: PLAYING
```

Typical debugging flow:

```text
STM32H743ZI2
      │
      ▼
UART Driver
      │
      ▼
USB / ST-LINK Virtual COM
      │
      ▼
PC Terminal
      │
      ▼
Runtime Logs
```

---

#  Unit Testing

The game logic is designed to be testable independently from hardware.

```text
                Game Logic
                    │
       ┌────────────┼────────────┐
       ▼            ▼            ▼
 Game Logic      Flood Fill     Mines
    Tests           Tests       Tests
       │            │            │
       └────────────┼────────────┘
                    ▼
                 Ubuntu
                    │
                    ▼
             GCC / CMake
```

Tests include:

```text
test_game_logic.c
    ├── Initialization
    ├── Coordinate validation
    └── Game state behavior

test_flood_fill.c
    ├── Empty area reveal
    ├── Boundary handling
    └── Recursive expansion

test_mines.c
    ├── Mine count
    ├── Mine placement
    └── Adjacent mine calculation
```

---

#  Build Flow

## Host Unit-Test Build

```text
Source Code
    │
    ▼
GCC
    │
    ▼
CMake / Make
    │
    ▼
Test Executables
    │
    ▼
Run Tests
```

Build using CMake:

```bash
mkdir build
cd build
cmake ..
make
ctest --output-on-failure
```

---

#  Makefile Build

Build:

```bash
make
```

Run tests:

```bash
make test
```

Clean:

```bash
make clean
```

Debug build:

```bash
make debug
```

---

#  STM32 Firmware Build Flow

For the actual MCU firmware:

```text
C / H Source Files
        │
        ▼
Preprocessor
        │
        ▼
ARM GCC Compiler
        │
        ▼
Object Files (.o)
        │
        ▼
Linker
        │
        ▼
ELF
        │
        ▼
HEX / BIN
        │
        ▼
ST-LINK / SWD
        │
        ▼
STM32H743ZI2 Flash
```

---

#  Toolchain

Recommended development environment:

```text
OS:
Linux / Ubuntu

Compiler:
arm-none-eabi-gcc

Host Compiler:
gcc

Build:
CMake
Make

IDE:
STM32CubeIDE / VS Code

Configuration:
STM32CubeMX

Debug:
GDB / ST-LINK

RTOS:
FreeRTOS

Version Control:
Git
```

---

#  Debugging Strategy

The project supports multiple debugging levels.

### Level 1 — Compile-Time

```text
-Wall
-Wextra
-Werror
```

### Level 2 — Unit Testing

```text
Game Logic
Flood Fill
Mine Calculation
```

### Level 3 — UART Logging

```text
Game State
Input Events
Timer
Mine Detection
```

### Level 4 — Source-Level Debugging

```text
GDB
   │
   ▼
ST-LINK
   │
   ▼
STM32H743ZI2
```

### Level 5 — Hardware Debugging

```text
GPIO
I2C
SPI
UART
Timer/PWM
```

---

#  Memory and Runtime Considerations

The Minesweeper board is intentionally small, making it suitable for an MCU environment.

For example:

```text
9 × 9 = 81 cells
```

Each cell can contain information such as:

```text
Mine status
Revealed status
Flag status
Adjacent mine count
```

The application therefore requires relatively little memory compared with large embedded applications.

The project also demonstrates an important embedded concept:

> Avoid unnecessary dynamic memory allocation in real-time firmware.

Static or preallocated data structures are preferred for predictable memory usage.

---

#  Software Layer Responsibilities

| Layer       | Responsibility                          |
| ----------- | --------------------------------------- |
| Core        | Startup, interrupts, MCU initialization |
| CMSIS       | Cortex-M7 / MCU definitions             |
| HAL         | STM32 peripheral abstraction            |
| BSP         | Board-specific configuration            |
| Drivers     | Hardware peripheral interfaces          |
| RTOS        | Tasks, scheduling, queues, mutexes      |
| Input       | Converts hardware input into events     |
| Game        | Game state and state machine            |
| Minesweeper | Core game algorithms                    |
| Display     | Converts game state to UI               |
| Tests       | Host-based verification                 |
| Docs        | Architecture and design documentation   |

---

#  End-to-End Runtime Flow

The complete runtime sequence is:

```text
                POWER ON
                   │
                   ▼
              MCU RESET
                   │
                   ▼
             STARTUP CODE
                   │
                   ▼
             SYSTEM CLOCK
                   │
                   ▼
            HAL INITIALIZATION
                   │
                   ▼
          PERIPHERAL INITIALIZATION
                   │
        ┌──────────┼──────────┐
        ▼          ▼          ▼
      GPIO        UART       TIMER
        │          │          │
        └──────────┼──────────┘
                   ▼
             FreeRTOS Init
                   │
                   ▼
             Create Tasks
                   │
                   ▼
              Scheduler
                   │
                   ▼
             INPUT TASK
                   │
                   ▼
            Button Driver
                   │
                   ▼
             Input Event
                   │
                   ▼
               Queue
                   │
                   ▼
              GAME TASK
                   │
                   ▼
           State Machine
                   │
                   ▼
          Minesweeper Logic
                   │
          ┌────────┼─────────┐
          ▼        ▼         ▼
        Reveal    Flag      Move
          │
          ▼
      Flood Fill
          │
          ▼
       Win Check
          │
          ▼
      Game State
          │
          ▼
       DISPLAY TASK
          │
          ▼
       OLED DRIVER
          │
          ▼
          OLED
```

---

#  Development Workflow

```text
Requirement
    │
    ▼
Architecture
    │
    ▼
Hardware Design
    │
    ▼
Driver Development
    │
    ▼
Game Logic
    │
    ▼
RTOS Integration
    │
    ▼
Unit Testing
    │
    ▼
Firmware Build
    │
    ▼
Flash MCU
    │
    ▼
Hardware Testing
    │
    ▼
UART/GDB Debugging
    │
    ▼
Optimization
    │
    ▼
Git Commit
    │
    ▼
GitHub
```

---

#  Git Workflow

Recommended workflow:

```text
main
 │
 ├── feature/game-logic
 │
 ├── feature/display
 │
 ├── feature/input
 │
 ├── feature/freertos
 │
 └── feature/unit-tests
```

Typical commands:

```bash
git status

git add .

git commit -m "Add Minesweeper game logic"

git push origin main
```

---

#  Example Git Commit Structure

```text
feat: add minesweeper game engine
feat: add flood fill algorithm
feat: add button driver
feat: add OLED driver
feat: add FreeRTOS task architecture
test: add mine calculation tests
test: add flood fill tests
fix: correct board boundary handling
debug: add UART game state logging
docs: add software architecture
```

---

#  Future Enhancements

Planned improvements:

* [ ] First-click safe mechanism
* [ ] Difficulty selection
* [ ] Beginner / Intermediate / Expert modes
* [ ] Improved OLED graphics
* [ ] Mine flag counter
* [ ] Game pause/resume
* [ ] Persistent high scores
* [ ] EEPROM/Flash high-score storage
* [ ] DMA-based UART
* [ ] DMA-based display updates
* [ ] Interrupt-driven button input
* [ ] Button debounce using timer
* [ ] Low-power mode
* [ ] Watchdog integration
* [ ] Fault handling
* [ ] Static analysis
* [ ] CI/CD using GitHub Actions
* [ ] Code coverage
* [ ] Hardware-in-the-loop testing

---

#  What This Project Demonstrates

This project is intentionally structured to demonstrate skills expected from an embedded software engineer:

```text
                    EMBEDDED SOFTWARE
                           │
       ┌───────────────────┼───────────────────┐
       │                   │                   │
       ▼                   ▼                   ▼
   C Programming        RTOS                Drivers
       │                   │                   │
       ▼                   ▼                   ▼
 Algorithms             Tasks              GPIO/UART
 Recursion              Queues             I2C/SPI
 Data Structures        Mutex              Timer/PWM
       │                   │                   │
       └───────────────────┼───────────────────┘
                           │
                           ▼
                     SYSTEM DESIGN
                           │
              ┌────────────┼────────────┐
              ▼            ▼            ▼
          Debugging      Testing      Build
              │            │            │
             GDB        Unit Tests   CMake/Make
              │            │            │
              └────────────┼────────────┘
                           ▼
                    STM32H743ZI2
```

---

#  Documentation

Detailed documentation is maintained under:

```text
docs/
├── architecture.md
├── hardware.md
├── software_design.md
├── state_machine.md
└── debugging.md
```

Recommended reading order:

```text
1. architecture.md
2. hardware.md
3. software_design.md
4. state_machine.md
5. debugging.md
```

---

#  Verification Strategy

The project follows a layered verification approach:

```text
                    Verification
                         │
        ┌────────────────┼────────────────┐
        │                │                │
        ▼                ▼                ▼
    Unit Tests       Integration       Hardware
        │               Tests            Tests
        ▼                │                │
   Game Logic            ▼                ▼
   Flood Fill       RTOS + App       MCU + HW
   Mine Logic       Driver Tests     OLED/Button
        │                │                │
        └────────────────┼────────────────┘
                         ▼
                    Final System
```

---

#  Project Status

**Status:** 🚧 Active Development

Current focus:

* STM32H743ZI2 platform
* Minesweeper game engine
* FreeRTOS architecture
* Hardware driver layer
* OLED display
* Button input
* UART debugging
* Unit testing

---

#  Project Goal

The goal of this project is not only to implement Minesweeper, but to demonstrate how a **real embedded software application is architected from hardware drivers through RTOS and application logic to testing and debugging**.

```text
Hardware
   ↓
BSP
   ↓
HAL / CMSIS
   ↓
Drivers
   ↓
FreeRTOS
   ↓
Application
   ↓
Minesweeper
   ↓
Testing
   ↓
Debugging
   ↓
Production-ready Embedded Software
```

**Build it. Flash it. Debug it. Understand every layer.**

>  **Embedded C + STM32H743ZI2 + Cortex-M7 + FreeRTOS + Drivers + Algorithms + Testing**
