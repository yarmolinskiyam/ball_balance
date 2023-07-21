#include "student.h"

/**
 * @brief Инициализировать систему управления
 * @details Вызывется один раз перед вызовом
 * функции \p controllerTick
 */
void controllerInit (Overlord &over)
{
    over.setSlider (SliderEnum::prog1, -2000, 2000);
    over.setSlider (SliderEnum::prog2, -10000, 10000);
}

/**
 * @brief Выполнить одну итерацию системы управления
 * @details Вызывается раз в 5мс
 */
void controllerTick (Overlord &over)
{
    /*!v Опрос слайдера чтобы получить инфу из внешнего мира */
    static float setPoint;
    setPoint = -over.getSetpoint();
    // setPoint = over.getSlider (SliderEnum::setPoint);
    // /*!v Опрос кнопки и перевод */
    // setPoint *= !digitalRead (13) * 0.0001;

    /*!v Переменные для хранения информации о мире */
    static float x, x_i, theta, w;
    
    /*!v Опрос реального мира */
    x = -over.getCarX();
    x_i = -over.getCarVel ();
    theta = over.getMotorTheta();
    w = over.getMotorVel ();

    /*!v Регулятор положения вагончика */
    static float err4, x_i0;
    static float constexpr Kp4 = 2.08;
    err4 = setPoint - x;
    x_i0 = err4 * Kp4;


    /*!v Регулятор скорости вагончика */
    static float err3, theta_0;
    static float constexpr Kp3 = 1.9;
    err3 = x_i0 - x_i;
    theta_0 = err3 * Kp3;
    if (theta_0 > 0.5){
    theta_0 = 0.5;
    } else if (theta_0 < -0.5) {
    theta_0 = -0.5;
    }
    /*!v Регулятор положения мотора */
    

    static float err2, w_0;
    static float constexpr Kp2 = 8.33 ;
    err2 = theta_0 - theta;
    w_0 = err2 * Kp2;

    
    

    /*!v Регулятор скорости мотора */
    static float err1, u;
    static float constexpr Kp1 = 2.22;
    static float constexpr Ki1 = 27.7;
    static float constexpr u_min = -9;
    static float constexpr u_max = 9;

    static float integral = 0;
    err1 = w_0 - w;

u = err1 * Kp1 + integral;

if (u > u_max) {
    u = u_max;
} else if (u < u_min) {
    u = u_min;
}
else
{
    integral += err1 * Ki1 * 0.006; 
}

    
    

     

    /*!v Выдача напряжения на мотор */
    over.setMotorU(u);

    /*!v Вывод отладочной информации */
    
    // Serial.print (setPoint  *100);
    // Serial.print('\t');
    // Serial.print(x*100);
    // Serial.println();


    

    
}
