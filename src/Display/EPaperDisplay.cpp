#include <Arduino.h>
#include "EPaperDisplay.h"
#include "EPaperDriver.h"

EPaperDisplay::EPaperDisplay(uint16_t width, uint16_t height, EPaperPinConfig pinConfig) 
                             : EPaperDriver(width, height, pinConfig),
                             ImgWidth{0}, ImgHeight{0}
{
    
    SetSize(Width, Height);
    Image = new unsigned char[ImgWidth * ImgHeight / 8];
}

// Clears the whole display
void EPaperDisplay::Clear(uint8_t color)
{
    memset(Image, color ? 0xFF : 0, Width * Height / 8);
}

// Draws a pixel by absolute coordinates
void EPaperDisplay::DrawPixel(uint16_t x, uint16_t y, uint8_t color)
{
    if (color)
        Image[(x + y * Width) / 8] |= 0x80 >> (x % 8);
    else
        Image[(x + y * Width) / 8] &= ~(0x80 >> (x % 8));
}

void EPaperDisplay::SetSize(uint16_t imgWidth, uint16_t imgHeight){

    // 1 byte = 8 pixels, so the width should be the multiple of 8 
    ImgWidth = imgWidth % 8 ? imgWidth + 8 - (imgWidth % 8) : imgWidth;
    ImgHeight = imgHeight;
}

// Draws a character
void EPaperDisplay::DrawChar(uint16_t x, uint16_t y, char asciiChar, sFONT* font, uint8_t color)
{
    uint16_t char_offset = (asciiChar - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &font->table[char_offset];   

    for (uint16_t i = 0; i < font->Height; i++)
    {
        for (uint16_t j = 0; j < font->Width; j++)
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
void EPaperDisplay::DrawString(uint16_t x, uint16_t y, const char* text, sFONT* font, uint8_t color)
{
    size_t textLength = strlen(text);

    SetSize(font->Width * textLength, font->Height);

    // Send the string character by character 
    for (uint16_t i = 0; i < textLength; i++)
        DrawChar(i * font->Width + x, y, *(text + i), font, color);

    SetFrameMemoryPartial(Image, x, y, ImgWidth, ImgHeight);
}

// Draws a line 
void EPaperDisplay::DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color)
{
    /* Bresenham algorithm */
    uint16_t dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    uint16_t sx = x0 < x1 ? 1 : -1;
    uint16_t dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    uint16_t sy = y0 < y1 ? 1 : -1;
    uint16_t err = dx + dy;

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

    uint16_t minX = min(x0, x1);
    uint16_t maxX = max(x0, x1);
    uint16_t minY = min(y0, y1);
    uint16_t maxY = max(y0, y1);
    SetFrameMemoryPartial(Image, minX, minY, maxX - minX, maxY - minY);
}

// Draws a horizontal line
void EPaperDisplay::DrawHorizontalLine(uint16_t x, uint16_t y, uint16_t lineWidth, uint8_t color)
{
    for (uint16_t i = x; i < x + lineWidth; i++)
        DrawPixel(i, y, color);

    SetFrameMemoryPartial(Image, x, y, lineWidth, 1);
}

// Draws a vertical line
void EPaperDisplay::DrawVerticalLine(uint16_t x, uint16_t y, uint16_t lineHeight, uint8_t color)
{
    for (uint16_t i = y; i < y + lineHeight; i++)
        DrawPixel(x, i, color);
    
    SetFrameMemoryPartial(Image, x, y, 1, lineHeight);
}

// Draws a rectangle
void EPaperDisplay::DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color)
{
    uint16_t minX, minY, maxX, maxY;
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
void EPaperDisplay::DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color)
{
    uint16_t min_x, min_y, max_x, max_y;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;

    for (uint16_t i = min_x; i <= max_x; i++)
        DrawVerticalLine(i, min_y, max_y - min_y + 1, color);
}

// Draws a circle
void EPaperDisplay::DrawCircle(uint16_t x, uint16_t y, uint16_t radius, uint8_t color)
{
    /* Bresenham algorithm */
    uint16_t x_pos = -radius;
    uint16_t y_pos = 0;
    uint16_t err = 2 - 2 * radius;
    uint16_t e2;

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
void EPaperDisplay::DrawFilledCircle(uint16_t x, uint16_t y, uint16_t radius, uint8_t color)
{
    /* Bresenham algorithm */
    uint16_t x_pos = -radius;
    uint16_t y_pos = 0;
    uint16_t err = 2 - 2 * radius;
    uint16_t e2;

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
