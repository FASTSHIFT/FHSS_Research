#ifndef __NRF_MANAGER_H
#define __NRF_MANAGER_H

#include "NRF.h"

class NRF_Manager
{
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

    bool GetFH_Enable();
    uint16_t GetFH_TraverseTime();
    void GetRxCnts(uint32_t* rxSuc, uint32_t* forceFH, uint32_t* resync);
    uint8_t GetRxPackLoss();
    uint32_t GetTickElaps(uint32_t prevTick);
    void ResetRxCnts();
    
    void Handle();
    void IRQ_Handle();

private:
    uint16_t IntervalTime;
    uint16_t TxWaitTime;
    uint32_t LastRxTime;
    uint32_t LastTxTime;
    uint32_t LastResyncTime;
    
    uint8_t* TxBuffer;
    uint8_t* RxBuffer;

    bool TxReq;
    bool ModeUpdateReq;

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
