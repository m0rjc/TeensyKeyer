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

    Common::OutputRouter localOutputs;
    Common::OutputRouterInput radioToLocalRouting;
    Common::OutputRouter radioOutput;

    Common::OutputRouterInput iambicKeyerOutputRouting;
    IambicKeyer::Keyer iambicKeyer;

    Common::OutputRouterInput straightKeyerOutputRouting;
    StraightKeyer::Keyer straightKeyer;

    public:
        TeensyKeyer() :
            // Pin assignments
            ledPin(13),
            radioPin(12),
            speakerPin(11),
            ditInputPin(3),
            dahInputPin(4),
            straightInputPin(5),
            // Iambic paddle and decoder
            paddle(ditInputPin, dahInputPin),
            keyboard(),
            decoder(keyboard),
            // Output routing for local only outputs, and radio output which also triggers
            // local only.
            localOutputs(ledPin, speakerPin),
            radioToLocalRouting(localOutputs),
            radioOutput(radioPin, radioToLocalRouting),
            iambicKeyerOutputRouting(radioOutput),
            iambicKeyer(paddle, decoder, iambicKeyerOutputRouting),
            straightKeyerOutputRouting(radioOutput),
            straightKeyer(straightInputPin, straightKeyerOutputRouting)
            {}

        void poll(void) {
            Common::pollingLoopTime_t time = millis();
            ditInputPin.poll(time);
            dahInputPin.poll(time);
            straightInputPin.poll(time);
            iambicKeyer.poll(time);
            straightKeyer.poll(time);
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
