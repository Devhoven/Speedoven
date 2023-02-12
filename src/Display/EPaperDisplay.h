#pragma once

#define SPI_CLOCK_SPEED 2000000

class EPaperDisplay
{
private:
    unsigned int RstPin;
    unsigned int DCPin;
    unsigned int CSPin;
    unsigned int BusyPin;
    unsigned int ClkPin;
    unsigned int DinPin;

    void InitSpi();
    void SpiTransfer(unsigned char data);
    void SpiTransfer(void* data, size_t length);

    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void SendData(void* data, size_t length);

    void SetLut(unsigned char *lut);
    void SetLutByHost(unsigned char *lut);
    void SetMemoryArea(int xStart, int yStart, int xEnd, int yEnd);
    void SetMemoryPointer(int x, int y);

    void WaitUntilIdle();

public:
    unsigned int Width;
    unsigned int Height;

    EPaperDisplay(unsigned int width, unsigned int height, 
                  unsigned int busyPin, unsigned int rstPin, unsigned int dcPin,
                  unsigned int csPin, unsigned int clkPin, unsigned int dinPin);
    void Init();
    void Reset();
    void Sleep();

    void ClearFrameMemory(unsigned char color);
    void SetFrameMemory(const unsigned char* imgBuf, 
                        int x, int y, 
                        int imgWidth, int imgHeight);
    void SetFrameMemoryPartial(const unsigned char* imgBuf, 
                                int x, int y, 
                                int imgWidth, int imgHeight);
                                
    void DisplayFrame();
    void DisplayFramePartial();
};