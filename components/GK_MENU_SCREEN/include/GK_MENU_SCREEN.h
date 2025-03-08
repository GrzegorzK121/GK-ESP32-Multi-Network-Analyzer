#ifndef GK_MENU_SCREEN_H
#define GK_MENU_SCREEN_H

#include <stddef.h>

/**
 * @brief Handle the menu logic and navigation.
 */
void gk_menu_handle(void);

/**
 * @brief Declare the current screen variable for global use.
 */
extern int current_screen;

#endif // GK_MENU_SCREEN_H
