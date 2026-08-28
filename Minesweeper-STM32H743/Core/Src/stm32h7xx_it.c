#include "main.h"
#include "stm32h7xx_it.h"

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void HardFault_Handler(void)
{
    while (1)
    {
        /*
         * Hard fault handling.
         */
    }
}
