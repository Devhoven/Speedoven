#include <Arduino.h>
#include <stdlib.h>
#include <SPI.h>
#include "EPaperDisplay.h"

unsigned char PARTIAL_REFRESH_WAVEFORM[159] =
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

unsigned char INIT_WAVEFORM[159] =
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


EPaperDisplay::EPaperDisplay(unsigned int width, unsigned int height, 
                             unsigned int busyPin, unsigned int rstPin, unsigned int dcPin,
                             unsigned int csPin, unsigned int clkPin, unsigned int dinPin) 
                             : Width{width}, Height{height}, BusyPin{busyPin}, RstPin{rstPin}, DCPin{dcPin}, CSPin{csPin}, ClkPin{clkPin}, DinPin{dinPin}
{ }

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

// Sends a byte of data via SPI
void EPaperDisplay::SpiTransfer(unsigned char data)
{
    digitalWrite(CSPin, LOW);
    SPI.transfer(data);
    digitalWrite(CSPin, HIGH);
}

// Sends an array of data via SPI
void EPaperDisplay::SpiTransfer(void* data, size_t length)
{
    digitalWrite(CSPin, LOW);
    SPI.transfer(data, length);
    digitalWrite(CSPin, HIGH);
}

// Sends a command 
void EPaperDisplay::SendCommand(unsigned char command)
{
    digitalWrite(DCPin, LOW);
    SpiTransfer(command);
}

// Sends one byte of data
void EPaperDisplay::SendData(unsigned char data)
{
    digitalWrite(DCPin, HIGH);
    SpiTransfer(data);
}

// Sends an array of data to the display
void EPaperDisplay::SendData(void* data, size_t length)
{
    digitalWrite(DCPin, HIGH);
    SpiTransfer(data, length);
}

// No idea what SetLut and SetLutByHost do exactly, but they are needed for initialization and partial refresh
void EPaperDisplay::SetLut(unsigned char *lut)
{
    // SendCommand(0x32);
    // SendData(lut, 153);
    // WaitUntilIdle();
    unsigned char count;
    SendCommand(0x32);
    for (count = 0; count < 153; count++)
        SendData(lut[count]);
    WaitUntilIdle();
}
void EPaperDisplay::SetLutByHost(unsigned char *lut)
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

// private function to specify the memory area for data R/W
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

// private function to specify the start point for data R/W
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

// Wait until the busy_pin goes LOW
void EPaperDisplay::WaitUntilIdle()
{
    while (digitalRead(BusyPin) != LOW)
        delay(1);
    delay(5);
}

// Initializes the display
void EPaperDisplay::Init()
{
    /* Hardware init start */
    InitSpi();

    Reset();

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

    SetLutByHost(INIT_WAVEFORM);
    /* Hardware init end */

    // Clears both memroy buffers of the display to white, the background's noisy otherwise
    ClearFrameMemory(0xFF);
    DisplayFrame();
}

// Module reset. 
// Can be used to wake the module out of deep sleep
void EPaperDisplay::Reset()
{
    digitalWrite(RstPin, HIGH);
    delay(20);
    digitalWrite(RstPin, LOW); // module reset
    delay(5);
    digitalWrite(RstPin, HIGH);
    delay(20);
}

// Clears both memory buffers with the specified color.
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

// Writes an image into the frame memory at a specified position
void EPaperDisplay::SetFrameMemory(const unsigned char *imgBuf, 
                                   int x, int y, 
                                   int imgWidth, int imgHeight)
{
    if (x + imgWidth > Width)
        imgWidth = Width - x;
    if (y + imgHeight > Height)
        imgHeight = Height - y;

    // x point must be the multiple of 8 or the last 3 bits will be ignored 
    x &= 0xF8;
    imgWidth &= 0xF8;

    SetMemoryArea(x, y, x + imgWidth - 1, y + imgHeight - 1);
    SetMemoryPointer(x, y);
    SendCommand(0x24);

    SendData((void*)imgBuf, imgWidth * imgHeight / 8);
}

// Writes an image into the frame memory at a specified position, for partial refresh
void EPaperDisplay::SetFrameMemoryPartial(const unsigned char *imgBuf, 
                                           int x, int y, 
                                           int imgWidth, int imgHeight)
{
    if (x + imgWidth > Width)
        imgWidth = Width - x;
    if (y + imgHeight > Height)
        imgHeight = Height - y;

    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    x &= 0xF8;
    imgWidth &= 0xF8;

    digitalWrite(RstPin, LOW);
    delay(2);
    digitalWrite(RstPin, HIGH);
    delay(2);

    SetLut(PARTIAL_REFRESH_WAVEFORM);
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
    
    SetMemoryArea(x, y, x + imgWidth - 1, y + imgHeight - 1);
    SetMemoryPointer(x, y);
    SendCommand(0x24);

    SendData((void*)imgBuf, imgWidth * imgHeight / 8);
}

// The display has got two internal memory buffers
// One contains the currently displayed content of the display, the other has the data for the next frame and get's written on with SetFrameMemory
// This method swaps the two buffers, so that the buffer with the new content gets shown
void EPaperDisplay::DisplayFrame()
{
    SendCommand(0x22);
    SendData(0xc7);
    SendCommand(0x20);
    WaitUntilIdle();
}

// Does the same as DisplayFrame, but for a partial refresh
void EPaperDisplay::DisplayFramePartial()
{
    SendCommand(0x22);
    SendData(0x0F);
    SendCommand(0x20);
    WaitUntilIdle();
}

// After this command is transmitted, the chip would enter the deep-sleep mode to save power.
// The deep sleep mode would return to standby by hardware reset. 
// Epd::Init() can be used to awaken the display
void EPaperDisplay::Sleep()
{
    SendCommand(0x10);
    SendData(0x01);
    // WaitUntilIdle();
}