// Mukhachev Denis, "4S" team

#include "student.h"

/**
 * @brief Инициализировать систему управления
 * @details Вызывется один раз перед вызовом
 * функции \p controllerTick
 */
void controllerInit(Overlord &over)
{
    over.setSlider(SliderEnum::prog1, 1500, -1500);
    over.setSlider(SliderEnum::prog2, 0, 2 * PI);
}

// return x'0 (car_vel0)
float carPosPreg(float carPosSetPoint, float carPos) // 4
{   
    static const float am = 2;
    static const float Tu = 0.095;

    static const float Tu2 = 2 * Tu;
    static const float Tu3 = 2 * Tu2; // 4 Tu
    // static const float Kp4 = 1 / (3 * am * (Tu3 + Tu)); // 0.8
    static const float Kp4 = 1 / (am * (Tu3 + Tu)); // 0.8

    static float err4 = 0;

    err4 = carPosSetPoint - carPos;

    return err4 * Kp4;
}

// return theta0
float carVelPreg(float carVelSetPoint, float carVel) // 3
{
    static const float am = 2;
    static const float Tu = 0.095;
    static const float i = 9.f / 2;
    static const float g = 9.81;
    static const float Tu2 = 2 * Tu;

    static const float Kp3 = 1 / (am * (Tu2 + Tu) * (g / i)); // 1.5
    static const float Ki3 = 0.006 * 1 / (am * (2 * Tu) * (g / i));
    static float err3 = 0;
    static float u = 0;

    static float I3 = 0;

    err3 = carVelSetPoint - carVel;

    // if(u > - 0.6 && u < 0.6){
    I3 += err3 * Ki3;

    u = err3 * Kp3 + I3;
    u = max(-0.6, min(0.6, u));

    return u;
}
// return w0
float motorPosPregPI(float setPoint, float pos) // 2
{
    static float err = 0;
    static float u = 0;

    static const float K = 1;
    // static const float K = 1;
    static const float am = 3;
    static const float Tu = 0.025; // 0.015
    static const float T = 0.095;
    static const float Tu2 = 2 * Tu;

    static const float Kp = 1 / (am * Tu2 * K);

    err = setPoint - pos;
    u = err * Kp;

    return u;
}

// return  u
float motorVelPIreg(float setPoint, float vel) // 1
{
    static float err;
    static float u;

    static const float Tu = 0.025; // 0.015
    static const float Tp = 0.085;
    static const float am = 1.7;
    static const float T = 0.095;
    static const float K = 8.2 / 12.f;

    static const float Kp = Tp / (am * Tu * K);
    static const float Ki = (Kp / Tp) * 0.006 * 1.5;

    static float I = 0;

    err = setPoint - vel;
    u = Kp * err;
    // if (u < 10 && u > -10)
    if (setPoint <= 10 && setPoint >= -10)
    {
        I += Ki * err;
        // Serial.print("\tmotorVel I: ");
        // Serial.print(I);
    }
    u += I;

    return u;
}

/**
 * @brief Выполнить одну итерацию системы управления
 * @details Вызывается раз в 5мс
 */
void controllerTick(Overlord &over)
{
    static float u, last_u;

    static float x_i, theta, w;
    static float x_raw, x_sum = 0, x_avg = 0, x_avg_last = 0;
    static int xc = 0;

    static const int buff_size = 50;

    static float carPosSetPoint;
    static float carVelSetPoint = 0;
    static float motorPosSetPoint = 0;
    static float motorVelSetPoint = 0;

    static float x_buff[buff_size];

    // carPosSetPoint = over.getSlider(SliderEnum::setPoint);
    //carPosSetPoint = over.getSlider(SliderEnum::prog1);
    //carPosSetPoint *= !digitalRead(13) * 0.0001;

    carPosSetPoint = -over.getSetpoint();

    /* filtering car pos */
    x_raw = -over.getCarX();
    x_buff[xc] = x_raw;
    x_sum = 0;
    for (int i = 0; i < buff_size; i++)
    {
        x_sum += x_buff[i];
    }
    x_avg = x_sum / buff_size;
    xc = (xc + 1) % buff_size;

    // x_avg_last = x_avg;

    x_i = -over.getCarVel();
    theta = over.getMotorTheta();
    w = over.getMotorVel();

    carVelSetPoint = carPosPreg(carPosSetPoint, x_avg);
    motorPosSetPoint = carVelPreg(carVelSetPoint, x_i);
    motorVelSetPoint = motorPosPregPI(motorPosSetPoint, theta);
    u = motorVelPIreg(motorVelSetPoint, w);

    if (abs(last_u - u) > 3)
    {
        if (u > 0)
            u = last_u + 3;
        else
            u = last_u - 3;
    }
    last_u = u;

    /* DEBUG
    Serial.print("\tcarX0: ");
    Serial.print(carPosSetPoint);
    Serial.print("\tcarX: ");
    Serial.print(x_avg);
    Serial.print("\tcarX'0: ");
    Serial.print(carVelSetPoint);
    Serial.print("\ttheta0: ");
    Serial.print(motorPosSetPoint);
    Serial.print("\tw0: ");
    Serial.print(motorVelSetPoint);
    Serial.print("\tu: ");
    Serial.print(u);
    */

    // Serial.print(carPosSetPoint * 100);
    // Serial.print("\t");
    // Serial.print(x_raw * 100);
    // Serial.print("\t");
    // Serial.print(u);


    over.setMotorU(u);

    // Serial.println();
}
