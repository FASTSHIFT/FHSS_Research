#include "ComPrivate.h"
#include "Basic/SysLog.h"

#define COM_BUFFER_LEN 32

NRF_Basic nrf1(
    NRF1_MOSI_Pin, NRF1_MISO_Pin, NRF1_SCK_Pin,
    NRF1_CE_Pin, NRF1_CSN_Pin
);

NRF_Basic nrf2(
    NRF2_MOSI_Pin, NRF2_MISO_Pin, NRF2_SCK_Pin,
    NRF2_CE_Pin, NRF2_CSN_Pin
);
//IRQ   MISO
//MOSI  SCK
//CSN   CE
//VCC   GND

/*NRF1发送数据缓冲区*/
uint8_t NRF1_TxBuff[COM_BUFFER_LEN];

/*NRF1接收数据缓冲区*/
uint8_t NRF1_RxBuff[COM_BUFFER_LEN];

/*NRF2发送数据缓冲区*/
uint8_t NRF2_TxBuff[COM_BUFFER_LEN];

/*NRF2接收数据缓冲区*/
uint8_t NRF2_RxBuff[COM_BUFFER_LEN];

static bool Com_DefaultInit(NRF_Basic* nrf)
{
    /*默认初始化*/
    nrf->Init();
    
    /*发送模式*/
    nrf->TX_Mode();

    /*数据包长度*/
    nrf->SetPayloadWidth(COM_BUFFER_LEN, COM_BUFFER_LEN);
    
    /*设置速率*/
    nrf->SetSpeed(NRF_Basic::SPEED_2Mbps);
    
    /*设置频率*/
    nrf->SetFreqency(24);
    
    /*设置地址*/
    nrf->SetAddress(0x12, 0x34, 0x56, 0x78, 0x90);

    /*禁用射频*/
    nrf->SetRF_Enable(false);

    /*更新寄存器状态*/
    nrf->UpdateRegs();
    
    return nrf->IsDetect();
}


bool Com_Init()
{
    bool nrf1_state = Com_DefaultInit(&nrf1);
    SysLog_Printf("NRF1 state: %s", nrf1_state ? "OK" : "FAILD");
    
    bool nrf2_state = Com_DefaultInit(&nrf2);
    SysLog_Printf("NRF2 state: %s", nrf2_state ? "OK" : "FAILD");
    
    return (nrf1_state && nrf2_state);
}
