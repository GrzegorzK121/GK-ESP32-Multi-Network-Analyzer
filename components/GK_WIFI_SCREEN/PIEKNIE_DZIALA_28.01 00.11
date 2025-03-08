#include "esp_wifi.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "GK_OLED_TEXT.h"
#include <string.h>

#define BUTTON_BACK_GPIO 23
#define BUTTON_UP_GPIO 18
#define BUTTON_DOWN_GPIO 19
#define BUTTON_SELECT_GPIO 5
#define MAX_NETWORKS 20
#define MAX_DISPLAY_NETWORKS 10

static const char *TAG = "GK_WIFI";

// Bufor wyników skanowania Wi-Fi
static wifi_ap_record_t ap_records[MAX_NETWORKS];
static int num_networks = 0;
static bool scanning_in_progress = false;

// Struktura do przechowywania danych wyœwietlanych sieci
typedef struct {
    int index;              // Indeks sieci (zgodny z `ap_records`)
    char ssid[33];          // Nazwa sieci (SSID)
    int rssi;               // Si³a sygna³u
} WiFiNetwork;

// Tablica widocznych sieci
static WiFiNetwork display_networks[MAX_DISPLAY_NETWORKS];
static int display_network_count = 0;

// Funkcja do wype³nienia tablicy `display_networks`
void populate_display_networks(void) {
    display_network_count = (num_networks > MAX_DISPLAY_NETWORKS) ? MAX_DISPLAY_NETWORKS : num_networks;

    for (int i = 0; i < display_network_count; i++) {
        display_networks[i].index = i;
        strncpy(display_networks[i].ssid, (char *)ap_records[i].ssid, sizeof(display_networks[i].ssid) - 1);
        display_networks[i].ssid[sizeof(display_networks[i].ssid) - 1] = '\0';
        display_networks[i].rssi = ap_records[i].rssi;
    }
}

// Funkcja inicjalizuj¹ca Wi-Fi i rozpoczynaj¹ca skanowanie
void wifi_scan_start(void) {
    ESP_LOGI(TAG, "Rozpoczêcie skanowania WiFi");

    // Inicjalizacja NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Konfiguracja Wi-Fi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    // Rozpocznij skanowanie
    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, false));
    scanning_in_progress = true;
}

// Funkcja do sprawdzenia statusu skanowania
int wifi_scan_get_status(void) {
    uint16_t ap_count = 0;
    esp_wifi_scan_get_ap_num(&ap_count);

    if (scanning_in_progress && ap_count > 0) {
        ESP_LOGI(TAG, "Znaleziono %d sieci", ap_count);
        scanning_in_progress = false;
        num_networks = (ap_count > MAX_NETWORKS) ? MAX_NETWORKS : ap_count;
        esp_wifi_scan_get_ap_records(&ap_count, ap_records);
        return 1; // Skanowanie zakoñczone
    }
    return 0; // Skanowanie w toku
}

// Funkcja do wyœwietlania szczegó³ów wybranej sieci
void gk_wifi_display_details_by_index(int selected_index) {
    int original_index = display_networks[selected_index].index;

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Siec: %.25s", (char *)ap_records[original_index].ssid);
    gk_oled_display(buffer, 0);

    snprintf(buffer, sizeof(buffer), "MAC: %02X:%02X:%02X:%02X:%02X:%02X",
             ap_records[original_index].bssid[0], ap_records[original_index].bssid[1],
             ap_records[original_index].bssid[2], ap_records[original_index].bssid[3],
             ap_records[original_index].bssid[4], ap_records[original_index].bssid[5]);
    gk_oled_display(buffer, 1);

    snprintf(buffer, sizeof(buffer), "Kanal: %d", ap_records[original_index].primary);
    gk_oled_display(buffer, 2);

    const char *auth_mode = (ap_records[original_index].authmode == WIFI_AUTH_OPEN) ? "Brak"
                          : (ap_records[original_index].authmode == WIFI_AUTH_WEP) ? "WEP"
                          : (ap_records[original_index].authmode == WIFI_AUTH_WPA_PSK) ? "WPA-PSK"
                          : (ap_records[original_index].authmode == WIFI_AUTH_WPA2_PSK) ? "WPA2-PSK"
                          : "Inne";
    snprintf(buffer, sizeof(buffer), "Zabez.: %s", auth_mode);
    gk_oled_display(buffer, 3);

    while (gpio_get_level(BUTTON_BACK_GPIO) != 0) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void gk_wifi_screen(void) {
    gk_oled_display("Sieci WiFi:", 0);

    // Konfiguracja przycisków
    gpio_set_direction(BUTTON_BACK_GPIO, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_BACK_GPIO);
    gpio_set_direction(BUTTON_UP_GPIO, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_UP_GPIO);
    gpio_set_direction(BUTTON_DOWN_GPIO, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_DOWN_GPIO);
    gpio_set_direction(BUTTON_SELECT_GPIO, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_SELECT_GPIO);

    bool exit_screen = false;
    bool scan_completed = false;

    // Rozpocznij skanowanie Wi-Fi
    wifi_scan_start();

    // Zmienne nawigacji
    int current_option = 0;
    int start_option = 0;
    const int visible_lines = 3;

    while (!exit_screen) {
        if (!scan_completed) {
            int status = wifi_scan_get_status();

            if (status == 0) {
                // Trwa skanowanie
                gk_oled_display("Skanowanie...", 1);
                gk_oled_display("", 2);
                gk_oled_display("", 3);
            } else if (status == 1) {
                // Skanowanie zakoñczone
                scan_completed = true;
                populate_display_networks(); // Zaktualizuj tablicê widocznych sieci
            }
        }

        if (scan_completed) {
            if (display_network_count == 0) {
                // Brak sieci
                gk_oled_display("Brak sieci", 1);
                gk_oled_display("", 2);
                gk_oled_display("", 3);
            } else {
                // Obs³uga dynamicznego przesuwania widoku
                if (current_option < start_option) {
                    start_option = current_option;
                } else if (current_option >= start_option + visible_lines) {
                    start_option = current_option - visible_lines + 1;
                }

                // Wyœwietlanie sieci
                for (int i = 0; i < visible_lines; i++) {
                    int option_index = start_option + i;
                    if (option_index < display_network_count) {
                        char line_buffer[24];
                        snprintf(line_buffer, sizeof(line_buffer), "%s%-10.10s %4d dBm",
                                 (option_index == current_option) ? "> " : "  ",
                                 display_networks[option_index].ssid,
                                 display_networks[option_index].rssi);
                        gk_oled_display(line_buffer, i + 1);
                    } else {
                        gk_oled_display("", i + 1);
                    }
                }
            }

            // Obs³uga przycisków
            bool button_pressed = false;
            while (!button_pressed) {
                if (gpio_get_level(BUTTON_DOWN_GPIO) == 0) {
                    current_option = (current_option + 1) % display_network_count;
                    button_pressed = true;
                    vTaskDelay(200 / portTICK_PERIOD_MS); // Debouncing
                }

                if (gpio_get_level(BUTTON_UP_GPIO) == 0) {
                    current_option = (current_option - 1 + display_network_count) % display_network_count;
                    button_pressed = true;
                    vTaskDelay(200 / portTICK_PERIOD_MS); // Debouncing
                }

                if (gpio_get_level(BUTTON_SELECT_GPIO) == 0) {
                    gk_wifi_display_details_by_index(current_option);
                    button_pressed = true;
                }

                if (gpio_get_level(BUTTON_BACK_GPIO) == 0) {
                    exit_screen = true;
                    button_pressed = true;
                }

                vTaskDelay(100 / portTICK_PERIOD_MS);
            }
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
