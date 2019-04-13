#ifndef __MORSE_DECODE_H__
#define __MORSE_DECODE_H__

#include "Common.h"

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
namespace MorseDecode
{
class MorseDecoder : public Common::IMorseDecoder
{
  private:
    Common::ISymbolCallback &m_callback;
    bool m_lastIsDah = false;
    bool m_isBackspacing = false;
    bool m_isIdle = true;
    unsigned char m_counter = 0;
    unsigned short m_currentSymbol;
    unsigned char m_numberOfSpaces = 0;

    void startCharacter(void);
    void onNoLongerIdle(void);
    void onLongSymbol(void);


  public:
    MorseDecoder(Common::ISymbolCallback &callback) : m_callback(callback) { startCharacter(); }

    void onDit(void);
    void onDah(void);
    void onCharacterGap();
};
}; // namespace MorseDecode

#endif