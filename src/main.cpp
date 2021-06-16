// This is my working file FTP SPIFFS mDNS
#include "include.h"
#include <AsyncElegantOTA.h>
/**/
// long timezone = 1;
// byte daysavetime = 1;
// char FileBuffer[50];
/**/
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket object
AsyncWebSocket ws("/ws");

// Set number of outputs
#define NUM_OUTPUTS 1

// Assign each GPIO to an output
int outputGPIOs[NUM_OUTPUTS] = {4};
// Initialize SPIFFS
void initSPIFFS()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}
String getOutputStates()
{
  JSONVar myArray;
  for (int i = 0; i < NUM_OUTPUTS; i++)
  {
    myArray["gpios"][i]["output"] = String(outputGPIOs[i]);
    myArray["gpios"][i]["state"] = String(digitalRead(outputGPIOs[i]));
  }
  String jsonString = JSON.stringify(myArray);
  Serial.println(jsonString);
  return jsonString;
}

void notifyClients(String state)
{
  ws.textAll(state);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    if (strcmp((char *)data, "states") == 0)
    {
      notifyClients(getOutputStates());
    }
    else
    {
      int gpio = atoi((char *)data);
      digitalWrite(gpio, !digitalRead(gpio));
      notifyClients(getOutputStates());
    }
  }
}
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}
void initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

/**/
void setup()
{
  CONSOLE.begin(115200);
  pinMode(2, OUTPUT);
  // Serial1.begin(115200, SERIAL_8N1, 15, 2);  //XBEE
  // Serial2.begin(115200, SERIAL_8N1, 17, 16); // HMI these ARE HARD RXTX
  WIFI_Connect__Scan_mDNS();
  /**/
  init_Machine_app();
  init_HTTPClient();
  /**/
  initSPIFFS();
  initWebSocket();
  /**/
  // xSemaphore_LED = xSemaphoreCreateMutex();
  // if (xSemaphore_LED != NULL)
  // {
  //   Serial.println("xSemaphore_LED Created Succefully");
  // }
  /**/
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html", false); });

  server.serveStatic("/", SPIFFS, "/");

  // Start ElegantOTA
  // AsyncElegantOTA.begin(&server);
  AsyncElegantOTA.begin(&server, "admin", "admin");

  // Start server
  server.begin();
  /**/
#ifdef useFTP
  FTP_init();
#endif
#ifdef useTimeDate
  init_PlatDateAndTime();
#endif

  // init_Xbee();
  /**/
  //------------------------------------------Tasking  Start
  /**/

  // //url: http://192.168.88.151:4002/api/machineState/add?machineID=1&faultID=1&state=0

  xTaskCreate(
      keepWifiAlive,
      "Keep Wifi Alive",
      4096,
      NULL,
      1,
      &xHandle_keepWifiAlive);
  /**/
  xTaskCreate(
      runtime_Read_IO,
      "runtime Read IO",
      1024,
      NULL,
      configMAX_PRIORITIES,
      NULL);
  xTaskCreate(
      runTimeMachine_app,
      "runTime Machine app",
      2048,
      NULL,
      configMAX_PRIORITIES - 1,
      NULL);
  xTaskCreate(
      runTime_HTTPClient,
      "runTime HTTPClient",
      4096,
      NULL,
      2,
      NULL);
  xTaskCreate(
      runTime_HTTPClient_Upon_MachineUpdate,
      "runTime HTTPClient Upon MachineUpdate",
      4096,
      NULL,
      3,
      NULL);
  /*LED Creating Task*/
  xTaskCreate(
      LEDindication_ErrorSendTohttp,
      "LEDindication ErrorSendTohttp",
      100,
      NULL,
      1,
      &ErrorSendTohttp);

  xTaskCreate(
      LEDindication_SuccessSendTohttp,
      "LEDindication SuccessSendTohttp",
      100,
      NULL,
      1,
      &xLED_SuccessSendTohttp);
  // xTaskCreate(
  //     LEDindication_WifiKeepAlive,
  //     "LEDindication WifiKeepAlive",
  //     100,
  //     NULL,
  //     1,
  //     &handlerLEDindication_WifiKeepAlive);
  // xTaskCreate(
  //     LEDindication_WifiDisconnect,
  //     "LEDindication WifiDisconnect",
  //     100,
  //     NULL,
  //     1,
  //     &handlerLEDindication_WifiDisconnect);
  /*End LED Creating Task*/
  vTaskSuspend(ErrorSendTohttp);
  vTaskSuspend(xLED_SuccessSendTohttp);
}
/**/
void loop()
{
  // vTaskDelay(5000 / portTICK_PERIOD_MS);
  AsyncElegantOTA.loop();
  ws.cleanupClients();
#ifdef useFTP
  runtime_ftp_loop_handleFTP();
#endif
  // runtimeWifi_Alive_Indication();
  // runTimeMachine_app();
  // runTime_HTTPClient();
#ifdef UpdatePlantFile
  runTimeUpdatePlatFile_txt();
#endif
  /**/
  // Xbee Event Handler
  /**/
  // runtimeXbeeHandler();
  // runtimeEvent_SendingData();
  /**/
}
/**/
// configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
// struct tm tmstruct;
// tmstruct.tm_year = 0;
// getLocalTime(&tmstruct, 5000);
// Serial.printf("\nNow is : %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);
// sprintf(FileBuffer, "\nNow is : %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);
// Serial.println("");
/**/
// listDir(SPIFFS, "/", 0);
// removeDir(SPIFFS, "/mydir");
// createDir(SPIFFS, "/mydir");
// deleteFile(SPIFFS, "/Plant_Status.txt");
// writeFile(SPIFFS, "/Plant_Status.txt", "This is Plant File ");
// appendFile(SPIFFS, "/Plant_Status.txt", FileBuffer);
// readFile(SPIFFS, "/Plant_Status.txt");
// listDir(SPIFFS, "/", 0);
/**/
/**/
// listDir(SPIFFS, "/", 0);
// removeDir(SPIFFS, "/mydir");
// createDir(SPIFFS, "/mydir");
// deleteFile(SPIFFS, "/hello.txt");
// writeFile(SPIFFS, "/hello.txt", "Hello ");
// appendFile(SPIFFS, "/hello.txt", "spffs and ftp Combine!\n");
// listDir(SPIFFS, "/", 0);
/**/
/**/
// Serial.println("\n----DIR: /folder1");
// listDir("/folder1");

// Serial.println("\n----DIR: /folder2:");
// listDir("/folder2");

// Serial.println("\n----DIR: /folder1/nested:");
// listDir("/folder1/nested");

// Serial.println("\n----DIR: /nested:");
// listDir("/nested");
/**/