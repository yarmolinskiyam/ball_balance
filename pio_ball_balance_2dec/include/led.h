#pragma once
#include <FastLED.h>

class LedManager
{
private:

    static const int dataPin = 12;

    static const int numLed = 30;
    static const int offset = numLed/2;
    static float constexpr length = 0.5;

    static float constexpr oneLed = length / numLed;

    static float constexpr brightness = 50;

    CRGB leds[numLed];

public:

    LedManager ();

    void clear ();

    void show ();

    void setCar (float carX, float carVel);

    void setSetpointPos (float setPoint);
};