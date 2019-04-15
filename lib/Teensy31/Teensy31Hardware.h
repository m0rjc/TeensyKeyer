#include <Arduino.h>
#include "Common.h"

#define ANALOG_FILTER_LENGTH 8

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
            uint16_t readings : 10,
                     lastRead : 1;
            Common::pollingLoopTime_t lastTime = 0;
        public:
          SwitchInputPin(uint8_t pin) : pin(pin), readings(0x3FF), lastRead(1) { pinMode(pin, INPUT_PULLUP); }
          void poll(Common::pollingLoopTime_t time);
          bool isOn(void);
    };

    class UsbKeyboardOutput : public Common::ISymbolCallback
    {
        public:
          UsbKeyboardOutput(void);
          void onSymbol(unsigned short symbol);
    };

    class DialInput
    {
        private:
            uint8_t pin;
            int inputs[ANALOG_FILTER_LENGTH];
            int8_t cursor = -1;
            int min, max, value;
            Common::pollingLoopTime_t lastPoll;

        public:
            DialInput(uint8_t pin, int min, int max) :
                 pin(pin), min(min), max(max) {}

            bool poll(Common::pollingLoopTime_t time);
            int getReading() { return value; }
    };
}