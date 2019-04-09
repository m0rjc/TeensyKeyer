#include "IambicKeyerStates.h"

namespace IambicKeyer {
    void StateIambicDah::onEnter(IStateContext *context) {
        context->setTimeout(TIME_DAH); 
        context->getSideTone().txOn();
    }

    void StateIambicDah::onTimeout(IStateContext *context) {
        context->getSideTone().txOff();
        context->getDecoder().onDah();
        context->enterState(STATE_IAMBIC_DAH_REST);
    }

    void StateIambicDah::onSwitchChange(IStateContext *context) {
    }

    StateIambicDah *STATE_IAMBIC_DAH = new StateIambicDah();
}
