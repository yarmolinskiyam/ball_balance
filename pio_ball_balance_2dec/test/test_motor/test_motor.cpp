#include <unity.h>
#include "motor.h"

void setUp (void)
{
    // set stuff up here
}

void tearDown (void)
{
    // clean stuff up here
}

void testMotor ()
{
    Motor motor (48, 47);

    float angle0 = motor.getAngleRad ();

    motor.setU (6);
    delay (200);

    TEST_ASSERT_TRUE (motor.getAngleRad () > angle0);
}


int main (int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(testMotor);

    UNITY_END();
}
