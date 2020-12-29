#include "Page.h"

void lv_obj_set_style_default(lv_obj_t* obj)
{
    static lv_style_t style = { NULL };

    if (style.map == NULL)
    {
        lv_style_init(&style);
        lv_style_set_bg_color(&style, LV_OBJ_PART_MAIN, LV_COLOR_BLACK);
        lv_style_set_border_width(&style, LV_OBJ_PART_MAIN, 0);
        lv_style_set_radius(&style, LV_OBJ_PART_MAIN, 0);
    }

    lv_obj_add_style(obj, LV_OBJ_PART_MAIN, &style);
}

void lv_obj_set_click_anim_default(lv_obj_t* obj)
{
    static lv_style_t style;

    if (style.map == NULL)
    {
        lv_style_init(&style);
        lv_style_set_radius(&style, LV_STATE_DEFAULT, 0);
        lv_style_set_border_width(&style, LV_STATE_DEFAULT, 0);
        lv_style_set_bg_color(&style, LV_STATE_PRESSED, LV_COLOR_GRAY);

        static lv_anim_path_t path_ease_in_out;
        lv_anim_path_init(&path_ease_in_out);
        lv_anim_path_set_cb(&path_ease_in_out, lv_anim_path_ease_in_out);

        lv_style_set_transition_path(&style, LV_STATE_PRESSED, &path_ease_in_out);
        lv_style_set_transition_path(&style, LV_STATE_DEFAULT, &path_ease_in_out);

        lv_style_set_transition_time(&style, LV_STATE_DEFAULT, 200);
        lv_style_set_transition_prop_1(&style, LV_STATE_DEFAULT, LV_STYLE_BG_COLOR);
    }

    lv_obj_add_style(obj, LV_OBJ_PART_MAIN, &style);
}
