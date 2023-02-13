#pragma once
#include <stdlib.h>

#define SPI_CLOCK_SPEED 2000000

struct EPaperPinConfig
{
    unsigned int BusyPin;
    unsigned int RstPin;
    unsigned int DCPin;
    unsigned int CSPin;
    unsigned int ClkPin;
    unsigned int DinPin;
};

class EPaperDriver
{
private:
    unsigned int Width;
    unsigned int Height;
    EPaperPinConfig PinConfig;

    void InitSpi();
    void SpiTransfer(unsigned char data);
    void SpiTransfer(void* data, uint32_t length);

    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void SendData(void* data, uint32_t length);

    void SetLut(unsigned char *lut);
    void SetLutByHost(unsigned char *lut);
    void SetMemoryArea(int xStart, int yStart, int xEnd, int yEnd);
    void SetMemoryPointer(int x, int y);

    void WaitUntilIdle();

public:
    EPaperDriver(unsigned int width, unsigned int height, EPaperPinConfig pinConfig);
    void Init();
    void Reset();
    void Sleep();

    void ClearFrameMemory(unsigned char color);
    void SetFrameMemory(const unsigned char* imgBuf, 
                        int x, int y, int imgWidth, int imgHeight);
    void SetFrameMemoryPartial(const unsigned char* imgBuf, 
                               int x, int y,  int imgWidth, int imgHeight);
                                
    void DisplayFrame();
    void DisplayFramePartial();
};