#ifndef _MACHINE_APP_H_
#define _MACHINE_APP_H_
#include "include.h"
#include "Hardware_io_app.h"
#include "Wifi_app.h"

/**/
// int16_t  MachineNumber = 1;
/**/
union machine_status
{
    int16_t uint16_Machine_status;
    struct
    {
        int8_t bit_mcRun : 4;
        int8_t bit_mcStop : 2;
        int8_t bit_mcStart : 1;
        int8_t bit_mcAlarm : 1;
        int8_t Rev1 : 1;
        int8_t Rev2 : 1;
        int8_t Rev3 : 1;
        int8_t Rev4 : 1;
        int8_t Rev5 : 1;
        int8_t Rev6 : 1;
        int8_t Rev7 : 1;
        int8_t Rev8 : 1;
    } bit_mc_status;
};
extern machine_status Machine_status;

void init_Machine_app();

// void runTimeMachine_app();
void runTimeMachine_app(void *pvParameters);

extern char Machine_UPdate;
extern int8_t MachineNumber;
/* data */
void runTimeUpdatePlatFile_txt();
#endif