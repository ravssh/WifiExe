// Main.cpp is changed to storage_wrapper.cpp
// This is done to integrate the esp32_sdcard_msc project developed by https://github.com/atomic14
// The original source code of can be found at https://github.com/atomic14/esp32-sdcard-msc

#include "storage_wrapper.h"

#ifndef SD_CARD_SPEED_TEST
USBMSC msc;
#endif

void log(const char *str)
{
  // Serial.println(str);
  return; // Not using Serial
}

static int32_t onWrite(uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize)
{
  // Serial.printf("Writing %d bytes to %d at offset\n", bufsize, lba, offset);
  // this writes a complete sector so we should return sector size on success
  return bufsize;
}

static int32_t onRead(uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize)
{
  // Serial.printf("Reading %d bytes from %d at offset %d\n", bufsize, lba, offset);
  // this reads a complete sector so we should return sector size on success
  return bufsize;
}

static bool onStartStop(uint8_t power_condition, bool start, bool load_eject)
{
  Serial.printf("StartStop: %d %d %d\n", power_condition, start, load_eject);
  if (load_eject)
  {
#ifndef SD_CARD_SPEED_TEST
    msc.end();
#endif
  }
  return true;
}

bool isBootButtonClicked()
{
  return digitalRead(BOOT_BUTTON) == LOW;
}

void mount_storage()
{
  // Initialize SD card in MSC mode
  msc.vendorID("ESP32");
  msc.productID("USB_MSC");
  msc.productRevision("1.0");
  msc.onRead(onRead);
  msc.onWrite(onWrite);
  msc.onStartStop(onStartStop);
  msc.mediaPresent(true);
  msc.begin(1024, 512); // Placeholder values - adjust based on SD card
}

// Eject storage connected by mount_storage and restart the device
// After restart, the device shall be in USB-HID mode
void eject_storage()
{
  msc.end();
  delay(1000);
  ESP.restart();
}
