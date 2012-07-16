#if !defined avr_tino_h
#define avr_tino_h

//
// Core includes

#include <avr/io.h>
#include <avr/pgmspace.h>

#if !defined BOARD
#  define BOARD "factory"
#endif

#if defined BOARD_Factory
#  include "avr-tino/target/factory.h"
#else
# error "Unknown target BOARD. Don't you forget '-DBOARD_...'?"
#endif

// Borrowed from /usr/lib/avr/include/avr/io.h 
#if defined (__AVR_ATtiny2313__)
#  include "avr-tino/avr/tiny2313.h"
#else
# error "Unknown target AVR. Don't you forget '-mmcu'?"
#endif


#include "avr-tino/pin.h"

#endif
