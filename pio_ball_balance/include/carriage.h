/**
 * @file carriage.h
 * @brief Модуль определения положения вагона на рельсах
*/
#pragma once

#include <stdint.h>
#include <Ultrasonic.h>

/**
 * @brief Класс для определения положения вагона на рельсах
*/
class Carriage
{
private:
    int32_t x; /*!< Координата центра вагона в сантиметрах */
    uint32_t halfCarLenght; /*!< Половина длины вагона */
    uint32_t halfTrackLength; /*!< Половина длины пути */

    Ultrasonic *sonarL; /*!< Указатель на сонар*/

public:
    
    /**
     * @brief Конструктор класса Carriage
     * @param [in] trackLength Длина пути в сантиметрах
     * @param [in] carLength Длина вагона в сантиметрах
    */
    Carriage (uint32_t trackLenght, uint32_t carLength);

    /**
     * @brief Получить координату центра вагона
     * @return Координата центра вагона в см.
    */
    int32_t getX ();
};

Carriage::Carriage (uint32_t trackLenght, uint32_t carLength)
{
    static Ultrasonic sonarL_ (7, 6, 1000000/330);
    sonarL = &sonarL_;

    halfTrackLength = trackLenght / 2;
    halfCarLenght = carLength / 2;
}

int32_t Carriage::getX ()
{
    x = sonarL->read () - halfTrackLength - halfCarLenght;
    return x;
}
