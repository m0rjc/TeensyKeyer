#include <Arduino.h>
#include <unity.h>

extern void runOutputRouterTests();
extern void runAsciiToMorseTests();

void setup()
{
    delay(5000);
    UNITY_BEGIN();
    delay(5000);
}

void loop()
{
    runOutputRouterTests();
    runAsciiToMorseTests();
    UNITY_END();
    delay(5000);
}