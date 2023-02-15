#pragma once

#define BLACK 0
#define WHITE 1

#include <Arduino.h>
#include "EPaperDriver.h"
#include "Fonts/Fonts.h"

// Draws on a black and white image
class EPaperDisplay : public EPaperDriver
{
private:
    unsigned int ImgWidth;
    unsigned int ImgHeight;

    void SetSize(unsigned int imgWidth, unsigned int imgHeight);
    
    void ClearRegion(int x, int y, unsigned int width, unsigned int height, int color);
    void DrawPixel(int x, int y, int color);
    void DrawChar(int x, int y, char asciiChar, sFONT* font, int color);

public:
    unsigned char* Image;

    EPaperDisplay(unsigned int width, unsigned int height, EPaperPinConfig pinConfig);

    void Clear(int color);
    void DrawString(int x, int y, const char* text, sFONT* font, int color);
    void DrawLine(int x0, int y0, int x1, int y1, int color);
    void DrawHorizontalLine(int x, int y, int lineWidth, int color);
    void DrawVerticalLine(int x, int y, int lineHeight, int color);
    void DrawRectangle(int x0, int y0, int x1, int y1, int color);
    void DrawFilledRectangle(int x0, int y0, int x1, int y1, int color);
    void DrawCircle(int x, int y, int radius, int color);
    void DrawFilledCircle(int x, int y, int radius, int color);
};