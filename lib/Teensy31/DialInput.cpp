#include "Teensy31Hardware.h"

namespace Teensy31
{
/** 
 * We cannot read too often, and don't need to respond too quickly to
 * changes. Low pass filtering added based on arduino documentation.
 * This gives a latency of 32ms which is still fast enough for the slow
 * changing user input.
 */
const Common::pollingLoopTime_t POLLING_INTERVAL = 4;
const int ANALOG_MAX = 1024; // From Arduino documentation.

bool DialInput::poll(Common::pollingLoopTime_t time)
{
    if (cursor == -1 || (time - lastPoll) > POLLING_INTERVAL)
    {
        int rawValue = analogRead(pin);

        // First time in fill the filter with the first input value.
        if (cursor == -1)
        {
            for (int i = 0; i < ANALOG_FILTER_LENGTH; i++)
                inputs[i] = rawValue;
            cursor = 0;
        }

        lastPoll = time;

        inputs[cursor] = rawValue;
        cursor++;
        if (cursor >= ANALOG_FILTER_LENGTH)
            cursor = 0;

        int runningAverage = (max - min) / 2048; // Shifts output up 0.5
        for(int i = 0; i < ANALOG_FILTER_LENGTH; i++) {
            runningAverage += inputs[i];
        }
        runningAverage /= ANALOG_FILTER_LENGTH;

        value = min + (runningAverage * (max + 1 - min) / 1024);
        return true;
    }
    return false;
}
} // namespace Teensy31