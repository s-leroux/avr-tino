/*
  Copyright (c) 2012-2013 Sylvain Leroux <sylvain@chicoree.fr>
  
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
#if !defined AVR_TINO_MEGA328_H
#define AVR_TINO_MEGA328_H

/*
 Support for ATmega328/ATmega328P MCU
*/

/*
 List of pins for this mcu

 Base data from ${AVR_BASE}/include/avr/iom328p.h

*/
enum __attribute__ ((__packed__)) port_t {
    PORT_BASE_B	= 0x10,
    PORT_BASE_C = 0x20,
    PORT_BASE_D = 0x30,
};
enum __attribute__ ((__packed__)) pin_t {
    PIN_PB0 = PORT_BASE_B | 0,
    PIN_PB1,
    PIN_PB2,
    PIN_PB3,
    PIN_PB4,
    PIN_PB5,
    PIN_PB6,
    PIN_PB7,
    PIN_PC0 = PORT_BASE_C | 0,
    PIN_PC1,
    PIN_PC2,
    PIN_PC3,
    PIN_PC4,
    PIN_PC5,
    PIN_PC6,
    PIN_PD0 = PORT_BASE_D | 0,
    PIN_PD1,
    PIN_PD2,
    PIN_PD3,
    PIN_PD4,
    PIN_PD5,
    PIN_PD6,
    PIN_PD7,
};

/* SPI interface */
static const pin_t MOSI = PIN_PB3;
static const pin_t MISO = PIN_PB4;
static const pin_t SCK	= PIN_PB5;

/*
 I/O Port abstraction layer
*/
#define pin_to_bit(p) ( p & 0x0F )
#define pin_to_mask(p) ( _BV(p & 0x0F) )

/* Those functions are as efficient as their macro counterparts */
static volatile uint8_t& pin_to_mode(pin_t p) {
    return ( (p < PIN_PC0) ? DDRB :
	     (p < PIN_PD0) ? DDRC :
	     DDRD );
}

static volatile uint8_t& pin_to_output(pin_t p) {
    return ( (p < PIN_PC0) ? PORTB :
	     (p < PIN_PD0) ? PORTC :
	     PORTD );
}

static volatile uint8_t& pin_to_input(pin_t p) {
    return ( (p < PIN_PC0) ? PINB :
	     (p < PIN_PD0) ? PINC :
	     PIND );
}

/**
    TWI (2-wire/i2c) implementation
*/
struct ATmega328_TWI {
    static const uint8_t    CR = 0xBC; /* XXX */
    static const uint8_t    START   = _BV(TWINT)|_BV(TWEN)|_BV(TWSTA);
    static const uint8_t    STOP    = _BV(TWINT)|_BV(TWEN)|_BV(TWSTO);
    static const uint8_t    WRITE   = _BV(TWINT)|_BV(TWEN);
    static const uint8_t    SEND    = _BV(TWINT);

    static const uint8_t    READ_ACK= _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
    static const uint8_t    READ_NACK= _BV(TWINT) | _BV(TWEN);

    static const uint8_t    SR = 0xB9; /* XXX */
    static const uint8_t    MT_START = 0x08; /* datasheet p229 */
    static const uint8_t    MT_W_SLA_ACK = 0x18; /* datasheet p229 */
    static const uint8_t    MT_W_SLA_NACK = 0x20; /* datasheet p229 */
    static const uint8_t    MT_W_DATA_ACK = 0x28; /* datasheet p229 */
    static const uint8_t    MT_W_DATA_NACK = 0x30; /* datasheet p229 */

    static const uint8_t    MR_R_SLA_ACK = 0x40; /* datasheet p232 */
    static const uint8_t    MR_R_SLA_NACK = 0x48; /* datasheet p232 */
    static const uint8_t    MR_R_DATA_ACK = 0x50; /* datasheet p232 */
    static const uint8_t    MR_R_DATA_NACK = 0x58; /* datasheet p232 */



    static const uint8_t    DR = 0xBB; /* XXX */
};


#include "avr-tino/pin.h"

typedef Port<0x03>	    PortB;
typedef Port<0x06>	    PortC;
typedef Port<0x09>	    PortD;


#include "avr-tino/EEPROM.h"
#include "avr-tino/SPI.h"
#include "avr-tino/serial.h"
#include "avr-tino/TWI.h"

typedef SPIMaster<0x2C,0x2D,0x2E,SPIF>			SPI;
typedef Serial<0xC6, 0xC0, 0xC1, 0xC2, 0xC4, 0xC5>	USART;


struct MCU {
    typedef ::PortB PortB;
    typedef ::PortC PortC;
    typedef ::PortD PortD;

    typedef ::TWI<ATmega328_TWI>    TWI;
    typedef ::USART		    USART;
#if 1
    typedef ::Buffer<USART>         RECV;
#endif
};

#include "avr/interrupt.h"

#if 1
ISR(USART_RX_vect) {
    MCU::PortC::set(_BV(0));

    MCU::RECV::append(UDR0);
}
#endif

/*
typedef EEPROM<0x1C, EEMPE, EEPE, EERE, EEPM0, EEPM1,
	       0x1E,
	       0x1D>		    EEPROM;
*/
#endif

