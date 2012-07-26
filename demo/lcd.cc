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
#include "avr-tino.h"
#include "avr-tino/delay.h"

#include <string.h>

static const pin_t LCD_E = PIN_PB5;
static const pin_t LCD_RS = PIN_PB4;
static const pin_t LCD_DB7 = PIN_PB3;
static const pin_t LCD_DB6 = PIN_PB2;
static const pin_t LCD_DB5 = PIN_PB1;
static const pin_t LCD_DB4 = PIN_PB0;

int main() {
    DDRB = 0x3F;

    // set to 4bit mode
    PORTB = 0x02;
    pinToHigh(LCD_E);
    delay(10);
    pinToLow(LCD_E);
    delay(40);

    delay(1000);

    // 4bit mode + 1 line
    PORTB = 0x02;
    pinToHigh(LCD_E);
    delay(10);
    pinToLow(LCD_E);
    delay(40);

    PORTB = 0x00;
    pinToHigh(LCD_E);
    delay(10);
    pinToLow(LCD_E);
    delay(40);

    delay(1000);

    // display on
    PORTB = 0x00;
    pinToHigh(LCD_E);
    delay(10);
    pinToLow(LCD_E);
    delay(40);

    PORTB = 0x0E;
    pinToHigh(LCD_E);
    delay(10);
    pinToLow(LCD_E);
    delay(40);

    delay(1000);

    // move to pos 10
    PORTB = 0x08;
    pinToHigh(LCD_E);
    delay(10);
    pinToLow(LCD_E);
    delay(40);

    PORTB = 0x0A;
    pinToHigh(LCD_E);
    delay(10);
    pinToLow(LCD_E);
    delay(40);

    delay(1000);

    // increase address
    PORTB = 0x00;
    pinToHigh(LCD_E);
    delay(10);
    pinToLow(LCD_E);
    delay(40);

    PORTB = 0x07;
    pinToHigh(LCD_E);
    delay(10);
    pinToLow(LCD_E);
    delay(40);

    delay(1000);
    // write "W"

    while(1) {
	const char* message = "Hello world      ";
	for(uint8_t i = 0; i < strlen(message); ++i) {
	    pinToHigh(LCD_RS);
	    delay(10);

	    PORTB = 0x30 | (message[i] >> 4);
	    pinToHigh(LCD_E);
	    delay(10);
	    pinToLow(LCD_E);
	    delay(40);

	    PORTB = 0x30 | (message[i] & 0x0F);
	    pinToHigh(LCD_E);
	    delay(10);
	    pinToLow(LCD_E);
	    delay(40);

	    pinToLow(LCD_RS);
	    delay(10);
	    delay(500);
	}
    }
    return 0;
}
