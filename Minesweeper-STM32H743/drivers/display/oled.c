#include "oled.h"

/*
 * OLED driver implementation.
 *
 * The actual implementation depends on
 * the OLED controller, for example:
 *
 * SSD1306
 * SH1106
 *
 * and whether I2C or SPI is used.
 */

bool OLED_Init(void)
{
    /*
     * Initialize I2C/SPI OLED controller.
     */

    return true;
}

void OLED_Clear(void)
{
    /*
     * Clear frame buffer.
     */
}

void OLED_Update(void)
{
    /*
     * Send frame buffer to OLED.
     */
}

void OLED_SetCursor(uint8_t row,
                    uint8_t column)
{
    (void)row;
    (void)column;
}

void OLED_WriteChar(char character)
{
    (void)character;
}

void OLED_WriteString(const char *text)
{
    if (text == 0)
    {
        return;
    }

    /*
     * Render string into OLED frame buffer.
     */
}

void OLED_WriteNumber(uint32_t number)
{
    (void)number;

    /*
     * Convert number to characters
     * and render to OLED.
     */
}

void OLED_DrawPixel(uint8_t x,
                    uint8_t y,
                    bool state)
{
    (void)x;
    (void)y;
    (void)state;
}

void OLED_SetPower(bool enable)
{
    (void)enable;
}
