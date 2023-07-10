/**
 * @file motor.h
 * @brief Модуль взаимодействия с мотором
 * @details
 * 1. Определяет угол поворота вала с помощью энкодера
 * 2. Позволяет управлять напряжением на моторе
*/
#pragma once

#include <stdint.h>
#include <Arduino.h>

/**
 * @brief Обработчик прерывания энкодера по первому пину
*/
void enc1Callback ();
/**
 * @brief Обработчик прерывания энкодера по второму пину
*/
void enc2Callback ();

class Motor
{
private:
    
    volatile int32_t encPos; /*!< Угол поворота мотора в тиках энкодера. Обновляется в прерывании */

    static const uint8_t in3 = 10; /*!< Пин IN3 на драйвере L298N */
    static const uint8_t in4 = 11; /*!< Пин IN4 на драйвере L298N */

    static const uint8_t enc1 = 2; /*!< Пин вывода 1 энкодера */
    static const uint8_t enc2 = 3; /*!< Пин вывода 2 энкодера */

    static constexpr float supplyU = 12; /*!< Напряжение питания драйвера мотора */

    uint8_t ticksPerRev; /*!< Количество тиков энкодера на оборот */
    uint8_t gearboxRatio; /*!< Передаточное отношение редуктора */

public:
    
    /**
     * @brief Конструктор объекта класса Motor
     * @param [in] ticksPerRev_ Количество тиков энкодера на оборот
     * @param [in] gearboxRatio_ Передаточное отношение редуктора
    */
    Motor (uint8_t ticksPerRev_, uint8_t gearboxRatio_);

    /**
     * @brief Обработчик прерывания энкодера
     * @param [in] isEnc1 Переменная для определения требуемого знака инкремента
    */
    void tickEncoder (bool isEnc1);

    /**
     * @brief Получить значение энкодера
    */
    int32_t getEncPos () { return encPos; }

    /**
     * @brief Задать напряжение на двигателе
     * @param [in] U Требуемое напряжение на двигателе в вольтах
    */
    void setU (float U);
};

Motor::Motor (uint8_t ticksPerRev_, uint8_t gearboxRatio_)
{
    pinMode (in3, OUTPUT);
    pinMode (in4, OUTPUT);
    pinMode (enc1, INPUT);
    pinMode (enc2, INPUT);

    ticksPerRev = ticksPerRev_;
    gearboxRatio = gearboxRatio_;

    attachInterrupt (digitalPinToInterrupt (enc1), enc1Callback, CHANGE);
    attachInterrupt (digitalPinToInterrupt (enc2), enc2Callback, CHANGE);
}

void Motor::tickEncoder (bool isEnc1)
{
    encPos += ((digitalRead (enc1) == isEnc1) ^ (digitalRead (enc2) == 0)) ? 1 : -1;
}

void Motor::setU (float U)
{
    U = constrain (U, -supplyU, supplyU);

    int8_t pwm = UINT8_MAX * (fabs (U) / supplyU);
    bool dir = U > 0;

    if (dir)
    {
        analogWrite (in3, 255 - pwm);
        digitalWrite (in4, HIGH);
    }
    else
    {
        analogWrite (in3, pwm);
        digitalWrite (in4, LOW);
    }
}

extern Motor motor;

void enc1Callback ()
{
    motor.tickEncoder (true);
}

void enc2Callback ()
{
    motor.tickEncoder (false);
}
