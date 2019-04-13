#include "Teensy31Hardware.h"
#include "MorseCharacters.h"

#include <Keyboard.h>

namespace Teensy31
{

class IKeyboardAction {
    public:
    virtual void act(void) = 0;
};

class CharacterKeyboardAction : public IKeyboardAction {
    private:
    char ch;
    public:
    CharacterKeyboardAction(char ch) : ch(ch) {}
    void act(void) {
        Serial.write(ch);
    }
};

/* Table of dah ending 4 bit characters. Based at 1 */
static char DAH_ENDING_CHARACTERS[16] = {
    /* 0 */  0,
    /* -  */ 't',
    /* .- */ 'a',
    /* -- */ 'm',
    /* ..- */ 'u',
    /* -.- */ 'k',
    /* .-- */ 'w',
    /* --- */ 'o',
    /* ...- */ 'v',
    /* -..- */ 'x',
    /* .-.- */ 0,
    /* --.- */ 'q',
    /* ..-- */ 0,
    /* -.-- */ 'y',
    /* .--- */ 'j',
    /* ---- */ 0
};

/* Table of dit ending 4 bit characters. Based at 1, bit inverse */
const static char DIT_ENDING_CHARACTERS[16] = {
    /* 10000  .... */ 'h',
    /* 10001  -... */ 'b',
    /* 10010  .-.. */ 'l',
    /* 10011  --.. */ 'z',
    /* 10100  ..-. */ 'f',
    /* 10101  -.-. */ 'c',
    /* 10110  .--. */ 'p',
    /* 10111  ---. */ 0,
    /* 11000  ...  */ 's',
    /* 11001  -..  */ 'd',
    /* 11010  .-.  */ 'r',
    /* 11011  --.  */ 'g',
    /* 11100  ..   */ 'i',
    /* 11101  -.   */ 'n',
    /* 11110  .    */ 'e',
    /* 11111  xxxx */ 0 
};

UsbKeyboardOutput::UsbKeyboardOutput(void)
{
    Keyboard.begin();
}

void UsbKeyboardOutput::onSymbol(unsigned short symbol)
{
    char ch = 0;
    unsigned short topBits = symbol & 0xFFF0;
    if (topBits == 0) {
        ch = DAH_ENDING_CHARACTERS[symbol];
    } else if(topBits == 0xF0) {
        ch = DIT_ENDING_CHARACTERS[symbol & 0x0F];
    } else switch(symbol) {
        case MORSE_0: ch = '0'; break;
        case MORSE_1: ch = '1'; break;
        case MORSE_2: ch = '2'; break;
        case MORSE_3: ch = '3'; break;
        case MORSE_4: ch = '4'; break;
        case MORSE_5: ch = '5'; break;
        case MORSE_6: ch = '6'; break;
        case MORSE_7: ch = '7'; break;
        case MORSE_8: ch = '8'; break;
        case MORSE_9: ch = '9'; break;
        case MORSE_BACKSPACE:
            Keyboard.press(KEY_BACKSPACE);
            Keyboard.release(KEY_BACKSPACE);
            break;
        case MORSE_SPACE:
            ch = ' '; break;
    }
    if(ch) Keyboard.write(ch);
}
} // namespace Teensy31