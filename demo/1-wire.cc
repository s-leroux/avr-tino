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
#include "avr-tino/printer.h"
#include "avr-tino/delay.h"

#include "avr-tino/1-wire.h"

static const pin_t LCD_E = PIN_PB5;
static const pin_t LCD_RS = PIN_PB4;
static const pin_t LCD_DB7 = PIN_PB3;
static const pin_t LCD_DB6 = PIN_PB2;
static const pin_t LCD_DB5 = PIN_PB1;
static const pin_t LCD_DB4 = PIN_PB0;

int main() {
    typedef Software1Wire<PortD>   Interface;

//    Interface::init(0x01);

    HD44780<Interface4Bits<LCD_DB4, LCD_RS, LCD_E> >    lcd;
    lcd.display();

    while(1) {
	uint8_t	id[8] = {0,0,0,0,0,0,0,0}; // 64-bits id
	uint8_t deviation = 0;

	Interface::detectPresence(_BV(0));
	//Interface::skipROM(_BV(0));
	Interface::readROM(_BV(0), id); {
	//while( (deviation = Interface::search(id, _BV(0), deviation)) != 0) {
	//    lcd.move(1,1);
	//    print(lcd, deviation);

	    for(uint8_t i = 0; i < 8; ++i) {
		char buffer[10];

		itoa(0x0000 | id[i], buffer, 16);
		if (buffer[1] == 0) {
		    buffer[2] = 0;
		    buffer[1] = buffer[0];
		    buffer[0] = '0';
		}
		lcd.move(2*i,0);
		print(lcd, buffer);
	    }
	    delay(1000);
	}

	lcd.move(10,1);
	print(lcd, "X");
    }
    return 0;
}

