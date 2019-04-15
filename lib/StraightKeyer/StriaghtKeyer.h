#ifndef __STRAIGHT_KEYER_H__
#define __STRAIGHT_KEYER_H__

#include <Arduino.h>
#include "Common.h"

namespace StraightKeyer
{
    enum Status : byte { unknown, on, off };

    class Keyer
    {
    private:
        Common::IPinInput &m_hardware;
        Common::IPinOutput &m_sideTone;
        Status m_status = unknown;

        Common::IMorseDecoder &m_decoder;
        Common::pollingLoopTime_t m_eventStart = 0;
        Common::pollingLoopTime_t m_ditDahBoundary = 0;
        Common::pollingLoopTime_t m_characterGapThreshold = 0;

        void discriminateTone(Common::pollingLoopTime_t duration);
    public:
        Keyer(Common::IPinInput &hardware,
              Common::IMorseDecoder &decoder,
              Common::IPinOutput &sideTone) : m_hardware(hardware), m_sideTone(sideTone), m_decoder(decoder) {}

        void setRate(uint8_t wpm);

        void poll(Common::pollingLoopTime_t time);
    };
} // namespace IambicKeyer

#endif
