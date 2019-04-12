#include <Arduino.h>
#include <unity.h>

extern void runKeyerTests();
extern void runStateDriverTests();

void setup()
{
    UNITY_BEGIN();
    delay(2000);
    runStateDriverTests();
    runKeyerTests();
    UNITY_END();
}

void loop() {
}