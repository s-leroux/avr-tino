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
class Serial {
    public:
    enum __attribute__ ((__packed__)) protocol_t {
	// Asynchronous
	A8N1	= 6 ,
	A8E1	= _BV(UPM1) | 6 ,
	A8O1	= _BV(UPM1) |_BV(UPM0) |  6 ,
	// Synchronous
	S8N1	= _BV(UMSEL) | 6 ,
	S8E1	= _BV(UMSEL) | _BV(UPM1) | 6 ,
	S8O1	= _BV(UMSEL) | _BV(UPM1) | _BV(UPM0) | 6 ,
    };

    void begin(uint32_t baud, protocol_t protocol = A8N1) const {
	uint16_t UBRR = F_CPU / 16 / baud - 1;
	
	UBRRH = (uint8_t)(UBRR >> 8);
	UBRRL = (uint8_t)(UBRR);
	
	UCSRB = _BV(RXEN) | _BV(TXEN);
	UCSRC = protocol;
    }

    void print(const char* str) const {
	while(*str) { send(*str++); }
    }

    void send(uint8_t data) const {
	while ( ! (UCSRA & _BV(UDRE)) ) {
	    // do nothing
	}
	UDR = data;
    }

    uint8_t receive() const {
	while ( ! (UCSRA & _BV(RXC) ) ) {
	    // do nothing
	}
	return UDR;
    }
};


#endif


