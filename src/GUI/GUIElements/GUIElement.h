#pragma once

#include "../../Display/EPaperDisplay.h"

class GUIElement
{
protected:
    EPaperDisplay* Display;

public: 
    GUIElement(EPaperDisplay* display): Display{display} 
    { }

    virtual void Update();

    virtual void Draw();
};