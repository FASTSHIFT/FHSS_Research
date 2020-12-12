#include "BSP.h"

/*实例化按键对象*/
ButtonEvent btOK;              //选择键

/**
  * @brief  按键事件回调处理
  * @param  btn:按键对象地址
  * @param  event:事件类型
  * @retval 无
  */
static void Button_EventHandler(ButtonEvent* btn, int event)
{
    
}

/**
  * @brief  按键初始化
  * @param  无
  * @retval 无
  */
void Button_Init()
{
    /*上拉输入*/
    pinMode(KEY_Pin, INPUT_PULLUP);

    
    /*关联事件*/
    btOK.EventAttach(Button_EventHandler);
}

/**
  * @brief  按键监控更新
  * @param  无
  * @retval 无
  */
void Button_Update()
{
    btOK.EventMonitor(!digitalRead(KEY_Pin));
}
