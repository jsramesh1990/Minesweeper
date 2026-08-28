#ifndef OLED_H
#define OLED_H

#include <stdint.h>
#include <stdbool.h>

/*
 * OLED initialization.
 */
bool OLED_Init(void);

/*
 * Clear complete display.
 */
void OLED_Clear(void);

/*
 * Update physical OLED display.
 */
void OLED_Update(void);

/*
 * Set cursor position.
 */
void OLED_SetCursor(uint8_t row,
                    uint8_t column);

/*
 * Draw a character.
 */
void OLED_WriteChar(char character);

/*
 * Draw a string.
 */
void OLED_WriteString(const char *text);

/*
 * Draw a number.
 */
void OLED_WriteNumber(uint32_t number);

/*
 * Draw a pixel.
 */
void OLED_DrawPixel(uint8_t x,
                    uint8_t y,
                    bool state);

/*
 * Turn OLED on/off.
 */
void OLED_SetPower(bool enable);

#endif /* OLED_H */
