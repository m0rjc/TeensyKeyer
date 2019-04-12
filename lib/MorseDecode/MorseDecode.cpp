#include "MorseDecode.h"
#include "MorseCharacters.h"

// See https://www.eham.net/ehamforum/smf/index.php?topic=8534.0;wap2
// But note that the keyer sees a character gap as 3 dits, so this gives
// 3*3=9. A little more tolerant for me.
const unsigned short GAPS_FOR_SPACE=3;

namespace MorseDecode {
    void MorseDecoder::startCharacter(void)
    {
        m_currentSymbol = 0;
        m_counter = 0;
        m_lastIsDah = false;
    }

    void MorseDecoder::onNoLongerIdle(void)
    {
        m_isIdle = false;
        m_numberOfSpaces = 0;
    }

    void MorseDecoder::onLongSymbol(void)
    {
        if (m_currentSymbol == 0)
        { // ........
            m_callback.onSymbol(MORSE_BACKSPACE);
            m_isBackspacing = true;
        }
        startCharacter();
    }

    void MorseDecoder::onDit() {
        onNoLongerIdle();
        m_lastIsDah = false;
        m_counter++;
        if(m_counter > 7)
        {
            onLongSymbol();
        }
    }

    void MorseDecoder::onDah()
    {
        onNoLongerIdle();
        m_isBackspacing = false;
        m_lastIsDah = true;
        m_currentSymbol |= (1 << m_counter);
        m_counter++;
        if (m_counter > 7)
        {
            onLongSymbol();
        }
    }

    void MorseDecoder::onCharacterGap()
    {
        // Discard spurious dits after the user leans on dit for backspace
        if(m_isBackspacing) {
            startCharacter();
            m_isBackspacing = false;
            m_isIdle = true;
        }

        if(!m_isIdle) {
            m_numberOfSpaces++;

            // If it's the first space after receiving some data then output it.
            if (m_numberOfSpaces == 1)
            {
                if (!m_lastIsDah)
                {
                    for (; (m_counter & 0x07); m_counter++)
                    {
                        m_currentSymbol |= (1 << m_counter);
                    }
                }
                m_callback.onSymbol(m_currentSymbol);
                startCharacter();
            } else if (m_numberOfSpaces == GAPS_FOR_SPACE) {
                m_callback.onSymbol(MORSE_SPACE);
                m_isIdle = true;            
            }
        }
    }
}

