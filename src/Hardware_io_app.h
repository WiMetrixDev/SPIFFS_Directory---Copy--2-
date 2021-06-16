#ifndef _HARDWARE_IO_APP_H_
#define _HARDWARE_IO_APP_H_
#include "include.h"
/**/
// HW IO Mapping
/**/
// #define Indicator_LED 3
/*Input*/
#define Stop_Signal 36
#define Start_Signal 39
#define Run_Signal 34
#define Alarm_Signal 35
#define EnergyMeter_Signal 32
#define Rev1_Signal 33
#define Rev2_Signal 25
/*End*/
union io_Status
{
    int16_t uint16_IO_Status;
    struct
    {
        bool bit_mcRun : 1;
        bool bit_mcStop : 1;
        bool bit_mcStart : 1;
        bool bit_mcAlarm : 1;
        bool mcRunSignalTimerReset : 1;
        bool bit_mcStopUpdate : 1;
        bool Rev3 : 1;
        bool Rev4 : 1;
        bool Rev5 : 1;
        bool Rev6 : 1;
        bool Rev7 : 1;
        bool Rev8 : 1;
        bool Rev9 : 1;
        bool Rev10 : 1;
        bool Rev11 : 1;
        bool Rev12 : 1;
    } bit_mc_status;
};
extern io_Status IO_Status;

extern char GPIO_UPdate;
extern int16_t currentMachineStatus_Code;

void init_Hardware_IO_Signals();
// void runtime_Read_IO();
void runtime_Read_IO(void *pvParameters);
#endif