#include "pid.h"

PID::PID(float Kp_, float Ki_, float Kd_, float T_, float Ts_,
        float satLo_, float satHi_)
{
    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;
    T = T_;
    Ts = Ts_;

    if (satLo_ > -INFINITY && satHi_ < INFINITY && satLo_ < satHi_)
    {
        isSat = true;
        satLo = satLo_;
        satHi = satHi_;
    }
}

float PID::tick (float err)
{
    float d = (err - Id) / T;
    u = Kp * err + Ii + d;

    if (isSat && u == constrain (u, satLo, satHi))
    {
        Ii += err * Ki * Ts;
        u = constrain (u, satLo, satHi);
    }
    Id += d * Ts;

    return u;
}
