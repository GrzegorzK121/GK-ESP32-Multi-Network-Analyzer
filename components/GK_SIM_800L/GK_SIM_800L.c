#include <stdio.h>
#include "GK_SIM_800L.h"

#include "driver/uart.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG = "SIM800L";


#define SIM800L_TX_PIN 1 // GPIO1 (TXD0 ESP32)
#define SIM800L_RX_PIN 3 // GPIO3 (RXD0 ESP32)

// Funkcja inicjalizacji modu³u SIM800L
void sim800l_init(void) {
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE); // TX = GPIO17, RX = GPIO16
    uart_driver_install(UART_NUM_1, 1024 * 2, 0, 0, NULL, 0);

    ESP_LOGI(TAG, "SIM800L Initialized. Checking connection...");

    sim800l_send_command("AT");
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    uint8_t data[128];
    int len = uart_read_bytes(UART_NUM_1, data, sizeof(data) - 1, 100 / portTICK_PERIOD_MS);
    if (len > 0) {
        data[len] = '\0';
        if (strstr((char *)data, "OK")) {
            ESP_LOGI(TAG, "Connection to SIM800L established via UART.");
        } else {
            ESP_LOGW(TAG, "SIM800L did not respond correctly. Response: %s", data);
        }
    } else {
        ESP_LOGE(TAG, "No response from SIM800L during initialization.");
    }
}

// Funkcja wysy³ania komend AT
void sim800l_send_command(const char *command) {
    ESP_LOGI(TAG, "Sending command: %s", command);
    uart_write_bytes(UART_NUM_1, command, strlen(command));
    uart_write_bytes(UART_NUM_1, "\r\n", 2);
    vTaskDelay(500 / portTICK_PERIOD_MS);
}

// Funkcja odczytu odpowiedzi
void sim800l_read_response(void) {
    uint8_t data[128];
    int len = uart_read_bytes(UART_NUM_1, data, sizeof(data) - 1, 100 / portTICK_PERIOD_MS);
    if (len > 0) {
        data[len] = '\0';
        ESP_LOGI(TAG, "Response: %s", data);

        if (strstr((char *)data, "+CREG: 1")) {
            ESP_LOGI(TAG, "SIM800L is registered with the base station (BCS). Connection is active.");
        } else if (strstr((char *)data, "+CREG: 0")) {
            ESP_LOGW(TAG, "SIM800L is not registered with the base station (BCS). Check SIM or network.");
        }
    } else {
        ESP_LOGW(TAG, "No response from SIM800L.");
    }
}

// Funkcja sprawdzaj¹ca jakoœæ sygna³u
void sim800l_check_signal(void) {
    sim800l_send_command("AT+CSQ");
    sim800l_read_response();
}

// Funkcja wysy³ania SMS
void sim800l_send_sms(const char *phone_number, const char *message) {
    sim800l_send_command("AT+CMGF=1"); // Ustaw tryb tekstowy SMS
    sim800l_read_response();

    char command[64];
    snprintf(command, sizeof(command), "AT+CMGS=\"%s\"", phone_number);
    sim800l_send_command(command);
    sim800l_read_response();

    uart_write_bytes(UART_NUM_1, message, strlen(message));
    uart_write_bytes(UART_NUM_1, "\x1A", 1); // CTRL+Z koñczy wiadomoœæ
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    sim800l_read_response();
}

int sim800l_get_signal_quality(void) {
    sim800l_send_command("AT+CSQ");

    uint8_t data[128];
    int len = uart_read_bytes(UART_NUM_1, data, sizeof(data) - 1, 100 / portTICK_PERIOD_MS);

    if (len > 0) {
        data[len] = '\0';
        ESP_LOGI("SIM800L", "Response: %s", data);

        // Parsowanie odpowiedzi AT+CSQ, np. "+CSQ: <rssi>,<ber>"
        char *rssi_ptr = strstr((char *)data, "+CSQ: ");
        if (rssi_ptr) {
            int rssi = 0;
            sscanf(rssi_ptr + 6, "%d", &rssi); // Pobierz wartoœæ RSSI
            return rssi; // Zwróæ wartoœæ RSSI
        }
    }
    return -1; // W przypadku b³êdu
}
