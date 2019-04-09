#ifndef __IAMBIC_KEYER_STATES_H__
#define __IAMBIC_KEYER_STATES_H__

#include "Common.h"

namespace IambicKeyer {

    enum KeyerState {
        idle = 0,
        dit = 1,
        ditRest = 2,
        dah = 3,
        dahRest = 4,
        iambicDit = 5,
        iambicDitRest = 6,
        iambicDah = 7,
        iambicDahRest = 8 
    };

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
        const virtual  void onEnter(IStateContext *context) = 0;
        const virtual  void onTimeout(IStateContext *context) = 0;
        const virtual  void onSwitchChange(IStateContext *context) = 0;
    };

    class StateDriver : public IStateContext {
        private:
            Common::IKeyHardware &m_hardware;
            Common::IMorseDecoder &m_decoder;
            Common::ISideToneHardware &m_sideTone;

            IState *m_currentState;
            bool m_timerRunning = false;
            bool m_squeezeCaptured = false;
            uint32_t m_timerStarted = 0;
            uint32_t m_timerDuration = 0;
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

            void poll(void);
            void setRate(unsigned int wpm);
            void setKeyerMode(Common::KeyerMode mode) { m_keyerMode = mode; }
    };

    class StateIdle : public IState {
        const void onEnter(IStateContext *context);
        const void onTimeout(IStateContext *context);
        const void onSwitchChange(IStateContext *context);
    };

    extern StateIdle *STATE_IDLE;

    class StateDit : public IState {
        const void onEnter(IStateContext *context);
        const void onTimeout(IStateContext *context);
        const void onSwitchChange(IStateContext *context);
    };

    extern StateDit *STATE_DIT;

    class StateDitRest : public IState {
        const void onEnter(IStateContext *context);
        const void onTimeout(IStateContext *context);
        const void onSwitchChange(IStateContext *context);
    };

    extern StateDitRest *STATE_DIT_REST;

    class StateIambicDit : public IState {
        const void onEnter(IStateContext *context);
        const void onTimeout(IStateContext *context);
        const void onSwitchChange(IStateContext *context);
    };

    extern StateIambicDit *STATE_IAMBIC_DIT;

    class StateIambicDitRest : public IState {
        const void onEnter(IStateContext *context);
        const void onTimeout(IStateContext *context);
        const void onSwitchChange(IStateContext *context);
    };

    extern StateIambicDitRest *STATE_IAMBIC_DIT_REST;

    class StateDah : public IState {
        const void onEnter(IStateContext *context);
        const void onTimeout(IStateContext *context);
        const void onSwitchChange(IStateContext *context);
    };

    extern StateDah *STATE_DAH;

    class StateDahRest : public IState {
        const void onEnter(IStateContext *context);
        const void onTimeout(IStateContext *context);
        const void onSwitchChange(IStateContext *context);
    };

    extern StateDahRest *STATE_DAH_REST;

    class StateIambicDah : public IState {
        const void onEnter(IStateContext *context);
        const void onTimeout(IStateContext *context);
        const void onSwitchChange(IStateContext *context);
    };

    extern StateIambicDah *STATE_IAMBIC_DAH;

    class StateIambicDahRest : public IState {
        const void onEnter(IStateContext *context);
        const void onTimeout(IStateContext *context);
        const void onSwitchChange(IStateContext *context);
    };

    extern StateIambicDahRest *STATE_IAMBIC_DAH_REST;
}

#endif