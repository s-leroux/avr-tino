/*
  Copyright (c) 2012-2013 Sylvain Leroux <sylvain@chicoree.fr>
  
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
#if !defined AVR_TINO_TINY13_H
#define AVR_TINO_TINY13_H

#include "avr-tino/pin.h"

typedef Port<0x16, 0x17, 0x18>	    PortB;

typedef PortB                       SPIPort;
static const uint8_t MOSI   = 0x01;
static const uint8_t MISO   = 0x02;
static const uint8_t SCK    = 0x04;



#endif

