#pragma once
#include <stdlib.h>

#define SPI_CLOCK_SPEED 2000000

struct EPaperPinConfig
{
    uint16_t BusyPin;
    uint16_t RstPin;
    uint16_t DCPin;
    uint16_t CSPin;
    uint16_t ClkPin;
    uint16_t DinPin;
};

class EPaperDriver
{
protected:
    uint16_t Width;
    uint16_t Height;

private:
    EPaperPinConfig PinConfig;

    void InitSpi();
    void SpiTransfer(unsigned char data);
    void SpiTransfer(void* data, uint32_t length);

    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void SendData(void* data, uint32_t length);

    void SetLut(unsigned char *lut);
    void SetLutByHost(unsigned char *lut);
    void SetMemoryArea(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
    void SetMemoryPointer(uint16_t x, uint16_t y);

    void WaitUntilIdle();

public:
    EPaperDriver(uint16_t width, uint16_t height, EPaperPinConfig pinConfig);
    
    void Init();
    void Reset();
    void Sleep();

    uint16_t GetHeight();
    uint16_t GetWidth();

    void ClearFrameMemory(unsigned char color);
    void SetFrameMemory(const unsigned char* imgBuf, 
                        uint16_t x, uint16_t y, uint16_t imgWidth, uint16_t imgHeight);
    void SetFrameMemoryPartial(const unsigned char* imgBuf, 
                               uint16_t x, uint16_t y,  uint16_t imgWidth, uint16_t imgHeight);
                                
    void DisplayFrame();
    void DisplayFramePartial();
};