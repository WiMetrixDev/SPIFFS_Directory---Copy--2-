#include "Machine_app.h"

#define _debugRunSignal
#define _debugStopSignal

String Machine_Name = mDNS_username;
char Machine_UPdate = false;
/**/
machine_status Machine_status;

elapsedMillis Machine_RunSignalMills;
unsigned long Machine_RunSignlaInterval = 1;
/**/
elapsedMillis Machine_StopSignalMills;
unsigned long Machine_StopSignlaInterval = 1;
/**/
signed int RunPulseSignal_Counter = 0;
signed int StopPulseSignal_Counter = 0;
/**/
int16_t prvMachineStatus_Code = 0;
int16_t currentMachineStatus_Code = 0;
/**/
int8_t MachineNumber = 1;
/**/
// enum enum_machine
// {
//     mcFirstStart,
//     mcHold,
//     mcStop,
//     mcundefined
// };

#define mcFirstStart 5
#define mcHold 1 // means machine is in Running State
#define mcStop 2
#define mcundefined 2
/**/
void runTimeReadRunSignal();
void runTimeReadStopSignal();
/**/
void init_Machine_app()
{
    init_Hardware_IO_Signals();
    Machine_status.uint16_Machine_status = 0;
}
// void runTimeMachine_app()
void runTimeMachine_app(void *pvParameters)
{
    (void)pvParameters;
    for (;;)
    // if (1 == 1)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        // runtime_Read_IO();
        if (GPIO_UPdate == true)
        {
            GPIO_UPdate = false;
            // Serial.println("GPIO_UPdate == True");
            /**/
            runTimeReadRunSignal();
            /**/
            runTimeReadStopSignal();
            /**/
            if (Machine_status.uint16_Machine_status != prvMachineStatus_Code)
            {
                Serial.println("Mahcine State Chage");
                prvMachineStatus_Code = Machine_status.uint16_Machine_status;

                currentMachineStatus_Code = prvMachineStatus_Code;
                Machine_UPdate = true;                
                CONSOLE.println(Machine_status.bit_mc_status.bit_mcStop, BIN);
                CONSOLE.println(Machine_status.bit_mc_status.bit_mcRun, BIN);
            }
            else
            {
            }
        }
    }
}
/**/
void runTimeReadStopSignal()
{
    if (Machine_StopSignalMills >= Machine_StopSignlaInterval)
    {
        Machine_StopSignalMills = 0;
        if (IO_Status.bit_mc_status.bit_mcStopUpdate == 1)
        {
            IO_Status.bit_mc_status.bit_mcStopUpdate = 0;
            if (IO_Status.bit_mc_status.bit_mcStop == 0)
            {
                Machine_status.bit_mc_status.bit_mcStop = true;
#ifdef _debugStopSignal
                CONSOLE.println("**Stop Signal true");
#endif
            }
            else if (IO_Status.bit_mc_status.bit_mcStop == 1)
            {
                Machine_status.bit_mc_status.bit_mcStop = false;
#ifdef _debugStopSignal
                CONSOLE.println("**Stop Signal false");
#endif
            }
        }
    }
}
/**/
void runTimeReadRunSignal()
{
    if (Machine_RunSignalMills >= Machine_RunSignlaInterval)
    {
        // digitalWrite(2, !digitalRead(2));
        Machine_RunSignalMills = 0;
        /**/
        if (IO_Status.bit_mc_status.mcRunSignalTimerReset == 1)
        {
            IO_Status.bit_mc_status.mcRunSignalTimerReset = 0;
            RunPulseSignal_Counter = 0;
        }
        /**/
        // Running PLus chacking
        if (IO_Status.bit_mc_status.bit_mcRun == 0)
        {
            // CONSOLE.println("IO_Status.bit_mc_status.bit_mcRun == 0");
            if (RunPulseSignal_Counter >= 3000)
            {
#ifdef _debugRunSignal
                if (RunPulseSignal_Counter == 3000)
                {
                    CONSOLE.println("**run Signal mcHold");
                }
#endif
                RunPulseSignal_Counter = 3000;
                Machine_status.bit_mc_status.bit_mcRun = mcHold;
            }
            RunPulseSignal_Counter++;
        }
        else if (IO_Status.bit_mc_status.bit_mcRun == 1)
        {
            // CONSOLE.println("IO_Status.bit_mc_status.bit_mcRun == 1");
            if (RunPulseSignal_Counter >= 3000)
            {
#ifdef _debugRunSignal
                if (RunPulseSignal_Counter == 3000)
                {
                    CONSOLE.println("**run Signal mcStop");
                }
#endif
                RunPulseSignal_Counter = 3000;
                Machine_status.bit_mc_status.bit_mcRun = mcStop;
            }
            RunPulseSignal_Counter++;
        }
        /*************************************************************************/
        if (RunPulseSignal_Counter == 1)
        {
            Machine_status.bit_mc_status.bit_mcRun = mcundefined;
#ifdef _debugRunSignal
            CONSOLE.println("run Signal Undefined");
#endif
        }
        /*************************************************************************/
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

typedef union
{
    struct
    {
        unsigned : 7;
        unsigned COLTEST : 1;
        unsigned DUPLEX : 1;
        unsigned AN_RESTART : 1;
        unsigned ISOLATE : 1;
        unsigned PDWN : 1;
        unsigned AN_ENABLE : 1;
        unsigned SPEED : 1;
        unsigned LOOPBACK : 1;
        unsigned RESET : 1;
    };
    struct
    {
        unsigned short w : 16;
    };
} __BMCONbits_t; // reg 0: PHY_REG_BMCON

__BMCONbits_t BMCONbits_t;

void new_Android()
{
    BMCONbits_t.SPEED = 1;
}
