#include <Arduino.h>
#include <ArduinoLog.h>

#include "carriage.h"

#define ARDUINO_LOGS_ENABLE_COLORS

#define ENC1 2
#define ENC2 3

#define IN3 10
#define IN4 11

#define BUTTON 13

Carriage car (36, 6); /*!< Для получения координаты вагона */

void setup()
{
  Serial.begin (9600);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  pinMode (ENC1, INPUT);
  pinMode (ENC2, INPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);

  pinMode (BUTTON, INPUT_PULLUP);
}

void loop()
{
  int carX = car.getX ();

  Log.notice ("carX = %d cm" CR, carX);


  // bool enc1 = digitalRead (ENC1);
  // bool enc2 = digitalRead (ENC2);

  // Log.notice ("%d %d\n", enc1, enc2);


  // digitalWrite (IN3, HIGH);
  // digitalWrite (IN4, digitalRead (BUTTON));
}