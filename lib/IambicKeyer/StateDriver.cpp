#include "IambicKeyer.h"
#include "IambicKeyerStates.h"

using Common::pollingLoopTime_t;

namespace IambicKeyer {
    void StateDriver::init(IState *initialState) {
        setRate(20);
        setKeyerMode(Common::KeyerMode::modeB);
        m_currentState = initialState;
    }

    void StateDriver::enterState(IState *state) {
        m_currentState = state;
        m_currentState->onEnter(this);
    }

    void StateDriver::poll(pollingLoopTime_t timeNow)
    {
        m_timeNow = timeNow;

        if(!m_started) {
            enterState(m_currentState);
            m_started = true;
        }

        Common::KeyInput lastSwitchState = m_switchState;
        m_switchState = m_hardware.readSwitches();
        if(m_switchState == Common::KeyInput::squeeze) m_squeezeCaptured = true;

        if(m_timerRunning) {
            pollingLoopTime_t time = timeNow - m_timerStarted;
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
        m_ditInterval = Common::getDitLength(wpm);
    }

    void StateDriver::setTimeout(unsigned int centiDitPeriods) {
        m_timerRunning = true;
        m_timerDuration = (pollingLoopTime_t)centiDitPeriods * (pollingLoopTime_t)m_ditInterval / 100;
        m_timerStarted = m_timeNow;
    }


}