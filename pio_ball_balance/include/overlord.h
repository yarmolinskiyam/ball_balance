/**
 * @file overlord.h
 * @author 
 * @brief Модуль-надзиратель для взаимодействия с железом и с функционалом оценки попытки
 * @version 0.1
 * @date 2023-07-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "motor.h"
#include "carriage.h"
#include "slider.h"
#include "led.h"

enum class SliderEnum
{
    setPoint = 0,
    prog1 = 1,
    prog2 = 2
};

extern Motor *motor;
    
class Overlord
{
private:

    /*!v Эти значения можно изменять */
    static float constexpr trackLength = 43; /*!< Длина пути в сантиметрах */
    static float constexpr carLength = 9; /*!< Длина вагона в сантиметрах */

    static const uint8_t ticksPerRev = 48; /*!< Количество тиков энкодера на оборот */
    static const uint8_t gearboxRatio = 47; /*!< Передаточное отношение редуктора */
    /*!^ */

    /*!v Это не трогать! */
    /**
     * @brief Приватный конструктор, т.к. singleton
     */
    Overlord ();

    /**
     * @brief Конструкторы перемещения и копирования удалены, т.к. singleton
     */
    Overlord (Overlord&) = delete;
    Overlord (Overlord&&) = delete;

    LedManager *ledManager;

    Carriage *car;

    static const size_t sliderCount = 3;
    Slider **sliders;

    static float constexpr Ts = 0.006; /*!< Интервал квантования регулятора */
    static const unsigned long Tsmicros = 6000; /*!< Интервал квантования в мкс */

    void (*controller) (Overlord &);

    /**
     * @brief Интегральная квадратичная ошибка
     */
    float integralError = 0;

    unsigned long timer;

    size_t timeTick = 0;

public:

    /**
     * @brief Получить ссылку на единственный экземпляр класса
     * @return Ссылка на объект Overlord
     */
    static Overlord& getInstance ();

    /**
     * @brief Инициализировать наздиратель
     * @param [in] controller_ указатель на реализацию регулятора
     */
    void init (void (*controller_) (Overlord &));

    /**
     * @brief Получить интервал квантования программы
     * @return Интервал квантования в секундах
     */
    float getTs () const { return Ts; }

    /**
     * @brief Получить значение уставки
     * @return Требуемая координата вагона в метрах
     */
    float getSetpoint ();

    /**
     * @brief Задать пределы программируемого слайдера
     * @param [in] slid Требуемый слайдер
     * @return Значение слайдера
     */
    void setSlider (SliderEnum slid, long low, long high)
    {
        sliders[static_cast<int> (slid)]->setSlider (low, high);
    }

    /**
     * @brief Получить значение требуемого слайдера
     * @param [in] slid Требуемый слайдер
     * @return Значение слайдера
     */
    long getSlider (SliderEnum slid) { return sliders[static_cast<int> (slid)]->getValue (); }

    /**
     * @brief Получить координату центра вагона
     * @return Координата центра вагона в метрах
     */
    float getCarX () { return car->getX (); }

    /**
     * @brief Получить скорость вагона
     * @return Скорость вагона в метрах в секунду
     */
    float getCarVel () { return car->getVel (); }

    /**
     * @brief Получить угол поворота мотора
     * @return Угол поворота мотора в радианах
     */
    float getMotorTheta () { return motor->getAngleRad (); }

    /**
     * @brief Получить угловую скорость мотора
     * @return Угловая скорость мотора в радианах в секунду
     */
    float getMotorVel () { return motor->getAngleVel (); }

    /**
     * @brief Выдать напряжение на двигатель
     * @param [in] u Требуемое напряжение на двигателе в вольтах
     */
    void setMotorU (float u) { motor->setU (u); }

    /**
     * @brief Сделать один шаг работы надзирателя
     * @note Вызывать раз в 5мс!
     * @details В режиме попытки вызывает функцию реализации регулятора и выдает
     * управляющее воздействие на мотор
     */
    void tick ();
};
