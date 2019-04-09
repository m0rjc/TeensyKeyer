#include <Arduino.h>
#include "IambicKeyerStates.h"

namespace IambicKeyer {
    const void StateDitRest::onEnter(IStateContext *context) {
        context->setTimeout(100); // 1 dit for a rest
    }

    const void StateDitRest::onTimeout(IStateContext *context) {
        IState *nextState = STATE_IDLE;

        if(context->isSqueezeCaptured()) {
            nextState = STATE_IAMBIC_DAH;
        } else {
            switch(context->readSwitches()) {
                case Common::KeyInput::none :
                    nextState = STATE_IDLE;
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
        }
        context->enterState(nextState);
    }

    const void StateDitRest::onSwitchChange(IStateContext *context) {
    }

    StateDitRest *STATE_DIT_REST = new StateDitRest();
}
