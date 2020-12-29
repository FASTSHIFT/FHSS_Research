#include "ComPrivate.h"
#include "Basic/CommonMacro.h"

static void LED_Toggle()
{
    static bool state;
    
    LED_SetEnable(LED_STA2, state);
    NRF1_TxBuff[1] = state;
    
    state = !state;
}

void ComMasterTest()
{
    NRF1_TxBuff[0]++;
    
    __IntervalExecute(LED_Toggle(), 500);
    
    __IntervalExecute(
        LED_SetEnable(LED_RX, (nrf1.GetTxPackLoss() < 50)),
        1000
    );
    
    nrf1_Mag.Handle();   
}
