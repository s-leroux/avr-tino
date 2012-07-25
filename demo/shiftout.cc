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

/*
    This program demonstrate the use of
    'shiftOut' to send a byte
*/
int main() {
    const pin_t clk_pin	    = PIN_PD0;
    const pin_t data_pin    = PIN_PD1;
    pinMode(clk_pin, OUTPUT);
    pinMode(data_pin, OUTPUT);


    while(1) {
	digitalWrite(clk_pin, LOW);
	shiftOut(data_pin, clk_pin, MSBFIRST, "Hello", 5);
	delay(50);

	digitalWrite(clk_pin, LOW);
	shiftOut(data_pin, clk_pin, LSBFIRST, "Hello", 5);
	delay(500);
    }
    return 0;
}
