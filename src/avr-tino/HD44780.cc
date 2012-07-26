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
#include "avr-tino/HD44780.h"

#include "avr-tino/pin.h"

template<pin_t DataBase, pin_t RS, pin_t E>
void Interface4Bits<DataBase, RS, E>::begin(message_t type) {
    const uint8_t dataMask = 0x0F << pin_to_bit(DataBase);
    pin_to_mode(DataBase) |= dataMask;
    pin_to_output(DataBase) &= (0xFF ^ dataMask);

    pinToOutput(RS);
    digitalWrite(RS, (type == DATA_MESSAGE) ? HIGH: LOW);
    pinToOutput(E);
    pinToLow(E);
}

template<pin_t DataBase, pin_t RS, pin_t E>
void Interface4Bits<DataBase, RS, E>::write(uint8_t data, bool highOnly) {
    const uint8_t dataMask = 0x0F << pin_to_bit(DataBase);
    const uint8_t hDataVal = (data >> 4) << pin_to_bit(DataBase);
    const uint8_t lDataVal = (data & 0x0F) << pin_to_bit(DataBase);

    const uint8_t actualValue = pin_to_output(DataBase);
    pin_to_output(DataBase) = (actualValue & ~dataMask) | hDataVal;
    pinToHigh(E);
    delay(40); // excessive !
    pinToLow(E);
    delay(10);

    if (!highOnly) {
	pin_to_output(DataBase) = (actualValue & ~dataMask) | lDataVal;
	pinToHigh(E);
	delay(40); // excessive !
	pinToLow(E);
	delay(10);
    }
}

template<pin_t DataBase, pin_t RS, pin_t E>
void Interface4Bits<DataBase, RS, E>::init() {
    // Set driver to 4bits mode
    write(0x20, true); // XXX should be a constant !

    write(0x20);
}

template<class Interface>
HD44780<Interface>::HD44780() 
{
    Instruction msg;

    msg.init();
    msg.move(0,0);
} 

template<class Interface>
void HD44780<Interface>::move(uint8_t x, uint8_t y) const {
    Instruction msg;

    msg.move(x,y);
}

template<class Interface>
void HD44780<Interface>::setEntryMode(entry_mode_t mode) const {
    Instruction msg;

    msg.setEntryMode(mode);
}

template<class Interface>
void HD44780<Interface>::setDisplayControl(display_control_t dc) const {
    Instruction msg;

    msg.setDisplayControl(dc);
}

template<class Interface>
void HD44780<Interface>::print(char c) const { 
    DataMessage msg;

    msg.write(c);
}

template<class Interface>
void HD44780<Interface>::Instruction::init() const {
    Interface::init();

    setDisplayControl(DISPLAY_ON | CURSOR_ON);
    setEntryMode(DISPLAY_SHIFT_LEFT);
}

template<class Interface>
void HD44780<Interface>::Instruction::setEntryMode(entry_mode_t mode) const {
    Interface::write(ENTRY_MODE_SET | mode);
}

template<class Interface>
void HD44780<Interface>::Instruction::setDisplayControl(display_control_t dc) const {
    Interface::write(DISPLAY_CONTROL | dc);
}

template<class Interface>
void HD44780<Interface>::Instruction::move(uint8_t x, uint8_t y) const {
    Interface::write(SET_DDRAM_ADDR | ( (x + 40*y) & 0x3F));
}

template<class Interface>
void HD44780<Interface>::DataMessage::write(char c) const {
    Interface::write((uint8_t) c );
}

