#include <Arduino.h>
#include "IambicKeyerStates.h"

namespace IambicKeyer {
    const void StateDah::onEnter(IStateContext *context) {
        context->setTimeout(TIME_DAH);
        context->getSideTone().txOn();
        context->clearSqueezeCapture();
    }

    const void StateDah::onTimeout(IStateContext *context) {
        context->getSideTone().txOff();
        context->getDecoder().onDah();
        context->enterState(STATE_DAH_REST);
    }

    const void StateDah::onSwitchChange(IStateContext *context) {
    }

    StateDah *STATE_DAH = new StateDah();
}
