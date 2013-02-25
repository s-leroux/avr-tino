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
#include "avr-tino/pin.h"
#include "avr-tino/SPI.h"

template <uint8_t CR, uint8_t SR, uint8_t DR, uint8_t IF, uint8_t CRFLAGS>
void SPIMaster<CR,SR,DR,IF,CRFLAGS>::begin() {
    pinToOutput(MISO); // Stupid naming scheme for attn2313 where miSO is *always* output...
    pinToOutput(SCK);
    pinToInput(MOSI);  // Stupid naming scheme for attn2313 where moSI is *always* input...
}

template <uint8_t CR, uint8_t SR, uint8_t DR, uint8_t IF, uint8_t CRFLAGS>
uint8_t SPIMaster<CR,SR,DR,IF,CRFLAGS>::transfert(uint8_t byte) {
    _SFR_IO8(DR) = byte;

    _SFR_IO8(SR) = _BV(IF);

    do {
	if (CRFLAGS) { _SFR_IO8(CR) = CRFLAGS; }
	//_BV(USIWM0) | _BV(USITC) | _BV(USICS1) | _BV(USICLK) ;
    } while (! (_SFR_IO8(SR) & _BV(IF)) );

    return _SFR_IO8(DR);
}


