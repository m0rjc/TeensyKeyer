#include <Arduino.h>
#include <unity.h>

#include "MorseDecode.h"
#include "MorseCharacters.h"

#define DIT MorseDecode::onDit()
#define DAH MorseDecode::onDah()
#define REST MorseDecode::onCharacterGap()

static unsigned short lastSymbol = 0xFF;
static unsigned int called = 0;

static void onSymbol(unsigned short symbol)
{
    lastSymbol = symbol;
    called++;
}

static void beforeEach()
{
    lastSymbol = 0xFF;
    called = 0;
    MorseDecode::init(onSymbol);
}

void itDoesntGenerateSpuriousOutputOnStartup() {
    beforeEach();
    REST;
    TEST_ASSERT_EQUAL(0, called);
}

void itDoesNotGenerateSpuriousSpaceOnIdleAtStartup() {
    beforeEach();
    for(int i = 0; i < 100; i++) REST;
    TEST_ASSERT_EQUAL(0, called);
}

void whenReceivingLetterA_givesCorrectCode()
{
    beforeEach();
    DIT;
    DAH;
    REST;
    TEST_ASSERT_EQUAL(MORSE_A, lastSymbol);
    TEST_ASSERT_EQUAL(1, called);
}

void whenReceivingLetterN_givesCorrectCode()
{
    beforeEach();
    DAH;
    DIT;
    REST;
    TEST_ASSERT_EQUAL(MORSE_N, lastSymbol);
    TEST_ASSERT_EQUAL(1, called);
}

void whenReceivingWordParis_givesCorrectCodes()
{
    beforeEach();
    DIT;
    DAH;
    DAH;
    DIT;
    REST;
    TEST_ASSERT_EQUAL(MORSE_P, lastSymbol);
    DIT;
    DAH;
    REST;
    TEST_ASSERT_EQUAL(MORSE_A, lastSymbol);
    DIT;
    DAH;
    DIT;
    REST;
    TEST_ASSERT_EQUAL(MORSE_R, lastSymbol);
    DIT;
    DIT;
    REST;
    TEST_ASSERT_EQUAL(MORSE_I, lastSymbol);
    DIT;
    DIT;
    DIT;
    REST;
    TEST_ASSERT_EQUAL(MORSE_S, lastSymbol);
    TEST_ASSERT_EQUAL(5, called);
}

void whenLessThanFourSpacesReceivedDoesNotSendSpace()
{
    beforeEach();
    DIT;
    REST;
    TEST_ASSERT_EQUAL(MORSE_E, lastSymbol);
    REST;
    TEST_ASSERT_EQUAL(1, called);
    REST;
    TEST_ASSERT_EQUAL(1, called);
    DAH;
    REST;
    TEST_ASSERT_EQUAL(MORSE_T, lastSymbol);
    TEST_ASSERT_EQUAL(2, called);
}

void whenFourSpacesReceivedSendsSpace()
{
    beforeEach();
    DIT;
    REST;
    TEST_ASSERT_EQUAL(MORSE_E, lastSymbol);
    REST;
    REST;
    REST;
    TEST_ASSERT_EQUAL(MORSE_SPACE, lastSymbol);
    TEST_ASSERT_EQUAL(2, called);
    DAH;
    REST;
    TEST_ASSERT_EQUAL(MORSE_T, lastSymbol);
    TEST_ASSERT_EQUAL(3, called);
}

void whenLeftIdleItOnlySendsSpaceOnce()
{
    beforeEach();
    DIT;
    REST;
    TEST_ASSERT_EQUAL(1, called);
    for (int i = 0; i < 20; i++)
        REST;
    TEST_ASSERT_EQUAL(2, called);
    TEST_ASSERT_EQUAL(MORSE_SPACE, lastSymbol);
    DIT;
    REST;
    TEST_ASSERT_EQUAL(3, called);
    TEST_ASSERT_EQUAL(MORSE_E, lastSymbol);
}

void whenReceivingEigthDitSendsBackspace()
{
    beforeEach();
    for(int i = 0; i < 7; i++) DIT;
    TEST_ASSERT_EQUAL(0, called);
    DIT;
    TEST_ASSERT_EQUAL(1, called);
    TEST_ASSERT_EQUAL(MORSE_BACKSPACE, lastSymbol);
}

void whenReceivingSixteenthDitSendsBackspaceAgain()
{
    beforeEach();
    for (int i = 0; i < 15; i++)
        DIT;
    TEST_ASSERT_EQUAL(1, called);
    DIT;
    TEST_ASSERT_EQUAL(2, called);
    TEST_ASSERT_EQUAL(MORSE_BACKSPACE, lastSymbol);
}

void whenReceivingAllDitsSendsBackspaceRepeatedly()
{
    beforeEach();
    for(int run = 1; run <= 100; run++) {
        for(int dit = 0; dit < 8; dit++) {
            DIT;
        }
        TEST_ASSERT_EQUAL(MORSE_BACKSPACE, lastSymbol);
        TEST_ASSERT_EQUAL(run, called);    
    }
    REST;
    DAH;
    REST;
    TEST_ASSERT_EQUAL(MORSE_T, lastSymbol);
}

void itDoesNotSendASpuriousAllDitCharacterAfterHoldingDownBackspace() {
    beforeEach();
    for (int i = 0; i < 18; i++)
        DIT;
    TEST_ASSERT_EQUAL(2, called);
    TEST_ASSERT_EQUAL(MORSE_BACKSPACE, lastSymbol);
    REST;
    TEST_ASSERT_EQUAL(2, called);
    DAH;
    REST;
    TEST_ASSERT_EQUAL(3, called);
    TEST_ASSERT_EQUAL(MORSE_T, lastSymbol);
}

void setup()
{
    UNITY_BEGIN();
    delay(5000);
}

void loop()
{
    RUN_TEST(itDoesntGenerateSpuriousOutputOnStartup);
    RUN_TEST(itDoesNotGenerateSpuriousSpaceOnIdleAtStartup);
    RUN_TEST(whenReceivingLetterA_givesCorrectCode);
    RUN_TEST(whenReceivingLetterN_givesCorrectCode);
    RUN_TEST(whenReceivingWordParis_givesCorrectCodes);
    RUN_TEST(whenLessThanFourSpacesReceivedDoesNotSendSpace);
    RUN_TEST(whenFourSpacesReceivedSendsSpace);
    RUN_TEST(whenLeftIdleItOnlySendsSpaceOnce);
    RUN_TEST(whenReceivingEigthDitSendsBackspace);
    RUN_TEST(whenReceivingSixteenthDitSendsBackspaceAgain);
    RUN_TEST(whenReceivingAllDitsSendsBackspaceRepeatedly);
    RUN_TEST(itDoesNotSendASpuriousAllDitCharacterAfterHoldingDownBackspace);
    UNITY_END();
    delay(5000);
}