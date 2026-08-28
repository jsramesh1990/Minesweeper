# Minesweeper — State Machine Design

## 1. Purpose

This document defines the state-machine architecture for the Minesweeper embedded application running on the **NUCLEO-H743ZI2 / STM32H743ZI2**.

The state machine controls the complete game lifecycle:

```text
Power On
   |
   v
Initialization
   |
   v
Idle
   |
   v
New Game
   |
   v
Playing
   |
   +----------+
   |          |
   v          v
Game Won   Game Lost
   |          |
   +-----+----+
         |
         v
       Idle
```

The state machine prevents game logic from becoming a collection of unrelated `if/else` conditions.

---

# 2. High-Level State Machine

```text
                         +----------------+
                         |    POWER_ON    |
                         +-------+--------+
                                 |
                                 v
                         +-------+--------+
                         | INITIALIZATION |
                         +-------+--------+
                                 |
                         Initialization OK
                                 |
                                 v
                         +-------+--------+
                         |      IDLE      |
                         +-------+--------+
                                 |
                              START
                                 |
                                 v
                         +-------+--------+
                         |   NEW_GAME    |
                         +-------+--------+
                                 |
                         Board Ready
                                 |
                                 v
                         +-------+--------+
                         |    PLAYING    |
                         +-------+--------+
                           |           |
                    MINE HIT           | ALL SAFE
                           |           |
                           v           v
                    +------+---+   +---+------+
                    | GAME_LOST|   | GAME_WON |
                    +------+---+   +---+------+
                           |           |
                           +-----+-----+
                                 |
                              RESTART
                                 |
                                 v
                              IDLE
```

---

# 3. State Definitions

The application uses the following states:

```cpp
enum class GameState
{
    POWER_ON,
    INITIALIZATION,
    IDLE,
    NEW_GAME,
    PLAYING,
    GAME_WON,
    GAME_LOST
};
```

Each state has a specific responsibility.

| State            | Purpose                               |
| ---------------- | ------------------------------------- |
| `POWER_ON`       | MCU has started execution             |
| `INITIALIZATION` | Initialize hardware and software      |
| `IDLE`           | Waiting for user to start a game      |
| `NEW_GAME`       | Create and initialize a new board     |
| `PLAYING`        | Main Minesweeper gameplay             |
| `GAME_WON`       | Player successfully cleared the board |
| `GAME_LOST`      | Player selected a mine                |

---

# 4. State Machine Architecture

```text
                    Game State Machine
                           |
       +-------------------+-------------------+
       |                   |                   |
       v                   v                   v
    System              Gameplay             Result
    States               State               States

 POWER_ON              PLAYING             GAME_WON
 INITIALIZATION                            GAME_LOST
 IDLE
 NEW_GAME
```

---

# 5. POWER_ON State

## Purpose

Represents the initial MCU startup condition.

The STM32H743ZI2 begins execution after reset.

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
HAL_Init()
  |
  v
POWER_ON
```

The state machine should not perform long initialization operations inside an interrupt.

---

# 6. Transition

```text
POWER_ON
   |
   | System startup complete
   v
INITIALIZATION
```

Transition condition:

```cpp
if (systemStarted)
{
    currentState = GameState::INITIALIZATION;
}
```

---

# 7. INITIALIZATION State

This state initializes:

```text
Clock
GPIO
UART
Timers
I2C
SPI
PWM
Display
Input
Buzzer
Game Engine
```

Architecture:

```text
INITIALIZATION
      |
      +-- GPIO_Init()
      |
      +-- UART_Init()
      |
      +-- Timer_Init()
      |
      +-- I2C_Init()
      |
      +-- SPI_Init()
      |
      +-- Display_Init()
      |
      +-- Input_Init()
      |
      +-- Buzzer_Init()
      |
      +-- Game_Init()
      |
      v
Initialization Complete
```

---

# 8. Initialization Failure

Hardware initialization should not blindly continue if a critical peripheral fails.

```text
              INITIALIZATION
                     |
                Init Hardware
                     |
               +-----+-----+
               |           |
             PASS         FAIL
               |           |
               v           v
             IDLE      ERROR HANDLER
```

For a more advanced implementation:

```cpp
enum class SystemState
{
    INIT,
    READY,
    ERROR
};
```

A fatal hardware failure may enter a separate system error state.

---

# 9. IDLE State

The system is initialized and waiting for the user.

```text
+-----------------------+
|         IDLE          |
|                       |
|   MINESWEEPER         |
|                       |
|   Press SELECT        |
|   to Start Game       |
+-----------------------+
```

Possible inputs:

```text
SELECT       -> Start new game
NEW_GAME     -> Start new game
```

No game board is actively being played in this state.

---

# 10. IDLE → NEW_GAME

Transition:

```text
IDLE
 |
 | START / SELECT
 v
NEW_GAME
```

Example:

```cpp
case GameState::IDLE:

    if (command == InputCommand::START)
    {
        currentState = GameState::NEW_GAME;
    }

    break;
```

---

# 11. NEW_GAME State

The `NEW_GAME` state prepares a fresh Minesweeper game.

Operations:

```text
Reset board
Clear revealed cells
Clear flags
Reset mine count
Generate mines
Calculate adjacent numbers
Reset cursor
Reset timer
Clear previous result
```

Flow:

```text
NEW_GAME
   |
   v
Reset Board
   |
   v
Place Mines
   |
   v
Calculate Numbers
   |
   v
Reset Cursor
   |
   v
Start Timer
   |
   v
PLAYING
```

---

# 12. PLAYING State

This is the main game state.

The game continuously processes:

```text
Button input
Cursor movement
Cell reveal
Flag/unflag
Timer
Display updates
Win detection
Loss detection
Buzzer events
```

Architecture:

```text
                     PLAYING
                         |
          +--------------+--------------+
          |              |              |
          v              v              v
      Input Event    Timer Update   Display Update
          |
          v
     Game Engine
          |
     +----+----+
     |         |
    SAFE      MINE
     |         |
     v         v
 Continue   GAME_LOST
```

---

# 13. Cursor Movement

The player moves around the board.

```text
UP
 |
 v
Cursor Row--

DOWN
 |
 v
Cursor Row++

LEFT
 |
 v
Cursor Column--

RIGHT
 |
 v
Cursor Column++
```

Boundary checking:

```cpp
if (row > 0)
{
    row--;
}
```

Similarly:

```cpp
if (row < BOARD_ROWS - 1)
{
    row++;
}
```

The cursor must never access an invalid board position.

---

# 14. SELECT Event

When the player selects a cell:

```text
SELECT
  |
  v
Current Cell
  |
  +----------------+
  |                |
 Mine             Safe
  |                |
  v                v
GAME_LOST       Reveal Cell
                   |
                   v
               Check Win
```

---

# 15. Safe Cell Processing

When the selected cell is not a mine:

```text
SELECT
  |
  v
Is Mine?
  |
  No
  |
  v
Reveal Cell
  |
  v
Adjacent Mines?
  |
  +---------+
  |         |
  0        >0
  |         |
  v         v
Flood Fill Stop
  |
  v
Check Win
```

---

# 16. Flood-Fill State Processing

For a zero-mine cell:

```text
Reveal Cell
     |
     v
Adjacent Mines == 0
     |
     v
Reveal Neighbors
     |
     +-- Neighbor 1
     +-- Neighbor 2
     +-- Neighbor 3
     +-- ...
     |
     v
Continue Until Boundary/Number
```

The game state remains:

```text
PLAYING
```

during flood-fill processing.

For an embedded implementation, an iterative flood-fill queue is recommended for larger boards to avoid excessive recursion depth.

---

# 17. FLAG Event

If flagging is supported:

```text
FLAG
 |
 v
Is Cell Revealed?
 |
 +----+----+
 |         |
 Yes       No
 |         |
Stop    Toggle Flag
```

Flag state:

```text
UNFLAGGED
    |
    | FLAG
    v
 FLAGGED
    |
    | FLAG
    v
UNFLAGGED
```

---

# 18. Game Win Condition

After every successful reveal:

```text
Check Win
    |
    v
Are all non-mine cells revealed?
    |
 +--+--+
 |     |
 No    Yes
 |      |
 v      v
PLAYING GAME_WON
```

Example:

```cpp
if (revealedSafeCells == totalSafeCells)
{
    currentState = GameState::GAME_WON;
}
```

---

# 19. GAME_WON State

When the player wins:

```text
GAME_WON
   |
   +-- Stop timer
   |
   +-- Display WIN
   |
   +-- Play victory sound
   |
   +-- Store score/time
   |
   v
Wait for user
```

Example display:

```text
+----------------------+
|                      |
|     YOU WIN!         |
|                      |
|     TIME: 01:24      |
|                      |
|  SELECT: NEW GAME    |
|                      |
+----------------------+
```

Possible transitions:

```text
GAME_WON
   |
   | NEW_GAME
   v
NEW_GAME
```

or:

```text
GAME_WON
   |
   | EXIT
   v
IDLE
```

---

# 20. Mine Detection

If the player selects a mine:

```text
SELECT CELL
     |
     v
Is Mine?
     |
    YES
     |
     v
Reveal Mines
     |
     v
Stop Timer
     |
     v
Play Failure Sound
     |
     v
GAME_LOST
```

---

# 21. GAME_LOST State

The game has ended because the player hit a mine.

Operations:

```text
Stop timer
Reveal mines
Display failure
Activate buzzer
Prevent normal gameplay
Wait for restart
```

Display example:

```text
+----------------------+
|                      |
|      GAME OVER       |
|                      |
|      💣 BOOM!        |
|                      |
|  SELECT: NEW GAME    |
|                      |
+----------------------+
```

---

# 22. Restart Flow

Both result states can restart the game.

```text
          +----------+
          | GAME WON |
          +----+-----+
               |
               | NEW GAME
               |
               v
          +----+-----+
          | NEW GAME |
          +----------+
               ^
               |
               |
          +----+-----+
          |GAME LOST |
          +----------+
```

---

# 23. Complete Transition Table

| Current State    | Event            | Next State       | Action               |
| ---------------- | ---------------- | ---------------- | -------------------- |
| `POWER_ON`       | Startup complete | `INITIALIZATION` | Begin initialization |
| `INITIALIZATION` | Success          | `IDLE`           | Ready for user       |
| `INITIALIZATION` | Failure          | Error handling   | Report failure       |
| `IDLE`           | Start            | `NEW_GAME`       | Create game          |
| `NEW_GAME`       | Board ready      | `PLAYING`        | Start timer          |
| `PLAYING`        | Safe cell        | `PLAYING`        | Reveal               |
| `PLAYING`        | Zero cell        | `PLAYING`        | Flood fill           |
| `PLAYING`        | Mine hit         | `GAME_LOST`      | Reveal mines         |
| `PLAYING`        | All safe cells   | `GAME_WON`       | Stop timer           |
| `GAME_WON`       | New game         | `NEW_GAME`       | Restart              |
| `GAME_LOST`      | New game         | `NEW_GAME`       | Restart              |
| `GAME_WON`       | Exit             | `IDLE`           | Return to menu       |
| `GAME_LOST`      | Exit             | `IDLE`           | Return to menu       |

---

# 24. Event Definition

The state machine can use events instead of directly checking hardware.

```cpp
enum class GameEvent
{
    NONE,

    START,
    UP,
    DOWN,
    LEFT,
    RIGHT,

    SELECT,
    FLAG,

    MINE_HIT,
    GAME_COMPLETED,

    NEW_GAME,
    EXIT
};
```

Hardware converts button presses into these events.

```text
Physical Button
      |
      v
GPIO Driver
      |
      v
Input Manager
      |
      v
GameEvent
      |
      v
State Machine
```

---

# 25. State Handler Design

Each state should have a dedicated handler.

```cpp
void StateMachine_Update(GameEvent event)
{
    switch (currentState)
    {
        case GameState::POWER_ON:
            HandlePowerOn(event);
            break;

        case GameState::INITIALIZATION:
            HandleInitialization(event);
            break;

        case GameState::IDLE:
            HandleIdle(event);
            break;

        case GameState::NEW_GAME:
            HandleNewGame(event);
            break;

        case GameState::PLAYING:
            HandlePlaying(event);
            break;

        case GameState::GAME_WON:
            HandleGameWon(event);
            break;

        case GameState::GAME_LOST:
            HandleGameLost(event);
            break;
    }
}
```

This keeps the main state-machine function simple.

---

# 26. State Entry and Exit

Each state can have:

```text
OnEntry
OnUpdate
OnExit
```

Example:

```cpp
void EnterPlaying()
{
    GameTimer_Start();
    Display_ShowBoard();
}

void UpdatePlaying(GameEvent event)
{
    Game_ProcessEvent(event);
}

void ExitPlaying()
{
    GameTimer_Stop();
}
```

Architecture:

```text
             State
               |
       +-------+-------+
       |       |       |
       v       v       v
    OnEntry OnUpdate OnExit
```

---

# 27. State Transition Function

Use one centralized transition function.

```cpp
void ChangeState(GameState nextState)
{
    if (currentState == nextState)
    {
        return;
    }

    State_OnExit(currentState);

    currentState = nextState;

    State_OnEntry(currentState);
}
```

This prevents duplicated state-transition logic.

---

# 28. State Machine With RTOS

When FreeRTOS is used:

```text
                    FreeRTOS
                       |
                       v
                 +-----------+
                 | Game Task |
                 +-----+-----+
                       |
                  Game Event
                       |
                       v
              +--------+--------+
              | State Machine   |
              +--------+--------+
                       |
          +------------+------------+
          |            |            |
          v            v            v
       Display       Buzzer       Timer
```

The Game Task owns the game state.

This avoids multiple tasks independently modifying the game state.

---

# 29. Input Event Queue

Recommended architecture:

```text
Button
  |
  v
GPIO ISR
  |
  v
Input Manager
  |
  v
FreeRTOS Queue
  |
  v
Game Task
  |
  v
State Machine
```

Example:

```cpp
GameEvent event;

if (xQueueReceive(gameQueue,
                  &event,
                  portMAX_DELAY))
{
    StateMachine_Update(event);
}
```

The exact RTOS API depends on the final implementation.

---

# 30. Interrupt Rule

The GPIO interrupt should remain short.

Do not do this:

```text
GPIO ISR
 |
 +-- Reveal Cell
 +-- Flood Fill
 +-- Check Win
 +-- Update LCD
 +-- Play Buzzer
```

Instead:

```text
GPIO ISR
 |
 +-- Detect button
 |
 +-- Queue event
 |
 v
Game Task
 |
 +-- Process event
 +-- Update game
 +-- Update state
```

This is much safer for a real embedded system.

---

# 31. Timer Events

The timer should not directly modify the game state unnecessarily.

```text
Hardware Timer
      |
      v
Timer Manager
      |
      v
Elapsed Time
      |
      v
Display Manager
```

When the game leaves `PLAYING`:

```text
GAME_WON
    |
    v
Timer Stop

GAME_LOST
    |
    v
Timer Stop
```

---

# 32. State Machine Timing

The state machine should be non-blocking.

Avoid:

```cpp
HAL_Delay(5000);
```

inside state processing.

Instead:

```text
State Entry
    |
    v
Record timestamp
    |
    v
Continue processing
    |
    v
Check elapsed time
```

This allows input, display, timer and other system functions to continue operating.

---

# 33. Error State

For a production-level embedded project, add:

```cpp
enum class SystemState
{
    BOOT,
    INITIALIZATION,
    READY,
    ERROR
};
```

Hardware error flow:

```text
Initialization
      |
      v
Peripheral Failure
      |
      v
ERROR
      |
      +-- UART error log
      +-- LED error indication
      +-- Display error
```

Example:

```text
+----------------------+
| SYSTEM ERROR         |
|                      |
| Display Init Failed  |
|                      |
| Check Hardware       |
+----------------------+
```

---

# 34. Watchdog Integration

The STM32 watchdog can protect against software hangs.

Conceptually:

```text
Game Task
    |
    v
State Machine
    |
    v
Healthy?
    |
    v
Feed Watchdog
```

If the system becomes stuck:

```text
Software Hang
     |
     v
Watchdog Timeout
     |
     v
MCU Reset
     |
     v
POWER_ON
```

The watchdog should be integrated carefully so that merely feeding it from an unrelated task does not hide a deadlocked or unhealthy application.

---

# 35. State Machine Testing

Every transition should be tested.

### Test 1 — Startup

```text
POWER_ON
    ↓
INITIALIZATION
    ↓
IDLE
```

### Test 2 — Start Game

```text
IDLE
    ↓ START
NEW_GAME
    ↓
PLAYING
```

### Test 3 — Safe Cell

```text
PLAYING
    ↓ SELECT
PLAYING
```

### Test 4 — Mine

```text
PLAYING
    ↓ SELECT MINE
GAME_LOST
```

### Test 5 — Win

```text
PLAYING
    ↓ ALL SAFE CELLS
GAME_WON
```

### Test 6 — Restart

```text
GAME_WON
    ↓ NEW_GAME
NEW_GAME
    ↓
PLAYING
```

---

# 36. State Transition Test Matrix

| Test | Starting State   | Event         | Expected State   |
| ---- | ---------------- | ------------- | ---------------- |
| T01  | `POWER_ON`       | Startup       | `INITIALIZATION` |
| T02  | `INITIALIZATION` | Success       | `IDLE`           |
| T03  | `IDLE`           | `START`       | `NEW_GAME`       |
| T04  | `NEW_GAME`       | Board ready   | `PLAYING`        |
| T05  | `PLAYING`        | Safe select   | `PLAYING`        |
| T06  | `PLAYING`        | Mine select   | `GAME_LOST`      |
| T07  | `PLAYING`        | Board cleared | `GAME_WON`       |
| T08  | `GAME_WON`       | `NEW_GAME`    | `NEW_GAME`       |
| T09  | `GAME_LOST`      | `NEW_GAME`    | `NEW_GAME`       |
| T10  | `GAME_WON`       | `EXIT`        | `IDLE`           |
| T11  | `GAME_LOST`      | `EXIT`        | `IDLE`           |

---

# 37. Final Embedded State Machine

```text
                       STM32H743ZI2
                             |
                           Reset
                             |
                             v
                      +--------------+
                      |   POWER_ON   |
                      +------+-------+
                             |
                             v
                   +---------+----------+
                   |  INITIALIZATION   |
                   +---------+----------+
                             |
                       Init Success
                             |
                             v
                       +-----+-----+
                       |    IDLE   |
                       +-----+-----+
                             |
                           START
                             |
                             v
                       +-----+-----+
                       |  NEW_GAME |
                       +-----+-----+
                             |
                      Board Created
                             |
                             v
                       +-----+-----+
                       |  PLAYING  |
                       +-----+-----+
                         |       |
                   Mine  |       | Win
                         |       |
                         v       v
                  +------+--+ +--+------+
                  |GAME_LOST| | GAME_WON|
                  +------+--+ +--+------+
                         |       |
                         +---+---+
                             |
                        NEW_GAME
                             |
                             v
                         NEW_GAME
```

---

# 38. Design Principles

The Minesweeper state machine follows these principles:

1. **One owner for game state** — the Game Task/state machine.
2. **Events drive transitions** rather than direct hardware access.
3. **ISRs remain short**.
4. **No blocking delays inside state processing**.
5. **Hardware drivers remain independent of game rules**.
6. **Each state has a clearly defined responsibility**.
7. **Transitions are centralized and testable**.
8. **Game logic is independent of the display**.
9. **Timer and buzzer are event-driven services**.
10. **The design can run bare-metal or under FreeRTOS**.

---

# 39. Recommended Implementation

For the first STM32H743ZI2 version, implement:

```text
POWER_ON
    ↓
INITIALIZATION
    ↓
IDLE
    ↓
NEW_GAME
    ↓
PLAYING
    ↓
GAME_WON / GAME_LOST
    ↓
IDLE
```

Then add advanced functionality:

```text
          +----------------+
          |  State Machine |
          +-------+--------+
                  |
       +----------+----------+
       |          |          |
       v          v          v
     Timer      Input      Display
       |          |          |
       v          v          v
   Game Time   Events     Rendering
                  |
                  v
            Minesweeper
               Engine
```

This provides a clean foundation for adding **FreeRTOS, button interrupts, LCD/OLED display, buzzer PWM, UART debugging, watchdog handling, and unit testing** without redesigning the core game.

