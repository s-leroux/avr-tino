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
#if !defined AVR_TINO_EEPROM_H
#define AVR_TINO_EEPROM_H

#include <util/atomic.h>

/*
template<uint8_t EECR, uint8_t EEMPE, uint8_t EEPE, uint8_t EERE,
		       uint8_t EEPM0, uint8_t EEPM1,
	    uint8_t EEAR,
	    uint8_t EEDR>
*/
class EEPROM {
    public:
    static void write(uint8_t addr, uint8_t data) {
        _wait();
	
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            _write(addr,data);
        }
    }

    static uint8_t read(uint8_t addr) {
        _wait();

        uint8_t result;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            result = _read(addr);
        }

        return result;
    }

    static void read(uint8_t baseAddr, uint8_t len, void* dest) {
        for(uint8_t i = 0; i < len; ++i) {
            ((uint8_t*)dest)[i] = read(baseAddr+i);
        }
    }

    static void _wait() {
	while(/*_SFR_IO8*/(EECR) & (1<<EEPE)) {
	    // do nothing
	}
    }

    static void _write(uint8_t addr, uint8_t data) {
        /*_SFR_IO8*/(EECR) = (0<<EEPM0)|(0<<EEPM1);
        /*_SFR_IO8*/(EEAR) = addr;
        /*_SFR_IO8*/(EEDR) = data;
        /*_SFR_IO8*/(EECR) |= (1<<EEMPE);
        /*_SFR_IO8*/(EECR) |= (1<<EEPE);
    }

    static uint8_t _read(uint8_t addr) {
        /*_SFR_IO8*/(EEAR) = addr;
        /*_SFR_IO8*/(EECR) |= (1<<EERE);

        return /*_SFR_IO8*/(EEDR);
    }
};

#endif

