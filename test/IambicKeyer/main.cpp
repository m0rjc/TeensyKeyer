#include <Arduino.h>
#include <unity.h>

extern void runKeyerTests();
extern void runStateDriverTests();

void setup()
{
    delay(5000);
    UNITY_BEGIN();
    delay(2000);
}

void loop() {
    runStateDriverTests();
    runKeyerTests();
    UNITY_END();
}