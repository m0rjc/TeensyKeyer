#include "Common.h"

using namespace Common;

const int DEFAULT_WPM = 20;

inline pollingLoopTime_t calculateTimeout(unsigned int wpm, unsigned int centiDits)
{
    return (pollingLoopTime_t)centiDits * 12 / wpm;
}

class HardwareMock : public IKeyHardware
{
  public:
    KeyInput currentInput = KeyInput::none;
    KeyInput readSwitches(void) { return currentInput; };
};

class DecoderMock : public IMorseDecoder
{
  private:
    void append(char c) {
        int i;
        for(i = 0; capture[i] != 0 && i < 63; i++);
        capture[i] = c;
        capture[i+1] = 0;
    }
  public:
    char capture[64];
    DecoderMock() { *capture = 0; }
    void onDit(void) { append('.'); }
    void onDah(void) { append('-'); }
    void onCharacterGap(void) { append(' '); }
};

class SideToneMock : public ISideToneHardware
{
  public:
    bool isTx = false;
    unsigned int callCount = 0;
    virtual void txOn(void) { isTx = true; callCount++; }
    virtual void txOff(void) { isTx = false; callCount++; }
};

class PinInputMock : public IPinInput
{
    public:
    bool state = false;
    void poll(pollingLoopTime_t tick) {}
    bool isOn(void) { return state; }
};