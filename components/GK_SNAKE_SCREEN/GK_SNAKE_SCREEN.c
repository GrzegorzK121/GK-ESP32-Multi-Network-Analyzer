#include "GK_SNAKE_SCREEN.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "mk_glcd_base.h"
#include "mk_i2c.h"
#include <stdlib.h>
#include <time.h>

// Definicje GPIO
#define BUTTON_UP_GPIO 18
#define BUTTON_DOWN_GPIO 19
#define BUTTON_LEFT_GPIO 23
#define BUTTON_RIGHT_GPIO 5

// Rozmiary ekranu i planszy
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define CELL_SIZE 4
#define SCORE_HEIGHT 16
#define BORDER_THICKNESS 1

#define GRID_WIDTH ((SCREEN_WIDTH - 2 * BORDER_THICKNESS) / CELL_SIZE)
#define GRID_HEIGHT ((SCREEN_HEIGHT - SCORE_HEIGHT - 2 * BORDER_THICKNESS) / CELL_SIZE)

// Punkt na planszy
typedef struct {
    int x;
    int y;
} Point;

// W¹¿ i jedzenie
static Point snake[GRID_WIDTH * GRID_HEIGHT];
static int snake_length;
static Point food;
static int direction; // 0 = UP, 1 = RIGHT, 2 = DOWN, 3 = LEFT
static int score = 0;
static int speed_game = 200; // Prêdkoœæ gry (domyœlnie 200 ms)

static const char *TAG = "SNAKE_GAME";

void draw_border(void) {
    // Rysowanie ramki
    glcd_drawRect(0, SCORE_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - SCORE_HEIGHT, 1);
}

void draw_score(void) {
    char score_text[16];
    snprintf(score_text, sizeof(score_text), "Score: %d", score);
    glcd_fillRect(0, 0, SCREEN_WIDTH, SCORE_HEIGHT, 0); // Wyczyœæ górny obszar
    setCurrentFont(&DefaultFont5x8);
    glcd_puts(0, 4, score_text, 1); // Wyœwietl wynik
}

void draw_game(void) {
    // Wyczyœæ planszê wewn¹trz ramki
    glcd_fillRect(BORDER_THICKNESS, SCORE_HEIGHT + BORDER_THICKNESS,
                  SCREEN_WIDTH - 2 * BORDER_THICKNESS,
                  SCREEN_HEIGHT - SCORE_HEIGHT - 2 * BORDER_THICKNESS, 0);

    // Rysuj jedzenie
    glcd_fillRect(BORDER_THICKNESS + food.x * CELL_SIZE,
                  SCORE_HEIGHT + BORDER_THICKNESS + food.y * CELL_SIZE,
                  CELL_SIZE, CELL_SIZE, 1);

    // Rysuj wê¿a
    for (int i = 0; i < snake_length; i++) {
        glcd_fillRect(BORDER_THICKNESS + snake[i].x * CELL_SIZE,
                      SCORE_HEIGHT + BORDER_THICKNESS + snake[i].y * CELL_SIZE,
                      CELL_SIZE, CELL_SIZE, 1);
    }

    draw_border(); // Rysuj ramkê na koniec
    glcd_display(); // Aktualizacja ekranu
}

void generate_food(void) {
    food.x = rand() % GRID_WIDTH;
    food.y = rand() % GRID_HEIGHT;
}

void gk_snake_screen(void) {
    // Inicjalizacja I2C i OLED
    i2c_init(0, 22, 21, 400); // GPIO22 = SCL, GPIO21 = SDA, 400 kHz
    glcd_init(220);
    glcd_reset_viewport();

    // Inicjalizacja przycisków
    gpio_set_direction(BUTTON_UP_GPIO, GPIO_MODE_INPUT);
    gpio_set_direction(BUTTON_DOWN_GPIO, GPIO_MODE_INPUT);
    gpio_set_direction(BUTTON_LEFT_GPIO, GPIO_MODE_INPUT);
    gpio_set_direction(BUTTON_RIGHT_GPIO, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_UP_GPIO);
    gpio_pullup_en(BUTTON_DOWN_GPIO);
    gpio_pullup_en(BUTTON_LEFT_GPIO);
    gpio_pullup_en(BUTTON_RIGHT_GPIO);

    // Inicjalizacja wê¿a
    srand(time(NULL));
    snake_length = 3;
    snake[0] = (Point){GRID_WIDTH / 2, GRID_HEIGHT / 2};
    snake[1] = (Point){GRID_WIDTH / 2, GRID_HEIGHT / 2 + 1};
    snake[2] = (Point){GRID_WIDTH / 2, GRID_HEIGHT / 2 + 2};
    direction = 0;
    score = 0;

    generate_food();
    bool game_over = false;

    while (!game_over) {
        // Obs³uga wejœcia
        if (gpio_get_level(BUTTON_UP_GPIO) == 0 && direction != 2) direction = 0;
        if (gpio_get_level(BUTTON_RIGHT_GPIO) == 0 && direction != 3) direction = 1;
        if (gpio_get_level(BUTTON_DOWN_GPIO) == 0 && direction != 0) direction = 2;
        if (gpio_get_level(BUTTON_LEFT_GPIO) == 0 && direction != 1) direction = 3;

        // Ruch wê¿a
        Point new_head = snake[0];
        if (direction == 0) new_head.y--;
        if (direction == 1) new_head.x++;
        if (direction == 2) new_head.y++;
        if (direction == 3) new_head.x--;

        // Przechodzenie przez œciany
        if (new_head.x < 0) new_head.x = GRID_WIDTH - 1;
        if (new_head.x >= GRID_WIDTH) new_head.x = 0;
        if (new_head.y < 0) new_head.y = GRID_HEIGHT - 1;
        if (new_head.y >= GRID_HEIGHT) new_head.y = 0;

        // Sprawdzenie kolizji z samym sob¹
        for (int i = 0; i < snake_length; i++) {
            if (snake[i].x == new_head.x && snake[i].y == new_head.y) {
                game_over = true;
                break;
            }
        }

        if (game_over) break;

        // Dodaj now¹ g³owê
        for (int i = snake_length - 1; i > 0; i--) {
            snake[i] = snake[i - 1];
        }
        snake[0] = new_head;

        // Sprawdzenie, czy zjedzono jedzenie
        if (new_head.x == food.x && new_head.y == food.y) {
            snake_length++;
            score++;
            generate_food();
            // Zwiêksz prêdkoœæ gry po zdobyciu punktu
            if (speed_game > 50) speed_game -= 10;
        }

        // Rysuj wynik i planszê
        draw_score();
        draw_game();

        vTaskDelay(speed_game / portTICK_PERIOD_MS);
    }

    // Wyœwietlenie komunikatu "KONIEC GRY"
    glcd_fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    setCurrentFont(&DefaultFont5x8);
    glcd_puts(10, 20, "KONIEC GRY", 1);

    char final_score[16];
    snprintf(final_score, sizeof(final_score), "WYNIK: %d", score);
    glcd_puts(10, 36, final_score, 1);

    glcd_puts(10, 48, "Nacisnij klawisz", 1);
    glcd_display();

    // Czekaj na wciœniêcie przycisku
    while (gpio_get_level(BUTTON_UP_GPIO) != 0 &&
           gpio_get_level(BUTTON_DOWN_GPIO) != 0 &&
           gpio_get_level(BUTTON_LEFT_GPIO) != 0 &&
           gpio_get_level(BUTTON_RIGHT_GPIO) != 0) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
