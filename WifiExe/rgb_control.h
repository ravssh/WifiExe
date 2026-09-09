// rgb_control.h
#ifndef RGB_CONTROL_H
#define RGB_CONTROL_H

#include <stdint.h>

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} led_color_t;


esp_err_t rgb_init(void);
esp_err_t rgb_deinit(void);

void clear_led(void);
void set_led(led_color_t *color_value);
void led_task_delay(uint8_t new_delay);
void led_idle(bool red, bool green, bool blue);
void led_radiate(bool red, bool green, bool blue);
void led_blink(uint8_t red, uint8_t green, uint8_t blue, bool continuous);
void led_check_error(esp_err_t err);

#endif
