#include "board.h"
#include "clock_config.h"

/*
 * Board initialization status.
 */
static bool board_ready = false;


void Board_Init(void)
{
    /*
     * 1. Configure system clock.
     */
    ClockConfig_Init();

    /*
     * 2. Initialize GPIO.
     */
    Board_GPIO_Init();

    /*
     * 3. Initialize board peripherals.
     */
    Board_Peripherals_Init();

    /*
     * Board initialization completed.
     */
    board_ready = true;
}


void Board_GPIO_Init(void)
{
    /*
     * STM32 GPIO initialization.
     *
     * Actual HAL-generated GPIO initialization
     * can be called here.
     *
     * Example:
     *
     * MX_GPIO_Init();
     */
}


void Board_Peripherals_Init(void)
{
    /*
     * Board-level peripheral initialization.
     *
     * Example:
     *
     * MX_USART3_UART_Init();
     * MX_I2C1_Init();
     * MX_SPI1_Init();
     * MX_TIM2_Init();
     */
}


bool Board_IsReady(void)
{
    return board_ready;
}


void Board_ErrorHandler(void)
{
    /*
     * Board-level fatal error handling.
     *
     * Example:
     *
     * Disable interrupts
     * Turn on error LED
     * Enter infinite loop
     */

    while (1)
    {
        /*
         * Error indication.
         */
    }
}
