namespace MorseDecode {
    typedef void (*symbolCallback)(unsigned short);

    /**
     * Initialise.
     */
    extern void init(symbolCallback callback);

    /**
     * The keyer has received a dit.
     */
    extern void onDit();

    /**
     * The keyer has received a dah.
     */
    extern void onDah();

    /**
     * The keyer has been idle long enough to terminate a character.
     * This will be called repeatedly if the user steps away from the keyer.
     */
    extern void onCharacterGap();
};
