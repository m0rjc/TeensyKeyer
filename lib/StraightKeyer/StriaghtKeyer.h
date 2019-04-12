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
        Common::ISideToneHardware &m_sideTone;
        Status m_status = unknown;

    public:
        Keyer(Common::IPinInput &hardware,
            Common::ISideToneHardware &sideTone) : m_hardware(hardware), m_sideTone(sideTone) {}

        void poll(Common::pollingLoopTime_t time);
    };
} // namespace IambicKeyer

#endif
