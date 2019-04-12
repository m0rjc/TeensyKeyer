#include <Arduino.h>
#include <unity.h>

#include "MorseDecode.h"
#include "MorseCharacters.h"

const unsigned short GAPS_FOR_SPACE=3;

class MockCallback : public Common::ISymbolCallback
{
    public: 
    unsigned short lastSymbol = 0xFF;
    unsigned int called = 0;

    void onSymbol(unsigned short symbol)
    {
        lastSymbol = symbol;
        called++;
    }
};

void itDoesntGenerateSpuriousOutputOnStartup() {
    MockCallback callback;
    MorseDecode::MorseDecoder decoder(callback); 
    decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(0, callback.called);
}

void itDoesNotGenerateSpuriousSpaceOnIdleAtStartup() {
    MockCallback callback;
    MorseDecode::MorseDecoder decoder(callback);
    for(int i = 0; i < 100; i++) decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(0, callback.called);
}

void whenReceivingLetterA_givesCorrectCode()
{
    MockCallback callback;
    MorseDecode::MorseDecoder decoder(callback);
    decoder.onDit();
    decoder.onDah();
    decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(MORSE_A, callback.lastSymbol);
    TEST_ASSERT_EQUAL(1, callback.called);
}

void whenReceivingLetterN_givesCorrectCode()
{
    MockCallback callback;
    MorseDecode::MorseDecoder decoder(callback);
    decoder.onDah();
    decoder.onDit();
    decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(MORSE_N, callback.lastSymbol);
    TEST_ASSERT_EQUAL(1, callback.called);
}

void whenReceivingWordParis_givesCorrectCodes()
{
    MockCallback callback;
    MorseDecode::MorseDecoder decoder(callback);
    decoder.onDit();
    decoder.onDah();
    decoder.onDah();
    decoder.onDit();
    decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(MORSE_P, callback.lastSymbol);
    decoder.onDit();
    decoder.onDah();
    decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(MORSE_A, callback.lastSymbol);
    decoder.onDit();
    decoder.onDah();
    decoder.onDit();
    decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(MORSE_R, callback.lastSymbol);
    decoder.onDit();
    decoder.onDit();
    decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(MORSE_I, callback.lastSymbol);
    decoder.onDit();
    decoder.onDit();
    decoder.onDit();
    decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(MORSE_S, callback.lastSymbol);
    TEST_ASSERT_EQUAL(5, callback.called);
}

void whenLessThanWordSpaceReceivedDoesNotSendSpace()
{
    MockCallback callback;
    MorseDecode::MorseDecoder decoder(callback);
    decoder.onDit();
    for(int i = 1; i < GAPS_FOR_SPACE; i++) decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(MORSE_E, callback.lastSymbol);
    TEST_ASSERT_EQUAL(1, callback.called);
    decoder.onDah();
    decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(MORSE_T, callback.lastSymbol);
    TEST_ASSERT_EQUAL(2, callback.called);
}

void whenWordSpaceSpacesReceivedSendsSpace()
{
    MockCallback callback;
    MorseDecode::MorseDecoder decoder(callback);
    decoder.onDit();
    decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(MORSE_E, callback.lastSymbol);
    for(int i = 1; i < GAPS_FOR_SPACE; i++) decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(MORSE_SPACE, callback.lastSymbol);
    TEST_ASSERT_EQUAL(2, callback.called);
    decoder.onDah();
    decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(MORSE_T, callback.lastSymbol);
    TEST_ASSERT_EQUAL(3, callback.called);
}

void whenLeftIdleItOnlySendsSpaceOnce()
{
    MockCallback callback;
    MorseDecode::MorseDecoder decoder(callback);
    decoder.onDit();
    decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(1, callback.called);
    for (int i = 0; i < 20; i++)
        decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(2, callback.called);
    TEST_ASSERT_EQUAL(MORSE_SPACE, callback.lastSymbol);
    decoder.onDit();
    decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(3, callback.called);
    TEST_ASSERT_EQUAL(MORSE_E, callback.lastSymbol);
}

void whenReceivingEigthDitSendsBackspace()
{
    MockCallback callback;
    MorseDecode::MorseDecoder decoder(callback);

    for(int i = 0; i < 7; i++) decoder.onDit();
    TEST_ASSERT_EQUAL(0, callback.called);
    decoder.onDit();
    TEST_ASSERT_EQUAL(1, callback.called);
    TEST_ASSERT_EQUAL(MORSE_BACKSPACE, callback.lastSymbol);
}

void whenReceivingSixteenthDitSendsBackspaceAgain()
{
    MockCallback callback;
    MorseDecode::MorseDecoder decoder(callback);
    for (int i = 0; i < 15; i++)
        decoder.onDit();
    TEST_ASSERT_EQUAL(1, callback.called);
    decoder.onDit();
    TEST_ASSERT_EQUAL(2, callback.called);
    TEST_ASSERT_EQUAL(MORSE_BACKSPACE, callback.lastSymbol);
}

void whenReceivingAllDitsSendsBackspaceRepeatedly()
{
    MockCallback callback;
    MorseDecode::MorseDecoder decoder(callback);

    for (int run = 1; run <= 100; run++)
    {
        for(int dit = 0; dit < 8; dit++) {
            decoder.onDit();
        }
        TEST_ASSERT_EQUAL(MORSE_BACKSPACE, callback.lastSymbol);
        TEST_ASSERT_EQUAL(run, callback.called);    
    }
    decoder.onCharacterGap();
    decoder.onDah();
    decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(MORSE_T, callback.lastSymbol);
}

void itDoesNotSendASpuriousAllDitCharacterAfterHoldingDownBackspace() {
    MockCallback callback;
    MorseDecode::MorseDecoder decoder(callback);

    for (int i = 0; i < 18; i++)
        decoder.onDit();
    TEST_ASSERT_EQUAL(2, callback.called);
    TEST_ASSERT_EQUAL(MORSE_BACKSPACE, callback.lastSymbol);
    decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(2, callback.called);
    decoder.onDah();
    decoder.onCharacterGap();
    TEST_ASSERT_EQUAL(3, callback.called);
    TEST_ASSERT_EQUAL(MORSE_T, callback.lastSymbol);
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
    RUN_TEST(whenLessThanWordSpaceReceivedDoesNotSendSpace);
    RUN_TEST(whenWordSpaceSpacesReceivedSendsSpace);
    RUN_TEST(whenLeftIdleItOnlySendsSpaceOnce);
    RUN_TEST(whenReceivingEigthDitSendsBackspace);
    RUN_TEST(whenReceivingSixteenthDitSendsBackspaceAgain);
    RUN_TEST(whenReceivingAllDitsSendsBackspaceRepeatedly);
    RUN_TEST(itDoesNotSendASpuriousAllDitCharacterAfterHoldingDownBackspace);
    UNITY_END();
    delay(5000);
}