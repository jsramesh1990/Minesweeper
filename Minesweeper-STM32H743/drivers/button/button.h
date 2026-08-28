#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    BUTTON_UP = 0,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_SELECT,
    BUTTON_FLAG,
    BUTTON_START,
    BUTTON_COUNT
} ButtonId;

/*
 * Initialize buttons.
 */
void Button_Init(void);

/*
 * Read button state.
 */
bool Button_IsPressed(ButtonId button);

/*
 * Detect a button press event.
 */
bool Button_WasPressed(ButtonId button);

/*
 * Clear button event.
 */
void Button_ClearEvent(ButtonId button);

#endif /* BUTTON_H */
