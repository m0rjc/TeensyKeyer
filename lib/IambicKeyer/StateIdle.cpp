#include <Arduino.h>
#include "IambicKeyerStates.h"

namespace IambicKeyer {
    const void StateIdle::onEnter(IStateContext *context) {
        context->setTimeout(TIME_CHARACTER_GAP); // 3 dits for character space
    }

    const void StateIdle::onTimeout(IStateContext *context) {
        context->getDecoder().onCharacterGap();
        context->setTimeout(TIME_CHARACTER_GAP); // 3 dits for character space
    }

    const void StateIdle::onSwitchChange(IStateContext *context) {
        switch(context->readSwitches()) {
            case Common::KeyInput::none:
                break;
            case Common::KeyInput::dit:
                context->enterState(STATE_DIT);
                break;
            case Common::KeyInput::squeeze:
                context->enterState(STATE_IAMBIC_DIT);
                break;
            case Common::KeyInput::dah:
                context->enterState(STATE_DAH);
                break;
        }
    }

    StateIdle *STATE_IDLE = new StateIdle();
}

