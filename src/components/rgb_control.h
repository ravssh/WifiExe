// rgb_control.h
#ifndef RGB_CONTROL_H
#define RGB_CONTROL_H

#include <Arduino.h>
#include <esp_err.h>

typedef struct led_color_t
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} led_color_t;

// Function declarations
esp_err_t rgb_init();
esp_err_t rgb_deinit();
void clear_led(void);
void set_led(led_color_t *color);
void led_task_delay(uint8_t del);
void led_idle(bool red, bool green, bool blue);
void led_blink(uint8_t red, uint8_t green, uint8_t blue, bool continuous);
void led_radiate(bool red, bool green, bool blue);
void led_check_error(esp_err_t err);

// // Function declarations
// void setup_rgb();
// void blink(bool check); // default value = true

#endif
