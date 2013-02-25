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

// XXX should use namespace instead ?
template <uint8_t DR, uint8_t SRA, uint8_t SRB, uint8_t SRC, uint8_t RRL, uint8_t RRH>
class Serial {
    public:
    
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
	TXEN	    = 3,
	RXEN	    = 4,
    };

    enum {
	UDRE	    = 5,
	TXC	    = 6,
	RXC	    = 7,
    };

    static void begin(uint32_t baud, protocol_t protocol = A8N1) {
	uint16_t UBRR = F_CPU / 16 / baud - 1;
	
	_SFR_MEM8(RRH) = (uint8_t)(UBRR >> 8);
	_SFR_MEM8(RRL) = (uint8_t)(UBRR);
	
	_SFR_MEM8(SRB) = _BV(RXEN) | _BV(TXEN);
	_SFR_MEM8(SRC) = protocol;
    }

    static void print(const char* str) {
	while(*str) { send(*str++); }
    }

    static void send(uint8_t data) { // XXX Shouldn't we call that 'write' ?
	while ( ! (_SFR_MEM8(SRA) & _BV(UDRE)) ) {
	    // do nothing
	}
	_SFR_MEM8(DR) = data;
    }

    static void send(const void* data, uint8_t len) {
	for(uint8_t i = 0; i < len; ++i) {
	    send(((const uint8_t*)data)[i]);
	}
    }

    static uint8_t receive() {
	while ( ! (_SFR_MEM8(SRA) & _BV(RXC) ) ) {
	    // do nothing
	}
	return _SFR_MEM8(DR);
    }
};


#endif


