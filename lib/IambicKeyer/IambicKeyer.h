#ifndef __IAMBIC_KEYER_H__
#define __IAMBIC_KEYER_H__

#include <Arduino.h>
#include "Common.h"
#include "IambicKeyerStates.h"

namespace IambicKeyer {

    class Keyer {
        private:
            StateDriver m_stateDriver;

        public:
            Keyer(Common::IKeyHardware &hardware,
                  Common::IMorseDecoder &decoder, 
                  Common::IPinOutput &sideTone) : 
                  m_stateDriver(hardware, decoder, sideTone, STATE_IDLE) {}

            void setRate(unsigned int wpm) {
                m_stateDriver.setRate(wpm);
            }

            void setMode(Common::KeyerMode mode) {
                m_stateDriver.setKeyerMode(mode);
            }

            void poll(Common::pollingLoopTime_t tick) {
                m_stateDriver.poll(tick);
            }
    };
}

#endif
