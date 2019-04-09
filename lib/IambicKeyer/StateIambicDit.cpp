#include "IambicKeyerStates.h"

namespace IambicKeyer {
    void StateIambicDit::onEnter(IStateContext *context) {
        context->setTimeout(TIME_DIT); 
        context->getSideTone().txOn();
    }

    void StateIambicDit::onTimeout(IStateContext *context) {
        context->getSideTone().txOff();
        context->getDecoder().onDit();
        context->enterState(STATE_IAMBIC_DIT_REST);
    }

    void StateIambicDit::onSwitchChange(IStateContext *context) {
    }

    StateIambicDit *STATE_IAMBIC_DIT = new StateIambicDit();
}
