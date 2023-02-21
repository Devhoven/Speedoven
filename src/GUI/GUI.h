#pragma once

#include "GUIElements/GUIElement.h"
#include "GUIElements/SpeedTracker.h"
#include "../Display/EpaperDisplay.h"
#include "../Display/Fonts/NotoSans16x24.h"

namespace GUI
{
    EPaperDisplay* Display;

    #define GUI_ELEMENT_COUNT 1
    GUIElement* GUIElements[] = { nullptr };

    void Init(EPaperDisplay* display) 
    { 
        Display = display;
        Display->Init();

        GUIElements[0] = new SpeedTracker(display);
    }

    // This method is run in a task, thus the void* - parameter and while(true) - loop
    void Update(void* parameter)
    {
        while (true)
        {
            for (uint8_t i = 0; i < GUI_ELEMENT_COUNT; i++)
                GUIElements[i]->Update();
        }
    }

    void Draw()
    {
        Display->Clear(WHITE);

        for (uint8_t i = 0; i < GUI_ELEMENT_COUNT; i++)
            GUIElements[i]->Draw();

        Display->Show();
    }
}
