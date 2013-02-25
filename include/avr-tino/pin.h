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
#if !defined AVR_TINO_PIN_H
#define AVR_TINO_PIN_H

#if !defined NOT_A_PORT
#  define NOT_A_PORT uint8_t(-1)
#else
#  error "Multiple definitions of NOT_A_PORT"
#endif

enum __attribute__ ((__packed__)) pinmode_t {
    INPUT = 0,
    OUTPUT,
};

enum __attribute__ ((__packed__)) pinstate_t {
    LOW,
    HIGH
};

//extern "C" {

#define pinToHigh(pin) ( pin_to_output(pin) |= pin_to_mask(pin) )
#define pinToLow(pin) ( pin_to_output(pin) &= ~pin_to_mask(pin) )
#define pinToOutput(pin) ( pin_to_mode(pin) |= pin_to_mask(pin) )
#define pinToInput(pin) ( pin_to_mode(pin) &= ~pin_to_mask(pin) )

template<pin_t pin>
class GuardPinLow {
    public:
    inline GuardPinLow() {
	pinToLow(pin);
    }

    inline ~GuardPinLow() {
	pinToHigh(pin);
    }
};

template<pin_t pin>
class GuardPinHigh {
    public:
    inline GuardPinHigh() {
	pinToHigh(pin);
    }

    inline ~GuardPinHigh() {
	pinToLow(pin);
    }
};

static void pinMode(pin_t pin, pinmode_t mode);
static void digitalWrite(pin_t pin, pinstate_t state);
static inline void digitalWrite(pin_t pin, bool state) {
	digitalWrite(pin, state ? HIGH : LOW);
}

static pinstate_t digitalRead(pin_t pin);

enum __attribute__ ((__packed__)) bitorder_t {
    LSBFIRST,
    MSBFIRST
};

static uint8_t shiftIn(pin_t dataPin, pin_t clockPin, 
		bitorder_t bitOrder);
static void shiftOut(pin_t dataPin, pin_t clockPin, 
		bitorder_t bitOrder, 
		uint8_t val);

static void shiftOut(pin_t dataPin, pin_t clockPin,
                bitorder_t bitOrder,
                const void *data, uint16_t size);

template<uint8_t PIN, uint8_t DDR = PIN+1, uint8_t POUT = DDR+1>
class Port {
    public:
    static void toInput(uint8_t mask) {
	_SFR_IO8(DDR) &= ~mask;
    }

    static void toOutput(uint8_t mask) {
	_SFR_IO8(DDR) |= mask;
    }

    static uint8_t get(uint8_t mask) {
	return _SFR_IO8(PIN) & mask;
    }

    static void set(uint8_t mask) {
	_SFR_IO8(POUT) |= mask;
    }

    static void clear(uint8_t mask) {
	_SFR_IO8(POUT) &= ~mask;
    }
};

//} // extern "C"

#endif
