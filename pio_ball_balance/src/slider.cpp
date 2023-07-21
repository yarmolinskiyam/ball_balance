#include "slider.h"

Slider::Slider(int pin, long low, long high)
{
    sliderPin = pin;
    lowLimit = low;
    highLimit = high;

    pinMode (sliderPin, INPUT);
}

void Slider::setSlider (long low, long high)
{
    lowLimit = low;
    highLimit = high;
}
