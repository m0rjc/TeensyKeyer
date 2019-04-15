#include <Arduino.h>
#include <usb_serial.h>

#include "IambicKeyer.h"
#include "MorseDecode.h"
#include "Teensy31Hardware.h"
#include "StriaghtKeyer.h"

class TeensyKeyer {
    Teensy31::PinOutput ledPin;
    Teensy31::PinOutput radioPin;
    Teensy31::ToneOutput speakerPin;

    Teensy31::SwitchInputPin ditInputPin;
    Teensy31::SwitchInputPin dahInputPin;
    Teensy31::SwitchInputPin straightInputPin;

    Common::KeyerHardware paddle;

    Teensy31::UsbKeyboardOutput keyboard;
    MorseDecode::MorseDecoder decoder;
    Common::KeyerSelector keyerSelector;

    Common::OutputRouter localOutputs;
    Common::OutputRouterInput radioToLocalRouting;
    Common::OutputRouter radioOutput;

    Common::KeyerSelectorInput iambicDecoderRouting;
    Common::OutputRouterInput iambicKeyerOutputRouting;
    IambicKeyer::Keyer iambicKeyer;

    Common::KeyerSelectorInput straightDecoderRouting;
    Common::OutputRouterInput straightKeyerOutputRouting;
    StraightKeyer::Keyer straightKeyer;

    Teensy31::DialInput rateInput;

    public:
        TeensyKeyer() :
            // Pin assignments
            ledPin(13),
            radioPin(12),
            speakerPin(11),
            ditInputPin(3),
            dahInputPin(4),
            straightInputPin(5),
            // Iambic paddle
            paddle(ditInputPin, dahInputPin),
            keyboard(),
            decoder(keyboard),
            keyerSelector(decoder),
            // Output routing for local only outputs, and radio output which also triggers
            // local only.
            localOutputs(ledPin, speakerPin),
            radioToLocalRouting(localOutputs),
            radioOutput(radioPin, radioToLocalRouting),
            // Iambic keyer feeds the decoder and radio
            iambicDecoderRouting(keyerSelector, 1),
            iambicKeyerOutputRouting(radioOutput),
            iambicKeyer(paddle, iambicDecoderRouting, iambicKeyerOutputRouting),
            // Straight keyer feeds the decoder and radio
            straightDecoderRouting(keyerSelector, 2),
            straightKeyerOutputRouting(radioOutput),
            straightKeyer(straightInputPin, straightDecoderRouting, straightKeyerOutputRouting),
            rateInput(A0, Common::MIN_RATE_WPM, Common::MAX_RATE_WPM )
            {}

        void poll(void) {
            Common::pollingLoopTime_t time = millis();
            ditInputPin.poll(time);
            dahInputPin.poll(time);
            straightInputPin.poll(time);
            iambicKeyer.poll(time);
            straightKeyer.poll(time);

            if(rateInput.poll(time))
            {
                int rate = rateInput.getReading();
                iambicKeyer.setRate(rate);
                straightKeyer.setRate(rate);
            }
        }

        void setRate(unsigned int wpm) {
            iambicKeyer.setRate(wpm);
        }
};

TeensyKeyer *keyer;

void setup (void) {
    keyer = new TeensyKeyer();
    keyer->setRate(10);
    Serial.begin(9600);
}

pollingLoopTime_t lastPoll = 0;

void loop (void) {
    keyer->poll();
}
