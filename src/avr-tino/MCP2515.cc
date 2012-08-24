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
void MCP2515<SPI,cs>::setOperationMode(reqop_t mode) const {
    update(CANCTRL, CANCTRL::REQOP, mode);
}

template<class SPI, pin_t cs>
void MCP2515<SPI,cs>::write(REG r, uint8_t len, const void *data) const {
    Command	cmd(WRITE);

    cmd.write(r);
    cmd.write(len, data);
}

template<class SPI, pin_t cs>
void MCP2515<SPI,cs>::write(REG r, uint8_t data) const {
    write(r, 1, &data);
}

template<class SPI, pin_t cs>
void MCP2515<SPI,cs>::update(REG r, uint8_t mask, uint8_t data) {
    Command cmd(BIT_MODIFY);

    cmd.write(r);        
    cmd.write(mask);        
    cmd.write(data);        
}

template<class SPI, pin_t cs>
void MCP2515<SPI,cs>::read(REG r, uint8_t len, void * buffer) {
    Command cmd(READ);

    cmd.read(len, buffer);
}

template<class SPI, pin_t cs>
uint8_t MCP2515<SPI,cs>::read(REG r) {
    Command cmd(READ);

    cmd.write(r);        
    return cmd.read();
}

template<class SPI, pin_t cs>
void MCP2515<SPI,cs>::setPrescaler(uint8_t prescaler) const {
    update(CANCTRL, 0x03, prescaler);
}

template<class SPI, pin_t cs>
void  MCP2515<SPI,cs>::loadTX(uint8_t load_tx_location,
                            uint16_t sid,
                            uint16_t eid,
                            uint8_t len,
                            const void *data)
{
    // adjust len
    if (len > 8) len = 8;

    Command	cmd(WRITE);

    cmd.write(load_tx_location);
    cmd.write(0); // reset TXBnCTRL
    cmd.write((uint8_t)(sid >> 8));
    cmd.write((uint8_t)(sid));
    cmd.write((uint8_t)(eid >> 8));
    cmd.write((uint8_t)(eid));
    cmd.write((uint8_t)(len));
    cmd.write(len, data);
}

template<class SPI, pin_t cs>
void MCP2515<SPI,cs>::doTransmitBuffer(txb_rts_t buffer_set) {
    Command	cmd((instr)(RTS | buffer_set));
}

template<class SPI, pin_t cs>
typename MCP2515<SPI,cs>::RXStatus MCP2515<SPI,cs>::readRXStatus() {
    Command	cmd(RX_STATUS);
    return cmd.read();
}

