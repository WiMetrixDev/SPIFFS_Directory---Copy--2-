#ifndef _HTTPCLIENT_H
#define _HTTPCLIENT_H
#include "include.h"

void init_HTTPClient();
// void runTime_HTTPClient();
void runTime_HTTPClient(void *pvParameters);
void runTime_HTTPClient_Upon_MachineUpdate(void *pvParameters);
void http_ServerInit();
size_t http_connected();
#endif