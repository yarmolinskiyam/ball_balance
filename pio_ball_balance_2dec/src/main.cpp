#include <Arduino.h>

#include "overlord.h"
#include "student.h"

#define BUTTON 13

Overlord &overlord = Overlord::getInstance ();

void setup()
{
  Serial.begin (2000000);

  pinMode (BUTTON, INPUT_PULLUP);

  overlord.init (controllerTick);

  controllerInit (overlord);

  Serial.println ("Init complete");
  while (digitalRead (BUTTON));
}

void loop ()
{
  overlord.tick ();
}
