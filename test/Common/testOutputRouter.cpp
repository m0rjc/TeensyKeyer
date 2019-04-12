#include <Arduino.h>
#include <unity.h>
#include "Mocks.h"
#include "Common.h"

// Test is as I plan to use it. Here are three outputs.
SideToneMock radio;
SideToneMock led;
SideToneMock buzzer;

// Each distinct output needs its own router.
OutputRouter onAirRouter(radio);
OutputRouter localRouter(led, buzzer);

// A router combines inputs for both outputs.
OutputRouterInput onAirGroupInput(onAirRouter);
OutputRouterInput localGroupInput(localRouter);
OutputRouter bothRouter(onAirGroupInput, localGroupInput);

// The three inputs
OutputRouterInput iambicKeyerInput(bothRouter);
OutputRouterInput straightKeyerInput(bothRouter);
OutputRouterInput localUxInput(localRouter);

void itStartsAllOff() {
    TEST_ASSERT_EQUAL(false, radio.isTx);
    TEST_ASSERT_EQUAL(false, led.isTx);
    TEST_ASSERT_EQUAL(false, buzzer.isTx);
}

// These tests become stateful as we're using statically compiled setup.
void theStraightKeyOperatesTheRadioTheLedAndTheBuzzer() {
    straightKeyerInput.txOn();
    TEST_ASSERT_EQUAL(true, radio.isTx);
    TEST_ASSERT_EQUAL(true, led.isTx);
    TEST_ASSERT_EQUAL(true, buzzer.isTx);

    straightKeyerInput.txOff();
    TEST_ASSERT_EQUAL(false, radio.isTx);
    TEST_ASSERT_EQUAL(false, led.isTx);
    TEST_ASSERT_EQUAL(false, buzzer.isTx);
}

void theIambicKeyOperatesTheRadioTheLedAndTheBuzzer() {
    iambicKeyerInput.txOn();
    TEST_ASSERT_EQUAL(true, radio.isTx);
    TEST_ASSERT_EQUAL(true, led.isTx);
    TEST_ASSERT_EQUAL(true, buzzer.isTx);

    iambicKeyerInput.txOff();
    TEST_ASSERT_EQUAL(false, radio.isTx);
    TEST_ASSERT_EQUAL(false, led.isTx);
    TEST_ASSERT_EQUAL(false, buzzer.isTx);
}

void theLocalMessageGeneratorOperatesOnlyTheLocalOutputs() {
    localUxInput.txOn();
    TEST_ASSERT_EQUAL(false, radio.isTx);
    TEST_ASSERT_EQUAL(true, led.isTx);
    TEST_ASSERT_EQUAL(true, buzzer.isTx);

    localUxInput.txOff();
    TEST_ASSERT_EQUAL(false, radio.isTx);
    TEST_ASSERT_EQUAL(false, led.isTx);
    TEST_ASSERT_EQUAL(false, buzzer.isTx);
}

void whenTheStraightKeyIsHeldDownTheOutputsLatchUntilItIsReleased() {
    straightKeyerInput.txOn();

    localUxInput.txOn();
    TEST_ASSERT_EQUAL(true, radio.isTx);
    TEST_ASSERT_EQUAL(true, led.isTx);
    TEST_ASSERT_EQUAL(true, buzzer.isTx);

    localUxInput.txOff();
    TEST_ASSERT_EQUAL(true, radio.isTx);
    TEST_ASSERT_EQUAL(true, led.isTx);
    TEST_ASSERT_EQUAL(true, buzzer.isTx);

    straightKeyerInput.txOff();
    TEST_ASSERT_EQUAL(false, radio.isTx);
    TEST_ASSERT_EQUAL(false, led.isTx);
    TEST_ASSERT_EQUAL(false, buzzer.isTx);
}

void repeatCallsDoNotBreakIt() {
    TEST_ASSERT_EQUAL(false, radio.isTx);
    straightKeyerInput.txOn();
    TEST_ASSERT_EQUAL(true, radio.isTx);
    straightKeyerInput.txOn();
    straightKeyerInput.txOn();
    straightKeyerInput.txOn();
    straightKeyerInput.txOn();
    TEST_ASSERT_EQUAL(true, radio.isTx);
    straightKeyerInput.txOff();
    TEST_ASSERT_EQUAL(false, radio.isTx);
}

void runOutputRouterTests() {
    RUN_TEST(itStartsAllOff);
    RUN_TEST(theStraightKeyOperatesTheRadioTheLedAndTheBuzzer);
    RUN_TEST(theIambicKeyOperatesTheRadioTheLedAndTheBuzzer);
    RUN_TEST(theLocalMessageGeneratorOperatesOnlyTheLocalOutputs);
    RUN_TEST(whenTheStraightKeyIsHeldDownTheOutputsLatchUntilItIsReleased);
    RUN_TEST(repeatCallsDoNotBreakIt);
}
