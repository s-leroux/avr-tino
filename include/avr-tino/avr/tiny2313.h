/*
  Copyright (c) 2012 Sylvain Leroux <sylvain@chicoree.fr>
  
  This file is part of avr-tino -- http://github.com/s-leroux/avr-tino
  
  avr-tino is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  avr-tino is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with avr-tino.  If not, see <http://www.gnu.org/licenses/>.
*/
#if !defined AVR_TINO_TINY2313_H
#define AVR_TINO_TINY2313_H

/*
 List of pins for this mcu
*/
enum __attribute__ ((__packed__)) port_t {
    PORT_BASE_A	= 0x10,
    PORT_BASE_B = 0x20,
    PORT_BASE_D = 0x30,
};
enum __attribute__ ((__packed__)) pin_t {
    PIN_PA0 = PORT_BASE_A | 0,
    PIN_PA1,
    PIN_PA2,
    PIN_PB0 = PORT_BASE_B | 0,
    PIN_PB1,
    PIN_PB2,
    PIN_PB3,
    PIN_PB4,
    PIN_PB5,
    PIN_PB6,
    PIN_PB7,
    PIN_PD0 = PORT_BASE_D | 0,
    PIN_PD1,
    PIN_PD2,
    PIN_PD3,
    PIN_PD4,
    PIN_PD5,
    PIN_PD6,
};

/* SPI interface */
static const pin_t MOSI = PIN_PB5;
static const pin_t MISO = PIN_PB6;
static const pin_t SCK	= PIN_PB7;

/*
 I/O Port abstraction layer
*/
#define pin_to_bit(p) ( p & 0x0F )
#define pin_to_mask(p) ( _BV(p & 0x0F) )

/* Those functions are as efficient as their macro counterparts */
static volatile uint8_t& pin_to_mode(pin_t p) {
    return ( (p <= PIN_PA2) ? DDRA :
	     (p <= PIN_PB7) ? DDRB :
	     DDRD );
}

static volatile uint8_t& pin_to_output(pin_t p) {
    return ( (p <= PIN_PA2) ? PORTA :
	     (p <= PIN_PB7) ? PORTB :
	     PORTD );
}

static volatile uint8_t& pin_to_input(pin_t p) {
    return ( (p <= PIN_PA2) ? PINA :
	     (p <= PIN_PB7) ? PINB :
	     PIND );
}

#include "avr-tino/pin.h"

typedef Port<0x1A, 0x19, 0x1B>	    PortA;
typedef Port<0x17, 0x16, 0x18>	    PortB;
typedef Port<0x11, 0x10, 0x12>	    PortD;

#endif

