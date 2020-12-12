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
#ifndef __LV_OBJ_EXT_FUNC_H
#define __LV_OBJ_EXT_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

#define LV_ANIM_TIME_DEFAULT 300
#define LV_ANIM_EXEC(attr) (lv_anim_exec_xcb_t) lv_obj_set_##attr

void lv_label_set_text_add(lv_obj_t* label, const char* text);
void lv_table_set_align(lv_obj_t* table, lv_label_align_t align);

bool lv_obj_del_safe(lv_obj_t** obj);
void lv_obj_set_opa_scale(lv_obj_t* obj, lv_opa_t opa);
lv_opa_t lv_obj_get_opa_scale(lv_obj_t* obj);
void lv_obj_set_brightness(lv_obj_t* obj, int16_t value);
int16_t lv_obj_get_brightness(lv_obj_t* obj);
lv_coord_t lv_obj_get_x_center(lv_obj_t* obj);
lv_coord_t lv_obj_get_y_center(lv_obj_t* obj);
void lv_obj_add_anim(
    lv_obj_t* obj, lv_anim_t* a,
    lv_anim_exec_xcb_t exec_cb,
    int32_t start,
    int32_t end,
    uint16_t time,
    uint32_t delay,
    lv_anim_ready_cb_t ready_cb,
    lv_anim_path_cb_t path_cb);
#define LV_OBJ_ADD_ANIM(obj, attr, target, time)   \
    do {                                           \
        lv_obj_add_anim(                           \
            (obj), NULL,                           \
            (lv_anim_exec_xcb_t)lv_obj_set_##attr, \
            lv_obj_get_##attr(obj),                \
            (target),                              \
            (time),                                \
            0,                                     \
            NULL,                                  \
            lv_anim_path_ease_out);                \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif
