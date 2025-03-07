#include "driver/gpio.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <string.h>

#include "GK_SIM_800L.h"

#define SIM800L_UART_PORT   UART_NUM_1
#define SIM800L_TX_PIN      1
#define SIM800L_RX_PIN      3
#define SIM800L_BAUD_RATE   115200

// UART initialization
void gk_sim800l_uart_init(void) {
    const uart_config_t uart_config = {
        .baud_rate = SIM800L_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(SIM800L_UART_PORT, &uart_config);
    uart_set_pin(SIM800L_UART_PORT, SIM800L_TX_PIN, SIM800L_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(SIM800L_UART_PORT, 1024 * 2, 0, 0, NULL, 0);
}

// Send AT command
void gk_sim800l_send_command(const char *command) {
    uart_write_bytes(SIM800L_UART_PORT, command, strlen(command));
    uart_write_bytes(SIM800L_UART_PORT, "\r\n", 2);
}

// Read response from SIM800L
void gk_sim800l_read_response(void) {
    char data[256];
    int len;
    int timeout = 20; // Max 2 seconds waiting

    printf("\n------ SIM800L RESPONSE START ------\n");
    while (timeout--) {
        len = uart_read_bytes(SIM800L_UART_PORT, (uint8_t *)data, sizeof(data) - 1, 100 / portTICK_PERIOD_MS);
        if (len > 0) {
            data[len] = '\0';
            printf("%s", data);
            if (strstr(data, "OK") || strstr(data, "ERROR")) {
                break;
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    printf("\n------ SIM800L RESPONSE END ------\n");
}

// Test SIM800L commands
void gk_sim800l_test_all(void) {
    const char *commands[] = {"AT", "AT+CPIN?", "AT+CREG?", "AT+CSQ", "AT+COPS?", "AT+CLCC"};

    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        gk_sim800l_send_command(commands[i]);
        gk_sim800l_read_response();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

// Send SMS function
void gk_sim800l_send_sms(void) {
    const char *phone_number = "887949478";
    const char *message = "TEST MESSAGE FROM SIM800L";

    gk_sim800l_send_command("AT+CMGF=1"); // Set text mode
    gk_sim800l_read_response();
    vTaskDelay(100 / portTICK_PERIOD_MS);

    char at_command[32];
    snprintf(at_command, sizeof(at_command), "AT+CMGS=\"%s\"", phone_number);
    gk_sim800l_send_command(at_command);
    gk_sim800l_read_response();
    vTaskDelay(100 / portTICK_PERIOD_MS);

    gk_sim800l_send_command(message);
    uart_write_bytes(SIM800L_UART_PORT, "\x1A", 1); // End message character
    gk_sim800l_read_response();
    vTaskDelay(100 / portTICK_PERIOD_MS);
}
