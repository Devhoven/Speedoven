#include <Arduino.h>
#include <stdlib.h>
#include <SPI.h>
#include "EPaperDisplay.h"

unsigned char _WF_PARTIAL_2IN9[159] =
{
    0x0,
    0x40,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x80,
    0x80,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x40,
    0x40,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x80,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0A,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x2,
    0x1,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x1,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x22,
    0x22,
    0x22,
    0x22,
    0x22,
    0x22,
    0x0,
    0x0,
    0x0,
    0x22,
    0x17,
    0x41,
    0xB0,
    0x32,
    0x36,
};

unsigned char WS_20_30[159] =
{
    0x80, 0x66, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x40, 0x0, 0x0, 0x0,
    0x10, 0x66, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20, 0x0, 0x0, 0x0,
    0x80, 0x66, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x40, 0x0, 0x0, 0x0,
    0x10, 0x66, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x14, 0x8, 0x0, 0x0, 0x0, 0x0, 0x1,
    0xA, 0xA, 0x0, 0xA, 0xA, 0x0, 0x1,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x14, 0x8, 0x0, 0x1, 0x0, 0x0, 0x1,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x0, 0x0, 0x0,
    0x22, 0x17, 0x41, 0x0, 0x32, 0x36
};

EPaperDisplay::EPaperDisplay(unsigned int width, 
                             unsigned int height, 
                             unsigned int busyPin,
                             unsigned int rstPin,
                             unsigned int dcPin,
                             unsigned int csPin,
                             unsigned int clkPin,
                             unsigned int dinPin)
{
    this->Width   = width;
    this->Height  = height;
    this->BusyPin = busyPin;
    this->RstPin  = rstPin;
    this->DCPin   = dcPin;
    this->CSPin   = csPin;
    this->ClkPin  = clkPin;
    this->DinPin  = dinPin;
}

void EPaperDisplay::InitSpi()
{
    pinMode(CSPin, OUTPUT);
    pinMode(RstPin, OUTPUT);
    pinMode(DCPin, OUTPUT);
    pinMode(BusyPin, INPUT);
    // -1 = no MISO - PIN
    SPI.begin(ClkPin, -1, DinPin);
    SPI.beginTransaction(SPISettings(SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE0));
}

int EPaperDisplay::Init()
{
    InitSpi();

    Reset();

    /* EPD hardware init start */
    WaitUntilIdle();
    SendCommand(0x12); // SWRESET
    WaitUntilIdle();

    SendCommand(0x01); // Driver output control
    SendData(0x27);
    SendData(0x01);
    SendData(0x00);

    SendCommand(0x11); // data entry mode
    SendData(0x03);

    SetMemoryArea(0, 0, Width - 1, Height - 1);

    SendCommand(0x21); //  Display update control
    SendData(0x00);
    SendData(0x80);

    SetMemoryPointer(0, 0);
    WaitUntilIdle();

    SetLut_by_host(WS_20_30);
    /* EPD hardware init end */
    return 0;
}

void EPaperDisplay::SpiTransfer(unsigned char data)
{
    digitalWrite(CSPin, LOW);
    SPI.transfer(data);
    digitalWrite(CSPin, HIGH);
}

/**
 *  @brief: basic function for sending commands
 */
void EPaperDisplay::SendCommand(unsigned char command)
{
    digitalWrite(DCPin, LOW);
    digitalWrite(CSPin, LOW);
    SpiTransfer(command);
    digitalWrite(CSPin, HIGH);
}

/**
 *  @brief: basic function for sending data
 */
void EPaperDisplay::SendData(unsigned char data)
{
    digitalWrite(DCPin, HIGH);
    digitalWrite(CSPin, LOW);
    SpiTransfer(data);
    digitalWrite(CSPin, HIGH);
}

/**
 *  @brief: Wait until the busy_pin goes LOW
 */
void EPaperDisplay::WaitUntilIdle(void)
{
    while (digitalRead(BusyPin) != LOW)
        delay(5);
    delay(5);
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void EPaperDisplay::Reset(void)
{
    digitalWrite(RstPin, HIGH);
    delay(20);
    digitalWrite(RstPin, LOW); // module reset
    delay(5);
    digitalWrite(RstPin, HIGH);
    delay(20);
}

/**
 *  @brief: put an image buffer to the frame memory.
 *          this won't update the display.
 */
void EPaperDisplay::SetFrameMemory(const unsigned char *imgBuf, 
                                   int x, int y, 
                                   int imgWidth, int imgHeight)
{
    int xEnd;
    int yEnd;

    if (imgBuf == NULL ||
        x < 0 || imgWidth < 0 ||
        y < 0 || imgHeight < 0)
        return;

    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    x &= 0xF8;
    imgWidth &= 0xF8;
    if (x + imgWidth >= this->Width)
        xEnd = this->Width - 1;
    else
        xEnd = x + imgWidth - 1;

    if (y + imgHeight >= this->Height)
        yEnd = this->Height - 1;
    else
        yEnd = y + imgHeight - 1;

    SetMemoryArea(x, y, xEnd, yEnd);
    SetMemoryPointer(x, y);
    SendCommand(0x24);
    /* send the image data */
    for (int j = 0; j < yEnd - y + 1; j++)
        for (int i = 0; i < (xEnd - x + 1) / 8; i++)
            SendData(imgBuf[i + j * (imgWidth / 8)]);
}

void EPaperDisplay::SetFrameMemory_Partial(const unsigned char *imgBuf, 
                                           int x, int y, 
                                           int imgWidth, int imgHeight)
{
    int xEnd;
    int yEnd;

    if (imgBuf == NULL ||
        x < 0 || imgWidth < 0 ||
        y < 0 || imgHeight < 0)
        return;

    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    x &= 0xF8;
    imgWidth &= 0xF8;
    if (x + imgWidth >= this->Width)
        xEnd = this->Width - 1;
    else
        xEnd = x + imgWidth - 1;

    if (y + imgHeight >= this->Height)
        yEnd = this->Height - 1;
    else
        yEnd = y + imgHeight - 1;

    digitalWrite(RstPin, LOW);
    delay(2);
    digitalWrite(RstPin, HIGH);
    delay(2);

    SetLut(_WF_PARTIAL_2IN9);
    SendCommand(0x37);
    SendData(0x00);
    SendData(0x00);
    SendData(0x00);
    SendData(0x00);
    SendData(0x00);
    SendData(0x40);
    SendData(0x00);
    SendData(0x00);
    SendData(0x00);
    SendData(0x00);

    SendCommand(0x3C); // BorderWavefrom
    SendData(0x80);

    SendCommand(0x22);
    SendData(0xC0);
    SendCommand(0x20);
    WaitUntilIdle();

    SetMemoryArea(x, y, xEnd, yEnd);
    SetMemoryPointer(x, y);
    SendCommand(0x24);
    /* send the image data */
    for (int j = 0; j < yEnd - y + 1; j++)
    {
        for (int i = 0; i < (xEnd - x + 1) / 8; i++)
        {
            SendData(imgBuf[i + j * (imgWidth / 8)]);
        }
    }
}

/**
 *  @brief: clears both memory buffers with the specified color.
 *          this won't update the display.
 */
void EPaperDisplay::ClearFrameMemory(unsigned char color)
{
    SetMemoryArea(0, 0, this->Width - 1, this->Height - 1);
    SetMemoryPointer(0, 0);
    SendCommand(0x24);
    /* send the color data */
    for (int i = 0; i < this->Width / 8 * this->Height; i++)
        SendData(color);

    SendCommand(0x26);
    /* send the color data */
    for (int i = 0; i < this->Width / 8 * this->Height; i++)
        SendData(color);
}

/**
 *  @brief: update the display
 *          there are 2 memory areas embedded in the e-paper display
 *          but once this function is called,
 *          the the next action of SetFrameMemory or ClearFrame will
 *          set the other memory area.
 */
void EPaperDisplay::DisplayFrame(void)
{
    SendCommand(0x22);
    SendData(0xc7);
    SendCommand(0x20);
    WaitUntilIdle();
}

void EPaperDisplay::DisplayFrame_Partial(void)
{
    SendCommand(0x22);
    SendData(0x0F);
    SendCommand(0x20);
    WaitUntilIdle();
}

void EPaperDisplay::SetLut(unsigned char *lut)
{
    unsigned char count;
    SendCommand(0x32);
    for (count = 0; count < 153; count++)
        SendData(lut[count]);
    WaitUntilIdle();
}

void EPaperDisplay::SetLut_by_host(unsigned char *lut)
{
    SetLut((unsigned char *)lut);
    SendCommand(0x3f);
    SendData(*(lut + 153));
    SendCommand(0x03); // gate voltage
    SendData(*(lut + 154));
    SendCommand(0x04);      // source voltage
    SendData(*(lut + 155)); // VSH
    SendData(*(lut + 156)); // VSH2
    SendData(*(lut + 157)); // VSL
    SendCommand(0x2c);      // VCOM
    SendData(*(lut + 158));
}

/**
 *  @brief: private function to specify the memory area for data R/W
 */
void EPaperDisplay::SetMemoryArea(int xStart, int yStart, int x_end, int y_end)
{
    SendCommand(0x44);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    SendData((xStart >> 3) & 0xFF);
    SendData((x_end >> 3) & 0xFF);
    SendCommand(0x45);
    SendData(yStart & 0xFF);
    SendData((yStart >> 8) & 0xFF);
    SendData(y_end & 0xFF);
    SendData((y_end >> 8) & 0xFF);
}

/**
 *  @brief: private function to specify the start point for data R/W
 */
void EPaperDisplay::SetMemoryPointer(int x, int y)
{
    SendCommand(0x4E);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    SendData((x >> 3) & 0xFF);
    SendCommand(0x4F);
    SendData(y & 0xFF);
    SendData((y >> 8) & 0xFF);
    WaitUntilIdle();
}

/**
 *  @brief: After this command is transmitted, the chip would enter the
 *          deep-sleep mode to save power.
 *          The deep sleep mode would return to standby by hardware reset.
 *          You can use Epd::Init() to awaken
 */
void EPaperDisplay::Sleep()
{
    SendCommand(0x10);
    SendData(0x01);
    // WaitUntilIdle();
}