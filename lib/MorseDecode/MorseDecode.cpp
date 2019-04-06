#include "MorseDecode.h"
#include "MorseCharacters.h"

const unsigned short GAPS_FOR_SPACE=4;

namespace MorseDecode {
    static symbolCallback m_callback;
    static bool m_lastIsDah = false;
    static bool m_isBackspacing = false;
    static bool m_isIdle = true;
    static unsigned char m_counter = 0;
    static unsigned short m_currentSymbol;
    static unsigned char m_numberOfSpaces = 0;

    static void startCharacter()
    {
        m_currentSymbol = 0;
        m_counter = 0;
        m_lastIsDah = false;
    }

    static inline void onNoLongerIdle()
    {
        m_isIdle = false;
        m_numberOfSpaces = 0;
    }

    static void onLongSymbol()
    {
        if (m_currentSymbol == 0)
        { // ........
            if(m_callback) m_callback(MORSE_BACKSPACE);
            m_isBackspacing = true;
        }
        startCharacter();
    }

    void init(symbolCallback callback) {
        m_callback = callback;
        m_numberOfSpaces = 0;
        m_isIdle = true;
        startCharacter();
    }

    void onDit() {
        onNoLongerIdle();
        m_lastIsDah = false;
        m_counter++;
        if(m_counter > 7)
        {
            onLongSymbol();
        }
    }

    void onDah()
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

    void onCharacterGap()
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
                if(m_callback) m_callback(m_currentSymbol);
                startCharacter();
            } else if (m_numberOfSpaces == GAPS_FOR_SPACE) {
                if(m_callback) m_callback(MORSE_SPACE);
                m_isIdle = true;            
            }
        }
    }
}

