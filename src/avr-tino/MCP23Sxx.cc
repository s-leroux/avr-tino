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
#include "avr-tino/MCP23Sxx.h"

#include "avr-tino/pin.h"
#include "avr-tino/SPI.h"

template<class SPI, pin_t cs>
MCP23Sxx<SPI,cs>::MCP23Sxx() 
{
    pinToOutput(cs);
    pinToHigh(cs);
} 

template<class SPI, pin_t cs>
void MCP23Sxx<SPI,cs>::reset() const {
    // Command cmd(RESET);
}

template<class SPI, pin_t cs>
void MCP23Sxx<SPI,cs>::setDirection(uint8_t addr, 
				uint8_t mask, uint8_t direction) const {
    Command	cmd(WRITE | ((addr & 0x3) << 1) );

    cmd.write(IODIR);
    cmd.write(direction); // XXX mask is not used
}

template<class SPI, pin_t cs>
void MCP23Sxx<SPI,cs>::setGPIO(uint8_t addr, 
				uint8_t mask, uint8_t state) const {
    Command	cmd(WRITE | ((addr & 0x3) << 1) );

    cmd.write(GPIO);
    cmd.write(state); // XXX mask is not used
}

template<class SPI, pin_t cs>
void MCP23Sxx<SPI,cs>::enableHardwareAddress(uint8_t addr) const {
    update(IOCON, addr, _BV(HAEN), _BV(HAEN));
}

template<class SPI, pin_t cs>
void MCP23Sxx<SPI,cs>::update(regs r, 
		    uint8_t addr, 
		    uint8_t mask, uint8_t value) const {
    if (mask != 0xFF) {
	Command cmd(READ | ((addr & 0x3) << 1) );

	cmd.write(r);
	uint8_t prev = cmd.read();
	value = prev ^ ((prev ^ value) & mask);

	if (prev == value)
	    mask = 0x00; // Don't change anything
    }
   
    if (mask != 0x00) {
	Command cmd(WRITE | ((addr & 0x3) << 1) );
	cmd.write(r);
	cmd.write(value);
    }
}
