#pragma once

#include "GUIElement.h"

// Processes the input of the bluetooth navigation signals
class NavigationWidget : public GUIElement
{
private: 
    const char* FormatDistance();

public:
    NavigationWidget(EPaperDisplay* display);

    void Update();

    void Draw();
};