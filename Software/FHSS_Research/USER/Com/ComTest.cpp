#include "ComPrivate.h"
#include "Basic/CommonMacro.h"

static bool Com_IsMaster = true;

/*NRF1发送数据缓冲区*/
uint8_t NRF1_TxBuff[COM_BUFFER_LEN];

/*NRF1接收数据缓冲区*/
uint8_t NRF1_RxBuff[COM_BUFFER_LEN];

/*NRF2发送数据缓冲区*/
uint8_t NRF2_TxBuff[COM_BUFFER_LEN];

/*NRF2接收数据缓冲区*/
uint8_t NRF2_RxBuff[COM_BUFFER_LEN];

uint8_t FreqHoppingList[] = {00, 05, 10, 15, 20, 35, 40, 45, 50};

/*跳频收发管理器*/
NRF_Manager nrf1_Mag(&nrf1);

uint8_t ComTest_GetTxData(int index)
{
    if(index >= sizeof(NRF1_TxBuff))
        return 0;
    
    return NRF1_TxBuff[index];
}

uint8_t ComTest_GetRxData(int index)
{
    if(index >= sizeof(NRF1_RxBuff))
        return 0;
    
    return NRF1_RxBuff[index];
}

bool ComTest_GetIsMaster()
{
    return Com_IsMaster;
}

void ComTest_SetMaster(bool en)
{
    TIM_Cmd(COM_TIM, DISABLE);
    Com_IsMaster = en;
    if(Com_IsMaster)
    {
        nrf1_Mag.SetRole(NRF_Manager::Role_Master);
        Timer_SetInterruptTimeUpdate(COM_TIM, 10 * 1000);
    }
    else
    {
        nrf1_Mag.SetRole(NRF_Manager::Role_Slave);
        Timer_SetInterruptTimeUpdate(COM_TIM, 1000);
    }
    TIM_Cmd(COM_TIM, ENABLE);
}

static void IRQ_Handler()
{
    bool state = digitalRead_FAST(NRF1_IRQ_Pin);
    
    LED_SetEnable(LED_TX, true);
    
    if(state == LOW)
    {
        nrf1_Mag.IRQ_Handle();
    }
    
    LED_SetEnable(LED_TX, false);
}

void ComTest_Init()
{
    SysLog_Printf("");

    nrf1.SetAutoRetry(0, 15);
    nrf1.SetAutoAck(true);
    nrf1.SetIRQEnable(false, false, true);
    
    attachInterrupt(NRF1_IRQ_Pin, IRQ_Handler, CHANGE);

    nrf1_Mag.SetTxBuffer(NRF1_TxBuff, sizeof(NRF1_TxBuff));
    nrf1_Mag.SetRxBuffer(NRF1_RxBuff, sizeof(NRF1_RxBuff));
    nrf1_Mag.SetMode(NRF_Manager::Mode_Simplex);
    
    nrf1_Mag.SetFreqHoppingList(FreqHoppingList, sizeof(FreqHoppingList));
    
    Timer_SetInterrupt(COM_TIM, 10 * 1000, ComTset_Update);
}

void ComTset_Update()
{
    if(Com_IsMaster)
    {
        ComMasterTest();
    }
    else
    {
        ComSlaveTest();
    }
}
