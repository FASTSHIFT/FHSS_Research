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
#include "lv_anim_timeline.h"

static void lv_anim_timeline_add(const lv_anim_timeline_t* anim_timeline, bool playback, uint32_t playtime)
{
    lv_obj_t* obj = anim_timeline->obj;
    uint32_t delay = anim_timeline->start_time;

    lv_anim_exec_xcb_t exec_cb = anim_timeline->exec_cb;
    uint16_t time = anim_timeline->duration;

    int32_t start = anim_timeline->start;
    int32_t end = anim_timeline->end;
    if (playback) {
        int32_t temp = start;
        start = end;
        end = temp;

        delay = playtime - (delay + time);
    }

    lv_anim_path_t path;
    lv_anim_path_init(&path);
    lv_anim_path_set_cb(&path, anim_timeline->path_cb);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_delay(&a, delay);
    lv_anim_set_exec_cb(&a, exec_cb);
    lv_anim_set_time(&a, time);
    lv_anim_set_values(&a, start, end);
    lv_anim_set_path(&a, &path);

    lv_anim_start(&a);
}

uint32_t lv_anim_timeline_start(const lv_anim_timeline_t* anim_timeline, uint32_t len, bool playback)
{
    uint32_t playtime = lv_anim_timeline_get_playtime(anim_timeline, len);
    for (uint32_t i = 0; i < len; i++) {
        lv_anim_timeline_add(&(anim_timeline[i]), playback, playtime);
    }
    return playtime;
}

void lv_anim_timeline_del(const lv_anim_timeline_t* anim_timeline, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        lv_obj_t* obj = anim_timeline[i].obj;
        lv_anim_exec_xcb_t exec_xcb = anim_timeline[i].exec_cb;
        lv_anim_del(obj, exec_xcb);
    }
}

uint32_t lv_anim_timeline_get_playtime(const lv_anim_timeline_t* anim_timeline, uint32_t len)
{
    uint32_t playtime = 0;
    for (uint32_t i = 0; i < len; i++) {
        uint32_t end = anim_timeline[i].start_time + anim_timeline[i].duration;
        if (end > playtime) {
            playtime = end;
        }
    }
    return playtime;
}
