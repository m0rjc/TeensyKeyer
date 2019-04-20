#include "Common.h"
#include "MorseCharacters.h"

namespace Common {

const uint8_t ASCII_TO_SYMBOLS[59] = {
    /* 32 - space */
    MORSE_SPACE,
    MORSE_EXCLAMATION_MARK,
    MORSE_QUOTE_MARK,
    MORSE_SPACE, // Hash not known
    MORSE_SPACE, // dollar - not known
    MORSE_SPACE, // Percent
    MORSE_AMPERSAND,
    MORSE_APOSTROPHE,
    MORSE_OPEN_PAREN,
    MORSE_CLOSE_PAREN,
    MORSE_SPACE, // Asterix
    MORSE_PLUS,
    MORSE_COMMA,
    MORSE_HYPHEN,
    MORSE_PERIOD,
    MORSE_SLASH,
    MORSE_0,
    MORSE_1,
    MORSE_2,
    MORSE_3,
    MORSE_4,
    MORSE_5,
    MORSE_6,
    MORSE_7,
    MORSE_8,
    MORSE_9,
    MORSE_COLON,
    MORSE_SPACE, // Semicolon
    MORSE_SPACE, // <
    MORSE_EQUALS,
    MORSE_SPACE, // >
    MORSE_QUESTION_MARK,
    MORSE_AT_SIGN,
    MORSE_A,
    MORSE_B,
    MORSE_C,
    MORSE_D,
    MORSE_E,
    MORSE_F,
    MORSE_G,
    MORSE_H,
    MORSE_I,
    MORSE_J,
    MORSE_K,
    MORSE_L,
    MORSE_M,
    MORSE_N,
    MORSE_O,
    MORSE_P,
    MORSE_Q,
    MORSE_R,
    MORSE_S,
    MORSE_T,
    MORSE_U,
    MORSE_V,
    MORSE_W,
    MORSE_X,
    MORSE_Y,
    MORSE_Z};

uint8_t asciiToMorse(char ch) {
    if(ch == 0) return 0;
    if(ch < 32) return MORSE_SPACE;
    if(ch > 96) ch -= ('a' - 'A');
    if(ch > (58 + 32)) return MORSE_SPACE;
    return ASCII_TO_SYMBOLS[ch - 32];
}

}
