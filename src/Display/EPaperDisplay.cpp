#include <Arduino.h>
#include "EPaperDisplay.h"
#include "EPaperDriver.h"

EPaperDisplay::EPaperDisplay(unsigned int width, unsigned int height, EPaperPinConfig pinConfig) 
                             : EPaperDriver(width, height, pinConfig),
                             ImgWidth{0}, ImgHeight{0}
{
    
    SetSize(Width, Height);
    Image = new unsigned char[ImgWidth * ImgHeight / 8];
}

// Clears the whole display
void EPaperDisplay::Clear(int color)
{
    memset(Image, color ? 0xFF : 0, Width * Height / 8);
}

// Clears a region of memory on the display in the set color
// x and width have to be a multiple of 8 
void EPaperDisplay::ClearRegion(int x, int y, unsigned int width, unsigned int height, int color)
{
    memset((Image + x / 8 + y), color ? 0xFF : 0, width * height / 8);
}

// Draws a pixel by absolute coordinates
void EPaperDisplay::DrawPixel(int x, int y, int color)
{
    if (color)
        Image[(x + y * Width) / 8] |= 0x80 >> (x % 8);
    else
        Image[(x + y * Width) / 8] &= ~(0x80 >> (x % 8));
}

void EPaperDisplay::SetSize(unsigned int imgWidth, unsigned int imgHeight){

    // 1 byte = 8 pixels, so the width should be the multiple of 8 
    ImgWidth = imgWidth % 8 ? imgWidth + 8 - (imgWidth % 8) : imgWidth;
    ImgHeight = imgHeight;
}

// Draws a character
void EPaperDisplay::DrawChar(int x, int y, char asciiChar, sFONT* font, int color)
{
    unsigned int char_offset = (asciiChar - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &font->table[char_offset];   

    for (int i = 0; i < font->Height; i++)
    {
        for (int j = 0; j < font->Width; j++)
        {
            if (*ptr & (0x80 >> (j % 8)))
                DrawPixel(x + j, y + i, color);
            else 
                DrawPixel(x + j, y + i, WHITE);

            if (j % 8 == 7)
                ptr++;
        }
        if (font->Width % 8 != 0)
            ptr++;
    }
}

// Draws a string
void EPaperDisplay::DrawString(int x, int y, const char* text, sFONT* font, int color)
{
    size_t textLength = strlen(text);

    SetSize(font->Width * textLength, font->Height);

    // Send the string character by character 
    for (int i = 0; i < textLength; i++)
        DrawChar(i * font->Width + x, y, *(text + i), font, color);

    SetFrameMemoryPartial(Image, x, y, ImgWidth, ImgHeight);
}

// Draws a line 
void EPaperDisplay::DrawLine(int x0, int y0, int x1, int y1, int color)
{
    /* Bresenham algorithm */
    int dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int sx = x0 < x1 ? 1 : -1;
    int dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while ((x0 != x1) && (y0 != y1))
    {
        DrawPixel(x0, y0, color);
        if (2 * err >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (2 * err <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }

    int minX = min(x0, x1);
    int maxX = max(x0, x1);
    int minY = min(y0, y1);
    int maxY = max(y0, y1);
    SetFrameMemoryPartial(Image, minX, minY, maxX - minX, maxY - minY);
}

// Draws a horizontal line
void EPaperDisplay::DrawHorizontalLine(int x, int y, int lineWidth, int color)
{
    for (int i = x; i < x + lineWidth; i++)
        DrawPixel(i, y, color);

    SetFrameMemoryPartial(Image, x, y, lineWidth, 1);
}

// Draws a vertical line
void EPaperDisplay::DrawVerticalLine(int x, int y, int lineHeight, int color)
{
    for (int i = y; i < y + lineHeight; i++)
        DrawPixel(x, i, color);
    
    SetFrameMemoryPartial(Image, x, y, 8, lineHeight);
}

// Draws a rectangle
void EPaperDisplay::DrawRectangle(int x0, int y0, int x1, int y1, int color)
{
    int minX, minY, maxX, maxY;
    minX = min(x0, x1);
    maxX = max(x0, x1);
    minY = min(y0, y1);
    maxY = max(y0, y1);

    DrawHorizontalLine(minX, minY, maxX - minX + 1, color);
    DrawHorizontalLine(minX, maxY, maxX - minX + 1, color);
    DrawVerticalLine  (minX, minY, maxY - minY + 1, color);
    DrawVerticalLine  (maxX, minY, maxY - minY + 1, color);
}

// Draws a filled rectangle
void EPaperDisplay::DrawFilledRectangle(int x0, int y0, int x1, int y1, int color)
{
    int min_x, min_y, max_x, max_y;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;

    for (int i = min_x; i <= max_x; i++)
        DrawVerticalLine(i, min_y, max_y - min_y + 1, color);
}

// Draws a circle
void EPaperDisplay::DrawCircle(int x, int y, int radius, int color)
{
    /* Bresenham algorithm */
    int x_pos = -radius;
    int y_pos = 0;
    int err = 2 - 2 * radius;
    int e2;

    do
    {
        DrawPixel(x - x_pos, y + y_pos, color);
        DrawPixel(x + x_pos, y + y_pos, color);
        DrawPixel(x + x_pos, y - y_pos, color);
        DrawPixel(x - x_pos, y - y_pos, color);
        e2 = err;
        if (e2 <= y_pos)
        {
            err += ++y_pos * 2 + 1;
            if (-x_pos == y_pos && e2 <= x_pos)
            {
                e2 = 0;
            }
        }
        if (e2 > x_pos)
        {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos <= 0);
}

// Draws a filled circle
void EPaperDisplay::DrawFilledCircle(int x, int y, int radius, int color)
{
    /* Bresenham algorithm */
    int x_pos = -radius;
    int y_pos = 0;
    int err = 2 - 2 * radius;
    int e2;

    do
    {
        DrawPixel(x - x_pos, y + y_pos, color);
        DrawPixel(x + x_pos, y + y_pos, color);
        DrawPixel(x + x_pos, y - y_pos, color);
        DrawPixel(x - x_pos, y - y_pos, color);
        DrawHorizontalLine(x + x_pos, y + y_pos, 2 * (-x_pos) + 1, color);
        DrawHorizontalLine(x + x_pos, y - y_pos, 2 * (-x_pos) + 1, color);
        e2 = err;
        if (e2 <= y_pos)
        {
            err += ++y_pos * 2 + 1;
            if (-x_pos == y_pos && e2 <= x_pos)
                e2 = 0;
        }
        if (e2 > x_pos)
            err += ++x_pos * 2 + 1;

    } while (x_pos <= 0);
}
