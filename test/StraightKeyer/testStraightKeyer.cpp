#include <Arduino.h>
#include <unity.h>
#include "StriaghtKeyer.h"
#include "../Common/Mocks.h"

Common::pollingLoopTime_t tick = 0;

void onFirstPollItWillSetTxOnIfKeyDown() {
    PinInputMock hardware;
    SideToneMock sideTone;
    DecoderMock decoder;

    hardware.state = true;

    StraightKeyer::Keyer keyer(hardware, decoder, sideTone);
    keyer.poll(tick++);

    TEST_ASSERT_EQUAL(true, sideTone.isTx);
    TEST_ASSERT_EQUAL(1, sideTone.callCount);
}

void onFirstPollItWillClearTxOnIfKeyUp()
{
    PinInputMock hardware;
    SideToneMock sideTone;
    DecoderMock decoder;

    hardware.state = false;

    StraightKeyer::Keyer keyer(hardware, decoder, sideTone);
    keyer.poll(tick++);

    TEST_ASSERT_EQUAL(false, sideTone.isTx);
    TEST_ASSERT_EQUAL(1, sideTone.callCount);
}

void onSubsequentPollsNoChangeItDoesNotCallSidetone() {
    PinInputMock hardware;
    SideToneMock sideTone;
    DecoderMock decoder;

    hardware.state = true;

    StraightKeyer::Keyer keyer(hardware, decoder, sideTone);
    keyer.poll(tick++);
    keyer.poll(tick++);
    keyer.poll(tick++);
    TEST_ASSERT_EQUAL(true, sideTone.isTx);
    TEST_ASSERT_EQUAL(1, sideTone.callCount);
}

void whenInputChangesToHighItCallsSideToneOn() {
    PinInputMock hardware;
    SideToneMock sideTone;
    DecoderMock decoder;

    hardware.state = false;

    StraightKeyer::Keyer keyer(hardware, decoder, sideTone);
    keyer.poll(tick++);

    hardware.state = true;
    keyer.poll(tick++);

    TEST_ASSERT_EQUAL(true, sideTone.isTx);
    TEST_ASSERT_EQUAL(2, sideTone.callCount);
}

void whenInputChangesToLowItCallsSideToneOff()
{
    PinInputMock hardware;
    SideToneMock sideTone;
    DecoderMock decoder;

    hardware.state = true;

    StraightKeyer::Keyer keyer(hardware, decoder, sideTone);
    keyer.poll(tick++);

    hardware.state = false;
    keyer.poll(tick++);

    TEST_ASSERT_EQUAL(false, sideTone.isTx);
    TEST_ASSERT_EQUAL(2, sideTone.callCount);
}

void setup()
{
    Serial.begin(9600);
    delay(4000);
    UNITY_BEGIN();
}

void loop()
{
    RUN_TEST(onFirstPollItWillSetTxOnIfKeyDown);
    RUN_TEST(onFirstPollItWillClearTxOnIfKeyUp);
    RUN_TEST(onSubsequentPollsNoChangeItDoesNotCallSidetone);
    RUN_TEST(whenInputChangesToHighItCallsSideToneOn);
    RUN_TEST(whenInputChangesToLowItCallsSideToneOff);
    UNITY_END();
}