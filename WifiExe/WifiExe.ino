// WifiExe.ino
#include "project_config.h"     // bring in the defines that were build_flags
#include "exescript.h"
#include "rgb_control.h"
#include "wifi_server.h"

void setup()
{
  // If you want serial debug, enable this:
  // Serial.begin(115200);
  // delay(1000);
  // Serial.println("\n\nStarting WifiExe...");

  // Initialize Tasks
  rgb_init();
  setup_usb();
  setup_WiFi();
  
  // Signal successful boot with LED
  led_idle(0, 1, 0); // Green idle LED
}

void loop()
{
  // Check web requests
  server.handleClient();

  // If SD-MSC mode, keep a delay
  if (check_delay)
  {
    delay(200);
  }
}
