#include "Graph.h"


Graph::Graph(EPaperDisplay* display, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t ) 
    : Display(display), PosX(x), PosY(y), Width(width), Height(height), Counter(1)
{
    Data = new uint8_t[Width - 1];
}

void Graph::AddNextDataPoint(uint8_t point)
{
    Counter++;

    if (Counter >= Width)
        Counter = 1;

    Data[Counter - 1] = point;
}

void Graph::Draw()
{
    Display->DrawHorizontalLine(PosX, PosY, Width);
    Display->DrawVerticalLine(PosX, PosY - Height, Height);

    for (uint16_t i = 0; i < Counter; i++)
    {
        Display->DrawPixel()
    }
    
}