#include "BSP.h"

static bool isMaster = true;

uint8_t ComTest_GetRxData(int index)
{
    static uint8_t data;
    return data++;
}

uint8_t ComTest_GetTxData(int index)
{
    static uint8_t data;
    return data++;
}

void ComTest_SetMaster(bool en)
{
    isMaster = en;
}

bool ComTest_GetIsMaster()
{
    return isMaster;
}

uint8_t ComSlave_GetPackLoss()
{
    return 50;
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
