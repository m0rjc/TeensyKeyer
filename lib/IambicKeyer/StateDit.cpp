#include <Arduino.h>
#include "IambicKeyerStates.h"

namespace IambicKeyer {
    const void StateDit::onEnter(IStateContext *context) {
        context->setTimeout(TIME_DIT);
        context->getSideTone().txOn();
        context->clearSqueezeCapture();
    }

    const void StateDit::onTimeout(IStateContext *context) {
        context->getSideTone().txOff();
        context->getDecoder().onDit();
        context->enterState(STATE_DIT_REST);
    }

    const void StateDit::onSwitchChange(IStateContext *context) {
    }

    StateDit *STATE_DIT = new StateDit();
}
