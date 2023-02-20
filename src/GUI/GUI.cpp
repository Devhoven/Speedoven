#include "GUI.h"
#include "../Display/EpaperDisplay.h"
#include "../Display/Fonts/NotoSans16x24.h"
#include "../Display/Fonts/NotoSans24x36.h"
#include "../Display/Fonts/NotoSans64x96.h"

uint16_t YPos = 0;
uint16_t Speed = 25;

GUI::GUI(EPaperDisplay display) : Display{display}
{ 
    Display.Init();
}

void GUI::Update()
{
    YPos = (YPos + 4) % 200;
    Speed += random(-1, 2);
}

void GUI::Draw()
{
    Display.Clear(WHITE);

    Display.DrawString(0,  0, String(Speed).c_str(), &Font64, BLACK);

    Display.DrawString(112, 10, "k", &Font16, BLACK);
    Display.DrawString(112, 34, "m", &Font16, BLACK);
    Display.DrawString(112, 58, "h", &Font16, BLACK);

    Display.DrawString(0,  260, String(millis()).c_str(), &Font16, BLACK);

    Display.Show();
}