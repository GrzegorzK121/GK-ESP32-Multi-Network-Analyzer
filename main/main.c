#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "GK_OLED_TEXT.h"
#include "GK_MENU_SCREEN.h"
#include "GK_LED_GPIO2_SCREEN.h"
#include "GK_SIM_800L_SCREEN.h"
#include "GK_SNAKE_SCREEN.h"
#include "GK_WIFI_SCREEN.h"


// Definicje GPIO
#define LED_GPIO 2
#define BUTTON_UP_GPIO 18
#define BUTTON_DOWN_GPIO 19
#define BUTTON_BACK_GPIO 23
#define BUTTON_SELECT_GPIO 5
#define MAX_NETWORKS 10

// Globalne zmienne
int current_screen = 0; // Aktualny ekran (0: menu g³ówne, 1: LED GPIO2, itd.)
int tryb = 0;                  // Tryb LED: 0 - OFF, 1 - ON, 2-5 - MRYGANIE
float blink_frequencies[] = {0.5f, 1.0f, 2.0f, 4.0f}; // Czêstotliwoœci migania
int frequency_index = 1;       // Indeks czêstotliwoœci (domyœlnie 1 Hz)
int signal_strength = 0;       // Wartoœæ domyœlna: brak sygna³u

// Funkcja kontroluj¹ca LED
void led_control_task(void *pvParameter) {
    while (true) {
        switch (tryb) {
            case 0: // OFF
                gpio_set_level(LED_GPIO, 0);
                vTaskDelay(100 / portTICK_PERIOD_MS);
                break;
            case 1: // ON
                gpio_set_level(LED_GPIO, 1);
                vTaskDelay(100 / portTICK_PERIOD_MS);
                break;
            case 2: case 3: case 4: case 5: { // MRYGANIE
                int delay = (int)(1000 / (2 * blink_frequencies[frequency_index]));
                gpio_set_level(LED_GPIO, 1);
                vTaskDelay(delay / portTICK_PERIOD_MS);
                gpio_set_level(LED_GPIO, 0);
                vTaskDelay(delay / portTICK_PERIOD_MS);
                break;
            }
        }
    }
}

// Funkcja g³ówna
void app_main(void) {
    ESP_LOGI("MAIN", "Initializing OLED...");

    // Inicjalizacja OLED i GPIO
    gk_oled_init();
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    // Uruchomienie taska kontroluj¹cego LED
    xTaskCreate(led_control_task, "LED Control Task", 2048, NULL, 5, NULL);

    // G³ówna pêtla obs³ugi ekranów
    while (true) {
        switch (current_screen) {
            case 0: // Menu g³ówne
                gk_menu_handle(); // Funkcja menu g³ównego zwraca numer ekranu
                break;

            case 1: // Ekran LED GPIO2
                gk_led_gpio2_handle(); // Obs³uga LED GPIO2
                current_screen = 0; // Powrót do menu g³ównego po obs³u¿eniu ekranu
                break;

            case 2: // Ekran GSM
                gk_oled_init();
                gk_sim800l_screen();
                current_screen = 0; // Powrót do menu g³ównego po obs³u¿eniu ekranu
                break;

            case 3: // WIFI
                gk_oled_init();
                gk_wifi_screen();
                current_screen = 0; // Powrót do menu g³ównego
                break;

            case 4: // SNAKE
                gk_oled_init();
                gk_snake_screen();
                current_screen = 0; // Powrót do menu g³ównego
                break;

            case 5: // USTAWIENIA
                gk_oled_init();
                gk_oled_display("USTAWIENIA - TODO", 0);
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                current_screen = 0; // Powrót do menu g³ównego
                break;



            default: // Nieznana opcja
                ESP_LOGE("MAIN", "Invalid screen index: %d", current_screen);
                current_screen = 0; // Powrót do menu g³ównego
                break;
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
