#include "IambicKeyer.h"
#include "IambicKeyerStates.h"

namespace IambicKeyer {

    const unsigned int MIN_RATE = 5;
    const unsigned int MAX_RATE = 50;

    void StateDriver::init(IState *initialState) {
        setRate(20);
        setKeyerMode(Common::KeyerMode::modeB);
        enterState(initialState);
    }

    void StateDriver::enterState(IState *state) {
        m_currentState = state;
        m_currentState->onEnter(this);
    }

    void StateDriver::poll(void) {
        Common::KeyInput lastSwitchState = m_switchState;
        m_switchState = m_hardware.readSwitches();
        if(m_switchState == Common::KeyInput::squeeze) m_squeezeCaptured = true;

        if(m_timerRunning) {
            uint32_t time = m_hardware.getTimeMillis() - m_timerStarted;
            if(time >= m_timerDuration) {
                m_timerRunning = false;
                m_currentState->onTimeout(this);
            }            
        }

        if(m_switchState != lastSwitchState) {
            m_currentState->onSwitchChange(this);
        }
    }

    void StateDriver::setRate(unsigned int wpm) {
        if(wpm < MIN_RATE) wpm = MIN_RATE;
        if(wpm > MAX_RATE) wpm = MAX_RATE;
        m_ditInterval = 1200/wpm;
    }

    void StateDriver::setTimeout(unsigned int centiDitPeriods) {
        m_timerRunning = true;
        m_timerDuration = (uint32_t)centiDitPeriods * (uint32_t)m_ditInterval / 100;
        m_timerStarted = m_hardware.getTimeMillis();
    }


}