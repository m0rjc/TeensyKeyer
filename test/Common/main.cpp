#include <Arduino.h>
#include <unity.h>

extern void runOutputRouterTests();

void setup()
{
    delay(5000);
    UNITY_BEGIN();
    delay(5000);
}

void loop()
{
    runOutputRouterTests();
    UNITY_END();
    delay(5000);
}