// //Яковлев Лев Борисович
// //Команда - МБОУ СОШ N237
// #include "student.h"


// /*
//  * @brief Инициализировать систему управления
//  * @details Вызывется один раз перед вызовом
//  * функции \p controllerTick
//  */
// void controllerInit (Overlord &over)
// {
//     over.setSlider (SliderEnum::prog1, -2000, 2000);
//     over.setSlider (SliderEnum::prog2, -2000, 2000);
// }

// /*
//  * @brief Выполнить одну итерацию системы управления
//  * @details Вызывается раз в 5мс
//  */
// void controllerTick (Overlord &over)
// {
//     /*!v Переменные для коэффициентов*/
//     static float constexpr T = 0.06;
//     static float constexpr Tu = 0.015;
//     static float constexpr K = 0.61;
//     static float constexpr Au = 4;
//     static float constexpr g = 9.8;
//     static float constexpr i = 9 / 2;
//     static float constexpr Kp = T / (Tu * 3.75 * K);
//     static float constexpr Ki = Kp / T;
//     static float constexpr T_sum = 2 * Tu;
//     static float constexpr K_sum = 1;
//     static float constexpr Kp1 = 1 / (3.85 * T_sum * K_sum);
//     static float constexpr T_sum1 = 2 * T_sum;
//     static float constexpr K_sum1 = 1;
//     static float constexpr Kp2 = 1 / (3.95 * T_sum1 * K_sum1 / i * g);
//     static float constexpr T_sum2 = 2 * T_sum1;
//     static float constexpr K_sum2 = 1;
//     static float constexpr Kp3 = 1 / (4.5 * T_sum2 * K_sum2);
//     static float constexpr Ki1 = 10;
//     /*!v Опрос слайдера чтобы получить инфу из внешнего мира */
//     static float setPoint;
//     setPoint = -over.getSetpoint ();


//     /*!v Переменные для хранения информации о мире */
//     static float x, x_i, theta, w;
    
//     /*!v Опрос реального мира */
//     x = -over.getCarX ();
//     x_i = -over.getCarVel ();
//     theta = over.getMotorTheta ();
//     w = over.getMotorVel ();

//     /*!v Регулятор положения вагончика */
//     static float err4, x_i0;
//     err4 = setPoint - x;
//     x_i0 = err4 * Kp3;

//     /*!v Регулятор скорости вагончика */
//     static float err3, theta_0, I1 = 0;
//     err3 = x_i0 - x_i;
//     I1 += err3 * Ki1 * 0.006;
//     theta_0 = err3 * Kp2 + I1;

//     /*!v Регулятор положения мотора */
//     static float err2, w_0;
//     err2 = theta_0 - theta;
//     w_0 = err2 * Kp1;

//     /*!v Регулятор скорости мотора */
//     static float err1, u, I = 0;
//     err1 = w_0 - w;
//     u = err1 * Kp + I;
//     if(u < 12 && u > -12) I += err1 * Ki * 0.006;

//     /*!v Защита от перенасыщения */
//     if(abs(theta) > 0.5 && u * theta > 0) u = 0;
//     /*
//     static float error;
//     error = setPoint * 100 - w;
//     u = error * Kp;
//     */
    

//     /*!v Выдача напряжения на мотор */
//     over.setMotorU (u);

//     // /*!v Вывод отладочной информации */
//     // Serial.print (setPoint);
//     // Serial.print ('\t');
//     // Serial.print (x);
//     // Serial.println ();
// }
// /*

// pio run --target upload; pio device monitor -b 115200

// */