#include "SpeedTracker.h"
#include "../../Display/Fonts/NotoSans16x24.h"
#include "../../Display/Fonts/NotoSans64x96.h"

SpeedTracker::SpeedTracker(EPaperDisplay* display) : GUIElement(display), Speed(30)
{ 
    PreviousTime = micros();
    pinMode(SPEED_PIN_1, INPUT);
    pinMode(SPEED_PIN_2, INPUT);
}

void SpeedTracker::Update()
{   
    // If the wheel spun one round this query should be set to true
    if (digitalRead(SPEED_PIN_1) == HIGH || digitalRead(SPEED_PIN_2) == HIGH)
    {
        // Speed is calculated and the time is saved
        unsigned long currentTime = micros();
        // "/ 3.6e+9" is the conversion from microseconds to hours, so we get km/h"
        Speed = (uint8_t)(WHEEL_CIRCUMFERENCE_IN_KM / ((currentTime - PreviousTime) / 3.6e+9));
        PreviousTime = currentTime;
        Serial.println("This is so sad :(");
    }
}

void SpeedTracker::Draw()
{
    if (Speed >= 10)
        Display->DrawString(0, 0, String(Speed).c_str(), &Font64);
    else
        Display->DrawString(64, 0, String(Speed).c_str(), &Font64);
    
    Display->DrawString(112, 10, "k", &Font16);
    Display->DrawString(112, 34, "m", &Font16);
    Display->DrawString(112, 58, "h", &Font16);
}