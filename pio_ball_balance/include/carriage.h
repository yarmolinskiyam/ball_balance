/**
 * @file carriage.h
 * @brief Модуль определения положения вагона на рельсах
*/
#pragma once

#include <stdint.h>
#include <Ultrasonic.h>
#include "pid.h"

/**
 * @brief Класс для определения положения вагона на рельсах
*/
class Carriage
{
private:
    float x; /*!< Координата центра вагона в метрах */
    float x_i; /*!< Скорость вагона в м/с */

    int32_t halfCarLenght; /*!< Половина длины вагона */
    int32_t halfTrackLength; /*!< Половина длины пути */

    Ultrasonic *sonarL; /*!< Указатель на сонар*/

    static constexpr float Ts = 0.005; /*!< Период, с которым надо вызывать метод getAngleVel */
    static constexpr float Tvel = 0.01; /*!< Постоянная времени фильтра скорости */
    static constexpr float Tpos = 0.01; /*!< Постоянная времени фильтра положения */

    PID *velFilter; /*!< Реальное дифференцирующее звено */
    PID *posFilter; /*!< Реальное дифференцирующее звено */

public:
    
    /**
     * @brief Конструктор класса Carriage
     * @param [in] trackLength Длина пути в сантиметрах
     * @param [in] carLength Длина вагона в сантиметрах
    */
    Carriage (uint32_t trackLength, uint32_t carLength);

    /**
     * @brief Обновить координату и скорость вагончика
     * @note Вызывать раз в 5 мс
     */
    void update ();

    /**
     * @brief Получить координату центра вагона
     * @return Координата центра вагона в м
    */
    float getX () { return x; }

    /**
     * @brief Получить скорость вагона
     * @return Скорость вагона в м/с 
     */
    float getVel () { return x_i; }
};

Carriage::Carriage (uint32_t trackLength, uint32_t carLength)
{
    static Ultrasonic sonarL_ (7, 6, 1000000/330);
    sonarL = &sonarL_;

    halfTrackLength = trackLength / 2;
    halfCarLenght = carLength / 2;
    
    static PID dv (0, 0, 1, Tvel, Ts);
    velFilter = &dv;
    static PID dp (0, 1/Tpos, 0, 1, Ts);
    posFilter = &dp;
}

void Carriage::update ()
{
    // x = posFilter->tick ((sonarL->readf () - halfTrackLength - halfCarLenght) * 0.01);
    x = (sonarL->read () - halfTrackLength - halfCarLenght) * 0.01;
    // x = posFilter->tick (x - posFilter->getLast ());
    x_i = velFilter->tick (x);
}
