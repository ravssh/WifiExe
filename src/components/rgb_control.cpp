#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>

const int pin = 21;
int blink_delay = 50;
TaskHandle_t end_rgb_blink = NULL, end_rgb_idle = NULL;

void blinker(void *pvParameters)
{
    // int del = *((int *)pvParameters);
    while (1)
    {
        neopixelWrite(pin, 0, 64, 64);
        vTaskDelay(pdMS_TO_TICKS(blink_delay));
        neopixelWrite(pin, 0, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(blink_delay));
    }
}

void idle(void *pvParameters)
{
    while (1)
    {
        for (int i = 64, j = 0, k = 0; i >= 0; i++, j++, k++)
        {
            neopixelWrite(pin, k, j, i);
            vTaskDelay(pdMS_TO_TICKS(blink_delay));
        }
    }
}

void rgb_tasks()
{
    // Create RGB blinker task pinned to core 1
    xTaskCreatePinnedToCore(
        blinker,        // Function Name
        "blink_rgb",    // Task name
        2000,           // Stack size
        NULL,           // When no parameter is used, simply pass NULL
        1,              // Priority
        &end_rgb_blink, // Task handle
        1               // Core on which the task will run
    );
    vTaskSuspend(end_rgb_blink);
    // Create RGB idle task pinned to core 1
    xTaskCreatePinnedToCore(
        idle,          // Function Name
        "idle_rgb",    // Task name
        2000,          // Stack size
        NULL,          // When no parameter is used, simply pass NULL
        1,             // Priority
        &end_rgb_idle, // Task handle
        1              // Core on which the task will run
    );
    vTaskSuspend(end_rgb_idle);
    neopixelWrite(21, 0, 0, 0);
    delay(50);
}