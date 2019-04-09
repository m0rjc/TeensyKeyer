#include "IambicKeyerStates.h"

namespace IambicKeyer {
    void StateDahRest::onEnter(IStateContext *context) {
        context->setTimeout(TIME_GAP); // 1 dit for a rest
    }

    void StateDahRest::onTimeout(IStateContext *context) {
        IState *nextState = STATE_IDLE;

        if(context->isSqueezeCaptured()) {
            nextState = STATE_IAMBIC_DIT;
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
                    nextState = STATE_IAMBIC_DIT;
                    break;    
            }
        }
        context->enterState(nextState);
    }

    void StateDahRest::onSwitchChange(IStateContext *context) {
    }

    StateDahRest *STATE_DAH_REST = new StateDahRest();
}
