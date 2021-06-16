#include "xbee_app.h"
/**/
elapsedMillis SendToZigbee_mills;
/*---*/
unsigned long SendToZigbee_Interval = 2500;
/**/
#define BUFF_LEN_ZTX 70
struct State_t
{
    uint16_t curr_state;
    uint16_t prev_state;
    uint16_t temp_state;
    uint16_t majorState;
    uint16_t minorState;
    uint16_t previousMajor;
    uint16_t previousMinor;
    uint16_t timeCounter = 0;
    uint32_t TimeInterval = 0;
    uint32_t previousInterval = 0;
    bool isFirst;
    bool sampleTime;
};
struct Eff_t
{
    bool idInserted;
    uint8_t xbeeStatusResponse;
    bool activeDrainTimer = false;
    bool activeDoorTimer = false;
    bool activeDestonerTimer = false;
    bool activePortholeTimer = false;
    bool isPreviousUnloading = false;
};

struct Serial_t
{
    char dataTemp[80];
    char buffAir[BUFF_LEN_ZTX];
};
Serial_t serial;
Eff_t eff;
State_t state;
/**/

XBeeWithCallbacks xbee;
XBeeAddress64 addr64 = XBeeAddress64(0x00000000, 0x00000000);

void zbTxStatusResponse(ZBTxStatusResponse &r, uintptr_t);
void zbReceive(ZBRxResponse &rx, uintptr_t);
void zigbee_dummy();

void init_Xbee()
{
    delay(500);
    xbee.setSerial(S2);
    xbee.onPacketError(printErrorCb, (uintptr_t)(Print *)&CONSOLE);
    xbee.onTxStatusResponse(printErrorCb, (uintptr_t)(Print *)&CONSOLE);
    xbee.onZBTxStatusResponse(zbTxStatusResponse);
    xbee.onZBRxResponse(zbReceive);
    xbee.onOtherResponse(printResponseCb, (uintptr_t)(Print *)&CONSOLE);
}
/**/
void runtimeXbeeHandler()
{
    if (S2.available())
    {
        xbee.loop();
    }
}
/**/
void runtimeEvent_SendingData()
{
    if (SendToZigbee_mills >= SendToZigbee_Interval)
    {
        zigbee_dummy();
        SendToZigbee_mills = 0;
    }
}
/**/
void zigbee_dummy()
{
    char pType = 'A';
    int Machine_Number = 12;
    char state_change = 0;
    state.previousMajor = 1;
    state.previousMinor = 2;
    state_change = 3;
    state.TimeInterval = 4;
    state.previousInterval = 5;
    sprintf(serial.buffAir, "%c,%lu,%d,%d,%u,%d,%d", pType, Machine_Number, state.previousMajor, state.previousMinor, state_change, state.TimeInterval, state.previousInterval);
    ZBTxRequest zbTx = ZBTxRequest(addr64, (uint8_t *)serial.buffAir, strlen(serial.buffAir));
    CONSOLE.print(serial.buffAir);
    xbee.send(zbTx);
    CONSOLE.println(" Sending");
}
/**/
void zbReceive(ZBRxResponse &rx, uintptr_t)
{
    CONSOLE.println("Got Response");
    uint8_t *temp = rx.getFrameData();
    uint8_t len = rx.getDataLength();
    uint8_t offset = rx.getDataOffset();
    char *resp;
    int i = 0;
    int j = 0;
    for (i = offset, j = 0; i < offset + len; i++, j++)
    {
        serial.dataTemp[j] = temp[i];
        //CONSOLE.println(serial.dataTemp[j]);
    }
    //resp = strtok(serial.dataTemp, " ,");
    //if (strstr(resp, "D"))  // JOB SERVER RESPONSE
}
/**/
void zbTxStatusResponse(ZBTxStatusResponse &r, uintptr_t)
{
    eff.xbeeStatusResponse = r.getDeliveryStatus();
    switch (eff.xbeeStatusResponse)
    {
    case SUCCESS:
        CONSOLE.println("SUCCESS");
        state.isFirst = false;
        HMI.print("DB.inf.pco=1472");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.inf.txt=\"Data Sent\"");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.flag.pic=7");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        break;

    case CCA_FAILURE:
        CONSOLE.println("CCA_FAILURE");
        HMI.print("DB.inf.pco=63488");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.inf.txt=\"Xbee Err 1\"");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.flag.pic=8");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        break;
    case INVALID_DESTINATION_ENDPOINT_SUCCESS:
        CONSOLE.println("INVALID_DESTINATION_ENDPOINT_SUCCESS");
        HMI.print("DB.inf.pco=63488");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.inf.txt=\"Xbee Err 2\"");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.flag.pic=8");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        break;
    case NETWORK_ACK_FAILURE:
        CONSOLE.println("NETWORK_ACK_FAILURE");
        HMI.print("DB.inf.pco=63488");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.inf.txt=\"Xbee Err 3\"");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.flag.pic=8");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        break;
    case NOT_JOINED_TO_NETWORK:
        CONSOLE.println("NOT_JOINED_TO_NETWORK");

        HMI.print("DB.inf.pco=63488");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.inf.txt=\"Xbee Err 4\"");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.flag.pic=8");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        break;

    case SELF_ADDRESSED:
        CONSOLE.println("SELF_ADDRESSED");

        HMI.print("DB.inf.pco=63488");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.inf.txt=\"Xbee Err 5\"");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.flag.pic=8");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        break;
    case ADDRESS_NOT_FOUND:
        CONSOLE.println("ADDRESS_NOT_FOUND");

        HMI.print("DB.inf.pco=63488");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.inf.txt=\"Xbee Err 6\"");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.flag.pic=8");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        break;

    case ROUTE_NOT_FOUND:
        CONSOLE.println("ROUTE_NOT_FOUND");
        HMI.print("DB.inf.pco=63488");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.inf.txt=\"Xbee Err 7\"");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.flag.pic=8");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        break;
    case PAYLOAD_TOO_LARGE:
        CONSOLE.println("PAYLOAD_TOO_LARGE");
        HMI.print("DB.inf.pco=63488");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.inf.txt=\"Xbee Err 8\"");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.flag.pic=8");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        break;

    default:
        CONSOLE.println("Other Response");

        HMI.print("DB.inf.pco=63488");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.inf.txt=\"Xbee Err 9\"");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.print("DB.flag.pic=8");
        HMI.write(0xFF);
        HMI.write(0xFF);
        HMI.write(0xFF);
        break;
    }
}