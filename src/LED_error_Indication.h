#ifndef _LED_ERROR_INDICATION_H_
#define _LED_ERROR_INDICATION_H_
#include "include.h"
extern SemaphoreHandle_t xSemaphore_LED;
extern TaskHandle_t xLED_SuccessSendTohttp;
extern TaskHandle_t ErrorSendTohttp;
extern TaskHandle_t handlerLEDindication_WifiDisconnect;
extern TaskHandle_t handlerLEDindication_WifiKeepAlive;
void LEDindication_SuccessSendTohttp(void *pvParameters);
void LEDindication_ErrorSendTohttp(void *pvParameters);
void LEDindication_WifiKeepAlive(void *pvParameters);
void LEDindication_WifiDisconnect(void *pvParameters);
#endif