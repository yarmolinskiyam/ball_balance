#include <Arduino.h>
#include <ArduinoLog.h>

#include "carriage.h"
#include "motor.h"
#include "slider.h"

#define ARDUINO_LOGS_ENABLE_COLORS

#define ENC1 2
#define ENC2 3

#define IN3 10
#define IN4 11

#define BUTTON 13
#define POT A0

Carriage car (36, 6); /*!< Для получения координаты вагона */
Motor motor (48, 47);
Slider setPointSlider (A0, -400, 400);

unsigned long timer;

void setup()
{
  Serial.begin (115200);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  pinMode (BUTTON, INPUT_PULLUP);

  timer = micros ();
}

void loop()
{
  while (micros () - timer < 5000);
  timer = micros ();

  float setPoint = setPointSlider.getValue () * 0.001;

  static float U_zn0 = 0;
  static float U_zn1 = 0;
  static float U_zn2 = 0;
  static float enc_zn0 = 0;
  static float enc_zn1 = 0;
  static float enc_zn2 = 0;

  enc_zn0 = (setPoint - motor.getAngleRad ());

  U_zn0 = 0  +1.942068*U_zn1 -0.942068*U_zn2
      +22.100716*enc_zn0 -42.178178*enc_zn1 +20.089397*enc_zn2;

  U_zn2 = U_zn1;
  U_zn1 = U_zn0;

  enc_zn2 = enc_zn1;
  enc_zn1 = enc_zn0;

  motor.setU (U_zn0);

  // float U = map (analogRead (POT), 0, 1023, -12, 12);

  // motor.setU (U);

  // Log.notice ("micros () = %d, U = %d\n", millis (), (int)(U * 10));
  // Log.notice ("speed = %d\n", speed * 1000);
  Serial.print ("micros = ");
  Serial.print (micros ());
  Serial.print (", setpoint = ");
  Serial.print (setPoint);
  Serial.print (", angle = ");
  Serial.print (motor.getAngleRad ());
  Serial.println ();
  // Log.notice ("setpoint = %d,\tenc = %d ticks" CR, long(setPoint*10), long (motor.getAngleRad () * 10));
  // delay (1);

  // int carX = car.getX ();

  // Log.notice ("carX = %d cm" CR, carX);

  // motor.setU (digitalRead (BUTTON) ? 0 : 5);


  // bool enc1 = digitalRead (ENC1);
  // bool enc2 = digitalRead (ENC2);

  // Log.notice ("%d %d\n", enc1, enc2);


  // digitalWrite (IN3, HIGH);
  // digitalWrite (IN4, digitalRead (BUTTON));
}