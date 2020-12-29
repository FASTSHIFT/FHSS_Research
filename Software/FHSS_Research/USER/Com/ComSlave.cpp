#include "ComPrivate.h"

void ComSlaveTest()
{
    nrf1_Mag.Handle();
    
    //LED_SetEnable(LED_STA2, NRF1_RxBuff[1]);
}

uint8_t ComSlave_GetPackLoss()
{
    return nrf1_Mag.GetRxPackLoss();
}

void ComSlave_GetCnts(uint32_t* rxSuc, uint32_t* forceFH, uint32_t* resync)
{
    nrf1_Mag.GetRxCnts(rxSuc, forceFH, resync);
}

void ComSlave_ResetCnts()
{
    nrf1_Mag.ResetRxCnts();
}
