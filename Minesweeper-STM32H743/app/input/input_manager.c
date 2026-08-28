#include "input_manager.h"

/*
 * Hardware-specific button functions can
 * be connected here later.
 *
 * Example:
 *
 * #include "button_driver.h"
 */

void InputManager_Init(void)
{
    /*
     * Initialize input hardware.
     *
     * Example:
     *
     * Button_Init();
     */
}

GameEvent InputManager_GetEvent(void)
{
    /*
     * Read physical buttons and convert
     * them into GameEvent values.
     *
     * Example:
     *
     * if (Button_UpPressed())
     * {
     *     return GAME_EVENT_UP;
     * }
     *
     * if (Button_DownPressed())
     * {
     *     return GAME_EVENT_DOWN;
     * }
     *
     * if (Button_LeftPressed())
     * {
     *     return GAME_EVENT_LEFT;
     * }
     *
     * if (Button_RightPressed())
     * {
     *     return GAME_EVENT_RIGHT;
     * }
     *
     * if (Button_SelectPressed())
     * {
     *     return GAME_EVENT_SELECT;
     * }
     *
     * if (Button_FlagPressed())
     * {
     *     return GAME_EVENT_FLAG;
     * }
     */

    return GAME_EVENT_NONE;
}

bool InputManager_IsEventAvailable(void)
{
    /*
     * Return true when a button/input
     * event is available.
     */

    return false;
}
