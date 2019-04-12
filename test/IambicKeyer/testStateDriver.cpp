#include <Arduino.h>
#include <unity.h>

#include "IambicKeyer.h"
#include "../Common/Mocks.h"

using namespace IambicKeyer;
using Common::pollingLoopTime_t;

class StateMock : public IState {
    public:
        int enterCalledCount = 0;
        Common::KeyerMode modeOnEnter;
        unsigned int *timeoutToSetOnEnter = NULL;

        void onEnter(IStateContext *context) {
            enterCalledCount++;
            modeOnEnter = context->getKeyerMode();
            if(timeoutToSetOnEnter) {
                context->setTimeout(*timeoutToSetOnEnter);
            }
        }

        int timeoutCalledCount = 0;
        IState *stateFromTimeout = NULL;
        Common::KeyInput keysOnTimeout;
        bool squeezeCapturedOnTimeout;

        void onTimeout(IStateContext *context) {
            timeoutCalledCount++;
            keysOnTimeout = context->readSwitches();
            squeezeCapturedOnTimeout = context->isSqueezeCaptured();
            if(stateFromTimeout) {
                context->enterState(stateFromTimeout);
            }
        }

        int switchChangeCalledCount = 0;
        KeyInput lastSwitchState = KeyInput::none;
        bool squeezeCaptured = false;

        void onSwitchChange(IStateContext *context)
        {
            switchChangeCalledCount++;
            lastSwitchState = context->readSwitches();
            squeezeCaptured = context->isSqueezeCaptured();
        }
};

void theTestHarnessMathsDoesNotOverflow() {
    TEST_ASSERT_EQUAL((pollingLoopTime_t)2400, calculateTimeout(1, 200)); // One whole DIT (mark+space) at 1wpm.
    TEST_ASSERT_EQUAL((pollingLoopTime_t)4800, calculateTimeout(1, 400)); // One whole DAH (mark+space) at 1wpm.
    TEST_ASSERT_EQUAL((pollingLoopTime_t)24,   calculateTimeout(50, 100)); // Mark only for the DIT at 50wpm.
}

void onFirstPoll_itCallsOnEnterInTheInitialState() {
    StateMock initialState;
    HardwareMock hardware;
    DecoderMock decoder;
    SideToneMock sideTone;

    StateDriver driver(hardware, decoder, sideTone, &initialState);

    driver.poll(0);

    TEST_ASSERT_EQUAL(1, initialState.enterCalledCount);
}

void onInitialise_startsUpInModeB() {
    StateMock initialState;
    HardwareMock hardware;
    DecoderMock decoder;
    SideToneMock sideTone;

    StateDriver driver(hardware, decoder, sideTone, &initialState);

    driver.poll(0);

    TEST_ASSERT_EQUAL(KeyerMode::modeB, initialState.modeOnEnter);
}

void setTimeout_willCallBackInTheSpecifiedTimeOnceOnly() {
    StateMock initialState;
    HardwareMock hardware;
    DecoderMock decoder;
    SideToneMock sideTone;
    pollingLoopTime_t currentTime = 10000;

    unsigned int timeoutVal = 400;
    initialState.timeoutToSetOnEnter = &timeoutVal;

    StateDriver driver(hardware, decoder, sideTone, &initialState);
    driver.poll(currentTime);

    currentTime += calculateTimeout(DEFAULT_WPM, timeoutVal) - 1;
    driver.poll(currentTime);
    TEST_ASSERT_EQUAL(0, initialState.timeoutCalledCount);

    currentTime += 1;
    driver.poll(currentTime);
    TEST_ASSERT_EQUAL(1, initialState.timeoutCalledCount);

    currentTime += calculateTimeout(DEFAULT_WPM, 400);
    driver.poll(currentTime);
    TEST_ASSERT_EQUAL(1, initialState.timeoutCalledCount);
}

void setRate_producesTimeIntervalOfDesiredRate()
{
    // This tests both entering a timeout from transition and changing rate. Earlier code did not allow me to set an
    // initial rate before first allowing the State to activate.
    // This doesn't matter in the live application because the device will sit there doing character gaps until the human
    // has time to reach for the keys.
    StateMock initialState;
    StateMock secondState;
    HardwareMock hardware;
    DecoderMock decoder;
    SideToneMock sideTone;
    pollingLoopTime_t currentTime = 10000;

    unsigned int timeoutVal1 = 50;
    initialState.timeoutToSetOnEnter = &timeoutVal1;
    initialState.stateFromTimeout = &secondState;

    unsigned int timeoutVal2 = 100;
    secondState.timeoutToSetOnEnter = &timeoutVal2;

    StateDriver driver(hardware, decoder, sideTone, &initialState);
    driver.poll(currentTime);  // Before rate change to simulate old behaviour.
    driver.setRate(12);

    currentTime += calculateTimeout(DEFAULT_WPM, timeoutVal1);
    driver.poll(currentTime);
    TEST_ASSERT_EQUAL(1, initialState.timeoutCalledCount);

    currentTime += calculateTimeout(12, timeoutVal2) - 1;
    driver.poll(currentTime);
    TEST_ASSERT_EQUAL(0, secondState.timeoutCalledCount);

    currentTime += 1;
    driver.poll(currentTime);
    TEST_ASSERT_EQUAL(1, secondState.timeoutCalledCount);
}

void setTimeout_willHandleTimerOverflowCorrectly() {
    StateMock initialState;
    HardwareMock hardware;
    DecoderMock decoder;
    SideToneMock sideTone;
    pollingLoopTime_t currentTime = 0xFFFFFFF0UL;

    unsigned int timeoutVal = 400;
    initialState.timeoutToSetOnEnter = &timeoutVal;

    StateDriver driver(hardware, decoder, sideTone, &initialState);
    driver.poll(currentTime);

    currentTime = calculateTimeout(DEFAULT_WPM, timeoutVal) - 0x11;
    driver.poll(currentTime);
    TEST_ASSERT_EQUAL(0, initialState.timeoutCalledCount);

    currentTime += 1;
    driver.poll(currentTime);
    TEST_ASSERT_EQUAL(1, initialState.timeoutCalledCount);
}

void setTimeout_canHandleTheLongestExpectedTime() {
    StateMock initialState;
    HardwareMock hardware;
    DecoderMock decoder;
    SideToneMock sideTone;
    pollingLoopTime_t currentTime = 10000;

    unsigned int timeoutVal1 = 400;
    initialState.timeoutToSetOnEnter = &timeoutVal1;

    StateDriver driver(hardware, decoder, sideTone, &initialState);
    // Rate set before starting state engine, so we can test this in a single transition.
    driver.setRate(5);
    driver.poll(currentTime);

    currentTime += calculateTimeout(5, timeoutVal1) - 1;
    driver.poll(currentTime);
    TEST_ASSERT_EQUAL(0, initialState.timeoutCalledCount);

    currentTime += 1;
    driver.poll(currentTime);
    TEST_ASSERT_EQUAL(1, initialState.timeoutCalledCount);
}

void setTimeout_canHandleTheShortestExpectedTime() {
    // 3/4 dit length at 50 if we simulate the KX3's Weight function
    StateMock initialState;
    StateMock secondState;
    HardwareMock hardware;
    DecoderMock decoder;
    SideToneMock sideTone;
    pollingLoopTime_t currentTime = 10000;

    unsigned int timeoutVal1 = 50;
    initialState.timeoutToSetOnEnter = &timeoutVal1;
    initialState.stateFromTimeout = &secondState;

    unsigned int timeoutVal2 = 75;
    secondState.timeoutToSetOnEnter = &timeoutVal2;

    StateDriver driver(hardware, decoder, sideTone, &initialState);
    driver.poll(currentTime);
    driver.setRate(50);

    currentTime += calculateTimeout(DEFAULT_WPM, timeoutVal1);
    driver.poll(currentTime);
    TEST_ASSERT_EQUAL(1, initialState.timeoutCalledCount);

    currentTime += calculateTimeout(50, timeoutVal2) - 1;
    driver.poll(currentTime);
    TEST_ASSERT_EQUAL(0, secondState.timeoutCalledCount);

    currentTime += 1;
    driver.poll(currentTime);
    TEST_ASSERT_EQUAL(1, secondState.timeoutCalledCount);
}

void pollWillNotifyStateOnSwitchChange() {
    StateMock initialState;
    HardwareMock hardware;
    DecoderMock decoder;
    SideToneMock sideTone;
    pollingLoopTime_t currentTime = 10000;

    StateDriver driver(hardware, decoder, sideTone, &initialState);

    driver.poll(currentTime);
    TEST_ASSERT_EQUAL(0, initialState.switchChangeCalledCount);

    hardware.currentInput = KeyInput::dit;
    driver.poll(currentTime);
    TEST_ASSERT_EQUAL(1, initialState.switchChangeCalledCount);
    TEST_ASSERT_EQUAL(KeyInput::dit, initialState.lastSwitchState);
    TEST_ASSERT_EQUAL(false, initialState.squeezeCaptured);
}

void squeezeCanBeCapturedAcrossTimouts() {
    StateMock initialState;
    HardwareMock hardware;
    DecoderMock decoder;
    SideToneMock sideTone;
    pollingLoopTime_t currentTime = 10000;

    unsigned int timeout1 = 100;
    initialState.timeoutToSetOnEnter = &timeout1;

    StateDriver driver(hardware, decoder, sideTone, &initialState);

    driver.poll(currentTime);

    hardware.currentInput = KeyInput::squeeze;
    driver.poll(currentTime); 

    hardware.currentInput = KeyInput::dah;
    driver.poll(currentTime);

    currentTime += calculateTimeout(DEFAULT_WPM, timeout1);
    driver.poll(currentTime);

    TEST_ASSERT_EQUAL(KeyInput::dah, initialState.keysOnTimeout);
    TEST_ASSERT_EQUAL(true, initialState.squeezeCapturedOnTimeout);
}

void runStateDriverTests()
{
    RUN_TEST(theTestHarnessMathsDoesNotOverflow);
    RUN_TEST(onFirstPoll_itCallsOnEnterInTheInitialState);
    RUN_TEST(onInitialise_startsUpInModeB);
    RUN_TEST(setTimeout_willCallBackInTheSpecifiedTimeOnceOnly);
    RUN_TEST(setRate_producesTimeIntervalOfDesiredRate);
    RUN_TEST(setTimeout_willHandleTimerOverflowCorrectly);
    RUN_TEST(setTimeout_canHandleTheLongestExpectedTime);
    RUN_TEST(setTimeout_canHandleTheShortestExpectedTime);
    RUN_TEST(pollWillNotifyStateOnSwitchChange);
    RUN_TEST(squeezeCanBeCapturedAcrossTimouts);
}