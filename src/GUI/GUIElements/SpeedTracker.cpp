#include "SpeedTracker.h"
#include "../../Display/Fonts/NotoSans16x24.h"
#include "../../Display/Fonts/NotoSans64x96.h"

SpeedTracker::SpeedTracker(EPaperDisplay* display) : GUIElement(display), Speed(30)
{ }

void SpeedTracker::Update()
{
    Speed += random(-1, 2);
    Speed = Speed > 99 ? 99 : Speed;
}

void SpeedTracker::Draw()
{
    Display->DrawString(0, 0, String(Speed).c_str(), &Font64, BLACK);
    
    Display->DrawString(112, 10, "k", &Font16, BLACK);
    Display->DrawString(112, 34, "m", &Font16, BLACK);
    Display->DrawString(112, 58, "h", &Font16, BLACK);
}