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
#if !defined AVR_TINO_SERIAL_H
#define AVR_TINO_SERIAL_H

#include "avr-tino/printer.h"

/*
    Hard coded values common (?) to every 8-bit AVR
    XXX
*/
enum __attribute__ ((__packed__)) protocol_t {
    // Asynchronous
    A8N1	= 6 ,
    A8E1	= _BV(5) | 6 ,
    A8O1	= _BV(5) |_BV(4) |  6 ,
    // Synchronous
    S8N1	= _BV(6) | 6 ,
    S8E1	= _BV(6) | _BV(5) | 6 ,
    S8O1	= _BV(6) | _BV(5) | _BV(4) | 6 ,
};

enum {
    f_TXEN	    = 3,
    f_RXEN	    = 4,
};

enum {
    f_UDRE	    = 5,
    f_TXC	    = 6,
    f_RXC	    = 7,
};

template <uint8_t mode, uint16_t _baud>
class SerialMode {
    /** Asynchronous serial communication */

    public:
    static const uint8_t protocol = mode;
    static const uint16_t baud = _baud;
    static const bool   hasU2X = ! (mode & _BV(6));
};

class NoFlowControl {
    public:
    static bool readyToSend() { return true; }
};

template<class USART, class Protocol, class FlowControl>
class NSerial {
    /** The new "Serial" class. Protocol based.
    
        USART is the register mapping for a given MCU. */
    
    public:
    static void begin() {
	uint16_t UBRR = (Protocol::hasU2X) ? F_CPU / 8 / Protocol::baud - 1
                                        : F_CPU / 16 / Protocol::baud - 1;
	
	_SFR_MEM8(USART::RRH) = (uint8_t)(UBRR >> 8);
	_SFR_MEM8(USART::RRL) = (uint8_t)(UBRR);
	
	_SFR_MEM8(USART::SRC) = Protocol::protocol;
        if (Protocol::hasU2X) {
            _SFR_MEM8(USART::SRA) |= _BV(U2X0);
        }
        USART::enable();
    }

    static void send(uint8_t data) { // XXX Shouldn't we call that 'write' ?
	while ( USART::busy() ) {
	    // do nothing
	}

	while ( ! FlowControl::readyToSend() ) {
	    // do nothing
	}

	USART::write(data);
    }


    static void write(const char* str) {
	while(*str) { send(*str++); }
    }

    static void send(const void* data, uint8_t len) {
	for(uint8_t i = 0; i < len; ++i) {
	    send(((const uint8_t*)data)[i]);
	}
    }

    static bool available() {
        return USART::available();
    }

    static uint8_t receive() {
	while ( ! available ) {
	    // do nothing
	}
	return USART::read();
    }
};

// XXX should use namespace instead ?
template <uint8_t DR, uint8_t SRA, uint8_t SRB, uint8_t SRC, uint8_t RRL, uint8_t RRH>
class Serial : public Printer<Serial<DR,SRA,SRB,SRC,RRL,RRH> > {
    public:

    static void begin(uint32_t baud, protocol_t protocol = A8N1) {
	uint16_t UBRR = F_CPU / 16 / baud - 1;
	
	_SFR_MEM8(RRH) = (uint8_t)(UBRR >> 8);
	_SFR_MEM8(RRL) = (uint8_t)(UBRR);
	
	_SFR_MEM8(SRB) = _BV(f_RXEN) | _BV(f_TXEN);
	_SFR_MEM8(SRC) = protocol;
    }

    static void write(const char* str) {
	while(*str) { send(*str++); }
    }

    static void send(uint8_t data) { // XXX Shouldn't we call that 'write' ?
	while ( ! (_SFR_MEM8(SRA) & _BV(f_UDRE)) ) {
	    // do nothing
	}

        // XXX Hardware flow control -- testing !!!
        // on the test board /CTS connected to PB7
        //
        while( (PINB & (1<<PB7)) ) {
	    // do nothing

        }
	_SFR_MEM8(DR) = data;
    }

    static void send(const void* data, uint8_t len) {
	for(uint8_t i = 0; i < len; ++i) {
	    send(((const uint8_t*)data)[i]);
	}
    }

    static bool available() {
        return _SFR_MEM8(SRA) & _BV(f_RXC);
    }

    static uint8_t receive() {
	while ( ! (_SFR_MEM8(SRA) & _BV(f_RXC) ) ) {
	    // do nothing
	}
	return _SFR_MEM8(DR);
    }
};

template <class USART>
class Buffer {
/**
    Support buffered read from USART
*/
    public:
    static const uint8_t BUFFER_SIZE    = 64;

    static inline bool available() {
//    if (top != tail) PORTC |= _BV(1);
        return top != tail;
    }

    static inline void append(uint8_t byte) {
        buffer[tail] = byte;

        tail = (tail+1) % BUFFER_SIZE;
        if (tail == top)
            top = (top + 1) % BUFFER_SIZE;
    }

    static inline uint8_t pop() {
        if (!available()) {
            return 0;
        }

        uint8_t result = buffer[top];
        top = (top + 1) % BUFFER_SIZE;

        return result;
    }
    

    static volatile uint8_t buffer[BUFFER_SIZE];
    static volatile uint8_t  top;
    static volatile uint8_t  tail;
};

template<class USART> volatile uint8_t  Buffer<USART>::top     = 0;
template<class USART> volatile uint8_t  Buffer<USART>::tail    = 0;
template<class USART> volatile uint8_t Buffer<USART>::buffer[Buffer<USART>::BUFFER_SIZE];


#endif


