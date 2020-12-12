#ifndef __LV_AUTO_EVENT_H
#define __LV_AUTO_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

typedef struct {
    lv_obj_t** obj_p;
    lv_event_t event;
    uint32_t delay;
    void* user_data;
}lv_auto_event_data_t;

typedef struct {
    lv_task_t* task;
    lv_auto_event_data_t* auto_event_data;
    uint32_t len;
    uint32_t run_index;
}lv_auto_event_t;

lv_auto_event_t* lv_auto_event_create(lv_auto_event_data_t* auto_event_data, uint32_t len);
void lv_auto_event_del(lv_auto_event_t* ae);

#ifdef __cplusplus
}
#endif

#endif
