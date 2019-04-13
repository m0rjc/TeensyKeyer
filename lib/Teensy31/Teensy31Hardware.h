#include <Arduino.h>
#include "Common.h"

namespace Teensy31 {
    class PinOutput : public Common::IPinOutput
    {
        private:
            int pin;
        public:
          PinOutput(int pin) : pin(pin) { pinMode(pin, OUTPUT); }
          void txOn() { digitalWrite(pin, HIGH); }
          void txOff() { digitalWrite(pin, LOW); }
    };

    class ToneOutput : public Common::IPinOutput
    {
      private:
        uint8_t pin;
        uint16_t freq = 600;
        bool state = false;

      public:
        ToneOutput(int pin) : pin(pin) { pinMode(pin, OUTPUT); }

        void txOn()
        {
            tone(pin, freq);
            state = true;
        };

        void txOff()
        {
            noTone(pin);
            state = false;
        }

        void setFrequency(uint16_t value)
        {
            freq = value;
            if (state)
            {
                txOff();
                txOn();
            }
        };
    };

    class SwitchInputPin : public Common::IPinInput {
        private:
            uint8_t pin;
        public:
          SwitchInputPin(uint8_t pin) : pin(pin) { pinMode(pin, INPUT_PULLUP); }
          void poll(Common::pollingLoopTime_t time) {}; // Debounce logic would go here.
          bool isOn(void) { return !digitalRead(pin); }
    };

    class UsbKeyboardOutput : public Common::ISymbolCallback
    {
        public:
          UsbKeyboardOutput(void);
          void onSymbol(unsigned short symbol);
    };
}