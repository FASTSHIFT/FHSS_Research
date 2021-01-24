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
  * @brief  ��Ƶ����
  * @param  ��
  * @retval ��
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
    
    /*�����һ�εķ������*/
    Basic->TranCheck();

    /*�������Ƶ*/
    FH_JumpNext();

    /*ֱ�ӷ���(���ܴӻ�״̬)*/
    Basic->Tran(TxBuffer);

    LED_SetEnable(LED_TX, false);
}

void NRF_Manager::SlaveSimplexHandle()
{
    /*����������գ�����һ��������Ƶ*/
    if(Basic->Recv(RxBuffer))
    {
        LED_SetEnable(LED_RX, true);
        
        CntRxCom++;
        CntRxSuccess++;
        LastRxTime = millis();
        
        /*��һ��Ƶ����м�*/
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
    /*�ڶ�ʧ�źų�ʱ�󣬽��ն˹ر�������Ƶ*/
    /*�����ȴ�����ͬ������ˣ��������ڴ��ڷ��˱�����Ƶ�������*/
    /*һ�����ճɹ�����������������Ƶ*/
    else if(GetTickElaps(LastRxTime) > GetFH_TraverseTime())
    {
        FH_WaitResync = true;
        
        /*��Ƶ��ȴ�һ��ʱ��*/
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
    /*����״̬*/
    if(Basic->RF_State == Basic->RF_State_RX)
    {
        /*����յ����ݰ�*/
        if(Basic->Recv(RxBuffer))
        {   
            /*������*/
            TxReq = true;
            
            /*ͨ�ż���*/
            CntRxCom++;
            CntRxSuccess++;
            
            /*��¼��ǰ���յ�ʱ��*/
            LastRxTime = millis();
            
            /*�����ڵ�ǰƵ��ȴ���ʱ��*/
            FH_WaitTime = IntervalTime * 2 - IntervalTime / 5;
            
            /*�趨����͵ȴ�ʱ��*/
            TxWaitTime = IntervalTime / 2 + IntervalTime % 2;
            
            /*�ر�����ͬ��*/
            FH_WaitResync = false;
            
            LED_SetEnable(LED_RX, false);
        }
        /*���δ�����ͣ�����û������ͬ���ȴ��������ڵ�ǰƵ��ȴ�ʱ�䳬ʱ*/
        else if(!TxReq && !FH_WaitResync && GetTickElaps(FH_LastTime) >= FH_WaitTime)
        {
            /*�����ͣ��ڷ�����ɺ���Ƶ*/
            TxReq = true;
            
            /*ͨ�ż���*/
            CntRxCom++;
            CntRxForceFH++;
            
            /*�������͵ȴ�̫��*/
            TxWaitTime = 1;
        }
        /*������߳�������һ����Ƶ���ʱ�䣬����Ϊͨ���жϣ������½���ͬ��*/
        else if(GetTickElaps(LastRxTime) > GetFH_TraverseTime())
        {
            /*���Ϊ����ͬ��״̬*/
            FH_WaitResync = true;
            
            /*��Ƶ��ȴ�һ��ʱ��*/
            if(GetTickElaps(LastResyncTime) < GetFH_TraverseTime())
                return;
            
            LastResyncTime = millis();
            
            /*ͨ�ż���*/
            CntRxCom++;
            CntRxResync++;
            
            /*��Ƶ*/
            FH_JumpNext();
        }
        
        /*���������󣬲��Ҿ�����һ�ν��ճ������ͨ������*/
        if(TxReq && GetTickElaps(LastRxTime) >= IntervalTime)
        {
            /*��Ϊ����״̬*/
            Basic->TX_Mode();
            
            /*�������ݰ�*/
            Basic->Tran(TxBuffer);
            
            /*��¼����ʱ��*/
            LastTxTime = millis();
            
            /*�Ѵ���������*/
            TxReq = false;
            
            LED_SetEnable(LED_TX, true);
        }
    }
    /*����״̬*/
    else if(Basic->RF_State == Basic->RF_State_TX)
    {   
        /*��ȡNRF״̬*/
        uint8_t status = Basic->GetStatus();
        
        /*���ͳɹ�������ʧ�ܣ����ͳ�ʱ*/
        if( status & Basic->TX_DS || 
            status & Basic->MAX_RT ||
            GetTickElaps(LastTxTime) >= TxWaitTime
        )
        {
            /*���ͳɹ���¼*/
            if(status & Basic->TX_DS)
            {
                Basic->CntTxSuccess++;
            }
            
            LED_SetEnable(LED_TX, false);
            
            /*��Ƶ��Ҫ�������ȴﵽ��Ƶ��*/
            FH_JumpNext();
            
            /*�н���ģʽ*/
            Basic->RX_Mode();
            
            LED_SetEnable(LED_RX, true);
        }
    }
}

void NRF_Manager::Handle()
{
    /*����ģʽ��������*/
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
    
    /*����*/
    if(Role == Role_Master)
    {
        if(TxBuffer == NULL)
            return;
        
        if(Mode == Mode_Simplex)
        {
            /*����*/
            MasterSimplexHandle();
        }
        else if(Mode == Mode_Duplex && RxBuffer != NULL)
        {
            /*˫��*/
            MasterDuplexHandle();
        }
    }
    /*�ӻ�*/
    else if(Role == Role_Slave)
    {
        if(RxBuffer == NULL)
            return;
        
        if(Mode == Mode_Simplex)
        {
            /*����*/
            SlaveSimplexHandle();
        }
        else if(Mode == Mode_Duplex && TxBuffer != NULL)
        {
            /*˫��*/
            SlaveDuplexHandle();
        }
    }
    
    LED_SetEnable(LED_STA2, (Basic->RF_State == Basic->RF_State_TX));
}

void NRF_Manager::IRQ_Handle()
{
}
