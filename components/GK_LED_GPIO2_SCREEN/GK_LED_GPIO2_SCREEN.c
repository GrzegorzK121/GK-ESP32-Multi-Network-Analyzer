#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "GK_OLED_TEXT.h"
#include "GK_LED_GPIO2_SCREEN.h"

#define LED_GPIO 2
#define BUTTON_UP_GPIO 18
#define BUTTON_DOWN_GPIO 19
#define BUTTON_BACK_GPIO 23
#define BUTTON_SELECT_GPIO 5

// Deklaracja globalnych zmiennych
extern int tryb;
extern int frequency_index;
extern float blink_frequencies[]; // Deklaracja z `main.c`

void gk_led_gpio2_handle(void) {
    int current_option = 0;

    while (true) {
        gk_oled_display("LED GPIO_2", 0);
        switch (current_option) {
            case 0:
                gk_oled_display(tryb == 0 ? "> STAN OFF" : "> STAN ON", 1);
                gk_oled_display("  TRYB STA£Y", 2);
                gk_oled_display("  CZÊSTOTLIWOŒÆ", 3);
                break;
            case 1:
                gk_oled_display(tryb == 0 ? "  STAN OFF" : "  STAN ON", 1);
                gk_oled_display(tryb <= 1 ? "> TRYB STA£Y" : "> TRYB MRYGANIE", 2);
                gk_oled_display("  CZÊSTOTLIWOŒÆ", 3);
                break;
            case 2:
                gk_oled_display(tryb == 0 ? "  STAN OFF" : "  STAN ON", 1);
                gk_oled_display(tryb <= 1 ? "  TRYB STA£Y" : "  TRYB MRYGANIE", 2);
                char freq_display[16];
                snprintf(freq_display, sizeof(freq_display), "> CZÊST. %.1f Hz", blink_frequencies[frequency_index]);
                gk_oled_display(freq_display, 3);
                break;
        }

        if (gpio_get_level(BUTTON_UP_GPIO) == 0) {
            current_option = (current_option - 1 < 0) ? 2 : current_option - 1;
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }

        if (gpio_get_level(BUTTON_DOWN_GPIO) == 0) {
            current_option = (current_option + 1 > 2) ? 0 : current_option + 1;
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }

        if (gpio_get_level(BUTTON_SELECT_GPIO) == 0) {
            switch (current_option) {
                case 0:
                    tryb = (tryb == 0) ? 1 : 0;
                    break;
                case 1:
                    tryb = (tryb <= 1) ? 2 : 1;
                    break;
                case 2:
                    if (tryb > 1) {
                        frequency_index = (frequency_index + 1) % 4;
                    }
                    break;
            }
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }

        if (gpio_get_level(BUTTON_BACK_GPIO) == 0) {
            break;
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
