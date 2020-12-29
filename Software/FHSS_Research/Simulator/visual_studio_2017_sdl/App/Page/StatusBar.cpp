#include "Page.h"

static lv_obj_t* objStatusBar;

static void StatusBar_Update(lv_task_t* task)
{
    
}

static void StatusBar_EventHandler(lv_obj_t* obj, lv_event_t event)
{
    if(event == LV_EVENT_LONG_PRESSED)
    {
        ComTest_SetMaster(!ComTest_GetIsMaster());
    }
}

lv_obj_t* StatusBar_Create()
{ 
    lv_obj_t* obj = lv_obj_create(lv_layer_top(), NULL);
    
    lv_obj_set_size(obj, LV_HOR_RES, 15);
    //lv_obj_set_style_default(obj);
    lv_obj_set_click_anim_default(obj);
    lv_obj_align(obj, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_event_cb(obj, StatusBar_EventHandler);

    objStatusBar = obj;
    
    lv_task_create(StatusBar_Update, 2000, LV_TASK_PRIO_MID, NULL);

    return obj;
}

void StatusBar_SetTxMode(bool tx)
{
    if(tx)
    {
    }
    else
    {
    }
}

lv_coord_t StatusBar_GetHeight()
{
    return lv_obj_get_height(objStatusBar);
}

lv_obj_t* StatusBar_GetObj(void)
{
    return objStatusBar;
}
