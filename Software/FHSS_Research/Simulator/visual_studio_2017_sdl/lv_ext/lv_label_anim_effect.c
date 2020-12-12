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
#include "lv_label_anim_effect.h"
#include <stdlib.h>

void lv_label_anim_effect_init(
    lv_label_anim_effect_t* effect,
    lv_obj_t* cont,
    lv_obj_t* label_copy,
    uint16_t anim_time)
{
    lv_obj_t* label = lv_label_create(cont, label_copy);
    effect->y_offset = (lv_obj_get_height(cont) - lv_obj_get_height(label_copy)) / 2 + 1;
    lv_obj_align(label, label_copy, LV_ALIGN_OUT_BOTTOM_MID, 0, effect->y_offset);
    effect->label_1 = label_copy;
    effect->label_2 = label;

    effect->value_last = 0;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_time(&a, anim_time);
    lv_anim_set_delay(&a, 0);

    lv_anim_path_t path;
    lv_anim_path_init(&path);
    lv_anim_path_set_cb(&path, lv_anim_path_ease_out);
    lv_anim_set_path(&a, &path);

    effect->anim_now = a;
    effect->anim_next = a;
}

void lv_label_anim_effect_check_value(lv_label_anim_effect_t* effect, uint8_t value, lv_anim_enable_t anim_enable)
{
    /*Don't switch if the values are equal*/
    if (value == effect->value_last)
        return;

    if (effect->label_1 == NULL || effect->label_2 == NULL) {
        return;
    }

    if (anim_enable == LV_ANIM_ON) {
        lv_obj_t* next_label;
        lv_obj_t* now_label;
        /*Determine the relative position of the two labels and determine who is the next label*/
        if (lv_obj_get_y(effect->label_2) > lv_obj_get_y(effect->label_1)) {
            now_label = effect->label_1;
            next_label = effect->label_2;
        } else {
            now_label = effect->label_2;
            next_label = effect->label_1;
        }

        lv_label_set_text_fmt(now_label, "%d", effect->value_last);
        lv_label_set_text_fmt(next_label, "%d", value);
        effect->value_last = value;
        /*Align*/
        lv_obj_align(next_label, now_label, LV_ALIGN_OUT_TOP_MID, 0, -effect->y_offset);
        /*Calculate the required Y offset*/
        lv_coord_t y_offset = abs(lv_obj_get_y(now_label) - lv_obj_get_y(next_label));

        /*Sliding animation*/
        lv_anim_t* a;
        a = &(effect->anim_now);
        lv_anim_set_var(a, now_label);
        lv_anim_set_values(a, lv_obj_get_y(now_label), lv_obj_get_y(now_label) + y_offset);
        lv_anim_start(a);

        a = &(effect->anim_next);
        lv_anim_set_var(a, next_label);
        lv_anim_set_values(a, lv_obj_get_y(next_label), lv_obj_get_y(next_label) + y_offset);
        lv_anim_start(a);
    } else {
        lv_label_set_text_fmt(effect->label_1, "%d", value);
        lv_label_set_text_fmt(effect->label_2, "%d", value);
        effect->value_last = value;
    }
}
