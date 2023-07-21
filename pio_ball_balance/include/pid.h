/**
 * @file pid.h
 * @author your name (you@domain.com)
 * @brief Модуль реализации дискретного ПИД регулятора
 * @version 0.1
 * @date 2023-07-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <Arduino.h>

class PID
{
private:
    float u; /*!< Последнее рассчитанное значение выхода */

    float Kp; /*!< Коэффициент П */
    float Ki; /*!< Коэффициент И */
    float Kd; /*!< Коэффициент Д */
    float T; /*!< Постоянная времени фильтра Д звена */
    float Ts; /*!< Период квантования регулятора */

    float Ii = 0; /*!< Интеграл интегральной части */
    float Id = 0; /*!< Интеграл дифференциальной части */

    bool isSat = false; /*!< Использовать защиту от насыщения? */
    float satLo; /*!< Нижний предел насыщения */
    float satHi; /*!< Верхний предел насыщения */

public:

    /**
     * @brief Конструктор 
     * @param [in] Kp_  Коэффициент П
     * @param [in] Ki_  Коэффициент И
     * @param [in] Kd_  Коэффициент Д
     * @param [in] T_  Постоянная времени фильтра Д звена
     * @param [in] Ts_  Период квантования регулятора
     */
    PID(float Kp_, float Ki_, float Kd_, float T_, float Ts_, float satLo_ = -INFINITY, float satHi_ = INFINITY);

    /**
     * @brief Получить выход звена регулятора
     * @param [in] err Текущее значение ошибки
     * @return Выход звена регулятора (управляющее воздействие)
     * @note Вызывать один раз в период квантования
     */
    float tick (float err);

    /**
     * @brief Получить последнее значение выхода звена
     * @return u
     */
    float getLast () { return u; }

    bool isSatEnabled () { return isSat; }
};
