#include "BSP.h"

static bool IsMaster = true;
static bool IsFHSS_Enable = true;
static bool IsDuplexMode = false;

uint8_t ComTest_GetRxData(int index)
{
    static uint8_t data;
    return data--;
}

uint8_t ComTest_GetTxData(int index)
{
    static uint8_t data;
    return data++;
}

void ComTest_SetMaster(bool en)
{
    IsMaster = en;
}

bool ComTest_GetIsMaster()
{
    return IsMaster;
}

void ComTest_GetPackLoss(uint8_t* txLoss, uint8_t* rxLoss)
{
    *txLoss = 50;
    *rxLoss = 50;
}

void ComSlave_GetCnts(uint32_t* rxSuc, uint32_t* forceFH, uint32_t* resync)
{
    *rxSuc = 100;
    *forceFH = 10;
    *resync = 1;
}

void ComSlave_ResetCnts()
{
    
}

void ComTest_GetTxRxCnts(uint32_t* txCnt, uint32_t* rxCnt)
{
    *txCnt = 100;
    *rxCnt = 100;
}

bool ComTest_GetFHSS_Enable()
{
    return IsFHSS_Enable;
}

void ComTest_SetFHSS_Enable(bool en)
{
    IsFHSS_Enable = en;
}

bool ComTest_GetIsDuplexMode()
{
    return IsDuplexMode;
}

void ComTest_SetDuplexMode(bool en)
{
    IsDuplexMode = en;
}

void ComTest_ResetCnts()
{

}
