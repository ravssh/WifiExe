// project_config.h
#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

// SD card pins (from platformio.ini build_flags)
#define SD_CARD_MISO GPIO_NUM_13
#define SD_CARD_MOSI GPIO_NUM_11
#define SD_CARD_CLK  GPIO_NUM_12
#define SD_CARD_CS   GPIO_NUM_10

// Equivalent of -DARDUINO_USB_MODE
#define ARDUINO_USB_MODE

#endif // PROJECT_CONFIG_H
