#include <Arduino.h>
#include "Common.h"

namespace Common
{
OutputRouter::OutputRouter(IPinOutput &output)
{
    inputs = 0;
    state = 0;
    allocatedBits = 0;
    outputs = new IPinOutput *[2];
    outputs[0] = &output;
    outputs[1] = NULL;
}

OutputRouter::OutputRouter(IPinOutput &output1, IPinOutput &output2)
{
    inputs = 0;
    state = 0;
    allocatedBits = 0;
    outputs = new IPinOutput *[3];
    outputs[0] = &output1;
    outputs[1] = &output2;
    outputs[2] = NULL;
}

OutputRouter::OutputRouter(IPinOutput &output1, IPinOutput &output2, IPinOutput &output3)
{
    inputs = 0;
    state = 0;
    allocatedBits = 0;
    outputs = new IPinOutput *[4];
    outputs[0] = &output1;
    outputs[1] = &output2;
    outputs[2] = &output3;
    outputs[3] = NULL;
}

OutputRouter::~OutputRouter()
{
    delete outputs;
}

unsigned char OutputRouter::allocateBitMask()
{
    unsigned char bitMask = 1;
    while (allocatedBits & bitMask && !(bitMask & 0x80))
        bitMask <<= 1;
    allocatedBits |= bitMask;
    return bitMask;
}

void OutputRouter::freeBitMask(unsigned char mask)
{
    unsigned char inverse = ~mask;
    allocatedBits &= inverse;
    inputs &= inverse;
    onChange();
}

void OutputRouter::onChange()
{
    bool newState = inputs != 0;
    bool oldState = state;
    if (newState != oldState)
    {
        state = newState;
        IPinOutput **ptr = outputs;
        while (*ptr)
        {
            if (state)
            {
                (*ptr)->txOn();
            }
            else
            {
                (*ptr)->txOff();
            }
            ptr++;
        }
    }
}
} // namespace Common