#include <Arduino.h>
#include "IambicKeyerStates.h"

namespace IambicKeyer {
    const void StateIambicDitRest::onEnter(IStateContext *context) {
        context->setTimeout(100); // 1 dit for a rest
    }

    const void StateIdle::onTimeout(IStateContext *context) {
        IState *nextState = STATE_IDLE;

        switch(context->readSwitches()) {
            case Common::KeyInput::none :
                nextState = context->getKeyerMode() == Common::KeyerMode::modeA ?
                    (IState *) STATE_IDLE :
                    (IState *) STATE_DAH;
                break;
            case Common::KeyInput::dit :
                nextState = STATE_DIT;
                break;
            case Common::KeyInput::dah :
                nextState = STATE_DAH;
                break;
            case Common::KeyInput::squeeze :
                nextState = STATE_IAMBIC_DAH;
                break;    
        }
        context->enterState(nextState);
    }

    const void StateIdle::onSwitchChange(IStateContext *context) {
    }

    StateIambicDitRest  *STATE_IAMBIC_DIT_REST = new StateIambicDitRest();
}
