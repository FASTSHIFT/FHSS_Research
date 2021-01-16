#include "Page.h"

typedef struct {
    lv_obj_t* contStatusBar;

    struct {
        lv_obj_t* labelLeft;
        lv_obj_t* labelRight;
    }Info;
}WinObj_t;

static WinObj_t WinObj;

void StatusBar_Update()
{
    WinObj_t* win = &WinObj;

    uint32_t txCnt, rxCnt;
    ComTest_GetTxRxCnts(&txCnt, &rxCnt);
    lv_label_set_text_fmt(win->Info.labelLeft, "TX: %d -- RX: %d", txCnt, rxCnt);
}

static void StatusBar_EventHandler(lv_obj_t* obj, lv_event_t event)
{
    static uint32_t lastClickTime;
    static uint16_t longPressedCnt;
    
    WinObj_t* win = (WinObj_t*)lv_obj_get_user_data(obj);

    if(event == LV_EVENT_LONG_PRESSED)
    {
        ComTest_SetMaster(!ComTest_GetIsMaster());
    }
    else if (event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        longPressedCnt++;
    }
    else if (event == LV_EVENT_RELEASED)
    {
        if (longPressedCnt > 5)
        {
            ComTest_SetDuplexMode(!ComTest_GetIsDuplexMode());
        }

        longPressedCnt = 0;
    }
    else if (event == LV_EVENT_SHORT_CLICKED)
    {
        if (lv_tick_elaps(lastClickTime) < LV_INDEV_DEF_LONG_PRESS_TIME)
        {
            ComTest_SetFHSS_Enable(!ComTest_GetFHSS_Enable());
            lv_label_set_text(win->Info.labelRight, ComTest_GetFHSS_Enable() ? "FHSS" : "");
        }

        lastClickTime = lv_tick_get();
    }
}

lv_obj_t* StatusBar_Create()
{ 
    lv_obj_t* cont = lv_cont_create(lv_layer_top(), NULL);
    
    lv_obj_set_size(cont, LV_HOR_RES, 12);
    //lv_obj_set_style_default(cont);
    lv_obj_set_user_data(cont, &WinObj);
    lv_obj_set_click_anim_default(cont);
    lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_event_cb(cont, StatusBar_EventHandler);
    lv_obj_set_style_local_pad_all(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_cont_set_layout(cont, LV_LAYOUT_PRETTY_MID);

    WinObj.contStatusBar = cont;

    lv_obj_t* label = lv_label_create(cont, NULL);
    lv_label_set_text(label, "TX: 0 -- RX: 0");
    WinObj.Info.labelLeft = label;

    label = lv_label_create(cont, NULL);
    lv_label_set_text(label, "FHSS");
    WinObj.Info.labelRight = label;

    return cont;
}

lv_coord_t StatusBar_GetHeight()
{
    return lv_obj_get_height(WinObj.contStatusBar);
}

lv_obj_t* StatusBar_GetObj(void)
{
    return WinObj.contStatusBar;
}
