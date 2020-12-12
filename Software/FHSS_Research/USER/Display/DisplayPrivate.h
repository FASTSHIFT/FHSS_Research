#ifndef __DISPLAYPRIVATE_H
#define __DISPLAYPRIVATE_H

/*Basic*/
#include "Adafruit_ST7735/Adafruit_ST7735.h"
#include "Basic/SysConfig.h"

typedef Adafruit_ST7735 SCREEN_CLASS;

extern SCREEN_CLASS screen;

void DisplayError_Init();

void lv_port_disp_init();
void lv_port_indev_init();
void lv_port_log_init();

void Display_Init();
void Display_Update();

/*LVGL*/
#include "lvgl/lvgl.h"

#endif
