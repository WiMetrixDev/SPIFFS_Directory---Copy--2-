#ifndef _WIFI_APP_H_
#define _WIFI_APP_H_
#include "include.h"

extern const char *mDNS_username; // machine.local

extern const char *ssid;
extern const char *password;
extern TaskHandle_t xHandle_keepWifiAlive;
void WIFI_Connect__Scan_mDNS();
void runtimeWifi_Alive_Indication();
void keepWifiAlive(void *pvParameters);

#endif