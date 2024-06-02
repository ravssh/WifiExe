#include "components/exescript.h"
#include "components/rgb_control.h"
#include "components/wifi_server.h"

void setup()
{
  // Use only for debugging - (May affect performance)
  // Serial.begin(115200);

  // Initialize Tasks
  setup_rgb();
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
