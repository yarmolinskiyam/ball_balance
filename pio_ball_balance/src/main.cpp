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
// Slider voltageSlider (A0, -12, 12);

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
  // bool button = !digitalRead (BUTTON);
  // float v = voltageSlider.getValue () * button;

  static float constexpr K = 0.65;
  static float constexpr T = 0.05;
  static float constexpr Tu = 0.01;

  static float constexpr Tk = T;
  static float constexpr Kk = T / (2*K*Tu);

  static PID piVel (Kk, Kk/Tk, 0, 1, 0.005);
  static PID pPos (1/ (2*2*Tu), 0, 0, 1, 0.005);


  float motorAngle = motor.getAngleRad ();
  float motorVel = motor.getAngleVel ();
  float u = piVel.tick (pPos.tick (setPoint - motorAngle) - motorVel);

  motor.setU (u);

  // Serial.print ("micros = ");
  // Serial.print (micros ());
  Serial.print (", voltage = ");
  Serial.print (u);
  Serial.print (", vel = ");
  Serial.print (motorVel);
  Serial.print (", pos = ");
  Serial.print (motorAngle);
  Serial.println ();
}