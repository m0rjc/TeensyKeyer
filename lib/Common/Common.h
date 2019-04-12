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

    class ISideToneHardware {
        public:
        virtual void txOn(void) = 0;
        virtual void txOff(void) = 0;
    };

    class IPinInput {
        public:
          virtual void poll(pollingLoopTime_t time) = 0;
          virtual bool isOn(void) = 0;
    };

    class SideToneRouterInput;

    /**
     * Multi input to one or two output sidetone routing.
     * For example to route straight key, iambic key and any message generator to radio and sidetone.
     * An informational message generator would not want to key the radio.
     * Up to 8 inputs are combined by OR
     */
    class SideToneRouter
    {
      private:
        unsigned char state : 1, 
                      inputs : 7, 
                      allocatedBits: 7;

        ISideToneHardware **outputs;

        void onChange(void);
        unsigned char allocateBitMask(void);
        void freeBitMask(unsigned char);

      public:
        SideToneRouter(ISideToneHardware &output);
        SideToneRouter(ISideToneHardware &output1, ISideToneHardware &output2);
        SideToneRouter(ISideToneHardware &output1, ISideToneHardware &output2, ISideToneHardware &output3);

        ~SideToneRouter();

        friend class SideToneRouterInput;
    };

    class SideToneRouterInput : public ISideToneHardware
    {
        private:
            SideToneRouter &router;
            uint8_t bitMask;

        public:
            SideToneRouterInput(SideToneRouter &router) : router(router) 
            {
                bitMask = router.allocateBitMask();
            }

            ~SideToneRouterInput() {
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
