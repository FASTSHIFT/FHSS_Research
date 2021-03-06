#include "../DisplayPrivate.h"

/*屏幕驱动结构体地址*/
static lv_disp_drv_t * disp_drv_p;

/*乒乓缓冲区*/
#define DISP_BUF_SIZE        (LV_HOR_RES_MAX * LV_VER_RES_MAX)
static lv_color_t lv_disp_buf1[DISP_BUF_SIZE];
static lv_color_t lv_disp_buf2[DISP_BUF_SIZE];

#define DISP_DMA_Channel DMA1_Channel3

/**
  * @brief  使用DMA发送缓冲区数据
  * @param  buf:缓冲区地址
  * @param  size:缓冲区长度
  * @retval 无
  */
static void disp_spi_dma_send(void* buf, uint32_t size)
{
    DMA_ChannelEnable(DISP_DMA_Channel, DISABLE);
    DISP_DMA_Channel->CMBA = (uint32_t)buf;
    DISP_DMA_Channel->TCNT = size;
    DMA_ChannelEnable(DISP_DMA_Channel, ENABLE);
}

/**
  * @brief  屏幕刷新回调函数
  * @param  disp:屏幕驱动地址
  * @param  area:刷新区域
  * @param  color_p:刷新缓冲区地址
  * @retval 无
  */
static void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    disp_drv_p = disp;

    //screen.drawFastRGBBitmap(area->x1, area->y1, (uint16_t*)color_p, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1));
    //lv_disp_flush_ready(disp_drv_p);
    //return;
    
    int16_t w = (area->x2 - area->x1 + 1);
    int16_t h = (area->y2 - area->y1 + 1);
    uint32_t size = w * h * sizeof(lv_color_t);

    /*设置刷新区域*/
    screen.setAddrWindow(area->x1, area->y1, area->x2, area->y2);

    /*数据模式*/
    digitalWrite_HIGH(SCREEN_DC_Pin);

    /*DMA发送请求*/
    disp_spi_dma_send(color_p, size);
}

extern "C" {
    void DMA1_Channel3_IRQHandler(void)
    {
        /*DMA发送完成中断*/
        if(DMA_GetITStatus(DMA1_INT_TC3) == SET)
        {
            lv_disp_flush_ready(disp_drv_p);/* tell lvgl that flushing is done */
            DMA_ClearITPendingBit(DMA1_INT_TC3);
        }
    }
}

/**
  * @brief  DMA初始化
  * @param  无
  * @retval 无
  */
static void disp_spi_dma_init()
{
    DMA_InitType  DMA_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_DMA1, ENABLE);

    DMA_Reset(DISP_DMA_Channel);

    DMA_DefaultInitParaConfig(&DMA_InitStructure);
    
    DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;  //工作在正常缓存模式
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;  //DMA通道 没有设置为内存到内存传输
    DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALDST;  //数据传输方向，从内存读取发送到外设
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_MEDIUM; //DMA通道 中优先级
    
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)lv_disp_buf1;  //DMA内存基地址
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;  //内存地址寄存器不变
    DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE; //数据宽度
    DMA_InitStructure.DMA_BufferSize = sizeof(lv_disp_buf1);  //DMA通道的DMA缓存的大小
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(SPI1->DT));  //DMA外设SPI基地址
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;  //外设地址寄存器不变
    DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;  //数据宽度

    DMA_Init(DISP_DMA_Channel, &DMA_InitStructure);

    SPI_I2S_DMAEnable(SPI1, SPI_I2S_DMA_TX, ENABLE);

    NVIC_EnableIRQ(DMA1_Channel3_IRQn);
    
    DMA_INTConfig(DISP_DMA_Channel, DMA_INT_TC, ENABLE);
}

/**
  * @brief  屏幕初始化
  * @param  无
  * @retval 无
  */
void lv_port_disp_init()
{
    disp_spi_dma_init();

    static lv_disp_buf_t disp_buf;
    lv_disp_buf_init(&disp_buf, lv_disp_buf1, lv_disp_buf2, DISP_BUF_SIZE);

    /*Initialize the display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);
}
