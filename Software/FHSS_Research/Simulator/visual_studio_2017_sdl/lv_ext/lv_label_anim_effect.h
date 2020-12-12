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
#ifndef __LV_LABEL_ANIM_EFFECT_H
#define __LV_LABEL_ANIM_EFFECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

typedef struct {
    lv_obj_t* label_1;
    lv_obj_t* label_2;
    lv_anim_t anim_now;
    lv_anim_t anim_next;
    lv_coord_t y_offset;
    uint8_t value_last;
} lv_label_anim_effect_t;

void lv_label_anim_effect_init(
    lv_label_anim_effect_t* effect,
    lv_obj_t* cont,
    lv_obj_t* label_copy,
    uint16_t anim_time);
void lv_label_anim_effect_check_value(
    lv_label_anim_effect_t* effect,
    uint8_t value,
    lv_anim_enable_t anim_enable);

#ifdef __cplusplus
}
#endif

#endif
