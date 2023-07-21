#include "student.h"
//#include <cmath>

/**
 * @brief Ð˜Ð½Ð¸Ñ†Ð¸Ð°Ð»Ð¸Ð·Ð¸Ñ€Ð¾Ð²Ð°Ñ‚ÑŒ ÑÐ¸ÑÑ‚ÐµÐ¼Ñƒ ÑƒÐ¿Ñ€Ð°Ð²Ð»ÐµÐ½Ð¸Ñ
 * @details Ð’Ñ‹Ð·Ñ‹Ð²ÐµÑ‚ÑÑ Ð¾Ð´Ð¸Ð½ Ñ€Ð°Ð· Ð¿ÐµÑ€ÐµÐ´ Ð²Ñ‹Ð·Ð¾Ð²Ð¾Ð¼
 * Ñ„ÑƒÐ½ÐºÑ†Ð¸Ð¸ \p controllerTick
 */
enum Regulators {
    PI_reg,
    Proportional,
    Integral
};

void controllerInit (Overlord &over)
{
    over.setSlider (SliderEnum::prog1, -150, 150);
    over.setSlider (SliderEnum::prog2, -10000, 10000);
}

float sign(float a) {
    if (a > 0) return 1;
    else if (a < 0) return -1;
    else return 0;
}

float PIR(float setPoint, float vel) {
    static float constexpr K = 8.22 / 12;
    static float constexpr T = 0.095;
    static float constexpr Tu = 0.025;

    static float constexpr Tp = T;
    static float constexpr Kp = Tp / (2 * (Tu) * K);
    static float constexpr Ki = Kp / Tp * 0.006;
    static float I = 0, P = 0;
    float err;
    float u;

    err = setPoint - vel;
    P = err * Kp;
    u = P + I;
    I += err * Ki;

    if (abs(u) >= 12) u = sign(u) * 12;

    return u;
}

float PR(float setPoint, float vel) {
    static float constexpr K = 8.22 / 12;
    static float constexpr T = 0.095;
    static float constexpr Tu = 0.025;

    static float constexpr Tp = T;
    static float constexpr Kp = 1 / (2 * (T + Tu) * K);
    float err;
    float u;

    err = setPoint - vel;
    u = err * Kp;
    if (abs(u) >= 12) u = sign(u) * 12;

    return u;
}

float IR(float setPoint, float vel) {
    static float constexpr K = 8.22 / 12;
    static float constexpr T = 0.095;
    static float constexpr Tu = 0.025;

    static float constexpr Tp = T;
    static float constexpr Kp = Tp / (2 * (Tu) * K);
    static float constexpr Ki = Kp / Tp * 0.006;
    static float I = 0;
    float err;

    err = setPoint - vel;
    I += err * Ki;

    if (abs(I) >= 12) I = sign(I) * 12;

    return I;
}

float theta_p(float setPoint, float theta, float vel, Regulators r) {
    static float constexpr KInterior = 8.22 / 12;
    static float K = 1;
    
    static float constexpr Tu = 0.025;
    static float constexpr T = 0.095;
    static float constexpr velKp = 1 / (2 * (T + Tu) * KInterior);
    if (r == Proportional) K = velKp * KInterior / (1 + velKp * KInterior);
    float Tu2 = 0;

    if (r == Proportional) Tu2 = 2 * T / (1 + K * velKp);
    else if (r == Integral) Tu2 = 2 * (Tu + T);
    else Tu2 = 2 * Tu;
    
    static float Kp = 1 / (2 * (Tu2) * K); //+T

    if (r == Proportional) return PR(Kp * (setPoint - theta), vel);
    else if (r == Integral) return IR(Kp * (setPoint - theta), vel);
    else if (r == PI_reg) return PIR(Kp * (setPoint - theta), vel);
    
    //return PR(Kp * (setPoint - theta), vel);
    return 0;
}

/**
 * @brief Ð’Ñ‹Ð¿Ð¾Ð»Ð½Ð¸Ñ‚ÑŒ Ð¾Ð´Ð½Ñƒ Ð¸Ñ‚ÐµÑ€Ð°Ñ†Ð¸ÑŽ ÑÐ¸ÑÑ‚ÐµÐ¼Ñ‹ ÑƒÐ¿Ñ€Ð°Ð²Ð»ÐµÐ½Ð¸Ñ
 * @details Ð’Ñ‹Ð·Ñ‹Ð²Ð°ÐµÑ‚ÑÑ Ñ€Ð°Ð· Ð² 5Ð¼Ñ
 */

float uOld = 0;
void controllerTick (Overlord &over)
{
    static float constexpr i = 2.f/9, g = 9.81;
    static float constexpr K = 8.22 / 12;
    static float constexpr T = 0.095;
    static float constexpr Tu = 0.025;

    static float Kp1 = T / (2.9 * Tu * K), Ki1 = 1 / (2.9 * Tu * K); //3.1
    static float K1s = 1, T1s = 2 * Tu;

    static float Kp2 = 1 / (2.5 * (T1s) * K1s); //2.65
    static float K2s = 1, T2s = 2 * T1s; //Kp1 * K1s / (1 + Kp1 * K1s)    2 * T1s / (1 + Kp2 * K1s)

    static float Kp3 = 1 / (3.5 * (T2s) * K2s * i * g); //3.4
    static float K3s =  1, T3s = 2 * T2s; //i * g * Kp2 * K2s / (1 + Kp2 * K2s)     2 * T2s / (1 + Kp3 * K2s)

    static float Kp4 = 1 / (1.4 * (T3s) * K3s); //1.6
    static float K4s =  1, T4s = 2 * T3s; //Kp3 * K3s / (1 + Kp3 * K3s)     2 * T3s / (1 + Kp4 * K3s)

    /*!v ÐžÐ¿Ñ€Ð¾Ñ ÑÐ»Ð°Ð¹Ð´ÐµÑ€Ð° Ñ‡Ñ‚Ð¾Ð±Ñ‹ Ð¿Ð¾Ð»ÑƒÑ‡Ð¸Ñ‚ÑŒ Ð¸Ð½Ñ„Ñƒ Ð¸Ð· Ð²Ð½ÐµÑˆÐ½ÐµÐ³Ð¾ Ð¼Ð¸Ñ€Ð° */
    static float setPoint;
    setPoint = -over.getSetpoint ();
    /*static float constexpr K = 8.22 / 12;
    static float constexpr T = 0.09;
    static float constexpr Tu = 0.01;

    static float constexpr Tp = T;
    static float constexpr Kp = Tp / (2 * (T + Tu) * K);
    static float constexpr Ki = Kp / Tp * 0.006;
    static float I = 0, P = 0;*/

    /*!v ÐŸÐµÑ€ÐµÐ¼ÐµÐ½Ð½Ð°Ñ, Ñ…Ñ€Ð°Ð½ÑÑ‰Ð°Ñ ÑÐºÐ¾Ñ€Ð¾ÑÑ‚ÑŒ Ð´Ð²Ð¸Ð³Ð°Ñ‚ÐµÐ»Ñ */
    static float vel, theta;
    static float x, x_i;
    /*static float err;
    static float u;*/

    /*!v ÐžÐ¿Ñ€Ð¾Ñ ÑÐºÐ¾Ñ€Ð¾ÑÑ‚Ð¸ Ð´Ð²Ð¸Ð³Ð°Ñ‚ÐµÐ»Ñ */
    x = -over.getCarX();
    x_i = -over.getCarVel();
    theta = over.getMotorTheta();
    vel = over.getMotorVel();
    /*!v Ð’Ñ‹Ð´Ð°Ñ‡Ð° Ð½Ð°Ð¿Ñ€ÑÐ¶ÐµÐ½Ð¸Ñ Ð½Ð° Ð¼Ð¾Ñ‚Ð¾Ñ€ */
    static float err4, x_i0;
    err4 = setPoint - x;
    x_i0 = err4 * Kp4;

    static float err3, theta_0;
    err3 = x_i0 - x_i;
    theta_0 = err3 * Kp3;
    theta_0 = max(-0.35, min(0.35, theta_0));

    static float err2, w_0;
    err2 = theta_0 - theta;
    w_0 = err2 * Kp2;

    static float err1, u;
    static float I1 = 0, P1;
    //static float constexpr Kp1 = T / (2 * Tu * K), Ki1 = 1 / (2 * Tu * K);
    err1 = w_0 - vel;
    //err1 = setPoint - vel;
    P1 = err1 * Kp1;
    I1 += err1 * Ki1;

    I1 = max(-3, min(3, I1));
    u = P1 + I1;

    /*if (abs(u - uOld) >= 3) {
        float c = u;
        u = uOld;
        uOld = c;
    } else uOld = u;*/
    
    //u = max(-2, min(2, u));
    //if (setPoint == x) u = 0;

    over.setMotorU(u);//theta_p(setPoint, theta, vel, PI_reg));
    //over.setMotorU (setPoint);

    /*!v Ð’Ñ‹Ð²Ð¾Ð´ Ð¾Ñ‚Ð»Ð°Ð´Ð¾Ñ‡Ð½Ð¾Ð¹ Ð¸Ð½Ñ„Ð¾Ñ€Ð¼Ð°Ñ†Ð¸Ð¸ */
    // Serial.print (-setPoint);
    // Serial.print ("\tx: ");
    // Serial.print (-x);
    // //Serial.print ("\tx_i: ");
    // /*Serial.print (x_i);
    // Serial.print ("\tvel: ");
    // Serial.print (vel);
    // Serial.print ("\ttheta: ");
    // Serial.print (theta);*/
    // //Serial.print ("\t \t \t");
    // //Serial.print (x_i0);
    // //Serial.print ('\t');
    // //Serial.print (theta_0);
    // //Serial.print ('\t');
    // //Serial.print (w_0);*/
    // //Serial.print ("\t\tu: ");
    // //Serial.print (u);
    // Serial.println ();
}