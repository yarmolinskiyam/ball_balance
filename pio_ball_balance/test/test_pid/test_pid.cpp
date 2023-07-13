#include <unity.h>
#include "pid.h"

void setUp (void)
{
    // set stuff up here
}

void tearDown (void)
{
    // clean stuff up here
}

void testIsSat ()
{
    PID pid (1, 2, 3, 4, 5);

    TEST_ASSERT_FALSE (pid.isSatEnabled ());

    PID pidsat (1, 2, 3, 4, 5, 6, 7);

    TEST_ASSERT_TRUE (pidsat.isSatEnabled ());
}


int main (int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(testIsSat);

    UNITY_END();
}
