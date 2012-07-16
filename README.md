avr-tino
========

A simple library to program 8bits AVR. Mostly intented as a lightweight low-footprint
replacement for Arduino while programming ATTiny.

Only core functionalities are implemented (on as-need basis) with no (or low) runtime 
overhead.

The library is written in C+11. as the time of this writting in order to use the new
''static_assert'' function in order to provide compile-time error for library functions.

The target compiler is avr-gcc
