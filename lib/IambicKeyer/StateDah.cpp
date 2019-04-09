#include "IambicKeyerStates.h"

namespace IambicKeyer {
    void StateDah::onEnter(IStateContext *context) {
        context->setTimeout(TIME_DAH);
        context->getSideTone().txOn();
        context->clearSqueezeCapture();
    }

    void StateDah::onTimeout(IStateContext *context) {
        context->getSideTone().txOff();
        context->getDecoder().onDah();
        context->enterState(STATE_DAH_REST);
    }

    void StateDah::onSwitchChange(IStateContext *context) {
    }

    StateDah *STATE_DAH = new StateDah();
}
