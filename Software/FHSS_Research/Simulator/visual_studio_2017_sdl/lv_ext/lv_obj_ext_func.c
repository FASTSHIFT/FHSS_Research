/*
 * Copyright (C) 2020 Xiaomi Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "lv_obj_ext_func.h"

/**
  * @brief  Safely delete the object, do not delete when the object is NULL
  * @param  obj:Object secondary address
  * @retval true success; false failure
  */
bool lv_obj_del_safe(lv_obj_t** obj)
{
    bool ret = false;
    if (*obj != NULL) {
        lv_obj_del(*obj);
        *obj = NULL;
        ret = true;
    }
    return ret;
}

void lv_obj_set_opa_scale(lv_obj_t* obj, lv_opa_t opa)
{
    lv_obj_set_style_local_opa_scale(obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, opa);
}

lv_opa_t lv_obj_get_opa_scale(lv_obj_t* obj)
{
    return lv_obj_get_style_opa_scale(obj, LV_OBJ_PART_MAIN);
}

void lv_obj_set_brightness(lv_obj_t* obj, int16_t value)
{
    lv_obj_set_style_local_bg_color(obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(value, value, value));
}

int16_t lv_obj_get_brightness(lv_obj_t* obj)
{
    lv_color_t color = lv_obj_get_style_bg_color(obj, LV_OBJ_PART_MAIN);
    return lv_color_brightness(color);
}

/**
  * @brief  Append string after label
  * @param  label:Object being appended
  * @param  text:Appended string
  * @retval None
  */
void lv_label_set_text_add(lv_obj_t* label, const char* text)
{
    if (!label) {
        return;
    }

    lv_label_ins_text(label, strlen(lv_label_get_text(label)), text);
}

/**
  * @brief  Get the X center coordinates of the object
  * @param  obj:Object address
  * @retval X center coordinate
  */
lv_coord_t lv_obj_get_x_center(lv_obj_t* obj)
{
    return (obj->coords.x2 + obj->coords.x1) / 2;
}

/**
  * @brief  Get the Y center coordinates of the object
  * @param  obj:Object address
  * @retval Y center coordinate
  */
lv_coord_t lv_obj_get_y_center(lv_obj_t* obj)
{
    return (obj->coords.y2 + obj->coords.y1) / 2;
}

/**
  * @brief  Set the alignment of all members of the table object
  * @param  table:Object address
  * @param  align:Align mode
  * @retval None
  */
void lv_table_set_align(lv_obj_t* table, lv_label_align_t align)
{
    uint16_t col = lv_table_get_col_cnt(table);
    uint16_t row = lv_table_get_row_cnt(table);
    for (uint16_t i = 0; i < col; i++) {
        for (uint16_t j = 0; j < row; j++) {
            lv_table_set_cell_align(table, j, i, align);
        }
    }
}

/**
  * @brief  Add animation to objects
  * @param  obj:Object address
  * @param  a:Animation controller address
  * @param  exec_cb:The address of the function that controls the properties of the object
  * @param  start:Start value of animation
  * @param  end:The end value of the animation
  * @param  time:Animation execution time
  * @param  delay:Delay time before the animation starts
  * @param  ready_cb:Animation end event callback
  * @param  path_cb:Animation curve
  * @retval None
  */
void lv_obj_add_anim(
    lv_obj_t* obj, lv_anim_t* a,
    lv_anim_exec_xcb_t exec_cb,
    int32_t start, int32_t end,
    uint16_t time,
    uint32_t delay,
    lv_anim_ready_cb_t ready_cb,
    lv_anim_path_cb_t path_cb)
{
    lv_anim_t anim_temp;

    if (a == NULL) {
        a = &anim_temp;

        /* INITIALIZE AN ANIMATION
        *-----------------------*/
        lv_anim_init(a);
    }

    /* MANDATORY SETTINGS
     *------------------*/

    /*Set the "animator" function*/
    lv_anim_set_exec_cb(a, exec_cb);

    /*Set the "animator" function*/
    lv_anim_set_var(a, obj);

    /*Length of the animation [ms]*/
    lv_anim_set_time(a, time);

    /*Set start and end values. E.g. 0, 150*/
    lv_anim_set_values(a, start, end);

    /* OPTIONAL SETTINGS
     *------------------*/

    /*Time to wait before starting the animation [ms]*/
    lv_anim_set_delay(a, delay);

    lv_anim_path_t path;
    lv_anim_path_init(&path);
    lv_anim_path_set_cb(&path, path_cb);

    /*Set the path in an animation*/
    lv_anim_set_path(a, &path);

    /*Set a callback to call when animation is ready.*/
    lv_anim_set_ready_cb(a, ready_cb);

    /*Set a callback to call when animation is started (after delay).*/
    lv_anim_set_start_cb(a, ready_cb);

    /* START THE ANIMATION
     *------------------*/
    lv_anim_start(a); /*Start the animation*/
}
