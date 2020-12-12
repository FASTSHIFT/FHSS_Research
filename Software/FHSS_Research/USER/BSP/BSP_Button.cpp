#include "BSP.h"

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
    
}

/**
  * @brief  ������ʼ��
  * @param  ��
  * @retval ��
  */
void Button_Init()
{
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
    btOK.EventMonitor(!digitalRead(KEY_Pin));
}
