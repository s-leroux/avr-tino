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
    LOW = 0,
    HIGH
};

//extern "C" {

enum __attribute__ ((__packed__)) bitorder_t {
    LSBFIRST,
    MSBFIRST
};

template<uint8_t PIN, uint8_t DDR = PIN+1, uint8_t POUT = DDR+1>
class Port {
    public:
    //
    // Port direction
    //
    static void toInput(uint8_t mask) {
	_SFR_IO8(DDR) &= ~mask;
    }

    static void toOutput(uint8_t mask) {
	_SFR_IO8(DDR) |= mask;
    }

    //
    // Whole port access
    //
    static uint8_t read(uint8_t mask = 0xFF) {
	return _SFR_IO8(PIN) & mask;
    }

    static void write(uint8_t value) {
	_SFR_IO8(POUT) = value;
    }

    static void invert(uint8_t mask = 0xFF) {
        write(read() ^ mask);
    }

    //
    // Pin-group access
    //
    static void set(uint8_t mask = 0xFF) {
	_SFR_IO8(POUT) |= mask;
    }

    static void clear(uint8_t mask = 0xFF) {
	_SFR_IO8(POUT) &= ~mask;
    }
};

template<class _Port, uint8_t _pin>
class Pin {
    public:
    typedef Pin<_Port, _pin>    This;

    static const uint8_t  pin = _pin;
    static const uint8_t  mask = 1<<_pin;
    typedef _Port    Port;

    static void set() { _Port::set(This::mask); }
    static void clear() { _Port::clear(This::mask); }
};

template<class DataPin, class ClockPin>
static void shiftOut(uint8_t value) {
    /* !!! data and clock are *masks* not pin numbers !!! */
    for(uint8_t i = 0; i < 8; ++i) {
        if ((value & 0x01)) {
            DataPin::set();
        }
        else {
            DataPin::clear();
        }
        ClockPin::set();
        value>>=1;
        ClockPin::clear();
    }
}

//} // extern "C"

#endif
