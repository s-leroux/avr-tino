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

typedef Pin<PortB,0>  PB0_PIN;
typedef Pin<PortB,1>  PB1_PIN;
typedef Pin<PortB,2>  PB2_PIN;
typedef Pin<PortB,3>  PB3_PIN;
typedef Pin<PortB,4>  PB4_PIN;
typedef Pin<PortB,5>  PB5_PIN;
typedef Pin<PortB,6>  PB6_PIN;
typedef Pin<PortB,7>  PB7_PIN;

typedef PortB                       SPIPort;
typedef PB0_PIN MOSI_PIN;
typedef PB1_PIN MISO_PIN;
typedef PB2_PIN SCK_PIN;

//
// ADC support
//
enum /* ADMUX constants */ {
    ADC0                = 0x00,
    ADC1                = 0x01,
    ADC2                = 0x02,
    ADC3                = 0x03,
    ADCLeftAdjustResult = 1 << ADLAR,
    ADCInternalVoltageReference = 1 << REFS0,
};


#endif

