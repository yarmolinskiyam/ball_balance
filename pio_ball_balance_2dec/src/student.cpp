#include "student.h"

/**
 * @brief Инициализировать систему управления
 * @details Вызывется один раз перед вызовом
 * функции \p controllerTick
 */
void controllerInit (Overlord &over)
{
    over.setSlider (SliderEnum::prog1, -12000, 12000);
    over.setSlider (SliderEnum::prog2, -10000, 10000);
}

/**
 * @brief Выполнить одну итерацию системы управления
 * @details Вызывается раз в 5мс
 */
void controllerTick (Overlord &over)
{
    static float setPoint, vel0, motorVel;
    
    setPoint = over.getSetpoint ();

    bool button = !digitalRead (13);
    vel0 = button * over.getSlider (SliderEnum::prog1) * 0.001;

    motorVel = over.getMotorVel ();

    /*!v Параметры объекта управления */
    static float constexpr Ko = 0.515; // rad/s/V
    static float constexpr To = 0.04; // s
    static float constexpr Tou = 0.01; // s

    /*!v Коэффициент регулятора для разомкнутого управления */
    static float constexpr Kk = 1/Ko;

    static float constexpr am = 4;
    /*!v Коэффициент П регулятора */
    static float constexpr Kp = 1/Ko * ((To + Tou)*(To + Tou) / (am*To*Tou) - 1);
    static float constexpr Kphio = Kp*Ko / (1 + Kp*Ko);

    /*!v Коэффициенты И регулятора */
    static float constexpr Tosum = To + Tou;
    static float constexpr Ki = 1 / (am*Ko*Tosum);

    /*!v Период квантования */
    static float constexpr Ts = 0.006; // s

    static float u, err, I = 0;

    err = vel0 - motorVel;

    u = I;

    I += err*Ki*Ts;

    over.setMotorU (vel0);

    // Serial.print (vel0);
    // Serial.print ('\t');
    // Serial.print (motorVel);
    // Serial.print ('\t');
    // Serial.println ();
}
