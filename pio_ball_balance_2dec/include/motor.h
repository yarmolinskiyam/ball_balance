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
#include <pid.h>

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
    static constexpr float Ts = 0.006; /*!< Период, с которым надо вызывать метод getAngleVel */
    static constexpr float T = 0.03; /*!< Постоянная времени фильтра скорости */

    float U; /*!< Текущее напряжение на моторе */

    uint8_t ticksPerRev; /*!< Количество тиков энкодера на оборот */
    uint8_t gearboxRatio; /*!< Передаточное отношение редуктора */

    PID *velFilter; /*!< Реальное дифференцирующее звено */

    float theta;
    float theta_i;

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
     * @brief Обновить значения положения и скорости двигателя
     * @note Вызывать один раз каждые 5 мс
     */
    void update ();
    
    /**
     * @brief Получить угол вала двигателя в радианах
    */
    float getAngleRad () { return theta; }

    /**
     * @brief Получить скорость двигателя в рад/с
     * @return Скорость вала двигателя в рад/с
     */
    float getAngleVel () { return theta_i; }

    /**
     * @brief Задать напряжение на двигателе
     * @param [in] U_ Требуемое напряжение на двигателе в вольтах
    */
    void setU (float U_);

    /**
     * @brief Получить текущее напряжение на моторе
     * @return Текущее напряжение на моторе в вольтах
     */
    float getU () { return U; }
};
