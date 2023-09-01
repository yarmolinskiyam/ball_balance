#include "overlord.h"
#include "setpoint.cpp"

Motor *motor;

Overlord::Overlord()
{
    static Carriage car_ (trackLength, carLength);
    static Motor motor_ (ticksPerRev, gearboxRatio);
    static LedManager ledManager_;

    static Slider slider0 (A0);
    static Slider slider1 (A1);
    static Slider slider2 (A2);

    static Slider *sliders_[sliderCount] =
    {
        &slider0,
        &slider1,
        &slider2
    };

    sliders = sliders_;

    sliders[static_cast<int> (SliderEnum::setPoint)]->setSlider (-200, 200);
    
    car = &car_;
    motor = &motor_;
    ledManager = &ledManager_;

    timer = micros ();
}

Overlord& Overlord::getInstance ()
{
    static Overlord overlord;
    return overlord;
}

void Overlord::init (void (*controller_) (Overlord &))
{
    controller = controller_;
}

float Overlord::getSetpoint ()
{
    return (int8_t)pgm_read_byte(&setPointArray[timeTick]) * 0.001;
    // return -sliders[static_cast<int> (SliderEnum::setPoint)]->getValue () * 0.001;
}

void Overlord::tick ()
{
    /*!v Эта переменная будет false если итерация цикла больше периода квантования */
    bool isTimeOk = false;
    while (micros () - timer < Tsmicros) isTimeOk = true;

    if (!isTimeOk)
    {
        unsigned long mcrs = micros ();
        Serial.print ("ПРЕВЫШЕН ПЕРИОД КВАНТОВАНИЯ НА [мкс]: ");
        Serial.println (mcrs - timer - Tsmicros);
    }
    timer = micros ();

    motor->update ();
    car->update ();
    
    float sp = getSetpoint ();
    float x = car->getX ();
    float x_i = car->getVel ();

    controller (*this);

    // float u = motor->getU ();

    ledManager->clear ();
    ledManager->setCar (x, x_i);
    ledManager->setSetpointPos (sp);
    ledManager->show ();

    Serial.print (timer/1000);
    Serial.print (' ');
    // Serial.print (timeTick);
    // Serial.print ('\t');
    Serial.print (sp, 4);
    Serial.print (' ');
    Serial.print (x, 4);
    Serial.print (' ');
    // Serial.print (u);
    // Serial.print (' ');
    Serial.println ();

    timeTick++;
    if (timeTick >= sizeof (setPointArray))
    {
        motor->setU (0);
        Serial.println ("End");

        ledManager->clear ();
        ledManager->show ();

        while (1);
    }
    // float err = getSetpoint () - car->getX ();
    // integralError += err*err * Ts;
}
