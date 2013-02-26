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
#include <stdlib.h>

#include "avr-tino.h"

#include "avr-tino/HD44780.h"
#include "avr-tino/delay.h"

#include "avr-tino/TWI.h"

int main() {
    uint8_t n = 0;
    static const  char* tag = "0123456789";

    MCU::USART::begin(9600);
    MCU::USART::print("\n\r\n\r" __FILE__ "\n\r");

    while(1) {
	MCU::USART::print("\n\r");
	MCU::USART::send((uint8_t)tag[n++ % 10]);

	bool result;
#if 1
	result = MCU::TWI::write(1,(uint8_t*)"A",0x27<<1);
	if (result) {
	    MCU::USART::print("WRITE(OK) ");
	}

	delay(50);
#endif
	int16_t word;
	result = MCU::TWI::read(2, (uint8_t*)&word, 0x27<<1);
	if (result) {
	    MCU::USART::print("READ(OK) ");
	}
	MCU::USART::print(word, 2);
	MCU::USART::print("  ");
	MCU::USART::print(word);

	#if 0
	bool result;
	
	result = MCU::TWI::start();
	MCU::USART::print(" START");

	if (result) {
	    MCU::USART::print("(OK)");
	}

	MCU::USART::print(" STOP");
	MCU::TWI::wait();
	result = MCU::TWI::stop();
	if (result) {
	    MCU::USART::print("(OK) ");
	}
	#endif
	delay(100);
	TWCR = 0; // XXX why is this necessary?
	delay(1000);
    }
    return 0;
}

