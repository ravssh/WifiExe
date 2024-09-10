#include "rgb_control.h"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// // Change the value w.r.t. your configuration
// static const int pin = 21;
// int blink_delay = 50;

// TaskHandle_t hndl_rgb_blink = nullptr, hndl_rgb_idle = nullptr;

// // Wrapper function
// void blink(bool check)
// {
//     if (check == true)
//     {
//         vTaskResume(hndl_rgb_blink);
//     }
//     else
//     {
//         vTaskResume(hndl_rgb_idle);
//     }
// }

// void blink_task(void *temp)
// {
//     (void)temp; // Unwanted

//     vTaskSuspend(hndl_rgb_idle);
//     vTaskDelay(pdMS_TO_TICKS(100));

//     while (true)
//     {
//         neopixelWrite(pin, 0, 64, 64);
//         vTaskDelay(pdMS_TO_TICKS(blink_delay));
//         neopixelWrite(pin, 0, 0, 0);
//         vTaskDelay(pdMS_TO_TICKS(blink_delay));
//     }
// }

// void idle_task(void *temp)
// {
//     (void)temp; // Unwanted

//     vTaskSuspend(hndl_rgb_blink);
//     vTaskDelay(pdMS_TO_TICKS(100));

//     while (true)
//     {
//         for (int i = 64, j = 0, k = 64; i >= 0; i--, j++, k--)
//         {
//             neopixelWrite(pin, k, j, i);
//             vTaskDelay(pdMS_TO_TICKS(blink_delay));
//         }
//     }
// }

// // Initialise RGB tasks & suspend them
// void setup_rgb()
// {
//     // Create RGB blinker task pinned to core 1
//     xTaskCreatePinnedToCore(
//         blink_task,      // Function Name
//         "blink_rgb",     // Task name
//         2000,            // Stack size
//         nullptr,         // When no parameter is used, simply pass NULL
//         1,               // Priority
//         &hndl_rgb_blink, // Task handle
//         1                // Core on which the task will run
//     );
//     vTaskSuspend(hndl_rgb_blink);
//     // Create RGB idle task pinned to core 1
//     xTaskCreatePinnedToCore(
//         idle_task,      // Function Name
//         "idle_rgb",     // Task name
//         2000,           // Stack size
//         nullptr,        // When no parameter is used, simply pass NULL
//         1,              // Priority
//         &hndl_rgb_idle, // Task handle
//         1               // Core on which the task will run
//     );
//     vTaskSuspend(hndl_rgb_idle);
//     neopixelWrite(pin, 0, 0, 0);
// }

enum
{
    INDICATOR_LED_STOP = 1 << 0,       // 1
    INDICATOR_LED_BLINK = 1 << 1,      // 2
    INDICATOR_LED_BLINK_CONT = 1 << 2, // 4
    INDICATOR_LED_RADIATE = 1 << 3,    // 8
    INDICATOR_LED_IDLE = 1 << 4        // 16
};

static void led_task(void *pvParameter);
static void handle_radiate(bool red, bool green, bool blue);
static void handle_blink(bool continuous);

static const int pin = 21;
static TaskHandle_t hndl_led_task = NULL;
static const uint8_t led_radiate_arr[] = {0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 120, 136, 153, 171, 190, 210, 231, 255};
static uint8_t led_delay = 50; // Delay in milliseconds
static led_color_t color = {0};

esp_err_t rgb_init(void)
{
    // Register Task to the core 1 only
    xTaskCreatePinnedToCore(
        led_task,       // Task function
        "led_task",     // Name for debugging
        4096,           // Stack depth
        NULL,           // Parameters
        0,              // Priority
        &hndl_led_task, // Task handle (optional)
        1               // Pin to core 1
    );

    if (hndl_led_task == NULL)
    {
        return ESP_FAIL;
    }
    else
    {
        return ESP_OK;
    }
}

esp_err_t rgb_deinit(void)
{
    vTaskDelete(xTaskGetHandle("led_task"));
    ESP_LOGI(TAG, "Led Task deleted");
    clear_led();
    return ESP_OK;
}

static void led_task(void *pvParameter)
{
    uint32_t notification = 0;
    while (true)
    {
        notification = 0;
        ESP_LOGI(TAG, "RGB Task Waiting for Notification");
        xTaskNotifyWait(0x00, ULONG_MAX, &notification, portMAX_DELAY);
        ESP_LOGI(TAG, "RGB Task Received Notification");

        if (notification == INDICATOR_LED_BLINK)
        {
            led_task_delay((uint8_t)100);
            handle_blink(false);
            continue;
        }

        else if (notification == INDICATOR_LED_RADIATE)
        {
            led_task_delay((uint8_t)150);
            handle_radiate((color.red != 0) ? true : false,
                           (color.green != 0) ? true : false,
                           (color.blue != 0) ? true : false);
        }

        else if (notification == INDICATOR_LED_BLINK_CONT)
        {
            led_task_delay((uint8_t)300);
            handle_blink(true);
        }

        else if (notification == INDICATOR_LED_IDLE)
        {
            led_task_delay((uint8_t)50);
            handle_radiate((color.red != 0) ? true : false,
                           (color.green != 0) ? true : false,
                           (color.blue != 0) ? true : false);
        }

        else if (notification == INDICATOR_LED_STOP)
        {
            clear_led();
            continue;
        }

        else
        {
            continue;
        }
    }

    // Should never reach here - If it does, suspend the task
    vTaskSuspend(NULL);
}

static void handle_radiate(bool red, bool green, bool blue)
{
    uint8_t len = sizeof(led_radiate_arr) / sizeof(led_radiate_arr[0]) - 1;
    uint8_t del = led_delay;
    uint8_t i = 0;
    led_color_t color_val = {0};
    uint32_t notification = 0;
    ESP_LOGI(TAG, "%s Flicking %s LED(s)", (del >= 100) ? "Smoothly" : "Fast",
             (red && green && blue) ? "All"
             : (red && green)       ? "Red & Green"
             : (red && blue)        ? "Red & Blue"
             : (green && blue)      ? "Green & Blue"
             : (red)                ? "Red"
             : (green)              ? "Green"
             : (blue)               ? "Blue"
                                    : "None");

    while (true)
    {
        if (xTaskNotifyWait(0x00, 0x00, &notification, pdMS_TO_TICKS(5)) == pdTRUE)
        {
            clear_led();
            xTaskNotifyStateClear(NULL);
            break;
        };

        for (i = 0; i < len; i++)
        {
            if (red)
            {
                color_val.red = led_radiate_arr[i];
            };
            if (green)
            {
                color_val.green = led_radiate_arr[i];
            };
            if (blue)
            {
                color_val.blue = led_radiate_arr[i];
            };
            set_led(&color_val);
            vTaskDelay(pdMS_TO_TICKS(del));
        }

        if (xTaskNotifyWait(0x00, 0x00, &notification, pdMS_TO_TICKS(5)) == pdTRUE)
        {
            clear_led();
            xTaskNotifyStateClear(NULL);
            break;
        };

        for (i = len - 1; i > 0; i--)
        {
            if (red)
            {
                color_val.red = led_radiate_arr[i];
            };
            if (green)
            {
                color_val.green = led_radiate_arr[i];
            };
            if (blue)
            {
                color_val.blue = led_radiate_arr[i];
            };
            set_led(&color_val);
            vTaskDelay(pdMS_TO_TICKS(del));
        }
    }
    ESP_LOGI(TAG, "Flicker Task Stopped");
    xTaskNotify(hndl_led_task, notification, eSetValueWithOverwrite);
}

static void handle_blink(bool continuous)
{
    if (continuous == true)
    {
        uint32_t notification = 0;
        while (true)
        {
            if (xTaskNotifyWait(0x00, 0x00, &notification, pdMS_TO_TICKS(5)) == pdTRUE)
            {
                clear_led();
                xTaskNotifyStateClear(NULL);
                xTaskNotify(hndl_led_task, notification, eSetValueWithOverwrite);
                break;
            };
            set_led(&color);
            vTaskDelay(pdMS_TO_TICKS(led_delay));
            clear_led();
            vTaskDelay(pdMS_TO_TICKS(led_delay));
        }
        xTaskNotify(hndl_led_task, notification, eSetValueWithOverwrite);
    }
    else
    {
        set_led(&color);
        vTaskDelay(pdMS_TO_TICKS(led_delay));
        clear_led();
    }
}

void clear_led(void)
{
    neopixelWrite(pin, 0, 0, 0);
}

void set_led(led_color_t *color_value)
{
    neopixelWrite(pin, color_value->green, color_value->red, color_value->blue);
}

void led_task_delay(uint8_t new_delay)
{
    // Keep delay >= 50ms - Helps prevent error with HID events
    led_delay = (new_delay < 50) ? 50 : new_delay;
}

void led_idle(bool red, bool green, bool blue)
{
    color.red = (red) ? 255 : 0;
    color.green = (green) ? 255 : 0;
    color.blue = (blue) ? 255 : 0;
    xTaskNotify(hndl_led_task, INDICATOR_LED_IDLE, eSetValueWithOverwrite);
}

void led_radiate(bool red, bool green, bool blue)
{
    color.red = (red) ? 255 : 0;
    color.green = (green) ? 255 : 0;
    color.blue = (blue) ? 255 : 0;
    xTaskNotify(hndl_led_task, INDICATOR_LED_RADIATE, eSetValueWithOverwrite);
}

void led_blink(uint8_t red, uint8_t green, uint8_t blue, bool continuous)
{
    color.red = red;
    color.green = green;
    color.blue = blue;
    if (continuous)
    {
        xTaskNotify(hndl_led_task, INDICATOR_LED_BLINK_CONT, eSetValueWithOverwrite);
    }
    else
    {
        xTaskNotify(hndl_led_task, INDICATOR_LED_BLINK, eSetValueWithOverwrite);
    }
}

void led_check_error(esp_err_t err)
{
    if (err == ESP_OK)
    {
        led_blink((uint8_t)0, (uint8_t)255, (uint8_t)0, false);
    }
    else
    {
        led_blink((uint8_t)255, (uint8_t)0, (uint8_t)0, false);
    }
}
