#include "Common.h"

namespace Ux {
    class MorseOutput {
        private:
         Common::IPinOutput &output;
         union {
             const uint8_t *symbol;
             const char *text;
         } ptr ;
         uint8_t currentChar = 0;
         Common::pollingLoopTime_t timeStarted;
         unsigned int duration = 0;
         bool isActive = false;
         bool isPlayingTone;
         bool isPlayingAscii;
         unsigned int ditLength = 0;

        public:
         MorseOutput(Common::IPinOutput &output) : output(output) {}
        void playSymbols(const uint8_t *symbols);
        void playAscii(const char *text);
        void poll(Common::pollingLoopTime_t time);
        void setRate(unsigned int wpm) { ditLength = Common::getDitLength(wpm); }
    };
}