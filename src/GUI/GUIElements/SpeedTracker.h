#pragma once

#include "GUIElement.h"

#define SPEED_PIN_1 21
#define SPEED_PIN_2 20

#define WHEEL_CIRCUMFERENCE_IN_KM (28 * PI / 39370.0)

class SpeedTracker : public GUIElement
{
private:
    uint8_t Speed;
    unsigned long PreviousTime;

public: 
    SpeedTracker(EPaperDisplay* display);

    void Update();

    void Draw();
};