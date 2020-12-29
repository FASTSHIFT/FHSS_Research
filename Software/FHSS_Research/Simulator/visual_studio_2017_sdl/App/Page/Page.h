#ifndef __PAGE_H
#define __PAGE_H

#include "lvgl/lvgl.h"
#include "../BSP/BSP.h"

void lv_obj_set_style_default(lv_obj_t* obj);
void lv_obj_set_click_anim_default(lv_obj_t* obj);

lv_obj_t* StatusBar_Create();
lv_coord_t StatusBar_GetHeight();
lv_obj_t* StatusBar_GetObj(void);

lv_obj_t* Page_ComSlaveTest_Create();

#endif
