#include "BSP.h"
#include "Basic/CommonMacro.h"

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
    SysLog_Printf("&btn = 0x%p, event = %d", btn, event);
}

/**
  * @brief  按键初始化
  * @param  无
  * @retval 无
  */
void Button_Init()
{
    SysLog_Printf("KEY_Pin = %d, &btOK = 0x%p", KEY_Pin, &btOK);
    
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
    __IntervalExecute(btOK.EventMonitor(!digitalRead(KEY_Pin)), 10);
}
