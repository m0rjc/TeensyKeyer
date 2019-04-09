#include "Common.h"

using namespace Common;

const int DEFAULT_WPM = 20;

inline uint32_t calculateTimeout(unsigned int wpm, unsigned int centiDits)
{
    return (uint32_t)centiDits * 12 / wpm;
}

class HardwareMock : public IKeyHardware
{
  public:
    uint32_t currentTime = 0;
    KeyInput currentInput = KeyInput::none;

    uint32_t getTimeMillis(void) { return currentTime; }
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
    virtual void txOn(void) { isTx = true; }
    virtual void txOff(void) { isTx = false; }
};