#include "Teensy31Hardware.h"

namespace Teensy31
{
    void SwitchInputPin::poll(Common::pollingLoopTime_t time)
    {
        if(time > lastTime)
        {
            uint16_t reading = digitalRead(pin) ? 1 : 0;
            uint16_t checkBit;
            int count = 0;
            readings = (readings << 1 | reading) & 0x03FF; // 10 bits, 10 milliseconds

            for(checkBit = 1; checkBit < 0x0400; checkBit <<= 1)
            {
                if(readings & checkBit) count++;
            } 

            // Implement hysteris, like a Schmidt Trigger.
            if(lastRead == 0 && count == 10) lastRead = 1;
            else if(lastRead == 1 && count == 0) lastRead = 0;

            lastTime = time;
        }
    }

    bool SwitchInputPin::isOn(void)
    {
        return lastRead == 0;
    }
}