#ifndef __NRF_MANAGER_H
#define __NRF_MANAGER_H

#include "NRF.h"

class NRF_Manager
{
    typedef void(*TimeSetCallbackFunc_t)(uint32_t);
public:
    NRF_Manager(NRF_Basic* nrf);
    NRF_Basic* Basic;

    typedef enum
    {
        Role_Master,
        Role_Slave,
    } Role_Type;
    Role_Type Role;

    typedef enum
    {
        State_TX,
        State_RX,
        State_WAIT,
    } State_Type;
    State_Type State;

    typedef enum
    {
        Mode_Simplex,
        Mode_Duplex,
    } Mode_Type;
    Mode_Type Mode;

    void SetFH_Enable(bool en);
    void SetRole(Role_Type role);
    void SetMode(Mode_Type mode);
    void SetTxBuffer(void* txbuff, uint16_t len);
    void SetRxBuffer(void* rxbuff, uint16_t len);
    void SetFreqHoppingList(const uint8_t* list, uint16_t length);
    void SetInterruptTimeCallback(TimeSetCallbackFunc_t func);

    bool GetFH_Enable();
    void GetRxCnts(uint32_t* rxSuc, uint32_t* forceFH, uint32_t* resync);
    uint8_t GetRxPackLoss();
    uint32_t GetTickElaps(uint32_t prevTick);
    void ResetRxCnts();
    
    void Handle();
    void IRQ_Handle();

private:
    uint16_t IntervalTime;
    uint32_t LastRxTime;
    uint32_t LastTxTime;
    
    uint8_t* TxBuffer;
    uint8_t* RxBuffer;

    bool TxReq;

    bool FH_Enable;
    bool FH_WaitResync;
    const uint8_t* FH_List;
    uint16_t FH_List_Length;
    uint16_t FH_List_Index;
    uint32_t FH_LastTime;
    uint16_t FH_WaitTime;
    void FH_JumpNext();

    uint32_t CntRxCom;
    uint32_t CntRxSuccess;
    uint32_t CntRxForceFH;
    uint32_t CntRxResync;

    void MasterSimplexHandle();
    void MasterDuplexHandle();
    void SlaveSimplexHandle();
    void SlaveDuplexHandle();
};

#endif
