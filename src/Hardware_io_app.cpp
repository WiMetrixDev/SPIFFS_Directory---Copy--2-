#include "Hardware_io_app.h"
// #define LED_Indication
int16_t tempMachineStatus = 0;
signed int mcRun_counter = 0;
signed int mcAlarm_counter = 0;
signed int mcStart_counter = 0;
signed int mcStop_counter = 0;
char GPIO_UPdate = false;

io_Status IO_Status;

elapsedMillis ReadIO_Millis;
unsigned long ReadIO_Interval = 2;

void init_Hardware_IO_Signals()
{
    /*OUTPUT*/
    pinMode(2, OUTPUT);
    /*END*/
    /*INPUT*/
    pinMode(Run_Signal, INPUT_PULLUP);
    pinMode(Alarm_Signal, INPUT_PULLUP);
    pinMode(Stop_Signal, INPUT_PULLUP);
    pinMode(Start_Signal, INPUT_PULLUP);
    pinMode(EnergyMeter_Signal, INPUT_PULLUP);
    pinMode(Rev1_Signal, INPUT_PULLUP);
    pinMode(Rev2_Signal, INPUT_PULLUP);
    /*END*/
    IO_Status.uint16_IO_Status = 0;
}

// void runtime_Read_IO()
void runtime_Read_IO(void *pvParameters)
{
    (void)pvParameters;
    // if (ReadIO_Millis >= ReadIO_Interval)
    for (;;)
    {
        ReadIO_Millis = 0;
        /**/
        vTaskDelay(1 / portTICK_PERIOD_MS);
        /**/
        if (digitalRead(Run_Signal) && IO_Status.bit_mc_status.bit_mcRun == 0)
        {
            //CONSOLE.println("IO_Status.bit_mc_status.bit_mcRun == 1");
            if (mcRun_counter >= 5)
            {
                mcRun_counter = 5;
                Serial.println("high");
                IO_Status.bit_mc_status.mcRunSignalTimerReset = 1;
                IO_Status.bit_mc_status.bit_mcRun = 1;

#ifdef LED_Indication
                digitalWrite(2, 1);
#endif
            }
            mcRun_counter++;
        }
        else if (!digitalRead(Run_Signal) && IO_Status.bit_mc_status.bit_mcRun == 1)
        {
            // Running signal Active Low
            //CONSOLE.println("IO_Status.bit_mc_status.bit_mcRun == 0");
            if (mcRun_counter <= 0)
            {
                mcRun_counter = 0;
                Serial.println("Low");
                IO_Status.bit_mc_status.mcRunSignalTimerReset = 1;
                IO_Status.bit_mc_status.bit_mcRun = 0;
#ifdef LED_Indication
                digitalWrite(2, 1);
#endif
            }
            mcRun_counter--;
        }
        /**/
        if (digitalRead(Alarm_Signal) && IO_Status.bit_mc_status.bit_mcAlarm == 0)
        {
            if (mcAlarm_counter >= 5)
            {
                mcAlarm_counter = 5;
                // Serial.printf("high");
                IO_Status.bit_mc_status.bit_mcAlarm = 1;
                IO_Status.bit_mc_status.bit_mcAlarm = 1;
#ifdef LED_Indication
                digitalWrite(2, 1);
#endif
            }
            mcAlarm_counter++;
        }
        else if (!digitalRead(Alarm_Signal) && IO_Status.bit_mc_status.bit_mcAlarm == 1)
        {
            // Alarn Signal Active Low  means no alarm
            if (mcAlarm_counter <= 0)
            {
                mcAlarm_counter = 0;
                // Serial.printf("Low");
                IO_Status.bit_mc_status.bit_mcAlarm = 0;
#ifdef LED_Indication
                digitalWrite(2, 1);
#endif
            }
            mcAlarm_counter--;
        }
        /**/
        if (digitalRead(Stop_Signal) && IO_Status.bit_mc_status.bit_mcStop == 0)
        {
            // stop signal active high HW LED OFF
            if (mcStop_counter >= 5)
            {
                mcStop_counter = 5;
                Serial.println("Stop high");
                IO_Status.bit_mc_status.bit_mcStop = 1;
                IO_Status.bit_mc_status.bit_mcStopUpdate = 1;
#ifdef LED_Indication
                digitalWrite(2, 1);
#endif
            }
            mcStop_counter++;
        }
        else if (!digitalRead(Stop_Signal) && IO_Status.bit_mc_status.bit_mcStop == 1)
        {
            if (mcStop_counter <= 0)
            {
                mcStop_counter = 0;
                Serial.println("Stop Low");
                IO_Status.bit_mc_status.bit_mcStop = 0;
                IO_Status.bit_mc_status.bit_mcStopUpdate = 1;
#ifdef LED_Indication
                digitalWrite(2, 1);
#endif
            }
            mcStop_counter--;
        }
        /**/
        if (digitalRead(Start_Signal) && IO_Status.bit_mc_status.bit_mcStart == 0)
        {
            if (mcStart_counter >= 5)
            {
                mcStart_counter = 5;
                // Serial.printf("high");
                IO_Status.bit_mc_status.bit_mcStart = 1;
#ifdef LED_Indication
                digitalWrite(2, 1);
#endif
            }
            mcStart_counter++;
        }
        else if (!digitalRead(Start_Signal) && IO_Status.bit_mc_status.bit_mcStart == 1)
        {
            // start signal active  low
            if (mcStart_counter <= 0)
            {
                mcStart_counter = 0;
                // Serial.printf("Low");
                IO_Status.bit_mc_status.bit_mcStart = 0;
#ifdef LED_Indication
                digitalWrite(2, 1);
#endif
            }
            mcStart_counter--;
        }
        GPIO_UPdate = true;
        /**/
        // if (IO_Status.uint16_IO_Status != prvMachineStatus_Code)
        // {
        //     Serial.println("State Changed");
        //     prvMachineStatus_Code = IO_Status.uint16_IO_Status;
        //     currentMachineStatus_Code = prvMachineStatus_Code;
        //     GPIO_UPdate = true;
        // }
        // else
        // {
        // }
        /**/
    }
}