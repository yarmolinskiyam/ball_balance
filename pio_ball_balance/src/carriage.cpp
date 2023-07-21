#include "carriage.h"

Carriage::Carriage (uint32_t trackLength, uint32_t carLength)
{
    static Ultrasonic sonarL_ (5, 4, 1000000/330);
    static Ultrasonic sonarR_ (7, 6, 1000000/330);
    sonarL = &sonarL_;
    sonarR = &sonarR_;

    halfTrackLength = (trackLength - carLength) / 2;
    halfCarLenght = carLength / 2;
    
    static PID dv (0, 0, 1, Tvel, Ts);
    velFilter = &dv;

    for (int i = 0; i < posFilter.count (); i++)
        posFilter.calculate (0);
}

void Carriage::update ()
{
    if (x > 0)
    {
        x = posFilter.calculate(-(sonarR->readf () - halfTrackLength - halfCarLenght) * 0.01).present_value ();
    }
    else
    {
        x = posFilter.calculate((sonarL->readf () - halfTrackLength - halfCarLenght) * 0.01).present_value ();
    }
    // x = (sonarL->read () - halfTrackLength - halfCarLenght) * 0.01;
    // x = posFilter->tick (x - posFilter->getLast ());
    x_i = velFilter->tick (x);
}
