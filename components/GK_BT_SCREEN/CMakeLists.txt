idf_component_register(
    SRCS "GK_BT_SCREEN.c"
    INCLUDE_DIRS "include"
    REQUIRES esp32
             bt
             GK_OLED_TEXT
             GK_MENU_NAVIGATION
)
