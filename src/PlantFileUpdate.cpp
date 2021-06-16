#include "PlantFileUpdate.h"

/**/
long timezone = 1;
byte daysavetime = 1;
char FileBuffer[50];
/**/
struct tm tmstruct;

void init_PlatDateAndTime()
{
    configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
    tmstruct.tm_year = 0;
}

void runTimeUpdatePlatFile_txt()
{
    if (Machine_UPdate == true)
    {
        Machine_UPdate = false;
        if (getLocalTime(&tmstruct, 5000))
        {
            CONSOLE.println("Time UPdate");
        }
        else
        {
            CONSOLE.println("Time UPdate Error");
        }
        Serial.printf("\nNow is : %d-%02d-%02d %02d:%02d:%02d", (tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);
        /**/
        sprintf(FileBuffer, "\nTime:%d-%02d-%02d %02d:%02d:%02d", (tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);
        appendFile(SPIFFS, "/Plant_Status.txt", FileBuffer);
        /**/
        sprintf(FileBuffer, "\nPlant Name:%s", mDNS_username);
        appendFile(SPIFFS, "/Plant_Status.txt", FileBuffer);
        /**/
        sprintf(FileBuffer, "\n\nPlant Status Code:%d", currentMachineStatus_Code);
        appendFile(SPIFFS, "/Plant_Status.txt", FileBuffer);
        /**/
        Serial.println("Plat file updat");
    }
    else
    {
        //nothing
    }
}
