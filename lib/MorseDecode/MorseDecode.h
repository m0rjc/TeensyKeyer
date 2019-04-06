#ifndef __MORSE_DECODE_H__
#define __MORSE_DECODE_H__

/**
 * Provide a state machine to identify individual Morse symbols.
 * A Morse symbol lasts until an inter-character gap is read.
 * 
 * Once enough inter-character gaps are read a special SPACE symbol will be
 * generated. This will only be generated once until new non-space data
 * is received.
 * 
 * A convention in Morse Code is to indicate error by sending a stream of dits,
 * perhaps by holding down the dit paddle on an electronic keyer. This code will
 * generate the BACKSPACE special symbol for each 8 dits it receives and keep doing
 * so until DIT is released. Any dits in the stream not a multiple of 8 will be
 * discarded to prevent spurious E,I,S,H,5 being generated.
 */
namespace MorseDecode {
    typedef void (*symbolCallback)(unsigned short);

    /**
     * Initialise.
     */
    extern void init(symbolCallback callback);

    /**
     * The keyer has received a dit.
     */
    extern void onDit();

    /**
     * The keyer has received a dah.
     */
    extern void onDah();

    /**
     * The keyer has been idle long enough to terminate a character.
     * This will be called repeatedly if the user steps away from the keyer.
     */
    extern void onCharacterGap();
};

#endif