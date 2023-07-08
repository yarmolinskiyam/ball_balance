#include <Arduino.h>
#include <Ultrasonic.h>
#include <ArduinoLog.h>

#define ARDUINO_LOGS_ENABLE_COLORS

#define ENC1 2
#define ENC2 3

#define IN3 10
#define IN4 11

#define BUTTON 13

Ultrasonic sonar (7, 6, 1000000/330);

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
  // uint32_t timeBefore = micros ();
  // unsigned int distance = sonar.read ();
  // uint32_t timeAfter = micros ();

  // if (distance > 40);
  //   // Log.warning ("dist = %d cm" CR, distance);
  // else
  //   Log.notice ("dist = %d cm" CR, distance);


  // bool enc1 = digitalRead (ENC1);
  // bool enc2 = digitalRead (ENC2);

  // Log.notice ("%d %d\n", enc1, enc2);


  digitalWrite (IN3, HIGH);
  digitalWrite (IN4, digitalRead (BUTTON));
}