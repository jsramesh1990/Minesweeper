#include "button.h"
#include "gpio.h"

/*
 * Replace these definitions with the actual
 * NUCLEO-H743ZI2 GPIO pins selected in CubeMX.
 */

typedef struct
{
    void *port;
    uint16_t pin;
} ButtonConfig;

/*
 * Example configuration table.
 *
 * Actual GPIO ports/pins should match
 * your hardware schematic.
 */
static const ButtonConfig button_config[BUTTON_COUNT] =
{
    {0, 0},    /* UP */
    {0, 0},    /* DOWN */
    {0, 0},    /* LEFT */
    {0, 0},    /* RIGHT */
    {0, 0},    /* SELECT */
    {0, 0},    /* FLAG */
    {0, 0}     /* START */
};

void Button_Init(void)
{
    /*
     * GPIO initialization is handled by
     * the GPIO driver / CubeMX.
     */
}

bool Button_IsPressed(ButtonId button)
{
    if (button >= BUTTON_COUNT)
    {
        return false;
    }

    /*
     * Implement using the actual GPIO
     * configuration.
     */

    return false;
}

bool Button_WasPressed(ButtonId button)
{
    if (button >= BUTTON_COUNT)
    {
        return false;
    }

    /*
     * Edge detection / debounce can be
     * implemented here.
     */

    return false;
}

void Button_ClearEvent(ButtonId button)
{
    (void)button;

    /*
     * Clear pending button event.
     */
}
