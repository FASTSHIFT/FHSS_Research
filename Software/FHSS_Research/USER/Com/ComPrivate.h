#ifndef __COMPRIVATE_H
#define __COMPRIVATE_H

#include "NRF/NRF.h"
#include "NRF/NRF_Manager.h"
#include "Basic/SysConfig.h"
#include "Basic/SysLog.h"
#include "BSP/BSP_Hardware.h"

#define COM_BUFFER_LEN  32
#define COM_TIM         TIM1

extern uint8_t NRF1_TxBuff[COM_BUFFER_LEN];
extern uint8_t NRF1_RxBuff[COM_BUFFER_LEN];
extern uint8_t NRF2_TxBuff[COM_BUFFER_LEN];
extern uint8_t NRF2_RxBuff[COM_BUFFER_LEN];

bool Com_Init();

void ComTest_Init();

void ComMasterTest();
void ComSlaveTest();

extern NRF_Basic nrf1;
extern NRF_Basic nrf2;
extern NRF_Manager nrf1_Mag;

#endif
