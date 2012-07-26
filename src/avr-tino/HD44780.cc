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
Interface4Bits<DataBase, RS, E>::Message::Message(bool isData) {
    const uint8_t dataMask = 0x0F << pin_to_bit(DataBase);
    pin_to_mode(DataBase) |= dataMask;
    pin_to_output(DataBase) &= (0xFF ^ dataMask);

    pinToOutput(RS);
    digitalWrite(RS, (isData) ? HIGH: LOW);
    pinToOutput(E);
    pinToLow(E);
}

template<pin_t DataBase, pin_t RS, pin_t E>
void Interface4Bits<DataBase, RS, E>::Message::write(uint8_t data, bool highOnly) {
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
    Message msg(false); 
    msg.write(0x20, true);

    msg.write(0x20);
    msg.write(0x0E);
    msg.write(0x8A);
}

template<class Interface>
HD44780<Interface>::HD44780() 
{
    Interface::init();


} 


