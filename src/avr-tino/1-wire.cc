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
#include "avr-tino/delay.h"
#include "avr-tino/interrupt.h"
#include "avr-tino/1-wire.h"


template<class Port>
void Software1Wire<Port>::init(uint8_t mask) {
    Port::toInput(mask);
    Port::set(mask);
}

template<class Port>
void Software1Wire<Port>::writeBit0(uint8_t mask) {
    GuardInterrupt  guard;

    Port::toOutput(mask);
    Port::clear(mask);
    delayMicroseconds(DELAY_A);
    Port::toInput(mask);
    Port::set(mask);
    delayMicroseconds(DELAY_B);
}

template<class Port>
void Software1Wire<Port>::writeBit1(uint8_t mask) {
    GuardInterrupt  guard;

    Port::toOutput(mask);
    Port::clear(mask);
    delayMicroseconds(DELAY_C);
    Port::toInput(mask);
    Port::set(mask);
    delayMicroseconds(DELAY_D);
}

template<class Port>
uint8_t Software1Wire<Port>::readBit(uint8_t mask) {
    GuardInterrupt  guard;

    Port::toOutput(mask);
    Port::clear(mask);
    delayMicroseconds(DELAY_A);
    Port::toInput(mask);
    Port::set(mask);
    delayMicroseconds(DELAY_E);
    uint8_t bits = Port::get(mask);
    delayMicroseconds(DELAY_F);

    return bits;
}

template<class Port>
uint8_t Software1Wire<Port>::detectPresence(uint8_t mask) {
    GuardInterrupt  guard;

    Port::toOutput(mask);
    Port::clear(mask);
    delayMicroseconds(DELAY_H);
    Port::toInput(mask);
    Port::set(mask);
    delayMicroseconds(DELAY_I);
    uint8_t state = Port::get(mask);
    delayMicroseconds(DELAY_J);

    return state;
}

template<class Port>
void Software1Wire<Port>::write(uint8_t mask, uint8_t len, const void* data) {
    for(uint8_t i = 0; i < len; ++i) {
	uint8_t	byte = ((const uint8_t*)data)[i];
	for(uint8_t bit = 0; bit < 8; ++bit) {
	    if (byte & 0x01) 
		writeBit1(mask);
	    else
		writeBit0(mask);
	    byte >>= 1;
	}
    }
}

template<class Port>
void Software1Wire<Port>::read(uint8_t mask, uint8_t len, void* data) {
    for(uint8_t i = 0; i < len; ++i) {
	((uint8_t*)data)[i] = read(mask);
    }
}

template<class Port>
uint8_t Software1Wire<Port>::read(uint8_t mask) {
    uint8_t result = 0;
    for(uint8_t i = 0; i < 8; ++i) {
	result >>= 1;

	if (readBit(mask) != 0)
	    result |= 0x80;
    }
    return result;
}

