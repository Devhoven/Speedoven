#pragma once

#include "GUIElement.h"

// The input which is connected to the speedometer input
#define SPEED_INPUT_PIN 20

// Used to calculate the speed in km/h
#define WHEEL_CIRCUMFERENCE_KM (28 * PI / 39370.0)

// ZERO_SPEED_TIMEOUT contains the time between the two signals of the speedometer when the user rides at 5km/h
#define ZERO_SPEED_TIMEOUT (WHEEL_CIRCUMFERENCE_KM / 5) * 3.6e+9

class SpeedTracker : public GUIElement
{
private:
    // Saves if the SPEED_INPUT_PIN was high or not
    bool InputWasHigh;
    // Saves the previous timestamp in microseconds, used for the speed calculation
    uint64_t PreviousTime;
    
    // Saves the current speed in km/h
    float CurrentSpeed;

    // SpeedSum and SpeedSumCount are used to calculate the average speed
    // Sum of all measures speeds
    float SpeedSum;
    // Count of how many measured speeds were summed up
    uint64_t SpeedSumCount;

    // Calculates the average speed
    uint8_t CalcAvgSpeed();

public: 
    SpeedTracker(EPaperDisplay* display);

    void Update();

    void Draw();
};