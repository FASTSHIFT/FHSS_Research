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
        ModeUpdateReq = true;
    }
}

void NRF_Manager::SetRole(Role_Type role)
{
    Role = role;
    ModeUpdateReq = true;
}

void NRF_Manager::SetMode(Mode_Type mode)
{
    Mode = mode;
    ModeUpdateReq = true;
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
    if(CntRxCom == 0)
        return 0;

    return (100 - 100 * CntRxSuccess / CntRxCom);
}

bool NRF_Manager::GetFH_Enable()
{
    return FH_Enable;
}

uint16_t NRF_Manager::GetFH_TraverseTime()
{
    if(Mode == Mode_Simplex)
    {
        return (IntervalTime * FH_List_Length);
    }
    else if(Mode == Mode_Duplex)
    {
        return (IntervalTime * 2 * FH_List_Length);
    }
    
    return 0;
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

    FH_List_Index++;
    FH_List_Index %= FH_List_Length;
    Basic->SetFreqency(FH_List[FH_List_Index]);

    LED_SetToggle(LED_STA1);
}

void NRF_Manager::MasterSimplexHandle()
{
    if(GetTickElaps(LastTxTime) < IntervalTime)
        return;
    
    LastTxTime = millis();
    
    LED_SetEnable(LED_TX, true);
    
    /*检查上一次的发送情况*/
    Basic->TranCheck();

    /*发射端跳频*/
    FH_JumpNext();

    /*直接发送(不管从机状态)*/
    Basic->Tran(TxBuffer);

    LED_SetEnable(LED_TX, false);
}

void NRF_Manager::SlaveSimplexHandle()
{
    /*如果正常接收，则下一次正常跳频*/
    if(Basic->Recv(RxBuffer))
    {
        LED_SetEnable(LED_RX, true);
        
        CntRxCom++;
        CntRxSuccess++;
        LastRxTime = millis();
        
        /*下一个频点的中间*/
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
    else if(GetTickElaps(LastRxTime) > GetFH_TraverseTime())
    {
        FH_WaitResync = true;
        
        /*在频点等待一段时间*/
        if(GetTickElaps(LastResyncTime) < GetFH_TraverseTime())
            return;
        
        LastResyncTime = millis();
        
        CntRxCom++;
        CntRxResync++;

        FH_JumpNext();
    }
}

void NRF_Manager::MasterDuplexHandle()
{
    if(GetTickElaps(LastTxTime) < IntervalTime)
        return;
    
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
    /*接收状态*/
    if(Basic->RF_State == Basic->RF_State_RX)
    {
        /*如果收到数据包*/
        if(Basic->Recv(RxBuffer))
        {   
            /*请求发送*/
            TxReq = true;
            
            /*通信计数*/
            CntRxCom++;
            CntRxSuccess++;
            
            /*记录当前接收的时间*/
            LastRxTime = millis();
            
            /*设置在当前频点等待的时间*/
            FH_WaitTime = IntervalTime * 2 - IntervalTime / 5;
            
            /*设定最长发送等待时间*/
            TxWaitTime = IntervalTime / 2 + IntervalTime % 2;
            
            /*关闭重新同步*/
            FH_WaitResync = false;
            
            LED_SetEnable(LED_RX, false);
        }
        /*如果未请求发送，并且没有重新同步等待，并且在当前频点等待时间超时*/
        else if(!TxReq && !FH_WaitResync && GetTickElaps(FH_LastTime) >= FH_WaitTime)
        {
            /*请求发送，在发送完成后跳频*/
            TxReq = true;
            
            /*通信计数*/
            CntRxCom++;
            CntRxForceFH++;
            
            /*不允许发送等待太久*/
            TxWaitTime = 1;
        }
        /*如果掉线超过遍历一次跳频表的时间，则认为通信中断，需重新进行同步*/
        else if(GetTickElaps(LastRxTime) > GetFH_TraverseTime())
        {
            /*标记为重新同步状态*/
            FH_WaitResync = true;
            
            /*在频点等待一段时间*/
            if(GetTickElaps(LastResyncTime) < GetFH_TraverseTime())
                return;
            
            LastResyncTime = millis();
            
            /*通信计数*/
            CntRxCom++;
            CntRxResync++;
            
            /*跳频*/
            FH_JumpNext();
        }
        
        /*处理发送请求，并且距离上一次接收超过半个通信周期*/
        if(TxReq && GetTickElaps(LastRxTime) >= IntervalTime)
        {
            /*设为发送状态*/
            Basic->TX_Mode();
            
            /*发送数据包*/
            Basic->Tran(TxBuffer);
            
            /*记录发送时间*/
            LastTxTime = millis();
            
            /*已处理发送请求*/
            TxReq = false;
            
            LED_SetEnable(LED_TX, true);
        }
    }
    /*发送状态*/
    else if(Basic->RF_State == Basic->RF_State_TX)
    {   
        /*获取NRF状态*/
        uint8_t status = Basic->GetStatus();
        
        /*发送成功，或发送失败，或发送超时*/
        if( status & Basic->TX_DS || 
            status & Basic->MAX_RT ||
            GetTickElaps(LastTxTime) >= TxWaitTime
        )
        {
            /*发送成功记录*/
            if(status & Basic->TX_DS)
            {
                Basic->CntTxSuccess++;
            }
            
            LED_SetEnable(LED_TX, false);
            
            /*跳频，要比主机先达到新频点*/
            FH_JumpNext();
            
            /*切接收模式*/
            Basic->RX_Mode();
            
            LED_SetEnable(LED_RX, true);
        }
    }
}

void NRF_Manager::Handle()
{
    /*处理模式更新请求*/
    if(ModeUpdateReq)
    {
        LED_SetEnable(LED_STA1, false);
        Basic->SetFreqency(FH_List[FH_List_Index]);
        if(Role == Role_Master)
        {
            Basic->TX_Mode(true);
        }
        else if(Role == Role_Slave)
        {
            Basic->RX_Mode(true);
        }
        ModeUpdateReq = false;
    }
    
    /*主机*/
    if(Role == Role_Master)
    {
        if(TxBuffer == NULL)
            return;
        
        if(Mode == Mode_Simplex)
        {
            /*单工*/
            MasterSimplexHandle();
        }
        else if(Mode == Mode_Duplex && RxBuffer != NULL)
        {
            /*双工*/
            MasterDuplexHandle();
        }
    }
    /*从机*/
    else if(Role == Role_Slave)
    {
        if(RxBuffer == NULL)
            return;
        
        if(Mode == Mode_Simplex)
        {
            /*单工*/
            SlaveSimplexHandle();
        }
        else if(Mode == Mode_Duplex && TxBuffer != NULL)
        {
            /*双工*/
            SlaveDuplexHandle();
        }
    }
    
    LED_SetEnable(LED_STA2, (Basic->RF_State == Basic->RF_State_TX));
}

void NRF_Manager::IRQ_Handle()
{
}
