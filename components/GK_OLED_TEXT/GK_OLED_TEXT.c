#include <stdio.h>
#include "GK_OLED_TEXT.h"

// GK_OLED_TEXT.c
#include "GK_OLED_TEXT.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "mk_glcd_base.h"
#include "mk_i2c.h"
#include <string.h>

static const char *TAG = "GK_OLED_TEXT";

void gk_oled_init(void) {
    // Initialize I2C for OLED communication using MK_I2C helper
    i2c_init(0, 22, 21, 400); // GPIO22 = SCL, GPIO21 = SDA, 400 kHz

    // Initialize OLED using MK_GLCD library
    glcd_init(220);
    glcd_reset_viewport(); // Reset viewport if active

    ESP_LOGI(TAG, "GK_OLED_TEXT Initialized.");
}

void gk_oled_display(const char *text, int section) {
    if (section < 0 || section > 3) {
        ESP_LOGW(TAG, "Invalid section number. Section must be between 0 and 3.");
        return;
    }

    // Divide the screen into 4 equal sections (16 pixels each)
    int y_start = section * 16;

    // Clear the specific section
    glcd_fillRect(0, y_start, 128, 16, 0);

    // Set font and display text in the section
    setCurrentFont(&DefaultFont5x8); // Use small default font
    glcd_puts(0, y_start + 4, text, 1); // Center text vertically in the section

    // Update the display
    glcd_display();

    ESP_LOGI(TAG, "Displayed text: '%s' in section %d", text, section);
}
