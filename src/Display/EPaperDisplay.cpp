#include <Arduino.h>
#include "EPaperDisplay.h"
#include "EPaperDriver.h"

unsigned int CalcWidth(unsigned int width)
{
    // 1 byte = 8 pixels, so the width should be the multiple of 8 
    return width % 8 ? width + 8 - (width % 8) : width;
}

EPaperDisplay::EPaperDisplay(unsigned int width, unsigned int height, uint8_t rotation,
                             EPaperPinConfig pinConfig) 
                             : EPaperDriver(width, height, pinConfig),
                             Width{CalcWidth(width)},
                             Height{height}, Rotation{rotation}
{
    Image = new unsigned char[Width * Height / 8];
}

void EPaperDisplay::Clear(int color)
{
    memset(Image, color ? 0xFF : 0, Width * Height / 8);
}

// Draws a pixel by absolute coordinates, no matter the rotation
void EPaperDisplay::DrawAbsolutePixel(int x, int y, int color)
{
    if (x < 0 || x >= Width || y < 0 || y >= Height)
        return;

    if (IF_INVERT_COLOR)
        color = color ? 1 : 0;

    if (color)
        Image[(x + y * Width) / 8] |= 0x80 >> (x % 8);
    else
        Image[(x + y * Width) / 8] &= ~(0x80 >> (x % 8));
}

// Getters and Setters
unsigned char *EPaperDisplay::GetImage()
{
    return Image;
}

int EPaperDisplay::GetWidth()
{
    return Width;
}

int EPaperDisplay::GetHeight()
{
    return Height;
}

int EPaperDisplay::GetRotate()
{
    return Rotation;
}

void EPaperDisplay::SetRotate(int rotation)
{
    Rotation = rotation;
}

// Draws a pixel at the given coordinates, affected by the rotation
void EPaperDisplay::DrawPixel(int x, int y, int color)
{
    if (x < 0 || y < 0)
        return;

    if (Rotation == ROTATE_0)
    {
        if (x >= Width || y >= Height)
            return;

        DrawAbsolutePixel(x, y, color);
    }
    else if (Rotation == ROTATE_90)
    {
        if (x >= Height || y >= Width)
            return;

        DrawAbsolutePixel(Width - y, x, color);
    }
    else if (Rotation == ROTATE_180)
    {
        if (x >= Width || y >= Height)
            return;

        DrawAbsolutePixel(Width - x, Height - y, color);
    }
    else if (Rotation == ROTATE_270)
    {
        if (x >= Height || y >= Width)
            return;

        DrawAbsolutePixel(y, Height - x, color);
    }
}

// Draws a character
void EPaperDisplay::DrawChar(int x, int y, char asciiChar, sFONT* font, int color)
{
    int i, j;
    unsigned int char_offset = (asciiChar - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &font->table[char_offset];

    for (j = 0; j < font->Height; j++)
    {
        for (i = 0; i < font->Width; i++)
        {
            if (*ptr & (0x80 >> (i % 8)))
                DrawPixel(x + i, y + j, color);

            if (i % 8 == 7)
                ptr++;
        }
        if (font->Width % 8 != 0)
            ptr++;
    }
}

// Draws a string
void EPaperDisplay::DrawString(int x, int y, const char* text, sFONT* font, int color)
{
    const char *textPointer = text;
    unsigned int counter = 0;
    int refcolumn = x;

    /* Send the string character by character on EPD */
    while (*textPointer != 0)
    {
        /* Display one character on EPD */
        DrawChar(refcolumn, y, *textPointer, font, color);
        /* Decrement the column position by 16 */
        refcolumn += font->Width;
        /* Point on the next character */
        textPointer++;
        counter++;
    }

    SetFrameMemoryPartial(GetImage(), x, y, font->Width * counter, font->Height);
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
}

// Draws a horizontal line
void EPaperDisplay::DrawHorizontalLine(int x, int y, int line_width, int color)
{
    for (int i = x; i < x + line_width; i++)
        DrawPixel(i, y, color);
}

// Draws a vertical line
void EPaperDisplay::DrawVerticalLine(int x, int y, int line_height, int color)
{
    for (int i = y; i < y + line_height; i++)
        DrawPixel(x, i, color);
}

// Draws a rectangle
void EPaperDisplay::DrawRectangle(int x0, int y0, int x1, int y1, int color)
{
    int min_x, min_y, max_x, max_y;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;

    DrawHorizontalLine(min_x, min_y, max_x - min_x + 1, color);
    DrawHorizontalLine(min_x, max_y, max_x - min_x + 1, color);
    DrawVerticalLine  (min_x, min_y, max_y - min_y + 1, color);
    DrawVerticalLine  (max_x, min_y, max_y - min_y + 1, color);
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
