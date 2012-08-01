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

template<class SPI, pin_t cs>
MCP2515<SPI,cs>::MCP2515() 
{
    pinToOutput(cs);
    pinToHigh(cs);
} 

template<class SPI, pin_t cs>
void MCP2515<SPI,cs>::reset() const {
    Command cmd(RESET);
}

template<class SPI, pin_t cs>
void MCP2515<SPI,cs>::write(uint8_t addr, uint8_t len, const void *data) const {
    Command	cmd(WRITE);

    cmd.write(addr);
    cmd.write(len, data);
}

template<class SPI, pin_t cs>
void MCP2515<SPI,cs>::write(uint8_t addr, uint8_t data) const {
    write(addr, 1, &data);
}

template<class SPI, pin_t cs>
void MCP2515<SPI,cs>::bitModify(uint8_t addr, 
			uint8_t mask, uint8_t data) const {
    Command cmd(BIT_MODIFY);

    cmd.write(addr);        
    cmd.write(mask);        
    cmd.write(data);        
}

template<class SPI, pin_t cs>
uint8_t MCP2515<SPI,cs>::read(uint8_t addr) const {
    Command cmd(READ);

    cmd.write(addr);        
    return cmd.read();
}

template<class SPI, pin_t cs>
void MCP2515<SPI,cs>::setPrescaler(uint8_t prescaler) const {
    bitModify(CANCTRL, 0x03, prescaler);
}

template<class SPI, pin_t cs>
void MCP2515<SPI,cs>::setMode(mode_t mode) const {
    bitModify(CANCTRL,
		_BV(REQOP2) | _BV(REQOP1) | _BV(REQOP0),
		mode << REQOP0);
}

template<class SPI, pin_t cs>
void  MCP2515<SPI,cs>::setTransmitBuffer(txb_t tx_base,
                            uint16_t sid,
                            uint16_t eid,
                            uint8_t len,
                            const void *data) const
{
    // adjust len
    if (len > 8) len = 8;

    Command	cmd(WRITE);

    cmd.write(tx_base & 0xF0);
    cmd.write(0); // reset TXBnCTRL
    cmd.write((uint8_t)(sid >> 8));
    cmd.write((uint8_t)(sid));
    cmd.write((uint8_t)(eid >> 8));
    cmd.write((uint8_t)(eid));
    cmd.write((uint8_t)(len));
    cmd.write(len, data);
}

template<class SPI, pin_t cs>
void MCP2515<SPI,cs>::doTransmitBuffer(uint8_t buffer_set) const {
    Command	cmd(RTS | (buffer_set & 0x0F));
}
