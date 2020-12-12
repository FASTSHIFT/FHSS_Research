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
#ifndef __LV_ANIM_TIMELINE_H
#define __LV_ANIM_TIMELINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

typedef struct {
    uint32_t start_time;
    lv_obj_t* obj;

    lv_anim_exec_xcb_t exec_cb;
    int32_t start;
    int32_t end;
    uint16_t duration;
    lv_anim_path_cb_t path_cb;
} lv_anim_timeline_t;

#define LV_ANIM_TIMELINE_ELEM_DEF(obj, start_time, attr, end, duration) \
    {                                                                   \
        (start_time),                                                   \
            (obj),                                                      \
            (lv_anim_exec_xcb_t)lv_obj_set_##attr,                      \
            lv_obj_get_##attr(obj),                                     \
            (end),                                                      \
            (duration),                                                 \
            lv_anim_path_ease_in_out                                    \
    }

uint32_t lv_anim_timeline_start(const lv_anim_timeline_t* anim_timeline, uint32_t len, bool playback);
void     lv_anim_timeline_del(const lv_anim_timeline_t* anim_timeline, uint32_t len);
uint32_t lv_anim_timeline_get_playtime(const lv_anim_timeline_t* anim_timeline, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif // ! __LV_ANIM_TIMELINE_H
