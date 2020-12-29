#include "NRF_Manager.h"
#include "BSP/BSP_Hardware.h"

NRF_Manager::NRF_Manager(NRF_Basic* nrf)
{
    Basic = nrf;

    TxBuffer = NULL;
    RxBuffer = NULL;

    IsHandling = false;

    FH_Enable = true;
    FH_WaitResync = true;
    FH_List = NULL;
    FH_List_Length = 0;
    FH_List_Index = 0;
    FH_LastTime = 0;
    FH_WaitTime = 0;

    LastRxTime = 0;
    IntervalTime = 10;
    
    Role = Role_Master;
    Mode = Mode_Simplex;
    ResetRxCnts();
}

void NRF_Manager::SetFH_Enable(bool en)
{
    FH_Enable = en;
    
    if(!FH_Enable && FH_List != NULL)
    {
        FH_List_Index = 0;
        Basic->SetFreqency(FH_List[FH_List_Index]);
    }
}

void NRF_Manager::SetRole(Role_Type role)
{
    Role = role;
    
    if(Role == Role_Master)
    {
        Basic->TX_Mode(true);
    }
    else if(Role == Role_Slave)
    {
        Basic->RX_Mode(true);
    }
}

void NRF_Manager::SetMode(Mode_Type mode)
{
    Mode = mode;
}

void NRF_Manager::SetTxBuffer(const void* txbuff, uint16_t len)
{
    if(len == Basic->RF_TX_PloadWidth)
    {
        TxBuffer = (const uint8_t*)txbuff;
    }
}

void NRF_Manager::SetRxBuffer(void* rxbuff, uint16_t len)
{
    if(len == Basic->RF_RX_PloadWidth)
    {
        RxBuffer = (uint8_t*)rxbuff;
    }
}

void NRF_Manager::SetFreqHoppingList(const uint8_t* list, uint16_t length)
{
    FH_List = list;
    FH_List_Length = length;
    FH_JumpNext();
}

void NRF_Manager::GetRxCnts(uint32_t* rxSuc, uint32_t* forceFH, uint32_t* resync)
{
    *rxSuc = CntRxSuccess;
    *forceFH = CntRxForceFH;
    *resync = CntRxResync;
}

void NRF_Manager::ResetRxCnts()
{
    CntRxSuccess = 0;
    CntRxForceFH = 0;
    CntRxResync = 0;
    CntRxCom = 0;
}

uint8_t NRF_Manager::GetRxPackLoss()
{
    if(CntRxCom == 0) return 0;

    return (100 - 100 * CntRxSuccess / CntRxCom);
}

/**
  * @brief  跳频处理
  * @param  无
  * @retval 无
  */
void NRF_Manager::FH_JumpNext()
{
    if(!FH_Enable)
    {
        return;
    }

    LED_SetEnable(LED_STA1, true);
    FH_List_Index++;
    FH_List_Index %= FH_List_Length;
    Basic->SetFreqency(FH_List[FH_List_Index]);
    LED_SetEnable(LED_STA1, false);
}

void NRF_Manager::MasterSimplexHandle()
{
    /*检查上一次的发送情况*/
    Basic->TranCheck();
    /*发射端跳频*/
    FH_JumpNext();
    /*再发送*/
    Basic->Tran(TxBuffer);
    /*不管接收端状态*/
}

void NRF_Manager::SlaveSimplexHandle()
{
    uint32_t tick = millis();

    /*如果正常接收，则下一次正常跳频*/
    if(Basic->Recv(RxBuffer))
    {
        CntRxCom++;
        CntRxSuccess++;
        LastRxTime = tick;
        FH_LastTime = tick;
        FH_WaitTime = IntervalTime + IntervalTime / 2 + 1;
        LED_SetEnable(LED_RX, true);
        FH_JumpNext();
        FH_WaitResync = false;
        LED_SetEnable(LED_RX, false);
    }
    else if(!FH_WaitResync && tick - FH_LastTime >= FH_WaitTime)
    {
        CntRxCom++;
        CntRxForceFH++;
        FH_LastTime = tick;
        FH_WaitTime = IntervalTime;
        LED_SetEnable(LED_STA2, true);
        FH_JumpNext();
        LED_SetEnable(LED_STA2, false);
    }
    /*在丢失信号超时后，接收端关闭正常跳频*/
    /*慢跳等待重新同步发射端，慢跳周期大于发端遍历跳频表的周期*/
    /*一旦接收成功，立即启动正常跳频*/
    else if(millis() - LastRxTime >= IntervalTime * (FH_List_Length + 1))
    {
        CntRxCom++;
        CntRxResync++;
        LastRxTime = millis();
        FH_JumpNext();
        FH_WaitResync = true;
    }
}


void NRF_Manager::MasterDuplexHandle()
{
    uint8_t status = Basic->GetStatus();
    if(Basic->RF_State == Basic->RF_State_RX)
    {
        Basic->Recv(RxBuffer);
        FH_JumpNext();
        Basic->TX_Mode();
        Basic->Tran(TxBuffer);
    }
    else if(Basic->RF_State == Basic->RF_State_TX)
    {
        if(status & Basic->TX_DS)
        {
            Basic->CntTxSuccess++;
        }
        Basic->RX_Mode();
    }
}

void NRF_Manager::SlaveDuplexHandle()
{
    uint8_t status = Basic->GetStatus();
    if(Basic->RF_State == Basic->RF_State_TX)
    {
        if(status & Basic->TX_DS)
        {
            Basic->CntTxSuccess++;
        }

        if(!FH_WaitResync)
        {
            FH_JumpNext();
        }
        Basic->RX_Mode();
    }
    else if(Basic->RF_State == Basic->RF_State_RX)
    {
        if(Basic->Recv(RxBuffer))
        {
            LastRxTime = millis();
            FH_WaitResync = false;
            Basic->TX_Mode();
            Basic->Tran(TxBuffer);
        }
        else
        {
            if(millis() - LastRxTime >= IntervalTime * (FH_List_Length + 1))
            {
                LastRxTime = millis();
                FH_JumpNext();
                FH_WaitResync = true;
            }
            Basic->RX_Mode();
        }
    }
}

void NRF_Manager::Handle()
{
    /*防止函数重入*/
    if(IsHandling)
        return;

    IsHandling = true;

    if(Role == Role_Master)
    {
        if(Mode == Mode_Simplex && TxBuffer != NULL)
        {
            MasterSimplexHandle();
        }
        else if(Mode == Mode_Duplex && TxBuffer != NULL && RxBuffer != NULL)
        {
            MasterDuplexHandle();
        }
    }
    else if(Role == Role_Slave)
    {
        if(Mode == Mode_Simplex && RxBuffer != NULL)
        {
            SlaveSimplexHandle();
        }
        else if(Mode == Mode_Duplex && RxBuffer != NULL && TxBuffer != NULL)
        {
            SlaveDuplexHandle();
        }
    }

    IsHandling = false;
}

void NRF_Manager::IRQ_Handle()
{
    if(Role == Role_Slave)
    {
        Handle();
    }
}
