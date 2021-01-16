#include "Page.h"

typedef struct {
    lv_obj_t* scr;

    struct {
        lv_obj_t* labelLeft;
        lv_obj_t* labelRight;
    }Info;

    lv_obj_t* labelGrp[4];

    struct {
        lv_obj_t* chart;
        lv_chart_series_t* ser_tx;
        lv_chart_series_t* ser_rx;
    }chart;
}WinObj_t;

static void LabelGrp_Update(lv_task_t* task)
{
    WinObj_t* win = (WinObj_t*)task->user_data;

    uint32_t rxSuc, forceFH, resync;
    ComSlave_GetCnts(&rxSuc, &forceFH, &resync);

    uint8_t txLoss, rxLoss;
    ComTest_GetPackLoss(&txLoss, &rxLoss);
    
    if (ComTest_GetIsMaster())
    {
        lv_label_set_text_fmt(
            win->Info.labelLeft,
            "Lost: %d%%",
            txLoss
        );
    }
    else
    {
        lv_label_set_text_fmt(
            win->Info.labelLeft,
            "Lost: %d%%\n"
            "FFH: %d\n"
            "ReSync: %d",
            rxLoss,
            forceFH,
            resync
        );
    }
    
    StatusBar_Update();
    
    ComTest_ResetCnts();
}

static void Chart_Update(lv_task_t* task)
{
    WinObj_t* win = (WinObj_t*)task->user_data;

    lv_chart_set_next(win->chart.chart, win->chart.ser_tx, ComTest_GetTxData(0));
    lv_chart_set_next(win->chart.chart, win->chart.ser_rx, ComTest_GetRxData(0));

    const char* role = ComTest_GetIsMaster() ? "#FF0000 Master#" : "#0000FF Slave#";
    const char* mode = ComTest_GetIsDuplexMode() ? "Duplex" : "Simplex";
    lv_label_set_text_fmt(win->Info.labelRight, "%s\n%s", role, mode);
}

static void Chart_Create(WinObj_t* win)
{
    lv_obj_t* par = win->scr;

    lv_obj_t* chart = lv_chart_create(par, NULL);
    lv_obj_set_size(chart, lv_obj_get_width(par), lv_obj_get_height(par));
    lv_obj_align(chart, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_range(chart, 0, 255);
    lv_chart_set_point_count(chart, 100);
    
    lv_obj_set_style_local_size(chart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 0);
    
    win->chart.chart = chart;
    
    win->chart.ser_tx = lv_chart_add_series(chart, LV_COLOR_RED);
    win->chart.ser_rx = lv_chart_add_series(chart, LV_COLOR_BLUE);
}

static void LabelGrp_Create(WinObj_t* win)
{
    lv_obj_t* par = win->scr;

    lv_obj_t* label = lv_label_create(par, NULL);
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_obj_set_auto_realign(label, true);
    lv_label_set_recolor(label, true);
    lv_label_set_text(label, "");
    win->Info.labelLeft = label;
    
    label = lv_label_create(par, label);
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
    lv_label_set_align(label, LV_LABEL_ALIGN_RIGHT);
    win->Info.labelRight = label;
}

lv_obj_t* Page_ComSlaveTest_Create()
{
    static WinObj_t win;

    lv_obj_t* scr = lv_obj_create(NULL, NULL);
    lv_obj_set_height(scr, LV_VER_RES - StatusBar_GetHeight());
    lv_obj_set_y(scr, StatusBar_GetHeight());

    win.scr = scr;
    
    Chart_Create(&win);
    LabelGrp_Create(&win);
    
    lv_task_create(LabelGrp_Update, 2000, LV_TASK_PRIO_MID, &win);
    lv_task_create(Chart_Update, 20, LV_TASK_PRIO_MID, &win);
    
    return scr;
}
