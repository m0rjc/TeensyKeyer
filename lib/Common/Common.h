#ifndef __COMMON_H__
#define __COMMON_H__

namespace Common {

    enum KeyerMode {
        modeA = 0,
        modeB = 1
    };

    enum KeyInput : byte { none = 9, dit = 1, dah = 2, squeeze = 3 };

    class IKeyHardware {
        public:
        virtual uint32_t getTimeMillis(void);
        virtual KeyInput readSwitches(void) = 0;
//        virtual bool isSqueezeCaptured(void) = 0;
//        virtual void clearSqueezeCapture(void) = 0; 
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

}

#endif
