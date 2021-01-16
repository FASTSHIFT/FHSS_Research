#include "ComPrivate.h"
#include "Basic/CommonMacro.h"

/*NRF1发送数据缓冲区*/
uint8_t NRF1_TxBuff[COM_BUFFER_LEN];

/*NRF1接收数据缓冲区*/
uint8_t NRF1_RxBuff[COM_BUFFER_LEN];

/*NRF2发送数据缓冲区*/
uint8_t NRF2_TxBuff[COM_BUFFER_LEN];

/*NRF2接收数据缓冲区*/
uint8_t NRF2_RxBuff[COM_BUFFER_LEN];

uint8_t FreqHoppingList[] = {50, 60, 70, 80, 90, 100, 110, 120, 125};

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
    return (nrf1_Mag.Role == NRF_Manager::Role_Master);
}

void ComTest_SetMaster(bool en)
{
    nrf1_Mag.SetRole(en ? NRF_Manager::Role_Master : NRF_Manager::Role_Slave);
}

void ComTest_GetTxRxCnts(uint32_t* txCnt, uint32_t* rxCnt)
{
    *txCnt = nrf1.CntTxSuccess;

    uint32_t temp;
    nrf1_Mag.GetRxCnts(rxCnt, &temp, &temp);
}

void ComTest_GetPackLoss(uint8_t* txLoss, uint8_t* rxLoss)
{
    *txLoss = nrf1.GetTxPackLoss();
    *rxLoss = nrf1_Mag.GetRxPackLoss();
}

bool ComTest_GetFHSS_Enable()
{
    return nrf1_Mag.GetFH_Enable();
}

void ComTest_SetFHSS_Enable(bool en)
{
    nrf1_Mag.SetFH_Enable(en);
}

bool ComTest_GetIsDuplexMode()
{
    return (nrf1_Mag.Mode == NRF_Manager::Mode_Duplex);
}

void ComTest_SetDuplexMode(bool en)
{
    nrf1_Mag.SetMode(en ? NRF_Manager::Mode_Duplex : NRF_Manager::Mode_Simplex);
}

void ComSlave_GetCnts(uint32_t* rxSuc, uint32_t* forceFH, uint32_t* resync)
{
    nrf1_Mag.GetRxCnts(rxSuc, forceFH, resync);
}

static void ComTset_BuffUpdate()
{
    if(ComTest_GetIsMaster())
    {
        NRF1_TxBuff[0]++;
    }
    else
    {
        NRF1_TxBuff[0] = 0xFF - NRF1_RxBuff[0];
    }
}

static void ComTset_Update()
{
    nrf1_Mag.Handle();
    __IntervalExecute(ComTset_BuffUpdate(), 10);
}

void ComTest_Init()
{
    SysLog_Printf("");

    nrf1.SetAutoRetry(0, 15);
    nrf1.SetAutoAck(true);

    nrf1_Mag.SetTxBuffer(NRF1_TxBuff, sizeof(NRF1_TxBuff));
    nrf1_Mag.SetRxBuffer(NRF1_RxBuff, sizeof(NRF1_RxBuff));
    nrf1_Mag.SetMode(NRF_Manager::Mode_Simplex);
    nrf1_Mag.SetFreqHoppingList(FreqHoppingList, sizeof(FreqHoppingList));
    
    Timer_SetInterrupt(COM_TIM, 100, ComTset_Update);
    TIM_Cmd(COM_TIM, ENABLE);
}

void ComTest_ResetCnts()
{
    nrf1_Mag.ResetRxCnts();
    nrf1.ResetTxCnts();
}
