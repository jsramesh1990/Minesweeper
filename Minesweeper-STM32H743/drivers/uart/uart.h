#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>

/*
 * Initialize UART.
 */
bool UART_Init(void);

/*
 * Send one character.
 */
void UART_SendChar(char character);

/*
 * Send string.
 */
void UART_SendString(const char *text);

/*
 * Send formatted debug message.
 */
void UART_Printf(const char *format, ...);

/*
 * Receive one character.
 */
bool UART_ReceiveChar(char *character);

/*
 * Check receive data.
 */
bool UART_DataAvailable(void);

#endif /* UART_H */
