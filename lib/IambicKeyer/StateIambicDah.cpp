#include <Arduino.h>
#include "IambicKeyerStates.h"

namespace IambicKeyer {
    const void StateIambicDah::onEnter(IStateContext *context) {
        context->setTimeout(TIME_DAH); 
        context->getSideTone().txOn();
    }

    const void StateIambicDah::onTimeout(IStateContext *context) {
        context->getSideTone().txOff();
        context->getDecoder().onDit();
        context->enterState(STATE_IAMBIC_DAH_REST);
    }

    const void StateIambicDah::onSwitchChange(IStateContext *context) {
    }

    StateIambicDah *STATE_IAMBIC_DAH = new StateIambicDah();
}
