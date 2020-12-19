#include "Adafruit_ST7789.h"
#include "SPI.h"

#define TFT_SPI SPI

#if defined(__STM32__)
#define TFT_CS_SET      GPIO_HIGH(csport,cspinmask)
#define TFT_RS_SET      GPIO_HIGH(rsport,rspinmask)
#define TFT_SCK_SET     GPIO_HIGH(sckport,sckpinmask)
#define TFT_MOSI_SET    GPIO_HIGH(mosiport,mosipinmask)

#define TFT_CS_CLR      GPIO_LOW(csport,cspinmask)
#define TFT_RS_CLR      GPIO_LOW(rsport,rspinmask)
#define TFT_SCK_CLR     GPIO_LOW(sckport,sckpinmask)
#define TFT_MOSI_CLR    GPIO_LOW(mosiport,mosipinmask)
#else
#define TFT_CS_SET      digitalWrite(CS,HIGH)
#define TFT_RS_SET      digitalWrite(RS,HIGH)
#define TFT_SCK_SET     digitalWrite(SCK,HIGH)
#define TFT_MOSI_SET    digitalWrite(MOSI,HIGH)

#define TFT_CS_CLR      digitalWrite(CS,LOW)
#define TFT_RS_CLR      digitalWrite(RS,LOW)
#define TFT_SCK_CLR     digitalWrite(SCK,LOW)
#define TFT_MOSI_CLR    digitalWrite(MOSI,LOW)
#endif


Adafruit_ST7789::Adafruit_ST7789(uint8_t cs, uint8_t rs, uint8_t rst)
    : Adafruit_GFX(ST7789_TFTWIDTH, ST7789_TFTHEIGHT)
{
    RST = rst;
    CS = cs;
    RS = rs;

#if defined(__STM32__)
    csport = digitalPinToPort(CS);
    cspinmask = digitalPinToBitMask(CS);
    rsport = digitalPinToPort(RS);
    rspinmask = digitalPinToBitMask(RS);
#endif

    hwSPI = true;
}

Adafruit_ST7789::Adafruit_ST7789(uint8_t cs, uint8_t rs, uint8_t rst, uint8_t clk, uint8_t mosi)
    : Adafruit_GFX(ST7789_TFTWIDTH, ST7789_TFTHEIGHT)
{
    RST = rst;
    CS = cs;
    RS = rs;
    SCK = clk;
    MOSI = mosi;

#if defined(__STM32__)
    csport = digitalPinToPort(CS);
    cspinmask = digitalPinToBitMask(CS);
    rsport = digitalPinToPort(RS);
    rspinmask = digitalPinToBitMask(RS);
    sckport = digitalPinToPort(SCK);
    sckpinmask = digitalPinToBitMask(SCK);
    mosiport = digitalPinToPort(MOSI);
    mosipinmask = digitalPinToBitMask(MOSI);
#endif

    hwSPI = false;
}

void Adafruit_ST7789::begin()
{
    pinMode(PA5, OUTPUT);
    pinMode(PA7, OUTPUT);
    digitalWrite(PA5, HIGH);
    digitalWrite(PA7, HIGH);
    
    

    pinMode(CS, OUTPUT);
    pinMode(RS, OUTPUT);
    pinMode(RST, OUTPUT);
    digitalWrite(CS, HIGH);
    digitalWrite(RS, HIGH);
    digitalWrite(RST, HIGH);
    
    delay(200);
    digitalWrite(RST, LOW);
    delay(200);
    digitalWrite(RST, HIGH);
    delay(200);
    
    if(hwSPI)
    {
        TFT_SPI.begin();
        TFT_SPI.setClock(200000000);
    }
    else
    {
        pinMode(SCK, OUTPUT);
        pinMode(MOSI, OUTPUT);
        digitalWrite(SCK, HIGH);
        digitalWrite(MOSI, HIGH);
    }

    //************* Start Initial Sequence **********//
    writeCommand(0x36);
    writeData(0x00);

    writeCommand(0x3A);
    writeData(0x05);

    writeCommand(0xB2);
    writeData(0x0C);
    writeData(0x0C);
    writeData(0x00);
    writeData(0x33);
    writeData(0x33);

    writeCommand(0xB7);
    writeData(0x35);

    writeCommand(0xBB);
    writeData(0x19);

    writeCommand(0xC0);
    writeData(0x2C);

    writeCommand(0xC2);
    writeData(0x01);

    writeCommand(0xC3);
    writeData(0x12);

    writeCommand(0xC4);
    writeData(0x20);

    writeCommand(0xC6);
    writeData(0x0F);

    writeCommand(0xD0);
    writeData(0xA4);
    writeData(0xA1);

    writeCommand(0xE0);
    writeData(0xD0);
    writeData(0x04);
    writeData(0x0D);
    writeData(0x11);
    writeData(0x13);
    writeData(0x2B);
    writeData(0x3F);
    writeData(0x54);
    writeData(0x4C);
    writeData(0x18);
    writeData(0x0D);
    writeData(0x0B);
    writeData(0x1F);
    writeData(0x23);

    writeCommand(0xE1);
    writeData(0xD0);
    writeData(0x04);
    writeData(0x0C);
    writeData(0x11);
    writeData(0x13);
    writeData(0x2C);
    writeData(0x3F);
    writeData(0x44);
    writeData(0x51);
    writeData(0x2F);
    writeData(0x1F);
    writeData(0x1F);
    writeData(0x20);
    writeData(0x23);

    writeCommand(0x21);

    writeCommand(0x11);

    writeCommand(0x29);
}

void Adafruit_ST7789::spiWrite(uint8_t data)
{
    for(uint8_t i = 0; i < 8; i++)
    {
        TFT_SCK_CLR;
        (data & 0x80) ? TFT_MOSI_SET : TFT_MOSI_CLR;
        TFT_SCK_SET;
        data <<= 1;
    }
}

void Adafruit_ST7789::writeCommand(uint8_t cmd)
{
    TFT_CS_CLR;
    TFT_RS_CLR;
    if(hwSPI)
        TFT_SPI.transfer(cmd);
    else
        spiWrite(cmd);
    TFT_CS_SET;
}

void Adafruit_ST7789::writeData16(uint16_t data)
{
    TFT_CS_CLR;
    TFT_RS_SET;
    if(hwSPI)
    {
        TFT_SPI.transfer(data >> 8);
        TFT_SPI.transfer(data);
    }
    else
    {
        spiWrite(data >> 8);
        spiWrite(data);
    }
    TFT_CS_SET;
}

void Adafruit_ST7789::writeData(uint8_t data)
{
    TFT_CS_CLR;
    TFT_RS_SET;
    if(hwSPI)
    {
        TFT_SPI.transfer(data);
    }
    else
    {
        spiWrite(data);
    }
    TFT_CS_SET;
}

void Adafruit_ST7789::setRotation(uint8_t r)
{
    rotation = r % 4;
    writeCommand(0x36);
    switch(rotation)
    {
    case 0:
        _width = ST7789_TFTWIDTH;
        _height = ST7789_TFTHEIGHT;

        writeData(0x00);
        break;

    case 1:
        _width = ST7789_TFTHEIGHT;
        _height = ST7789_TFTWIDTH;

        writeData(0xC0);
        break;

    case 2:
        _width = ST7789_TFTWIDTH;
        _height = ST7789_TFTHEIGHT;

        writeData(0x70);
        break;

    case 3:
        _width = ST7789_TFTHEIGHT;
        _height = ST7789_TFTWIDTH;

        writeData(0xA0);
        break;
    }
}

void Adafruit_ST7789::setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    switch(rotation)
    {
    case 0:
        writeCommand(0x2A);
        writeData16(x0);
        writeData16(x1);

        writeCommand(0x2B);
        writeData16(y0);
        writeData16(y1);
        break;
    case 1:
        writeCommand(0x2A);
        writeData16(x0);
        writeData16(x1);

        writeCommand(0x2B);
        writeData16(y0 + 80);
        writeData16(y1 + 80);
        break;
    case 2:
        writeCommand(0x2A);
        writeData16(x0);
        writeData16(x1);

        writeCommand(0x2B);
        writeData16(y0);
        writeData16(y1);
        break;
    case 3:
        writeCommand(0x2A);
        writeData16(x0 + 80);
        writeData16(x1 + 80);

        writeCommand(0x2B);
        writeData16(y0);
        writeData16(y1);
        break;
    }

    writeCommand(0x2C);
}

void Adafruit_ST7789::setCursor(int16_t x, int16_t y)
{
    cursor_x = x;
    cursor_y = y;
    if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;
    setAddrWindow(x, y, x, y);
}

void Adafruit_ST7789::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;
    setAddrWindow(x, y, x, y);
    writeData16(color);
}

void Adafruit_ST7789::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    // Rudimentary clipping
    if((x >= _width) || (y >= _height) || h < 1) return;
    if((y + h - 1) >= _height) h = _height - y;
    if(x < 0) x = 0;
    if(y < 0) y = 0;

    setAddrWindow(x, y, x, y + h - 1);

    while (h--)
        writeData16(color);
}

void Adafruit_ST7789::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    // Rudimentary clipping
    if((x >= _width) || (y >= _height) || w < 1) return;
    if((x + w - 1) >= _width)  w = _width - x;
    if(x < 0) x = 0;
    if(y < 0) y = 0;

    setAddrWindow(x, y, x + w - 1, y);

    while (w--)
        writeData16(color);
}

void Adafruit_ST7789::drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
{
    if((x >= _width) || (y >= _height)) return;

    int16_t actual_cursor_x = x;
    int16_t actual_cursor_y = y;
    int16_t actual_cursor_x1 = x + w - 1;
    int16_t actual_cursor_y1 = y + h - 1;

    if(actual_cursor_x < 0)
    {
        actual_cursor_x = 0;
    }
    else if(actual_cursor_x >= _width)
    {
        actual_cursor_x = _width - 1;
    }

    if(actual_cursor_y < 0)
    {
        actual_cursor_y = 0;
    }
    else if(actual_cursor_y >= _height)
    {
        actual_cursor_y = _height - 1;
    }

    if(actual_cursor_x1 < 0)
    {
        actual_cursor_x1 = 0;
    }
    else if(actual_cursor_x1 >= _width)
    {
        actual_cursor_x1 = _width - 1;
    }

    if(actual_cursor_y1 < 0)
    {
        actual_cursor_y1 = 0;
    }
    else if(actual_cursor_y1 >= _height)
    {
        actual_cursor_y1 = _height - 1;
    }

    setAddrWindow(actual_cursor_x, actual_cursor_y, actual_cursor_x1, actual_cursor_y1);

    for(int16_t Y = 0; Y < h; Y++)
    {
        for(int16_t X = 0; X < w; X++)
        {
            int16_t index = X + Y * w;
            int16_t actualX = x + X;
            int16_t actualY = y + Y;
            if(actualX >= 0 && actualX < _width && actualY >= 0 && actualY < _height)
            {
                writeData16(bitmap[index]);
            }
        }
    }
}

#define SPIx SPI1
#define SPI_I2S_GET_FLAG(SPI_I2S_FLAG) (SPIx->STS & SPI_I2S_FLAG)
#define SPI_I2S_TXDATA(data)           (SPIx->DT = data)
#define SPI_IS_TXE()                   (SPI_I2S_GET_FLAG(SPI_STS_TE))
#define SPI_IS_BUSY()                  (SPI_I2S_GET_FLAG(SPI_STS_BSY))

void Adafruit_ST7789::fillScreen(uint16_t color)
{
    setAddrWindow(0, 0, _width - 1, _height - 1);
    uint32_t size = _width * _height;
    if(hwSPI)
    {
        TFT_CS_CLR;
        TFT_RS_SET;
        while(size--)
        {
            while (!SPI_IS_TXE());
            SPI_I2S_TXDATA(color >> 8);
            while (!SPI_IS_TXE());
            SPI_I2S_TXDATA(color);
        }
        while (!SPI_IS_TXE());
        while (SPI_IS_BUSY());
        TFT_CS_SET;
    }
    else
    {
        for(uint32_t i = 0; i < size; i++)
        {
            writeData16(color);
        }
    }
}

void Adafruit_ST7789::drawFastRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
{
    if(!hwSPI)
        return;

    setAddrWindow(x, y, x + w - 1, y + h - 1);
    uint32_t size = w * h;

    if(hwSPI)
    {
        TFT_CS_CLR;
        TFT_RS_SET;
        while(size--)
        {
            while (!SPI_IS_TXE());
            SPI_I2S_TXDATA(*bitmap >> 8);
            while (!SPI_IS_TXE());
            SPI_I2S_TXDATA(*bitmap++);
        }
        while (!SPI_IS_TXE());
        while (SPI_IS_BUSY());
        TFT_CS_SET;
    }
}
