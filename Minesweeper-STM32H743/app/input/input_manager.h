#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "../game/game_state.h"

/*
 * Initialize input manager.
 */
void InputManager_Init(void);

/*
 * Read hardware inputs and generate
 * application-level game events.
 */
GameEvent InputManager_GetEvent(void);

/*
 * Check whether an input is available.
 */
bool InputManager_IsEventAvailable(void);

#endif /* INPUT_MANAGER_H */
