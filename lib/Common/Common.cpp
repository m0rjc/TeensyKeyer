#include <Arduino.h>
#include "Common.h"

namespace Common
{
SideToneRouter::SideToneRouter(ISideToneHardware &output)
{
    inputs = 0;
    state = 0;
    allocatedBits = 0;
    outputs = new ISideToneHardware *[2];
    outputs[0] = &output;
    outputs[1] = NULL;
}

SideToneRouter::SideToneRouter(ISideToneHardware &output1, ISideToneHardware &output2)
{
    inputs = 0;
    state = 0;
    allocatedBits = 0;
    outputs = new ISideToneHardware *[3];
    outputs[0] = &output1;
    outputs[1] = &output2;
    outputs[2] = NULL;
}

SideToneRouter::SideToneRouter(ISideToneHardware &output1, ISideToneHardware &output2, ISideToneHardware &output3)
{
    inputs = 0;
    state = 0;
    allocatedBits = 0;
    outputs = new ISideToneHardware *[4];
    outputs[0] = &output1;
    outputs[1] = &output2;
    outputs[2] = &output3;
    outputs[3] = NULL;
}

SideToneRouter::~SideToneRouter()
{
    delete outputs;
}

unsigned char SideToneRouter::allocateBitMask()
{
    unsigned char bitMask = 1;
    while (allocatedBits & bitMask && !(bitMask & 0x80))
        bitMask <<= 1;
    allocatedBits |= bitMask;
    return bitMask;
}

void SideToneRouter::freeBitMask(unsigned char mask)
{
    unsigned char inverse = ~mask;
    allocatedBits &= inverse;
    inputs &= inverse;
    onChange();
}

void SideToneRouter::onChange()
{
    bool newState = inputs != 0;
    bool oldState = state;
    if (newState != oldState)
    {
        state = newState;
        ISideToneHardware **ptr = outputs;
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