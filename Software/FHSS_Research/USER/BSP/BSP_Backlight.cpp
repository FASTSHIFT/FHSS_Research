#include "Display/DisplayPrivate.h"
#include "BSP.h"

#define BLK_Pin SCREEN_BLK_Pin

/**
  * @brief  �������Ƚ��䣬��lv_anim����
  * @param  obj:����
  * @param  brightness:����ֵ
  * @retval None
  */
static void Backlight_AnimCallback(void * obj, int16_t brightness)
{
    Backlight_SetValue(brightness);
}

/**
  * @brief  �����ʼ��
  * @param  ��
  * @retval ��
  */
void Backlight_Init()
{
    /*PWM��ʼ����1000����20KƵ��*/
    //PWM_Init(BLK_Pin, 1000, 20000);
    pinMode(BLK_Pin, OUTPUT);
}

/**
  * @brief  �������ã�����Ч��
  * @param  target:Ŀ������(0~1000 -> 0~100%)
  * @retval ��
  */
void Backlight_SetGradual(uint16_t target, uint16_t time)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)Backlight_AnimCallback);
    lv_anim_set_var(&a, NULL);
    lv_anim_set_time(&a, time);
    lv_anim_set_values(&a, Backlight_GetValue(), target);

    lv_anim_path_t path;
    lv_anim_path_init(&path);
    lv_anim_path_set_cb(&path, lv_anim_path_ease_in_out);
    lv_anim_set_path(&a, &path);
    lv_anim_start(&a); 
}

/**
  * @brief  ��ȡ��������
  * @param  ��
  * @retval ��ǰ����(0~1000 -> 0~100%)
  */
uint16_t Backlight_GetValue()
{
    return timer_get_compare(PIN_MAP[BLK_Pin].TIMx, PIN_MAP[BLK_Pin].TimerChannel);
}

/**
  * @brief  ���ñ�������
  * @param  val: ����(0~1000 -> 0~100%)
  * @retval ��
  */
void Backlight_SetValue(int16_t val)
{
    digitalWrite(BLK_Pin, val);
    //val = constrain(val, 0, 1000);
    //analogWrite(BLK_Pin, val);
}
