#include "BSP_Hardware.h"

static const uint8_t LED_Pin_Grp[] = {
        LED_TX_Pin, LED_RX_Pin, 
        LED_STA1_Pin, LED_STA2_Pin
};

void LED_Init()
{
    SysLog_Printf("LED_Pin_Grp = [%d]", sizeof(LED_Pin_Grp));
    pinMode(LED_TX_Pin, OUTPUT);
    pinMode(LED_RX_Pin, OUTPUT);
    pinMode(LED_STA1_Pin, OUTPUT);
    pinMode(LED_STA2_Pin, OUTPUT);
}

void LED_Test()
{
    for(int i = 0; i < sizeof(LED_Pin_Grp); i++)
    {
        digitalWrite(LED_Pin_Grp[i], HIGH);
        delay(100);
    }
    for(int i = 0; i < sizeof(LED_Pin_Grp); i++)
    {
        digitalWrite(LED_Pin_Grp[i], LOW);
        delay(100);
    }
}

void LED_SetEnable(LED_Type led, bool en)
{
    digitalWrite(LED_Pin_Grp[led], en);
}
