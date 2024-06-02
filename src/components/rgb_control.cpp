#include "rgb_control.h"

// Change the value w.r.t. your configuration
const int pin = 21;
int blink_delay = 50;

TaskHandle_t hndl_rgb_blink = nullptr, hndl_rgb_idle = nullptr;

// Wrapper function
void blink(bool check)
{
    if (check == true)
    {
        vTaskResume(hndl_rgb_blink);
    }
    else
    {
        vTaskResume(hndl_rgb_idle);
    }
}

void blink_task(void *temp)
{
    (void)temp; // Unwanted

    vTaskSuspend(hndl_rgb_idle);
    vTaskDelay(pdMS_TO_TICKS(100));

    while (true)
    {
        neopixelWrite(pin, 0, 64, 64);
        vTaskDelay(pdMS_TO_TICKS(blink_delay));
        neopixelWrite(pin, 0, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(blink_delay));
    }
}

void idle_task(void *temp)
{
    (void)temp; // Unwanted

    vTaskSuspend(hndl_rgb_blink);
    vTaskDelay(pdMS_TO_TICKS(100));

    while (true)
    {
        for (int i = 64, j = 0, k = 64; i >= 0; i--, j++, k--)
        {
            neopixelWrite(pin, k, j, i);
            vTaskDelay(pdMS_TO_TICKS(blink_delay));
        }
    }
}

// Initialise RGB tasks & suspend them
void setup_rgb()
{
    // Create RGB blinker task pinned to core 1
    xTaskCreatePinnedToCore(
        blink_task,      // Function Name
        "blink_rgb",     // Task name
        2000,            // Stack size
        nullptr,         // When no parameter is used, simply pass NULL
        1,               // Priority
        &hndl_rgb_blink, // Task handle
        1                // Core on which the task will run
    );
    vTaskSuspend(hndl_rgb_blink);
    // Create RGB idle task pinned to core 1
    xTaskCreatePinnedToCore(
        idle_task,      // Function Name
        "idle_rgb",     // Task name
        2000,           // Stack size
        nullptr,        // When no parameter is used, simply pass NULL
        1,              // Priority
        &hndl_rgb_idle, // Task handle
        1               // Core on which the task will run
    );
    vTaskSuspend(hndl_rgb_idle);
    neopixelWrite(pin, 0, 0, 0);
}