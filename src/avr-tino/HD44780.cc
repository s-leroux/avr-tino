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
    delay(1); // excessive !
    pinToLow(E);
    delay(1); // excessive !

    if (!highOnly) {
	pin_to_output(DataBase) = (actualValue & ~dataMask) | lDataVal;
	pinToHigh(E);
	delay(1); // excessive !
	pinToLow(E);
	delay(1); // excessive !
    }
}

template<pin_t DataBase, pin_t RS, pin_t E>
void Interface4Bits<DataBase, RS, E>::init(uint8_t function_set) {
#if 0
    // Set driver to 4bits mode
    write(0x20, true); // XXX should be a constant !
    write(0x20 /* | function_set */); // XXX BUG That does not accept
				    // anything but 0x20 !!
#else
    write(0x33); // XXX This should use constants ?!?
    write(0x32);
    write(0x20 | function_set);
#endif
}

template<class Interface>
HD44780<Interface>::HD44780() 
{
    Instruction msg;

    msg.init(TWO_LINES);
    msg.setDisplayControl(0);
    msg.clear();
} 

template<class Interface>
void HD44780<Interface>::move(uint8_t x, uint8_t y) const {
    Instruction msg;

    msg.move(x,y);
}

template<class Interface>
void HD44780<Interface>::clear() const {
    Instruction msg;

    msg.clear();
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
void HD44780<Interface>::print(const char* str) const { 
    DataMessage msg;
   
    char c;
    while((c = *str++) != 0)
	msg.write(c);
}

template<class Interface>
void HD44780<Interface>::Instruction::init(uint8_t function_set) const {
    Interface::init(function_set);
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
    Interface::write(SET_DDRAM_ADDR | ( (x + 0x40*y) & 0xEF));
}

template<class Interface>
void HD44780<Interface>::Instruction::clear() const {
    Interface::write(CLEAR_DISPLAY);
}

template<class Interface>
void HD44780<Interface>::DataMessage::write(char c) const {
    Interface::write((uint8_t) c );
}

