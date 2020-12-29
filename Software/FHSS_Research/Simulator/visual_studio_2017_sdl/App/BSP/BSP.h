#ifndef __BSP_H
#define __BSP_H

#include <stdint.h>
#include <stdbool.h>

uint8_t ComSlave_GetPackLoss();
void ComSlave_GetCnts(uint32_t* rxSuc, uint32_t* forceFH, uint32_t* resync);
void ComSlave_ResetCnts();

uint8_t ComTest_GetRxData(int index);
uint8_t ComTest_GetTxData(int index);
void ComTest_SetMaster(bool en);
bool ComTest_GetIsMaster();

#endif
