#ifndef GK_SIM_800L_H
#define GK_SIM_800L_H

#include <stdbool.h>

// Inicjalizacja modu³u SIM800L
void sim800l_init(void);

// Wysy³anie komendy AT
void sim800l_send_command(const char *command);

// Odczyt odpowiedzi na komendê
void sim800l_read_response(void);

// Sprawdzenie jakoœci sygna³u
void sim800l_check_signal(void);

// Wysy³anie SMS
void sim800l_send_sms(const char *phone_number, const char *message);



#endif // GK_SIM_800L_H
