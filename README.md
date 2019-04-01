# TeensyKeyer
Morse Code Keyboard using a Teensy 3.2

The long term aim is to create an iambic keyer around a Teensy 3.2 microcontroller.
This keyer will also act as a USB keyboard so allowing me to practice Morse Code by using it to type.

The program is being developed incrementally. My first task has been to prove my development environment by programming the Teensy to
blink its LED in Morse Code.

Development Plan, not necessarily in order

* Generate a side-tone.
* A basic keyer that can read dits and dahs but no iambic keying.
* Interpret input Morse characters and send corresponding key codes as a USB keyboard.
* Add analogue inputs to control speed, ratio, sidetone.
* Iambic keying, ideally switchable A and B
* The finished hardware will have an open collector output stage on the D13 pin to allow it to key my old Ten-Tec kit radio.
