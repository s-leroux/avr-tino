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
#include "avr-tino.h"
#include "avr-tino/pin.h"

#if 0

void pinMode(pin_t pin, pinmode_t mode) {
    if (mode == INPUT) {
	pin_to_mode(pin) &= ~pin_to_mask(pin);
    }
    else {
	pin_to_mode(pin) |= pin_to_mask(pin);
    }
}

void digitalWrite(pin_t pin, pinstate_t state) {
    if (state == LOW) {
	pin_to_output(pin) &= ~pin_to_mask(pin);
    }
    else {
	pin_to_output(pin) |= pin_to_mask(pin);
    }
}

pinstate_t digitalRead(pin_t pin) {
    return ( pin_to_input(pin) & pin_to_mask(pin) ) ? HIGH : LOW;
}

/*
    Based on shiftIn part of Arduino by David A. Mellis
*/
uint8_t shiftIn(pin_t dataPin, pin_t clockPin,
                bitorder_t bitOrder) {
    uint8_t value = 0;

    for (uint8_t i = 0; i < 8; ++i) {
	digitalWrite(clockPin, HIGH);
	if (bitOrder == LSBFIRST) {
	    value |= digitalRead(dataPin) << i;
	}
	else {
	    value |= digitalRead(dataPin) << (7 - i);
	}
	digitalWrite(clockPin, LOW);
    }

    return value;
}

/*
    Based on shiftIn part of Arduino by David A. Mellis
*/
void shiftOut(pin_t dataPin, pin_t clockPin,
                bitorder_t bitOrder,
                uint8_t val) {
    for (uint8_t i = 0; i < 8; ++i) {
	if (bitOrder == LSBFIRST) {
	    digitalWrite(dataPin, (pinstate_t)((val >> i) & 0x01));
	}
	else {
	    digitalWrite(dataPin, (pinstate_t)((val >> (7 - i)) & 0x01));
	}
	digitalWrite(clockPin, HIGH);
	digitalWrite(clockPin, LOW);
    }
}

void shiftOut(pin_t dataPin, pin_t clockPin,
                bitorder_t bitOrder,
                const void *data, uint16_t len) {
    for(uint16_t i = 0; i < len; ++i) {
	shiftOut(dataPin, clockPin, bitOrder, ((const uint8_t*)data)[i]);
    }
}

#endif
