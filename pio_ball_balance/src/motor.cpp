#include "motor.h"

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

    static PID d (0, 0, 1, T, Ts);
    velFilter = &d;
}

void Motor::tickEncoder (bool isEnc1)
{
    encPos += ((digitalRead (enc1) == isEnc1) ^ (digitalRead (enc2) == 0)) ? 1 : -1;
}

void Motor::setU (float U_)
{
    U = constrain (U_, -supplyU, supplyU);

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

void Motor::update ()
{
    theta = encPos*6.2832f / (ticksPerRev * gearboxRatio);
    theta_i = velFilter->tick (theta);
}

extern Motor *motor;

void enc1Callback ()
{
    motor->tickEncoder (true);
}

void enc2Callback ()
{
    motor->tickEncoder (false);
}