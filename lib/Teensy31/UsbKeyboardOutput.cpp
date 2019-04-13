#include "Teensy31Hardware.h"

#include <Keyboard.h>

namespace Teensy31
{

UsbKeyboardOutput::UsbKeyboardOutput(void)
{
//    Keyboard.begin();
}

void UsbKeyboardOutput::onSymbol(unsigned short symbol)
{
}
} // namespace Teensy31