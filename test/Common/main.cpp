#include <Arduino.h>
#include <unity.h>

extern void runSideToneRouterTests();

void setup()
{
    delay(5000);
    UNITY_BEGIN();
    delay(5000);
}

void loop()
{
    runSideToneRouterTests();
    UNITY_END();
    delay(5000);
}