#include <Arduino.h>
#include <SPI.h>
#include "Display/EPaperDisplay.h"
#include "Display/ImgPainter.h"

#define COLORED 0
#define UNCOLORED 1

// Display resolution
#define EPD_WIDTH  128
#define EPD_HEIGHT 296

// Pin definition
#define BUSY_PIN 14
#define RST_PIN  27
#define DC_PIN   26
#define CS_PIN   25
#define CLK_PIN  33
#define DIN_PIN  32

ImgPainter paint(EPD_WIDTH, EPD_HEIGHT); 
EPaperDisplay display(EPD_WIDTH, EPD_HEIGHT, 
                      BUSY_PIN, RST_PIN, DC_PIN, CS_PIN, CLK_PIN, DIN_PIN);

// Used for the timer
unsigned long startMS;
unsigned long currentMS;

void setup()
{
    Serial.begin(115200);
    display.Init();

    Serial.println("Started");

    startMS = millis();
}

void loop()
{
    currentMS = (millis() - startMS) / 1000;
    char time_string[] = {'0', '0', ':', '0', '0', '\0'};
    time_string[0] = currentMS / 60 / 10 + '0';
    time_string[1] = currentMS / 60 % 10 + '0';
    time_string[3] = currentMS % 60 / 10 + '0';
    time_string[4] = currentMS % 60 % 10 + '0';

    paint.SetWidth(96);
    paint.SetHeight(32);
    paint.SetRotate(ROTATE_0);

    paint.Clear(UNCOLORED);
    paint.DrawStringAt(0, 0, time_string, &Font24, COLORED);
    display.SetFrameMemoryPartial(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
    display.DisplayFramePartial();
}