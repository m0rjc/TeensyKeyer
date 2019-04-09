#include "IambicKeyerStates.h"

namespace IambicKeyer {
    const void StateIambicDahRest::onEnter(IStateContext *context) {
        context->setTimeout(TIME_GAP); // 1 dit for a rest
    }

    const void StateIambicDahRest::onTimeout(IStateContext *context) {
        IState *nextState = STATE_IDLE;

        switch(context->readSwitches()) {
            case Common::KeyInput::none :
                nextState = context->getKeyerMode() == Common::KeyerMode::modeA ?
                    (IState *) STATE_IDLE :
                    (IState *) STATE_DIT;
                break;
            case Common::KeyInput::dit :
                nextState = STATE_DIT;
                break;
            case Common::KeyInput::dah :
                nextState = STATE_DAH;
                break;
            case Common::KeyInput::squeeze :
                nextState = STATE_IAMBIC_DIT;
                break;    
        }
        context->enterState(nextState);
    }

    const void StateIambicDitRest::onSwitchChange(IStateContext *context) {
    }

    StateIambicDahRest  *STATE_IAMBIC_DAH_REST = new StateIambicDahRest();
}
