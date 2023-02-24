#pragma once

#include "GUIElement.h"

// Processes the input of the bluetooth navigation signals
class NavigationWidget : public GUIElement
{
public:
    NavigationWidget(EPaperDisplay* display);

    void Update();

    void Draw();
};