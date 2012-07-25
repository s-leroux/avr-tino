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
#include "avr-tino/MCP2515.h"

#include "avr-tino/pin.h"
#include "avr-tino/SPI.h"

template<class SPI, pin_t cs, pin_t pin_reset>
MCP2515<SPI,cs,pin_reset>::MCP2515() 
{
    pinToOutput(cs);
    pinToHigh(cs);
} 

template<class SPI, pin_t cs, pin_t pin_reset>
void MCP2515<SPI,cs,pin_reset>::reset() const {
    pinToOutput(pin_reset);
    
    pinToLow(pin_reset);
    pinToHigh(pin_reset);
}

template<class SPI, pin_t cs, pin_t pin_reset>
void MCP2515<SPI,cs,pin_reset>::write(uint8_t addr, uint8_t len, const void *data) const {
    Command	cmd(WRITE);

    cmd.write(addr);
    cmd.write(len, data);
}

template<class SPI, pin_t cs, pin_t pin_reset>
void MCP2515<SPI,cs,pin_reset>::write(uint8_t addr, uint8_t data) const {
    write(addr, 1, &data);
}

template<class SPI, pin_t cs, pin_t pin_reset>
void MCP2515<SPI,cs,pin_reset>::bitModify(uint8_t addr, 
			uint8_t mask, uint8_t data) const {
    Command cmd(BIT_MODIFY);

    cmd.write(addr);        
    cmd.write(mask);        
    cmd.write(data);        
}

template<class SPI, pin_t cs, pin_t pin_reset>
uint8_t MCP2515<SPI,cs,pin_reset>::read(uint8_t addr) const {
    Command cmd(READ);

    cmd.write(addr);        
    return cmd.read();
}

template<class SPI, pin_t cs, pin_t pin_reset>
void MCP2515<SPI,cs,pin_reset>::setPrescaler(uint8_t prescaler) const {
    bitModify(CANCTRL, 0x03, prescaler);
}

template<class SPI, pin_t cs, pin_t pin_reset>
void MCP2515<SPI,cs,pin_reset>::setMode(mode_t mode) const {
    bitModify(CANCTRL,
		_BV(REQOP2) | _BV(REQOP1) | _BV(REQOP0),
		mode << REQOP0);
}

template<class SPI, pin_t cs, pin_t pin_reset>
void  MCP2515<SPI,cs,pin_reset>::setTransmitBuffer(txb_t buffer,
                            uint16_t sid,
                            uint16_t eid,
                            uint8_t len,
                            const void *data) const
{
    // adjust len
    if (len > 8) len = 8;

    Command	cmd(WRITE);

    cmd.write(buffer+1);
    cmd.write((uint8_t)(sid >> 8));
    cmd.write((uint8_t)(sid));
    cmd.write((uint8_t)(eid >> 8));
    cmd.write((uint8_t)(eid));
    cmd.write((uint8_t)(len));
    cmd.write(len, data);
}
