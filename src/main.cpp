#include <Arduino.h>
#include "Display/EPaperDisplay.h"
#include "GUI/GUI.h"

// Display resolution
#define EPD_WIDTH  128
#define EPD_HEIGHT 296

// Pin definition
#define BUSY_PIN 7
#define RST_PIN  6
#define DC_PIN   5
#define CS_PIN   4
#define CLK_PIN  3
#define DIN_PIN  2

EPaperDisplay Display(EPD_WIDTH, EPD_HEIGHT,
                      {BUSY_PIN, RST_PIN, DC_PIN, CS_PIN, CLK_PIN, DIN_PIN});

void setup()
{
    Serial.begin(115200);
    Serial.println("Started");

    GUI::Init(&Display);
}

void loop()
{
    GUI::Update();
    GUI::Draw();
}