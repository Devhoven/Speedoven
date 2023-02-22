#include "Graph.h"
#include "../../Display/Fonts/NotoSans16x24.h"

Graph::Graph(EPaperDisplay* display, uint16_t x, uint16_t y, uint16_t width, uint16_t height) 
    : Display(display), PosX(x), PosY(y), Width(width), Height(height), Counter(0), MaxValue(0)
{
    // - 1, because the first pixel is the graph line itself
    DataLength = Width - FONT_PADDING_LEFT - 1;
    Data = new uint8_t[DataLength];
}

uint16_t Graph::CalcDataPosY(GraphDataType dataPoint)
{
    return PosY + Height - ((dataPoint / (float)(!MaxValue ? 1 : MaxValue)) * (Height - 1) + 1);
}

uint16_t Graph::CalcDataPosX(uint16_t index)
{
    return PosX + FONT_PADDING_LEFT + index;
}

void Graph::AddNextValue(GraphDataType value)
{
    Data[Counter] = value;

    Counter++;

    if (Counter >= DataLength)
    {
        Counter = 0;
        MaxValue = 0;
    }

    if (value > MaxValue)
        MaxValue = value;
}

void Graph::Draw()
{
    // Drawing the graph-lines
    Display->DrawHorizontalLine(PosX + FONT_PADDING_LEFT, PosY + Height, Width - FONT_PADDING_LEFT + 1);
    Display->DrawVerticalLine(PosX + FONT_PADDING_LEFT, PosY, Height);

    // Drawing the arrow heads
    // Top left, right side
    Display->DrawPixel(PosX + FONT_PADDING_LEFT + 1, PosY + 1); 
    Display->DrawPixel(PosX + FONT_PADDING_LEFT + 2, PosY + 2);
    // Top left, left side
    Display->DrawPixel(PosX + FONT_PADDING_LEFT - 1, PosY + 1);
    Display->DrawPixel(PosX + FONT_PADDING_LEFT - 2, PosY + 2);
    // Bottom right, bottom side
    Display->DrawPixel(PosX + Width - 1, PosY + Height + 1);
    Display->DrawPixel(PosX + Width - 2, PosY + Height + 2);
    // Bottom right, bottom side
    Display->DrawPixel(PosX + Width - 1, PosY + Height - 1);
    Display->DrawPixel(PosX + Width - 2, PosY + Height - 2);

    // Drawing the y-lables
    Display->DrawString(PosX, PosY - 8, String(MaxValue).c_str(), &Font16);
    Display->DrawChar(PosX + 16, PosY + Height - 16, '0', &Font16);

    // The graph data is only shown if there are more than two data points
    if (Counter < 2)
        return;

    // Drawing the graph
    uint16_t prevDataPos = CalcDataPosY(Data[0]);
    Display->DrawPixel(CalcDataPosX(0), prevDataPos);
    for (uint16_t i = 1; i < Counter; i++)
    {
        uint16_t currentDataPos = CalcDataPosY(Data[i]);
        // The queryes are there to draw lines between two data points which are too far apart
        if (prevDataPos == currentDataPos)
        {
            Display->DrawPixel(CalcDataPosX(i), currentDataPos);
        }
        else if (prevDataPos < currentDataPos)
        {
            Display->DrawVerticalLine(CalcDataPosX(i - 1), prevDataPos, currentDataPos - prevDataPos + 1);
            Display->DrawPixel(CalcDataPosX(i), currentDataPos);
        }
        else // prevDataPos > currentDataPos 
        {
            Display->DrawVerticalLine(CalcDataPosX(i), currentDataPos, prevDataPos - currentDataPos + 1);
        }
        prevDataPos = currentDataPos;
    }
}