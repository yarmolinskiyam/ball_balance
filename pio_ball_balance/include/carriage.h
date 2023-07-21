/**
 * @file carriage.h
 * @brief Модуль определения положения вагона на рельсах
*/
#pragma once

#include <stdint.h>
#include <Ultrasonic.h>
#include "pid.h"
#include "moving_avg.h"

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

    Ultrasonic *sonarL; /*!< Указатель на левый сонар*/
    Ultrasonic *sonarR; /*!< Указатель на правый сонар*/

    static constexpr float Ts = 0.006; /*!< Период, с которым надо вызывать метод getAngleVel */
    static constexpr float Tvel = 0.1; /*!< Постоянная времени фильтра скорости */

    PID *velFilter; /*!< Реальное дифференцирующее звено */
    SimpleMovingAverage<float, 12> posFilter;

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
