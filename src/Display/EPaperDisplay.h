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

    void SetLut(unsigned char *lut);
    void SetLut_by_host(unsigned char *lut);
    void SetMemoryArea(int xStart, int yStart, int xEnd, int yEnd);
    void SetMemoryPointer(int x, int y);

public:
    unsigned int Width;
    unsigned int Height;

    EPaperDisplay(unsigned int width, 
                  unsigned int height, 
                  unsigned int BusyPin,
                  unsigned int RstPin,
                  unsigned int DCPin,
                  unsigned int CSPin,
                  unsigned int ClkPin,
                  unsigned int DinPin);
    int Init();
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void WaitUntilIdle(void);
    void Reset(void);
    void SetFrameMemory(const unsigned char *imgBuf, 
                        int x, int y, 
                        int imgWidth, int imgHeight);
    void SetFrameMemory_Partial(const unsigned char *imgBuf, 
                                int x, int y, 
                                int imgWidth, int imgHeight);
    void ClearFrameMemory(unsigned char color);
    void DisplayFrame(void);
    void DisplayFrame_Partial(void);
    void Sleep(void);
};