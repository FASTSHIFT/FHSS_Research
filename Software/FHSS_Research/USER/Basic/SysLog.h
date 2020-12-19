#ifndef __SYSLOG_H
#define __SYSLOG_H

#include "Arduino.h"

#define SysLog_Serial              Serial
#define SysLog_Init()              SysLog_Serial.begin(115200)
#define SysLog_Printf(format, ...)\
do{\
    SysLog_Serial.printf("%s (%dms) : ",__FUNCTION__, millis());\
    SysLog_Serial.printf(format, ##__VA_ARGS__);\
    SysLog_Serial.println();\
}while(0)

#endif
