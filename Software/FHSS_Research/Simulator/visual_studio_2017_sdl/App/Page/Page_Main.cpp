#include "Page.h"

static lv_obj_t* labelGrp[4];
static lv_obj_t* chartRx;
static lv_chart_series_t* chartRxSer;

static void LabelGrp_Update(lv_task_t* task)
{
    uint32_t rxSuc, forceFH, resync;
    ComSlave_GetCnts(&rxSuc, &forceFH, &resync);
    
    lv_label_set_text_fmt(labelGrp[0], "RX:%d", rxSuc);
    lv_label_set_text_fmt(labelGrp[1], "fFH:%d", forceFH);
    lv_label_set_text_fmt(labelGrp[2], "sync:%d", resync);
    lv_label_set_text_fmt(labelGrp[3], "%d%%", ComSlave_GetPackLoss());
    
    ComSlave_ResetCnts();
}

static void Chart_Update(lv_task_t* task)
{
    lv_coord_t y = ComTest_GetIsMaster() ? ComTest_GetTxData(0) : ComTest_GetRxData(0);
    lv_chart_set_next(chartRx, chartRxSer, y);
}

static void Chart_Create(lv_obj_t* par)
{
    lv_obj_t* chart = lv_chart_create(par, NULL);
    lv_obj_set_size(chart, lv_obj_get_width(par), lv_obj_get_height(par));
    lv_obj_align(chart, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_range(chart, 0, 255);
    lv_chart_set_point_count(chart, 100);
    
    lv_obj_set_style_local_size(chart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 0);
    
    chartRx = chart;
    
    lv_chart_series_t* ser = lv_chart_add_series(chart, LV_COLOR_RED);
    
    chartRxSer = ser;
}

static void LabelGrp_Create(lv_obj_t* par)
{
    lv_obj_t* label = lv_label_create(par, NULL);
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_obj_set_auto_realign(label, true);
    labelGrp[0] = label;
    
    label = lv_label_create(par, label);
    lv_obj_align(label, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
    labelGrp[1] = label;
    
    label = lv_label_create(par, label);
    lv_obj_align(label, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    labelGrp[2] = label;
    
    label = lv_label_create(par, label);
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
    labelGrp[3] = label;
}

lv_obj_t* Page_ComSlaveTest_Create()
{
    lv_obj_t* scr = lv_obj_create(NULL, NULL);
    lv_obj_set_height(scr, LV_VER_RES - StatusBar_GetHeight());
    lv_obj_set_y(scr, StatusBar_GetHeight());
    
    Chart_Create(scr);
    LabelGrp_Create(scr);
    
    lv_task_create(LabelGrp_Update, 3000, LV_TASK_PRIO_MID, NULL);
    lv_task_create(Chart_Update, 20, LV_TASK_PRIO_MID, NULL);
    
    return scr;
}
