#include "IambicKeyerStates.h"

namespace IambicKeyer {
    const void StateIambicDit::onEnter(IStateContext *context) {
        context->setTimeout(TIME_DIT); 
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
