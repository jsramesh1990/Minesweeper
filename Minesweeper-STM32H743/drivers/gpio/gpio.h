#ifndef GPIO_H
#define GPIO_H

#include "stm32h7xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/*
 * Initialize GPIO hardware.
 */
void GPIO_Init(void);

/*
 * Write GPIO output.
 */
void GPIO_Write(GPIO_TypeDef *port,
                uint16_t pin,
                GPIO_PinState state);

/*
 * Toggle GPIO output.
 */
void GPIO_Toggle(GPIO_TypeDef *port,
                 uint16_t pin);

/*
 * Read GPIO input.
 */
GPIO_PinState GPIO_Read(GPIO_TypeDef *port,
                        uint16_t pin);

/*
 * Read GPIO input as boolean.
 */
bool GPIO_IsHigh(GPIO_TypeDef *port,
                 uint16_t pin);

#endif /* GPIO_H */
