#include <Arduino.h>
#include "IambicKeyerStates.h"

namespace IambicKeyer {
    const void StateIambicDit::onEnter(IStateContext *context) {
        context->setTimeout(100); // 1 dit for a dit
        context->getSideTone().txOn();
    }

    const void StateIambicDit::onTimeout(IStateContext *context) {
        context->getSideTone().txOff();
        context->getDecoder().onDit();
        context->enterState(STATE_IAMBIC_DIT_REST);
    }

    const void StateIambicDit::onSwitchChange(IStateContext *context) {
    }

    StateIambicDit *STATE_IAMBIC_DIT = new StateIambicDit();
}
