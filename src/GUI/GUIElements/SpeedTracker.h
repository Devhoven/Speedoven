#pragma once

#include "GUIElement.h"

class SpeedTracker : public GUIElement
{
private:
    uint8_t Speed;

public: 
    SpeedTracker(EPaperDisplay* display);

    void Update();

    void Draw();
};