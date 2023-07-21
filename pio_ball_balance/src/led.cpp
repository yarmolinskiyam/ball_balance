#include "led.h"

LedManager::LedManager ()
{
    FastLED.addLeds<WS2812, dataPin, GRB>(leds, numLed);
    // FastLED.show ();
}

void LedManager::clear ()
{
    FastLED.clear ();
}

void LedManager::show ()
{
    FastLED.show ();
}

void LedManager::setCar (float carX, float carVel)
{
    float coor = carX / oneLed;
    float fidx;
    int idx;
    float t = modff (coor + offset - 0.5, &fidx);

    idx = fidx;

    idx = constrain (idx, 0, numLed - 1);

    leds[idx].green = brightness * (1 - t);
    leds[idx + 1].green = brightness * t;

    if (t > 0.5)
        idx++;

    int carVelSign = carVel > 0 ? 1 : -1;

    for (int count = 0; idx >= 0 && idx < numLed && carVel * carVelSign > 0 && count < 5; idx += carVelSign, carVel -= carVelSign * oneLed, count++)
        leds[idx].blue = brightness;
}

void LedManager::setSetpointPos (float setPoint)
{
    float coor = setPoint / oneLed;
    float fidx;
    int idx;
    float t = modff (coor + offset - 0.5, &fidx);

    idx = fidx;

    idx = constrain (idx, 0, numLed - 1);

    leds[idx].red = brightness * (1 - t);
    leds[idx + 1].red = brightness * t;
}
