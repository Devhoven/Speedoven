// This project is used to mimic the output of the speedometer receiver 

#define OUTPUT_PIN 8

#define WHEEL_CIRCUMFERENCE_KM (28 * PI / 39370.0)

// The speed it is supposed to mimic in km/h
double Speed = 30;
// Time saved in microseconds for setting the speed signal
unsigned long PreviousTime;
// Time saved in microseconds for changing up the speed
unsigned long PreviousTimeCount;

void setup() 
{
    pinMode(OUTPUT_PIN, OUTPUT);
    PreviousTime = micros();
    PreviousTimeCount = micros();
}

void loop() 
{
    unsigned long currentTime = micros();

    // Sets the output signal at the appropriate time
    if (currentTime - PreviousTime > WHEEL_CIRCUMFERENCE_KM / Speed * 3.6e+9)
    {
        digitalWrite(OUTPUT_PIN, HIGH);
        delay(1);
        digitalWrite(OUTPUT_PIN, LOW);
        PreviousTime = currentTime;
    }
    
    // Changes the speed randomly every second
    if (currentTime - PreviousTimeCount > 1e+6)
    {
        Speed += random(-1, 2);
        PreviousTimeCount = currentTime;
    }
}
