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
#include "avr-tino/DS18B20.h"

static const pin_t LCD_E = PIN_PB5;
static const pin_t LCD_RS = PIN_PB4;
static const pin_t LCD_DB7 = PIN_PB3;
static const pin_t LCD_DB6 = PIN_PB2;
static const pin_t LCD_DB5 = PIN_PB1;
static const pin_t LCD_DB4 = PIN_PB0;

int main() {
    typedef Software1Wire<PortD>    Bus;
    typedef DS18B20<Bus>	    Sensor;

    Bus::init(_BV(0));

//    Interface::init(0x01);

    HD44780<Interface4Bits<LCD_DB4, LCD_RS, LCD_E> >    lcd;
    lcd.display();

    while(1) {
	Sensor::Scratchpad  scratchpad;

	Bus::detectPresence(_BV(0));
	Bus::skipROM(_BV(0));

	Sensor::convert(_BV(0));
	delay(1000);

	Bus::detectPresence(_BV(0));
	Bus::skipROM(_BV(0));
	memset(&scratchpad, 0x77, sizeof(scratchpad));
	Sensor::readScratchpad(_BV(0), &scratchpad);

	for(uint8_t byte = 0; byte < sizeof(scratchpad); ++byte) {
	    char    buffer[] = { '#', 'X', 'X', 0};
	    itoa(((uint8_t*)&scratchpad)[byte], buffer+1, 16);

	    lcd.move(2*(byte % 8), byte / 8);
	    print(lcd, buffer + ((buffer[2] == 0) ? 0 : 1));
	}
	int16_t temp = scratchpad.temperature;

	char buffer[10] = { '0','0','0','0',' ',' ',' ',' ',' ',0};
	itoa(temp / 16, buffer+4, 10);
	lcd.move(6,1);
	print(lcd, buffer+strlen(buffer+4));

	delay(500);
    }
    return 0;
}

