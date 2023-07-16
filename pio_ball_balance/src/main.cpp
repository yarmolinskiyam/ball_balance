#include <Arduino.h>

// #include "carriage.h"
// #include "motor.h"
// #include "slider.h"
#include "overlord.h"

#define BUTTON 13

Overlord &overlord = Overlord::getInstance ();

void controller (Overlord &over)
{
  float setPoint = over.getSetpoint ();
  // bool button = !digitalRead (BUTTON);
  // float v = voltageSlider.getValue () * button;

  static float constexpr K = 0.65;
  static float constexpr T = 0.05;
  static float constexpr Tu = 0.01;

  static float constexpr Tk = T;
  static float constexpr Kk = T / (2*K*Tu);

  static float constexpr ia = 15/3;
  static float constexpr g = 9.81;

  float Tum = Tu;

  static PID piVel (Kk, Kk/Tk, 0, 1, 0.005, -12, 12);
  Tum *= 2;
  static PID pPos (1/ (2*Tum), 0, 0, 1, 0.005, -0.1, 0.1);
  Tum = 2*Tum + 0.2;
  static PID pCarVel (ia / (2*g*Tum), 0, 0, 1, 0.005);
  Tum = 2*Tum;
  static PID pCarPos (1 / (2*Tum), 0, 0, 1, 0.005);
  
  float carX = -over.getCarX ();
  float carVel = -over.getCarVel ();
  float motorSetPoint = pCarVel.tick (pCarPos.tick (setPoint - carX) - carVel);

  float motorAngle = over.getMotorTheta ();
  float motorVel = over.getMotorVel ();
  float u = piVel.tick (pPos.tick (motorSetPoint - motorAngle) - motorVel);

  // over. (u);

  // Serial.print (micros ());
  // Serial.print ('\t');
  Serial.print (carX);
  Serial.print ('\t');
  Serial.print (carVel);
  Serial.print ('\t');
  // Serial.print (motorAngle);
  // Serial.print ('\t');
  // Serial.print (motorVel);
  // Serial.print ('\t');
  Serial.println ();
}

void setup()
{
  Serial.begin (115200);

  pinMode (BUTTON, INPUT_PULLUP);

  overlord.init (controller);
}

void loop ()
{
  overlord.tick ();
}
