#include "StriaghtKeyer.h"

namespace StraightKeyer {
    void Keyer::poll(Common::pollingLoopTime_t time) {
        Status newState = m_hardware.isOn() ? Status::on : Status::off;

        if(newState != m_status) {
            if(newState == Status::on) {
                Serial.printf("[%ld|OFF]\n", time - m_eventStart);
                m_sideTone.txOn();
            } else {
                m_sideTone.txOff();
                discriminateTone(time - m_eventStart);
            }
            m_status = newState;
            m_eventStart = time;
        } else if (m_status == Status::off && (time - m_eventStart) >= m_characterGapThreshold) {
            m_decoder.onCharacterGap();
            m_eventStart = time;
        }
    }

    void Keyer::discriminateTone(Common::pollingLoopTime_t interval) {
        if(m_ditDahBoundary > 0) {
            Serial.printf("[%ld|%ld]\n", interval, m_ditDahBoundary); 
            if(interval > m_ditDahBoundary) {
                m_decoder.onDah();
            } else {
                m_decoder.onDit();
            }
        }
    }

    void Keyer::setRate(uint8_t wpm) {
        // A dit takes 1 dit-length of tone.
        // A dah takes 3 dit-length of tone.
        // split in the middle at 2.
        Common::pollingLoopTime_t ditLength = Common::getDitLength(wpm);
        m_ditDahBoundary = 2 * ditLength;
        m_characterGapThreshold = 3 * ditLength;
    }
}
