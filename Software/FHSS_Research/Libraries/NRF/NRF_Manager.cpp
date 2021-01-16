#include "NRF_Manager.h"
#include "BSP/BSP_Hardware.h"

NRF_Manager::NRF_Manager(NRF_Basic* nrf)
{
    Basic = nrf;

    TxBuffer = NULL;
    RxBuffer = NULL;

    FH_Enable = true;
    FH_WaitResync = true;
    FH_List = NULL;
    FH_List_Length = 0;
    FH_List_Index = 0;
    FH_LastTime = 0;
    FH_WaitTime = 0;

    LastRxTime = 0;
    IntervalTime = 5;

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
    SetRole(Role);
}

void NRF_Manager::SetTxBuffer(void* txbuff, uint16_t len)
{
    if(len == Basic->RF_TX_PloadWidth)
    {
        TxBuffer = (uint8_t*)txbuff;
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

bool NRF_Manager::GetFH_Enable()
{
    return FH_Enable;
}

uint32_t NRF_Manager::GetTickElaps(uint32_t prevTick)
{
    uint32_t actTime = millis();

    if(actTime >= prevTick)
    {
        prevTick = actTime - prevTick;
    }
    else
    {
        prevTick = /*UINT32_MAX*/(uint32_t)0xFFFFFFFF - prevTick + 1;
        prevTick += actTime;
    }

    return prevTick;
}

/**
  * @brief  跳频处理
  * @param  无
  * @retval 无
  */
void NRF_Manager::FH_JumpNext()
{
    FH_LastTime = millis();
    
    if(!FH_Enable)
    {
        return;
    }

    
    LED_SetToggle(LED_STA1);
    
    FH_List_Index++;
    FH_List_Index %= FH_List_Length;
    Basic->SetFreqency(FH_List[FH_List_Index]);
    
    //LED_SetEnable(LED_STA1, false);
}

void NRF_Manager::MasterSimplexHandle()
{
    if(GetTickElaps(LastTxTime) < IntervalTime)
    {
        return;
    }
    
    LastTxTime = millis();
    
    LED_SetEnable(LED_TX, true);
    
    /*检查上一次的发送情况*/
    Basic->TranCheck();

    /*发射端跳频*/
    FH_JumpNext();

    /*再发送(不管接收端状态)*/
    Basic->Tran(TxBuffer);

    LED_SetEnable(LED_TX, false);
}

void NRF_Manager::SlaveSimplexHandle()
{
    uint32_t tick = millis();

    /*如果正常接收，则下一次正常跳频*/
    if(Basic->Recv(RxBuffer))
    {
        LED_SetEnable(LED_RX, true);
        
        CntRxCom++;
        CntRxSuccess++;
        LastRxTime = tick;
        FH_WaitTime = IntervalTime + IntervalTime / 2 + 1;
        
        FH_JumpNext();
        FH_WaitResync = false;
        
        LED_SetEnable(LED_RX, false);
    }
    else if(!FH_WaitResync && GetTickElaps(FH_LastTime) >= FH_WaitTime)
    {
        CntRxCom++;
        CntRxForceFH++;
        FH_WaitTime = IntervalTime;
        FH_JumpNext();
    }
    /*在丢失信号超时后，接收端关闭正常跳频*/
    /*慢跳等待重新同步发射端，慢跳周期大于发端遍历跳频表的周期*/
    /*一旦接收成功，立即启动正常跳频*/
    else if(GetTickElaps(LastRxTime) >= IntervalTime * (FH_List_Length + 1))
    {
        CntRxCom++;
        CntRxResync++;
        LastRxTime = tick;
        FH_JumpNext();
        FH_WaitResync = true;
    }
}

void NRF_Manager::MasterDuplexHandle()
{
    if(GetTickElaps(LastTxTime) < IntervalTime)
    {
        return;
    }
    
    LastTxTime = millis();

    if(Basic->RF_State == Basic->RF_State_TX)
    {
        uint8_t status = Basic->GetStatus();
        if(status & Basic->TX_DS)
        {
            Basic->CntTxSuccess++;
        }

        LED_SetEnable(LED_TX, false);
        
        Basic->RX_Mode();
        LED_SetEnable(LED_RX, true);
        CntRxCom++;
    }
    else if(Basic->RF_State == Basic->RF_State_RX)
    {
        if(Basic->Recv(RxBuffer))
        {
            CntRxSuccess++;
        }
        LED_SetEnable(LED_RX, false);
        
        LED_SetEnable(LED_TX, true);
        FH_JumpNext();
        Basic->TX_Mode();
        Basic->Tran(TxBuffer);
    }
}

void NRF_Manager::SlaveDuplexHandle()
{
    uint8_t status = Basic->GetStatus();
    
    uint32_t tick = millis();
    
    if(Basic->RF_State == Basic->RF_State_RX)
    {
        if(Basic->Recv(RxBuffer))
        {
            TxReq = true;
            CntRxCom++;
            CntRxSuccess++;
            LastRxTime = tick;
            FH_WaitTime = IntervalTime * 2 + IntervalTime;
            
            FH_WaitResync = false;
            
            LED_SetEnable(LED_RX, false);
        }
        else if(!FH_WaitResync && GetTickElaps(FH_LastTime) >= FH_WaitTime)
        {
            TxReq = true;
            CntRxCom++;
            CntRxForceFH++;
            FH_LastTime = millis();
            FH_WaitTime = IntervalTime * 2;
        }
        else if(GetTickElaps(LastRxTime) >= (IntervalTime * 2) * (FH_List_Length + 1))
        {
            CntRxCom++;
            CntRxResync++;
            LastRxTime = tick;
            FH_JumpNext();
            FH_WaitResync = true;
        }
        
        if(TxReq && GetTickElaps(LastRxTime) > IntervalTime)
        {
            TxReq = false;
            Basic->TX_Mode();
            Basic->Tran(TxBuffer);
            LastTxTime = tick;
            LED_SetEnable(LED_TX, true);
        }
    }
    else if(Basic->RF_State == Basic->RF_State_TX)
    {
        if( status & Basic->TX_DS || 
            status & Basic->MAX_RT ||
            GetTickElaps(LastTxTime) >= (IntervalTime - 1)
        )
        {
            if(status & Basic->TX_DS)
            {
                Basic->CntTxSuccess++;
            }
            LED_SetEnable(LED_TX, false);
            
            FH_JumpNext();
            Basic->RX_Mode();
            LED_SetEnable(LED_RX, true);
        }
    }
}

void NRF_Manager::Handle()
{
    LED_SetEnable(LED_STA2, (Basic->RF_State == Basic->RF_State_TX));

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
}

void NRF_Manager::IRQ_Handle()
{
}
