#include <Arduino.h>
#include "Common.h"

namespace Teensy31 {
    class PinSidetoneHardware : public Common::ISideToneHardware
    {
        private:
            int pin;
        public:
          PinSidetoneHardware(int pin);
          void txOn();
          void txOff();
    };

    class ToneSidetoneHardware : public Common::ISideToneHardware
    {
      private:
        int pin;
        int freq = 600;
        bool state = false;
      public:
        ToneSidetoneHardware(int pin);
        void txOn();
        void txOff();
        void setFrequency(int value);
    };

    class DebouncedInputPin : public Common::IPinInput {
        private:
            int pin;
        public:
          DebouncedInputPin(int pin) : pin(pin) {}
          void poll(void);
    };

    class KeyerHardware : public Common::IKeyHardware
    {
        private:
            Common::IPinInput &ditPin, &dahPin, &straightPin;
            pollingLoopTime_t pollTime;

        public:
            void poll(void);
            pollingLoopTime_t getTimeMillis(void);
            Common::KeyInput readSwitches(void);
            bool isStraightKeyDown(void);
    };
}