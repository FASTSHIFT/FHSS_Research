#include "ComPrivate.h"

NRF_Basic nrf1(
    NRF1_MOSI_Pin, NRF1_MISO_Pin, NRF1_SCK_Pin,
    NRF1_CE_Pin, NRF1_CSN_Pin
);

NRF_Basic nrf2(
    NRF2_MOSI_Pin, NRF2_MISO_Pin, NRF2_SCK_Pin,
    NRF2_CE_Pin, NRF2_CSN_Pin
);

static bool Com_DefaultInit(NRF_Basic* nrf)
{
    /*Ĭ�ϳ�ʼ��*/
    nrf->Init();
    
    /*����ģʽ*/
    nrf->TX_Mode(true);

    /*���ݰ�����*/
    nrf->SetPayloadWidth(COM_BUFFER_LEN, COM_BUFFER_LEN);
    
    /*��������*/
    nrf->SetSpeed(NRF_Basic::SPEED_1Mbps);
    
    /*����Ƶ��*/
    nrf->SetFreqency(24);
    
    /*���õ�ַ*/
    nrf->SetAddress(0x12, 0x34, 0x56, 0x78, 0x90);

    /*������Ƶ*/
    nrf->SetRF_Enable(true);

    /*���¼Ĵ���״̬*/
    nrf->UpdateRegs();
    
    return nrf->IsDetect();
}

bool Com_Init()
{
    pinMode(NRF1_IRQ_Pin, INPUT_PULLUP);
    pinMode(NRF2_IRQ_Pin, INPUT_PULLUP);
    
    bool nrf1_state = Com_DefaultInit(&nrf1);
    SysLog_Printf("NRF1 state: %s", nrf1_state ? "OK" : "FAILD");
    
    bool nrf2_state = Com_DefaultInit(&nrf2);
    SysLog_Printf("NRF2 state: %s", nrf2_state ? "OK" : "FAILD");
    
    ComTest_Init();
    ComTest_SetMaster(true);
    
    return (nrf1_state && nrf2_state);
}
