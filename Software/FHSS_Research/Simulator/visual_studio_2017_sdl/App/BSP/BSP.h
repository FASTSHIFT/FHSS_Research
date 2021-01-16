#ifndef __BSP_H
#define __BSP_H

#include <stdint.h>
#include <stdbool.h>

void ComSlave_GetCnts(uint32_t* rxSuc, uint32_t* forceFH, uint32_t* resync);

uint8_t ComTest_GetRxData(int index);
uint8_t ComTest_GetTxData(int index);
void ComTest_SetMaster(bool en);
bool ComTest_GetIsMaster();
void ComTest_GetTxRxCnts(uint32_t* txCnt, uint32_t* rxCnt);
void ComTest_GetPackLoss(uint8_t* txLoss, uint8_t* rxLoss);
bool ComTest_GetFHSS_Enable();
void ComTest_SetFHSS_Enable(bool en);
bool ComTest_GetIsDuplexMode();
void ComTest_SetDuplexMode(bool en);
void ComTest_ResetCnts();

#endif
