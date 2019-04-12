#ifndef __IAMBIC_KEYER_STATES_H__
#define __IAMBIC_KEYER_STATES_H__

#include "Common.h"

using Common::pollingLoopTime_t;

namespace IambicKeyer {

    const unsigned int TIME_DIT = 100;
    const unsigned int TIME_DAH = 300;
    const unsigned int TIME_GAP = 100;
    const unsigned int TIME_CHARACTER_GAP = 300;

    class IState;

    class IStateContext {
        public:
        virtual void setTimeout(unsigned int centiDitPeriods) = 0;
        virtual Common::KeyInput readSwitches(void) = 0;
        virtual bool isSqueezeCaptured(void) = 0;
        virtual void clearSqueezeCapture(void) = 0;
        virtual void enterState(IState *state) = 0;
        virtual Common::KeyerMode getKeyerMode(void) = 0;
        virtual Common::IMorseDecoder &getDecoder(void) = 0;
        virtual Common::ISideToneHardware &getSideTone(void) = 0;
    };

    class IState {
        public:
        virtual void onEnter(IStateContext *context) = 0;
        virtual void onTimeout(IStateContext *context) = 0;
        virtual void onSwitchChange(IStateContext *context) = 0;
    };

    class StateDriver : public IStateContext {
        private:
            Common::IKeyHardware &m_hardware;
            Common::IMorseDecoder &m_decoder;
            Common::ISideToneHardware &m_sideTone;

            IState *m_currentState;
            bool m_started = false;
            bool m_timerRunning = false;
            bool m_squeezeCaptured = false;
            pollingLoopTime_t m_timeNow;
            pollingLoopTime_t m_timerStarted = 0;
            pollingLoopTime_t m_timerDuration = 0;
            int m_ditInterval;
            Common::KeyInput m_switchState = Common::KeyInput::none;
            Common::KeyerMode m_keyerMode;

            void init(IState *initialState);
       
        public:
            StateDriver(Common::IKeyHardware &hardware,
                        Common::IMorseDecoder &decoder, 
                        Common::ISideToneHardware &sideTone,
                        IState *initialState) :
                    m_hardware(hardware),
                    m_decoder(decoder),
                    m_sideTone(sideTone)
                    { this->init(initialState); };

            void setTimeout(unsigned int centiDitPeriods);
            Common::KeyInput readSwitches(void) { return m_switchState; }
            bool isSqueezeCaptured(void) { return m_squeezeCaptured; }
            void clearSqueezeCapture(void) { m_squeezeCaptured = false; }
            void enterState(IState *state);
            Common::KeyerMode getKeyerMode(void) { return m_keyerMode; }
            Common::IMorseDecoder &getDecoder(void) { return m_decoder; }
            Common::ISideToneHardware &getSideTone(void) { return m_sideTone; }

            void poll(pollingLoopTime_t tick);
            void setRate(unsigned int wpm);
            void setKeyerMode(Common::KeyerMode mode) { m_keyerMode = mode; }
    };

    class StateIdle : public IState {
        void onEnter(IStateContext *context);
        void onTimeout(IStateContext *context);
        void onSwitchChange(IStateContext *context);
    };

    extern StateIdle *STATE_IDLE;

    class StateDit : public IState {
        void onEnter(IStateContext *context);
        void onTimeout(IStateContext *context);
        void onSwitchChange(IStateContext *context);
    };

    extern StateDit *STATE_DIT;

    class StateDitRest : public IState {
        void onEnter(IStateContext *context);
        void onTimeout(IStateContext *context);
        void onSwitchChange(IStateContext *context);
    };

    extern StateDitRest *STATE_DIT_REST;

    class StateIambicDit : public IState {
        void onEnter(IStateContext *context);
        void onTimeout(IStateContext *context);
        void onSwitchChange(IStateContext *context);
    };

    extern StateIambicDit *STATE_IAMBIC_DIT;

    class StateIambicDitRest : public IState {
        void onEnter(IStateContext *context);
        void onTimeout(IStateContext *context);
        void onSwitchChange(IStateContext *context);
    };

    extern StateIambicDitRest *STATE_IAMBIC_DIT_REST;

    class StateDah : public IState {
        void onEnter(IStateContext *context);
        void onTimeout(IStateContext *context);
        void onSwitchChange(IStateContext *context);
    };

    extern StateDah *STATE_DAH;

    class StateDahRest : public IState {
        void onEnter(IStateContext *context);
        void onTimeout(IStateContext *context);
        void onSwitchChange(IStateContext *context);
    };

    extern StateDahRest *STATE_DAH_REST;

    class StateIambicDah : public IState {
        void onEnter(IStateContext *context);
        void onTimeout(IStateContext *context);
        void onSwitchChange(IStateContext *context);
    };

    extern StateIambicDah *STATE_IAMBIC_DAH;

    class StateIambicDahRest : public IState {
        void onEnter(IStateContext *context);
        void onTimeout(IStateContext *context);
        void onSwitchChange(IStateContext *context);
    };

    extern StateIambicDahRest *STATE_IAMBIC_DAH_REST;
}

#endif