#include "components/exescript.h"
#include "components/rgb_control.h"
#include "components/wifi_server.h"

void setup()
{
  // Use only for debugging - (May affect performance)
  // Serial.begin(115200);
  // delay(1000);
  // Serial.println("Starting WifiExe...");

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
