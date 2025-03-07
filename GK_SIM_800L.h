#ifndef GK_SIM_800L_H
#define GK_SIM_800L_H

#include <stdbool.h>

// Global status variables
extern bool sim800l_check_connection_status;
extern bool sim800l_call_status;
extern bool sim800l_send_sms_status;

void gk_sim800l_uart_init(void);
void gk_sim800l_send_command(const char *command);
void gk_sim800l_read_response(void);  // <=== Dodana deklaracja
void gk_sim800l_test_all(void);
void gk_sim800l_send_sms(void);

#endif // GK_SIM_800L_H
