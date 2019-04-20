#include <Arduino.h>
#include <unity.h>
#include "Common.h"
#include "MorseCharacters.h"

void nullBecomesNull() {
    TEST_ASSERT_EQUAL(0, Common::asciiToMorse(0));
}

void asciiSpaceBecomesCodeSpace() {
    TEST_ASSERT_EQUAL(MORSE_SPACE, Common::asciiToMorse(' '));
}

void asciiLowerABecomesCodeA() {
    TEST_ASSERT_EQUAL(MORSE_A, Common::asciiToMorse('a'));
}

void asciiUpperABecomesCodeA() {
    TEST_ASSERT_EQUAL(MORSE_A, Common::asciiToMorse('A'));
}

void numberZeroBecomesCodeZero() {
    TEST_ASSERT_EQUAL(MORSE_0, Common::asciiToMorse('0'));
}

void runAsciiToMorseTests() {
    RUN_TEST(nullBecomesNull);
    RUN_TEST(asciiSpaceBecomesCodeSpace);
    RUN_TEST(asciiLowerABecomesCodeA);
    RUN_TEST(asciiUpperABecomesCodeA);
    RUN_TEST(numberZeroBecomesCodeZero);
}