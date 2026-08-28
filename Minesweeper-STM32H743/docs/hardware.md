# Minesweeper STM32H743ZI2 — Hardware Architecture

## 1. Overview

The Minesweeper project is implemented on the **NUCLEO-H743ZI2 development board**, which uses the **STM32H743ZI2 microcontroller**.

The STM32H743ZI2 is a **32-bit microcontroller (MCU)** based on the **ARM Cortex-M7 processor core**.

The hardware is responsible for:

* Executing the Minesweeper application
* Reading user input
* Driving the game display
* Maintaining the game timer
* Providing UART debugging
* Generating buzzer output
* Handling interrupts
* Running the optional FreeRTOS environment

---

# 2. Hardware Block Diagram

```text
                         NUCLEO-H743ZI2
                                |
                                v
                       STM32H743ZI2 MCU
                                |
                        ARM Cortex-M7
                                |
       +------------------------+------------------------+
       |                        |                        |
       v                        v                        v
      GPIO                    TIMER                    UART
       |                        |                        |
       v                        v                        v
    Buttons                 Game Timer               Debug PC
       |
       v
 Input Controller
       |
       v
 Minesweeper Engine
       |
       +----------------------+----------------+
       |                      |                |
       v                      v                v
    OLED/LCD                Buzzer           LEDs
    I2C/SPI                  PWM             GPIO
```

---

# 3. Main Hardware Components

```text
+----------------------+----------------------------------+
| Component            | Purpose                          |
+----------------------+----------------------------------+
| NUCLEO-H743ZI2       | Main development board           |
| STM32H743ZI2         | Main microcontroller             |
| Cortex-M7            | Executes application code        |
| GPIO                 | Button and LED control           |
| Timer                | Game timing and PWM              |
| UART                 | Debug communication              |
| I2C/SPI              | Display communication            |
| PWM                  | Buzzer control                   |
| OLED/LCD             | Game display                     |
| Push Buttons         | User input                       |
| Buzzer               | Audio feedback                   |
| LEDs                 | Status/debug indication          |
+----------------------+----------------------------------+
```

---

# 4. STM32H743ZI2 MCU

The STM32H743ZI2 is the central processing device.

```text
STM32H743ZI2
|
+-- ARM Cortex-M7 CPU
|
+-- Flash Memory
|
+-- SRAM
|
+-- GPIO
|
+-- Timers
|
+-- UART / USART
|
+-- SPI
|
+-- I2C
|
+-- ADC
|
+-- DMA
|
+-- Interrupt Controller
|
+-- Other peripherals
```

The Cortex-M7 executes:

```text
Boot Code
    |
    v
System Initialization
    |
    v
Application
    |
    v
Minesweeper Engine
```

---

# 5. CPU Architecture

The STM32H743ZI2 uses an ARM Cortex-M7 processor core.

```text
                 Cortex-M7
                    |
        +-----------+-----------+
        |           |           |
        v           v           v
      CPU        Registers     FPU
        |
        v
   Instruction
    Execution
        |
        v
   STM32 Peripherals
```

The CPU executes the Minesweeper game logic while peripherals handle external hardware.

---

# 6. Memory Architecture

The Minesweeper application uses MCU memory for code, variables, game state, stack, and buffers.

Conceptually:

```text
STM32H743ZI2 Memory
|
+----------------------------+
| Flash                      |
|                            |
| .text                      |
| Program Code               |
|                            |
| .rodata                    |
| Constants                  |
+----------------------------+
|
+----------------------------+
| SRAM                       |
|                            |
| .data                      |
| .bss                       |
| Game Board                 |
| Game State                 |
| Buffers                    |
| RTOS Objects               |
| Stack                      |
+----------------------------+
```

The game board can use fixed-size arrays:

```c
#define BOARD_ROWS 9
#define BOARD_COLS 9

uint8_t mines[BOARD_ROWS][BOARD_COLS];
uint8_t numbers[BOARD_ROWS][BOARD_COLS];
uint8_t revealed[BOARD_ROWS][BOARD_COLS];
uint8_t flagged[BOARD_ROWS][BOARD_COLS];
```

This provides predictable memory usage.

---

# 7. GPIO Hardware

GPIO is used for digital input and output.

```text
GPIO
 |
 +-- Push Buttons
 |
 +-- LEDs
 |
 +-- Buzzer Control
 |
 +-- Debug Signals
```

Button flow:

```text
Push Button
     |
     v
GPIO Input
     |
     v
Interrupt / Polling
     |
     v
Button Driver
     |
     v
Input Manager
     |
     v
Minesweeper
```

---

# 8. Button Interface

Recommended controls:

```text
+----------------+
|     UP         |
+----------------+
| LEFT | SELECT  | RIGHT
+----------------+
|    DOWN        |
+----------------+

Additional:

FLAG
NEW GAME
```

Possible game mapping:

```text
UP       → Move cursor up
DOWN     → Move cursor down
LEFT     → Move cursor left
RIGHT    → Move cursor right
SELECT   → Reveal cell
FLAG     → Flag/unflag cell
```

The buttons can be implemented using GPIO input pins.

---

# 9. Button Debouncing

Mechanical switches can produce multiple electrical transitions.

```text
Ideal:

_____|‾‾‾‾‾‾‾‾

Actual:

_____|‾|_|‾|_|‾‾‾
```

Therefore, software debouncing should be implemented.

Possible approach:

```text
GPIO Interrupt
      |
      v
Debounce Timer
      |
      v
Validate Button
      |
      v
Generate Input Event
```

---

# 10. Display Interface

An external OLED or LCD can be used as the game display.

Recommended communication interfaces:

```text
STM32
  |
  +---- I2C ----> OLED
  |
  +---- SPI -----> OLED / LCD
```

I2C is suitable for many small OLED displays.

SPI can provide higher throughput for displays requiring faster updates.

---

# 11. Display Data Flow

```text
Minesweeper Engine
        |
        v
Game Display Manager
        |
        v
OLED Driver
        |
        v
I2C / SPI Driver
        |
        v
STM32 Peripheral
        |
        v
OLED / LCD
```

The display should show:

```text
+---------------------------+
|      MINESWEEPER          |
|                           |
|  # # # # # # # # #        |
|  # # 1 1 # # # # #        |
|  # 1 0 0 1 # # # #        |
|  # # # ^ # # # # #        |
|                           |
|  Mines: 10                |
|  Time : 00:32             |
+---------------------------+
```

---

# 12. Game Cursor

A cursor is useful when using physical buttons.

Example:

```text
# # # # # # #
# # # # # # #
# # # [#] # #
# # # # # # #
# # # # # # #
```

The cursor position is maintained in RAM:

```c
uint8_t cursor_row;
uint8_t cursor_col;
```

The display manager renders the cursor.

---

# 13. UART Debug Interface

UART is used to communicate with the development PC.

```text
STM32H743ZI2
      |
      v
UART / USART
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

Typical debug settings:

```text
Baud Rate : 115200
Data Bits : 8
Parity    : None
Stop Bits : 1
Flow Ctrl : None
```

Example output:

```text
[INFO] System initialized
[INFO] Minesweeper initialized
[INFO] Mines = 10
[INFO] Game started
[DEBUG] Cursor = (4,4)
[DEBUG] Cell value = 0
```

---

# 14. Timer Hardware

The STM32 hardware timers are used for the Minesweeper game timer.

```text
STM32 Clock
     |
     v
Timer Peripheral
     |
     v
Timer Counter
     |
     v
Interrupt
     |
     v
Game Timer
```

Example:

```text
Start Game
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

The timer stops when:

```text
GAME_WON
```

or:

```text
GAME_LOST
```

---

# 15. PWM and Buzzer

The buzzer can be controlled using PWM.

```text
STM32 Timer
     |
     v
PWM Output
     |
     v
Buzzer
```

Different events can generate different sounds.

```text
Button Press
    ↓
Short Beep

Flag
    ↓
Double Beep

Mine
    ↓
Long Beep

Win
    ↓
Success Melody
```

PWM parameters include:

```text
Frequency
Duty Cycle
Duration
```

---

# 16. LEDs

The NUCLEO board LEDs can be used for status indication.

Example:

```text
LED OFF
    |
    v
Game Running

LED BLINK
    |
    v
Game Active

LED ON
    |
    v
Game Won

LED FAST BLINK
    |
    v
Game Lost
```

LEDs are also useful for debugging hardware initialization.

---

# 17. Interrupt Architecture

Interrupts can be generated by:

```text
GPIO
Timer
UART
DMA
```

Example button interrupt:

```text
Button Press
     |
     v
GPIO Edge
     |
     v
EXTI
     |
     v
NVIC
     |
     v
ISR
     |
     v
Input Event
```

The interrupt handler should perform minimal work.

---

# 18. NVIC

The Nested Vectored Interrupt Controller manages Cortex-M7 interrupts.

Conceptually:

```text
GPIO IRQ
     |
Timer IRQ
     |
UART IRQ
     |
DMA IRQ
     |
     v
    NVIC
     |
     v
Cortex-M7
```

Interrupt priorities should be configured according to system requirements.

For example:

```text
Higher Priority
      |
      +-- Critical hardware event
      |
      +-- Timer
      |
      +-- GPIO
      |
      +-- UART
      |
Lower Priority
```

Actual priorities should be selected according to the final application architecture.

---

# 19. DMA

DMA can be used to transfer data without requiring the CPU to handle every byte.

Possible applications:

```text
UART DMA
    |
    v
Debug data

SPI DMA
    |
    v
Display update

ADC DMA
    |
    v
Sensor data
```

DMA is not mandatory for the basic Minesweeper version, but it is a useful advanced enhancement.

---

# 20. Clock Architecture

The STM32 clock system provides clocks to:

```text
Cortex-M7
AHB
APB
GPIO
Timers
UART
I2C
SPI
```

Conceptually:

```text
Clock Source
     |
     v
PLL / Clock Tree
     |
     +--------> CPU
     |
     +--------> AHB
     |
     +--------> APB
                  |
          +-------+-------+
          |       |       |
         UART    Timer   I2C/SPI
```

Correct clock configuration is important for:

* CPU operation
* UART baud rate
* Timer accuracy
* I2C/SPI communication
* PWM frequency

---

# 21. Power Architecture

The NUCLEO-H743ZI2 provides the development-board power and programming/debug infrastructure.

Conceptually:

```text
USB / External Power
        |
        v
NUCLEO-H743ZI2
        |
        v
Power Regulation
        |
        v
STM32H743ZI2
        |
        +---- GPIO
        +---- Display
        +---- Buttons
        +---- Buzzer
```

External peripherals should be powered according to their voltage and current requirements.

Do not connect a peripheral directly without checking:

```text
Voltage
Current
GPIO capability
Signal levels
Ground reference
```

---

# 22. ST-LINK Debugger

The NUCLEO board includes ST-LINK functionality for programming and debugging.

```text
Ubuntu PC
    |
    v
USB
    |
    v
ST-LINK
    |
    +------ SWD ------> STM32H743ZI2
    |
    +------ Debug/Programming
```

ST-LINK can be used for:

```text
Flash programming
Debugging
Breakpoints
Watchpoints
Memory inspection
Register inspection
CPU control
```

---

# 23. SWD Interface

The Serial Wire Debug interface provides low-level access to the Cortex-M7.

```text
Debugger
    |
    v
SWD
    |
    v
Cortex-M7
    |
    +-- CPU Registers
    +-- Memory
    +-- Peripherals
```

SWD is particularly useful for:

* HardFault debugging
* Register inspection
* Memory inspection
* Step-by-step execution
* Breakpoints

---

# 24. Hardware Communication Interfaces

The project can use several STM32 communication peripherals.

```text
+--------+-------------------------+
| UART   | Debug / Serial Console  |
+--------+-------------------------+
| I2C    | OLED / Sensors         |
+--------+-------------------------+
| SPI    | Fast Display / Devices |
+--------+-------------------------+
| GPIO   | Buttons / LEDs          |
+--------+-------------------------+
| PWM    | Buzzer                 |
+--------+-------------------------+
```

---

# 25. Recommended Hardware Connections

A practical configuration is:

```text
NUCLEO-H743ZI2
|
+-- GPIO
|    |
|    +-- UP Button
|    +-- DOWN Button
|    +-- LEFT Button
|    +-- RIGHT Button
|    +-- SELECT Button
|    +-- FLAG Button
|
+-- I2C
|    |
|    +-- OLED
|
+-- Timer/PWM
|    |
|    +-- Buzzer
|
+-- UART
|    |
|    +-- Debug PC
|
+-- GPIO
     |
     +-- Status LEDs
```

The exact GPIO pins should be defined centrally in the BSP and `.ioc` configuration rather than hard-coded throughout the application.

---

# 26. Hardware Abstraction

The application should not directly depend on physical pin numbers.

Bad architecture:

```c
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
```

inside game logic.

Preferred:

```c
Buzzer_On();
```

The driver handles:

```text
Buzzer_On()
    |
    v
GPIO/PWM Driver
    |
    v
Actual STM32 Pin
```

This keeps the application portable.

---

# 27. Hardware Initialization Sequence

Recommended startup sequence:

```text
Reset
  |
  v
Startup Code
  |
  v
System Clock
  |
  v
HAL Initialization
  |
  v
GPIO Initialization
  |
  v
UART Initialization
  |
  v
Timer Initialization
  |
  v
I2C/SPI Initialization
  |
  v
Display Initialization
  |
  v
Buzzer Initialization
  |
  v
Minesweeper Initialization
  |
  v
Start Application
```

With FreeRTOS:

```text
Hardware Initialization
        |
        v
FreeRTOS Initialization
        |
        v
Create Tasks
        |
        v
Create Queues
        |
        v
Start Scheduler
        |
        v
Application Tasks
```

---

# 28. Hardware Failure Troubleshooting

### Board does not start

Check:

```text
USB/power
ST-LINK connection
Clock configuration
Reset line
Flash programming
Debugger connection
```

### Button does not work

Check:

```text
Power
Ground
GPIO configuration
Pull-up/pull-down
Pin mapping
EXTI configuration
Debouncing
```

### OLED does not work

Check:

```text
Power
Ground
SCL
SDA
I2C address
I2C configuration
Clock
Driver initialization
```

### UART does not work

Check:

```text
TX
RX
GND
UART instance
Baud rate
GPIO alternate function
Clock
Terminal configuration
```

### Buzzer does not work

Check:

```text
PWM timer
GPIO alternate function
Frequency
Duty cycle
Power
Ground
Buzzer polarity
```

---

# 29. Hardware Debugging Tools

Useful tools include:

```text
+----------------------+------------------------------+
| Tool                 | Purpose                      |
+----------------------+------------------------------+
| ST-LINK              | Programming/debugging        |
| Serial Terminal      | UART debugging               |
| Multimeter           | Voltage/continuity checks    |
| Oscilloscope         | Signal/timing analysis       |
| Logic Analyzer       | Digital protocol analysis    |
| SWD Debugger         | CPU/register debugging       |
+----------------------+------------------------------+
```

---

# 30. Hardware Development Phases

Build the hardware incrementally.

```text
Phase 1
STM32 + ST-LINK
      |
      v
Verify flashing/debugging

Phase 2
STM32 + UART
      |
      v
Verify debug output

Phase 3
STM32 + Buttons
      |
      v
Verify GPIO/input

Phase 4
STM32 + OLED/LCD
      |
      v
Verify display

Phase 5
STM32 + Timer
      |
      v
Verify game timer

Phase 6
STM32 + Buzzer
      |
      v
Verify PWM/audio

Phase 7
Complete Hardware
      |
      v
Minesweeper
```

---

# 31. Final Hardware Architecture

```text
                         Ubuntu PC
                            |
                     USB / ST-LINK
                            |
                 +----------+----------+
                 |                     |
                SWD                   UART
                 |                     |
                 +----------+----------+
                            |
                            v
                    NUCLEO-H743ZI2
                            |
                            v
                    STM32H743ZI2 MCU
                            |
                     ARM Cortex-M7
                            |
        +-------------------+-------------------+
        |                   |                   |
       GPIO                TIMER               UART
        |                   |                   |
        v                   v                   v
     Buttons            Game Timer           Debug PC
        |
        v
   Input Manager
        |
        v
 Minesweeper Engine
        |
        +------------------+------------------+
        |                  |                  |
        v                  v                  v
      I2C/SPI             PWM               GPIO
        |                  |                  |
        v                  v                  v
     OLED/LCD            Buzzer             LEDs
```

---

# 32. Hardware Design Principles

The project follows these principles:

1. Keep hardware access inside drivers.
2. Keep board-specific configuration inside the BSP.
3. Keep GPIO pin mappings centralized.
4. Use interrupts for asynchronous hardware events.
5. Keep interrupt handlers short.
6. Use timers for accurate game timing.
7. Use UART for diagnostics.
8. Use I2C/SPI for external displays.
9. Use PWM for buzzer control.
10. Use SWD for low-level debugging.
11. Validate voltage and current requirements for external peripherals.
12. Avoid hard-coding hardware details in game logic.
13. Build and test hardware incrementally.
14. Keep the game engine independent from the physical hardware.

---

# 33. Project Hardware Objective

The final hardware system should demonstrate:

```text
                   User
                    |
                    v
                 Buttons
                    |
                    v
                  GPIO
                    |
                    v
              STM32H743ZI2
                    |
          +---------+---------+
          |         |         |
          v         v         v
        Game      Timer     Display
        Logic       |          |
          |         |          |
          v         v          v
       Game FSM   Counter    OLED/LCD
          |
          +-------> Buzzer
          |
          +-------> LEDs
          |
          +-------> UART
                         |
                         v
                      Debug PC
```

The hardware architecture is designed to demonstrate a complete embedded system based on an **ARM Cortex-M7 MCU**, from physical input and peripherals through drivers and application logic to debugging and real-time operation.

