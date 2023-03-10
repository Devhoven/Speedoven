#pragma once

#define BLACK 0
#define WHITE 1

#include <Arduino.h>
#include <stdlib.h>
#include "EPaperDriver.h"
#include "Fonts/Fonts.h"

// Draws on a black and white image
class EPaperDisplay : public EPaperDriver
{
private:
    uint16_t ImgWidth;
    uint16_t ImgHeight;

    void SetSize(uint16_t imgWidth, uint16_t imgHeight);
    

public:
    unsigned char* Image;

    EPaperDisplay(uint16_t width, uint16_t height, EPaperPinConfig pinConfig);

    void Clear(uint8_t color = WHITE);

    void DrawPixel(uint16_t x, uint16_t y, uint8_t color = BLACK);
    void DrawImage(uint16_t x, uint16_t y, const uint8_t* img, uint16_t width, uint16_t height, uint8_t color = BLACK);
    void DrawChar(uint16_t x, uint16_t y, char asciiChar, FONT* font, uint8_t color = BLACK);
    void DrawString(uint16_t x, uint16_t y, const char* text, FONT* font, uint8_t color = BLACK);
    void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color = BLACK);
    void DrawHorizontalLine(uint16_t x, uint16_t y, uint16_t lineWidth, uint8_t color = BLACK);
    void DrawVerticalLine(uint16_t x, uint16_t y, uint16_t lineHeight, uint8_t color = BLACK);
    void DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color = BLACK);
    void DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color = BLACK);
    void DrawCircle(uint16_t x, uint16_t y, uint16_t radius, uint8_t color = BLACK);
    void DrawFilledCircle(uint16_t x, uint16_t y, uint16_t radius, uint8_t color = BLACK);

    void Show();
};