#include <unity.h>
typedef bool boolean;
#include "carriage.h"

void setUp (void)
{
    // set stuff up here
}

void tearDown (void)
{
    // clean stuff up here
}

void testSonar ()
{
    Carriage car (0, 0);
    car.update ();

    TEST_ASSERT_TRUE (car.getX () > 0.001);
}


int main (int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(testSonar);

    UNITY_END();
}
