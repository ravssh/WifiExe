#ifndef WIFI_SERVER_H
#define WIFI_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SD.h>
#include <FS.h>
#include "web_content.h"
#include "exescript.h"
#include "storage/storage_wrapper.h"

extern WebServer server;

void setupWiFiServer();

#endif
