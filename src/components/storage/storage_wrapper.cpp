// Main.cpp is changed to storage_wrapper.cpp
// This is done to integrate the esp32_sdcard_msc project developed by https://github.com/atomic14
// The original source code of can be found at https://github.com/atomic14/esp32-sdcard-msc

#include "storage_wrapper.h"
#include "components/rgb_control.h"

#ifndef SD_CARD_SPEED_TEST
USBMSC msc;
#endif
SDCard *card;

void log(const char *str)
{
  // Serial.println(str);
  return; // Not using Serial
}

static int32_t onWrite(uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize)
{
  // Serial.printf("Writing %d bytes to %d at offset\n", bufsize, lba, offset);
  // this writes a complete sector so we should return sector size on success
  if (card->writeSectors(buffer, lba, bufsize / card->getSectorSize()))
  {
    return bufsize;
  }
  return bufsize;
  // return -1;
}

static int32_t onRead(uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize)
{
  // Serial.printf("Reading %d bytes from %d at offset %d\n", bufsize, lba, offset);
  // this reads a complete sector so we should return sector size on success
  if (card->readSectors((uint8_t *)buffer, lba, bufsize / card->getSectorSize()))
  {
    return bufsize;
  }
  return -1;
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
  // Prevent LED from starting
  vTaskSuspend(xTaskGetHandle("blink_rgb"));
  vTaskSuspend(xTaskGetHandle("idle_rgb"));

  pinMode(GPIO_NUM_2, OUTPUT);

#ifdef USE_SDIO
  card = new SDCardMultiSector(Serial, "/sd", SD_CARD_CLK, SD_CARD_CMD, SD_CARD_DAT0, SD_CARD_DAT1, SD_CARD_DAT2, SD_CARD_DAT3);
#else
  card = new SDCardLazyWrite(Serial, "/sd", SD_CARD_MISO, SD_CARD_MOSI, SD_CARD_CLK, SD_CARD_CS);
#endif

  msc.vendorID("ESP32");
  msc.productID("USB_MSC");
  msc.productRevision("1.0");
  msc.onRead(onRead);
  msc.onWrite(onWrite);
  msc.onStartStop(onStartStop);
  msc.mediaPresent(true);
  msc.begin(card->getSectorCount(), card->getSectorSize());
}

// Eject storage connected by mount_storage and restart the device
// After restart, the device shall be in USB-HID mode
void eject_storage()
{
  msc.end();
  delay(1000);
  ESP.restart();
}