#ifndef __BSP_HARDWARE_H
#define __BSP_HARDWARE_H

#include "app/BSP/BSP.h"

#include "Basic/SysConfig.h"
#include "Arduino.h"
#include "Basic/SysLog.h"

/*Backlight*/
void Backlight_Init();
uint16_t Backlight_GetValue();
void Backlight_SetValue(int16_t val);
void Backlight_SetGradual(uint16_t target, uint16_t time = 500);
void Backlight_UpdateBKP(void);
uint16_t Backlight_GetBKP(void);

/*LED*/
typedef enum{
    LED_TX,
    LED_RX,
    LED_STA1,
    LED_STA2
}LED_Type;
void LED_Init();
void LED_SetEnable(LED_Type led, bool en);
void LED_SetToggle(LED_Type led);
void LED_Test();

#endif
