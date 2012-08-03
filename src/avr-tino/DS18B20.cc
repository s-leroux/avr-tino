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
#include "avr-tino/DS18B20.h"


template<class OW>
void DS18B20<OW>::convert(uint8_t pins) {
    OW::write(pins, CONVERT_T);
}

template<class OW>
void DS18B20<OW>::readScratchpad(uint8_t pin, Scratchpad* scratchpad) {
    OW::write(pin, READ_SCRATCHPAD);
    OW::read(pin, sizeof(scratchpad), &scratchpad);
}

template<class OW>
int16_t DS18B20<OW>::readTemperature(uint8_t pin) {
    convert(pin);
    Scratchpad	scratchpad;
    readScratchpad(pin, &scratchpad);

    return scratchpad.temperature;
    OW::write(pin, READ_SCRATCHPAD);
    OW::read(pin, sizeof(scratchpad), &scratchpad);
}


