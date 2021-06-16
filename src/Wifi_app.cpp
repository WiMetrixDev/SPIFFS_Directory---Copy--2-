
#include "Wifi_app.h"
#include <ESPmDNS.h>

#define office_wifi_or_MobileHotSpot

#ifdef office_wifi_or_MobileHotSpot
const char *ssid = "WiMetrix-Unify";
const char *password = "12qweASD";
#endif
#ifndef office_wifi_or_MobileHotSpot
const char *ssid = "HUAWEI Y9 2019";
const char *password = "12345678";
#endif
const char *mDNS_username = "mc01"; // mc01.local
/**/
TaskHandle_t xHandle_keepWifiAlive;
void keepWifiAlive(void *pvParameters)
{
    (void)pvParameters;
    unsigned char cc_counter = 0;
    for (;;)
    {
        static unsigned char IP_address_display = 0;
        if (WiFi.status() == WL_CONNECTED)
        {
            vTaskDelay(3000 / portTICK_PERIOD_MS);
            // CONSOLE.println("Wifi still connected");

            // vTaskResume(handlerLEDindication_WifiKeepAlive);
            cc_counter = 0;
            if (IP_address_display == 0)
            {
                IP_address_display = 1;
                CONSOLE.println("[wifi] Connected:");
                CONSOLE.print("IP Address");
                CONSOLE.println(WiFi.localIP());
                if (!MDNS.begin("mc01"))
                {
                    Serial.println("Error starting mDNS");
                }
                else
                {
                    Serial.println("mDNS Create Success mc01.local");
                }
                // Serial.println("[wifi] Connected:" );
                http_ServerInit();
            }
            if (http_connected())
            {
            }
            continue;
        }
        if (WiFi.status() != WL_CONNECTED)
        {
            cc_counter++;
        }
        if (cc_counter == 1)
        {
            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid, password);
            Serial.println("Wifi SSID and Password Applied");
            IP_address_display = 0;
        }
        if (cc_counter == 2)
        {
            // vTaskResume(handlerLEDindication_WifiDisconnect);
            static unsigned char d_counter = 0;
            IP_address_display = 0;
            d_counter++;
            if (d_counter == 10)
            {
                d_counter = 0;
                // Serial.println("Wifi Connecting.");
                // Serial.println("Wifi Connecting..");
                // Serial.println("Wifi Connecting...");
                // Serial.println("");
            }
            cc_counter--;
        }
    }
}

/**/
void WIFI_Connect__Scan_mDNS()
{
    int connTimeOut = 0;
    /**/
    Serial.begin(115200);
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.println("Setup done");
/**/
#ifdef Scan_WIFI
    Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0)
    {
        Serial.println("no networks found");
    }
    else
    {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i)
        {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
            delay(10);
        }
    }
    Serial.println("");
#endif
    /**/
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.mode(WIFI_STA);
        // Connect to WiFi network
        WiFi.begin(ssid, password);
        CONSOLE.println("");
        while (WiFi.status() != WL_CONNECTED)
        {
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
            CONSOLE.print(".");
            connTimeOut++;
            delay(500);
            if (connTimeOut > 20)
            {
                Serial.print("not Connected to interent");
                connTimeOut = 0;
                digitalWrite(LED_BUILTIN, 0);
                return;
            }
        }
    }
    digitalWrite(LED_BUILTIN, 1);
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.localIP());
    /**/
    http_ServerInit();
    /**/
    if (!MDNS.begin(mDNS_username))
    {
        Serial.println("Error starting mDNS");
        return;
    }
    else
    {
        Serial.println("starting mDNS");
    }
    /**/
}
/**/
/**/
elapsedMillis BlinkLED1Sec_Millis;
unsigned long BlinkLED1Sec_Interval = 1000;
/**/
elapsedMillis BlinkLED0_5Sec_Millis;
unsigned long BlinkLED0_5Sec_Interval = 100;
/**/
void runtime_vfLED1SecBlink()
{
    if (BlinkLED1Sec_Millis >= BlinkLED1Sec_Interval)
    {
        BlinkLED1Sec_Millis = 0;
        digitalWrite(2, !digitalRead(2));
        CONSOLE.println("Wifi still connected");
    }
}
void runtime_vfLED0_5SecBlink()
{
    if (BlinkLED0_5Sec_Millis >= BlinkLED0_5Sec_Interval)
    {
        BlinkLED0_5Sec_Millis = 0;
        digitalWrite(2, !digitalRead(2));
        CONSOLE.println("[Wifi] Disconnect");
    }
}

void runtimeWifi_Alive_Indication()
{
    static unsigned char LEDBlink_Type = 0;
    if (WiFi.status() == WL_CONNECTED)
    {
        LEDBlink_Type = 1;
    }
    else if (WiFi.status() != WL_CONNECTED)
    {
        LEDBlink_Type = 2;
    }
    if (LEDBlink_Type == 1)
    {
        runtime_vfLED1SecBlink();
    }
    else if (LEDBlink_Type == 2)
    {
        runtime_vfLED0_5SecBlink();
    }
}
