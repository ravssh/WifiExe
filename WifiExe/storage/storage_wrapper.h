#ifndef STORAGE_WRAPPER_H
#define STORAGE_WRAPPER_H

#include <Arduino.h>
#include "USB.h"
#include "USBMSC.h"

#define BOOT_BUTTON 0

#define SPEED_TEST_BUFFER_SIZE 4096
#define SPEED_TEST_NUMBER_SECTORS (SPEED_TEST_BUFFER_SIZE / 512)

// Function declarations
void mount_storage();
void eject_storage();
bool isBootButtonClicked();

#endif
