#ifndef GK_WIFI_SCREEN_H
#define GK_WIFI_SCREEN_H

#include "esp_wifi.h"

#define MAX_NETWORKS 20
#define MAX_STRING_LENGTH 33

extern char wynik_wifi[MAX_NETWORKS][4][MAX_STRING_LENGTH];


void gk_wifi_set_country(void);
void gk_wifi_scan_start(void);
void gk_wifi_scan_get_results(void);
void gk_wifi_screen(void);
void gk_wifi_display_details(int index);

#endif // GK_WIFI_SCREEN_H
