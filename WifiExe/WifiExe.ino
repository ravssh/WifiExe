// WifiExe.ino
// Arduino sketch entrypoint for WifiExe project (converted from PlatformIO layout)

#include "project_config.h"     // bring in the defines that were build_flags
#include "exescript.h"
#include "rgb_control.h"
#include "wifi_server.h"

void setup()
{
  // If you want serial debug, enable this:
  // Serial.begin(115200);

  // Initialize Tasks
  rgb_init();
  setup_usb();
  setup_WiFi();
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
