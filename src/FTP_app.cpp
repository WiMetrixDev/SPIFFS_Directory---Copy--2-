#include "include.h"

const char *ftp_username = "esp8266";
const char *ftp_password = "esp8266";

FtpServer ftpSrv; //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial
void FTP_init()
{
    if (SPIFFS.begin(true))
    {
        Serial.println("SPIFFS opened!");
        ftpSrv.begin(ftp_username, ftp_password); //username, password for ftp.   (default 21, 50009 for PASV)
    }
    else
    {
        Serial.println("SPIFFS NOT opened!");
    }
}

void runtime_ftp_loop_handleFTP()
{
    ftpSrv.handleFTP(); //make sure in loop you call handleFTP()!!
}