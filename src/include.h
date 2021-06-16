#ifndef _INCLUDE_H_
#define _INCLUDE_H_
//  #define useFTP
// / #define useTimeDate
//  #define UpdatePlantFile
/**/
// System Header Files
/**/
#include <Arduino.h>
#include <stdio.h>
#include <String.h>
#include <elapsedMillis.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
// #include <WebServer.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <Arduino_JSON.h>
#include <time.h>
#include "SPIFFS.h"
#include "FS.h"
#include <EEPROM.h>
#include <XBee.h>
#include <defines.h>
#include <Printers.h>
/**/
// Local Header Files
/**/
#include "Wifi_app.h"
#include "defines.h"
#include "SPIFFS_app.h"
#include "FTP_app.h"
#include "Hardware_io_app.h"
#include "Machine_app.h"
#include "PlantFileUpdate.h"
#include "xbee_app.h"
#include "HTTPClient_app.h"
#include "LED_error_Indication.h"
#endif