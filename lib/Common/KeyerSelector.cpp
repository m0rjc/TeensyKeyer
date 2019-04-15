#include "Common.h"

namespace Common
{

void KeyerSelectorInput::onDit(void)
{
    selector.activeInput = myNumber;
    selector.decoder.onDit();
}

void KeyerSelectorInput::onDah(void)
{
    selector.activeInput = myNumber;
    selector.decoder.onDah();
}

void KeyerSelectorInput::onCharacterGap(void)
{
    if (selector.activeInput == myNumber)
    {
        selector.decoder.onCharacterGap();
    }
}

} // namespace Common