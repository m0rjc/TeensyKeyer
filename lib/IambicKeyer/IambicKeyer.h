#ifndef __IAMBIC_KEYER_H__
#define __IAMBIC_KEYER_H__

#include "Common.h"
#include "IambicKeyerStates.h"

namespace IambicKeyer {

    class Keyer {
        private:
            StateDriver m_stateDriver;

        public:
            Keyer(Common::IKeyHardware &hardware,
                  Common::IMorseDecoder &decoder, 
                  Common::ISideToneHardware &sideTone) : 
                  m_stateDriver(hardware, decoder, sideTone, STATE_IDLE) {}

            void setRate(unsigned int wpm);

            void setMode(Common::KeyerMode mode);

            void poll(void);
    };
}

#endif
