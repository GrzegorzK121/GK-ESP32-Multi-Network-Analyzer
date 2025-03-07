/**
 * @file main.c
 * @brief G��wna p�tla aplikacji ESP32 z obs�ug� OLED, LED, SIM800L, Wi-Fi i gier.
 * @author Twoje Imi�
 * @date 2024
 */

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
#define LED_GPIO 2                ///< GPIO dla diody LED
#define BUTTON_UP_GPIO 18          ///< GPIO dla przycisku "G�ra"
#define BUTTON_DOWN_GPIO 19        ///< GPIO dla przycisku "D�"
#define BUTTON_BACK_GPIO 23        ///< GPIO dla przycisku "Wstecz"
#define BUTTON_SELECT_GPIO 5       ///< GPIO dla przycisku "Wyb�r"
#define MAX_NETWORKS 10            ///< Maksymalna liczba sieci Wi-Fi do wy�wietlenia

// Globalne zmienne
int current_screen = 0; ///< Aktualnie wy�wietlany ekran: 0 - menu g��wne, 1 - LED GPIO2, 2 - SIM800L, 3 - WiFi, 4 - Gra Snake
int tryb = 0; ///< Tryb LED: 0 - OFF, 1 - ON, 2-5 - miganie
float blink_frequencies[] = {0.5f, 1.0f, 2.0f, 4.0f}; ///< Cz�stotliwo�ci migania LED
int frequency_index = 1; ///< Indeks cz�stotliwo�ci LED (domy�lnie 1 Hz)
int signal_strength = 0; ///< Si�a sygna�u (dla GSM / Wi-Fi)

/**
 * @brief Task obs�uguj�cy sterowanie diod� LED.
 * @param pvParameter Nie u�ywany parametr FreeRTOS.
 */
void led_control_task(void *pvParameter) {
    while (true) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Inicjalizuje przyciski steruj�ce ESP32.
 */
void initialize_buttons(void) {
    int buttons[] = {BUTTON_UP_GPIO, BUTTON_DOWN_GPIO, BUTTON_BACK_GPIO, BUTTON_SELECT_GPIO};
    for (int i = 0; i < 4; i++) {
        gpio_set_direction(buttons[i], GPIO_MODE_INPUT);
        gpio_pullup_en(buttons[i]);
    }
}

/**
 * @brief G��wna funkcja aplikacji ESP32.
 *
 * Inicjalizuje OLED, przyciski i diod� LED, a nast�pnie obs�uguje prze��czanie ekran�w.
 */
void app_main(void) {
    ESP_LOGI("MAIN", "Initializing OLED...");

    // Inicjalizacja OLED i GPIO
    gk_oled_init();
    initialize_buttons();
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    // Uruchomienie taska kontroluj�cego LED
    xTaskCreate(led_control_task, "LED Control Task", 2048, NULL, 5, NULL);

    // G��wna p�tla obs�ugi ekran�w
    while (true) {
        switch (current_screen) {
            case 0:
                gk_menu_handle();  ///< Obs�uga menu g��wnego
                break;
            case 1:
                // gk_led_gpio2_screen();  // Nieaktywne
                current_screen = 0;
                break;
            case 2:
                gk_sim800l_screen();  ///< Testy SIM800L (GSM, SMS, po��czenia)
                current_screen = 0;
                break;
            case 3:
                gk_wifi_screen();  ///< Skanowanie sieci Wi-Fi
                current_screen = 0;
                break;
            case 4:
                gk_snake_screen();  ///< Gra Snake na OLED
                current_screen = 0;
                break;
            default:
                ESP_LOGE("MAIN", "Invalid screen index: %d", current_screen);
                current_screen = 0;
                break;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
