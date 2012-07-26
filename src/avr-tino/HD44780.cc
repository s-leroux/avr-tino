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
    write(0x20, true);

    write(0x20);
    write(0x0E);
    write(0x07);
}

template<class Interface>
HD44780<Interface>::HD44780() 
{
    Message msg(INSTRUCTION_MESSAGE);

    msg.init();
    msg.move(0,0);
} 

template<class Interface>
void HD44780<Interface>::move(uint8_t x, uint8_t y) const {
    Message msg(INSTRUCTION_MESSAGE);

    msg.move(x,y);
}

template<class Interface>
void HD44780<Interface>::print(char c) const { 
    Message msg(DATA_MESSAGE);

    msg.write(c);
}

template<class Interface>
void HD44780<Interface>::Message::init() const {
    Interface::init();
}

template<class Interface>
void HD44780<Interface>::Message::move(uint8_t x, uint8_t y) const {
    Interface::write(0x80 | ( (x + 40*y) & 0x3F));
}

template<class Interface>
void HD44780<Interface>::Message::write(char c) const {
    Interface::write((uint8_t) c );
}

