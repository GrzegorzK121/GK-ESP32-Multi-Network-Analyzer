// GK_OLED_TEXT.h
#ifndef GK_OLED_TEXT_H
#define GK_OLED_TEXT_H

/**
 * @brief Initialize the OLED display.
 *
 * This function sets up the I2C communication and initializes the OLED display.
 */
void gk_oled_init(void);

/**
 * @brief Display text on the OLED in one of 4 sections.
 *
 * @param text The text to display (null-terminated string).
 * @param section The section number (0-3) where the text should be displayed.
 *                Section 0 is the top of the display, section 3 is the bottom.
 */
void gk_oled_display(const char *text, int section);

#endif // GK_OLED_TEXT_H
