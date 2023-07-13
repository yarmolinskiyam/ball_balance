/**
 * @file slider.h
 * @author your name (you@domain.com)
 * @brief Модуль для опроса потенциометров
 * @version 0.1
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <Arduino.h>

class Slider
{
private:
    
    int sliderPin; /*!< Пин, к которому подключен потенциометр */

    long lowLimit; /*!< Минимальное значение */
    long highLimit; /*!< Максимальное значение */

public:
    Slider(int pin, long lowLimit, long highLimit);


    // float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
    // {
    //     return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    // }

    /**
     * @brief Получить значение слайдера
     * @return Значение слайдера в диапазоне [ \p lowLimit, \p highLimit ]
     */
    long getValue ()
    {
        return map (analogRead (sliderPin), 0, 1023, lowLimit, highLimit);
    }
};

Slider::Slider(int pin, long low, long high)
{
    sliderPin = pin;
    lowLimit = low;
    highLimit = high;

    pinMode (sliderPin, INPUT);
}
