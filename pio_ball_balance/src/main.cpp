#include <Arduino.h>
#include <Ultrasonic.h>
#include <ArduinoLog.h>

#define ARDUINO_LOGS_ENABLE_COLORS

Ultrasonic sonar (6, 7, 1000000/330);

void setup()
{
  Serial.begin (9600);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
}

void loop()
{
  uint32_t timeBefore = micros ();
  unsigned int distance = sonar.read ();
  uint32_t timeAfter = micros ();

  if (distance > 40);
    // Log.warning ("dist = %d cm" CR, distance);
  else
    Log.notice ("dist = %d cm" CR, distance);
}