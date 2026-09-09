// project_config.h
#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

// SD card pins (from platformio.ini build_flags)
// Use numeric GPIO numbers for Arduino core compatibility
#define SD_CARD_MISO 13
#define SD_CARD_MOSI 11
#define SD_CARD_CLK  12
#define SD_CARD_CS   10

// Equivalent of -DARDUINO_USB_MODE
#define ARDUINO_USB_MODE

#endif // PROJECT_CONFIG_H
