#ifndef __BSP_H
#define __BSP_H

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

/*Buttons*/
#include "ButtonEvent/ButtonEvent.h"
extern ButtonEvent btOK;
void Button_Init();
void Button_Update();

/*LED*/
typedef enum{
    LED_TX,
    LED_RX,
    LED_STA1,
    LED_STA2
}LED_Type;
void LED_Init();
void LED_SetEnable(LED_Type led, bool en);
void LED_Test();

#endif
