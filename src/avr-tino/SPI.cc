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

template <class SPI>
void SPIMaster<SPI>::begin() {
    SPI::MISO::toInput(); // Stupid naming scheme for attn2313 where 
                           // miSO is *always* output...
    SPI::SCK::toOutput();
    SPI::MOSI::toOutput(); // Stupid naming scheme for attn2313 where 
                           // moSI is *always* input...

    _SFR_IO8(SPI::CR) = _BV(SPE) | _BV(MSTR) | _BV(SPR0); /* clk /16 */
    _SFR_IO8(SPI::SR) |= _BV(SPI2X); /* clk 2/16 */
}

template <class SPI>
uint8_t SPIMaster<SPI>::transfert(uint8_t byte) {
    _SFR_IO8(SPI::DR) = byte;

    _SFR_IO8(SPI::SR) = _BV(SPI::IF);

    do {
	if (SPI::CRFLAGS) { _SFR_IO8(SPI::CR) = SPI::CRFLAGS; }
	//_BV(USIWM0) | _BV(USITC) | _BV(USICS1) | _BV(USICLK) ;
    } while (! (_SFR_IO8(SPI::SR) & _BV(SPI::IF)) );

    return _SFR_IO8(SPI::DR);
}


