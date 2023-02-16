#include <Arduino.h>
#include <SPI.h>
#include "Display/EPaperDriver.h"
#include "Display/EPaperDisplay.h"

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

EPaperDisplay Display(EPD_WIDTH, EPD_HEIGHT,
                      {BUSY_PIN, RST_PIN, DC_PIN, CS_PIN, CLK_PIN, DIN_PIN});

// Used for the timer
unsigned long StartMS;
unsigned long CurrentMS;

void setup()
{
    Serial.begin(115200);
    Display.Init();

    Serial.println("Started");

    StartMS = millis();
}

void loop()
{
    CurrentMS = (millis() - StartMS) / 1000;
    char timeStr[] = {'0', '0', ':', '0', '0', '\0'};
    timeStr[0] = CurrentMS / 60 / 10 + '0';
    timeStr[1] = CurrentMS / 60 % 10 + '0';
    timeStr[3] = CurrentMS % 60 / 10 + '0';
    timeStr[4] = CurrentMS % 60 % 10 + '0';

    Display.Clear(WHITE);
    Display.DrawString(0, 0,  "Fortnite", &Font20, BLACK);
    Display.DrawString(0, 24, "Sussy", &Font24, BLACK);
    Display.DrawString(0, 48, timeStr, &Font24, BLACK);
    Display.DrawString(0, 72, timeStr, &Font24, BLACK);
    Display.DrawString(0, 84, timeStr, &Font24, BLACK);
    
    Display.DrawString(0, 108, timeStr, &Font24, WHITE);
    Display.DrawString(0, 120, timeStr, &Font24, WHITE);

    Display.DisplayFramePartial();
    delay(3000);
}