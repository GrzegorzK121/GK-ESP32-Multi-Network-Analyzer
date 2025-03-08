#include "GK_SIM_800L_SCREEN.h"
#include "GK_SIM_800L.h"
#include "GK_OLED_TEXT.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <string.h>
#include "driver/uart.h"


// Definicje przycisków
#define BUTTON_UP_GPIO 18
#define BUTTON_DOWN_GPIO 19
#define BUTTON_BACK_GPIO 23
#define BUTTON_SELECT_GPIO 5

extern int signal_strength; // Zmienna globalna
static bool is_connected = false; // Flaga po³¹czenia

void gk_sim800l_screen(void) {
    int current_option = 0; // 0: PO£¥CZ/ROZ£¥CZ, 1: STAN
    const int num_options = 2;
    int dots_state = 0;      // Stan kropek (0-3)

    while (true) {
        // Wyœwietlanie opcji na OLED
        gk_oled_display("SIM800L", 0);

        // Opcja 1: PO£¥CZ/ROZ£¥CZ
        char connect_line[16];
        if (!is_connected) {
            snprintf(connect_line, sizeof(connect_line), "%sPO£¥CZ",
                     (current_option == 0) ? "> " : "  ");
        } else {
            snprintf(connect_line, sizeof(connect_line), "%sROZ£¥CZ",
                     (current_option == 0) ? "> " : "  ");
        }
        gk_oled_display(connect_line, 1);

        // Opcja 2: STAN
        char status_line[16];
        snprintf(status_line, sizeof(status_line), "%sSTAN: %d%%",
                 (current_option == 1) ? "> " : "  ",
                 signal_strength);
        gk_oled_display(status_line, 2);

        // Obs³uga przycisków
        if (gpio_get_level(BUTTON_UP_GPIO) == 0) {
            current_option = (current_option - 1 + num_options) % num_options;
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }

        if (gpio_get_level(BUTTON_DOWN_GPIO) == 0) {
            current_option = (current_option + 1) % num_options;
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }

        if (gpio_get_level(BUTTON_SELECT_GPIO) == 0) {
            if (current_option == 0) { // PO£¥CZ/ROZ£¥CZ
                if (!is_connected) {
                    // Animacja "£¥CZÊ..."
                    for (int i = 0; i < 8; i++) {
                        char connecting_line[16];
                        snprintf(connecting_line, sizeof(connecting_line), "  £¥CZÊ%s",
                                 (dots_state == 0) ? "   " : (dots_state == 1) ? ".  " : (dots_state == 2) ? ".. " : "...");
                        gk_oled_display(connecting_line, 1);
                        dots_state = (dots_state + 1) % 4;
                        vTaskDelay(250 / portTICK_PERIOD_MS);
                    }

                    // Wysy³anie komendy AT+CREG? w celu sprawdzenia rejestracji
                    sim800l_send_command("AT+CREG?");
                    uint8_t data[128];
                    int len = uart_read_bytes(UART_NUM_1, data, sizeof(data) - 1, 100 / portTICK_PERIOD_MS);
                    if (len > 0) {
                        data[len] = '\0';
                        if (strstr((char *)data, "+CREG: 1")) {
                            gk_oled_display("PO£¥CZONO", 1);
                            is_connected = true; // Po³¹czenie udane
                        } else {
                            gk_oled_display("B£¥D PO£¥CZ.", 1);
                        }
                    } else {
                        gk_oled_display("B£¥D PO£¥CZ.", 1);
                    }
                    vTaskDelay(2000 / portTICK_PERIOD_MS);
                } else {
                    // Roz³¹czanie
                    gk_oled_display("ROZ£¥CZONO", 1);
                    is_connected = false;
                    vTaskDelay(2000 / portTICK_PERIOD_MS);
                }
            } else if (current_option == 1) { // STAN
                sim800l_send_command("AT+CSQ");

                uint8_t data[128];
                int len = uart_read_bytes(UART_NUM_1, data, sizeof(data) - 1, 100 / portTICK_PERIOD_MS);
                if (len > 0) {
                    data[len] = '\0';
                    char *rssi_ptr = strstr((char *)data, "+CSQ: ");
                    if (rssi_ptr) {
                        int rssi = 0;
                        sscanf(rssi_ptr + 6, "%d", &rssi); // Pobierz wartoœæ RSSI
                        signal_strength = (rssi == 99) ? 0 : rssi * 3; // RSSI w procentach
                    }
                }
            }
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }

        if (gpio_get_level(BUTTON_BACK_GPIO) == 0) {
            break; // Powrót do menu g³ównego
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

