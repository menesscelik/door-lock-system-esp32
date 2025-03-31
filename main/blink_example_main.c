#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"

#define SERVO_GPIO      15
#define ROWS            4
#define COLS            4

// Tuş takımı pinleri (güncel GPIO'ōlara göre)
int row_pins[ROWS] = {16, 17, 5, 18};
int col_pins[COLS] = {19, 21, 20, 14};

const char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

char entered_pass[7] = {0};
int pass_index = 0;
const char *correct_pass = "1967";
bool is_locked = true;

static const char *TAG = "KEYPAD_LOCK";

void set_servo_angle(int angle)
{
    uint32_t duty = (angle * (1638 / 180)) + 327; // 14-bit için yaklaşık hesap
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void servo_init()
{
    ledc_timer_config_t timer_conf = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_14_BIT,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf);

    ledc_channel_config_t channel_conf = {
        .gpio_num = SERVO_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_conf);
}

void gpio_init()
{
    for (int i = 0; i < ROWS; i++) {
        gpio_set_direction(row_pins[i], GPIO_MODE_OUTPUT);
        gpio_set_level(row_pins[i], 0);
    }
    for (int i = 0; i < COLS; i++) {
        gpio_set_direction(col_pins[i], GPIO_MODE_INPUT);
        gpio_set_pull_mode(col_pins[i], GPIO_PULLDOWN_ONLY);
    }
}

char scan_keypad()
{
    for (int row = 0; row < ROWS; row++) {
        gpio_set_level(row_pins[row], 1);
        for (int col = 0; col < COLS; col++) {
            if (gpio_get_level(col_pins[col])) {
                gpio_set_level(row_pins[row], 0);
                return keys[row][col];
            }
        }
        gpio_set_level(row_pins[row], 0);
    }
    return 0;
}

void reset_password()
{
    memset(entered_pass, 0, sizeof(entered_pass));
    pass_index = 0;
}

bool check_password()
{
    return strcmp(entered_pass, correct_pass) == 0;
}

void door_locked()
{
    if (check_password()) {
        set_servo_angle(10);
    } else {
        set_servo_angle(30);
        vTaskDelay(pdMS_TO_TICKS(300));
        set_servo_angle(10);
    }
    reset_password();
}

void door_open()
{
    if (check_password()) {
        set_servo_angle(180);
        vTaskDelay(pdMS_TO_TICKS(5000));
        set_servo_angle(10);
    } else {
        set_servo_angle(30);
        vTaskDelay(pdMS_TO_TICKS(300));
        set_servo_angle(10);
    }
    reset_password();
}

void app_main()
{
    ESP_LOGI(TAG, "Setup started");
    gpio_init();
    servo_init();
    set_servo_angle(180);
    vTaskDelay(pdMS_TO_TICKS(1000));
    set_servo_angle(10);
    vTaskDelay(pdMS_TO_TICKS(500));

    while (1) {
        char key = scan_keypad();
        if (key != 0) {
            ESP_LOGI(TAG, "Key pressed: %c", key);
            if (key == '#') {
                reset_password();
            } else if (key == '*') {
                is_locked = !is_locked;
                is_locked ? door_locked() : door_open();
            } else if (pass_index < sizeof(entered_pass) - 1) {
                entered_pass[pass_index++] = key;
            }
            vTaskDelay(pdMS_TO_TICKS(200));
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}