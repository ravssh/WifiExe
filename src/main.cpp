#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include "components/rgb_control.h"
#include "components/wifi_server.h"

extern int blink_delay;
extern USBHIDKeyboard Keyboard;

void setup()
{
  // Use only for debugging
  // Serial.begin(115200);
  setup_usb();
  setupWiFiServer();
  rgb_tasks();
  create_write_task();
}

void loop()
{
  server.handleClient();

  if (check_delay)
  {     
    delay(200);
  }
}
