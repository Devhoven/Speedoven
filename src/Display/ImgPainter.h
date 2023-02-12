#pragma once

// Display orientation
#define ROTATE_0 0
#define ROTATE_90 1
#define ROTATE_180 2
#define ROTATE_270 3

// Color inverse. 1 or 0 = set or reset a bit if set a colored pixel
#define IF_INVERT_COLOR 1

#include "Fonts/Fonts.h"

// Draws on a black and white image
class ImgPainter
{
private:
    unsigned char *Image;
    int Width;
    int Height;
    int Rotate;

public:
    ImgPainter(int width, int height);
    
    int GetWidth();
    void SetWidth(int width);
    int GetHeight();
    void SetHeight(int height);
    int GetRotate();
    void SetRotate(int rotate);
    unsigned char *GetImage();

    void Clear(int colored);
    void DrawAbsolutePixel(int x, int y, int colored);
    void DrawPixel(int x, int y, int colored);
    void DrawCharAt(int x, int y, char ascii_char, sFONT *font, int colored);
    void DrawStringAt(int x, int y, const char *text, sFONT *font, int colored);
    void DrawLine(int x0, int y0, int x1, int y1, int colored);
    void DrawHorizontalLine(int x, int y, int width, int colored);
    void DrawVerticalLine(int x, int y, int height, int colored);
    void DrawRectangle(int x0, int y0, int x1, int y1, int colored);
    void DrawFilledRectangle(int x0, int y0, int x1, int y1, int colored);
    void DrawCircle(int x, int y, int radius, int colored);
    void DrawFilledCircle(int x, int y, int radius, int colored);
};
