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

static char CHARACTERS[256] = {
    /* 0  */ 0,
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
    /* ---- */ 0,
    /* ....- */ '4',
    /* -...- */ '=',
    /* .-..- */ 0,
    /* --..- */ 0,
    /* ..-.- */ 0,
    /* -.-.- */ 0,
    /* .--.- */ 0,
    /* ---.- */ 0,
    /* ...-- */ '3',
    /* -..-- */ 0,
    /* .-.-- */ 0,
    /* --.-- */ 0,
    /* ..--- */ '2',
    /* -.--- */ 0,
    /* .---- */ '1',
    /* ----- */ '0',
    /* .....- */ 0
};

UsbKeyboardOutput::UsbKeyboardOutput(void)
{
//    Keyboard.begin();
}

void UsbKeyboardOutput::onSymbol(unsigned short symbol)
{
    if(symbol < 32)
    {
//        IKeyboardAction *action = ACTIONS[symbol]; 
//        if(action) action->act();
        char ch = CHARACTERS[symbol];
        if(ch) Serial.write(ch);
    }
    else if(symbol == MORSE_BACKSPACE) {
        Serial.printf("<BS>");
    } else if (symbol == MORSE_SPACE) {
        Serial.write(' ');
    }
//    Serial.printf("Symbol %04x\n", (int)symbol);
}
} // namespace Teensy31