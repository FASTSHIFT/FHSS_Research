#include "BSP.h"
#include "Basic/CommonMacro.h"

/*ʵ������������*/
ButtonEvent btOK;              //ѡ���

/**
  * @brief  �����¼��ص�����
  * @param  btn:���������ַ
  * @param  event:�¼�����
  * @retval ��
  */
static void Button_EventHandler(ButtonEvent* btn, int event)
{
    SysLog_Printf("&btn = 0x%p, event = %d", btn, event);
}

/**
  * @brief  ������ʼ��
  * @param  ��
  * @retval ��
  */
void Button_Init()
{
    SysLog_Printf("KEY_Pin = %d, &btOK = 0x%p", KEY_Pin, &btOK);
    
    /*��������*/
    pinMode(KEY_Pin, INPUT_PULLUP);

    
    /*�����¼�*/
    btOK.EventAttach(Button_EventHandler);
}

/**
  * @brief  ������ظ���
  * @param  ��
  * @retval ��
  */
void Button_Update()
{
    __IntervalExecute(btOK.EventMonitor(!digitalRead(KEY_Pin)), 10);
}
