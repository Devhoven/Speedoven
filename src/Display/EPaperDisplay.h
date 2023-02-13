#pragma once

// Display orientation
#define ROTATE_0   0
#define ROTATE_90  1
#define ROTATE_180 2
#define ROTATE_270 3

// Color inverse. 1 or 0 = set or reset a bit if set a color pixel
#define IF_INVERT_COLOR 1

#include <Arduino.h>
#include "EPaperDriver.h"
#include "Fonts/Fonts.h"

// Draws on a black and white image
class EPaperDisplay : public EPaperDriver
{
private:
    unsigned int Width;
    unsigned int Height;
    uint8_t Rotation;
    
public:
    unsigned char* Image;

    EPaperDisplay(unsigned int width, unsigned int height, uint8_t rotation,
                  EPaperPinConfig pinConfig);
    
    int GetWidth();
    int GetHeight();
    int GetRotate();
    void SetRotate(uint8_t rotation);

    void Clear(int color);
    void DrawAbsolutePixel(int x, int y, int color);
    void DrawPixel(int x, int y, int color);
    void DrawChar(int x, int y, char ascii_char, sFONT* font, int color);
    void DrawString(int x, int y, const char* text, sFONT* font, int color);
    void DrawLine(int x0, int y0, int x1, int y1, int color);
    void DrawHorizontalLine(int x, int y, int width, int color);
    void DrawVerticalLine(int x, int y, int height, int color);
    void DrawRectangle(int x0, int y0, int x1, int y1, int color);
    void DrawFilledRectangle(int x0, int y0, int x1, int y1, int color);
    void DrawCircle(int x, int y, int radius, int color);
    void DrawFilledCircle(int x, int y, int radius, int color);
};