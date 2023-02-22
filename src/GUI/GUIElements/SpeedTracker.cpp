#include "SpeedTracker.h"
#include "../../Display/Fonts/NotoSans16x24.h"
#include "../../Display/Fonts/NotoSans24x36.h"
#include "../../Display/Fonts/NotoSans64x96.h"

SpeedTracker::SpeedTracker(EPaperDisplay* display) 
    : GUIElement(display), InputWasHigh(false), CurrentSpeed(0), SpeedSum(0), SpeedSumCount(0)
{ 
    pinMode(SPEED_INPUT_PIN, INPUT_PULLDOWN);
    PreviousTime = micros();

    CurrentSpeedGraph = new Graph(Display, 0, 130, 120, 40);

    PreferencesUpdateTime = micros();
    SpeedPreferences.begin("SpeedData", false);
    LoadPreferences();
}

void SpeedTracker::LoadPreferences()
{
    DistanceDriven = SpeedPreferences.getFloat("Distance", 0);
}

void SpeedTracker::StorePreferences()
{
    SpeedPreferences.putFloat("Distance", DistanceDriven);
}

// Calculates the average speed
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

        DistanceDriven += WHEEL_CIRCUMFERENCE_KM;

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

    // After SPEED_PREFERENCES_UPDATE_SPEED_MICS - μs have passed, the permanent data store is going to be updated
    if (currentTime - PreferencesUpdateTime > SPEED_PREFERENCES_UPDATE_SPEED_MICS)
    {
        PreferencesUpdateTime = currentTime;
        StorePreferences();
    }
}

void SpeedTracker::Draw()
{
    // Drawing the big km/h numbers
    Display->DrawString(CurrentSpeed >= 10 ? 0 : 64, 0, String((uint8_t)CurrentSpeed).c_str(), &Font64);

    // Drawing k m h under each other
    Display->DrawChar(112, 10, 'k', &Font16);
    Display->DrawChar(112, 34, 'm', &Font16);
    Display->DrawChar(112, 58, 'h', &Font16);

    // Drawing the average speed
    Display->DrawString(0, 80, "Avg:", &Font16);
    Display->DrawString(64, 80, String(CalcAvgSpeed()).c_str(), &Font16);

    // Drawing the driven distance
    Display->DrawString(0, 100, "Dst:", &Font16); 
    Display->DrawString(64, 100, String(DistanceDriven, DistanceDriven >= 1 ? 0 : 1).c_str(), &Font16);

    // Drawing a graph with the time on the x-axis and the speed on the y-axis
    CurrentSpeedGraph->AddNextValue((uint8_t)CurrentSpeed);
    CurrentSpeedGraph->Draw();

    // Debug draw
    Display->DrawString(0, 260, String(millis()).c_str(), &Font16);
}