#include <stdlib.h>
#include "../../Display/EPaperDisplay.h"

class Graph 
{
private: 
    EPaperDisplay* Display;
    uint16_t PosX, PosY;
    uint16_t Width, Height;
    uint8_t* Data;

    uint16_t Counter;

public: 
    Graph(EPaperDisplay* display, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

    void AddNextDataPoint(uint8_t point);

    void Draw();
};