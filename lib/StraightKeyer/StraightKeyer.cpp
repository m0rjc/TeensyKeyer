#include "StriaghtKeyer.h"

namespace StraightKeyer {
    void Keyer::poll(Common::pollingLoopTime_t time) {
        Status newState = m_hardware.isOn() ? Status::on : Status::off;
        if(newState != m_status) {
            if(newState == Status::on) {
                m_sideTone.txOn();
            } else {
                m_sideTone.txOff();
            }
            m_status = newState;
        }
    }
}
