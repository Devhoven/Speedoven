#include "SpeedTracker.h"
#include "../../Display/Fonts/NotoSans16x24.h"
#include "../../Display/Fonts/NotoSans24x36.h"
#include "../../Display/Fonts/NotoSans64x96.h"

SpeedTracker::SpeedTracker(EPaperDisplay* display) 
    : GUIElement(display), InputWasHigh(false), CurrentSpeed(0)
{ 
    PreviousTime = micros();
    pinMode(SPEED_INPUT_PIN, INPUT_PULLDOWN);
}

uint8_t SpeedTracker::CalcAvgSpeed()
{
    return (uint8_t)(SpeedSum / SpeedSumCount);
}

void SpeedTracker::Update()
{   
    // The current time is saved for the speed calculations
    uint64_t currentTime = micros();
    
    if (digitalRead(SPEED_INPUT_PIN) == LOW) 
    {
        InputWasHigh = false;
    }
    // If the wheel spun one round this query is true
    else if (!InputWasHigh) 
    {
        // Speed calculation
        // "/ 3.6e+9" is the conversion scalar from microseconds to hours, so km/h is calculated and not km/μs
        CurrentSpeed = (WHEEL_CIRCUMFERENCE_KM / (currentTime - PreviousTime)) * 3.6e+9;

        // For calculating the average speed
        SpeedSum += CurrentSpeed;
        SpeedSumCount++;

        PreviousTime = currentTime;

        // Prevents the microcontroller from detecting the same signal twice
        InputWasHigh = true;
    }

    // If the user isn't riding or slower or equal to 5km/h the current speed is set to zero
    if (currentTime - PreviousTime > ZERO_SPEED_TIMEOUT)
        CurrentSpeed = 0;
}

void SpeedTracker::Draw()
{
    if (CurrentSpeed >= 10)
        Display->DrawString(0,  0, String((uint8_t)CurrentSpeed).c_str(), &Font64);
    else
        Display->DrawString(64, 0, String((uint8_t)CurrentSpeed).c_str(), &Font64);
    
    Display->DrawChar(112, 10, 'k', &Font16);
    Display->DrawChar(112, 34, 'm', &Font16);
    Display->DrawChar(112, 58, 'h', &Font16);

    Display->DrawString(0, 260, String(millis()).c_str(), &Font16);
}