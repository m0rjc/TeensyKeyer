
#include "IambicKeyer.h"
#include "IambicKeyerStates.h"

namespace IambicKeyer {

    void Keyer::poll(void) {
        m_stateDriver.poll();
    }

    void Keyer::setMode(Common::KeyerMode mode) {
        m_stateDriver.setKeyerMode(mode);
    }

    void Keyer::setRate(unsigned int rateWpm) {
        m_stateDriver.setRate(rateWpm);
    }
}

