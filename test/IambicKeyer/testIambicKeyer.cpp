#include <Arduino.h>
#include <unity.h>
#include <string.h>

#include "IambicKeyer.h"
#include "../Common/Mocks.h"

using namespace IambicKeyer;
using namespace Common;

void runKeyerTest(KeyerMode mode, const char *inputWaveform, const char *toneWaveform, const char *expectedDecode) {
    HardwareMock hardware;
    DecoderMock decoder;
    SideToneMock sideTone;
    pollingLoopTime_t currentTime = 0;

    char actualTone[49];

    const pollingLoopTime_t timeInterval = calculateTimeout(DEFAULT_WPM, 25); // We're working in quarter dits.

    IambicKeyer::Keyer keyer(hardware, decoder, sideTone);
    keyer.setMode(mode);
    keyer.poll(currentTime);

    int i;
    for(i = 0; inputWaveform[i] && toneWaveform[i && i < 48]; i++) {
        currentTime += timeInterval;

        switch(inputWaveform[i]) {
            case ' ':
                hardware.currentInput = KeyInput::none;
                break;
            case '.':
                hardware.currentInput = KeyInput::dit;
                break;
            case '-':
                hardware.currentInput = KeyInput::dah;
                break;
            case 'S':
                hardware.currentInput = KeyInput::squeeze;
                break;
        }

        keyer.poll(currentTime);

        actualTone[i] = sideTone.isTx ? '*' : ' ';
    }
    actualTone[i] = 0;

    TEST_ASSERT_EQUAL_STRING(toneWaveform, actualTone);
    TEST_ASSERT_EQUAL_STRING(expectedDecode, decoder.capture);
}

void givenMorseLetterAOutputsA() {              
    runKeyerTest(KeyerMode::modeB, 
            //   :...:...:...:...:...:...:...:...:...:...:...:...
                "..    -----                             ",
                "****    ************                    ",
                ".- ");
}

void givenMorseLetterIOutputsI()
{
    runKeyerTest(KeyerMode::modeB,
            //    :...:...:...:...:...:...:...:...:...:...:...:...  (48 chars)
                 "..............               ",
                 "****    ****                 ",
                 ".. ");
}

void givenMorseLetterKOutputsK() {
    runKeyerTest(KeyerMode::modeB,
            //    :...:...:...:...:...:...:...:...:...:...:...:...  (48 chars)
                 "-----     ............ ------           ",
                 "************    ****    ************    ",
                 "-.-");
}

void givenIambicRInModeBOutputsR() {
    runKeyerTest(KeyerMode::modeB,
            //    :...:...:...:...:...:...:...:...:...:...:...:...  (48 chars)
                 "...SSS                                        ",
                 "****    ************    ****                  ",
                 ".-. ");
}

void givenIambicKInModeBOutputsK() {
    runKeyerTest(KeyerMode::modeB,
            //    :...:...:...:...:...:...:...:...:...:...:...:...  (48 chars)
                 "---SSS                                       ",
                 "************    ****    ************         ",
                 "-.-");
}

void givenIambicRInModeAOutputsR() {
    runKeyerTest(KeyerMode::modeA,
            //    :...:...:...:...:...:...:...:...:...:...:...:...  (48 chars)
                 "...SSSSSSSSSSSSSSSSSSSSSSS                    ",
                 "****    ************    ****                  ",
                 ".-. ");
}

void givenIambicKInModeAOutputsK() {
    runKeyerTest(KeyerMode::modeA,
            //    :...:...:...:...:...:...:...:...:...:...:...:...  (48 chars)
                 "-SSSSSSSSSSSSSSSSSSSSSSSSS                    ",
                 "************    ****    ************          ",
                 "-.-");
}

void givenIambicFInModeBOutputsF() {
    runKeyerTest(KeyerMode::modeB,
            //    :...:...:...:...:...:...:...:...:...:...:...:...  (48 chars)
                 "...........SSS...                             ",
                 "****    ****    ************    ****          ",
                 "..-.");
}

void givenIambicLInModeBOutputsL() {
    runKeyerTest(KeyerMode::modeB,
            //    :...:...:...:...:...:...:...:...:...:...:...:...  (48 chars)
                 ".....SSSSSS........................         ",
                 "****    ************    ****    ****        ",
                 ".-..");
}

void givenIambicLInModeAOutputsL()
{
    runKeyerTest(KeyerMode::modeA,
            //    :...:...:...:...:...:...:...:...:...:...:...:...  (48 chars)
                 ".....SSSSSS........................         ",
                 "****    ************    ****    ****        ",
                 ".-..");
}

void givenNoInputOutputsStreamOfCharacterGaps()
{
    runKeyerTest(KeyerMode::modeA,
            //    :...:...:...:...:...:...:...:...:...:...:...:...  (48 chars)
                 "                                                ",
                 "                                                ",
                 "    ");
}

void runKeyerTests() {
    RUN_TEST(givenMorseLetterAOutputsA);
    RUN_TEST(givenMorseLetterIOutputsI);
    RUN_TEST(givenMorseLetterKOutputsK);
    RUN_TEST(givenIambicRInModeBOutputsR);
    RUN_TEST(givenIambicKInModeBOutputsK);
    RUN_TEST(givenIambicRInModeAOutputsR);
    RUN_TEST(givenIambicKInModeAOutputsK);
    RUN_TEST(givenIambicFInModeBOutputsF);
    RUN_TEST(givenIambicLInModeBOutputsL);
    RUN_TEST(givenIambicLInModeAOutputsL);
    RUN_TEST(givenNoInputOutputsStreamOfCharacterGaps);
}