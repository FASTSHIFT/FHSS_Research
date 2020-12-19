#include "DisplayPrivate.h"
#include "BSP/BSP.h"

/**
  * @brief  ͨ����ʾ��ʼ��
  * @param  ��
  * @retval ��
  */
static void Display_CommonInit()
{
    /*�Զ��������ʼ��*/
}

#ifdef ARDUINO

#define USE_FPS_TEST 0

/*ʵ������Ļ����*/
SCREEN_CLASS screen(
    SCREEN_CS_Pin, 
    SCREEN_DC_Pin, 
    SCREEN_RES_Pin
    //,
    //SCREEN_SCLK_Pin,
    //SCREEN_MOSI_Pin
);

#if (USE_FPS_TEST == 1) 
static void Display_FPSTest();
#endif

extern "C" {
    void lv_demo_benchmark(void);
    void lv_demo_widgets(void);
    void lv_demo_stress(void);
}

/**
  * @brief  ��ʾ��ʼ��
  * @param  ��
  * @retval ��
  */
void Display_Init()
{
    SysLog_Printf("Adafruit_ST7735 &screen = 0x%p", &screen);
    /*��Ļ��ʼ��*/
    screen.begin();
    screen.setRotation(0);
    screen.fillScreen(screen.Black);
    
    Backlight_Init();
    Backlight_SetValue(1000);
    
#if (USE_FPS_TEST == 1)
    Display_FPSTest();
#endif
    
    /*�Զ������ʼ��*/
    DisplayError_Init();
    
    /*lvgl��ʼ��*/
    lv_init();
    lv_port_disp_init();
    lv_port_log_init();
    
    Display_CommonInit();
    
    /*���⽥��*/
    Backlight_SetGradual(1000, 100);
    
    lv_demo_benchmark();
    //lv_demo_stress();
    //lv_demo_widgets();
}

#if (USE_FPS_TEST == 1) 
static void Display_FPSTest()
{
    float Ftime, Ltime;
    float FPS;
    Ftime = millis();
    int i;
    for(i = 100; i > 0; i--)
    {
        screen.fillScreen(screen.Black);
        screen.fillScreen(screen.White);
    }
    Ltime = millis() - Ftime;
    FPS = 100.0f / (Ltime / 1000.0f) * 2.0f;
    screen.fillScreen(screen.Blue);
    screen.setTextSize(1);
    screen.setTextColor(screen.White);
    screen.setCursor(0, 0);
    screen.print(Ltime);
    screen.setCursor(45, 0);
    screen.print("ms");
    screen.setCursor(0, 9);
    screen.print("FPS:");
    screen.setCursor(25, 9);
    screen.print(FPS);
    while(1);
}
#endif

#else

void Display_Init()
{
    Display_CommonInit();
}

#endif

void Display_Update()
{
    lv_task_handler();
}
