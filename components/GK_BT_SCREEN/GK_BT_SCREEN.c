#include "GK_BT_SCREEN.h"
#include "GK_OLED_TEXT.h"
#include "GK_MENU_NAVIGATION.h"
#include "esp_bt.h"
#include "esp_gap_bt_api.h"
#include "esp_log.h"
#include <string.h>

#define MAX_BT_DEVICES 20
static esp_bd_addr_t bt_devices[MAX_BT_DEVICES];
static char bt_device_names[MAX_BT_DEVICES][32];
static int bt_device_count = 0;

static void bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {
    switch (event) {
        case ESP_BT_GAP_DISC_RES_EVT: {
            if (bt_device_count < MAX_BT_DEVICES) {
                memcpy(bt_devices[bt_device_count], param->disc_res.bda, ESP_BD_ADDR_LEN);
                snprintf(bt_device_names[bt_device_count], sizeof(bt_device_names[bt_device_count]),
                         "%s", (char *)param->disc_res.dev_name);
                bt_device_count++;
            }
            break;
        }
        default:
            break;
    }
}

void gk_bt_scan_start(void) {
    bt_device_count = 0;
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_bt_controller_init(&bt_cfg);
    esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);
    esp_bluedroid_init();
    esp_bluedroid_enable();
    esp_bt_gap_register_callback(bt_gap_cb);
    esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
}

void gk_bt_screen(void) {
    gk_oled_display("Bluetooth:", 0);
    gk_oled_display("Skanowanie...", 1);

    gk_bt_scan_start();
    vTaskDelay(10000 / portTICK_PERIOD_MS);

    const char *options[MAX_BT_DEVICES];
    for (int i = 0; i < bt_device_count; i++) {
        options[i] = bt_device_names[i];
    }

    int selected = gk_menu_navigation("Urządzenia BT", options, bt_device_count, 0);
    if (selected >= 0) {
        char buf[64];
        sprintf(buf, "Adres: %02X:%02X:%02X:%02X:%02X:%02X",
                bt_devices[selected][0], bt_devices[selected][1], bt_devices[selected][2],
                bt_devices[selected][3], bt_devices[selected][4], bt_devices[selected][5]);
        gk_oled_display(bt_device_names[selected], 0);
        gk_oled_display(buf, 1);
        while (gpio_get_level(BUTTON_BACK_GPIO) != 0) {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }

    esp_bt_gap_cancel_discovery();
    esp_bluedroid_disable();
    esp_bluedroid_deinit();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();
}
