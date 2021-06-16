#include "LED_error_Indication.h"

// SemaphoreHandle_t xSemaphore_LED;

TaskHandle_t handlerLEDindication_WifiDisconnect;
TaskHandle_t handlerLEDindication_WifiKeepAlive;
TaskHandle_t xLED_SuccessSendTohttp;
TaskHandle_t ErrorSendTohttp;

void LEDindication_SuccessSendTohttp(void *pvParameters)
{
    for (;;)
    {
        // if (xSemaphoreTake(xSemaphore_LED, portTICK_PERIOD_MS * 500) == pdTRUE)
        // {
        digitalWrite(2, 1);
        vTaskDelay(50 / portTICK_PERIOD_MS);
        digitalWrite(2, 0);
        vTaskDelay(50 / portTICK_PERIOD_MS);
        digitalWrite(2, 1);
        vTaskDelay(50 / portTICK_PERIOD_MS);
        digitalWrite(2, 0);
        vTaskDelay(50 / portTICK_PERIOD_MS);
        digitalWrite(2, 1);
        vTaskDelay(50 / portTICK_PERIOD_MS);
        digitalWrite(2, 0);
        vTaskSuspend(NULL);
        // }
        // xSemaphoreGive(xSemaphore_LED);
    }
}
void LEDindication_ErrorSendTohttp(void *pvParameters)
{
    for (;;)
    {
        // if (xSemaphoreTake(xSemaphore_LED, portTICK_PERIOD_MS * 500) == pdTRUE)
        // {
        digitalWrite(2, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        digitalWrite(2, 0);
        vTaskSuspend(NULL);
        // }
        // xSemaphoreGive(xSemaphore_LED);
    }
}
void LEDindication_WifiKeepAlive(void *pvParameters)
{
    for (;;)
    {
        // if (xSemaphoreTake(xSemaphore_LED, portTICK_PERIOD_MS * 500) == pdTRUE)
        // {
        digitalWrite(2, 1);
        vTaskDelay(50 / portTICK_PERIOD_MS);
        digitalWrite(2, 0);
        vTaskSuspend(NULL);
        // }
        // xSemaphoreGive(xSemaphore_LED);
    }
}
void LEDindication_WifiDisconnect(void *pvParameters)
{
    for (;;)
    {
        // if (xSemaphoreTake(xSemaphore_LED, portTICK_PERIOD_MS * 500) == pdTRUE)
        // {
        digitalWrite(2, 1);
        vTaskDelay(50 / portTICK_PERIOD_MS);
        digitalWrite(2, 0);
        vTaskSuspend(NULL);
        // }
        // xSemaphoreGive(xSemaphore_LED);
    }
}
