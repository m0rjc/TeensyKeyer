#include "Ux.h"
#include "MorseCharacters.h"

namespace Ux {

static bool
isEndOfChar(uint8_t ch)
{
    return ch == 0 || ch == 0xFF;
}

void MorseOutput::playSymbols(const uint8_t *symbols) {
    output.txOff();
    ptr.symbol = symbols;
    isPlayingAscii = false;
    if(symbols && *symbols) {
        currentChar = *symbols;
        isPlayingTone = false;
        isActive = true;
        duration = 0;
    }
}

void MorseOutput::playAscii(const char *text) {
    output.txOff();
    ptr.text = text;
    isPlayingAscii = true;
    if(text && *text) {
        currentChar = Common::asciiToMorse(*text);
        isPlayingTone = false;
        isActive = true;
        duration = 0;
    }
}


void MorseOutput::poll(Common::pollingLoopTime_t time) {
    if (!isActive) return; // Idle
    if (duration == 0 || (time - timeStarted) >= duration) {
        timeStarted = time;
        // If a tone is playing then stop the tone and play a dit gap.
        if(isPlayingTone) {
            isPlayingTone = false;
            duration = isEndOfChar(currentChar) ? 4*ditLength : ditLength;
            output.txOff();
            return;
        }
        // If we're at end of character move on to the next.
        if(isEndOfChar(currentChar)) {
            if(isPlayingAscii) {
                ptr.text++;
                currentChar = Common::asciiToMorse(*(ptr.text));
            } else {
                ptr.symbol++;
                currentChar = *ptr.symbol;
            }

            if(currentChar == MORSE_SPACE) {
                duration = 8 * ditLength;
            } else if(currentChar == 0) {
                isActive = false;
            }
        }
        // If we have an active character play the next symbol. 
        if(!isEndOfChar(currentChar)) {
            isPlayingTone = true;
            duration = currentChar & 1 ? 3*ditLength : ditLength;
            output.txOn();
            currentChar >>= 1;
            if(currentChar & 0x40) currentChar |= 0x80;// This was needed on Arduino.
        }
    }
}
}