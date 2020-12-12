#include "BSP.h"

void LED_Init()
{
    pinMode(LED_TX_Pin, OUTPUT);
    pinMode(LED_RX_Pin, OUTPUT);
    pinMode(LED_STA1_Pin, OUTPUT);
    pinMode(LED_STA2_Pin, OUTPUT);
}

void LED_SetEnable(LED_Type led, bool en)
{
    const uint8_t LED_Pin_Grp[] = {
        LED_TX_Pin, LED_RX_Pin, 
        LED_STA1_Pin, LED_STA2_Pin
    };
    
    digitalWrite(LED_Pin_Grp[led], en);
}
