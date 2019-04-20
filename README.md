# TeensyKeyer
Morse Code Keyboard using a Teensy 3.2

The long term aim is to create an iambic keyer around a Teensy 3.2 microcontroller.
This first version of the keyer acts as a USB keyboard so allowing me to practice Morse Code by using it to type.
A later version will drive an LCD display.

The software design is modular, so allowing hopefully easy configuration depending on available hardware
and desired use.

Pin assignment is defined in src/TeensyKeyer.cpp. It is not final. If I add an LCD display or other items
I may need to move pins around.