#include <Arduino.h>
#include <unity.h>
// #include "carriage.h"

void setUp (void)
{
    pinMode (13, INPUT_PULLUP);
    pinMode (A0, INPUT);
    pinMode (A1, INPUT);
    pinMode (A2, INPUT);
}

void tearDown (void)
{
    // clean stuff up here
}

void testButton ()
{
    Serial.println ("Press the button");

    int i;
    for (i = 0; i < 5000; i++)
    {
        if (LOW == digitalRead (13))
        {
            TEST_ASSERT_TRUE (true);
            break;
        }
        delay (1);
    }
    if (i == 4999)
        TEST_ASSERT_TRUE (false);
}

void testPot ()
{
    int pot = analogRead (A0);
    TEST_ASSERT_INT16_WITHIN (200, 512, pot);
    pot = analogRead (A1);
    TEST_ASSERT_INT16_WITHIN (200, 512, pot);
    pot = analogRead (A2);
    TEST_ASSERT_INT16_WITHIN (200, 512, pot);
}

int main (int argc, char **argv)
{
    UNITY_BEGIN();

    // RUN_TEST(testButton);
    RUN_TEST(testPot);

    UNITY_END();
}
