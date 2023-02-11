#include "ImgPaint.h"
#include <Arduino.h>

ImgPaint::ImgPaint(int width, int height)
{
    SetWidth(width);
    SetHeight(height);
    SetRotate(ROTATE_0);

    this->Image = new unsigned char[Width * Height / 8];
}

/**
 *  @brief: clear the image
 */
void ImgPaint::Clear(int colored)
{
    memset(Image, colored ? 0xFF : 0, Width * Height / 8);
}

/**
 *  @brief: this draws a pixel by absolute coordinates.
 *          this function won't be affected by the rotate parameter.
 */
void ImgPaint::DrawAbsolutePixel(int x, int y, int colored)
{
    if (x < 0 || x >= Width || y < 0 || y >= Height)
        return;

    if (IF_INVERT_COLOR)
        colored = colored ? 1 : 0;

    if (colored)
        Image[(x + y * Width) / 8] |= 0x80 >> (x % 8);
    else
        Image[(x + y * Width) / 8] &= ~(0x80 >> (x % 8));
}

/**
 *  @brief: Getters and Setters
 */
unsigned char *ImgPaint::GetImage()
{
    return Image;
}

int ImgPaint::GetWidth()
{
    return Width;
}

void ImgPaint::SetWidth(int width)
{
    /* 1 byte = 8 pixels, so the width should be the multiple of 8 */
    Width = width % 8 ? width + 8 - (width % 8) : width;
}

int ImgPaint::GetHeight()
{
    return Height;
}

void ImgPaint::SetHeight(int height)
{
    Height = height;
}

int ImgPaint::GetRotate()
{
    return Rotate;
}

void ImgPaint::SetRotate(int rotate)
{
    Rotate = rotate;
}

/**
 *  @brief: this draws a pixel by the coordinates
 */
void ImgPaint::DrawPixel(int x, int y, int colored)
{
    if (x < 0 || y < 0)
        return;

    if (Rotate == ROTATE_0)
    {
        if (x >= Width || y >= Height)
            return;

        DrawAbsolutePixel(x, y, colored);
    }
    else if (Rotate == ROTATE_90)
    {
        if (x >= Height || y >= Width)
            return;

        DrawAbsolutePixel(Width - y, x, colored);
    }
    else if (Rotate == ROTATE_180)
    {
        if (x >= Width || y >= Height)
            return;

        DrawAbsolutePixel(Width - x, Height - y, colored);
    }
    else if (Rotate == ROTATE_270)
    {
        if (x >= Height || y >= Width)
            return;

        DrawAbsolutePixel(y, Height - x, colored);
    }
}

/**
 *  @brief: this draws a charactor on the frame buffer but not refresh
 */
void ImgPaint::DrawCharAt(int x, int y, char asciiChar, sFONT *font, int colored)
{
    int i, j;
    unsigned int char_offset = (asciiChar - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &font->table[char_offset];

    for (j = 0; j < font->Height; j++)
    {
        for (i = 0; i < font->Width; i++)
        {
            if (*ptr & (0x80 >> (i % 8)))
                DrawPixel(x + i, y + j, colored);

            if (i % 8 == 7)
                ptr++;
        }
        if (font->Width % 8 != 0)
            ptr++;
    }
}

/**
 *  @brief: this displays a string on the frame buffer but not refresh
 */
void ImgPaint::DrawStringAt(int x, int y, const char *text, sFONT *font, int colored)
{
    const char *textPointer = text;
    unsigned int counter = 0;
    int refcolumn = x;

    /* Send the string character by character on EPD */
    while (*textPointer != 0)
    {
        /* Display one character on EPD */
        DrawCharAt(refcolumn, y, *textPointer, font, colored);
        /* Decrement the column position by 16 */
        refcolumn += font->Width;
        /* Point on the next character */
        textPointer++;
        counter++;
    }
}

/**
 *  @brief: this draws a line on the frame buffer
 */
void ImgPaint::DrawLine(int x0, int y0, int x1, int y1, int colored)
{
    /* Bresenham algorithm */
    int dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int sx = x0 < x1 ? 1 : -1;
    int dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while ((x0 != x1) && (y0 != y1))
    {
        DrawPixel(x0, y0, colored);
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

/**
 *  @brief: this draws a horizontal line on the frame buffer
 */
void ImgPaint::DrawHorizontalLine(int x, int y, int line_width, int colored)
{
    for (int i = x; i < x + line_width; i++)
        DrawPixel(i, y, colored);
}

/**
 *  @brief: this draws a vertical line on the frame buffer
 */
void ImgPaint::DrawVerticalLine(int x, int y, int line_height, int colored)
{
    for (int i = y; i < y + line_height; i++)
        DrawPixel(x, i, colored);
}

/**
 *  @brief: this draws a rectangle
 */
void ImgPaint::DrawRectangle(int x0, int y0, int x1, int y1, int colored)
{
    int min_x, min_y, max_x, max_y;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;

    DrawHorizontalLine(min_x, min_y, max_x - min_x + 1, colored);
    DrawHorizontalLine(min_x, max_y, max_x - min_x + 1, colored);
    DrawVerticalLine  (min_x, min_y, max_y - min_y + 1, colored);
    DrawVerticalLine  (max_x, min_y, max_y - min_y + 1, colored);
}

/**
 *  @brief: this draws a filled rectangle
 */
void ImgPaint::DrawFilledRectangle(int x0, int y0, int x1, int y1, int colored)
{
    int min_x, min_y, max_x, max_y;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;

    for (int i = min_x; i <= max_x; i++)
        DrawVerticalLine(i, min_y, max_y - min_y + 1, colored);
}

/**
 *  @brief: this draws a circle
 */
void ImgPaint::DrawCircle(int x, int y, int radius, int colored)
{
    /* Bresenham algorithm */
    int x_pos = -radius;
    int y_pos = 0;
    int err = 2 - 2 * radius;
    int e2;

    do
    {
        DrawPixel(x - x_pos, y + y_pos, colored);
        DrawPixel(x + x_pos, y + y_pos, colored);
        DrawPixel(x + x_pos, y - y_pos, colored);
        DrawPixel(x - x_pos, y - y_pos, colored);
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

/**
 *  @brief: this draws a filled circle
 */
void ImgPaint::DrawFilledCircle(int x, int y, int radius, int colored)
{
    /* Bresenham algorithm */
    int x_pos = -radius;
    int y_pos = 0;
    int err = 2 - 2 * radius;
    int e2;

    do
    {
        DrawPixel(x - x_pos, y + y_pos, colored);
        DrawPixel(x + x_pos, y + y_pos, colored);
        DrawPixel(x + x_pos, y - y_pos, colored);
        DrawPixel(x - x_pos, y - y_pos, colored);
        DrawHorizontalLine(x + x_pos, y + y_pos, 2 * (-x_pos) + 1, colored);
        DrawHorizontalLine(x + x_pos, y - y_pos, 2 * (-x_pos) + 1, colored);
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
