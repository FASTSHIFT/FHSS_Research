#include "lv_auto_event.h"

static void lv_auto_event_task_handler(lv_task_t* task)
{
    lv_auto_event_t* ae = (lv_auto_event_t*)(task->user_data);

    lv_event_send(
        *(ae->auto_event_data[ae->run_index].obj_p),
        ae->auto_event_data[ae->run_index].event,
        ae->auto_event_data[ae->run_index].user_data
    );

    ae->run_index++;

    if (ae->run_index < ae->len)
    {
        lv_task_set_period(task, ae->auto_event_data[ae->run_index].delay);
    }
    else
    {
        lv_task_del(task);
        lv_mem_free(ae);
    }
}

lv_auto_event_t* lv_auto_event_create(lv_auto_event_data_t* auto_event_data, uint32_t len)
{
    lv_auto_event_t* ae = lv_mem_alloc(sizeof(lv_auto_event_t));
    ae->auto_event_data = auto_event_data;
    ae->len = len;
    ae->run_index = 0;
    ae->task = lv_task_create(lv_auto_event_task_handler, auto_event_data[0].delay, LV_TASK_PRIO_MID, ae);
    return ae;
}

void lv_auto_event_del(lv_auto_event_t* ae)
{
    lv_task_del(ae->task);
    lv_mem_free(ae);
}
