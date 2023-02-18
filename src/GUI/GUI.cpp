#include "GUI.h"
#include "../Display/EpaperDisplay.h"

uint16_t YPos = 0;

GUI::GUI(EPaperDisplay display) : Display{display}
{ 
    Display.Init();
}

void GUI::Update()
{
    YPos = (YPos + 4) % 200;
}

void GUI::Draw()
{
    Display.Clear(WHITE);

    Display.DrawString(0, YPos, "Fortnite", &Font20, BLACK);
    Display.DrawString(0, 72, String(millis()).c_str(), &Font24, BLACK);
    Display.DrawVerticalLine(60, 0, 200, BLACK);

    Display.Show();
}