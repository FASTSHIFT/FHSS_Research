/**
 * @file lv_port_indev_templ.c
 *
 */

/*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "../DisplayPrivate.h"

/*********************
 *      DEFINES
 *********************/
 
#ifndef ARRAY_SIZE
#  define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#endif

/**********************
 *      TYPEDEFS
 **********************/

typedef struct
{
    uint8_t pin;
    lv_point_t point;
} button_map_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void button_init(void);
static bool button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static int8_t button_get_pressed_id(void);
static bool button_is_pressed(uint8_t id);

/**********************
 *  STATIC VARIABLES
 **********************/

static lv_indev_t * indev_button;

static const button_map_t button_map[] =
{
    {KEY_Pin, {10, 10}},
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void)
{
    lv_indev_drv_t indev_drv;

    /*------------------
     * Button
     * -----------------*/

    /*Initialize your button if you have*/
    button_init();

    /*Register a button input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_BUTTON;
    indev_drv.read_cb = button_read;
    indev_button = lv_indev_drv_register(&indev_drv);

    /*Assign buttons to points on the screen*/
    static lv_point_t btn_points[ARRAY_SIZE(button_map)];
    
    for(int i = 0; i < ARRAY_SIZE(btn_points); i++)
    {
        btn_points[i].x = button_map[i].point.x;
        btn_points[i].y = button_map[i].point.y;
    }
    
    lv_indev_set_button_points(indev_button, btn_points);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Button
 * -----------------*/

/* Initialize your buttons */
static void button_init(void)
{
    /*Your code comes here*/
    for(int i = 0; i < ARRAY_SIZE(button_map); i++)
    {
        pinMode(button_map[i].pin, INPUT_PULLUP);
    }
}

/* Will be called by the library to read the button */
static bool button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{

    static uint8_t last_btn = 0;

    /*Get the pressed button's ID*/
    int8_t btn_act = button_get_pressed_id();

    if(btn_act >= 0)
    {
        data->state = LV_INDEV_STATE_PR;
        last_btn = btn_act;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Save the last pressed button's ID*/
    data->btn_id = last_btn;

    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}

/*Get ID  (0, 1, 2 ..) of the pressed button*/
static int8_t button_get_pressed_id(void)
{
    uint8_t i;

    /*Check to buttons see which is being pressed (assume there are 2 buttons)*/
    for(i = 0; i < ARRAY_SIZE(button_map); i++)
    {
        /*Return the pressed button's ID*/
        if(button_is_pressed(i))
        {
            return i;
        }
    }

    /*No button pressed*/
    return -1;
}

/*Test if `id` button is pressed or not*/
static bool button_is_pressed(uint8_t id)
{

    /*Your code comes here*/

    return !digitalRead(button_map[id].pin);
}

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
