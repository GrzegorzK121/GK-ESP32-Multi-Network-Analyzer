#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "GK_OLED_TEXT.h"
#include "GK_MENU_SCREEN.h"
#include "GK_LED_GPIO2_SCREEN.h"

// Globalna zmienna zdefiniowana w main.c
int current_screen;

#define BUTTON_UP_GPIO 18
#define BUTTON_DOWN_GPIO 19
#define BUTTON_BACK_GPIO 23
#define BUTTON_SELECT_GPIO 5

void gk_menu_handle(void) {
    gpio_set_direction(BUTTON_UP_GPIO, GPIO_MODE_INPUT);
    gpio_set_direction(BUTTON_DOWN_GPIO, GPIO_MODE_INPUT);
    gpio_set_direction(BUTTON_BACK_GPIO, GPIO_MODE_INPUT);
    gpio_set_direction(BUTTON_SELECT_GPIO, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_UP_GPIO);
    gpio_pullup_en(BUTTON_DOWN_GPIO);
    gpio_pullup_en(BUTTON_BACK_GPIO);
    gpio_pullup_en(BUTTON_SELECT_GPIO);

    // Lista opcji menu
    const char *menu_options[] = {
        "LED GPIO_2",
        "GSM",
        "WIFI",
        "SNAKE",
        "USTAWIENIA"
    };
    const int total_options = sizeof(menu_options) / sizeof(menu_options[0]);
    const int visible_lines = 3; // Liczba widocznych opcji

    int current_option = 0;   // Aktualnie wybrana opcja (indeks)
    int start_option = 0;     // Pierwsza opcja widoczna na ekranie
    int last_option = -1;     // Ostatnio wybrana opcja

    while (true) {
        // Dynamika widocznych opcji: przesuwanie widoku
        if (current_option < start_option) {
            start_option = current_option; // Przesuń widok w górę
        } else if (current_option >= start_option + visible_lines) {
            start_option = current_option - visible_lines + 1; // Przesuń widok w dół
        }

        // Wyświetlanie tytułu
        gk_oled_display("Menu", 0);

        // Wyświetlanie opcji w widocznych liniach
        for (int i = 0; i < visible_lines; i++) {
            int option_index = start_option + i;
            if (option_index < total_options) {
                char line[16];
                snprintf(line, sizeof(line), "%s%s",
                         (option_index == current_option) ? "> " : "  ",
                         menu_options[option_index]);
                gk_oled_display(line, i + 1); // Wyświetl na linii 1, 2, lub 3
            } else {
                gk_oled_display("", i + 1); // Czyść niewykorzystane linie
            }
        }

        // Obsługa przycisków
        if (gpio_get_level(BUTTON_UP_GPIO) == 0) {
            current_option = (current_option - 1 + total_options) % total_options; // Nawigacja w górę
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }

        if (gpio_get_level(BUTTON_DOWN_GPIO) == 0) {
            current_option = (current_option + 1) % total_options; // Nawigacja w dół
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }

        if (gpio_get_level(BUTTON_SELECT_GPIO) == 0) {
            current_screen = current_option + 1; // Ustaw numer ekranu
            break; // Wyjdź z funkcji po wyborze
        }

        if (gpio_get_level(BUTTON_BACK_GPIO) == 0) {
            current_screen = 0; // Powrót do ekranu głównego
            break;
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
