#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"

#include "SDCard.h"

SDCard::SDCard(Stream &debug, const char *mount_point) : m_debug(debug), m_mount_point(mount_point)
{
}

SDCard::~SDCard()
{
}
