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

int main() {
    pinMode(PIN_PD0, OUTPUT);
    pinMode(PIN_PD1, OUTPUT);
    pinMode(PIN_PD3, INPUT);

    digitalWrite(PIN_PD1, LOW);
    digitalWrite(PIN_PD3, HIGH);

    while(1) {
	digitalWrite(PIN_PD0, digitalRead(PIN_PD3));
	delay(100);
    }
    return 0;
}
