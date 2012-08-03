avr-tino
========

A simple library to program 8bits AVR. Mostly intented as a lightweight low-footprint
library for Atmel AVR ATtiny mcu familly.

Only core functionalities are implemented (on as-need basis) with no (or low) runtime 
overhead.

The library is written in C++ using template for agressive optimization.

The target compiler is avr-gcc. Build and tested against avr-gcc-4.4.8 (avr-gcc 4.7.1
appears to produce bad code)
