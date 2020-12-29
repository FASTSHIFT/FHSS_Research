#include "Arduino.h"
#include "Display/DisplayPrivate.h"
#include "Com/ComPrivate.h"
#include "BSP/BSP_Hardware.h"

void setup()
{
    SysLog_Init();
    SysLog_Printf(SYS_NAME);
    Display_Init();
    LED_Init();
    LED_Test();
    Com_Init();
}

void loop()
{
    Display_Update();
}

/**
  * @brief  Main Function
  * @param  None
  * @retval None
  */
int main(void)
{
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
    GPIO_JTAG_Disable();
    Delay_Init();
    //ADCx_Init(ADC1);
    setup();
    for(;;)loop();
}
