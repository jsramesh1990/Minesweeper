#include "gpio.h"

void GPIO_Init(void)
{
    /*
     * GPIO configuration is normally generated
     * by STM32CubeMX/CubeIDE.
     *
     * Example:
     *
     * __HAL_RCC_GPIOA_CLK_ENABLE();
     * __HAL_RCC_GPIOB_CLK_ENABLE();
     */
}

void GPIO_Write(GPIO_TypeDef *port,
                uint16_t pin,
                GPIO_PinState state)
{
    HAL_GPIO_WritePin(port, pin, state);
}

void GPIO_Toggle(GPIO_TypeDef *port,
                 uint16_t pin)
{
    HAL_GPIO_TogglePin(port, pin);
}

GPIO_PinState GPIO_Read(GPIO_TypeDef *port,
                        uint16_t pin)
{
    return HAL_GPIO_ReadPin(port, pin);
}

bool GPIO_IsHigh(GPIO_TypeDef *port,
                 uint16_t pin)
{
    return (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET);
}
