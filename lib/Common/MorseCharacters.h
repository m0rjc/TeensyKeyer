#ifndef __MORSE_CHARACTERS_H__
#define __MORSE_CHARACTERS_H__

/**
 * This file defines the character codes generated by the MorseDecode module.
 * These codes are the same consumed by the original Morse Blink code.
 * The code is read from least significant bit towards most significant bit.
 * A 0 represents a Dit, a 1 represents Dah.
 * The characer ends when all remaining bits are the same.
 * 
 * All of the normal symbols fit within one unsigned byte, however a short is
 * used to allow special symbols to be sent. These all have bit 9 set to indicate
 * the special nature.
 */

#define MORSE_A B00000010
#define MORSE_B B11110001
#define MORSE_C B11110101
#define MORSE_D B11111001
#define MORSE_E B11111110
#define MORSE_F B11110100
#define MORSE_G B11111011
#define MORSE_H B11110000
#define MORSE_I B11111100
#define MORSE_J B00001110
#define MORSE_K B00000101
#define MORSE_L B11110010
#define MORSE_M B00000011
#define MORSE_N B11111101
#define MORSE_O B00000111
#define MORSE_P B11110110
#define MORSE_Q B00001011
#define MORSE_R B11111010
#define MORSE_S B11111000
#define MORSE_T B00000001
#define MORSE_U B00000100
#define MORSE_V B00001000
#define MORSE_W B00000110
#define MORSE_X B00001001
#define MORSE_Y B00001101
#define MORSE_Z B11110011

#define MORSE_0 B00011111
#define MORSE_1 B00011110
#define MORSE_2 B00011100
#define MORSE_3 B00011000
#define MORSE_4 B00010000
#define MORSE_5 B11100000
#define MORSE_6 B11100001
#define MORSE_7 B11100011
#define MORSE_8 B11100111
#define MORSE_9 B11101111

#define MORSE_SLASH B11010010
#define MORSE_QUESTION_MARK B11001100
#define MORSE_PERIOD B00101010
#define MORSE_COMMA B00110011
#define MORSE_EXCLAMATION_MARK   B00110101
#define MORSE_QUOTE_MARK B11010010
#define MORSE_AMPERSAND B11100010
#define MORSE_APOSTROPHE B11011110
#define MORSE_OPEN_PAREN B11101101
#define MORSE_CLOSE_PAREN B00101101
#define MORSE_PLUS B11101010
#define MORSE_HYPHEN B00100001
#define MORSE_COLON B11000111
#define MORSE_EQUALS B00010001
#define MORSE_AT_SIGN B11010110

#define MORSE_PROSIGN_AA B00001010

// This is within 8 bits so can be included in messages.
#define MORSE_SPACE 0xFF

#define MORSE_SPECIAL 0x100

// Backspace is all dits, normally an error.
#define MORSE_BACKSPACE (MORSE_SPECIAL | 0x00)

#endif