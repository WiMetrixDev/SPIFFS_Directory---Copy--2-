#include "HTTPClient_app.h"
/**/
elapsedSeconds HTTPClientSend_Sec;
unsigned long HTTPClientSend_Interval = 5;
/**/
HTTPClient http;
const String url = "http://192.168.88.172/PanelData/paneldata.php?";
char HTTPClient_SendBuffer[100];
void init_HTTPClient()
{
    if (WiFi.status() == WL_CONNECTED)
    {

        CONSOLE.print("[HTTP] begin...\n");
    }
}
void http_force_end()
{
    http.end();
}

size_t http_connected()
{
    return http.connected();
}

void http_ServerInit()
{
    Machine_status.bit_mc_status.bit_mcRun = 4;
    // sprintf(HTTPClient_SendBuffer, "http://192.168.88.172/PanelData/paneldata.php?macid=%d&run=%d&stop=%d",
    // sprintf(HTTPClient_SendBuffer, "http://192.168.88.151:4002/api/machineState/add?machineID=1&faultID=1&state=0",
    sprintf(HTTPClient_SendBuffer, "http://192.168.88.151:4002/api/machineState/add?machineID=%d&faultID=%d&state=%d",
            MachineNumber,
            Machine_status.bit_mc_status.bit_mcStop,
            Machine_status.bit_mc_status.bit_mcRun);
}
// void runTime_HTTPClient()
void runTime_HTTPClient(void *pvParameters)
{
    (void)pvParameters;
    /**/
    // Send data every Sec
    /**/
    for (;;)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        if (WiFi.status() == WL_CONNECTED)
        {
            if (HTTPClientSend_Sec >= HTTPClientSend_Interval)
            {
                HTTPClientSend_Sec = 0;
                // if (http.connected()) //HTTP
                // if (1 == 1) //HTTP
                // {
                CONSOLE.println(http.connected());
                CONSOLE.println("Client Send over 5 Sec");
                // sprintf(HTTPClient_SendBuffer, "%smacid=%s&run=%d", url, mDNS_username, Machine_status.bit_mc_status.bit_mcRun);
                // Running API
                // sprintf(HTTPClient_SendBuffer, "http://192.168.88.172/PanelData/paneldata.php?macid=%d&run=%d&stop=%d",
                // New API
                sprintf(HTTPClient_SendBuffer, "http://192.168.88.151:4002/api/machineState/add?machineID=%d&faultID=%d&state=%d",
                        // sprintf(HTTPClient_SendBuffer, "http://192.168.88.151:4002/api/machineState/add?machineID=1&faultID=1&state=0",
                        MachineNumber,
                        Machine_status.bit_mc_status.bit_mcStop,
                        Machine_status.bit_mc_status.bit_mcRun);
                http.begin(HTTPClient_SendBuffer); //HTTP
                http.setConnectTimeout(2500);
                if (http.connected())
                {
                    CONSOLE.println(http.connected());
                    CONSOLE.println("[http] Client Connected");
                }
                else
                {
                    CONSOLE.println("[http] Waiting for Client Reponse");
                    // digitalWrite(2, 1);
                    // vTaskDelay(500 / portTICK_PERIOD_MS);
                    // vTaskResume(ErrorSendTohttp);
                    CONSOLE.println(http.connected());
                }
                CONSOLE.println(HTTPClient_SendBuffer);
                // CONSOLE.print("[HTTP] GET...\n");
                // start connection and send HTTP header
                int httpCode = http.GET();
                // httpCode will be negative on error
                if (httpCode > 0)
                {
                    // HTTP header has been send and Server response header has been handled
                    CONSOLE.printf("[HTTP] GET... code: %d\n", httpCode);

                    // file found at server
                    if (httpCode == HTTP_CODE_OK)
                    {
                        String payload = http.getString();
                        CONSOLE.println(payload);
                        vTaskResume(xLED_SuccessSendTohttp);
                    }
                }
                else
                {
                    CONSOLE.printf("[HTTP] GET... code: %d\n", httpCode);
                    CONSOLE.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
                    vTaskResume(ErrorSendTohttp);
                }
                CONSOLE.println("Client Send Over 5 Sec End");
                http.end();
            }
        }
        else
        {
        }
    }
}
void runTime_HTTPClient_Upon_MachineUpdate(void *pvParameters)
{
    (void)pvParameters;
    /**/
    // Send data every Sec
    /**/
    for (;;)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        /**/
        // hard trigger if machine status changed
        /**/
        if (WiFi.status() == WL_CONNECTED)
        {

            if (Machine_UPdate == true)
            {
                // first end http client if any
                http.end();
                Machine_UPdate = false;
                // CONSOLE.println("**Client Update");
                HTTPClientSend_Sec = 0;
                if (http.connected())
                {
                    // sprintf(HTTPClient_SendBuffer, "%smacid=%s&status=%d", url, mDNS_username, Machine_status.bit_mc_status.bit_mcRun);
                    CONSOLE.println(HTTPClient_SendBuffer);
                    // Running API
                    // sprintf(HTTPClient_SendBuffer, "http://192.168.88.172/PanelData/paneldata.php?macid=%d&run=%d&stop=%d",
                    // New API
                    sprintf(HTTPClient_SendBuffer, "http://192.168.88.151:4002/api/machineState/add?machineID=%d&faultID=%d&state=%d",
                            // sprintf(HTTPClient_SendBuffer, "http://192.168.88.151:4002/api/machineState/add?machineID=1&faultID=1&state=0",
                            MachineNumber,
                            Machine_status.bit_mc_status.bit_mcStop,
                            Machine_status.bit_mc_status.bit_mcRun);
                    http.begin(HTTPClient_SendBuffer); //HTTP

                    // CONSOLE.print("[HTTP] GET...\n");
                    // start connection and send HTTP header
                    int httpCode = http.GET();

                    // httpCode will be negative on error
                    if (httpCode > 0)
                    {
                        // HTTP header has been send and Server response header has been handled
                        CONSOLE.printf("[HTTP] GET... code: %d\n", httpCode);

                        // file found at server
                        if (httpCode == HTTP_CODE_OK)
                        {
                            String payload = http.getString();
                            CONSOLE.println(payload);
                            vTaskResume(xLED_SuccessSendTohttp);
                        }
                    }
                    else
                    {
                        CONSOLE.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
                    }
                }
                else
                {
                    // CONSOLE.println("**Client Update End");
                    // digitalWrite(LED_BUILTIN, 1);
                    vTaskResume(ErrorSendTohttp);
                    // digitalWrite(LED_BUILTIN, 0);
                }
                http.end();
            }
        }
    }
}

// String url = "http://192.168.88.172/PanelData/paneldata.php?";
// if (run == 1)
// {
//     sprintf("%smacid=%d&run=%d", url, machineid, run);
// }
// if (run1 == 1)
// {
//     sprintf("%smacid=%d&run=%d", url, machineid, run);
// }
// if (run2 == 1)
// {
//     sprintf("%smacid=%d&run=%d", url, machineid, run);
// }
// if (run3 == 1)
// {
//     sprintf("%smacid=%d&run=%d", url, machineid, run);
// }
