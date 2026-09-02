// wifi_server.h
#ifndef WIFI_SERVER_H
#define WIFI_SERVER_H

#include <WebServer.h>

extern WebServer server;
extern bool check_delay;

void setup_WiFi();

#endif
