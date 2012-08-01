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
#if !defined AVR_TINO_1WIRE_H
#define AVR_TINO_1WIRE_H

/*
    Dallas Semiconductors 1-wire software implementation
    based on Atmel AVR318pplication Note AVR318
*/

template<class Port>
class Software1Wire {
    public:
    static void init(uint8_t mask);

    static void write(uint8_t mask, uint8_t len, const void* data);
    static void write(uint8_t mask, uint8_t data) {
	write(mask, 1, &data);
    }	

    static uint8_t read(uint8_t mask);
    static void read(uint8_t mask, uint8_t len, void* data);

    // timing delays (in µs) as per Dallas Semiconductors AN126
    static const uint16_t DELAY_A   = 6;
    static const uint16_t DELAY_B   = 64;
    static const uint16_t DELAY_C   = 60;
    static const uint16_t DELAY_D   = 10;
    static const uint16_t DELAY_E   = 9;
    static const uint16_t DELAY_F   = 55;
    static const uint16_t DELAY_G   = 0;
    static const uint16_t DELAY_H   = 480;
    static const uint16_t DELAY_I   = 70;
    static const uint16_t DELAY_J   = 410;

    private:
    static void writeBit1(uint8_t mask);
    static void writeBit0(uint8_t mask);
    static uint8_t readBit(uint8_t mask);
    static uint8_t detectPresence(uint8_t mask);
};

#endif
