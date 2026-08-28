#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdbool.h>

/*
 * Initialize complete STM32H743ZI2 board.
 */
void Board_Init(void);

/*
 * Initialize board GPIOs.
 */
void Board_GPIO_Init(void);

/*
 * Initialize board peripherals.
 */
void Board_Peripherals_Init(void);

/*
 * Board status.
 */
bool Board_IsReady(void);

/*
 * Board error handler.
 */
void Board_ErrorHandler(void);

#endif /* BOARD_H */
