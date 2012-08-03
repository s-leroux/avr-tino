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
#if !defined AVR_TINO_DS18S20_H
#define AVR_TINO_DS18S20_H

/*
    Support for Maxim DS18S20 -- Programmable Resolution 1-Wire Digital Thermometer
*/

template<class OW>
class DS18S20 {
    public:
    struct __attribute__ ((__packed__)) Scratchpad {
	int16_t	temperature;
	uint8_t	th;
	uint8_t	tl;
	uint8_t	configuration;
	uint8_t	reserved[3];
	uint8_t	crc;
    };

    /**
	Start a convertion
    */
    static void convert(uint8_t pins);
    static void readScratchpad(uint8_t pin, Scratchpad* scratchpad);

    static int16_t	readTemperature(uint8_t pin);

    enum __attribute__ ((__packed__)) {
	CONVERT_T       = 0x44,
	READ_SCRATCHPAD	= 0xBE,
    };
};

#endif
