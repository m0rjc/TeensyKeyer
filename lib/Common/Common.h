#ifndef __COMMON_H__
#define __COMMON_H__

#include <Arduino.h>

namespace Common {

typedef uint32_t pollingLoopTime_t;

enum KeyerMode
{
    modeA = 0,
    modeB = 1
};

enum KeyInput : byte
{
    none = 0,
    dit = 1,
    dah = 2,
    squeeze = 3
};

class IKeyHardware
{
  public:
    virtual KeyInput readSwitches(void) = 0;
    };

    class IMorseDecoder {
        public:
        virtual void onDit(void) = 0;
        virtual void onDah(void) = 0;
        virtual void onCharacterGap(void) = 0;
    };

    class ISymbolCallback {
        public:
        virtual void onSymbol(unsigned short symbol) = 0;
    };

    class IPinOutput {
        public:
        virtual void txOn(void) = 0;
        virtual void txOff(void) = 0;
    };

    class IPinInput {
        public:
          virtual void poll(pollingLoopTime_t time) = 0;
          virtual bool isOn(void) = 0;
    };

    class OutputRouterInput;

    /**
     * Multi input to one or two output sidetone routing.
     * For example to route straight key, iambic key and any message generator to radio and sidetone.
     * An informational message generator would not want to key the radio.
     * Up to 8 inputs are combined by OR
     */
    class OutputRouter
    {
      private:
        unsigned char state : 1, 
                      inputs : 7, 
                      allocatedBits: 7;

        IPinOutput **outputs;

        void onChange(void);
        unsigned char allocateBitMask(void);
        void freeBitMask(unsigned char);

      public:
        OutputRouter(IPinOutput &output);
        OutputRouter(IPinOutput &output1, IPinOutput &output2);
        OutputRouter(IPinOutput &output1, IPinOutput &output2, IPinOutput &output3);

        ~OutputRouter();

        friend class OutputRouterInput;
    };

    class OutputRouterInput : public IPinOutput
    {
        private:
            OutputRouter &router;
            uint8_t bitMask;

        public:
            OutputRouterInput(OutputRouter &router) : router(router) 
            {
                bitMask = router.allocateBitMask();
            }

            ~OutputRouterInput() {
                router.freeBitMask(bitMask);
            }

            void txOn()
            {
                router.inputs |= bitMask;
                router.onChange();
            }

            void txOff()
            {
                router.inputs &= ~bitMask;
                router.onChange();
            }
    };
}

#endif
