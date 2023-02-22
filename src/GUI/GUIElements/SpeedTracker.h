#pragma once

#include "GUIElement.h"
#include "Graph.h"
#include <Preferences.h>

// The input which is connected to the speedometer input
#define SPEED_INPUT_PIN 20

// Used to calculate the speed in km/h
#define WHEEL_CIRCUMFERENCE_KM (28 * PI / 39370.0)

// ZERO_SPEED_TIMEOUT contains the time between the two signals of the speedometer when the user rides at 5km/h
#define ZERO_SPEED_TIMEOUT (WHEEL_CIRCUMFERENCE_KM / 5) * 3.6e+9

// After how many μs the preferences are updated; 900000000μs are 15min
// This is so limited, since you can't write on the EEPROM infinitly often
#define SPEED_PREFERENCES_UPDATE_SPEED_MICS 900000000


class SpeedTracker : public GUIElement
{
private:
    // These preferences are used to permanently store the driven distance
    Preferences SpeedPreferences;
    // The last time when the preferences were updated
    uint64_t PreferencesUpdateTime;

    // Saves if the SPEED_INPUT_PIN was high or not
    bool InputWasHigh;
    // Saves the previous timestamp in microseconds, used for the speed calculation
    uint64_t PreviousTime;
    
    // Saves the current speed in km/h
    float CurrentSpeed;

    // A graph which shows the current speed
    Graph* CurrentSpeedGraph;

    // The entire distance in km the driver has driven with this speedometer enabled
    float DistanceDriven;

    // SpeedSum and SpeedSumCount are used to calculate the average speed
    // Sum of all measures speeds of the current ride
    float SpeedSum;
    // Count of how many measured speeds were summed up
    uint64_t SpeedSumCount;

    // Calculates the average speed
    uint8_t CalcAvgSpeed();

    // Load and store the prefernces respectively
    void LoadPreferences();
    void StorePreferences();

public: 
    SpeedTracker(EPaperDisplay* display);

    void Update();

    void Draw();
};